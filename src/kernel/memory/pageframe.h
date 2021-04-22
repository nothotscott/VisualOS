/*
 * File:		pageframe.h
 * Description:	Keeps track of individual page frames
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "bootloader.h"
#include "bitmap.h"

#define MEMORY_PAGEABLE_TYPE			BOOTLOADER_CONVENTIONAL_MEMORY
#define MEMORY_INITIAL_RESERVE_PAGES	256


// Initalizes paging by discovering the usable memory segment from [mem_map],
// [mem_map_size], and [mem_map_descriptor_size] and creates a bitmap
// of the usable space
void pageframe_init(struct MemoryDescriptor* mem_map, size_t mem_map_size, size_t mem_map_descriptor_size);

// Calculates free memory space that's pageable
size_t memory_get_free();

// Checks if a page is in [state] at [index] and sets it to [state]. 
// Will return true if it's state is already in [state].
bool pageframe_manipulate(uint64_t index, bool state);

// Will find, lock, and returns [pages] free pages for use
void* pageframe_request();
void* pageframe_request_pages(size_t pages);

// Free pages at [address] and thereafter [pages] times and track used
void pageframe_free(void* address, size_t pages);

// Make pages in-use at [address] and therafter [pages] times and track used
void pageframe_lock(void* address, size_t pages);

// Free pages at [address] and thereafter [pages] times and track reserved
void pageframe_unreserve(void* address, size_t pages);

// Make pages in-use at [address] and therafter [pages] times and track reserved
void pageframe_reserve(void* address, size_t pages);

// Gets the total size of memory space (bytes), regardless of whether it's usable or not
size_t pageframe_get_memory_total_size();
// Gets the size of memory space (bytes) used by paging
size_t pageframe_get_memory_used_size();
// Gets the size of memory space (bytes) used by reserved paging
size_t pageframe_get_memory_reserved_size();