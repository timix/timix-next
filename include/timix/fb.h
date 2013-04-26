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

#ifndef TIMIX_FB_H_
#define TIMIX_FB_H_

struct fbmode
{
    uint16_t  bpp;
    uint16_t  rmax;
    uint16_t  gmax;
    uint16_t  bmax;
    uint8_t   rshift;
    uint8_t   gshift;
    uint8_t   bshift;
    uint32_t  width;
    uint32_t  height;
};

struct fbdev
{
    void*           bufptr;
    struct fbmode   mode;
};

#endif // TIMIX_FB_H_
