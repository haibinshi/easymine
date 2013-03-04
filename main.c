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
#include <getopt.h>
#include <SDL.h>

#include "graphic.h"
#include "control.h"
#include "version.h"

#define W_DEF 20
#define H_DEF 15

static const char *progname;

static void usage(int stat)
{
	if (stat)
		fprintf (stderr, "Try `%s --help' for more information.\n", progname);
	else
		printf("Usage: %s [OPTIONS]...\n"
		"  -w, --width  N (1-99)    set the field width  (default %d)\n"
		"  -h, --height N (1-99)    set the field height (default %d)\n"
		"  -b, --bombs  N (1-9801)  set the number of bombs\n"
		"  -H, --help               show this help and exit\n"
		"VERSION " VERSION " (C) 2008 John Shi\n", progname, W_DEF, H_DEF);
}

int main(int argc, char **argv)
{
	SDL_Event event;
	Uint8 mstat;
	int xw, yh, optc;
	int bombs = -1, width = -1, height = -1;
	struct option const longopts[] = {
		{"width", 1, NULL, 'w'},
		{"height", 1, NULL, 'h'},
		{"bombs", 1, NULL, 'b'},
		{"help", 0, NULL, 'H'},
		{NULL, 0, NULL, 0}
	};

	progname = argv[0];

	while ((optc = getopt_long(argc, argv, "w:h:b:H", longopts, NULL)) != -1) {
		switch (optc) {
			case 'w':
				width = atoi(optarg);
				break;
			case 'h':
				height = atoi(optarg);
				break;
			case 'b':
				bombs = atoi(optarg);
				break;
			case 'H':
				usage(0);
				exit(EXIT_SUCCESS);
			default:
				usage(1);
				exit(EXIT_FAILURE);
		}
	}
	usage(0);

	/* Use default value if invalid argument is given. */
	if (width < 1 || width > 99)
		width = W_DEF;
	if (height < 1 || height > 99)
		height = H_DEF;
	if (bombs < 1 || bombs > width*height)
		bombs = width*height / 6;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	atexit(SDL_Quit);

	init_graphic(width, height);
	atexit(deinit_graphic);

	init_game(width, height, bombs);
	atexit(deinit_game);

	start_game();

	while (SDL_WaitEvent(&event) >= 0) {
		switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
				if (is_gameover()) {
					start_game();
					break;
				}

				xw = event.motion.x;
				yh = event.motion.y;
				pixel2pos(&xw, &yh);

				mstat = SDL_GetMouseState(NULL, NULL);
				if(mstat & SDL_BUTTON(1) || mstat & SDL_BUTTON(2)) 
					open_box(get_box(xw, yh));
				else if (mstat & SDL_BUTTON(3))
					set_flag(get_box(xw, yh));
				break;
			case SDL_QUIT:
				putchar('\n');
				exit(EXIT_SUCCESS);
				break;
			default:
				break; /* Nothing. */
		}
	}

	return 0;
}
