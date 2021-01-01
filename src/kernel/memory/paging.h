/*
 * File:		paging.h
 * Description:	Describes the 4-level page mapping
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define MEMORY_PAGE_SIZE		4096
#define	MEMORY_PAGE_ENTRY_SIZE	MEMORY_PAGE_SIZE/sizeof(long)


typedef ulong_t	page_directory_entry_t;


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
	ulong_t entries[MEMORY_PAGE_ENTRY_SIZE];
} __attribute__((aligned(MEMORY_PAGE_SIZE)));

// x86_64 multi-level addressing
struct PageLevelIndexes {
	ushort_t	L4_i;	// Index to the page directory pointer table
	ushort_t	L3_i;	// Index to the page directory table
	ushort_t	L2_i;	// Index to the page table
	ushort_t	L1_i;	// Index to the page
};


// Global page table level 4 pointer
extern struct PageTable* g_pagetable_l4;


// *** Miscellaneous functions  *** //

// Breaks the virtual [address] into its indexes and puts it in [out]
void paging_get_indexes(void* address, struct PageLevelIndexes* out);

// Returns the [entry] with the [address] applied to it
page_directory_entry_t paging_set_entry_address(page_directory_entry_t entry, void* address);

// Returns the address component of [entry]
void* paging_get_entry_address(page_directory_entry_t entry);


// *** Class functions  *** //

// Initializes paging by creating the page table level 4 and sets the 
// initial mapping between virtual and physical memory.
// Must call pageframe_init before calling
void paging_init();

// Maps 1 to 1 a virtual address to a physical address that's
// [size] bytes, aligned to the nearest page size
// Only use for previously established pages in the kernel
void paging_identity_map(void* address, size_t size);

// Loads the paging information into the appropriate control register
void paging_load();

// Maps [virtual_address] to virtual_address using [pagetable_l4]
void paging_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address);

// Handles page fault exceptions with the [stack]
void paging_fault_handler(struct InterruptStack* stack);