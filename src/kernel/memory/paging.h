/*
 * File:		paging.h
 * Description:	Describes the 4-level page mapping
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "memory.h"

typedef uint64_t				page_directory_entry_t;
typedef page_directory_entry_t	page_table_entry_t;

enum PageDirectoryFlagBit {
	PAGE_DIRECTORY_PRESENT		= 0,	// (P) Page is in RAM and MMU can access it
	PAGE_DIRECTORY_WRITABLE		= 1,	// (R) Page can be written to
	PAGE_DIRECTORY_USERSPACE	= 2,	// (U) Page can be accessed by userspace
	PAGE_DIRECTORY_WRITETHROUGH	= 3,	// (W/WT) Write-through cache (else write-back)
	PAGE_DIRECTORY_NOCACHE		= 4,	// (D/CD) Disable cache
	PAGE_DIRECTORY_ACCESSED		= 5,	// (A) The page has previously been accessed
	PAGE_DIRECTORY_LARGERPAGES	= 7,	// (S) Page points to a page that describes a 4Mb page
	PAGE_DIRECTORY_OS_AVAILABLE	= 11,	// OS-specific (3 bits)
	PAGE_DIRECTORY_NOEXECUTE	= 63	// Disabe execution, only if supported
};
enum PageTableFlagBit {
	PAGE_TABLE_PRESENT		= PAGE_DIRECTORY_PRESENT,
	PAGE_TABLE_WRITABLE		= PAGE_DIRECTORY_WRITABLE,
	PAGE_TABLE_USERSPACE	= PAGE_DIRECTORY_USERSPACE,
	PAGE_TABLE_WRITETHROUGH	= 3,							// (PWT) Page writethrough
	PAGE_TABLE_NOCACHE		= 4,							// (C/PCD) Page cache disable
	PAGE_TABLE_ACCESSED		= PAGE_DIRECTORY_ACCESSED,
	PAGE_TABLE_PAT			= 7,							// (PAT) Page attribute table
	PAGE_TABLE_GLOBAL		= 8,							// (G) Prevents the TLB from updating the address in its cache if CR3 is reset
	PAGE_TABLE_OS_AVAILABLE	= PAGE_DIRECTORY_OS_AVAILABLE,
	PAGE_TABLE_NOEXECUTE	= PAGE_DIRECTORY_NOEXECUTE
};

enum PagePATMode {
	PAGE_PAT_UNCACHED			= 0,	// Write combining and speculative accesses are not allowed
	PAGE_PAT_WRITE_COMBINE		= 1,	// Write combining is allowed & speculative reads are allowed
	PAGE_PAT_WRITE_THROUGH		= 4,	// Write hits update the cache and main memory
	PAGE_PAT_WRITE_PROTECTED	= 5,	// Write hits invalidate the cache line and update main memory
	PAGE_PAT_WRITE_BACK			= 6,	// Writes allocate to the modified state on a cache miss (default)
	PAGE_PAT_UC_MINUS			= 7,	// Uncached, but can be overridden by MTRR
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

// Loads the PAT (page attribute table) MSR and invalidates cache
void paging_load_pat();

// *** Class functions *** //

// Initializes paging by creating the page table level 4 and sets the 
// initial mapping between virtual and physical memory.
// Must call pageframe_init before calling
void paging_init();

// Initializes the PAT by preparing MMIO OS-preferred caches (does not load the PAT and this should be called after doing so)
void paging_init_pat();

// Maps [virtual_address] to [physical_address]
void paging_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address);

// Identity maps [address]. Is not write protected. Should only be used with MMIO.
void paging_identity_map(void* address, size_t pages);
void paging_identity_map_size(void* address, size_t size);

// Sets the page directory attributes of [virtual_address] for [pages] of [attribute] to [enabled]
// Also assumes [attribute] is the same for the page table attributes
void paging_set_attribute(struct PageTable* pagetable_l4, void* virtual_address, size_t pages, enum PageDirectoryFlagBit attribute, bool enabled);

// Sets the page table corresponding to [virtual_address] for [pages] to have a cache policy of [mode] 
void paging_set_cache(void* virtual_address, size_t pages, enum PagePATMode mode);
void paging_set_cache_size(void* virtual_address, size_t size, enum PagePATMode mode);

// Sets [pages] (or [size] rounded up to the nearest page) at [virtual_address] to be writable
// Assumes the page has been mapped previously
void paging_set_writable(void* virtual_address, size_t pages);
void paging_set_writable_size(void* virtual_address, size_t size);