/*
 * File:		loader.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdlib.h>
#include "elf.h"
#include "memory/pageframe.h"
#include "process.h"
#include "loader.h"


// Creates an enivronment and its stack
static inline struct ProcessEnvironment* _loader_create_environment(size_t stack_pages);

static const void* stack_virtual_base = (void*)0xffff800000000000;

static inline struct ProcessEnvironment* _loader_create_environment(size_t stack_pages) {
	struct ProcessEnvironment* environment = malloc(sizeof(struct ProcessEnvironment));
	void* stack_physical_base = pageframe_request_pages(stack_pages);
	*environment = (struct ProcessEnvironment){
		.physical_address = stack_physical_base - stack_pages * MEMORY_PAGE_SIZE,
		.virtual_address = (void*)stack_virtual_base,
		.pages = stack_pages
	};
	return environment;
}

void loader_execute_elf(void* module) {
	struct ProcessEnvironment* environment = _loader_create_environment(LOADER_DEFAULT_STACK_PAGES);
	struct ELF64Header* header = module;
	struct ELF64ProgramHeader* program_headers = module + header->e_phoff;
	for(size_t i = 0; i < header->e_phnum; i++) {
		struct ELF64ProgramHeader* segment = program_headers + i;

	}
	
}