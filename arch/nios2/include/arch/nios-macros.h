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

.macro inten rn, rm
  rdctl \rn, ctl0     // Read 'status' register.
  movi  \rm, 1        // load bitmask
  nor   \rm, \rm, r0  // invert bitmask
  and   \rn, \rn, \rm // Clear interrupt enable bit
  wrctl ctl0, \rn     // Write 'status' back.
.endm

.macro intdis rn, rm
  rdctl \rn, ctl0     // Read 'status' register.
  movi  \rm, 1        // load bitmask
  or    \rm, \rm, r0  // invert bitmask
  wrctl ctl0, \rn     // Write 'status' back.
.endm
