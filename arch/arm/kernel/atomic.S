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
//  @see timix/atomic.h for function documentation.
//

.globl atomic_swap
.globl atomic_swap_conditional
.globl atomic_swap8_conditional
.globl atomic_add


// void* atomic_swap(void* mem, void* val)
atomic_swap:
1:  ldrex r2, [r0]      // r2 = *mem
    strex r3, r1, [r0]  // *mem = val, r4 == 0 if exclusive
    cmpeq r3, #0        // did strex succeed?
    bne 1b              // if strex failed, try again
    mov r0, r2          // put old value in return register
    bx lr               // return

// int atomic_swap_conditional(void *mem, void *newavl, void* oldval);
atomic_swap_conditional:
1:  ldrex r3, [r0]      // r3 = *mem
    cmpeq r2, r3        // r2 == r3?
    beq   1f            // 1f on val != oldval
    strex r3, r1, [r0]  // *mem = val, r4 == 0 if exclusive
    cmp   r3, #0        // did strex succeed?
    bne   1b            // if strex failed, try again
    mov   r0, #0        // return 0 on xchg success
    bx    lr            // return
1:  clrex               // clear exclusive hold
    mov   r0, #1        // non-zero on mismatch/no xchg
    bx    lr            // return

// int atomic_swap8_conditional(void* mem, void* newlo, void* newhi,
//                                         void* oldlo, void* oldhi);
atomic_swap8_conditional:
    stmdb   sp!, {r4 - r7}  // save called saved regs
    ldr     r4, [sp, #0x14] // r4 = oldhi
1:  ldrexd  r6, r7, [r0]    // r6..r7 = *mem
    cmp     r3, r6          // *mem == old?
    cmpeq   r4, r7          //   "     "
    bne     1f              // branch if not equal
    strexd  r5, r6, r7, [r0]// *mem = new
    cmp     r5, #0          // strex succeed?
    bne     1b              // try again on failure
    mov     r0, #0          // load 0 into retval reg
    ldmia   sp!, {r4, r6}   // restore callee saved regs
    bx      lr              // return
1:  clrex                   // clear exclusive monitor
    mov     r0, #1          // load non-zero into retval regsiter
    ldmia   sp!, {r4-r7}    // restore callee saved regs
    bx      lr              // return

// int atomic_add(int* mem, int val);
atomic_add:
1:  ldrex r2, [r0]      // r2 = *mem
    add   r2, r2, r1    // r2 += val
    strex r3, r2, [r0]  // *mem = val, r1 == 0 if exclusive
    cmpeq r3, #0
    bne 1b              // if strex failed, try again
    mov r0, r2          // return new value
    bx lr
