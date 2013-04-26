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
/** @file sem.c
 *  @brief Semaphore Implementation.
 *
 *  Building on top of mutexes and condition variables makes the implementation
 *  of a semaphore rather simple. Just use the mutex to protect the count value
 *  of the semaphore, and once it's at or below zero, we use the condition
 *  variable to allow threads to wait efficiently.
 *
 *  @author Tim Detwiler <tdetwile@andrew.cmu.edu>
 */


#include <diag.h>
#include <cond.h>
#include <error.h>
#include <mutex.h>
#include <sem.h>

#include <sim.h>
#define LOG_MODULE LOG_SYNC
#include <log.h>

int sem_init(sem_t *sem, int count)
{

    assert(sem != NULL);
    cond_init(&(sem->sem_signal));

    mutex_init(&(sem->sem_lock));

    sem->sem_count = count;

    return 0;
}

void sem_destroy(sem_t *sem)
{
    assert(sem != NULL);
    assert(sem->sem_count == 0);
    mutex_destroy(&(sem->sem_lock));
    cond_destroy(&(sem->sem_signal));
    sem->sem_count = 0;

    return;
}

int sem_wait(sem_t *sem)
{
    mutex_lock(&(sem->sem_lock));

    sem->sem_count--;
    if (sem->sem_count < 0) {
        if(cond_wait(&(sem->sem_signal), &(sem->sem_lock)) == -EINTR) {
            // Unable to sleep. Propagate error.
            sem->sem_count++;
            mutex_unlock(&(sem->sem_lock));
            log(LOG_INFO, "Sem unable to sleep!");
            return -EINTR;
        }
    }

    mutex_unlock(&(sem->sem_lock));
    return 0;
}

void sem_signal(sem_t *sem)
{
    mutex_lock(&(sem->sem_lock));

    sem->sem_count++;
    cond_signal(&(sem->sem_signal));

    mutex_unlock(&(sem->sem_lock));
    return;
}
