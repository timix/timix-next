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

#define LCDTIMING0          0x000
#define LCDTIMING1          0x004
#define LCDTIMING2          0x008
#define LCDTIMING3          0x00c
#define LCDUPBASE           0x010
#define LCDLPBASE           0x014
#define LCDCONTROL          0x018
#define LCDIMSC             0x01c
#define LCDRIS              0x020
#define LCDMIS              0x024
#define LCDICR              0x028
#define LCDUPCURR           0x02c
#define LCDLPCURR           0x030
#define LCDPALETTE          0x200
#define CURSOR_IMAGE        0x800
#define CLCD_CRSR_CTRL      0xc00
#define CLCD_CRSR_CONFIG    0xc04
#define CLCD_CRSR_PALETTE0  0xc08
#define CLCD_CRSR_PALETTE1  0xc0c
#define CLCD_CRSR_XY        0xc10
#define CLCD_CRSR_CLIP      0xc14
#define CLCD_CRSR_IMSC      0xc20
#define CLCD_CRSR_ICR       0xc24
#define CLCD_CRSR_RIS       0xc28
#define CLCD_CRSR_MIS       0xc2c
#define CLCD_PERIPH_ID0     0xfe0
#define CLCD_PERIPH_ID1     0xfe4
#define CLCD_PERIPH_ID2     0xfe8
#define CLCD_PERIPH_ID3     0xfec
#define CLCD_PCELL_ID0      0xff0
#define CLCD_PCELL_ID1      0xff4
#define CLCD_PCELL_ID2      0xff8
#define CLCD_PCELL_ID3      0xffc

struct pl111
{
    struct fbdev  upper_panel;
    struct fbdev  lower_panel;
};

int pl111_init(struct pl111 *dev);
int pl111_mode_set(struct pl111 *dev struct fbmode *fbmode);

#endif // TIMIX_DRIVERS_PL111_H_
