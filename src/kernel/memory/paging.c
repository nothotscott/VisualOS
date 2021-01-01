/*
 * File:		paging.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <memory.h>
#include <string.h>
#include "pageframe.h"
#include "paging.h"
#include "shell/shell.h"


struct PageTable* g_pagetable_l4;


void paging_get_indexes(void* address, struct PageLevelIndexes* out) {
	ulong_t address64 = (ulong_t)address;
	address64 >>= 12;	// shift off offset
	// Get 9 LSBs and bit shift off for each of the 4 levels
	out->L1_i = address64 & 0x01ff;
	address64 >>= 9;
	out->L2_i = address64 & 0x01ff;
	address64 >>= 9;
	out->L3_i = address64 & 0x01ff;
	address64 >>= 9;
	out->L4_i = address64 & 0x01ff;
}

page_directory_entry_t paging_set_entry_address(page_directory_entry_t entry, void* address) {
	entry &= 0xfff0000000000fff;						// clear address
	entry |= ((ulong_t)address & 0x000000ffffffffff) << 12;
	return entry;
}

void* paging_get_entry_address(page_directory_entry_t entry) {
	return (void*)(ulong_t)((entry & 0x000ffffffffff000) >> 12);
}


void paging_init() {
	g_pagetable_l4 = (struct PageTable*)pageframe_request();
	memset_byte(g_pagetable_l4, 0, MEMORY_PAGE_SIZE);
	// Identity map (for now) each page
	// TODO better page mapping
	paging_identity_map(0, g_memory_total_size);
}

void paging_identity_map(void* address, size_t size) {
	size = ROUND_UP(size, MEMORY_PAGE_SIZE);
	for(void* ptr = address; (uintptr_t)ptr < (uintptr_t)address + size; ptr += MEMORY_PAGE_SIZE) {
		paging_map(g_pagetable_l4, ptr, ptr);	// 1 to 1 mapping
	}
}

void paging_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address) {
	struct PageTable* pagetable_l3;
	struct PageTable* pagetable_l2;
	struct PageTable* pagetable_l1;
	page_directory_entry_t entry;
	size_t index;
	// Break virtual address into it's level index components
	struct PageLevelIndexes indexes;
	paging_get_indexes(virtual_address, &indexes);
	// Index page table map level 4 to get the page directory pointer table (L3)
	index = indexes.L4_i;
	entry = pagetable_l4->entries[index];
	if(!GET_BIT(entry, PAGE_PRESENT)) {	// create page directory pointer table if it's not present
		pagetable_l3 = (struct PageTable*)pageframe_request();
		memset_byte(pagetable_l3, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((ulong_t)pagetable_l3 >> 12));
		entry = SET_BIT(entry, PAGE_PRESENT, true);
		entry = SET_BIT(entry, PAGE_WRITABLE, true);
		pagetable_l4->entries[index] = entry;
	} else {
		pagetable_l3 = (struct PageTable*)((ulong_t)paging_get_entry_address(entry) << 12);	// restore the address
	}
	// Index page table map level 3 to get the page directory table (L2)
	index = indexes.L3_i;
	entry = pagetable_l3->entries[index];
	if(!GET_BIT(entry, PAGE_PRESENT)) {
		pagetable_l2 = (struct PageTable*)pageframe_request();
		memset_byte(pagetable_l2, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((ulong_t)pagetable_l2 >> 12));
		entry = SET_BIT(entry, PAGE_PRESENT, true);
		entry = SET_BIT(entry, PAGE_WRITABLE, true);
		pagetable_l3->entries[index] = entry;
	} else {
		pagetable_l2 = (struct PageTable*)((ulong_t)paging_get_entry_address(entry) << 12);
	}
	// Index page table map level 2 to get the page table (L1)
	index = indexes.L2_i;
	entry = pagetable_l2->entries[index];
	if(!GET_BIT(entry, PAGE_PRESENT)) {
		pagetable_l1 = (struct PageTable*)pageframe_request();
		memset_byte(pagetable_l1, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((ulong_t)pagetable_l1 >> 12));
		entry = SET_BIT(entry, PAGE_PRESENT, true);
		entry = SET_BIT(entry, PAGE_WRITABLE, true);
		pagetable_l2->entries[index] = entry;
	} else {
		pagetable_l1 = (struct PageTable*)((ulong_t)paging_get_entry_address(entry) << 12);
	}
	// Index the page table to get the physical address
	index = indexes.L1_i;
	entry = pagetable_l1->entries[index];
	entry = paging_set_entry_address(entry, (void*)((ulong_t)physical_address >> 12));	// offset is irrelvant, just mapping the page levels
	entry = SET_BIT(entry, PAGE_PRESENT, true);
	entry = SET_BIT(entry, PAGE_WRITABLE, true);
	pagetable_l1->entries[index] = entry;
}

void paging_fault_handler(struct InterruptStack* stack) {
	print_newline();
	print("PAGE FAULT DETECTED", SHELL_COLOR_RED); print_newline();
	print("  ERROR CODE:   0x", SHELL_COLOR_RED); print(string_str_from_ulong(stack->error_code), SHELL_COLOR_RED); print_newline();
	print("  REGISTER RIP: 0x", SHELL_COLOR_RED); print(string_str_from_ulong(stack->rip), SHELL_COLOR_RED); print_newline();
	while(true);
}