/*
 * File:		shell.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "shell.h"


struct FrameBuffer* g_shell_frame_buffer;


void shell_init(struct FrameBuffer* frame_buffer){
	g_shell_frame_buffer = frame_buffer;
}

void shell_draw_tga(struct TGAImage* img, uint32_t xoff, uint32_t yoff){
	uint32_t* pixel_ptr = (uint32_t*)g_shell_frame_buffer->base;
	uint32_t* img_ptr = (uint32_t*)img->buffer;
	uint32_t height = img->header_ptr->height;
	uint32_t width = img->header_ptr->width;
	for(size_t dy = 0; dy < height; dy++){
		for(size_t dx = 0; dx < width; dx++){
			size_t offset = dx + (height * dy);
			uint32_t color = *(img_ptr + offset);
			size_t x = dx + xoff;
			size_t y = dy + yoff;
			*(pixel_ptr + x + (y * g_shell_frame_buffer->ppsl)) = color;
		}
	}
}

void shell_clear() {
	memset(g_shell_frame_buffer->base, 0, g_shell_frame_buffer->size);
}