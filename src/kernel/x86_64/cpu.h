/*
 * File:		cpu.h
 * Description:	Functions that should initialize either the BSP or APs
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "memory/memory.h"
#include "gdt.h"
#include "idt.h"

#define CPU_BSP_STACK_PAGES	1

#define CPU_STACK_PAGES_ISR		1
#define CPU_STACK_PAGES_IRQ		2
#define CPU_STACK_PAGES_TIMER	1

struct CPUContext {
	uint8_t				local_apic_id;
	uint8_t				reserved[3];
	uint64_t			rsp_userspace;	// do not touch, for assembly only
	struct GDTBlock*	gdt_block;
	struct IDTBlock*	idt_block;
	void*				stack_kernel;
	void*				stack_isr;
	void*				stack_irq;
	void*				stack_timer_begin;
	void*				stack_timer;
} __attribute__((packed));

// Global BSP stack
extern uint8_t g_bsp_stack[MEMORY_PAGE_SIZE * CPU_BSP_STACK_PAGES];


// Initializes the bootstrap processor
void cpu_init_bsp();
// Initializes the application processor
void cpu_init_ap(struct CPUContext* cpu_context, void* stack);

// Allocates block of memory for CPU components and sets the result in [context]
void cpu_allocate(struct CPUContext* cpu_context);

// Initializes the CPU components
void cpu_init(struct CPUContext* cpu_context);

// Enables Miscellaneous features for the calling processor
void cpu_enable_features();

// Gets the BSP context
struct CPUContext* cpu_get_bsp();