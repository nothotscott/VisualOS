/*
 * File:		stivale2.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "stivale2.h"
#include "memory/paging.h"

static uint8_t s_stack[MEMORY_PAGE_SIZE * STIVALE2_STACK_PAGES] __attribute__((aligned (MEMORY_PAGE_SIZE)));	// Initial BSP stack

static struct Stivale2HeaderTagFrameBuffer s_tag0 = {
	.tag.identifier = STIVALE2_TAG_ID_HEADER_FRAME_BUFFER,
	.tag.next = 0
};

__attribute__((section(".stivale2hdr"), used))
static struct Stivale2Header s_stivale2_header = {
	.entry_point = 0,
	.stack = (uint64_t)s_stack + sizeof(s_stack),
	.flags = 0,
	.tags = (uint64_t)&s_tag0
};