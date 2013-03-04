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


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#include "graphic.h"
#include "matrix.h"
#include "control.h"

struct _Box {
	int x, y;
	int isbomb;
	int flags;
	int bombs;
	int face;
};

enum {
	left = 0,
	right,
	up,
	down,
	left_up,
	left_down,
	right_up,
	right_down,
	endway
};

static Box *boxes;
static int width, height;
static int total_box, total_bomb, bomb_count;
static int gameover;


Box *get_box(int x, int y)
{
	return locate(boxes,x,y);
}

static void change_face(Box *box, int face)
{
	box->face = face;
	draw(face, box->x, box->y);
}

/* Get the round of Box */
static Box *move(const Box *box, int way)
{
	int x, y;

	x = box->x;
	y = box->y;
	switch (way) {
		case left:
			if (--x < 0)
				return NULL;
			break;
		case right:
			if (++x > width-1) 
				return NULL;
			break;
		case up:
			if (--y < 0) 
				return NULL;
			break;
		case down:
			if (++y > height-1) 
				return NULL;
			break;
		case left_up:
			if (--x < 0 || --y < 0) 
				return NULL;
			break;
		case left_down:
			if (--x < 0 || ++y > height-1)
				return NULL;
			break;
		case right_up:
			if (++x > width-1 || --y < 0) 
				return NULL;
			break;
		case right_down:
			if (++x > width-1 || ++y > height-1) 
				return NULL;
			break;
		default:
			return NULL;
	}

	return get_box(x, y);
}

static void clean()
{
	int i;

	bomb_count = total_bomb;
	for (i = 0; i < total_box; i++) {
		boxes[i].isbomb = 0;
		boxes[i].bombs = 0;
		boxes[i].flags = 0;
		change_face(&boxes[i], FACE_BUTTON);
	}
}

static void succeed()
{
	int i;

	for (i = 0; i < total_box; i++)
		if (boxes[i].face == FACE_FLAG && !boxes[i].isbomb) 
			return;
	gameover = 1;

	printf("\x1b[32;40m");
	printf("\nCongratulations, all mines were cleaned.\n");
	printf("\x1b[0;0m");
}

static void failed(Box *box)
{
	int i;

	for (i = 0; i < total_box; i++) {
		if (boxes[i].face == FACE_FLAG) {
			if (!boxes[i].isbomb)
				change_face(&boxes[i], FACE_BOMBERR);
		} else if (boxes[i].isbomb)
			change_face(&boxes[i], FACE_BOMB);
	}
	change_face(box, FACE_BOMBING);

	gameover = 1;

	/* green foreground and black background */
	printf("\x1b[32;40m");
	printf("\nSorry, you failed.\n");
	printf("\x1b[0;0m");
}

static void detect_bomb(Box *box)
{
	int i;
	Box *next;

	if (box->face != FACE_BUTTON)
		return;

	if (box->isbomb) {
		failed(box);
		return;
	}

	change_face(box, box->bombs);

	if (box->bombs == 0) {
		for (i = 0; i < endway; i++) {
			next = move(box, i);
			if (next) detect_bomb(next);
		}
	}
}

void init_game(int x, int y, int bombs)
{
	int i, j;

	width = x;
	height = y;
	total_bomb = bomb_count = bombs;

	total_box = x * y;
	boxes = new(Box,x,y);

	for (i = 0; i < x; i++) {
		for (j = 0; j < y; j++) {
			get_box(i, j)->x = i;
			get_box(i, j)->y = j;
		}
	}

	printf("\nThe size of mine field is %dx%d.\n", width, height);
	printf("%d mines were deployed.\n", bombs);
}

void deinit_game()
{
	del(boxes);
}

void start_game()
{
	Box *next;
	int i, j, index, *cont;

	gameover = 0;
	clean();

	cont = (int *)malloc(total_box * sizeof (int));
	if (!cont) {
		fputs("Out of memory.\n", stderr);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < total_box; i++)
		cont[i] = i;

	srand((int)time(0));

	for (i = 0; i < total_bomb; i++) {
		index = (int)((float)(total_box-i) * rand() / (RAND_MAX+1.0));

		boxes[cont[index]].isbomb = 1;
		for (j = 0; j < endway; j++) {
			next = move(&boxes[cont[index]], j);
			if (next) next->bombs++;
		}

		cont[index] = cont[total_box-1-i];
	}

	free(cont);
}

/* 
 * Two missions:
 * 1. open a bomboxesox which you never touched(FACE_BUTTON icon).
 * 2. open the round of bomboxesox
 */
void open_box(Box *box)
{
	int i;
	Box *next;

	if (box->face == FACE_BUTTON)
		detect_bomb(box);
	else if (box->face != FACE_FLAG && box->flags == box->bombs) {
		for (i = 0; i < endway; i++) {
			next = move(box, i);
			if (next) detect_bomb(next);
		}
	}
}

/* Set or cancel the flag on a bomboxesox */
void set_flag(Box *box)
{
	Box *next;
	int addon, i;

	switch (box->face) {
		case FACE_BUTTON:
			change_face(box, FACE_FLAG);
			bomb_count--;
			addon = 1;
			break;
		case FACE_FLAG:
			change_face(box, FACE_BUTTON);
			bomb_count++;
			addon = -1;
			break;
		default:
			return;
	}

	for (i = 0; i < endway; i++) {
		next = move(box, i);
		if (next) next->flags += addon;
	}

	printf("\rRemaining mines: ");
	printf("\x1b[31;40m");   /* red foreground and black background */
	printf("%d ", bomb_count);
	printf("\x1b[0;0m");
	fflush(stdout);
	if (bomb_count == 0)
		succeed();
}

int is_gameover()
{
	return gameover;
}
