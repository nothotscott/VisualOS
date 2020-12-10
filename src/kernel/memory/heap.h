/*
 * File:		heap.h
 * Desciption:	Heap for memory
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "memory.h"

#define MEMORY_HEAP_PAGE_SIZE		64
#define MEMORY_HEAP_START_OFFSET	0

#define MEMORY_HEAP_PROPERTY_FREE	0b00000001

struct PageTableEntry {
	size_t					size;
	struct PageTableEntry*	next;
	ulong					properties;
};

// Initalize heap at base location
void heap_init(struct MemoryRegion* base);

// Allocates memory for size block
void* malloc(size_t size);

// Frees the memory at address
void free(void* address);