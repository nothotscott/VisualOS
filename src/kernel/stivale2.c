/*
 * File:		stivale2.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "x86_64/cpu.h"
#include "bootloader.h"
#include "framebuffer.h"
#include "module.h"
#include "stivale2.h"

// From kernel.c
extern void kernel_start_stivale2();

static struct Stivale2HeaderTagFrameBuffer s_tag0 = {
	.tag.identifier = STIVALE2_HEADER_TAG_IDENTIFIER_FRAMEBUFFER,
	.tag.next = 0,
	.width = 0,
	.height = 0,
	.bpp = 32
};

__attribute__((section(".stivale2hdr"), used))
static struct Stivale2Header s_stivale2_header = {
	.entry_point = (uint64_t)&kernel_start_stivale2,
	.stack = (uint64_t)g_bsp_stack + sizeof(g_bsp_stack),
	.flags = 0,
	.tags = (uint64_t)&s_tag0
};


// *** Functions *** //

void stivale2_init(struct Stivale2Structure* structure) {
	struct BootloaderInformation* info = bootloader_get_info();
	stivale2_get_framebuffer(structure, info->framebuffer);
	stivale2_get_memorymap(structure, info->memorymap);
	info->rsdp = (struct RSDP2*)((struct Stivale2StructureRSDP*)stivale2_get_structure(structure, STIVALE2_STRUCTURE_TAG_IDENTIFIER_RSDP))->rsdp;
	info->kernel_header = (void*)((struct Stivale2StructureKernelFile*)stivale2_get_structure(structure, STIVALE2_STRUCTURE_TAG_IDENTIFIER_KERNEL_FILE))->kernel_file;
	info->kernel_offset = ((struct Stivale2StructureKernelSlide*)stivale2_get_structure(structure, STIVALE2_STRUCTURE_TAG_IDENTIFIER_KERNEL_SLIDE))->kernel_slide;
	stivale2_get_module(structure, info->font, MODULE_SIGNATURE_FONT, sizeof(struct ModulePSF1Header));
	stivale2_get_module(structure, info->image, MODULE_SIGNATURE_IMAGE, sizeof(struct ModuleTGAHeader));
}

struct Stivale2StructureTag* stivale2_get_structure(struct Stivale2Structure* structure, enum Stivale2StructureTagIdentifier identifier) {
	struct Stivale2StructureTag* current = (struct Stivale2StructureTag*)structure->tags;
	while(true) {
		if(current == NULL) {
			return NULL;
		}
		if(current->identifier == identifier) {
			return current;
		}
		current = (struct Stivale2StructureTag*)current->next;
	}
}

void stivale2_get_framebuffer(struct Stivale2Structure* structure, struct Framebuffer* framebuffer) {
	struct Stivale2StructureTagFramebuffer* stivale2_framebuffer = (struct Stivale2StructureTagFramebuffer*)stivale2_get_structure(structure, STIVALE2_STRUCTURE_TAG_IDENTIFIER_FRAMEBUFFER);
	*framebuffer = (struct Framebuffer){
		.base = (void*)stivale2_framebuffer->base,
		.width = stivale2_framebuffer->width,
		.height = stivale2_framebuffer->height,
		.pitch = stivale2_framebuffer->pitch,
		.bpp = stivale2_framebuffer->bpp
	};
}

void stivale2_get_memorymap(struct Stivale2Structure* structure, struct MemoryMap* memorymap) {
	struct Stivale2StructureTagMemoryMap* stivale2_memorymap = (struct Stivale2StructureTagMemoryMap*)stivale2_get_structure(structure, STIVALE2_STRUCTURE_TAG_IDENTIFIER_MEMORYMAP);
	size_t entries_num = stivale2_memorymap->entries_num;
	for(size_t i = 0; i < entries_num; i++) {
		struct Stivale2MemoryMapEntry* stivale2_memorymap_entry = stivale2_memorymap->entries + i;
		memorymap->entries[i] = (struct MemoryMapEntry){
			.physical_base = (void*)stivale2_memorymap_entry->base,
			.num_pages = NEAREST_PAGE(stivale2_memorymap_entry->length),
			.type = MEMORY_TYPE_USABLE ? stivale2_memorymap_entry->type == STIVALE2_MEMORY_TYPE_USABLE : MEMORY_TYPE_UNUSABLE
		};
	}
	memorymap->entries_num = entries_num;
}

bool stivale2_get_module(struct Stivale2Structure* structure, struct Module* module, const char* signature, size_t header_size) {
	struct Stivale2StructureModules* stivale2_modules = (struct Stivale2StructureModules*)stivale2_get_structure(structure, STIVALE2_STRUCTURE_TAG_IDENTIFIER_MODULES);
	for(size_t i = 0; i < stivale2_modules->modules_num; i++) {
		struct Stivale2Module* stivale2_module = &stivale2_modules->modules[i];
		if(strcmp(signature, stivale2_module->string) != 0) {
			continue;
		}
		*module = (struct Module){
			.header = (void*)stivale2_module->begin,
			.body = (void*)(stivale2_module->begin + header_size),
			.size = stivale2_module->end - stivale2_module->begin,
		};
		return true;
	}
	return false;
}