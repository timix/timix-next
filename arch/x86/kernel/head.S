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
// TODO: setup segments and a task
//

//
// Multiboot Header
//
// This needs to be at the top of the kernel image to be compatible with grub
// (or any other multiboot bootloader for that matter).
//
.text
mb_header:
    .long 0x1BADB002                        // Multiboot 'Magic'
    .long 0x00000000                        // Flags
    .long (0 - 0x1BADB002 - 0x00000000)     // Checksum
    .space 244                              // Unused

.global _start
_start:
    cli                                     // Disable Interrupts
    leal istack, %esp                       // Setup initial stack pointer.
    push %ebx                               // arg0 = mb_info pointer
    call c_entry                            // TODO: Pass multiboot info to c_entry

.data
.space 4096                                 // Space for an initial stack.
istack:                                     // TODO: this won't work when the mmu is on.
