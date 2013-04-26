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

// 1MB sections
#define SECTION_SIZE 0x100000
#define SECTION_MASK (SECTION_SIZE -1)
// 16KB page directory
#define PAGEDIR_SIZE 0x4000

//
// Kernel Entry Point
//
.globl _start
_start:
  mov r0, pc
  bic r0, r0, #0xff   // r0 = physical address of _start
  mov r10, r0         // Save physical start address
  bl  __setup_pgtbl   // Setup the initial page directory
  ldr r1, =__call_kernel
  bl  __enable_mmu    // Turn on MMU
.ltorg                // Place constant pool here

//
// Jump to the kernel (assumes mmu enabled)
//
__call_kernel:
  ldr r0, =kern_phys_base
  str r1, [r0]        // save kern_phys_start value
  mov r0, r2          // atags pointer in arg0
  ldr sp, =istack     // Setup initial stack pointer
  b c_entry           // Branch to C entry point

//
// Enable the MMU given a constructed page table.
//
// r0 -> Physical address of page directory
// r1 -> Virtual address to jump to once enabled
//
__enable_mmu:
  mrc p15, 0, r3, c2, c0, 2   // Read TTBCR
  bic r3, r3, #7              // Set TTBCR.N = 0
  mcr p15, 0, r3, c2, c0, 2   // Write TTBCR

  mcr p15, 0, r0, c2, c0, 0   // Write TTBR0

  mrc p15, 0, r3, c3, c0, 0   // Read DACR
  orr r3, r3, #3              // Set full permission for domain 0
  mcr p15, 0, r3, c3, c0, 0   // Write DACR

  mrc p15, 0, r3, c1, c0, 0   // Read SCTLR
  orr r3, r3, #1              // Enable MMU
  mcr p15, 0, r3, c1, c0, 0   // Read SCTLR
  bx r1

//
// Builds just enough of a page table to enable the mmu without crashing.
//
// For now just map 1MB section assuming no overlap (this won't hold if/when
// the kernel gets too large or the kernel needs relocated (not 1MB aligned
// wrt the physical address map).
//
// Arguments:
//  r0 -> Physical address of _start
// Returns:
//  r0 -> Page Directory
// Trashes:
//  r0, r3 - r7
//
__setup_pgtbl:
  ldr r4, =PAGEDIR_SIZE
  sub r3, r0, r4      // r3 = phys_start - pgdir_size
  ldr r4, =SECTION_MASK
  ldr r8, =_KERNEL_END_
  //
  // zero out page directory
  //
  mov r5, #0          // r5 = 0
  mov r6, r3          // r6 = pagedir base
  ldr r7, =PAGEDIR_SIZE // r7 = pagedir size
1:
  str r5, [r6, #0x0]  // zero out 4 words
  str r5, [r6, #0x4]  //  "       "
  str r5, [r6, #0x8]  //  "       "
  str r5, [r6, #0xc]  //  "       "
  add r6, r6, #0x10   // Increment pointer
  cmp r6, r0          // check if we have more bytes
  bne 1b              // loop if more bytes to zero

  //
  // Populate page directory entries
  //
  bic r5, r0, r4      // r5 = 1:1 section base
  ldr r6, =_KERNEL_START_ // r6 = p:v section base
1:
  // Setup pagedir entry (r7)
  mov r7, r5          // r7 = section index
  ldr r8, =0xc02      // r6 = pagedir setup bitmask
  orr r7, r7, r8      // r7 = pagedir entry

  // Install 1:1 mapped entry
  lsr r8, r5, #18     // r8 = page dir offset
  add r8, r3, r8      // r8 = page dir entry address
  str r7, [r8]        // write entry to table
  
  // Install p:v entry
  lsr r8, r6, #18     // r8 = page dir offset
  add r8, r3, r8      // r8 = page dir entry address
  str r7, [r8]        // write entry to table

  ldr r8, =SECTION_SIZE
  add r5, r5, r8      // increment 1:1 pointer
  add r6, r6, r8      // increment p:v pointer
  ldr r8, =_KERNEL_END_
  cmp r8, r6          // kernel mapped?
  bge 1b              // repeat if more memory to map

  mov r0, r3          // return pagedir location
  bx lr

.globl kern_phys_base
kern_phys_base:
.space 4

//
// Initial stack: 1 frame.
//
.data                 // data section
.align 12             // 4kb aligned
istack_end:
.space 4096           // Some space for the initial stack.
istack:
