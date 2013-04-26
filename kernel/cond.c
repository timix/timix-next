//
//  Copyright(C) 2012, Tim Detwiler <timdetwiler@gmail.com>
//
//  This file is part of Timix.
//
//  Timix is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Timix is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Timix.  If not, see <http://www.gnu.org/licenses/>.
//

#include <stddef.h>
#include <timix/assert.h>
#include <timix/cond.h>
#include <timix/mutex.h>
#include <timix/variable_queue.h>

// Private functions
static inline void do_cond_signal_unsafe(cond_t *cv);
static void cond_q_push(cond_t *cv,cond_q_entry_t *ent, struct tcb *tcb);
static int  cond_q_pop(cond_t *cv, struct tcb **tcb);
static int  cond_q_remove(cond_t *cv, struct tcb *thread);

void cond_init(cond_t *cv)
{
    assert(cv != NULL);
    mutex_init(&cv->q_lock);

    Q_INIT_HEAD(&(cv->waiting));

    return;
}

void cond_destroy(cond_t *cv)
{
    assert(Q_GET_FRONT(&(cv->waiting)) == NULL);
    mutex_destroy(&cv->q_lock);

    return;
}


// TODO: pass in current thread when sched.h is merged.
static int do_cond_wait(cond_t *cv, mutex_t *mp, int opts)
{
    void *tcb;
    cond_q_entry_t* q = &(current_thread->cond_q_node);

    tcb = current_thread;

    Q_INIT_ELEM(q, entry);
    mutex_lock(&(tcb->cond_q_node.lock));
    mutex_lock(&(cv->q_lock));
    cond_q_push(cv, q, tcb);

    q->flags = 0;
    if (opts & COND_UNINTERRUPTABLE)
        q->flags |= COND_UNINTERRUPTABLE;
    q->cv = cv;
    mutex_unlock(&(tcb->cond_q_node.lock));

    // Atomic unlock and block
    sched_lock_scheduler();

    if (opts & COND_UNINTERRUPTABLE) {
        sched_force_set_blocked();
    } else {
        if (sched_set_blocked() == -EINTR) {
            // Rollback changes and propagate error.
            cond_q_pop(cv, NULL);
            mutex_unlock(&(cv->q_lock));
            sched_unlock_scheduler();
            return -EINTR;
        }
    }
    mutex_unlock(mp);
    mutex_unlock(&(cv->q_lock));
    sched_unlock_scheduler_and_yield();

    // We've been woken up when we get here.
    mutex_lock(&(tcb->cond_q_node.lock));
    tcb->cond_q_node.cv = NULL;   

    mutex_lock(mp);
    if ((q->flags & COND_INTERRUPTED)) {
        mutex_unlock(&(tcb->cond_q_node.lock));
        return -EINTR;
    }

    mutex_unlock(&(tcb->cond_q_node.lock));
    return 0;
}

int cond_wait(cond_t *cv, mutex_t *mp)
{
    return do_cond_wait(cv, mp, 0);
}

void cond_wait_uninterruptable(cond_t *cv, mutex_t *mp)
{
    assert(do_cond_wait(cv, mp, COND_UNINTERRUPTABLE) == 0);
    return;
}

void cond_signal(cond_t *cv)
{
    mutex_lock(&(cv->q_lock));
    do_cond_signal_unsafe(cv);
    mutex_unlock(&(cv->q_lock));
}

void cond_broadcast(cond_t *cv)
{
    mutex_lock(&(cv->q_lock));

    while (Q_GET_FRONT(&(cv->waiting)) != NULL)
        do_cond_signal_unsafe(cv);

    mutex_unlock(&(cv->q_lock));

    return;
}

