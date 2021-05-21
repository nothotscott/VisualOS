/*
 * File:		paging.c
 * *****************************************************************************
 * Copyright 2020-201 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "log.h"
#include "bootloader.h"
#include "framebuffer.h"
#include "bitmap.h"
#include "pageframe.h"
#include "paging.h"


// Returns the address component of [entry]
static inline void* paging_get_entry_address(page_directory_entry_t entry);

// Static kernel page table level 4 pointer
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

static inline void* paging_get_entry_address(page_directory_entry_t entry) {
	return (void*)(uint64_t)((entry & 0x000ffffffffff000) >> 12);
}

struct PageTable* paging_get_pagetable_l4() {
	return s_pagetable_l4;
}

// *** Class Functions *** //

void paging_init() {
	s_pagetable_l4 = (struct PageTable*)pageframe_request();
	memset(s_pagetable_l4, 0, MEMORY_PAGE_SIZE);
	void* kernel_physical_start = KERNEL_PHYSICAL_ADDRESS(&_kernel_start);
	void* kernel_physical_end = KERNEL_PHYSICAL_ADDRESS(&_kernel_end);
	void* kernel_writable_physical_start = KERNEL_PHYSICAL_ADDRESS(&_kernel_writable_start);
	void* kernel_writable_physical_end = KERNEL_PHYSICAL_ADDRESS(&_kernel_writable_end);
	uint64_t kernel_virtual_base = (uint64_t)&_virtual_base;
	// Map kernel
	for(void* ptr = kernel_physical_start; (uint64_t)ptr < (uint64_t)kernel_physical_end; ptr += MEMORY_PAGE_SIZE) {
		void* virtual_address = (void*)((uint64_t)ptr + kernel_virtual_base);
		paging_map_page(s_pagetable_l4, virtual_address, ptr);
		if(ptr >= kernel_writable_physical_start && ptr < kernel_writable_physical_end) {
			paging_set_writable(virtual_address, 1);
		}
	}
	// Map useable memory regions
	struct MemoryMap* memorymap = bootloader_get_info()->memorymap;
	size_t entries_num = memorymap->entries_num;
	for(size_t i = 0; i < entries_num; i++) {
		struct MemoryMapEntry* memorymap_entry = memorymap->entries + i;
		if(memorymap_entry->type != MEMORY_TYPE_USABLE){
			continue;
		}
		paging_identity_map(memorymap_entry->physical_base, memorymap_entry->num_pages);
		paging_set_writable(memorymap_entry->physical_base, memorymap_entry->num_pages);
	}
	// Map the rest of the memory regions
	paging_identity_map_size((void*)0, memory_get_total_size());
}

void paging_init_pat() {
	struct Framebuffer* framebuffer = bootloader_get_info()->framebuffer;
	paging_set_cache_size(framebuffer->base, framebuffer->pitch * framebuffer->height, PAGE_PAT_UNCACHED);
}

void paging_map_page(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address) {
	struct PageTable* pagetable_l3;
	struct PageTable* pagetable_l2;
	struct PageTable* pagetable_l1;
	page_directory_entry_t entry;
	size_t index;
	page_directory_entry_t present_attribute = (1 << PAGE_DIRECTORY_PRESENT);
	// Break virtual address into it's level index components
	struct PageLevelIndexes indexes;
	paging_get_indexes(virtual_address, &indexes);
	// Index page table map level 4 to get the page directory pointer table (L3)
	index = indexes.L4_i;
	entry = pagetable_l4->entries[index];
	if(!GET_BIT(entry, PAGE_DIRECTORY_PRESENT)) {	// create page directory pointer table if it's not present
		pagetable_l3 = (struct PageTable*)pageframe_request();
		memset(pagetable_l3, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((uint64_t)pagetable_l3 >> 12));
		entry |= present_attribute;
		pagetable_l4->entries[index] = entry;
	} else {
		pagetable_l3 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);	// restore the address
	}
	// Index page table map level 3 to get the page directory table (L2)
	index = indexes.L3_i;
	entry = pagetable_l3->entries[index];
	if(!GET_BIT(entry, PAGE_DIRECTORY_PRESENT)) {
		pagetable_l2 = (struct PageTable*)pageframe_request();
		memset(pagetable_l2, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((uint64_t)pagetable_l2 >> 12));
		entry |= present_attribute;
		pagetable_l3->entries[index] = entry;
	} else {
		pagetable_l2 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
	}
	// Index page table map level 2 to get the page table (L1)
	index = indexes.L2_i;
	entry = pagetable_l2->entries[index];
	if(!GET_BIT(entry, PAGE_DIRECTORY_PRESENT)) {
		pagetable_l1 = (struct PageTable*)pageframe_request();
		memset(pagetable_l1, 0, MEMORY_PAGE_SIZE);
		entry = paging_set_entry_address(entry, (void*)((uint64_t)pagetable_l1 >> 12));
		entry |= present_attribute;
		pagetable_l2->entries[index] = entry;
	} else {
		pagetable_l1 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
	}
	// Index the page table to get the physical address
	index = indexes.L1_i;
	entry = pagetable_l1->entries[index];
	entry = paging_set_entry_address(entry, (void*)((uint64_t)physical_address >> 12));	// offset is irrelvant, just mapping the page levels
	entry |= (1 << PAGE_TABLE_PRESENT);
	pagetable_l1->entries[index] = entry;
}
void paging_map(struct PageTable* pagetable_l4, void* virtual_address, void* physical_address, size_t pages) {
	for(int i = 0; i < pages; i++){
		size_t offset = i * MEMORY_PAGE_SIZE;
		paging_map_page(pagetable_l4, virtual_address + offset, physical_address + offset);
	}
}

void paging_identity_map(void* address, size_t pages) {
	for(void* ptr = address; (uint64_t)ptr < (uint64_t)address + pages * MEMORY_PAGE_SIZE; ptr += MEMORY_PAGE_SIZE) {
		paging_map_page(s_pagetable_l4, ptr, ptr);
	}
}
void paging_identity_map_size(void* address, size_t size) {
	paging_identity_map(address, NEAREST_PAGE(size));
}


void paging_set_attribute(struct PageTable* pagetable_l4, void* virtual_address, size_t pages, enum PageDirectoryFlagBit attribute, bool enabled) {
	for(size_t i = 0; i < pages; i++){
		page_directory_entry_t entry;
		struct PageLevelIndexes indexes;
		paging_get_indexes(virtual_address + (i * MEMORY_PAGE_SIZE), &indexes);
		// Go through page tables and set the attribute flag
		entry = pagetable_l4->entries[indexes.L4_i];
		entry = SET_BIT(entry, attribute, enabled);
		pagetable_l4->entries[indexes.L4_i] = entry;
		struct PageTable* pagetable_l3 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
		entry = pagetable_l3->entries[indexes.L3_i];
		entry = SET_BIT(entry, attribute, enabled);
		pagetable_l3->entries[indexes.L3_i] = entry;
		struct PageTable* pagetable_l2 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
		entry = pagetable_l2->entries[indexes.L2_i];
		entry = SET_BIT(entry, attribute, enabled);
		pagetable_l2->entries[indexes.L2_i] = entry;
		struct PageTable* pagetable_l1 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
		entry = pagetable_l1->entries[indexes.L1_i];
		entry = SET_BIT(entry, attribute, enabled);
		pagetable_l1->entries[indexes.L1_i] = entry;
	}
}

void paging_set_cache(void* virtual_address, size_t pages, enum PagePATMode mode) {
	struct PageTable* pagetable_l4 = s_pagetable_l4;
	// Linear correspondence to the PA register field
	bool bit_pat = (mode >> 2) & 0x1;
	bool bit_pcd = (mode >> 1) & 0x1;
	bool bit_pwt = (mode >> 0) & 0x1;
	for(size_t i = 0; i < pages; i++){
		page_directory_entry_t entry;
		struct PageLevelIndexes indexes;
		paging_get_indexes(virtual_address + (i * MEMORY_PAGE_SIZE), &indexes);
		entry = pagetable_l4->entries[indexes.L4_i];
		pagetable_l4->entries[indexes.L4_i] = entry;
		struct PageTable* pagetable_l3 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
		entry = pagetable_l3->entries[indexes.L3_i];
		pagetable_l3->entries[indexes.L3_i] = entry;
		struct PageTable* pagetable_l2 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
		entry = pagetable_l2->entries[indexes.L2_i];
		pagetable_l2->entries[indexes.L2_i] = entry;
		struct PageTable* pagetable_l1 = (struct PageTable*)((uint64_t)paging_get_entry_address(entry) << 12);
		entry = pagetable_l1->entries[indexes.L1_i];
		// Set the page table entry
		entry = SET_BIT(entry, PAGE_TABLE_PAT, bit_pat);
		entry = SET_BIT(entry, PAGE_TABLE_NOCACHE, bit_pcd);
		entry = SET_BIT(entry, PAGE_TABLE_WRITETHROUGH, bit_pwt);
		pagetable_l1->entries[indexes.L1_i] = entry;
	}
}
void paging_set_cache_size(void* virtual_address, size_t size, enum PagePATMode mode) {
	paging_set_cache(virtual_address, NEAREST_PAGE(size), mode);
}


void paging_set_writable(void* virtual_address, size_t pages) {
	paging_set_attribute(s_pagetable_l4, virtual_address, pages, PAGE_DIRECTORY_WRITABLE, true);
}
void paging_set_writable_size(void* virtual_address, size_t size) {
	paging_set_writable(virtual_address, NEAREST_PAGE(size));
}