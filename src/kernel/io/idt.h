/*
 * File:		idt.h
 * Description:	Provides functions for the Interrupt Descriptor Table
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define IDT_SIZE	256

#define IDT_TYPE_PRESENT	0b1
#define IDT_TYPE_PRIVILEGE	0b00

enum IDTGateSelector {
	IDT_TYPE_GATE_INTERRUPT	= 0b1110,
	IDT_TYPE_GATE_TRAP		= 0b1111,
	IDT_TYPE_GATE_TASK		= 0b0101
};

struct IDTEntry {
	ushort_t	offset_low;
	ushort_t	selector;
	byte_t		ist;
	byte_t		type_attr;
	ushort_t	offset_mid;
	uint_t		offset_high;
	uint_t		zero;
} __attribute__((packed));

struct IDTDescriptor {
	ushort_t			limit;	// max size of 64 bit idt, minus 1
	struct IDTEntry*	base;	// where the IDT is
} __attribute__((packed));


// Global idt entry pointer
extern struct IDTEntry* g_idt;
//extern struct IDTEntry g_idt[IDT_SIZE];

// Global idt descriptor
extern struct IDTDescriptor g_idt_descriptor;



// Sets the global idt entry pointer and global idt descriptor
void idt_init();

// Sets the global idt entry at [index] to the [isr_ptr]
void idt_set_isr(size_t index, ulong_t isr_ptr, enum IDTGateSelector gate);

// Loads the global idt descriptor
extern void idt_load();

// Gets the idt and stores it in [location]
extern void idt_get(void* location);