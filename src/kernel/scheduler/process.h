/*
 * File:		process.h
 * Description:	VOS process library
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "memory/paging.h"
#include "scheduler.h"

#define PROCESS_DEFAULT_QUEUE		SCHEDULER_QUEUE_REGULAR

#define PROCESS_STACK_VIRTUAL_BASE	((void*)0x8000000000000000)
#define PROCESS_RPL					3

struct ProcessEnvironment {
	void*		entry;
	void*		stack_physical;
	size_t		stack_pages;
	void*		text_physical;	// code
	void*		text_virtual;
	size_t		text_pages;
	void*		data_physical;
	void*		data_virtual;
	size_t		data_pages;
	void*		bss_physical;	// block starting symbol
	void*		bss_virtual;
	size_t		bss_pages;
	void*		rodata_physical;
	void*		rodata_virtual;
	size_t		rodata_pages;
} __attribute__((packed));
struct Process {
	uint64_t					pid;
	struct PageTable*			pml4;	// Page map level 4
	uint64_t					flags;
	struct SchedulerContext*	thread_contexts;
	// Not needed in assembly past this point
	struct Process*				parent;
	struct Process*				next;
	struct Process*				child_list;
	struct ProcessEnvironment	environment;
} __attribute__((packed));

enum ProcessFlagBits {
	PROCESS_FLAG_LOCKED
};

// Initializes by setting the root process
void process_init();

// Creates a new process as a subprocess of [parent] in [environment] and adds the main thread to [queue_num]
// If [parent] is null, it will use the root process
void process_create_new(struct Process* parent, struct ProcessEnvironment* environment, enum SchedulerQueueNumber queue_num);