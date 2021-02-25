/*
 * File:		setup.c
 * Description:	Setup functions for initializing the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "shell/debug.h"
#include "shell/shell.h"
#include "shell/text.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "memory/paging.h"
#include "x86_64/io.h"
#include "x86_64/gdt.h"
#include "x86_64/idt.h"
#include "x86_64/interrupt_handlers.h"
#include "x86_64/syscall.h"


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

void setup_syscall(){
	syscall_enable_sce();
}

extern void test_userspace();

void setup() {
	setup_shell();
	debug_output("Setup shell\n");
	setup_memory();
	debug_output("Setup memory\n");
	setup_gdt();
	debug_output("Setup gdt\n");
	setup_interrupts();
	debug_output("Setup interrupts\n");
	setup_syscall();
	debug_output("Setup syscall\n");

	paging_donate_to_userspace(&test_userspace);
	void* userspace_stack = pageframe_request();
	paging_donate_to_userspace(userspace_stack);
	debug_output_info("Entering test_userspace\n", true);
	syscall_goto_userspace(&test_userspace, userspace_stack + 4096 - 8);
	debug_output_info("Back to kernel\n", true);
}