/*
  Copyright(C) 2012, Tim Detwiler <tdetwile@andrew.cmu.edu>
  Copyright(C) 2012, Tim Shields <tshield1@andrew.cmu.edu>

  This file is part of Timix.

  Timix is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Timix is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Timix.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file cond.h
 *  @brief This file defines the type for condition variables.
 */

#ifndef _TIMIX_COND_H_
#define _TIMIX_COND_H_


#include <mutex.h>
#include <variable_queue.h>

/** @brief Indicated an empty queue.*/
#define COND_Q_EMPTY        (-1)
/** @brief Indicates the desired element is not found in the queue.*/
#define COND_Q_NOT_ELEM     (-2)
/** @brief Indicates a condition queue operation was successful.*/
#define COND_Q_OK           (0)
/** @brief Indicates that at least one signaling event has occured.*/
#define COND_SIGNALING      (1)
/** @brief Indicates that no signaling events have occured.*/
#define COND_NOSIGNAL       (0)

/** @breif Indicates a sleeping thread has been interrupted. */
#define COND_INTERRUPTED    (1)

/** @brief Indicates a sleeping thread must not be interrupted. */
#define COND_UNINTERRUPTABLE  (2)
struct cond;

struct cond_q_entry {
    /** @brief Lock to guard contents of this structure. */
    mutex_t                     lock;

    /**
     * @brief TCB of the specified thread.
     */
    struct tcb*                 tcb;

    /** @brief Pointer to the condition variable waiting on. */
    struct cond*                cv;

    /** @brief Flags indicating state. */
    int                         flags;

    /**
     * @brief Pointers to next/prev element in the linked-list.
     */
    Q_NEW_LINK(cond_q_entry)    entry;
};
typedef struct cond_q_entry cond_q_entry_t;

Q_NEW_HEAD(cond_q_t, cond_q_entry);

typedef struct cond {
    /**
     * @brief Queue of threads that are currently waiting on this condition
     * variable.
     *
     * This queue is stored in reverse order, so this is a tail pointer. In
     * general this speeds up most of our accesses.
     */
    cond_q_t      waiting;


    /**
     * @brief Used to lock the queue when enqueuing and dequeuing threads
     *
     * Since semantics don't demand that the application call cond_signal or
     * cond_broadcast while the calling thread owns the condition variables
     * mutex, we use this lock to protect the queue from interleaved calls to
     * cond_signal and cond_broadcast.
     */
    mutex_t       q_lock;

} cond_t;

/* condition variable functions */
/**
 * @brief Initializes a condition variable
 *
 * The behavior of an application using a condition variable before
 * initializing it is undefined.
 *
 * @param cv Condition variable to initialize
 */
void cond_init( cond_t *cv );

/** 
 * @brief Destroys a condition variable
 *
 * It is illegal to destory a cv if threads are waiting on it
 * Using a destroyed cv is undefined.
 *
 * @param cv Condition variable to destroy
 */
void cond_destroy( cond_t *cv );

/**
 * @brief Waits on a condition variable
 *
 * Queues a thread in the condition variable and deschedules it until the
 * it is signaled. A thread must have a lock to wait on a condition variable.
 * The lock will be reacquired before cond_wait returns.
 *
 * @param cv Condition variable to wait on
 * @param mp Mutex to wait with
 * @return 0 on success, or -EINTR if the thread is unable to sleep.
 */
int  cond_wait( cond_t *cv, mutex_t *mp ) MUST_CHECK;

/**
 * @brief Uninterruptable version of cond wait.
 *
 * This allows the kernel to allow some limited blocking under certain
 * conditions, such as during vanishing.
 *
 * @see cond_wait
 * @param cv The condition variable to wait on.
 * @param mp The mutex that guards the condition.
 */
void cond_wait_uninterruptable(cond_t *cv, mutex_t *mp);

/**
 * @brief Signals a thread waiting on a condition variable
 *
 * Reschedules a single thread from the CVs queue.
 *
 * @param cv Condition variable to signal on
 */
void cond_signal( cond_t *cv );

/**
 * @brief Signals all threads waiting on a condition variable
 *
 * Reschedules all threads from a CVs queue.
 *
 * @param cv Condition variable to signal on
 */
void cond_broadcast( cond_t *cv );

/**
 * @brief Force a thread waiting on a thread to wake up.
 *
 * @param thread The waiting thread. The condition variable will be
 * identified using data from the tcb.
 */
int cond_interrupt(struct tcb* thread);

/**
 * @brief Initializes parts of a tcb that are used by condition variables.
 *
 * @param thread The tcb to initialize.
 */
void cond_thread_init(struct tcb* thread);

#endif /* _TIMIX_COND_H_ */
