/*
 * File:		cpu.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
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


// Allocates a stack of [pages] size and turns the top of the stack (ready to be loaded into rsp)
// Will memset the stack to 0 if [clear] is true
static inline void* _cpu_allocate_stack(size_t pages, bool clear);

static struct CPUContext s_cpu_bsp __attribute__((aligned(16)));
static struct IDTBlock s_shared_idt __attribute__((aligned(8)));
static bool s_idt_initialized = false;

static inline void* _cpu_allocate_stack(size_t pages, bool clear) {
	void* stack = pageframe_request_pages(pages);
	paging_identity_map(stack, pages * MEMORY_PAGE_SIZE);
	if(clear){
		memset(stack, 0, pages * MEMORY_PAGE_SIZE);
	}
	return stack + pages * MEMORY_PAGE_SIZE;
}
void cpu_allocate(struct CPUContext* cpu_context) {
	// Blocks
	struct GDTBlock* gdt_block = pageframe_request_pages(NEAREST_PAGE(sizeof(struct GDTBlock)));
	struct IDTBlock* idt_block = &s_shared_idt;
	paging_identity_map(gdt_block, sizeof(struct GDTBlock));
	paging_identity_map(idt_block, sizeof(struct IDTBlock));
	cpu_context->gdt_block = gdt_block;
	cpu_context->idt_block = idt_block;
	// Stacks
	cpu_context->stack_irq = _cpu_allocate_stack(CPU_STACK_PAGES_IRQ, false);
	cpu_context->stack_timer = _cpu_allocate_stack(CPU_STACK_PAGES_TIMER, true);
	cpu_context->stack_timer -= 16;	// stack must be 16-byte aligned for some reason
	//*(uint64_t*)(cpu_context->stack_timer) = 0x123;
	log_default("stack: 0x%x\n", cpu_context->stack_timer);
}

struct CPUContext* cpu_get_bsp() {
	return &s_cpu_bsp;
}

#include "syscall.h"
extern void test_userspace();

void cpu_init(struct CPUContext* cpu_context) {
	cpu_allocate(cpu_context);
	bool is_bsp = cpu_context->local_apic_id == s_cpu_bsp.local_apic_id;
	// Setup GDT
	gdt_init(cpu_context->gdt_block);
	gdt_set_tss_ist(cpu_context->gdt_block, IDT_IST_IRQ, cpu_context->stack_irq);
	gdt_set_tss_ist(cpu_context->gdt_block, IDT_IST_TIMER, cpu_context->stack_timer);
	gdt_load(&cpu_context->gdt_block->gdt_descriptor);
	// Setup IDT
	if(s_idt_initialized == false) {
		idt_init(cpu_context->idt_block);
		s_idt_initialized = true;
	}
	idt_load(&cpu_context->idt_block->idt_descriptor, is_bsp);
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