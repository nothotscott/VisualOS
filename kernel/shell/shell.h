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

#define	SHELL_WHITE	0xffffffff
#define	SHELL_RED	0xffff0000
#define	SHELL_GREEN	0xff00ff00
#define	SHELL_BLUE	0xff0000ff


// Initalize shell with the _frame_buffer and _font
void shell_init(struct FrameBuffer* _frame_buffer, struct PSF1Font* _font);


// Sets the cursor to the x and y
void set_cursor(uint x, uint y);

// Draws a character chr with color at xoffset and yoffset
void draw_char(char chr, uint color, uint xoff, uint yoff);

// Prints a string at the current cursor position with color
void print(char* str, uint color);

// Draws TGA img at xoffset and yoffset
void draw_tga(struct TGAImage* img, uint xoff, uint yoff);