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

#include <stdlib.h>
#include <timix/assert.h>

void* malloc(size_t bytes)
{
    assert(0 && "malloc unimplemented\n");
    return NULL;
}

void* calloc(size_t num, size_t size)
{
    assert(0 && "calloc unimplemented\n");
    return NULL;
}

void* realloc(void* ptr, size_t size)
{

    assert(0 && "realloc unimplemented\n");
    return NULL;
}

void* memalign(size_t align, size_t size)
{

    assert(0 && "memalign unimplemented\n");
    return NULL;
}

void  free(void* ptr)
{
    assert(0 && "Malloc unimplemented\n");
    return;
}
