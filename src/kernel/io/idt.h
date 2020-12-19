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

#define IDT_DEFAULT_SELECTOR	0x08
#define IDT_DEFAULT_TYPES_ATTR	0x8E


struct IDTEntry {
	ushort	offset_low;
	ushort	selector;
	byte	ist;
	byte	types_attr;
	ushort	offset_mid;
	uint	offset_high;
	uint	zero;
} __attribute__((__packed__));

struct IDTDescriptor {
	ushort				limit;	// max size of 64 bit idt
	struct IDTEntry*	base;	// where the IDT is
} __attribute__((__packed__));


// Global idt entry pointer
struct IDTEntry* g_idt;
// Global idt descriptor
struct IDTDescriptor g_idt_descriptor;


// Sets the global idt entry pointer and global idt descriptor
void idt_init();

// Sets the global idt entry plus offset to the [isr_ptr]
void idt_set_isr(size_t offset, void* isr_ptr);

// Loads the global idt descriptor
extern void idt_load();