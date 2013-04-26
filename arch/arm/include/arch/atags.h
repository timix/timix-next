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

#ifndef TIMIX_ATAGS_H_
#define TIMIX_ATAGS_H_

#include <stdint.h>

#define ATAG_NEXT(ptr) (((uint32_t*)ptr) + ((struct atag_header*)ptr)->size)

#define ATAG_NONE       0x00000000
#define ATAG_CORE       0x54410001
#define ATAG_MEM        0x54410002
#define ATAG_VIDEOTEXT  0x54410003
#define ATAG_RAMDISK    0x54410004
#define ATAG_INITRD1    0x54410005
#define ATAG_INITRD2    0x54420005
#define ATAG_SERIAL     0x54410006
#define ATAG_REVISION   0x54410007
#define ATAG_VIDEOLFB   0x54410008
#define ATAG_CMDLINE    0x54410009

struct atag_header {
    uint32_t size;
    uint32_t tag;
};

struct atag_core {
    uint32_t flags;
    uint32_t pagesize;
    uint32_t rootdev;
};

struct atag_mem {
    uint32_t size;
    uint32_t start;
};

struct atag_videotext {
    uint8_t x;
    uint8_t y;
    uint16_t video_page;
    uint8_t video_mode;
    uint8_t video_cols;
    uint16_t video_ega_bx;
    uint8_t video_lines;
    uint8_t video_isvga;
    uint16_t video_points;
};

struct atag_ramdisk {
    uint32_t flags;
    uint32_t size;
    uint32_t start;
};

struct atag_initrd {
    uint32_t start;
    uint32_t size;
};

struct atag_serial {
    uint32_t low;
    uint32_t high;
};

struct atag_revision {
    uint32_t rev;
};

struct atag_videolfb {
    uint16_t lfb_width;
    uint16_t lfb_height;
    uint16_t lfb_depth;
    uint16_t lfb_linelength;
    uint32_t lfb_base;
    uint32_t lfb_size;
    uint8_t red_size;
    uint8_t red_pos;
    uint8_t green_size;
    uint8_t green_pos;
    uint8_t blue_size;
    uint8_t blue_pos;
    uint8_t rsvd_size;
    uint8_t rsvd_pos;
};

struct atag_cmdline {
    // Size is determined by header, 1 is minimum size.
    char cmdline[1];
};

struct atag {
    struct atag_header hdr;
    union {
        struct atag_core      core;
        struct atag_mem       mem;
        struct atag_videotext videotext;
        struct atag_ramdisk   ramdisk;
        struct atag_initrd    initrd;
        struct atag_serial    serial;
        struct atag_revision  revision;
        struct atag_videolfb  videolfb;
    } u;
};

static inline int atags_ok(struct atag* atag)
{
    return (atag->hdr.size == 2 || atag->hdr.size == 5) &&
            atag->hdr.tag == ATAG_CORE;
}

#endif // TIMIX_ATAGS_H_
