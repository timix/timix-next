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

#include <compiler.h>
#include <timix/kernel.h>
#include <mm/pmm.h>
#include <arch/atags.h>
#include <arch/arm-arch.h>
#include <arch/kernel-limits.h>

//
// TODO: QEMU won't pass atags unless an "Image" is built (which is essentially
// a raw ramdisk of the kernel). Until that time all values typically received
// via atag values will have to be hardcoded.
//
void c_entry(struct atag *tags)
{
    uint32_t __unused kvstart = KERNEL_START;
    uint32_t __unused kvend   = KERNEL_END;

    pmm_init();
    // 0 - 32MB RAM
    pmm_add_region(0x0000, 0x32 << 20);
    // TODO: determine v/p offset and compensate here.
    // TODO: implement an interface like this to remove memory occupied now.
    // pmm_free_region(kvstart, kvend - kvstart);
    kernel_main();
}
