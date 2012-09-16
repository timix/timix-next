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

.globl readCpsr
.globl writeCpsr
.globl readSpsr
.globl writeSpsr

readCpsr:
    mrs r0, cpsr
    bx lr

writeCpsr:
    msr cpsr, r0
    bx lr

readSpsr:
    mrs r0, spsr
    bx lr

writeSpsr:
    msr spsr, r0
    bx lr
