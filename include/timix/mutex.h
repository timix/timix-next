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

#ifndef TIMIX_MUTEX_H_
#define TIMIX_MUTEX_H_

#include <compiler.h>

/** @brief Indicates a mutex is unlocked.*/
#define MUTEX_OWNER_UNLOCKED    (NULL)
/** @brief Indicates thread doesn't own lock */
#define MUTEX_DONT_GOT_IT       (0)
/** @brief Indicates thread owns the lock */
#define MUTEX_GOT_IT            (1)

struct mutex {
    /**
     * @brief The owner of the thread that owns this lock or NULL if it is
     * unlocked.
     */
    void                *owner;
    // TODO: Move to tcb pointer when tcbs are merged into this codebase.
    //struct tcb          *owner;
};
typedef struct mutex mutex_t;

/**
 * @brief Attempts to acquire the lock.
 *
 * Uses atomic instructions to try to aquire the lock. If the lock was unlocked,
 * it will be locked. If the lock was already locked, the lock is left in its
 * previous state. It is up to the calling function to update the owner field
 * of the mutex, and thus this function isn't exposed to the user.
 *
 * @param lock The mutex to attempt to acquire.
 * @return 0 if the mutex is locked by some other thread, or 1 if the thread was
 * unlocked but now locked under our control.
 */
int mutex_trylock(mutex_t*) MUST_CHECK;

/**
 * @brief Initializes a mutex variable
 *
 * The effects of using a mutex that has not been initialized or of
 * reinitializing a mutex in use are undefined.
 *
 * @param mp Mutex to initialize
 * @return 0 on success, or a negative number on error
 */
void mutex_init(mutex_t *mp);

/**
 * @brief Destroys the specified mutex
 *
 * It is illegal to destory a mutex if it is locked.
 * Using a destroyed mutex is undefined.
 *
 * @param mp Mutex to destory
 */
void mutex_destroy(mutex_t *mp);

/**
 * @brief Locks a mutex
 *
 * This function will block until it has acquired the mutex
 *
 * @param mp Mutex to lock
 */
void mutex_lock(mutex_t *mp);

/**
 * @brief Unlocks the specified mutex
 *
 * It is illegal to unlock a mutex that is not locked. Behavior is undefined if an
 * application attempts to unlock a mutex it does not own.
 *
 * @param mp Mutex to unlock
 */
void mutex_unlock(mutex_t *mp);

/**
 * @brief Determines if a mutex is locked
 *
 * Does not change the state of the lock
 * @param mp Mutex to check
 * @return TRUE if the mutex is locked FALSE if it is not
 */
int mutex_is_locked(mutex_t *mp) MUST_CHECK;

#endif // TIMIX_MUTEX_H_
