/*
 * File:		gdt.h
 * Description:	x86_64 global descriptor table
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define GDT_SIZE	5	// the null segment + code segment + data segment + TSS descriptor is the size of 2 GDT entries

#define GDT_TSS_STACK_SIZE		4096
#define GDT_TSS_RING_STACKS_NUM	3
#define GDT_TSS_IST_STACKS_NUM	7


struct GDTEntry {
	uint16_t	limit_low;	// Limit (lower 16 bits)
	uint16_t	base_low;	// Base (lower 16 bits)
	uint8_t		base_mid;	// Base (next 8 bits)
	uint8_t		access;		// present(1), privilege(3), type(1), executable(1), conforming(1), readable(1), access(0)
	uint8_t		flags;		// granularity(1), size(1), long-mode(1), limit(5)
	uint8_t		base_high;	// Base (highest 8 bits)
} __attribute__((packed));

struct GDTDescriptor {
	uint16_t			size;	// max size of 65 bit gdt, minus 1
	struct GDTEntry*	offset;	// linear address of the table
} __attribute__((packed));

struct TSS {
	uint32_t	reserved0;
	uint32_t	rsp0_low, rsp0_high;
	uint32_t	rsp1_low, rsp1_high;
	uint32_t	rsp2_low, rsp2_high;
	uint64_t	reserved1;
	uint32_t	ist1_low, ist1_high;
	uint32_t	ist2_low, ist2_high;
	uint32_t	ist3_low, ist3_high;
	uint32_t	ist4_low, ist4_high;
	uint32_t	ist5_low, ist5_high;
	uint32_t	ist6_low, ist6_high;
	uint32_t	ist7_low, ist7_high;
	uint64_t	reserved2;
	uint16_t	reserved3;
	uint16_t	io_map_base;
} __attribute__((packed));

struct TSSDescriptor {
	uint16_t	limit_low;	// Segment limit (lower 16 bits)
	uint16_t	base_low;	// Base (lower 16 bits)
	uint8_t		base_mid;	// Base (next 8 bits)
	uint8_t		access;		// present(1), ring(2), reserved(1) type(4)
	uint8_t		flags;		// granularity(1), reserved(2), available(1), limit_high(4)
	uint8_t		base_mid2;	// Base (next 8 bits)
	uint32_t	base_high;	// Base (highest 32 bits)
	uint32_t	reserved;
} __attribute__((packed));

struct StackTable {
	uint8_t		data[GDT_TSS_STACK_SIZE];
};


// Global idt entry pointer
extern struct GDTEntry* g_gdt;
// Global gdt descriptor
extern struct GDTDescriptor g_gdt_descriptor;
// Global TSS pointer
extern struct TSS* g_tss;
// Global array of pointers to the ring stacks
extern struct StackTable* g_ring_stacks[];
// Global array of pointers to the ist stacks
extern struct StackTable* g_ist_stacks[];



// Sets the global gdt entry pointer and global gdt descriptor
void gdt_init();

// Sets the gdt entry at [index] to [is_code]
void gdt_set_entry(size_t index, bool is_code);

// Sets up the task state segment at [index]
void gdt_set_tss(size_t index);

// Loads the global gdt, this will also clear interrupts
void gdt_load();