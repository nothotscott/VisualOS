/*
 * File:		pageframe.h
 * Description:	Keeps track of individual page frames
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "bootloader.h"
#include "bitmap.h"
#include "memory.h"

#define PAGEFRAME_INITIAL_RESERVE_PAGES	256

enum PageframeMutexBit {
	PAGEFRAME_MUTEX_REQUEST,
	PAGEFRAME_MUTEX_LOCK,
	PAGEFRAME_MUTEX_FREE
};

// Initalizes pageframes by discovering useable memory segments passed by the bootloader
void pageframe_init();

// Checks if a page is in [state] at [index] and sets it to [state]. 
// Will return true if it's state is already in [state].
bool pageframe_manipulate(uint64_t index, bool state);

// Will find, lock, and returns [pages] free pages for use
void* pageframe_request();
void* pageframe_request_pages(size_t pages);

// Free pages at [virtual_address] and thereafter [pages] times and track used
void pageframe_free(void* physical_address, size_t pages);

// Make pages in-use at [physical_address] and therafter [pages] times and track used
void pageframe_lock(void* physical_address, size_t pages);

// Free pages at [physical_address] and thereafter [pages] times and track reserved
void pageframe_unreserve(void* physical_address, size_t pages);

// Make pages in-use at [physical_address] and therafter [pages] times and track reserved
void pageframe_reserve(void* physical_address, size_t pages);
// Shortcut for reserving a page at [physical_address] of [size]
void pageframe_reserve_size(void* physical_address, size_t size);