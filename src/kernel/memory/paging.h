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


/*enum PageDirectoryEntry {
	PAGE_PRESENT		= 1 << 0,	// Page is in RAM and MMU can access it
	PAGE_WRITABLE		= 1 << 1,	// Page can be written to
	PAGE_USER			= 1 << 2,	// Page can be accessed by userland
	PAGE_WRITETHROUGH	= 1 << 3,	// Write-through cache (else write-back)
	PAGE_NOCACHE		= 1 << 4,	// Disable cache
	PAGE_ACCESSED		= 1 << 5,	// The page has previously been accessed
	PAGE_LARGERPAGES	= 1 << 7,	// Page points to a page that describes a page
	PAGE_OSAVAIL1		= 1 << 9,	// Undefined OS-specific 1
	PAGE_OSAVAIL2		= 1 << 10,	// Undefined OS-specific 2
	PAGE_OSAVAIL3		= 1 << 11,	// Undefined OS-specific 3
	PAGE_ADDRESS		= 0xfffffffffffff << 12	// Final address
};*/

struct PageDirectoryEntry {
	bool	present			: 1;	// Page is in RAM and MMU can access it
	bool	writable		: 1;	// Page can be written to
	bool	userland		: 1;	// Page can be accessed by userland
	bool	writethrough	: 1;	// Write-through cache (else write-back)
	bool	nocache			: 1;	// Disable cache
	bool	accessed		: 1;	// The page has previously been accessed
	bool	ignore0			: 1;	
	bool	largerpages		: 1;	// Page points to a page that describes a page
	bool	ignore1			: 1;	
	byte	os_available	: 3;	// Undefined OS-specific 1
	ulong	address			: 52;	// Final address
};


struct PageTable {
	struct PageDirectoryEntry entries[MEMORY_PAGE_ENTRY_SIZE];
} __attribute__((aligned(MEMORY_PAGE_SIZE)));

// x86_64 multi-level addressing
struct PageLevelIndexes {
	ushort	L4_i;	// Index to the page directory pointer table
	ushort	L3_i;	// Index to the page directory table
	ushort	L2_i;	// Index to the page table
	ushort	L1_i;	// Index to the page
};


// Global page table level 4 pointer
extern struct PageTable* g_pagetable_l4;


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

// Breaks the virtual [address] into its indexes and puts it in [out]
void paging_get_indexes(void* address, struct PageLevelIndexes* out);

// Maps [virtual_address] to virtual_address using [pagetable_l4]
void paging_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address);