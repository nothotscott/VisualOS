/*
 * File:		setup.c
 * Description:	Setup functions for initializing the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "shell/shell.h"
#include "shell/text.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "memory/paging.h"
#include "x86_64/io.h"
#include "x86_64/gdt.h"
#include "x86_64/idt.h"
#include "interrupts/dispatcher.h"


// From kernel.c
extern struct KernelEntryInterface* g_interface;


void setup_shell() {
	struct FrameBuffer* frame_buffer = g_interface->frame_buffer;
	struct PSF1Font* font = g_interface->font;
	struct TGAImage* img = g_interface->img;
	shell_init(frame_buffer);
	text_init(font);
	shell_clear();
	uint32_t screen_width = frame_buffer->width;
	uint32_t screen_height = frame_buffer->height;
	uint16_t img_width = img->header_ptr->width;
	uint16_t img_height = img->header_ptr->height;
	shell_draw_tga(img, screen_width / 2 - img_width / 2, 0);
	text_set_cursor(0, img_height);
}

void setup_memory() {
	struct FrameBuffer* frame_buffer = g_interface->frame_buffer;
	pageframe_init(g_interface->mem_map, g_interface->mem_map_size, g_interface->mem_map_descriptor_size);
	pageframe_reserve(frame_buffer->base, ROUND_UP(frame_buffer->size, MEMORY_PAGE_SIZE) / MEMORY_PAGE_SIZE);
	paging_init();
	paging_identity_map(frame_buffer->base, frame_buffer->size);
	paging_load();
}

void setup_gdt() {
	gdt_init();
	gdt_load();
}

void setup_interrupts() {
	idt_init();
	idt_register_isr_handler(8, double_fault_handler);
	idt_register_isr_handler(13, general_protection_fault_handler);
	idt_register_isr_handler(14, paging_fault_handler);
	idt_register_isr_handler(33, keyboard_handler);
	io_pic_remap();
	io_pic_mask();
	idt_load();
}

void setup() {
	setup_shell();
	setup_memory();
	setup_gdt();
	setup_interrupts();
}