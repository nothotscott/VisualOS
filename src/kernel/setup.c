/*
 * File:		setup.c
 * Description:	Setup functions for initializing the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "shell/shell.h"
#include "shell/text.h"
#include "debug/debug.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "memory/paging.h"
#include "x86_64/io.h"
#include "x86_64/gdt.h"
#include "x86_64/idt.h"
#include "x86_64/pit.h"
#include "x86_64/acpi.h"
#include "x86_64/pci.h"
#include "x86_64/apic/madt.h"
#include "x86_64/apic/apic.h"
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
	idt_register_handlers();
	io_pic_remap();
	io_pic_mask();
	pit_init();
	idt_load();
}

void setup_acpi() {
	struct RSDP2* rsdp = g_interface->rsdp;
	struct SDTHeader* xsdt = (struct SDTHeader*)rsdp->xsdt_base;
	struct MCFGHeader* mcfg = (struct MCFGHeader*)acpi_get_table(xsdt, "MCFG");
	struct MADTHeader* madt = (struct MADTHeader*)acpi_get_table(xsdt, "APIC");
	pci_init(mcfg);
	io_enable_apic();
	madt_init(madt);
	//pci_print();
}

void setup_apic() {
	apic_init();
	apic_start_smp();
}

void setup_syscall(){
	syscall_enable_sce();
}

extern void test_userspace();

void setup() {
	setup_shell();
	debug("Setup shell\n");
	setup_memory();
	debug("Setup memory\n");
	setup_gdt();
	debug("Setup gdt\n");
	setup_interrupts();
	debug("Setup interrupts\n");
	setup_acpi();
	debug("Setup ACPI/PCI\n");
	setup_apic();
	debug("Setup APIC/MP\n");
	setup_syscall();
	debug("Setup syscall\n");

	paging_donate_to_userspace(&test_userspace);
	void* userspace_stack = pageframe_request();
	paging_donate_to_userspace(userspace_stack);
	debug_options((struct DebugOptions){DEBUG_TYPE_INFO, true}, "Entering test_userspace\n");
	syscall_goto_userspace(&test_userspace, userspace_stack + 4096 - 8);
	debug_options((struct DebugOptions){DEBUG_TYPE_INFO, true}, "Back to kernel\n");
}