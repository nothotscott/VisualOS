/*
 * File:		scheduler.h
 * Description:	The main scheduler
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "x86_64/isr.h"

#define SCHEDULER_QUEUES_NUM	_SCHEDULER_QUEUES_NUM

struct Process {
	uint64_t	pid;
} __attribute__((packed));

struct Thread {
	uint64_t	tid;
	void*		parent;
} __attribute__((packed));

struct SchedulerContextFrame {
	struct SchedulerContextFrameRegisters {
		uint64_t	rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
	}			general;
	uint64_t	rip, cs, rflags, rsp, ss;
	uint64_t	self;
} __attribute__((packed));
struct SchedulerContext {
	struct SchedulerContextFrame	context_frame;
	uint64_t						flags;
	uint64_t						error_code;
	// Not needed in assembly past this point
	struct Thread					task;
} __attribute__((packed));

struct SchedulerNode {
	struct SchedulerContext	context;
	size_t					queue_num;
	struct SchedulerNode*	previous;
	struct SchedulerNode*	next;
} __attribute__((packed));


struct SchedulerTaskInitialState {
	struct SchedulerContextFrameRegisters	general;	// general purpose registers
	void*									entry;		// start point
	void*									stack;		// initial stack pointer
	uint8_t									rpl;		// requested privilege level
};

enum SchedulerQueueNumber {
	SCHEDULER_QUEUE_PRIORITY,
	SCHEDULER_QUEUE_REGULAR,
	SCHEDULER_QUEUE_BATCH,
	_SCHEDULER_QUEUES_NUM
};

enum SchedulerQueueFlagBits {
	SCHEDULER_QUEUE_FLAG_LOCKED	= 0,
};


enum SchedulerContextFlagBits {
	SCHEDULER_CONTEXT_FLAG_LOCKED	= 0,
	SCHEDULER_CONTEXT_FLAG_FINISHED	= 1,
};


// Initializes scheduler
void scheduler_init();

// Gets the next task and advances the task ring
struct SchedulerNode* scheduler_next_task(struct SchedulerNode* current);

// Adds the task with [initial_state] to the queue of [queue_num]
struct SchedulerNode* scheduler_add_task(struct SchedulerTaskInitialState* initial_state, enum SchedulerQueueNumber queue_num);
// Adds a task with default initial state and [entry] start point into the queue of [queue_num]
struct SchedulerNode* scheduler_add_task_default(void* entry, size_t code_pages, enum SchedulerQueueNumber queue_num);

// Frees the resources of task [node] and dequeues it from its queue. Assumes [node] has already acquired a lock.
void scheduler_free_task(struct SchedulerNode* node);

// *** From scheduler.asm *** //

// Begins scheduling tasks once unlocked for the processor calling this
// There is no going back once the scheduler has been entered
// [should_unlock] will unlock a spin loop
void scheduler_entry(bool should_unlock);

// Causes the processor to halt until it's interrupted
void scheduler_idle();