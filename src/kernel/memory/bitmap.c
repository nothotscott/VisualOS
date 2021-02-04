/*
 * File:		bitmap.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <memory.h>
#include "bitmap.h"


void bitmap_initialize(struct Bitmap* bitmap, void* buffer, size_t size) {
	bitmap->buffer = buffer;
	bitmap->size = size;
	memset_byte(bitmap->buffer, 0, size);
}

inline size_t bitmap_adjusted_size(struct Bitmap* bitmap) {
	return bitmap->size * BITMAP_SCALE;
}

bool bitmap_get(struct Bitmap* bitmap, size_t index) {
	if(index > bitmap_adjusted_size(bitmap)) {	// out of range
		return false;
	}
	ulong_t byte_index = index / BITMAP_SCALE;
	byte_t bit_index = index % BITMAP_SCALE;
	byte_t bit_selector = 0b10000000 >> bit_index;
	if((bitmap->buffer[byte_index] & bit_selector) != 0){
		return true;
	}
	return false;
}

bool bitmap_set(struct Bitmap* bitmap, size_t index, bool value) {
	if(index > bitmap_adjusted_size(bitmap)) {	// out of range
		return false;
	}
	ulong_t byte_index = index / BITMAP_SCALE;
	byte_t bit_index = index % BITMAP_SCALE;
	byte_t bit_selector = 0b10000000 >> bit_index;
	if(value){
		bitmap->buffer[byte_index] |= bit_selector;		// force the bit on
	} else {
		bitmap->buffer[byte_index] &= ~bit_selector;	// force the bit off
	}
	return true;
}