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
#include <SDL.h>
#include "graphic.h"

#define FACE_NUM ((int)(sizeof(path)/sizeof(char*)))

static char *path[] = {
	"data/num0.bmp",
	"data/num1.bmp",
	"data/num2.bmp",
	"data/num3.bmp",
	"data/num4.bmp",
	"data/num5.bmp",
	"data/num6.bmp",
	"data/num7.bmp",
	"data/num8.bmp",
	"data/bomb.bmp",
	"data/flag.bmp",
	"data/bombing.bmp",
	"data/bomberr.bmp",
	"data/button.bmp"
};

static SDL_Surface *face[FACE_NUM] = {NULL};
static SDL_Surface *screen = NULL;

void init_graphic(int width, int height)
{
	int i;

	for (i = 0; i < FACE_NUM; i++) {
		face[i] = SDL_LoadBMP(path[i]);
		if (!face[i]) {
			fprintf(stderr, "Error: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
	}

	screen = SDL_SetVideoMode(face[FACE_BUTTON]->w*width, face[FACE_BUTTON]->h*height, 16, SDL_SWSURFACE);
	if (!screen) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

void deinit_graphic()
{
	int i;
	if (screen) SDL_FreeSurface(screen);
	for (i = 0; i < FACE_NUM; i++)
		if (face[i]) SDL_FreeSurface(face[i]);
}

void draw(int pic, int x, int y)
{
	SDL_Rect dest;

	dest.x = face[pic]->w * x;
	dest.y = face[pic]->h * y;
	dest.w = face[pic]->w;
	dest.h = face[pic]->h;
	if (SDL_BlitSurface(face[pic], NULL, screen, &dest) < 0) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
}

void pixel2pos(int *x, int *y)
{
	*x /= face[FACE_BUTTON]->w;
	*y /= face[FACE_BUTTON]->h;
}
