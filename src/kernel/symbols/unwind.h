/*
 * File:		unwind.h
 * Description:	Unwinds the stack
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define UNWIND_MAX_FRAMES	16


struct StackFrame {
	struct StackFrame*	rbp;
	uint64_t			rip;
} __attribute__((packed));


// Unwinds the stack starting at [rbp] and performs [callback]
void unwind(uint64_t rbp, void (*callback)(size_t frame_num, uint64_t rip));