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
//  @bug inten/intdis aren't what we want here, we want something similar to
//       "make/restore interruptable" state.
//

#include <arch/nios-macros.h>
#include <timix/atomic.h>

.globl atomic_swap
.globl atomic_swap_conditional
.globl atomic_swap8_conditional
.globl atomic_add


// void* atomic_swap(void* mem, void* val)
atomic_swap:
    intdis  r8, r9      // Disable interrupts
    ldw     r2, 0(r4)   // ret_val = *mem
    stw     r5, 0(r4)   // *mem = val
    inten   r8, r9      // Enable interrupts
    ret                 // return

// int atomic_swap_conditional(void *mem, void *newavl, void* oldval);
atomic_swap_conditional:
    intdis  r8, r9      // Disable interrupts
    ldw     r2, 0(r4)   // r2 = *mem
    bne     r2, r6, 1f  // if *mem != oldval then goto 1f
    stw     r5, 0(r4)   // *mem = val
    inten   r8, r9      // Enable interrupts
    movi    r2, XCHG_SUCCESS
    ret                 // return
// No Swap
1:  inten   r8, r9      // re-enable interrupts
    movi    r4, XCHG_FAILURE
    ret                 // return


// int atomic_swap8_conditional(void* mem, void* newlo, void* newhi,
//                                         void* oldlo, void* oldhi);
atomic_swap8_conditional:
    mov     r4, r0      // msg = NULL
    call    panic       // PANIC (unimplemented)

// int atomic_add(int* mem, int val);
atomic_add:
    intdis  r8, r9      // Disable interrupts
    ldw     r2, 0(r4)   // ret_val = *mem
    addi    r2, r2, 1
    stw     r2, 0(r4)   // *mem = *mem + 1
    inten   r8, r9      // Enable interrupts
    ret                 // return
