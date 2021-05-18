/*
 * File:		idt.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "gdt.h"
#include "isr.h"
#include "idt.h"

#define IDT_SET_ISR(num)		idt_set_isr(idt_block, num, isr##num, IDT_TYPE_GATE_TRAP, IDT_IST_ISR)
#define IDT_SET_IRQ(num)		idt_set_isr(idt_block, num, isr##num, IDT_TYPE_GATE_INTERRUPT, IDT_IST_IRQ)
#define IDT_SET_LVT(num,ist)	idt_set_isr(idt_block, num, isr##num, IDT_TYPE_GATE_INTERRUPT, ist)

void idt_init(struct IDTBlock* idt_block) {
	size_t idt_total_size = IDT_SIZE * sizeof(struct IDTEntry);	// size of all IDT enteries in bytes
	memset(&idt_block->idt, 0, idt_total_size);
	idt_block->idt_descriptor.limit = idt_total_size - 1;
	idt_block->idt_descriptor.base = idt_block->idt;
	// Internal ISRs
	IDT_SET_ISR(8);
	IDT_SET_ISR(10);
	IDT_SET_ISR(11);
	IDT_SET_ISR(13);
	IDT_SET_ISR(14);
	// IRQs
	IDT_SET_IRQ(32);
	IDT_SET_IRQ(33);
	IDT_SET_IRQ(35);
	IDT_SET_IRQ(36);
	IDT_SET_IRQ(37);
	IDT_SET_IRQ(38);
	IDT_SET_IRQ(39);
	IDT_SET_IRQ(40);
	IDT_SET_IRQ(41);
	IDT_SET_IRQ(42);
	IDT_SET_IRQ(43);
	IDT_SET_IRQ(44);
	IDT_SET_IRQ(45);
	IDT_SET_IRQ(46);
	IDT_SET_IRQ(47);
	// Custom LVT
	IDT_SET_LVT(48, IDT_IST_TIMER);
}

void idt_set_isr(struct IDTBlock* idt_block, size_t index, void* isr_ptr, enum IDTGateType gate, uint8_t ist) {
	struct IDTEntry* idt = idt_block->idt;
	idt[index] = (struct IDTEntry){
		.zero = 0,
		.offset_low = (uint16_t)(((uint64_t)isr_ptr & 0x000000000000ffff)),
		.offset_mid = (uint16_t)(((uint64_t)isr_ptr & 0x00000000ffff0000) >> 16),
		.offset_high = (uint32_t)(((uint64_t)isr_ptr & 0xffffffff00000000) >> 32),
		.ist = ist & 0x7,
		.selector = 1 * sizeof(struct GDTEntry),									// code segment at index 1
		.type_attr = IDT_TYPE_PRESENT << 7 | IDT_TYPE_PRIVILEGE << 5 | gate
	};
}