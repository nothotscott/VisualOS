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
static struct Point cursor = {
	.x = 0,
	.y = 0
};
static struct FrameBuffer* frame_buffer;
static struct PSF1Font* font;

void shell_init(struct FrameBuffer* _frame_buffer, struct PSF1Font* _font){
	frame_buffer = _frame_buffer;
	font = _font;
}

void shell_set_cursor(uint_t x, uint_t y) {
	cursor.x = x;
	cursor.y = y;
}

void shell_draw_char(char chr, SHELL_COLOR color, uint_t xoff, uint_t yoff){
	uint_t* pixel_ptr = (uint_t*)frame_buffer->base_ptr;
	byte_t charsize = font->header_ptr->charsize;
	byte_t* font_ptr = font->glyph_buffer + (chr * charsize);	// map character to glyph "array"
	// Draw; dx is delta x and dy is delta y
	for(size_t dy = 0; dy < charsize; dy++){
		for(size_t dx = 0; dx < PSF1_WIDTH; dx++){
			if((*font_ptr & (0b10000000 >> dx)) > 0){	// bit is enabled along the dx'th position on the glyph row
				size_t x = dx + xoff;
				size_t y = dy + yoff;
				*(uint_t*)(pixel_ptr + x + (y * frame_buffer->ppsl)) = color;
			}
		}
		font_ptr++;
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
	shell_draw_char(chr, color, cursor.x, cursor.y);
	cursor.x += PSF1_WIDTH;
	// Check for overflow
	if(cursor.x < frame_buffer->width){
		return;
	}
	newline:
		cursor.x = 0;
		cursor.y += font->header_ptr->charsize;
}
void print_newline(){
	print_char('\n', NULL);
}

void shell_draw_tga(struct TGAImage* img, uint_t xoff, uint_t yoff){
	uint_t* pixel_ptr = (uint_t*)frame_buffer->base_ptr;
	uint_t* img_ptr = (uint_t*)img->buffer;
	uint_t height = img->header_ptr->height;
	uint_t width = img->header_ptr->width;
	for(size_t dy = 0; dy < height; dy++){
		for(size_t dx = 0; dx < width; dx++){
			size_t offset = dx + (height * dy);
			uint_t color = *(img_ptr + offset);
			size_t x = dx + xoff;
			size_t y = dy + yoff;
			*(uint_t*)(pixel_ptr + x + (y * frame_buffer->ppsl)) = color;
		}
	}
}

void shell_clear() {
	memset_byte(frame_buffer->base_ptr, 0, frame_buffer->size);
}