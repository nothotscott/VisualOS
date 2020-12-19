/*
 * File:		setup.c
 * Description:	Setup functions for initializing the kernel
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
#include "io/isr.h"
#include "io/pic.h"


// From kernel.c
extern struct KernelEntryInterface* g_interface;


void setup_shell() {
	struct FrameBuffer* frame_buffer = g_interface->frame_buffer_ptr;
	struct PSF1Font* font = g_interface->font_ptr;
	struct TGAImage* img = g_interface->img_ptr;
	shell_init(frame_buffer, font);

	uint screen_width = frame_buffer->width;
	uint screen_height = frame_buffer->height;
	ushort img_width = img->header_ptr->width;
	ushort img_height = img->header_ptr->height;

	draw_tga(img, screen_width / 2 - img_width / 2, 0);
	set_cursor(0, img_height);
}

void setup_memory() {
	init_paging(g_interface->mem_map, g_interface->mem_map_size, g_interface->mem_map_descriptor_size);
	heap_init(g_pagemap.buffer + g_pagemap.size, paging_get_free());
}

void setup_interrupts() {
	idt_init();
	idt_set_isr(1, (ulong)isr1);
	pic_remap();
	pic_mask();
	idt_load();
}

void setup() {
	// Don't do string to numbers until after setup_memory() is called
	setup_shell();
	print("Shell setup\n", SHELL_COLOR_FOREGROUND);
	print_newline();

	setup_memory();
	print("Total memory size:   ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)g_memory_total_size / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Reserved memory:     ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)g_memory_reserved_size / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Free memory:         ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)paging_get_free() / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Page map address:   ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong)g_pagemap.buffer), SHELL_COLOR_ADDRESS); print_newline();
	print("Heap start address: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong)g_pagemap.buffer + g_pagemap.size), SHELL_COLOR_ADDRESS); print_newline();
	print_newline();

	setup_interrupts();
	print("Interrupt descriptor table address: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong)g_idt), SHELL_COLOR_ADDRESS); print_newline();
	print_newline();

	print_memory((void*)&g_idt_descriptor, sizeof(struct IDTDescriptor), SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);
}

//print_memory((void*)pagemap.buffer, pagemap.size, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);