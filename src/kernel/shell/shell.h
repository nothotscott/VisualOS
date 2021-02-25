/*
 * File:		shell.h
 * Description:	Graphical shell in kernel space
 * *****************************************************************************
 * Copyright 2020-201 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "bootloader.h"


extern struct FrameBuffer* g_shell_frame_buffer;


// Initalize shell with the [frame_buffer]
void shell_init(struct FrameBuffer* frame_buffer);

// Draws TGA img at [xoffset] and [yoffset]
void shell_draw_tga(struct TGAImage* img, uint32_t xoff, uint32_t yoff);

// Clears the framebuffer
void shell_clear();