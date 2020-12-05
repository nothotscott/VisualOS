/*
 * File:		shell.h
 * Desciption:	Provide a temporary, full resoluton, text interface 
 * Isn't it ironic to have a shell in an OS named VisualOS?
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

typedef uint	SHELL_COLOR;

#define	SHELL_COLOR_WHITE	0xffffffff
#define	SHELL_COLOR_RED		0xffff0000
#define	SHELL_COLOR_GREEN	0xff00ff00
#define	SHELL_COLOR_BLUE	0xff0000ff
#define	SHELL_COLOR_CYAN	0xff00ffff


// Initalize shell with the _frame_buffer and _font
void shell_init(struct FrameBuffer* _frame_buffer, struct PSF1Font* _font);


// Sets the cursor to the x and y
void set_cursor(uint x, uint y);

// Draws a character chr with color at xoffset and yoffset
void draw_char(char chr, SHELL_COLOR color, uint xoff, uint yoff);

// Prints a string at the current cursor position with color
void print(char* str, SHELL_COLOR color);
void print_char(char chr, SHELL_COLOR color);
void print_newline();

// Draws TGA img at xoffset and yoffset
void draw_tga(struct TGAImage* img, uint xoff, uint yoff);