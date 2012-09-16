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
//  mm/pmm.c - Physical Memory Manager
//
//  The purpose of this module is to track phyisical memory regions in the
//  system.
//
//  TODO: Should the frame manager be rolled into here?
//

#include <mm/pmm.h>
#include <timix/error.h>

/**
 * @brief Max number of non-contiguous memory regions.
 *
 * We don't want to depend on the heap for this (since the heap will be pulling
 * memory from this module.
 */
#define PMM_MAX_REGIONS 10

/**
 * @brief Define a region of physical memory.
 */
struct pmm_region
{
    /** @brief Start address of a region. */
    pa_t start;
    /** @brief Size of a region (in bytes) */
    pa_t size;
};

/**
 * @brief Table of available physical memory regions.
 */
struct pmm_region_map
{
    /**brief The number of valid entries in 'regions'. */
    uint32_t count;
    /** @brief Array of memory region descriptors. */
    struct pmm_region regions[PMM_MAX_REGIONS];
};

static struct pmm_region_map pmm_available;

void pmm_init(void)
{
    pmm_available.count = 0;
}

int pmm_add_region(pa_t start, pa_t size)
{
    int idx = pmm_available.count;

    if (pmm_available.count + 1 >= PMM_MAX_REGIONS) {
        // No room to store descriptor.
        return -ENOMEM;
    }

    pmm_available.count++;
    pmm_available.regions[idx].start = start;
    pmm_available.regions[idx].size = size;

    return 0;
}
