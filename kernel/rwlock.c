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
/** 
 * @file rwlock.c
 * @brief Read/Write lock implementation.
 */
#include <cond.h>
#include <diag.h>
#include <error.h>
#include <mutex.h>
#include <rwlock.h>
#include <sched.h>
#include <variable_queue.h>

#include <log.h>
#define LOG_MODULE LOG_SYNC

/** @brief Read/write lock is unlocked */
#define RWLOCK_UNLOCKED     (-1)
/** @brief Error code indicating the thread is not in the queue */
#define RWLOCK_Q_NOT_FOUND  (-1)
/** @brief Status code indicating success returned by queue functions */
#define RWLOCK_Q_SUCCESS    (0)
/** @brief Error code indicating the queue is empty */
#define RWLOCK_Q_EMPTY      (-1)

int rwlock_init(rwlock_t *lock)
{
    if(IS_NULL(lock)) {
        return -1;
    }
    lock->count = 0;
    Q_INIT_HEAD(&(lock->waiting));
    lock->mode = RWLOCK_UNLOCKED;

    cond_init(&(lock->readers));
    cond_init(&(lock->writers));
    mutex_init(&(lock->lock));

    return 0;
}

void rwlock_destroy(rwlock_t *lock)
{
    assert(lock->mode == RWLOCK_UNLOCKED);
    assert(lock->count == 0);
    cond_destroy(&(lock->readers));
    cond_destroy(&(lock->writers));
    mutex_destroy(&(lock->lock));
}

static inline int rwlock_wait(cond_t *cv, mutex_t *mp, int flags)
{
    if ((flags & RWLOCK_UNINTERRUPTABLE)) {
        cond_wait_uninterruptable(cv, mp);
        return 0;
    } else {
        return cond_wait(cv,mp);
    }
}

static int do_rwlock_lock(rwlock_t *lock, int type, int flags)
{
    rwlock_q_entry *q = &current_thread->rwlock_q_node;

    assert(type == RWLOCK_READ || type == RWLOCK_WRITE);

    mutex_lock(&(lock->lock));
    if (lock->count == 0 && lock->mode == RWLOCK_UNLOCKED) {
        lock->count++;
        lock->mode = type;
        mutex_unlock(&(lock->lock));
        return 0;
    }

    if (type == RWLOCK_READ) {
        // If someones waiting, or the lock is in "write" mode, enqueue.
        if (rwlock_q_peek(lock, NULL) == RWLOCK_Q_SUCCESS) {
            rwlock_q_push(lock, q, type);
            while (lock->mode == RWLOCK_WRITE) {
                int ret = rwlock_wait(&(lock->readers), &(lock->lock), flags);
                if (ret == -EINTR) {
                    assert(rwlock_q_remove(lock, q) == RWLOCK_Q_SUCCESS);
                    mutex_unlock(&(lock->lock));
                    return -EINTR;
                }
            }
            assert(rwlock_q_remove(lock, q) == RWLOCK_Q_SUCCESS);
        }

    } else if (type == RWLOCK_WRITE) {
        rwlock_q_push(lock, q, type);
        while(lock->count != 0) {
            if (rwlock_wait(&(lock->writers), &(lock->lock), flags) == -EINTR) {
                assert(rwlock_q_remove(lock, q) == 0);
                mutex_unlock(&(lock->lock));
                return -EINTR;
            }
        }
        assert(rwlock_q_remove(lock, q) == RWLOCK_Q_SUCCESS);
    }

    lock->mode = type;
    lock->count++;
    mutex_unlock(&(lock->lock));

    return 0;
}

int rwlock_lock(rwlock_t *lock, int type)
{
    return do_rwlock_lock(lock, type, 0);
}

void rwlock_lock_uninterruptable(rwlock_t *lock, int type)
{
    assert(do_rwlock_lock(lock, type, RWLOCK_UNINTERRUPTABLE) == 0);
    return;
}

void rwlock_unlock(rwlock_t *lock)
{
    int next_mode;

    mutex_lock(&(lock->lock));

    lock->count--;
    assert(lock->count >= 0);
    if (lock->count > 0) {
        mutex_unlock(&(lock->lock));
        return;
    }

    // Pick who to hand off lock to.
    if (rwlock_q_peek(lock, &next_mode) != RWLOCK_Q_SUCCESS) {
        // Nobody waiting
        lock->mode = RWLOCK_UNLOCKED;
        mutex_unlock(&(lock->lock));
        return;
    }

    // If read, broadcast to readers.
    if (next_mode == RWLOCK_READ) {
        lock->mode = RWLOCK_READ;
        cond_broadcast(&(lock->readers));
    } else if (next_mode == RWLOCK_WRITE) {
        lock->mode = RWLOCK_WRITE;
        cond_signal(&(lock->writers));
    }

    mutex_unlock(&(lock->lock));

    return;
}

void rwlock_downgrade(rwlock_t *lock)
{
    assert(lock->mode == RWLOCK_WRITE);
    assert(lock->count == 1);

    mutex_lock(&(lock->lock));
    lock->mode = RWLOCK_READ;
    cond_broadcast(&(lock->readers));
    mutex_unlock(&(lock->lock));

    return;
}

/******************************************************************************
 * rwlock_q_t functions
 *****************************************************************************/

void rwlock_q_push(rwlock_t *lock, rwlock_q_entry *q, int mode)
{
    q->mode = mode;

    Q_INIT_ELEM(q, entry);
    Q_INSERT_FRONT(&(lock->waiting), q, entry);

    return;
}

int rwlock_q_peek(rwlock_t *lock, int *mode)
{
    rwlock_q_entry *head = Q_GET_FRONT(&(lock->waiting));

    if (head == NULL)
        return RWLOCK_Q_EMPTY;

    if (!IS_NULL(mode))
        *mode = head->mode;

    return RWLOCK_Q_SUCCESS;
}

int rwlock_q_remove(rwlock_t *lock, rwlock_q_entry *thr)
{
    rwlock_q_entry *curr;

    Q_FOREACH(curr, &(lock->waiting), entry) {
        if (curr == thr) {
            Q_REMOVE(&(lock->waiting), curr, entry);
            return RWLOCK_Q_SUCCESS;
        }
    }

    // not found
    return RWLOCK_Q_NOT_FOUND;
}
