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

#ifndef TIMIX_MULTIBOOT_H_
#define TIMIX_MULTIBOOT_H_

#include <stdint.h>

#define MB_MEM_VALID        0x0001
#define MB_BOOTDEV_VALID    0x0002
#define MB_CMDLINE_VALID    0x0004
#define MB_MODS_VALID       0x0008
// TODO: Figure out the difference between these
#define MB_SYMS1_VALID      0x0010
#define MB_SYMS2_VALID      0x0020
#define MB_MMAP_VALID       0x0040
#define MB_DRIVES_VALID     0x0080
#define MB_CFGTAB_VALID     0x0100
#define MB_BLNAME_VALID     0x0200
#define MB_APMTAB_VALID     0x0400
#define MB_GFXTAB_VALID     0x0800

struct mb_syms {
    uint32_t  tabsize;
    uint32_t  strsize;
    void*     addr;
    uint32_t  reserved;
};

struct mb_module {
    void*       mod_start;
    void*       mod_end;
    const char* mod_name;
    uint32_t    reserved;
};

struct mb_info {
    uint32_t          flags;
    uint32_t          mem_lower;
    uint32_t          mem_upper;
    uint32_t          boot_device;
    const char*       cmdline;
    uint32_t          mods_count;
    struct mb_module* mods_addr;
    struct mb_syms    syms;
    uint32_t          mmap_length;
    void*             mmap_addr;
    uint32_t          drives_length;
    void*             drives_addr;
    void*             config_table;
    const char*       boot_loader_name;
    void*             apm_table;

    uint32_t          vbe_control_info;
    uint32_t          vbe_mode_info;
    uint32_t          vbe_mode;
    uint32_t          vbe_interface_seg;
    uint32_t          vbe_interface_off;
    uint32_t          vbe_interface_len;
};

#endif // TIMIX_MULTIBOOT_H_
