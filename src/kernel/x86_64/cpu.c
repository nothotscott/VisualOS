/*
 * File:		cpu.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "memory/paging.h"
#include "memory/pageframe.h"
#include "io.h"
#include "gdt.h"
#include "idt.h"
#include "cpu.h"


static struct CPUContext s_cpu_bsp;
static struct IDTBlock s_shared_idt __attribute__((aligned (8)));

void cpu_create_blocks(struct CPUContext* cpu_context) {
	size_t gdt_pages = NEAREST_PAGE(sizeof(struct GDTBlock));
	//size_t idt_pages = NEAREST_PAGE(sizeof(struct IDTBlock));
	struct GDTBlock* gdt_block = pageframe_request_pages(gdt_pages);
	//struct IDTBlock* idt_block = pageframe_request_pages(idt_pages);
	struct IDTBlock* idt_block = &s_shared_idt;
	paging_identity_map(gdt_block, sizeof(struct GDTBlock));
	paging_identity_map(idt_block, sizeof(struct IDTBlock));
	cpu_context->gdt_block = gdt_block;
	cpu_context->idt_block = idt_block;
}

struct CPUContext* cpu_get_bsp() {
	return &s_cpu_bsp;
}


void cpu_init(struct CPUContext* cpu_context) {
	cpu_create_blocks(cpu_context);
	// Setup GDT
	gdt_init(cpu_context->gdt_block);
	gdt_load(&cpu_context->gdt_block->gdt_descriptor);
	// Setup IDT
	idt_init(cpu_context->idt_block);
	idt_register_handlers();
	io_pic_remap();
	io_pic_mask();
	idt_load(&cpu_context->idt_block->idt_descriptor);
}