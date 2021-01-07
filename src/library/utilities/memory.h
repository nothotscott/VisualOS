/*
 * File:		memory.h
 * Description:	Memory utilities that do not involve syscalls
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once


#ifdef __cplusplus
namespace libvos {
#endif

// Sets the memory at [start] to [value] up to, but not
// including [size] bytes. Only works with standard integers
#ifdef __cplusplus
template<typename T>
void memset(void* start, T value, size_t size);
#else
void memset_ulong(void*, ulong_t, size_t);
void memset_uint(void*, uint_t, size_t);
void memset_ushort(void*, ushort_t, size_t);
void memset_byte(void*, byte_t, size_t);
#endif

// Compares content of location [a] against [b] for [size]
#ifdef __cplusplus
extern "C"
#endif
int memcmp(void* a, void* b, size_t size);

#ifdef __cplusplus
} // namespace
#endif
