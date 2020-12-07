/*
 * File:		kernel.c
 * Desciption:	Entry point of the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include "bootloader.h"
#include "shell/shell.h"
#include "shell/tools.h"

// Global interface
struct KernelEntryInterface* interface;

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

void setup_memory(){
	uint num_enteries = interface->mem_map_size / interface->mem_map_descriptor_size;
	for(uint i = 0; i < num_enteries; i++){
		//struct MemoryDescriptor* descriptor = interface->mem_map + i;
		struct MemoryDescriptor* descriptor = (struct MemoryDescriptor*)((void*)interface->mem_map + (i * interface->mem_map_descriptor_size));
		print_newline();
		print((char*)memory_type_names[descriptor->type], SHELL_COLOR_YELLOW);
	}
}

void _start(struct KernelEntryInterface* _interface){
	interface = _interface;

	setup_shell();
	print("Shell setup\n", SHELL_COLOR_WHITE);
	setup_memory();
	//print("Memory setup\n", SHELL_COLOR_WHITE);

	while(1);
}

//print_memory((void*)*memory_type_names, 128, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);