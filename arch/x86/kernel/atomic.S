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

.globl atomic_swap
.globl atomic_swap_conditional
.globl atomic_swap8_conditional
.globl atomic_add

atomic_swap:
    movl    4(%esp), %ecx           // Get Memory Argument
    movl    8(%esp), %eax           // Get Value
    lock xchgl   %eax, (%ecx)       // Do the exchange, old value in eax
    ret

atomic_swap_conditional:
    movl      12(%esp), %eax        // Move oldval to %eax
    movl      8(%esp), %ecx         // Move newval to %ecx
    movl      4(%esp), %edx         // Move memory address to %edx
    lock cmpxchgl  %ecx, (%edx)     // Do the compare and exchange
    jnz       no_xchg               // Jump if unsuccessful
    movl      $0, %eax              // Move success return value to %eax
    ret
no_xchg:
    movl      $1, %eax              // Move error return value to %eax
    ret

atomic_swap8_conditional:
    pushl     %ebx
    pushl     %edi
    movl      12(%esp), %edi        // Move memory address into %edi
    movl      16(%esp), %ebx        // Get new_lo
    movl      20(%esp), %ecx        // Get new_hi
    movl      24(%esp), %eax        // Get old_lo
    movl      28(%esp), %edx        // Get old_hi
    lock cmpxchg8b (%edi)           // Do cmpxchg
    jnz       no_xchg8b             // Jump if memory has changed and no swap
    movl      $0, %eax              // Return 0 on success
    popl      %edi
    popl      %ebx
    ret
no_xchg8b:
    movl      $1, %eax              // Return 1 on failure
    popl      %edi
    popl      %ebx
    ret

atomic_add:
    movl      4(%esp), %ecx     // Move pointer to %eax
    movl      8(%esp), %eax     // Move amount to add into %ecx
    xaddl     %eax, (%ecx)      // Do the addition.
    ret                         // Returns old val of (%ecx)
