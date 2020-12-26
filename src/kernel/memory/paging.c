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


struct PageTable* g_pagetable_l4;

#include <string.h>
#include "shell/shell.h"
#include "shell/tools.h"

void paging_init() {
	g_pagetable_l4 = (struct PageTable*)pageframe_request();
	memset_byte(g_pagetable_l4, 0, MEMORY_PAGE_SIZE);
	// Identity map (for now) each page
	// TODO better page mapping
	//paging_identity_map(0, g_memory_total_size);
	for(void* ptr = 0; (uintptr_t)ptr < MEMORY_PAGE_SIZE * 64; ptr += MEMORY_PAGE_SIZE) {
		paging_map(g_pagetable_l4, ptr, ptr);	// 1 to 1 mapping
		/*if((uintptr_t)ptr / MEMORY_PAGE_SIZE <= 64 + 50) {
			continue;
		}
		print("New page: ", SHELL_COLOR_FOREGROUND); print(string_str_from_int((ulong)pageframe_request()), SHELL_COLOR_ADDRESS); print_newline();*/
	}
}

void paging_identity_map(void* address, size_t size) {
	size_t mod = size % MEMORY_PAGE_SIZE;
	size -= mod;
	if(mod != 0){
		size += MEMORY_PAGE_SIZE;
	}
	for(void* ptr = address; (uintptr_t)ptr < (uintptr_t)address + size; ptr += MEMORY_PAGE_SIZE) {
		paging_map(g_pagetable_l4, ptr, ptr);	// 1 to 1 mapping
	}
}

void paging_get_indexes(void* address, struct PageLevelIndexes* out) {
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

int i = 0;
void paging_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address) {
	struct PageTable* pagetable_l3;
	struct PageTable* pagetable_l2;
	struct PageTable* pagetable_l1;
	struct PageDirectoryEntry entry;
	size_t index;
	// Break virtual address into it's level index components
	struct PageLevelIndexes indexes;
	paging_get_indexes(virtual_address, &indexes);
	// Index page table map level 4 to get the page directory pointer table (L3)
	index = indexes.L4_i;
	entry = pagetable_l4->entries[index];
	if(entry.present == false) {	// create page directory pointer table if it's not present
		pagetable_l3 = (struct PageTable*)pageframe_request();
		memset_byte(pagetable_l3, 0, MEMORY_PAGE_SIZE);
		entry.address = (ulong)pagetable_l3 >> 12;
		entry.present = true;
		entry.writable = true;
		pagetable_l4->entries[index] = entry;
	} else {
		pagetable_l3 = (struct PageTable*)((ulong)entry.address << 12);	// restore the address
	}
	// Index page table map level 3 to get the page directory table (L2)
	index = indexes.L3_i;
	entry = pagetable_l3->entries[index];
	if(entry.present == false) {
		pagetable_l2 = (struct PageTable*)pageframe_request(); // ??
		memset_byte(pagetable_l2, 0, MEMORY_PAGE_SIZE);
		entry.address = (ulong)pagetable_l2 >> 12;
		entry.present = true;
		entry.writable = true;
		pagetable_l3->entries[index] = entry;
	} else {
		pagetable_l2 = (struct PageTable*)((ulong)entry.address << 12);
	}
	// Index page table map level 2 to get the page table (L1)
	index = indexes.L2_i;
	entry = pagetable_l2->entries[index];
	if(entry.present == false) {
		pagetable_l1 = (struct PageTable*)pageframe_request();
		memset_byte(pagetable_l1, 0, MEMORY_PAGE_SIZE);
		entry.address = (ulong)pagetable_l1 >> 12;
		entry.present = true;
		entry.writable = true;
		pagetable_l2->entries[index] = entry;
	} else {
		pagetable_l1 = (struct PageTable*)((ulong)entry.address << 12);
	}
	// Index the page table to get the physical address
	index = indexes.L1_i;
	entry = pagetable_l1->entries[index];
	entry.address = (ulong)physical_address >> 12;	// offset is irrelvant, just mapping the page levels
	entry.present = true;
	entry.writable = true;
	pagetable_l1->entries[index] = entry;
	print(string_str_from_int(i), SHELL_COLOR_FOREGROUND); print_char(' ', SHELL_COLOR_FOREGROUND); 
	i++;
}