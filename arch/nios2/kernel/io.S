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

.global __read8
.global __read16
.global __read32
.global __write8
.global __write16
.global __write32

//
// x86 has instructions to read/write to devices. The instructions 'in' and
// 'out' read and write to io registers, the width of the access is determined
// by the width of the operand registers.
//

// uint8_t __read8(uint16_t addr);
__read8:
    ldbio   r2, 0(r4)     // r2 = *addr
    ret

// uint16_t __read16(uint16_t addr);
__read16:
    ldhio   r2, 0(r4)     // r2 = *addr
    ret

// uint32_t __read32(uint16_t addr);
__read32:
    ldwio   r2, 0(r4)     // r2 = *addr
    ret

// void __write8(uint16_t addr, uint8_t val);
__write8:
    stbio   r5, 0(r4)
    ret

// void __write8(uint16_t addr, uint16_t val);
__write16:
    sthio   r5, 0(r4)
    ret

// void __write8(uint16_t addr, uint32_t val);
__write32:
    stwio   r5, 0(r4)
    ret
