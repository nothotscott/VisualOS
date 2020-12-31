/*
 * File:		setup.c
 * Description:	Setup functions for initializing the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include <memory.h>
#include "bootloader.h"
#include "shell/shell.h"
#include "shell/tools.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "memory/heap.h"
#include "cpu/gdt.h"
#include "io/idt.h"
#include "io/isr.h"
#include "io/pic.h"


// From kernel.c
extern struct KernelEntryInterface* g_interface;


void setup_shell() {
	struct FrameBuffer* frame_buffer = g_interface->frame_buffer;
	struct PSF1Font* font = g_interface->font;
	struct TGAImage* img = g_interface->img;
	shell_init(frame_buffer, font);
	shell_clear();
	uint_t screen_width = frame_buffer->width;
	uint_t screen_height = frame_buffer->height;
	ushort_t img_width = img->header_ptr->width;
	ushort_t img_height = img->header_ptr->height;
	shell_draw_tga(img, screen_width / 2 - img_width / 2, 0);
	shell_set_cursor(0, img_height);
}

void setup_memory() {
	struct FrameBuffer* frame_buffer = g_interface->frame_buffer;
	pageframe_init(g_interface->mem_map, g_interface->mem_map_size, g_interface->mem_map_descriptor_size);
	paging_init();
	paging_identity_map(frame_buffer->base, frame_buffer->size);
	pageframe_reserve(frame_buffer->base, ROUND_UP(frame_buffer->size, MEMORY_PAGE_SIZE) / MEMORY_PAGE_SIZE);
	paging_load();
	heap_init(g_pageframemap.buffer + g_pageframemap.size, memory_get_free());
}

void setup_gdt() {
	gdt_init();
	gdt_set_entry(1, true);		// code segment
	gdt_set_entry(2, false);	// data segment
	gdt_load();
}

void setup_interrupts() {
	idt_init();
	/*for(size_t i = 0; i < 256; i++){
		idt_set_isr(i, (ulong_t)isr_nothing, IDT_TYPE_GATE_INTERRUPT);
	}*/
	idt_set_isr(33, (ulong_t)isr1, IDT_TYPE_GATE_INTERRUPT);
	pic_remap();
	pic_mask();
	idt_load();
}

/*void setup_interrupts() {
	struct IDTDescriptor uefi_idt_descriptor;
	idt_get(&uefi_idt_descriptor);
	g_idt_descriptor = uefi_idt_descriptor;
	g_idt = uefi_idt_descriptor.base;

	idt_set_isr(0x21, (ulong_t)isr1, IDT_TYPE_GATE_INTERRUPT);

	pic_remap();
	pic_mask();
	asm("sti");
}*/

void setup() {
	// Don't do string to numbers until after setup_memory() is called
	setup_shell();
	print("Shell setup\n", SHELL_COLOR_FOREGROUND);
	print_newline();

	setup_memory();
	print("Total memory size:      ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong_t)g_memory_total_size / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Reserved memory:        ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong_t)g_memory_reserved_size / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Free memory:            ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong_t)memory_get_free() / 1024 / 1024), SHELL_COLOR_NUMBER); print(" MB\n", SHELL_COLOR_NUMBER);
	print("Page frame map address: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong_t)g_pageframemap.buffer), SHELL_COLOR_ADDRESS); print_newline();
	print_newline();

	paging_map(g_pagetable_l4, (void*)0x600000000, (void*)0x80000);
	ulong_t* asd = (ulong_t*)0x600000000;
	*asd = 26;
	print(string_str_from_int(*asd), SHELL_COLOR_NUMBER);
	print_newline();

	setup_gdt();
	print("Global descriptor table address: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong_t)g_gdt), SHELL_COLOR_ADDRESS); print_newline();


	// TODO make interrupts better
	setup_interrupts();
	print("Interrupt descriptor table address: ", SHELL_COLOR_FOREGROUND); print("0x", SHELL_COLOR_ADDRESS); print(string_str_from_ulong((ulong_t)g_idt), SHELL_COLOR_ADDRESS); print_newline();
	print_memory((void*)&g_idt_descriptor, 10, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);
	print_newline();
}

//print_memory((void*)pagemap.buffer, pagemap.size, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);