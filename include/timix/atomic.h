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

#ifndef TIMIX_ATOMIC_H_
#define TIMIX_ATOMIC_H_

#define XCHG_SUCCESS    0
#define XCHG_FAILURE    1

/**
 * @brief Atomically swaps a value into a memory location.
 *
 * Given a memory location and a value, atomically swap val into mem and return
 * the old value of mem.
 *
 * @param mem A memory location to swap with.
 * @param val The value to swap in.
 * @return The old value at that location
 */
void *atomic_swap(void *mem, void *val);

/**
 * @brief Function to atomically compare and swap 32 bit values.
 *
 * Given a memory location, an old value and a new value, atomically swap the
 * new value into the memory location if its value is equal to oldval.
 *
 * @param mem The memory address to swap with.
 * @param newval The new value to place in "mem".
 * @param oldval The expected value to be in *mem.
 *
 * @return 0 if the xchg was successful, or non-zero if *mem != oldval and thus
 * no change was made to *mem.
 */
int atomic_swap_conditional(void *mem, void* newval, void* oldval);

/**
 * @brief Function to atomically compare and swap 8B values.
 *
 * Given a memory location, an old value and a new value, atomically swap the
 * new value into the memory location if its value is equal to oldval. Old and
 * new values are split into high and low, as this function checks 64 bits at
 * the specified memory address.
 *
 * @param mem The memory address to swap with.
 * @param newval_lo The lower 32 bits of the new value to place in "mem".
 * @param newval_hi The upper 32 bits of the new value to place in "mem".
 * @param oldval_lo The lower 32 bits of the expected value to be in *mem.
 * @param oldval_hi The upper 32 bits of the expected value to be in *mem.
 *
 * @return 0 if the xchg was successful, or non-zero if *mem != oldval and thus
 * no change was made to *mem.
 */
int atomic_swap8_conditional(void *mem, void* newval_lo, void* newval_hi,
                                        void* oldval_lo, void* oldval_hi);

/**
 * @brief Atomically add a value to a memory address.
 *
 * Given a memory location and an integer increment amount, atomically add the
 * integer to the value at the memory location and return the old value.
 *
 * @param mem The memory address to add to.
 * @param val The amount to add to *mem.
 * @return The old value of *mem, before the addition.
 */
int atomic_add(int *mem, int val);

#endif // TIMIX_ATOMIC_H_
