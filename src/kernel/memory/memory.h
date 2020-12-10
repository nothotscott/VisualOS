/*
 * File:		memory.h
 * Desciption:	Handle initial memory conditions
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "bootloader.h"

#define MEMORY_PRIMARY_REGION_TYPE	CONVENTIONAL_MEMORY

struct MemoryRegion {
	void*	base;
	size_t	size;
};

// Where there's memory that's available for the kernel
extern struct MemoryRegion memory_space_primary;

// Initalize memory
void memory_init(struct MemoryDescriptor* mem_map, size_t mem_map_size, size_t mem_map_descriptor_size);