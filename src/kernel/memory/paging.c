/*
 * File:		paging.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <memory.h>
#include "pageframe.h"
#include "paging.h"


void page_get_indexes(void* address, struct PageLevelIndexes* out) {
	ulong address64 = (ulong)address;
	address64 >>= 12;	// shift off offset
	// Get 9 LSBs and bit shift off for each of the 4 levels
	out->L4_i = address64 & 0x01ff;
	address64 >>= 9;
	out->L3_i = address64 & 0x01ff;
	address64 >>= 9;
	out->L2_i = address64 & 0x01ff;
	address64 >>= 9;
	out->L1_i = address64 & 0x01ff;
}

void page_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address) {
	enum PageDirectoryEntry entry;
	struct PageTable* pagetable;
	// Break virtual address into it's level index components
	struct PageLevelIndexes indexes;
	page_get_indexes(virtual_address, &indexes);
	// Index page map level 4 to get page directory pointer table
	entry = pagetable_l4->entries[indexes.L4_i];
	if(!(entry & PAGE_PRESENT)) {	// create page directory pointer table if it's not present
		pagetable = (struct PageTable*)page_request();
		memset(pagetable, 0, MEMORY_PAGE_SIZE);
	}
}