/*
 * File:		memory.h
 * Description:	For general memory information passed by the bootloader
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define MEMORY_PAGE_SIZE		4096
#define	MEMORY_PAGE_ENTRY_SIZE	(MEMORY_PAGE_SIZE/sizeof(long))

#define	ROUND_UP_PAGE_SIZE(n)	ROUND_UP(n, MEMORY_PAGE_SIZE)
#define	NEAREST_PAGE(n)			(ROUND_UP_PAGE_SIZE(n) / MEMORY_PAGE_SIZE)

#define MEMORY_MEMORYMAP_MAX_ENTRIES	256


enum MemoryType {
	MEMORY_TYPE_UNUSABLE,
	MEMORY_TYPE_USABLE
};

struct MemoryMapEntry {
	void*			physical_base;
	size_t			num_pages;
	enum MemoryType	type;
};

struct MemoryMap {
	struct MemoryMapEntry	entries[MEMORY_MEMORYMAP_MAX_ENTRIES];
	size_t					entries_num;
};


// *** Implemented in pageframe.c *** //

// Gets the total size of memory space (bytes), regardless of whether it's usable or not
size_t memory_get_total_size();
// Gets the size of memory space (bytes) used by paging
size_t memory_get_used_size();
// Gets the size of memory space (bytes) used by reserved paging
size_t memory_get_reserved_size();
// Calculates free memory space that's pageable
size_t memory_get_free();