/*
 * File:		scheduler.h
 * Description:	The main scheduler
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


struct Process {
	uint64_t	id;
};

struct SchedulerContext {
	void*			rsp_task;
	struct Process	task;
};

struct SchedulerNode {
	struct SchedulerContext	context;
	struct SchedulerTask*	next;
};

// Begins scheduling tasks to the processor calling this
// There is no going back once the scheduler has been entered
void scheduler_entry();

// Gets the next task and advances the task ring
struct SchedulerContext* scheduler_next_task(struct SchedulerContext* context);