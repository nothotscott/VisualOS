/*
 * File:		heap.h
 * Description:	Heap for memory
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

struct HeapEntry {
	size_t				size;
	struct HeapEntry*	next;
	ulong				properties;
};

// Initalize heap at base location with size
void heap_init(void* base, size_t size);

// Allocates memory for size block
void* malloc(size_t size);

// Frees the memory at address
void free(void* address);