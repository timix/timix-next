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

#ifndef TIMIX_DRIVERS_PL111_H_
#define TIMIX_DRIVERS_PL111_H_

#include <timix/fb.h>
#include <timix/io.h>
#include <timix/string.h>
#include <drivers/pl111.h>

int pl111_init(struct pl111 *dev)
{
    memset(dev, 0, sizeof(*dev))
}

int pl111_mode_set(struct fbdev *fbdev struct fbmode *fbmode)
{

}

#endif // TIMIX_DRIVERS_PL111_H_
