/*
 * File:		pageframe.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "bitmap.h"
#include "paging.h"
#include "pageframe.h"


size_t g_memory_total_size = 0;
size_t g_memory_used_size = 0;
size_t g_memory_reserved_size = 0;
struct Bitmap g_pageframemap;
// Holds current bitmap index to help optimize paging
static size_t g_current_index = 0;


void pageframe_init(struct MemoryDescriptor* mem_map, size_t mem_map_size, size_t mem_map_descriptor_size) {
	g_memory_total_size = 0;
	g_memory_used_size = 0;
	g_memory_reserved_size = 0;
	uint32_t num_enteries = mem_map_size / mem_map_descriptor_size;
	struct MemoryDescriptor* largest_primary = NULL;
	// Find largest primary memory region
	for(uint32_t i = 0; i < num_enteries; i++){
		struct MemoryDescriptor* descriptor = (struct MemoryDescriptor*)((void*)mem_map + (i * mem_map_descriptor_size));
		if(descriptor->type == MEMORY_PAGEABLE_TYPE && (largest_primary == NULL || descriptor->num_pages > largest_primary->num_pages)){
			largest_primary = descriptor;
		}
		g_memory_total_size += descriptor->num_pages * BOOTLOADER_MEMORY_PAGE_SIZE;
	}
	// Assign bitmap
	bitmap_initialize(&g_pageframemap, largest_primary->phys_address, g_memory_total_size / MEMORY_PAGE_SIZE / BITMAP_SCALE);
	// Reserve the g_pageframemap itself
	size_t pageframemap_pages = g_pageframemap.size % MEMORY_PAGE_SIZE > 0 ? g_pageframemap.size / MEMORY_PAGE_SIZE + 1 : g_pageframemap.size / MEMORY_PAGE_SIZE;
	pageframe_reserve(g_pageframemap.buffer, pageframemap_pages);
	// Reserve kernel space
	size_t kernel_size = (size_t)&_kernel_end - (size_t)&_kernel_start;
	size_t kernel_pages = kernel_size % MEMORY_PAGE_SIZE > 0 ? kernel_size / MEMORY_PAGE_SIZE + 1 : kernel_size / MEMORY_PAGE_SIZE;
	pageframe_reserve(&_kernel_start, kernel_pages);
	// Reserve unusable memory segments
	for(uint32_t i = 0; i < num_enteries; i++){
		struct MemoryDescriptor* descriptor = (struct MemoryDescriptor*)((void*)mem_map + (i * mem_map_descriptor_size));
		if(descriptor->type == MEMORY_PAGEABLE_TYPE){
			continue;
		}
		size_t reserved_size = descriptor->num_pages * BOOTLOADER_MEMORY_PAGE_SIZE;
		pageframe_reserve(descriptor->phys_address, reserved_size / MEMORY_PAGE_SIZE);
	}
	// Reserve first 256 pages
	pageframe_reserve(0, MEMORY_INITIAL_RESERVE_PAGES);
}

size_t memory_get_free() {
	return g_memory_total_size - g_memory_used_size - g_memory_reserved_size;
}

bool pageframe_manipulate(uint64_t index, bool state) {
	if(bitmap_get(&g_pageframemap, index) == state){
		return true;	// already in state
	}
	return bitmap_set(&g_pageframemap, index, state);
}

void* pageframe_request() {
	for(; g_current_index < bitmap_adjusted_size(&g_pageframemap); g_current_index++){
		if(bitmap_get(&g_pageframemap, g_current_index) == true){
			continue;	// not free
		}
		void* page = (void*)(g_current_index * MEMORY_PAGE_SIZE);	// transform the index into the page address
		pageframe_lock(page, 1);
		return page;
	}
	// TODO Page frame swap to file
	return NULL;
}

void pageframe_free(void* address, size_t pages) {
	uint64_t start = (uint64_t)address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, false)){
			if(g_memory_used_size >= MEMORY_PAGE_SIZE) {		// prevent overflow
				g_memory_used_size -= MEMORY_PAGE_SIZE;
			}
			if(g_current_index > i) {
				g_current_index = i;
			}
		}
	}
}

void pageframe_lock(void* address, size_t pages) {
	uint64_t start = (uint64_t)address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, true)){
			g_memory_used_size += MEMORY_PAGE_SIZE;
		}
	}
}

void pageframe_unreserve(void* address, size_t pages) {
	uint64_t start = (uint64_t)address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, false)){
			if(g_memory_reserved_size >= MEMORY_PAGE_SIZE) {	// prevent overflow
				g_memory_reserved_size -= MEMORY_PAGE_SIZE;
			}
			if(g_current_index > i) {
				g_current_index = i;
			}
		}
	}
}

void pageframe_reserve(void* address, size_t pages) {
	uint64_t start = (uint64_t)address / MEMORY_PAGE_SIZE;
	for(uint64_t i = start; i < start + pages; i++){
		if(pageframe_manipulate(i, true)){
			g_memory_reserved_size += MEMORY_PAGE_SIZE;
		}
	}
}