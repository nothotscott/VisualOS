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
void memset_ulong(void* start, ulong value, size_t size);
void memset_uint(void* start, uint value, size_t size);
void memset_ushort(void* start, ushort value, size_t size);
void memset(void* start, byte value, size_t size);
#endif


#ifdef __cplusplus
} // namespace
#endif
