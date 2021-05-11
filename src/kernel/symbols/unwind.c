/*
 * File:		unwind.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "unwind.h"


void unwind(uint64_t rbp, void (*callback)(size_t frame_num, uint64_t rip)) {
	struct StackFrame* stack_frame = (struct StackFrame*)rbp;
	for(size_t i = 0; stack_frame->rbp != 0 && i < UNWIND_MAX_FRAMES; i++) {
		callback(i, stack_frame->rip);
		stack_frame = stack_frame->rbp;
	}
}