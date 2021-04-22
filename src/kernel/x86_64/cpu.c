/*
 * File:		cpu.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "memory/paging.h"
#include "memory/pageframe.h"
#include "gdt.h"
#include "cpu.h"


static struct CPUContext s_cpu_bsp;


void cpu_create_gdt_block(struct CPUContext* cpu_context) {
	size_t pages = NEAREST_PAGE(sizeof(struct GDTBlock));
	struct GDTBlock* gdt_block = pageframe_request_pages(pages);
	paging_identity_map(gdt_block, sizeof(struct GDTBlock));
	cpu_context->gdt_block = gdt_block;
}

struct CPUContext* cpu_get_bsp() {
	return &s_cpu_bsp;
}