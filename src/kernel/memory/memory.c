/*
 * File:		memory.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "bootloader.h"
#include "memory.h"

size_t memory_total_size = 0;

void memory_init(struct MemoryDescriptor* mem_map, size_t mem_map_size, size_t mem_map_descriptor_size) {
	uint num_enteries = mem_map_size / mem_map_descriptor_size;
	struct MemoryDescriptor* largest_primary = NULL;
	// Find largest primary memory region
	for(uint i = 0; i < num_enteries; i++){
		struct MemoryDescriptor* descriptor = (struct MemoryDescriptor*)((void*)mem_map + (i * mem_map_descriptor_size));
		if(descriptor->type == MEMORY_PRIMARY_REGION_TYPE && (largest_primary == NULL || descriptor->num_pages > largest_primary->num_pages)){
			largest_primary = descriptor;
		}
		memory_total_size += descriptor->num_pages * BOOTLOADER_MEMORY_PAGE_SIZE;
	}
	memory_space_primary = (struct MemoryRegion){
		.base = largest_primary->phys_address,
		.size = largest_primary->num_pages * BOOTLOADER_MEMORY_PAGE_SIZE
	};
}