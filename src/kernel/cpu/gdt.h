/*
 * File:		gdt.h
 * Description:	x86-64 global descriptor table
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define GDT_SIZE	3


struct GDTEntry {
	ushort_t	limit_low;	// Limit (lower 16 bits)
	ushort_t	base_low;	// Base (lower 16 bits)
	byte_t		base_mid;	// Base (next 8 bits)
	byte_t		access;		// present(1), privilege(3), type(1), executable(1), conforming(1), readable(1), access(0)
	byte_t		flags;		// granularity(1), size(1), long-mode(1), limit(5)
	byte_t		base_high;	// Base (highest 8 bits)
} __attribute__((packed));

struct GDTDescriptor {
	ushort_t			size;	// max size of 65 bit gdt, minus 1
	struct GDTEntry*	offset;	// linear address of the table
} __attribute__((packed));


// Global idt entry pointer
extern struct GDTEntry* g_gdt;

// Global gdt descriptor
extern struct GDTDescriptor g_gdt_descriptor;


// Sets the global gdt entry pointer and global gdt descriptor
void gdt_init();

// Sets the gdt entry at [index] to [is_code]
void gdt_set_entry(size_t index, bool is_code);

// Loads the global gdt, this will also clear interrupts
void gdt_load();