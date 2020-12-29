/*
 * File:		memory.cpp
 * Description:	Unknown
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "memory.h"

namespace libvos {


template<typename T>
void memset(void* start, T value, size_t size) {
	size_t size_bytes = size % sizeof(T);
	size_t size_words = size - size_bytes;
	for(T* ptr = (T*)start; ptr < (T*)start + size_words / sizeof(T); ptr++){
		*ptr = value;
	}
	byte_t* value_ptr = (byte_t*)&value;
	for(byte_t* ptr = (byte_t*)start + size_words; ptr < (byte_t*)start + size; ptr++){
		*ptr = *value_ptr;
	}
}
template void memset(void*, ulong_t, size_t);
template void memset(void*, uint_t, size_t);
template void memset(void*, ushort_t, size_t);
template void memset(void*, byte_t, size_t);
extern "C" void memset_ulong(void* start, ulong_t value, size_t size)		{ memset(start, value, size); }
extern "C" void memset_uint(void* start, uint_t value, size_t size)		{ memset(start, value, size); }
extern "C" void memset_ushort(void* start, ushort_t value, size_t size)	{ memset(start, value, size); }
extern "C" void memset_byte(void* start, byte_t value, size_t size)		{ memset(start, value, size); }


}