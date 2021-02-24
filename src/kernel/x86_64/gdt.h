/*
 * File:		gdt.h
 * Description:	x86_64 global descriptor table
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define GDT_SIZE	8	// the null segment + 4 code / data segments + TSS descriptor is the size of 2 GDT entries

#define GDT_TSS_STACK_SIZE		4096
#define GDT_TSS_RING_STACKS_NUM	3
#define GDT_TSS_IST_STACKS_NUM	7


struct GDTEntry {
	uint16_t	limit_low;	// Limit (lower 16 bits)
	uint16_t	base_low;	// Base (lower 16 bits)
	uint8_t		base_mid;	// Base (next 8 bits)
	uint8_t		access;		// present(1), ring(2), type(1), executable(1), conforming(1), writable(1), accessed(1)
	uint8_t		flags;		// granularity(1), protected-mode(1), long-mode(1), available(1), limit_high(4)
	uint8_t		base_high;	// Base (highest 8 bits)
} __attribute__((packed));

struct GDTDescriptor {
	uint16_t			size;	// max size of 65 bit gdt, minus 1
	struct GDTEntry*	offset;	// linear address of the table
} __attribute__((packed));

enum GDTAccess {
	GDT_ACCESS_WRITABLE		= 0b00000010,
	GDT_ACCESS_TYPE			= 0b00010000,
	GDT_ACCESS_PRESENT		= 0b10000000,
	GDT_ACCESS_EXECUTABLE	= 0b00001000,
	GDT_ACCESS_DPL			= 0b01100000,	// segment privilege level is ring 3
};

enum GDTFlags {
	GDT_FLAG_LONG_MODE			= 0b00100000,
	GDT_FLAG_PAGE_GRANULARITY	= 0b10000000,
};

struct TSS {
	uint32_t	reserved0;
	uint64_t	rsp0;	// ring stacks
	uint64_t	rsp1;
	uint64_t	rsp2;
	uint64_t	reserved1;
	uint64_t	ist1;	// interrupt stack tables
	uint64_t	ist2;
	uint64_t	ist3;
	uint64_t	ist4;
	uint64_t	ist5;
	uint64_t	ist6;
	uint64_t	ist7;
	uint64_t	reserved2;
	uint16_t	reserved3;
	uint16_t	io_map_base;
} __attribute__((packed));

struct TSSDescriptor {
	uint16_t	limit_low;	// Segment limit (lower 16 bits)
	uint16_t	base_low;	// Base (lower 16 bits)
	uint8_t		base_mid;	// Base (next 8 bits)
	uint8_t		access;		// present(1), ring(2), reserved(1), type(4)
	uint8_t		flags;		// granularity(1), reserved(2), available(1), limit_high(4)
	uint8_t		base_mid2;	// Base (next 8 bits)
	uint32_t	base_high;	// Base (highest 32 bits)
	uint32_t	reserved;
} __attribute__((packed));


// Global idt entry pointer
extern struct GDTEntry* g_gdt;
// Global gdt descriptor
extern struct GDTDescriptor g_gdt_descriptor;
// Global TSS pointer
extern struct TSS* g_tss;



// Sets the global gdt entry pointer and global gdt descriptor
void gdt_init();

// Sets the gdt entry at [index] with flags of [access] and [flags]
void gdt_set_entry(size_t index, enum GDTAccess access, enum GDTFlags flags);

// Sets up the task state segment at [index]
void gdt_set_tss(size_t index);

// Loads the global gdt, this will also clear interrupts
void gdt_load();

// Sets the kernel stack to [stack]
void gdt_set_ring0_stack(void* stack);

// Gets TSS ring0 stack
void* gdt_get_ring0_stack();