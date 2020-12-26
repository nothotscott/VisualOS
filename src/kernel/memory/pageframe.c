/*
 * File:		pageframe.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "paging.h"
#include "bitmap.h"
#include "pageframe.h"


size_t g_memory_total_size = 0;
size_t g_memory_used_size = 0;
size_t g_memory_reserved_size = 0;
struct Bitmap g_pageframemap;


void pageframe_init(struct MemoryDescriptor* mem_map, size_t mem_map_size, size_t mem_map_descriptor_size) {
	g_memory_total_size = 0;
	g_memory_used_size = 0;
	g_memory_reserved_size = 0;
	uint num_enteries = mem_map_size / mem_map_descriptor_size;
	struct MemoryDescriptor* largest_primary = NULL;
	// Find largest primary memory region
	for(uint i = 0; i < num_enteries; i++){
		struct MemoryDescriptor* descriptor = (struct MemoryDescriptor*)((void*)mem_map + (i * mem_map_descriptor_size));
		if(descriptor->type == MEMORY_PAGEABLE_TYPE && (largest_primary == NULL || descriptor->num_pages > largest_primary->num_pages)){
			largest_primary = descriptor;
		}
		g_memory_total_size += descriptor->num_pages * BOOTLOADER_MEMORY_PAGE_SIZE;
	}
	// Assign bitmap
	bitmap_initialize(&g_pageframemap, largest_primary->phys_address, g_memory_total_size / MEMORY_PAGE_SIZE / BITMAP_SCALE);
	// Reserve the g_pageframemap itself
	size_t g_pageframemap_pages = g_pageframemap.size % MEMORY_PAGE_SIZE > 0 ? g_pageframemap.size / MEMORY_PAGE_SIZE + 1 : g_pageframemap.size / MEMORY_PAGE_SIZE;
	pageframe_reserve(g_pageframemap.buffer, g_pageframemap_pages);
	// Reserve kernel space
	size_t kernel_size = (size_t)&_kernel_end - (size_t)&_kernel_start;
	size_t kernel_pages = kernel_size % MEMORY_PAGE_SIZE > 0 ? kernel_size / MEMORY_PAGE_SIZE + 1 : kernel_size / MEMORY_PAGE_SIZE;
	pageframe_reserve(&_kernel_start, kernel_pages);
	// Reserve unusable memory segments
	for(uint i = 0; i < num_enteries; i++){
		struct MemoryDescriptor* descriptor = (struct MemoryDescriptor*)((void*)mem_map + (i * mem_map_descriptor_size));
		if(descriptor->type == MEMORY_PAGEABLE_TYPE){
			continue;
		}
		size_t reserved_size = descriptor->num_pages * BOOTLOADER_MEMORY_PAGE_SIZE;
		pageframe_reserve(descriptor->phys_address, reserved_size / MEMORY_PAGE_SIZE);
	}
}

size_t memory_get_free() {
	return g_memory_total_size - g_memory_used_size - g_memory_reserved_size;
}

bool pageframe_manipulate(ulong index, bool state) {
	if(bitmap_get(&g_pageframemap, index) == state){
		return true;	// already in state
	}
	bitmap_set(&g_pageframemap, index, state);
	return false;
}

void* pageframe_request() {
	for(ulong i = 0; i < bitmap_adjusted_size(&g_pageframemap); i++){
		if(bitmap_get(&g_pageframemap, i) == true){
			continue;	// not free
		}
		void* page = (void*)(i * MEMORY_PAGE_SIZE);	// transform the index into the page address
		pageframe_lock(page, 1);
		return page;
	}
	// TODO Page frame swap to file
	return NULL;
}

void pageframe_free(void* address, size_t pages) {
	ulong start = (ulong)address / MEMORY_PAGE_SIZE;
	for(ulong i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, false)){
			if(g_memory_used_size >= MEMORY_PAGE_SIZE) {		// prevent overflow
				g_memory_used_size -= MEMORY_PAGE_SIZE;
			}
		}
	}
}

void pageframe_lock(void* address, size_t pages) {
	ulong start = (ulong)address / MEMORY_PAGE_SIZE;
	for(ulong i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, true)){
			g_memory_used_size += MEMORY_PAGE_SIZE;
		}
	}
}

void pageframe_unreserve(void* address, size_t pages) {
	ulong start = (ulong)address / MEMORY_PAGE_SIZE;
	for(ulong i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, false)){
			if(g_memory_reserved_size >= MEMORY_PAGE_SIZE) {	// prevent overflow
				g_memory_reserved_size -= MEMORY_PAGE_SIZE;
			}
		}
	}
}

void pageframe_reserve(void* address, size_t pages) {
	ulong start = (ulong)address / MEMORY_PAGE_SIZE;
	for(ulong i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, true)){
			g_memory_reserved_size += MEMORY_PAGE_SIZE;
		}
	}
}