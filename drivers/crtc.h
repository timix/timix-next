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

#ifndef TIMIX_CRTC_H_
#define TIMIX_CRTC_H_

#include <compiler.h>
#include <timix/chardev.h>

#define CRTC_MODE_TEXT  1
#define CRTC_MODE_VIDEO 2

#define CRTC_MEM_BASE 0xb8000
#define CRTC_INDEX    0x3d4
#define CRTC_DATA     0x3d5

#define CRTC_TEXT_WIDTH  80
#define CRTC_TEXT_HEIGHT 25

#define CURSOR_STATE_VISIBLE 1
#define CURSOR_STATE_HIDDEN  0

//
// Color Definitions
//
#define CRTC_FGND_BLACK     0x0
#define CRTC_FGND_BLUE      0x1
#define CRTC_FGND_GREEN     0x2
#define CRTC_FGND_CYAN      0x3
#define CRTC_FGND_RED       0x4
#define CRTC_FGND_MAGENTA   0x5
#define CRTC_FGND_BROWN     0x6
#define CRTC_FGND_LGREY     0x7
#define CRTC_FGND_DGREY     0x8
#define CRTC_FGND_BBLUE     0x9
#define CRTC_FGND_BGREEN    0xa
#define CRTC_FGND_BCYAN     0xb
#define CRTC_FGND_PINK      0xc
#define CRTC_FGND_BMAGENTA  0xd
#define CRTC_FGND_YELLOW    0xe
#define CRTC_FGND_WHITE     0xf

#define CRTC_BGND_BLACK     0x00
#define CRTC_BGND_BLUE      0x10
#define CRTC_BGND_GREEN     0x20
#define CRTC_BGND_CYAN      0x30
#define CRTC_BGND_RED       0x40
#define CRTC_BGND_MAGENTA   0x50
#define CRTC_BGND_BROWN     0x60
#define CRTC_BGND_LGREY     0x70

#define CRTC_BLINK          0x80

struct crtc_char
{
    char ascii;
    char color
} __packed;

struct crtc_text_screen
{
    struct crtc_char* screen;
    int               cursor_x;
    int               cursor_y;
    int               cursor_state;
    int               cursor_color;
};

struct crtc_video_screen
{
    // TODO:
};

struct crtc
{
    // @brief Character device
    struct chardev chardev;

    // @brief Mode (text/video)
    int mode;

    union
    {
        struct crtc_text_screen text;
        struct crtc_video_screen video;
    } u;
};

int crtc_init(struct crtc *dev);
int crtc_read(char *buf, size_t len);
int crtc_write(const char *buf, size_t len);

#endif // TIMIX_CRTC_H_
