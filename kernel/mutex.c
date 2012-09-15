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
#include <timix/atomic.h>
#include <timix/assert.h>
#include <timix/mutex.h>

void mutex_init(mutex_t *mp)
{
    assert(mp != NULL);

    mp->owner = MUTEX_OWNER_UNLOCKED;
}

void mutex_destroy(mutex_t *mp)
{
    assert(mp->owner == MUTEX_OWNER_UNLOCKED);

    return;
}

int mutex_trylock(mutex_t *mp)
{
    // TODO: Swap in TCB pointer when proc.h is integrated.
    int swap = atomic_swap_conditional(&(mp->owner), 
                                       (void*)1,
                                       MUTEX_OWNER_UNLOCKED);
                                       
    // If state is already locked, we didnt' get the lock
    if(swap == XCHG_SUCCESS)
        return MUTEX_GOT_IT;
    return MUTEX_DONT_GOT_IT;
}

void mutex_lock(mutex_t *mp)
{
    // Try to grab the lock.
    while (mutex_trylock(mp) == MUTEX_DONT_GOT_IT) {
        // yield on up, spin on smp
#if 0
        sched_yield_to_tcb(mp->owner);
#endif
    }

    return;
}

void mutex_unlock(mutex_t *mp)
{
    mp->owner = MUTEX_OWNER_UNLOCKED;
    return;
}

int mutex_is_locked(mutex_t *mp)
{
    return (mp->owner != MUTEX_OWNER_UNLOCKED);
}
