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

#ifndef _GRAPHIC_H
#define _GRAPHIC_H

#define FACE_BOMB     9
#define FACE_FLAG    10
#define FACE_BOMBING 11
#define FACE_BOMBERR 12
#define FACE_BUTTON  13

void init_graphic(int width, int height);
void deinit_graphic();
void draw(int pic, int x, int y);
void pixel2pos(int *x, int *y);

#endif
