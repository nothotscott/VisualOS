/*
 * File:		text.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "bootloader.h"
#include "framebuffer.h"
#include "module.h"
#include "shell.h"
#include "color.h"
#include "text.h"


static struct Point {
	uint32_t	x;
	uint32_t	y;
} s_cursor = {
	.x = 0,
	.y = 0
};
// TODO make this thread safe
static text_color_t s_color_state = TEXT_COLOR_FOREGROUND;


void text_set_cursor(uint32_t x, uint32_t y) {
	s_cursor.x = x;
	s_cursor.y = y;
}

void text_draw_char(char chr, text_color_t color, uint32_t xoff, uint32_t yoff){
	struct Framebuffer* framebuffer = bootloader_get_info()->framebuffer;
	struct Module* font = bootloader_get_info()->font;
	uint8_t charsize = ((struct ModulePSF1Header*)font->header)->charsize;
	uint8_t* text_font_ptr = font->body + (chr * charsize);	// map character to glyph "array"
	// Draw; dx is delta x and dy is delta y
	for(size_t dy = 0; dy < charsize; dy++){
		for(size_t dx = 0; dx < MODULE_PSF1_WIDTH; dx++){
			if((*text_font_ptr & (0b10000000 >> dx)) > 0){	// bit is enabled along the dx'th position on the glyph row
				size_t x = dx + xoff;
				size_t y = dy + yoff;
				framebuffer_putpix32(framebuffer, x, y, color);
			}
		}
		text_font_ptr++;
	}
}

// Changes the color's state based on [str] and [i] if it matches with an ansi color
static inline int text_handle_ansi_color(char* str, size_t i) {
	int relative_index;
	struct ColorInterface* color = color_from_ansi(str + i, &relative_index);
	if(relative_index != COLOR_MATCH_FAIL) {
		s_color_state = color->shell;
	}
	return relative_index;
}

// *** Output functions *** //

void text_output_color_size(char* str, text_color_t color, size_t size){
	for(size_t i = 0; i < size && str[i] != '\0'; i++) {
		text_output_char_color(str[i], color);
	}
}
void text_output_color(char* str, text_color_t color){
	char* chr = str;
	while(*chr != '\0'){
		text_output_char_color(*chr, color);
		chr++;
	}
}
void text_output(char* str) {
	size_t i = 0;
	while(str[i] != '\0') {
		int relative_index = text_handle_ansi_color(str, i);
		if(relative_index != COLOR_MATCH_FAIL) {
			i += relative_index - 1;	// subtract 1 since we're going to skip
			continue;
		}
		text_output_char(str[i]);
		i++;
	}
}
void text_output_size(char* str, size_t size){
	for(size_t i = 0; i < size /*&& str[i] != '\0'*/; i++) {
		int relative_index = text_handle_ansi_color(str, i);
		if(relative_index != COLOR_MATCH_FAIL) {
			i += relative_index - 1;
			continue;
		}
		text_output_char(str[i]);
	}
}

void text_output_char(char chr){
	text_output_char_color(chr, s_color_state);
}
void text_output_char_color(char chr, text_color_t color) {
	struct Framebuffer* framebuffer = bootloader_get_info()->framebuffer;
	struct ModulePSF1Header* font_header = (struct ModulePSF1Header*)(bootloader_get_info()->font->header);
	if(chr == '\n'){
		goto newline;
	}
	// Draw, if there's no objections
	text_draw_char(chr, color, s_cursor.x, s_cursor.y);
	s_cursor.x += MODULE_PSF1_WIDTH;
	// Check for overflow
	if(s_cursor.x < framebuffer->width){
		return;
	}
	newline:
		s_cursor.x = 0;
		s_cursor.y += font_header->charsize;
}

void text_output_newline(){
	text_output_char_color('\n', TEXT_COLOR_FOREGROUND);
}