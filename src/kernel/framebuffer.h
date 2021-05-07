/*
 * File:		framebuffer.h
 * Description:	Simply describes framebuffer structures and inline functions
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

typedef uint16_t	framebuffer_dim_t;

struct Framebuffer {
	void*				base;
	framebuffer_dim_t	width;
	framebuffer_dim_t	height;
	framebuffer_dim_t	pitch;	// bytes in each row
	framebuffer_dim_t	bpp;	// bits per pixel
};


// Puts [pix] in [framebuffer] at [x],[y]
__attribute__((always_inline)) inline void framebuffer_putpix32(struct Framebuffer* framebuffer, framebuffer_dim_t x, framebuffer_dim_t y, uint32_t pix) {
	((uint32_t*)framebuffer->base)[y * framebuffer->width + x] = pix;
}