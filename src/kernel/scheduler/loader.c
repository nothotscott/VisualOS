/*
 * File:		loader.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdlib.h>
#include <string.h>
#include "log.h"
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
		.pages = stack_pages,
		.next = NULL
	};
	return environment;
}

void loader_execute_elf(void* module) {
	struct ProcessEnvironment* environment = _loader_create_environment(LOADER_DEFAULT_STACK_PAGES);
	struct ProcessEnvironment* environment_last = environment;
	struct ELF64Header* header = module;
	struct ELF64ProgramHeader* program_headers = module + header->e_phoff;
	for(size_t i = 0; i < header->e_phnum; i++) {
		struct ELF64ProgramHeader* segment = program_headers + i;
		void* physical_address = malloc(segment->p_memsz);
		memcpy(physical_address, module + segment->p_offset, segment->p_memsz);
		environment_last->next = malloc(sizeof(struct ProcessEnvironment));
		*environment_last->next = (struct ProcessEnvironment){
			.physical_address = physical_address,
			.virtual_address = (void*)segment->p_vaddr,
			.pages = NEAREST_PAGE(segment->p_memsz),
			.next = NULL
		};
		environment_last = environment_last->next;
	}
	process_create_new(NULL, environment, (void*)header->e_entry, PROCESS_DEFAULT_QUEUE);
}