int cond_interrupt(tcb_t* thread)
{
    int val;
    cond_t *cv;

    mutex_lock(&(thread->cond_q_node.lock));
    cv = thread->cond_q_node.cv;

    if (cv == NULL) {
        mutex_unlock(&(thread->cond_q_node.lock));
        return -EINVAL;
    }

    if ((thread->cond_q_node.flags & COND_UNINTERRUPTABLE)) {
        // Attempting to interrupt an uninterruptable thread.
        mutex_unlock(&(thread->cond_q_node.lock));
        return -EINVAL;
    } 

    mutex_lock(&(cv->q_lock));
    val = cond_q_remove(cv, thread);
    mutex_unlock(&(cv->q_lock));

    if (val == -ESRCH) {
        // Not sleeping
        mutex_unlock(&(thread->cond_q_node.lock));
        return -EINVAL;
    } else {
        assert(val == 0);
        thread->cond_q_node.flags |= COND_INTERRUPTED;
        sched_unblock_tcb(thread);
    }

    mutex_unlock(&(thread->cond_q_node.lock));
    return 0;
}

void cond_thread_init(tcb_t *thread)
{
    mutex_init(&(thread->cond_q_node.lock));
    thread->cond_q_node.cv = NULL;
    thread->cond_q_node.tcb = thread;
    thread->cond_q_node.flags = 0;
    Q_INIT_ELEM(&(thread->cond_q_node), entry);
}

////////////////////////////////////////////////////////////////////////////////
//    Internal functions                                                      //
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Does the condition variable signal operation.
 *
 * This is a helper function for both cond_signal and cond_broadcast, but with 
 * each calling function having its own locking semantics
 *
 * @param cv Condition variable to signal on
 */
static inline void do_cond_signal_unsafe(cond_t *cv)
{
    struct tcb *tcb;

    // Check if no one is waiting
    if(cond_q_pop(cv, &tcb) == COND_Q_EMPTY)
        return;

    log(LOG_VV, "Cond var signaling %p", tcb);

    // Otherwise, make them runnable
    sched_unblock_tcb(tcb);

    return;
}

////////////////////////////////////////////////////////////////////////////////
//    Queueing functions                                                      //
////////////////////////////////////////////////////////////////////////////////
/**
 * @brief Queues a thread on the condition variable.
 *
 * Adds a thread to the waiting queue.
 *
 * @param cv The condition variable to operate on.
 * @param tcb The tcb to queue.
 */
static void cond_q_push(cond_t *cv, cond_q_entry_t *q, struct tcb *tcb)
{
    q->tcb = tcb;

    Q_INSERT_FRONT(&(cv->waiting), q, entry);

    return;
}

/**
 * @brief Removes the oldest element from a wait queue.
 *
 * @param cv The condition variable to pop from.
 * @param tcb The popped tcb is written here.
 * @return COND_Q_OK on success, COND_Q_EMPTY if there are no waiting threads.
 */
static int cond_q_pop(cond_t *cv, struct tcb **tcb)
{
    cond_q_entry_t *node;

    // Return error on empty queue.
    node = Q_GET_TAIL(&(cv->waiting));
    if(node == NULL)
        return COND_Q_EMPTY;

    Q_REMOVE(&(cv->waiting), node, entry);

    Q_INIT_ELEM(node, entry);
    // Set the tcb of the signaled thread.
    if (tcb)
        *tcb = node->tcb;

    return COND_Q_OK;
}

/**
 * @brief Removes a thread waiting at an arbitrary location in the queue.
 *
 * Used for early wake up.
 *
 * @param cv The condition variable to remove from.
 * @param thread The thread to remove.
 */
static int cond_q_remove(cond_t *cv, struct tcb *thread)
{
    cond_q_entry_t* node, *thread_node = NULL;
    Q_FOREACH(node, &(cv->waiting), entry) {
        if(node->tcb == thread) { 
            thread_node = node;
            break;
        }
    }

    if (thread_node == NULL)
        return -ESRCH;

    assert(thread_node == &thread->cond_q_node);
    Q_REMOVE(&(cv->waiting), thread_node, entry);

    return 0;
}
