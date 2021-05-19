/*
 * File:		pageframe.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "bootloader.h"
#include "bitmap.h"
#include "memory.h"
#include "paging.h"
#include "pageframe.h"


// Total size of memory space (bytes), regardless of whether it's usable or not
static size_t s_memory_total_size = 0;
// Size of memory space (bytes) used by paging
static size_t s_memory_used_size = 0;
// Size of memory space (bytes) used by reserved paging
static size_t s_memory_reserved_size = 0;
// Bitmap of usable memory space
// A bit will be 1 if it's in-use, else 0 if it's free
static struct Bitmap s_pageframemap;
// Holds current bitmap index to help optimize paging
static size_t s_current_index = 0;


void pageframe_init() {
	struct MemoryMap* memorymap = bootloader_get_info()->memorymap;
	size_t entries_num = memorymap->entries_num;
	struct MemoryMapEntry* largest_primary = NULL;
	// Find largest primary memory region
	for(size_t i = 0; i < entries_num; i++) {
		struct MemoryMapEntry* memorymap_entry = memorymap->entries + i;
		if(memorymap_entry->type == MEMORY_TYPE_USABLE && (largest_primary == NULL || memorymap_entry->num_pages > largest_primary->num_pages)){
			largest_primary = memorymap_entry;
		}
		s_memory_total_size += memorymap_entry->num_pages * MEMORY_PAGE_SIZE;
	}
	// Assign bitmap
	bitmap_initialize(&s_pageframemap, largest_primary->physical_base, s_memory_total_size / MEMORY_PAGE_SIZE / BITMAP_SCALE);
	// Reserve all pages
	pageframe_reserve(0, s_memory_total_size / MEMORY_PAGE_SIZE);
	// Unreserve unusable memory segments
	for(size_t i = 0; i < entries_num; i++) {
		struct MemoryMapEntry* memorymap_entry = memorymap->entries + i;
		if(memorymap_entry->type != MEMORY_TYPE_USABLE){
			continue;
		}
		size_t reserved_size = memorymap_entry->num_pages * MEMORY_PAGE_SIZE;
		pageframe_unreserve(memorymap_entry->physical_base, reserved_size / MEMORY_PAGE_SIZE);
	}
	// Lock the s_pageframemap itself
	size_t pageframemap_pages = NEAREST_PAGE(s_pageframemap.size);
	pageframe_lock(s_pageframemap.buffer, pageframemap_pages);
	// Lock kernel space
	size_t kernel_size = (size_t)&_kernel_end - (size_t)&_kernel_start;
	size_t kernel_pages = NEAREST_PAGE(kernel_size);
	pageframe_lock(PHYSICAL_ADDRESS(&_kernel_start), kernel_pages);
	// Reserve first 256 pages
	pageframe_reserve(0, PAGEFRAME_INITIAL_RESERVE_PAGES);
}

bool pageframe_manipulate(uint64_t index, bool state) {
	if(bitmap_get(&s_pageframemap, index) == state){
		return true;	// already in state
	}
	return bitmap_set(&s_pageframemap, index, state);
}

void* pageframe_request() {
	for(; s_current_index < bitmap_adjusted_size(&s_pageframemap); s_current_index++){
		if(bitmap_get(&s_pageframemap, s_current_index) == true){
			continue;	// not free
		}
		void* page = (void*)(s_current_index * MEMORY_PAGE_SIZE);	// transform the index into the page address
		s_current_index++;
		pageframe_lock(page, 1);
		return page;
	}
	// TODO Page frame swap to file
	return NULL;
}
void* pageframe_request_pages(size_t pages) {
	size_t bitmap_size = bitmap_adjusted_size(&s_pageframemap);
	while(s_current_index < bitmap_size) {
		for(size_t j = 0; j < pages; j++) {
			if(bitmap_get(&s_pageframemap, s_current_index + j) == true) {
				s_current_index += j + 1;
				goto not_free;
			}
		}
		goto exit;
		not_free:
			continue;
		exit: {
			void* page = (void*)(s_current_index * MEMORY_PAGE_SIZE);	// transform the index into the physical page address
			s_current_index += pages;
			pageframe_lock(page, pages);
			return page;
		}
	}
	return NULL;
}

void pageframe_free(void* physical_address, size_t pages) {
	uint64_t start = (uint64_t)physical_address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, false)){
			if(s_memory_used_size >= MEMORY_PAGE_SIZE) {		// prevent overflow
				s_memory_used_size -= MEMORY_PAGE_SIZE;
			}
			if(s_current_index > i) {
				s_current_index = i;
			}
		}
	}
}

void pageframe_lock(void* physical_address, size_t pages) {
	uint64_t start = (uint64_t)physical_address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, true)){
			s_memory_used_size += MEMORY_PAGE_SIZE;
		}
	}
}

void pageframe_unreserve(void* physical_address, size_t pages) {
	uint64_t start = (uint64_t)physical_address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, false)){
			if(s_memory_reserved_size >= MEMORY_PAGE_SIZE) {	// prevent overflow
				s_memory_reserved_size -= MEMORY_PAGE_SIZE;
			}
			if(s_current_index > i) {
				s_current_index = i;
			}
		}
	}
}

void pageframe_reserve(void* address, size_t pages) {
	uint64_t start = (uint64_t)address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, true)){
			s_memory_reserved_size += MEMORY_PAGE_SIZE;
		}
	}
}
void pageframe_reserve_size(void* address, size_t size) {
	pageframe_reserve(address, NEAREST_PAGE(size));
}


size_t memory_get_total_size() {
	return s_memory_total_size;
}
size_t memory_get_used_size() {
	return s_memory_used_size;
}
size_t memory_get_reserved_size() {
	return s_memory_reserved_size;
}
size_t memory_get_free() {
	return s_memory_total_size - s_memory_used_size - s_memory_reserved_size;
}