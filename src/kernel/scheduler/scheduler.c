/*
 * File:		scheduler.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "log.h"
#include "x86_64/gdt.h"
#include "scheduler.h"

static struct SchedulerContext s_idle_context = {
	.context_frame.rip = (uint64_t)scheduler_idle,
	.context_frame.cs = GDT_OFFSET_KERNEL_CODE,
	.context_frame.rflags = RFLAGS,
	.context_frame.ss = GDT_OFFSET_KERNEL_DATA,
};

__attribute__((hot)) struct SchedulerContext* scheduler_next_task(struct SchedulerContext* context) {
	if(context == 0){
		log_default("null context\n");
	}
	return &s_idle_context;
}