/*
 * File:		shell.h
 * Description:	Graphical shell in kernel space
 * *****************************************************************************
 * Copyright 2020-201 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "module.h"


// Initializes shell by drawing the bootloader image and setting the cursor
void shell_init();

// Draws TGA [image] at [xoff]set and [yoff]set
void shell_draw_tga(struct Module* image, uint32_t xoff, uint32_t yoff);

// Clears the framebuffer
void shell_clear();