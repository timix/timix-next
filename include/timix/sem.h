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
/** @file sem.h
 *  @brief Semaphore implementation.
 */

#ifndef _TIMIX_SEM_H_
#define _TIMIX_SEM_H_

#include <cond.h>
#include <mutex.h>

struct sem {
    /* @brief Condition variable used to signal waiting threads. */
    cond_t          sem_signal;

    /* @brief Mutex used to protect semaphore count variable. */
    mutex_t         sem_lock;

    /* @brief The Semaphores value. */
    int             sem_count;
};
typedef struct sem sem_t;

/**
 * @brief  Initializes a semaphore with a specified count
 *
 * Using a semaphore that has not been initialized or reinitializing a 
 * semaphore after it has been used is illegal and has undefined results.
 *
 * @param sem Semaphore to initialize
 * @param count Number to initialize the semaphore to
 * @return 0 on success or a negative number on error
 */
int sem_init(sem_t *sem, int count);

/**
 * @brief Decrement the semaphore value
 *
 * Blocks until it is legal for the thread to decrement.
 *
 * @param sem Semaphore to wait on
 * @return 0 on success, or -EINTR if unable to sleep.
 */
int sem_wait(sem_t *sem) MUST_CHECK;

/**
 * @brief Increment the semaphore value
 *
 * Increments the value and signals any threads waiting on the semaphore.
 *
 * @param sem Semaphore to signal
 */
void sem_signal(sem_t *sem);

/**
 * @brief Destroys the specified semaphore
 *
 * It is illegal to destory a semaphore if threads are waiting for it and
 * the usage of a destroyed semaphore is undefined.
 *
 * @param sem Semaphore to destory
 */
void sem_destroy(sem_t *sem);

#endif /* _TIMIX_SEM_H_ */
