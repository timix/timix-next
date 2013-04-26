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
 * @file rwlock.h
 * @brief read-write locks ported into our kernel from p2
 */

#ifndef _TIMIX_RWLOCK_H_
#define _TIMIX_RWLOCK_H_

/** @brief Indicates a thread desires read mode access. */
#define RWLOCK_READ  0
/** @brief Indicates a thread desires write mode access. */
#define RWLOCK_WRITE 1

/** @brief Indicates an rwlock must not be interrupted. */
#define RWLOCK_UNINTERRUPTABLE  1

typedef struct rwlock_q_entry {
    /** @brief The access mode this thread desires. */
    int                         mode;

    /** @brief link to the other nodes in the list. */
    Q_NEW_LINK(rwlock_q_entry)  entry;
} rwlock_q_entry;

Q_NEW_HEAD(rwlock_q_t, rwlock_q_entry);

typedef struct rwlock {
    /** @brief Mutex protecting the internal state of the rwlock. */
    mutex_t             lock;

    /** @brief Threads waiting to access in read mode. */
    cond_t              readers;

    /** @brief Threads waiting to access in write mode. */
    cond_t              writers;

    /** @brief List of ALL threads waiting in the order they called lock. */
    rwlock_q_t          waiting;

    /** @brief The number of threads that _own_ the lock. */
    int                 count;

    /** @brief The mode the lock is in. Either read, write, or unlocked. */
    int                 mode;
} rwlock_t;

/**
 * @brief Initializes a rwlock
 *
 * The effects of using a rwlock that has not been initialized or of 
 * reinitializing a rwlock in use are undefined.
 *
 * @param lock Lock to initialize
 * @return 0 on success, or a negative number on error
 */
int rwlock_init(rwlock_t *lock);

/**
 * @brief Destroys the specified rwlock
 *
 * It is illegal to destory a rwlock if it is locked.
 * Using a destroyed rwlock is undefined.
 *
 * @param lock rwlock to destory
 */
void rwlock_destroy(rwlock_t *lock);

/**
 * @brief Acquires a rwlock in the specified mode
 *
 * This function will block until it has acquired the lock in the desired mode.
 *
 * @param lock Lock to acquire
 * @param type Mode to acquire the lock for
 * @return 0 on success, or -EINTR if the calling thread is interrupted from
 * its sleep.
 */
int rwlock_lock(rwlock_t *lock, int type) MUST_CHECK;

/**
 * @brief Uninterruptable lock.
 *
 * Used when its critical that the kernel acquire a lock without interruption.
 *
 * @see rwlock_lock
 * @param lock Lock to acquire.
 * @param type Mode to acquire the lock for.
 */
void rwlock_lock_uninterruptable(rwlock_t *lock, int type);

/**
 * @brief Unlocks the specified lock.
 *
 * This function removes the calling thread from the lock. Contrary to its name
 * it does not guarentee that the lock will become available as there may still
 * be other readers using it.
 *
 * @pre You own the lock in read or write mode
 * @param lock Lock to unlock
 */
void rwlock_unlock(rwlock_t *lock);

/**
 * @brief Downgrades access on a lock from write to read
 *
 * This function downgrades a lock from write to read to allow other threads
 * that are waiting to read into the critical section.
 *
 * @pre You own the lock in write mode
 * @param lock Lock to be downgraded
 */
void rwlock_downgrade(rwlock_t *lock);

/**
 * @brief Places tid/mode at the end of the queue
 *
 * Sets the tid and mode of the entry and adds the new entry to the queue.
 *
 * @param lock  Lock enqueue on
 * @param q     New entry to enqueue
 * @param mode  mode (read/write) of the element to be added
 */
void rwlock_q_push(rwlock_t *lock, rwlock_q_entry *q, int mode);

/**
 * @brief Peeks at the tid/mode at the top of the queue.
 *
 * If tid or mode are NULL, they will not be set allowing the caller to 
 * simply use the return value to determine if anyone is waiting.
 *
 * @param lock  [in]  Lock whose queue to peek at
 * @param mode  [out] requested mode of the thread at the head of the queue
 * @return 0 on success, or -1 if the queue is empty.
 */
int rwlock_q_peek(rwlock_t *lock, int *mode);

/**
 * @brief Removes node for tid specified from the queue. Does not have to be at
 * the head of the queue.
 *
 * @param lock  Lock whose queue we are removing from
 * @param thr The rwlock_q_entry to remove.
 * @return RWLOCK_Q_SUCCESS on success or RWLOCK_Q_NOT_FOUND if the tid does
 *          not exist.
 */
int rwlock_q_remove(rwlock_t *lock, rwlock_q_entry *tid);

#endif /* _TIMIX_RWLOCK_H_ */
