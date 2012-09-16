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

#include <mm/pmm.h>
#include <timix/panic.h>
#include <timix/kernel.h>
#include <arch/multiboot.h>

#define X86_HIGHMEM_START 0x00100000

void c_entry(struct mb_info *info)
{
    uint32_t membase = 0;
    uint32_t memsize = 0;

    if (info->flags & MB_MEM_VALID) {
        // TODO: I think there's more complexity involved here.

        //
        // Low memory starts at 0x00000000
        //
        if (info->mem_lower) {
            membase = 0;
            memsize = info->mem_lower;
            pmm_add_region(membase, memsize);
        }
        //
        // Upper memory starts at X86_HIGHMEM_START
        //
        if (info->mem_upper) {
            membase = X86_HIGHMEM_START;
            memsize = info->mem_upper;
            pmm_add_region(membase, memsize);
        }
    } else {
        // TODO: Can we recover (bios call)?
        panic("Unable to detect physical memory!\n");
    }

    pmm_init();
    pmm_add_region(0x0000, 0x32 << 20);
    // TODO: Free memory occupied by kernel now.
    kernel_main();
}
