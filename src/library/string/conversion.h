/*
 * File:		convert.h
 * Description:	Inline functions to help convert strings.
				Note: size functions do not include null terminator,
				only dynamic functions will give the real size
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "libvos.h"


#ifdef __cplusplus
namespace libvos {
#endif


// Suite that converts [value] into an integer at [buffer]
#ifndef __cplusplus
#undef	T
#define T	ulong_t
#endif
#ifdef __cplusplus
template<typename T>
#endif
__attribute__((always_inline)) inline size_t _from_int_size(T value) {
	bool negative = value < 0;
	size_t size = negative;
	if(negative) {
		value *= -1;
	}
	ulong_t temp64 = (ulong_t)value;
	while(temp64 / 10 > 0) {
		temp64 /= 10;
		size++;
	}
	return size + 1;
}
#ifdef __cplusplus
template<typename T>
#endif
__attribute__((always_inline)) inline void _from_int(void* buffer, size_t size, T value) {
	char* str = (char*)buffer;
	bool negative = value < 0;
	if(negative) {
		value *= -1;
	}
	// Extract each digit
	ulong_t temp64 = value;
	byte_t index = 0;
	byte_t digit;
	while(temp64 / 10 > 0){
		digit = temp64 % 10;
		temp64 /= 10;
		str[size - index - 1] = digit + '0';	// ASCII offset
		index++;
	}
	digit = temp64 % 10;
	str[size - index - 1] = digit + '0';
	if(negative) {
		str[0] = '-';
	}
}
#ifdef __cplusplus
template<typename T>
#endif
__attribute__((always_inline)) inline size_t _from_int_dynamic(T value, void** out) {
	size_t size = _from_int_size(value);
	void* buffer = malloc(size);
	_from_int(buffer, size, value);
	((char*)buffer)[size] = 0;
	*out = buffer;
	return size;
}


// Suite that converts [value] into an decimal at [buffer]
#ifndef __cplusplus
#undef	T
#define T	double
#endif
#ifdef __cplusplus
template<typename T>
#else
#define T double
#endif
__attribute__((always_inline)) inline size_t _from_decimal_size(T value, size_t places) {
	return places;
}
#ifdef __cplusplus
template<typename T>
#else
#define T double
#endif
__attribute__((always_inline)) inline void _from_decimal(void* buffer, size_t int_size, T value, size_t places) {
	char* str = (char*)buffer;
	_from_int(str, int_size, value);
	if(value < 0) {
		value *= -1;
	}
	str[int_size] = '.';
	T dec_value = value - (ulong_t)value;
	for(size_t i = 0; i < places; i++) {
		dec_value *= 10;
		str[int_size + 1 + i] = (byte_t)dec_value + '0';
		dec_value -= (byte_t)dec_value;
	}
}
#ifdef __cplusplus
template<typename T>
#else
#define T double
#endif
__attribute__((always_inline)) inline size_t _from_decimal_dynamic(T value, size_t places, void** out) {
	size_t int_size = _from_int_size(value);
	size_t dec_size = _from_decimal_size(value, places);
	size_t size = int_size + dec_size + 1;
	void* buffer = malloc(size);
	_from_decimal(buffer, int_size, value, dec_size);
	((char*)buffer)[size] = 0;
	*out = buffer;
	return size;
}


// Suite that converts [value] into hexidecimal at [buffer]
#ifndef __cplusplus
#undef	T
#define T	ulong_t
#endif
#ifdef __cplusplus
template<typename T>
#endif
__attribute__((always_inline)) inline size_t _from_hex_size(T value) {
	return sizeof(T) * 2;
}
#ifdef __cplusplus
template<typename T>
#endif
__attribute__((always_inline)) inline void _from_hex(void* buffer, size_t size, T value) {
	char* str = (char*)buffer;
	//size--;
	for (byte_t i = 1; i < size; i+=2) {
		byte_t byte = value & 0xff;
		byte_t nibble = (byte & 0xf0) >> 4;
		str[size - i - 1] = nibble + (nibble > 9 ? 55 : 48);
		nibble = byte & 0x0f;
		str[size - i - 0] = nibble + (nibble > 9 ? 55 : 48);
		value >>= 8;
	}
}
#ifdef __cplusplus
template<typename T>
#endif
__attribute__((always_inline)) inline size_t _from_hex_dynamic(T value, void** out) {
	size_t size = _from_hex_size(value);
	void* buffer = malloc(size);
	_from_hex(buffer, size, value);
	((char*)buffer)[size] = 0;
	*out = buffer;
	return size;
}


#ifdef __cplusplus
}
#else
#undef	T
#endif