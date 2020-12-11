/*
 * File:		bitmap.h
 * Desciption:	Map of bits data structure for each page
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

struct Bitmap {
	byte*	buffer;
	size_t	size;
};

// Initalize bitmap with size and buffer. Will also clear buffer.
void bitmap_initalize(struct Bitmap* bitmap, void* buffer, size_t size);

// Get value based on index for the bitmap
bool bitmap_get(struct Bitmap* bitmap, ulong index);

// Set value based on index for the bitmap
void bitmap_set(struct Bitmap* bitmap, ulong index, bool value);