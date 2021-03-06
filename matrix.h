/*
    EASYMINE: A little game using SDL libraries
    Copyright (C) 2008  John Shi

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    John Shi
    jshi@novell.com
*/

#ifndef _MATRIX_H
#define _MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define new(type,w,h) ({      \
	type *__mem;          \
	size_t __w = (w);     \
	size_t __h = (h);     \
	__mem = (type *)malloc(sizeof(size_t) + sizeof(type) * __w * __h); \
	if (!__mem) {     \
		fputs("Out of memory.\n", stderr); \
		exit(EXIT_FAILURE); \
	}    \
	*(size_t *)__mem = __h;     \
	__mem = (type *)((size_t *)__mem + 1); \
})

#define del(ptr) ({        \
	size_t *__ptr = (size_t *)(ptr);  \
	free(--__ptr);     \
})

#define locate(ptr,w,h) ({    \
	typeof(ptr) __ptr = (ptr);  \
	size_t __w = (w);  \
	size_t __h = (h);  \
	&__ptr[*((size_t *)__ptr-1) * __w + __h];  \
})

#endif
