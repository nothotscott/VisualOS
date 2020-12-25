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


enum PageDirectoryEntry {
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
};


struct PageTable {
	enum PageDirectoryEntry entries[MEMORY_PAGE_ENTRY_SIZE];
} __attribute__((aligned(MEMORY_PAGE_SIZE)));

// x86_64 multi-level addressing
struct PageLevelIndexes {
	ushort	L4_i;	// Index to the page directory pointer table
	ushort	L3_i;	// Index to the page directory table
	ushort	L2_i;	// Index to the page table
	ushort	L1_i;	// Index to the page
};


// Breaks the virtual [address] into its indexes and puts it in [out]
void page_get_indexes(void* address, struct PageLevelIndexes* out);

// Maps [virtual_address] to virtual_address using [pagetable_l4]
void page_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address);