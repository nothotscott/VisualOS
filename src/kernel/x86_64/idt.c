/*
 * File:		idt.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "gdt.h"
#include "interrupt.h"
#include "interrupt_handlers.h"
#include "idt.h"


void (*g_isr_handlers[ISR_MAX])(struct InterruptStack*, size_t);


void idt_init(struct IDTBlock* idt_block) {
	size_t idt_total_size = IDT_SIZE * sizeof(struct IDTEntry);	// size of all IDT enteries in bytes
	memset(&idt_block->idt, 0, idt_total_size);
	idt_block->idt_descriptor.limit = idt_total_size - 1;
	idt_block->idt_descriptor.base = &idt_block->idt;
	// Sets isrs from assembly
	idt_set_isr(idt_block, 8, isr8, IDT_TYPE_GATE_TRAP);
	idt_set_isr(idt_block, 13, isr13, IDT_TYPE_GATE_TRAP);
	idt_set_isr(idt_block, 14, isr14, IDT_TYPE_GATE_TRAP);
	idt_set_isr(idt_block, 32, isr32, IDT_TYPE_GATE_INTERRUPT);
	idt_set_isr(idt_block, 33, isr33, IDT_TYPE_GATE_INTERRUPT);
}

void idt_set_isr(struct IDTBlock* idt_block, size_t index, void* isr_ptr, enum IDTGateType gate) {
	struct IDTEntry* idt = &idt_block->idt;
	idt[index].zero = 0;
	idt[index].offset_low = (uint16_t)(((uint64_t)isr_ptr & 0x000000000000ffff));
	idt[index].offset_mid = (uint16_t)(((uint64_t)isr_ptr & 0x00000000ffff0000) >> 16);
	idt[index].offset_high = (uint32_t)(((uint64_t)isr_ptr & 0xffffffff00000000) >> 32);
	idt[index].ist = 0;
	idt[index].selector = 1 * sizeof(struct GDTEntry);	// code segment at index 1
	idt[index].type_attr = IDT_TYPE_PRESENT << 7 | IDT_TYPE_PRIVILEGE << 5 | gate;
}

void idt_register_isr_handler(size_t num, void (*handler)(struct InterruptStack*, size_t)) {
	g_isr_handlers[num] = handler;
}

void idt_register_handlers() {
	idt_register_isr_handler(8, double_fault_handler);
	idt_register_isr_handler(13, general_protection_fault_handler);
	idt_register_isr_handler(14, paging_fault_handler);
	idt_register_isr_handler(32, pit_handler);
	idt_register_isr_handler(33, keyboard_handler);
}