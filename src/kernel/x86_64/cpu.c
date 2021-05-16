/*
 * File:		cpu.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "log.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/pageframe.h"
#include "apic/local_apic.h"
#include "io.h"
#include "gdt.h"
#include "isr.h"
#include "idt.h"
#include "syscall.h"
#include "cpu.h"


static struct CPUContext s_cpu_bsp __attribute__((aligned(16)));
static struct IDTBlock s_shared_idt __attribute__((aligned(8)));
static bool s_idt_initialized = false;

void cpu_allocate(struct CPUContext* cpu_context) {
	// Blocks
	struct GDTBlock* gdt_block = pageframe_request_pages(NEAREST_PAGE(sizeof(struct GDTBlock)));
	struct IDTBlock* idt_block = &s_shared_idt;
	paging_identity_map(gdt_block, sizeof(struct GDTBlock));
	paging_identity_map(idt_block, sizeof(struct IDTBlock));
	cpu_context->gdt_block = gdt_block;
	cpu_context->idt_block = idt_block;
	// Stacks
	void* stack_irq = pageframe_request_pages(CPU_STACK_IRQ_PAGES);
	paging_identity_map(stack_irq, CPU_STACK_IRQ_PAGES * MEMORY_PAGE_SIZE);
	cpu_context->stack_irq = stack_irq + CPU_STACK_IRQ_PAGES * MEMORY_PAGE_SIZE;
}

struct CPUContext* cpu_get_bsp() {
	return &s_cpu_bsp;
}

#include "syscall.h"
extern void test_userspace();

void cpu_init(struct CPUContext* cpu_context) {
	cpu_allocate(cpu_context);
	// Setup GDT
	gdt_init(cpu_context->gdt_block);
	gdt_set_tss_ist(cpu_context->gdt_block, 1, cpu_context->stack_irq);
	gdt_load(&cpu_context->gdt_block->gdt_descriptor);
	// Setup IDT
	if(s_idt_initialized == false) {
		idt_init(cpu_context->idt_block);
		s_idt_initialized = true;
	}
	idt_load(&cpu_context->idt_block->idt_descriptor);
	// Setup miscellaneous features
	cpu_enable_features();
	//paging_setup_pat();
	syscall_enable();
	log_default("Processor %d successfully initialized\n", cpu_context->local_apic_id);

	void* userspace_stack = pageframe_request();
	paging_identity_map(userspace_stack, MEMORY_PAGE_SIZE);
	paging_set_userspace_access(test_userspace, true);
	paging_set_userspace_access(userspace_stack, true);
	syscall_goto_userspace(test_userspace, userspace_stack + MEMORY_PAGE_SIZE);
}