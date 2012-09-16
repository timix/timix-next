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

#ifndef TIMIX_ARM_KERNEL_LIMITS_H_
#define TIMIX_ARM_KERNEL_LIMITS_H_

#include <arch/types.h>

//
// These values will be populated by the linker (defined in the linker script).
//

extern void* _KERNEL_START_;
extern void* _KERNEL_END_;

extern void* _TEXT_START_;
extern void* _TEXT_END_;

extern void* _DATA_START_;
extern void* _DATA_END_;

extern void* _BSS_START_;
extern void* _BSS_END_;

#define KERNEL_START  ((intptr_t)&_KERNEL_START_)
#define KERNEL_END    ((intptr_t)&_KERNEL_END_)

#define TEXT_START    ((intptr_t)&_TEXT_START_)
#define TEXT_END      ((intptr_t)&_TEXT_END_)

#define DATA_START    ((intptr_t)&_DATA_START_)
#define DATA_END      ((intptr_t)&_DATA_END_)

#define BSS_START     ((intptr_t)&_BSS_START_)
#define BSS_END       ((intptr_t)&_BSS_END_)

#endif // TIMIX_ARM_KERNEL_LIMITS_H_
