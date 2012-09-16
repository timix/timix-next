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

#include <arch/types.h>

/**
 * @brief Physical address type.
 */
typedef intptr_t pa_t;

/**
 * @brief Initialize the physical memory manager.
 */
void pmm_init(void);

/**
 * @brief Add a region of physical memory
 *
 * @param start The base address of the phyical region.
 * @param size  The size of they physical region.
 */
int pmm_add_region(pa_t start, pa_t size);
