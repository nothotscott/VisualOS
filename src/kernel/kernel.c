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
#include "memory/memory.h"
#include "memory/heap.h"
#include "io/idt.h"

#include "memory/bitmap.h"

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
	memory_init(interface->mem_map, interface->mem_map_size, interface->mem_map_descriptor_size);
	heap_init(&memory_space_primary, 0);
}

void setup(){
	// Don't do string to numbers until after setup_memory() is called
	setup_shell();
	print("Shell setup\n", SHELL_COLOR_FOREGROUND);

	setup_memory();
	print("Total memory size: ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)memory_total_size / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Primary kernel memory region address: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong)memory_space_primary.base), SHELL_COLOR_ADDRESS); print_newline();
	print("Primary kernel memory region size:    ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)memory_space_primary.size / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print_newline();
	/*char* poop = string_str_from_uint((ulong)0x1234);
	print(string_str_from_uint((ulong)poop), SHELL_COLOR_NUMBER); print_newline();
	print_memory(poop - 24, 128 + 24, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);
	void* page2 = malloc(0x1000);
	print(string_str_from_uint((ulong)page2), SHELL_COLOR_NUMBER); print_newline();*/


}

void _start(struct KernelEntryInterface* _interface){
	interface = _interface;
	setup();

	//print(string_str_from_int(test(5)), SHELL_COLOR_NUMBER); print_newline();

	while(1);
}

//print_memory((void*)*memory_type_names, 128, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);