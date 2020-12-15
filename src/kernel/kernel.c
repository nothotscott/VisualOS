/*
 * File:		kernel.c
 * Description:	Entry point of the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include "bootloader.h"
#include "shell/shell.h"
#include "shell/tools.h"
#include "memory/paging.h"
#include "memory/heap.h"
#include "io/idt.h"

// Global interface
struct KernelEntryInterface* interface;

void setup_shell() {
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

void setup_memory() {
	init_paging(interface->mem_map, interface->mem_map_size, interface->mem_map_descriptor_size);
	heap_init(pagemap.buffer + pagemap.size, paging_get_free());
}

void setup() {
	// Don't do string to numbers until after setup_memory() is called
	setup_shell();
	print("Shell setup\n", SHELL_COLOR_FOREGROUND);

	setup_memory();
	print("Total memory size:   ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)memory_total_size / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Reserved memory:     ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)memory_reserved_size / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Free memory:         ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)paging_get_free() / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	
	print("Page map address:   ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong)pagemap.buffer), SHELL_COLOR_ADDRESS); print_newline();
	print("Heap start address: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong)pagemap.buffer + pagemap.size), SHELL_COLOR_ADDRESS); print_newline();
	
	print_newline();


}

void _start(struct KernelEntryInterface* _interface) {
	interface = _interface;
	setup();

	//print(string_str_from_int(test(5)), SHELL_COLOR_NUMBER); print_newline();

	while(1);
}

//print_memory((void*)*memory_type_names, 128, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);