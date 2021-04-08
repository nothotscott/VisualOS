/*
 * File:		paging.h
 * Description:	Describes the 4-level page mapping
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define MEMORY_PAGE_SIZE		4096
#define	MEMORY_PAGE_ENTRY_SIZE	MEMORY_PAGE_SIZE/sizeof(long)


typedef uint64_t	page_directory_entry_t;


enum PageDirectoryFlagBit {
	PAGE_PRESENT		= 0,	// Page is in RAM and MMU can access it
	PAGE_WRITABLE		= 1,	// Page can be written to
	PAGE_USER			= 2,	// Page can be accessed by userland
	PAGE_WRITETHROUGH	= 3,	// Write-through cache (else write-back)
	PAGE_NOCACHE		= 4,	// Disable cache
	PAGE_ACCESSED		= 5,	// The page has previously been accessed
	PAGE_LARGERPAGES	= 7,	// Page points to a page that describes a page
	PAGE_OSAVAIL1		= 9,	// Undefined OS-specific 1
	PAGE_OSAVAIL2		= 10,	// Undefined OS-specific 2
	PAGE_OSAVAIL3		= 11,	// Undefined OS-specific 3
	PAGE_NOEXECUTE		= 63	// Disabe execution, only if supported
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


// *** Miscellaneous functions  *** //

// Breaks the virtual [address] into its indexes and puts it in [out]
void paging_get_indexes(void* address, struct PageLevelIndexes* out);

// Returns the [entry] with the [address] applied to it
page_directory_entry_t paging_set_entry_address(page_directory_entry_t entry, void* address);

// Returns the address component of [entry]
void* paging_get_entry_address(page_directory_entry_t entry);

// Gets the global page table level 4 pointer
struct PageTable* paging_get_pagetable_l4();

// *** Class functions  *** //

// Initializes paging by creating the page table level 4 and sets the 
// initial mapping between virtual and physical memory.
// Must call pageframe_init before calling
void paging_init();

// Maps 1 to 1 a virtual address to a physical address that's
// [size] bytes, aligned to the nearest page size
// Only use for previously established pages in the kernel
void paging_identity_map(void* address, size_t size);
void paging_identity_map_page(void* address);

// Loads the paging information into the appropriate control register
void paging_load();

// Maps [virtual_address] to [physical_address]
void paging_map(void* virtual_address, void* physical_address);

// Donates a page to userspace at [virtual_address]
void paging_donate_to_userspace(void* virtual_address);