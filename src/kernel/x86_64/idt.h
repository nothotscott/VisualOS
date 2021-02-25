/*
 * File:		idt.h
 * Description:	Provides functions for the Interrupt Descriptor Table
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "interrupt.h"

#define IDT_SIZE	256
#define	ISR_MAX		IDT_SIZE

#define IDT_TYPE_PRESENT	0b1
#define IDT_TYPE_PRIVILEGE	0b00

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

// Global idt entry pointer
extern struct IDTEntry* g_idt;

// Global idt descriptor
extern struct IDTDescriptor g_idt_descriptor;

// Global isr handler table
extern void (*g_isr_handlers[ISR_MAX])(struct InterruptStack*, size_t);


// Sets the global idt entry pointer and global idt descriptor
void idt_init();

// Sets the global idt entry at [index] to the [isr_ptr]
// This should be done during idt_load
void idt_set_isr(size_t index, void* isr_ptr, enum IDTGateType gate);

// Loads the global idt descriptor
extern void idt_load();

// Adds isr [handler] for interrupt [num]
void idt_register_isr_handler(size_t num, void (*handler)(struct InterruptStack*, size_t));


// Interrupt service routines defined in idt.asm
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr30();
extern void isr33();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr39();