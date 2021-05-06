/*
 * File:		shell.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "module.h"
#include "framebuffer.h"
#include "bootloader.h"
#include "text.h"
#include "shell.h"


void shell_init() {
	struct Framebuffer* framebuffer = bootloader_get_info()->framebuffer;
	struct Module* image = bootloader_get_info()->image;
	struct ModuleTGAHeader* image_header = (struct ModuleTGAHeader*)image->header;
	uint32_t screen_width = framebuffer->width;
	uint32_t screen_height = framebuffer->height;
	uint16_t image_width = image_header->width;
	uint16_t image_height = image_header->height;
	shell_draw_tga(image, screen_width / 2 - image_width / 2, 0);
	text_set_cursor(0, image_height);
}

void shell_draw_tga(struct Module* image, uint32_t xoff, uint32_t yoff) {
	struct Framebuffer* framebuffer = bootloader_get_info()->framebuffer;
	struct ModuleTGAHeader* image_header = (struct ModuleTGAHeader*)image->header;
	uint32_t* image_ptr = (uint32_t*)image->body;
	uint32_t height = image_header->height;
	uint32_t width = image_header->width;
	for(size_t dy = 0; dy < height; dy++){
		for(size_t dx = 0; dx < width; dx++){
			size_t offset = dx + (height * dy);
			uint32_t color = *(image_ptr + offset);
			size_t x = dx + xoff;
			size_t y = dy + yoff;
			framebuffer_putpix32(framebuffer, x, y, color);
		}
	}
}

void shell_clear() {
	struct Framebuffer* framebuffer = bootloader_get_info()->framebuffer;
	memset(framebuffer->base, 0, framebuffer->pitch * framebuffer->height);
}