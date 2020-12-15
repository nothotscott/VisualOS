/*
 * File:		paging.h
 * Description:	Paging data structure
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "bootloader.h"
#include "bitmap.h"

#define MEMORY_PAGEABLE_TYPE	CONVENTIONAL_MEMORY
#define MEMORY_PAGE_SIZE		4096


// Total size of memory space (bytes), regardless of whether it's usable or not
extern size_t memory_total_size;
// Size of memory space (bytes) used by paging
extern size_t memory_used_size;
// Size of memory space (bytes_ used by reserved paging
extern size_t memory_reserved_size;

// Bitmap of usable memory space
// A bit will be 1 if it's in-use, else 0 if it's free
struct Bitmap pagemap;


// Initalizes paging by discovering the usable memory segment from [mem_map],
// [mem_map_size], and [mem_map_descriptor_size] and creates a bitmap
// of the usable space
void init_paging(struct MemoryDescriptor* mem_map, size_t mem_map_size, size_t mem_map_descriptor_size);

// Calculates free memory space that's pageable
size_t paging_get_free();

// Checks if a page is in [state] at [index] and sets it to [state]. 
// Will return true if it's state is already in [state].
bool page_manipulate(ulong index, bool state);

// Will find, lock, and return a free page for use
void* page_request();

// Free pages at [address] and thereafter [pages] times and track used
void page_free(void* address, size_t pages);

// Make pages in-use at [address] and therafter [pages] times and track used
void page_lock(void* address, size_t pages);

// Free pages at [address] and thereafter [pages] times and track reserved
void page_unreserve(void* address, size_t pages);

// Make pages in-use at [address] and therafter [pages] times and track reserved
void page_reserve(void* address, size_t pages);