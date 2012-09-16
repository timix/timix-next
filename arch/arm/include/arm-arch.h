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

#ifndef TIMIX_ARM_ARCH_H_
#define TIMIX_ARM_ARCH_H_

#include <stdint.h>

/**
 * @brief Read the Current Processor State Register.
 */
uint32_t readCpsr();

/**
 * @brief Write the Current Processor State Register.
 */
void writeCpsr(uint32_t val);

/**
 * @brief Read the Saved Processor State Register.
 */
uint32_t readSpsr();

/**
 * @brief Write the Saved Processor State Register.
 */
void writeSpsr(uint32_t val);

#endif // TIMIX_ARM_ARCH_H_
