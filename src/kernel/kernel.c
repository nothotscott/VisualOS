/*
 * File:		kernel.c
 * Desciption:	Entry point of the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include "shell/shell.h"
#include "shell/tools.h"

// Global interface
struct UefiKernelInterface* interface;

const char* poop = "this is some poop";

void setup_shell(){
	struct FrameBuffer* frame_buffer = interface->frame_buffer_ptr;
	struct PSF1Font* font = interface->font_ptr;
	struct TGAImage* img = interface->img_ptr;
	shell_init(frame_buffer, font);

	uint screen_width = frame_buffer->width;
	uint screen_height = frame_buffer->height;
	ushort img_width = img->header_ptr->width;
	ushort img_height = img->header_ptr->height;

	draw_tga(img, screen_width / 2 - img_width / 2, 0);
	set_cursor(0, img_height);
}

void _start(struct UefiKernelInterface* _interface){
	interface = _interface;
	setup_shell();

	print("This is a test of the kernel printing to the screen using a custom font. \nAnd this is a new line\n", SHELL_COLOR_WHITE);
	print_newline();
	print_memory((void*)poop, 128, SHELL_COLOR_WHITE);

	while(1);
}

