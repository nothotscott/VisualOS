/*
 * File:		text.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "shell.h"
#include "text.h"


static struct PSF1Font* s_text_font;
static struct Point s_cursor = {
	.x = 0,
	.y = 0
};


void text_init(struct PSF1Font* font){
	s_text_font = font;
}

void text_set_cursor(uint32_t x, uint32_t y) {
	s_cursor.x = x;
	s_cursor.y = y;
}

void text_draw_char(char chr, text_color_t color, uint32_t xoff, uint32_t yoff){
	uint32_t* pixel_ptr = (uint32_t*)g_shell_frame_buffer->base;
	uint8_t charsize = s_text_font->header_ptr->charsize;
	uint8_t* text_font_ptr = s_text_font->glyph_buffer + (chr * charsize);	// map character to glyph "array"
	// Draw; dx is delta x and dy is delta y
	for(size_t dy = 0; dy < charsize; dy++){
		for(size_t dx = 0; dx < PSF1_WIDTH; dx++){
			if((*text_font_ptr & (0b10000000 >> dx)) > 0){	// bit is enabled along the dx'th position on the glyph row
				size_t x = dx + xoff;
				size_t y = dy + yoff;
				*(uint32_t*)(pixel_ptr + x + (y * g_shell_frame_buffer->ppsl)) = color;
			}
		}
		text_font_ptr++;
	}
}

void text_output_color(char* str, text_color_t color){
	char* chr = str;
	while(*chr != 0){
		text_output_char(*chr, color);
		chr++;
	}
}
void text_output(char* str){
	text_output_color(str, TEXT_COLOR_FOREGROUND);
}
void text_output_char(char chr, text_color_t color){
	if(chr == '\n'){
		goto newline;
	}
	// Draw, if there's no objections
	text_draw_char(chr, color, s_cursor.x, s_cursor.y);
	s_cursor.x += PSF1_WIDTH;
	// Check for overflow
	if(s_cursor.x < g_shell_frame_buffer->width){
		return;
	}
	newline:
		s_cursor.x = 0;
		s_cursor.y += s_text_font->header_ptr->charsize;
}
void text_output_newline(){
	text_output_char('\n', 0);
}