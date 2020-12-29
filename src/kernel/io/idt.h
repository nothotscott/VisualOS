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

#define IDT_SELECTOR_INGATE		0x8e
#define IDT_SELECTOR_TRAPGATE	0x08
#define IDT_SELECTOR_DEFAULT	IDT_SELECTOR_TRAPGATE

#define IDT_TYPES_ATTR_DEFAULT	0x8e


struct IDTEntry {
	ushort_t	offset_low;
	ushort_t	selector;
	byte_t	ist;
	byte_t	types_attr;
	ushort_t	offset_mid;
	uint_t	offset_high;
	uint_t	zero;
};

struct IDTDescriptor {
	ushort_t			limit;	// max size of 64 bit idt
	struct IDTEntry*	base;	// where the IDT is
} __attribute__((packed));


// Global idt entry pointer
extern struct IDTEntry* g_idt;
//extern struct IDTEntry g_idt[IDT_SIZE];

// Global idt descriptor
extern struct IDTDescriptor g_idt_descriptor;



// Sets the global idt entry pointer and global idt descriptor
void idt_init();

// Sets the global idt entry plus offset to the [isr_ptr]
void idt_set_isr(size_t offset, ulong_t isr_ptr);

// Loads the global idt descriptor
extern void idt_load();

// Gets the idt and stores it in [location]
extern void idt_get(void* location);