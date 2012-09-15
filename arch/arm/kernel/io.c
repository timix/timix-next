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

#include <timix/io.h>

inline void __wmb(void)
{
    asm("dsb");
}

inline void __rmb(void)
{
    asm("dsb");
}

void __write32(ioaddr_t addr, uint32_t val)
{
    __wmb();
    *(uint32_t*)addr = val;
}

void __write16(ioaddr_t addr, uint16_t val)
{
    __wmb();
    *(uint16_t*)addr = val;
}

void __write8(ioaddr_t addr, uint8_t val)
{
    __wmb();
    *(uint8_t*)addr = val;
}

uint32_t __read32(ioaddr_t addr)
{
    __rmb();

    return *(uint32_t*)addr;
}

uint16_t __read16(ioaddr_t addr)
{
    __rmb();
    return *(uint16_t*)addr;
}

uint8_t  __read8(ioaddr_t addr)
{
    __rmb();
    return *(uint8_t*)addr;
}
