/*
 * File:		paging.c
 * *****************************************************************************
 * Copyright 2020-201 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "pageframe.h"
#include "paging.h"


// Static global page table level 4 pointer
static struct PageTable* s_pagetable_l4;


void paging_get_indexes(void* address, struct PageLevelIndexes* out) {
	uint64_t address64 = (uint64_t)address;
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
	entry |= ((uint64_t)address & 0x000000ffffffffff) << 12;
	return entry;
}

void* paging_get_entry_address(page_directory_entry_t entry) {
	return (void*)(uint64_t)((entry & 0x000ffffffffff000) >> 12);
}

struct PageTable* paging_get_pagetable_l4() {
	return s_pagetable_l4;
}

// *** Class Functions *** //

void paging_init() {
	s_pagetable_l4 = (struct PageTable*)pageframe_request();
	memset(s_pagetable_l4, 0, MEMORY_PAGE_SIZE);
	// Identity map (for now) each page
	// TODO better page mapping
	paging_identity_map(0, memory_get_total_size());
}

void paging_identity_map(void* address, size_t size) {
	size = ROUND_UP(size, MEMORY_PAGE_SIZE);
	for(void* ptr = address; (uintptr_t)ptr < (uintptr_t)address + size; ptr += MEMORY_PAGE_SIZE) {
		// NOTE a conditional breakpoint of ptr > 0x12400000 for the line below will result in showing the drawing
		paging_map(ptr, ptr);	// 1 to 1 mapping
	}
}
void paging_identity_map_page(void* address) {
	paging_map(address, address);
}

void paging_map(void* virtual_address, void* physical_address) {
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
	entry = s_pagetable_l4->entries[index];
	if(!GET_BIT(entry, PAGE_PRESENT)) {	// create page directory pointer table if it's not present
		pagetable_l3 = (struct PageTable*)pageframe_request();
		memset(pagetable_l3, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((uint64_t)pagetable_l3 >> 12));
		entry = SET_BIT(entry, PAGE_PRESENT, true);
		entry = SET_BIT(entry, PAGE_WRITABLE, true);
		s_pagetable_l4->entries[index] = entry;
	} else {
		pagetable_l3 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);	// restore the address
	}
	// Index page table map level 3 to get the page directory table (L2)
	index = indexes.L3_i;
	entry = pagetable_l3->entries[index];
	if(!GET_BIT(entry, PAGE_PRESENT)) {
		pagetable_l2 = (struct PageTable*)pageframe_request();
		memset(pagetable_l2, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((uint64_t)pagetable_l2 >> 12));
		entry = SET_BIT(entry, PAGE_PRESENT, true);
		entry = SET_BIT(entry, PAGE_WRITABLE, true);
		pagetable_l3->entries[index] = entry;
	} else {
		pagetable_l2 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
	}
	// Index page table map level 2 to get the page table (L1)
	index = indexes.L2_i;
	entry = pagetable_l2->entries[index];
	if(!GET_BIT(entry, PAGE_PRESENT)) {
		pagetable_l1 = (struct PageTable*)pageframe_request();
		memset(pagetable_l1, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((uint64_t)pagetable_l1 >> 12));
		entry = SET_BIT(entry, PAGE_PRESENT, true);
		entry = SET_BIT(entry, PAGE_WRITABLE, true);
		pagetable_l2->entries[index] = entry;
	} else {
		pagetable_l1 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
	}
	// Index the page table to get the physical address
	index = indexes.L1_i;
	entry = pagetable_l1->entries[index];
	entry = paging_set_entry_address(entry, (void*)((uint64_t)physical_address >> 12));	// offset is irrelvant, just mapping the page levels
	entry = SET_BIT(entry, PAGE_PRESENT, true);
	entry = SET_BIT(entry, PAGE_WRITABLE, true);
	pagetable_l1->entries[index] = entry;
}

void paging_donate_to_userspace(void* virtual_address, bool userspace_access) {
	page_directory_entry_t entry;
	struct PageLevelIndexes indexes;
	paging_get_indexes(virtual_address, &indexes);
	// Go through page tables and set userspace flag
	entry = s_pagetable_l4->entries[indexes.L4_i];
	entry = SET_BIT(entry, PAGE_USER, userspace_access);
	s_pagetable_l4->entries[indexes.L4_i] = entry;
	struct PageTable* pagetable_l3 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
	entry = pagetable_l3->entries[indexes.L3_i];
	entry = SET_BIT(entry, PAGE_USER, userspace_access);
	pagetable_l3->entries[indexes.L3_i] = entry;
	struct PageTable* pagetable_l2 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
	entry = pagetable_l2->entries[indexes.L2_i];
	entry = SET_BIT(entry, PAGE_USER, userspace_access);
	pagetable_l2->entries[indexes.L2_i] = entry;
	struct PageTable* pagetable_l1 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
	entry = pagetable_l1->entries[indexes.L1_i];
	entry = SET_BIT(entry, PAGE_USER, userspace_access);
	pagetable_l1->entries[indexes.L1_i] = entry;
}
