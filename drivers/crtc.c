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

#include <crtc.h>

int crtc_init(struct crtc *dev)
{
    dev->chardev.read  = &crtc_read;
    dev->chardev.write = &crtc_write;

    dev->mode = CRTC_MODE_TEXT;
    dev->u.text.screen = (struct crtc_char*)CRTC_MEM_BASE;
    dev->u.text.cursor_x = 0;
    dev->u.text.cursor_y = 0;
    dev->u.text.cursor_state = CURSOR_STATE_VISIBLE;
    dev->u.text.cursor_color = (CRTC_FGND_WHITE | CRTC_BGND_BLACK);
}

int crtc_read(char *buf, size_t len)
{
    // CRTC provides no output
    return 0;
}

int crtc_write(const char *buf, size_t len)
{
    return 0;
}
