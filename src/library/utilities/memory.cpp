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
	for(T* ptr = (T*)start; ptr < (T*)start + size_words; ptr++){
		*ptr = value;
	}
	byte* value_ptr = (byte*)&value;
	for(byte* ptr = (byte*)start + size_words; ptr < (byte*)start + size; ptr++){
		*ptr = *value_ptr;
	}
}
template void memset(void*, ulong, size_t);
template void memset(void*, uint, size_t);
template void memset(void*, ushort, size_t);
template void memset(void*, byte, size_t);
extern "C" void memset_ulong(void* start, ulong value, size_t size)		{ memset(start, value, size); }
extern "C" void memset_uint(void* start, uint value, size_t size)		{ memset(start, value, size); }
extern "C" void memset_ushort(void* start, ushort value, size_t size)	{ memset(start, value, size); }
extern "C" void memset(void* start, byte value, size_t size)			{ memset(start, value, size); }


}