/*
 * File:		process.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include <stdlib.h>
#include "memory/memory.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "x86_64/atomic.h"
#include "scheduler.h"
#include "process.h"

// Spinlocks/unlock [process] on its PROCESS_FLAG_LOCKED flag
static inline void _process_spinlock(struct Process* process);
static inline bool _process_unlock(struct Process* process);
// Adds [process] to [parent]'s child list. Does not set [process]'s parent. Assumes a lock is acquired on [parent]
static inline void _process_add(struct Process* parent, struct Process* process);
// Creates a page map for [environment] and returns the PML4
static inline struct PageTable* _process_pagemap(struct ProcessEnvironment* environment);


static struct Process s_root = {
	.pid = 0,
	.pml4 = 0,
	.flags = 0,
	.parent = NULL,
	.next = NULL,
	.child_list = NULL,
};
static uint64_t	s_processes_num = 0;
static uint64_t s_pid_accumulator = 1;


// *** Internal Functions *** ///

static inline void _process_spinlock(struct Process* process) {
	atomic_spinlock(&process->flags, PROCESS_FLAG_LOCKED);
}
static inline bool _process_unlock(struct Process* process) {
	atomic_unlock(&process->flags, PROCESS_FLAG_LOCKED);
}

static inline void _process_add(struct Process* parent, struct Process* process) {
	if(parent == NULL) {
		parent = &s_root;
	}
	struct Process* last = parent->child_list;
	while(last != NULL) {
		last = last->next;
	}
	atomic_accumulate(&s_processes_num, 1);
}

static inline struct PageTable* _process_pagemap(struct ProcessEnvironment* environment) {
	struct PageTable* pml4 = (struct PageTable*)pageframe_request();
	if(environment->stack_pages) {
		paging_map(pml4, PROCESS_STACK_VIRTUAL_BASE, environment->stack_physical, environment->stack_pages);
		paging_set_attribute(pml4, PROCESS_STACK_VIRTUAL_BASE, environment->stack_pages, PAGE_DIRECTORY_WRITABLE, true);
	}
	if(environment->text_pages) {
		paging_map(pml4, environment->text_virtual, environment->text_physical, environment->text_pages);
	}
	if(environment->data_pages) {
		paging_map(pml4, environment->data_virtual, environment->data_physical, environment->data_pages);
		paging_set_attribute(pml4, environment->data_virtual, environment->data_pages, PAGE_DIRECTORY_WRITABLE, true);
	}
	if(environment->bss_pages) {
		paging_map(pml4, environment->bss_virtual, environment->bss_physical, environment->bss_pages);
		paging_set_attribute(pml4, environment->bss_virtual, environment->bss_pages, PAGE_DIRECTORY_WRITABLE, true);
	}
	if(environment->rodata_pages) {
		paging_map(pml4, environment->rodata_virtual, environment->rodata_physical, environment->rodata_pages);
	}
	return pml4;
}

// *** Regular Functions *** ///

void process_init() {
	s_root.pml4 = paging_get_pagetable_l4();
}

void process_create_new(struct Process* parent, struct ProcessEnvironment* environment, enum SchedulerQueueNumber queue_num) {
	struct Process* process = malloc(sizeof(struct Process));
	if(parent == NULL) {
		parent = &s_root;
	}
	_process_spinlock(parent);
	// Create process
	*process = (struct Process){
		.pid = atomic_accumulate(&s_pid_accumulator, 1),
		.pml4 = _process_pagemap(environment),
		.flags = 0,
		.parent = parent,
		.next = NULL,
		.child_list = NULL
	};
	memcpy(&process->environment, environment, sizeof(struct ProcessEnvironment));
	// Create the context & thread
	struct SchedulerTaskInitialState new_state = (struct SchedulerTaskInitialState){
		.entry = environment->entry,
		.stack = (void*)PROCESS_STACK_VIRTUAL_BASE,
		.rpl = PROCESS_RPL
	};
	// Set the process's thread context and exit
	process->thread_contexts = &scheduler_add_task(&new_state, queue_num)->context;
	_process_add(parent, process);
	_process_unlock(parent);
}