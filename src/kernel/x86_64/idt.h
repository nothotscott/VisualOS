/*
 * File:		idt.h
 * Description:	Provides functions for the Interrupt Descriptor Table
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define IDT_SIZE	256

#define IDT_TYPE_PRESENT	0b1
#define IDT_TYPE_PRIVILEGE	0b00

#define IDT_IST_ISR	0
#define IDT_IST_IRQ	1

enum IDTGateType {
	IDT_TYPE_GATE_INTERRUPT	= 0b1110,
	IDT_TYPE_GATE_TRAP		= 0b1111,
	IDT_TYPE_GATE_TASK		= 0b0101
};

struct IDTEntry {
	uint16_t	offset_low;
	uint16_t	selector;
	uint8_t		ist;
	uint8_t		type_attr;
	uint16_t	offset_mid;
	uint32_t	offset_high;
	uint32_t	zero;
} __attribute__((packed));

struct IDTDescriptor {
	uint16_t			limit;	// max size of 64 bit idt, minus 1
	struct IDTEntry*	base;	// where the IDT is
} __attribute__((packed));

struct IDTBlock {
	struct IDTDescriptor	idt_descriptor;
	struct IDTEntry			idt[IDT_SIZE];
};

// Initializes the idt and idt descriptors in [idt_block]
void idt_init(struct IDTBlock* idt_block);

// Sets the [idt_block] idt entry at [index] to the [isr_ptr]
// This should be done during idt_load
void idt_set_isr(struct IDTBlock* idt_block, size_t index, void* isr_ptr, enum IDTGateType gate, uint8_t ist);


// *** From idt.asm *** //


// Loads the [idt_descriptor]
void idt_load(struct IDTDescriptor* idt_descriptor);