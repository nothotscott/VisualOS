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
	heap_init(&memory_space_primary);
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

byte test[20];

struct Bitmap bitmap = (struct Bitmap){
	.buffer = (byte*)&test,
	.size = 20
};

void _start(struct KernelEntryInterface* _interface){
	interface = _interface;
	setup();

	bitmap_set(&bitmap, 0, true);
	bitmap_set(&bitmap, 1, false);
	bitmap_set(&bitmap, 2, false);
	bitmap_set(&bitmap, 3, true);
	bitmap_set(&bitmap, 4, true);
	bitmap_set(&bitmap, 5, false);
	bitmap_set(&bitmap, 6, false);
	bitmap_set(&bitmap, 7, true);
	bitmap_set(&bitmap, 8, true);

	print_memory((void*)&test, 8, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);
	for(int i = 0; i < 20; i++){
		bool val = bitmap_get(&bitmap, i);
		print(val == true ? "true\n" : "false\n", SHELL_COLOR_FOREGROUND);
	}

	while(1);
}

//print_memory((void*)*memory_type_names, 128, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);