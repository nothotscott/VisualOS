/*
 * File:		shell.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include <spacial.h>
#include <memory.h>
#include "shell.h"

// Initalize globals
static struct Point g_cursor = {
	.x = 0,
	.y = 0
};
static struct FrameBuffer* g_frame_buffer;
static struct PSF1Font* g_font;

void shell_init(struct FrameBuffer* frame_buffer, struct PSF1Font* font){
	g_frame_buffer = frame_buffer;
	g_font = font;
}

void shell_set_cursor(uint_t x, uint_t y) {
	g_cursor.x = x;
	g_cursor.y = y;
}

void shell_draw_char(char chr, SHELL_COLOR color, uint_t xoff, uint_t yoff){
	uint_t* pixel_ptr = (uint_t*)g_frame_buffer->base;
	byte_t charsize = g_font->header_ptr->charsize;
	byte_t* g_font_ptr = g_font->glyph_buffer + (chr * charsize);	// map character to glyph "array"
	// Draw; dx is delta x and dy is delta y
	for(size_t dy = 0; dy < charsize; dy++){
		for(size_t dx = 0; dx < PSF1_WIDTH; dx++){
			if((*g_font_ptr & (0b10000000 >> dx)) > 0){	// bit is enabled along the dx'th position on the glyph row
				size_t x = dx + xoff;
				size_t y = dy + yoff;
				*(uint_t*)(pixel_ptr + x + (y * g_frame_buffer->ppsl)) = color;
			}
		}
		g_font_ptr++;
	}
}

void print(char* str, SHELL_COLOR color){
	char* chr = str;
	while(*chr != 0){
		print_char(*chr, color);
		chr++;
	}
}
void print_char(char chr, SHELL_COLOR color){
	if(chr == '\n'){
		goto newline;
	}
	// Draw, if there's no objections
	shell_draw_char(chr, color, g_cursor.x, g_cursor.y);
	g_cursor.x += PSF1_WIDTH;
	// Check for overflow
	if(g_cursor.x < g_frame_buffer->width){
		return;
	}
	newline:
		g_cursor.x = 0;
		g_cursor.y += g_font->header_ptr->charsize;
}
void print_newline(){
	print_char('\n', NULL);
}

void shell_draw_tga(struct TGAImage* img, uint_t xoff, uint_t yoff){
	uint_t* pixel_ptr = (uint_t*)g_frame_buffer->base;
	uint_t* img_ptr = (uint_t*)img->buffer;
	uint_t height = img->header_ptr->height;
	uint_t width = img->header_ptr->width;
	for(size_t dy = 0; dy < height; dy++){
		for(size_t dx = 0; dx < width; dx++){
			size_t offset = dx + (height * dy);
			uint_t color = *(img_ptr + offset);
			size_t x = dx + xoff;
			size_t y = dy + yoff;
			*(pixel_ptr + x + (y * g_frame_buffer->ppsl)) = color;
		}
	}
}

void shell_clear() {
	memset_byte(g_frame_buffer->base, 0, g_frame_buffer->size);
}