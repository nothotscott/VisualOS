/*
 * File:		paging.h
 * Description:	Describes the 4-level page mapping
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "memory.h"

typedef uint64_t	page_directory_entry_t;

enum PageDirectoryFlagBit {
	PAGE_PRESENT		= 0,	// Page is in RAM and MMU can access it
	PAGE_WRITABLE		= 1,	// Page can be written to
	PAGE_USERSPACE		= 2,	// Page can be accessed by userspace
	PAGE_WRITETHROUGH	= 3,	// Write-through cache (else write-back)
	PAGE_NOCACHE		= 4,	// Disable cache
	PAGE_ACCESSED		= 5,	// The page has previously been accessed
	PAGE_LARGERPAGES	= 7,	// Page points to a page that describes a larger page
	PAGE_OSAVAIL1		= 9,	// Undefined OS-specific 1
	PAGE_OSAVAIL2		= 10,	// Undefined OS-specific 2
	PAGE_OSAVAIL3		= 11,	// Undefined OS-specific 3
	PAGE_NOEXECUTE		= 63	// Disabe execution, only if supported
};

enum PagePATMode {
	PAGE_PAT_UNCACHED			= 0,
	PAGE_PAT_WRITE_COMBINE		= 1,
	PAGE_PAT_WRITE_THROUGH		= 4,
	PAGE_PAT_WRITE_PROTECTED	= 5,
	PAGE_PAT_WRITE_BACK			= 6,	// default
	PAGE_PAT_UC_MINUS			= 7,	// UC, but can be overridden by MTRR
};

struct PageTable {
	uint64_t entries[MEMORY_PAGE_ENTRY_SIZE];
} __attribute__((aligned(MEMORY_PAGE_SIZE)));

// x86_64 multi-level addressing
struct PageLevelIndexes {
	uint16_t	L4_i;	// Index to the page directory pointer table
	uint16_t	L3_i;	// Index to the page directory table
	uint16_t	L2_i;	// Index to the page table
	uint16_t	L1_i;	// Index to the page
};


// *** Miscellaneous functions *** //

// Breaks the virtual [address] into its indexes and puts it in [out]
void paging_get_indexes(void* address, struct PageLevelIndexes* out);

// Returns the [entry] with the [address] applied to it
page_directory_entry_t paging_set_entry_address(page_directory_entry_t entry, void* address);

// Gets the kernel page table level 4 pointer
struct PageTable* paging_get_pagetable_l4();

// *** Assembly functions *** //

// Loads the paging information into the appropriate control register
void paging_load();

// Creates page attribute table
void paging_setup_pat();

// *** Class functions *** //

// Initializes paging by creating the page table level 4 and sets the 
// initial mapping between virtual and physical memory.
// Must call pageframe_init before calling
void paging_init();

// Maps [virtual_address] to [physical_address]
void paging_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address);

// Identity maps [address]. Is not write protected. Should only be used with MMIO.
void paging_identity_map(void* address, size_t pages);
void paging_identity_map_size(void* address, size_t size);

// Sets the page directory attributes of [virtual_address] for [pages] of [attribute] to [enabled]
void paging_set_attribute(struct PageTable* pagetable_l4, void* virtual_address, size_t pages, enum PageDirectoryFlagBit attribute, bool enabled);

// Sets [pages] (or [size] rounded up to the nearest page) at [virtual_address] to be writable
// Assumes the page has been mapped previously
void paging_set_writable(void* virtual_address, size_t pages);
void paging_set_writable_size(void* virtual_address, size_t size);