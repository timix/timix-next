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

#ifndef TIMIX_CHARDEV_H_
#define TIMIX_CHARDEV_H_

#include <stddef.h>

typedef int (*reader_t)(char* buf, size_t len);
typedef int (*writer_t)(const char* buf, size_t len);

struct chardev
{
    reader_t read;
    writer_t write;
};

#endif // TIMIX_CHARDEV_H_
