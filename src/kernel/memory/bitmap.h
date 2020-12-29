/*
 * File:		bitmap.h
 * Description:	Map an index to a bit
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

// TODO port to library?

#define BITMAP_SCALE	sizeof(size_t)

struct Bitmap {
	byte_t*	buffer;	
	size_t	size;	// Real size
};

// *** Class functions  *** //

// Initialize [bitmap] with real [size] of [buffer]. Will also clear buffer.
void bitmap_initialize(struct Bitmap* bitmap, void* buffer, size_t size);

// Gets the adjusted [bitmap] size, or 8 times [bitmap] size
size_t bitmap_adjusted_size(struct Bitmap* bitmap);

// Get value based on [index] for the [bitmap]
bool bitmap_get(struct Bitmap* bitmap, size_t index);

// Set [value] based on [index] for the [bitmap] and returns true on success
bool bitmap_set(struct Bitmap* bitmap, size_t index, bool value);