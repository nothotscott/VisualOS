/*
 * File:		pageframe.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "x86_64/apic/apic.h"
#include "bitmap.h"
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


void pageframe_init(struct MemoryDescriptor* mem_map, size_t mem_map_size, size_t mem_map_descriptor_size) {
	s_memory_total_size = 0;
	s_memory_used_size = 0;
	s_memory_reserved_size = 0;
	uint32_t num_enteries = mem_map_size / mem_map_descriptor_size;
	struct MemoryDescriptor* largest_primary = NULL;
	// Find largest primary memory region
	for(uint32_t i = 0; i < num_enteries; i++){
		struct MemoryDescriptor* descriptor = (struct MemoryDescriptor*)((void*)mem_map + (i * mem_map_descriptor_size));
		if(descriptor->type == MEMORY_PAGEABLE_TYPE && (largest_primary == NULL || descriptor->num_pages > largest_primary->num_pages)){
			largest_primary = descriptor;
		}
		s_memory_total_size += descriptor->num_pages * BOOTLOADER_MEMORY_PAGE_SIZE;
	}
	// Assign bitmap
	bitmap_initialize(&s_pageframemap, largest_primary->phys_address, s_memory_total_size / MEMORY_PAGE_SIZE / BITMAP_SCALE);
	// Reserve all pages
	pageframe_reserve(0, s_memory_total_size / MEMORY_PAGE_SIZE);
	// Unreserve unusable memory segments
	for(uint32_t i = 0; i < num_enteries; i++){
		struct MemoryDescriptor* descriptor = (struct MemoryDescriptor*)((void*)mem_map + (i * mem_map_descriptor_size));
		if(descriptor->type != MEMORY_PAGEABLE_TYPE){
			continue;
		}
		size_t reserved_size = descriptor->num_pages * BOOTLOADER_MEMORY_PAGE_SIZE;
		pageframe_unreserve(descriptor->phys_address, reserved_size / MEMORY_PAGE_SIZE);
	}
	// Reserve the s_pageframemap itself
	size_t pageframemap_pages = s_pageframemap.size % MEMORY_PAGE_SIZE > 0 ? s_pageframemap.size / MEMORY_PAGE_SIZE + 1 : s_pageframemap.size / MEMORY_PAGE_SIZE;
	pageframe_reserve(s_pageframemap.buffer, pageframemap_pages);
	// Reserve kernel space
	size_t kernel_size = (size_t)&_kernel_end - (size_t)&_kernel_start;
	size_t kernel_pages = kernel_size % MEMORY_PAGE_SIZE > 0 ? kernel_size / MEMORY_PAGE_SIZE + 1 : kernel_size / MEMORY_PAGE_SIZE;
	pageframe_reserve(&_kernel_start, kernel_pages);
	// Reserve first 256 pages
	pageframe_reserve(0, MEMORY_INITIAL_RESERVE_PAGES);
	// Lock the APIC trampoline code
	pageframe_lock((void*)APIC_TRAMPOLINE_TARGET, APIC_TRAMPOLINE_TARGET_SIZE);
}

size_t memory_get_free() {
	return s_memory_total_size - s_memory_used_size - s_memory_reserved_size;
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
				s_current_index = s_current_index + j + 1;
				goto not_free;
			}
		}
		goto exit;
		not_free:
			continue;
		exit:
			s_current_index++;
			void* page = (void*)(s_current_index * MEMORY_PAGE_SIZE);	// transform the index into the page address
			pageframe_lock(page, pages);
			return page;
	}
	return NULL;
}

void pageframe_free(void* address, size_t pages) {
	uint64_t start = (uint64_t)address / MEMORY_PAGE_SIZE;
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

void pageframe_lock(void* address, size_t pages) {
	uint64_t start = (uint64_t)address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, true)){
			s_memory_used_size += MEMORY_PAGE_SIZE;
		}
	}
}

void pageframe_unreserve(void* address, size_t pages) {
	uint64_t start = (uint64_t)address / MEMORY_PAGE_SIZE;
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

size_t pageframe_get_memory_total_size() {
	return s_memory_total_size;
}
size_t pageframe_get_memory_used_size() {
	return s_memory_used_size;
}
size_t pageframe_get_memory_reserved_size() {
	return s_memory_reserved_size;
}