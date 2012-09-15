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

// uint8_t __read8(uint16_t addr);
__read8:
    xorl %eax, %eax
    movw 4(%esp), %dx
    in %dx, %al
    ret

// uint16_t __read16(uint16_t addr);
__read16:
    xorl %eax, %eax
    movw 4(%esp), %dx
    in %dx, %ax
    ret

// uint32_t __read32(uint16_t addr);
__read32:
    xorl %eax, %eax
    movw 4(%esp), %dx
    in %dx, %eax
    ret

// void __write8(uint16_t addr, uint8_t val);
__write8:
    movw 4(%esp), %dx
    movb 8(%esp), %al
    out %al, %dx
    ret

// void __write8(uint16_t addr, uint16_t val);
__write16:
    movw 4(%esp), %dx
    movw 8(%esp), %ax
    out %ax, %dx
    ret

// void __write8(uint16_t addr, uint32_t val);
__write32:
    movw 4(%esp), %dx
    movl 8(%esp), %eax
    out %eax, %dx
    ret
