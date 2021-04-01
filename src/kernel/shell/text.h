/*
 * File:		text.h
 * Description: Provides visible text output with the provided frame buffer
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "bootloader.h"

typedef uint32_t	text_color_t;

#define	TEXT_COLOR_WHITE	0xffffffff
#define	TEXT_COLOR_BLACK	0xff000000
#define	TEXT_COLOR_GRAY		0xff888888
#define	TEXT_COLOR_RED		0xffff0000
#define	TEXT_COLOR_GREEN	0xff00ff00
#define	TEXT_COLOR_BLUE		0xff0000ff
#define	TEXT_COLOR_CYAN		0xff00ffff
#define	TEXT_COLOR_YELLOW	0xffffff00
#define	TEXT_COLOR_MAGENTA	0xffff00ff

#define TEXT_COLOR_BACKGROUND	TEXT_COLOR_BLACK
#define TEXT_COLOR_FOREGROUND	TEXT_COLOR_WHITE
#define TEXT_COLOR_ADDRESS		TEXT_COLOR_MAGENTA
#define TEXT_COLOR_NUMBER		TEXT_COLOR_YELLOW


struct Point {
	uint32_t	x;
	uint32_t	y;
};


// Initializes shell text with [font]
void text_init(struct PSF1Font* font);

// Sets the cursor to the [x] and [y]
void text_set_cursor(uint32_t x, uint32_t y);

// Draws a character [chr] with color at [xoffset] and [yoffset]
void text_draw_char(char chr, text_color_t color, uint32_t xoff, uint32_t yoff);

// Outputs a string [str] at the current cursor position with [color]
void text_output_color_size(char* str, text_color_t color, size_t size);
void text_output_color(char* str, text_color_t color);
void text_output(char* str);
void text_output_char(char chr, text_color_t color);
void text_output_newline();