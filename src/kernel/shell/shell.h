/*
 * File:		shell.h
 * Description:	Graphical shell in kernel space
 * *****************************************************************************
 * Copyright 2020-201 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
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