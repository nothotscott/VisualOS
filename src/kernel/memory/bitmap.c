/*
 * File:		bitmap.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "bitmap.h"


void bitmap_initialize(struct Bitmap* bitmap, void* buffer, size_t size) {
	bitmap->buffer = buffer;
	bitmap->size = size;
	memset(bitmap->buffer, 0, size);
}

inline size_t bitmap_adjusted_size(struct Bitmap* bitmap) {
	return bitmap->size * BITMAP_SCALE;
}

bool bitmap_get(struct Bitmap* bitmap, size_t index) {
	if(index > bitmap_adjusted_size(bitmap)) {	// out of range
		return false;
	}
	uint64_t byte_index = index / BITMAP_SCALE;
	uint8_t bit_index = index % BITMAP_SCALE;
	uint8_t bit_selector = 0b10000000 >> bit_index;
	if((bitmap->buffer[byte_index] & bit_selector) != 0){
		return true;
	}
	return false;
}

bool bitmap_set(struct Bitmap* bitmap, size_t index, bool value) {
	if(index > bitmap_adjusted_size(bitmap)) {	// out of range
		return false;
	}
	uint64_t byte_index = index / BITMAP_SCALE;
	uint8_t bit_index = index % BITMAP_SCALE;
	uint8_t bit_selector = 0b10000000 >> bit_index;
	if(value){
		bitmap->buffer[byte_index] |= bit_selector;		// force the bit on
	} else {
		bitmap->buffer[byte_index] &= ~bit_selector;	// force the bit off
	}
	return true;
}