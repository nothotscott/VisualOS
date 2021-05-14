/*
 * File:		cpu.h
 * Description:	Functions that should initialize either the BSP or APs
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "gdt.h"
#include "idt.h"

#define CPU_STACK_IRQ_PAGES	2
#define CPU_STACK_LVT_PAGES	2

struct CPUContext {
	uint8_t				local_apic_id;
	struct GDTBlock*	gdt_block;
	struct IDTBlock*	idt_block;
	void*				stack_irq;
	void*				stack_lvt;
} __attribute__((packed));


// Initializes the bootstrap processor
void cpu_init_bsp();
// Initializes the application processor
void cpu_init_ap(struct CPUContext* cpu_context);

// Allocates block of memory for CPU components and sets the result in [context]
void cpu_allocate(struct CPUContext* cpu_context);

// Initializes the CPU components
void cpu_init(struct CPUContext* cpu_context);

// Enables Miscellaneous features for the calling processor
void cpu_enable_features();

// Getters
struct CPUContext* cpu_get_bsp();