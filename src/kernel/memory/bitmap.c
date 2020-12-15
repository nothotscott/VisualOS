/*
 * File:		bitmap.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "bitmap.h"


void bitmap_initialize(struct Bitmap* bitmap, void* buffer, size_t size) {
	bitmap->buffer = buffer;
	bitmap->size = size;
	// TODO use memset to be more efficient
	for(ulong i = 0; i < size; i++){
		bitmap->buffer[i] = 0;
	}
}

size_t bitmap_adjusted_size(struct Bitmap* bitmap) {
	return bitmap->size * BITMAP_SCALE;
}

bool bitmap_get(struct Bitmap* bitmap, ulong index) {
	ulong byte_index = index / BITMAP_SCALE;
	byte bit_index = index % BITMAP_SCALE;
	byte bit_selector = 0b10000000 >> bit_index;
	if((bitmap->buffer[byte_index] & bit_selector) != 0){
		return true;
	}
	return false;
}

void bitmap_set(struct Bitmap* bitmap, ulong index, bool value) {
	ulong byte_index = index / BITMAP_SCALE;
	byte bit_index = index % BITMAP_SCALE;
	byte bit_selector = 0b10000000 >> bit_index;
	if(value){
		bitmap->buffer[byte_index] |= bit_selector;		// force the bit on
	} else {
		bitmap->buffer[byte_index] &= ~bit_selector;	// force the bit off
	}
}