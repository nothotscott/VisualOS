/*
 * File:		shell.h
 * Description:	Provide a temporary, full resoluton, text interface 
 * Isn't it ironic to have a shell in an OS named VisualOS?
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "bootloader.h"

typedef uint_t	SHELL_COLOR;

#define	SHELL_COLOR_WHITE	0xffffffff
#define	SHELL_COLOR_BLACK	0xff000000
#define	SHELL_COLOR_GRAY	0xff888888
#define	SHELL_COLOR_RED		0xffff0000
#define	SHELL_COLOR_GREEN	0xff00ff00
#define	SHELL_COLOR_BLUE	0xff0000ff
#define	SHELL_COLOR_CYAN	0xff00ffff
#define	SHELL_COLOR_YELLOW	0xffffff00
#define	SHELL_COLOR_MAGENTA	0xffff00ff

#define SHELL_COLOR_BACKGROUND	SHELL_COLOR_BLACK
#define SHELL_COLOR_FOREGROUND	SHELL_COLOR_WHITE
#define SHELL_COLOR_ADDRESS		SHELL_COLOR_MAGENTA
#define SHELL_COLOR_NUMBER		SHELL_COLOR_YELLOW


struct Point {
	uint_t	x;
	uint_t	y;
};


// Initalize shell with the [frame_buffer] and [font]
void shell_init(struct FrameBuffer* frame_buffer, struct PSF1Font* font);


// Sets the cursor to the [x] and [y]
void shell_set_cursor(uint_t x, uint_t y);

// Draws a character [chr] with color at [xoffset] and [yoffset]
void shell_draw_char(char chr, SHELL_COLOR color, uint_t xoff, uint_t yoff);

// Prints a string [str] at the current cursor position with [color]
void print_color(char* str, SHELL_COLOR color);
void print(char* str);
void print_char(char chr, SHELL_COLOR color);
void print_newline();

// Draws TGA img at [xoffset] and [yoffset]
void shell_draw_tga(struct TGAImage* img, uint_t xoff, uint_t yoff);

// Clears the framebuffer
void shell_clear();