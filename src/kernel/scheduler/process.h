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
#define PROCESS_RPL					3

struct ProcessEnvironment {
	void*						virtual_address;
	void*						physical_address;
	uint64_t					flags;
	size_t						pages;
	struct ProcessEnvironment*	next;
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
	void*						entry;
	struct ProcessEnvironment*	environment;
} __attribute__((packed));

enum ProcessFlagBit {
	PROCESS_FLAG_LOCKED
};
enum ProcessEnvironmentFlagBit {
	PROCESS_ENVIRONMENT_EXECUTABLE	= 1,
	PROCESS_ENVIRONMENT_WRITABLE	= 2,
	PROCESS_ENVIRONMENT_READABLE	= 3,
	PROCESS_ENVIRONMENT_STACK		= 4,
};

// Initializes by setting the root process
void process_init();

// Creates a new process as a subprocess of [parent] in [environment] and adds the main thread to [queue_num]
// If [parent] is null, it will use the root process
// NOTE: [environment] is a linked list and should NOT be passed as a stack variable
void process_create_new(struct Process* parent, struct ProcessEnvironment* environment, void* entry, enum SchedulerQueueNumber queue_num);