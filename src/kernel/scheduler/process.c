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
	memset(pml4, 0, MEMORY_PAGE_SIZE);
	while(environment != NULL) {
		paging_map(pml4, environment->virtual_address, environment->physical_address, environment->pages);
		if(environment->flags & (1 << PROCESS_ENVIRONMENT_WRITABLE)) {
			paging_set_attribute(pml4, environment->virtual_address, environment->pages, PAGE_DIRECTORY_WRITABLE, true);
		}
		if(environment->flags & ( 1 << PROCESS_ENVIRONMENT_STACK)) {
			// TODO map page to thread
		}
		environment = environment->next;
	}
	return pml4;
}

// *** Regular Functions *** ///

void process_init() {
	s_root.pml4 = paging_get_pagetable_l4();
}

void process_create_new(struct Process* parent, struct ProcessEnvironment* environment, void* entry, enum SchedulerQueueNumber queue_num) {
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
	// Find stack
	void* stack = NULL;
	struct ProcessEnvironment* current_environment = environment;
	while(current_environment != NULL) {
		if(current_environment->flags & (1 << PROCESS_ENVIRONMENT_STACK)) {
			stack = current_environment->virtual_address;
			break;
		}
		current_environment = current_environment->next;
	}
	// Create the context & thread
	struct SchedulerTaskInitialState new_state = (struct SchedulerTaskInitialState){
		.entry = entry,
		.stack = stack,
		.rpl = PROCESS_RPL
	};
	// Set the process's thread context and exit
	process->thread_contexts = &scheduler_add_task(&new_state, queue_num)->context;
	process->thread_contexts->task.process = process;
	_process_add(parent, process);
	_process_unlock(parent);
}