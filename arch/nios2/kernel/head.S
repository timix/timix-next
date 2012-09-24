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

#include <arch/nios-macros.h>

//
// Kernel Entry Point
//
.globl _start
_start:
  intdis  r3, r2        // Disable interrupts
  movia   sp, istack    // setup initial stack pointer
  movia   r4, c_entry   // load address of c entry point
  jmp     r4            // branch to c entry point

.data
.space 4096             // Some space for the initial stack.
istack:                 // TODO: This will only work if kernel is 1:1 mapped.
