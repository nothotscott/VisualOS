/*
 * File:		pageframe.h
 * Description:	Keeps track of individual page frames
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "bootloader.h"
#include "bitmap.h"
#include "memory.h"

#define PAGEFRAME_INITIAL_RESERVE_PAGES	256


// Initalizes pageframes by discovering useable memory segments passed by the bootloader
void pageframe_init();

// Checks if a page is in [state] at [index] and sets it to [state]. 
// Will return true if it's state is already in [state].
bool pageframe_manipulate(uint64_t index, bool state);

// Will find, lock, and returns [pages] free pages for use
void* pageframe_request();
void* pageframe_request_pages(size_t pages);

// Free pages at [address] and thereafter [pages] times and track used
void pageframe_free(void* address, size_t pages);

// Make pages in-use at [address] and therafter [pages] times and track used
void pageframe_lock(void* address, size_t pages);

// Free pages at [address] and thereafter [pages] times and track reserved
void pageframe_unreserve(void* address, size_t pages);

// Make pages in-use at [address] and therafter [pages] times and track reserved
void pageframe_reserve(void* address, size_t pages);
// Shortcut for reserving a page at [address] of [size]
void pageframe_reserve_size(void* address, size_t size);