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

#ifndef _CONTROL_H
#define _CONTROL_H

typedef struct _Box Box;

void init_game(int x, int y, int bombs);
void deinit_game();

void start_game();
void open_box(Box *box);
void set_flag(Box *box);
Box *get_box(int x, int y);
int is_gameover();

#endif
