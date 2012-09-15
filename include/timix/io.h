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

#ifndef TIMIX_IO_H_
#define TIMIX_IO_H_

#include <stdint.h>

//
// Architecture-provided functions.
//
void __write32(ioaddr_t addr, uint32_t val);
void __write16(ioaddr_t addr, uint16_t val);
void __write8(ioaddr_t addr, uint8_t val);
uint32_t __read32(ioaddr_t addr);
uint16_t __read16(ioaddr_t addr);
uint8_t  __read8(ioaddr_t addr);
void __wmb(void);
void __rmb(void);

/**
 * @brief Write Memory Barrier.
 */
static inline void wmb(void)
{
    __wmb();
}

/**
 * @brief Read Memory Barrier.
 */
static inline void rmb(void)
{
    __rmb();
}

/**
 * @brief Write a 32 bit value to an IO device.
 */
static inline void write32(ioaddr_t addr, uint32_t val)
{
    __write32(addr, val);
}

/**
 * @brief Write a 16 bit value to an IO device.
 */
static inline void write16(ioaddr_t addr, uint16_t val)
{
    __write16(addr, val);
}

/**
 * @brief Write a 8 bit value to an IO device.
 */
static inline void write8(ioaddr_t addr, uint8_t val)
{
    __write8(addr, val);
}

/**
 * @brief Read a 32 bit value from an IO device.
 */
static inline uint32_t read32(ioaddr_t addr)
{
    return __read32(addr);
}

/**
 * @brief Read a 16 bit value from an IO device.
 */
static inline uint16_t read16(ioaddr_t addr)
{
    return __read16(addr);
}

/**
 * @brief Read a 8 bit value from an IO device.
 */
static inline uint8_t read8(ioaddr_t addr)
{
    return __read8(addr);
}

#endif // TIMIX_IO_H_
