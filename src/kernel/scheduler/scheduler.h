/*
 * File:		scheduler.h
 * Description:	The main scheduler
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "x86_64/isr.h"

struct Process {
	uint64_t	id;
};

struct SchedulerContextFrame {
	uint64_t	rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t	rip, cs, rflags, rsp, ss;
	uint64_t	self;
};
struct SchedulerContext {
	struct SchedulerContextFrame	context_frame;
	// Not needed in assembly past this point
	struct Process					task;
} __attribute__((packed));

struct SchedulerNode {
	struct SchedulerContext	context;
	struct SchedulerTask*	next;
};

// Begins scheduling tasks to the processor calling this
// There is no going back once the scheduler has been entered
void scheduler_entry();

// Causes the processor to halt until it's interrupted
void scheduler_idle();

// Gets the next task and advances the task ring
struct SchedulerContext* scheduler_next_task(struct SchedulerContext* context);