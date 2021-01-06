/*
 * File:		format.hpp
 * Description:	Formats strings in a printf format
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "libvos.h"
#include "conversion.h"

#define STRING_FORMAT_TOKEN	'%'


#ifdef __cplusplus
namespace libvos {

// Writes the formatted [format] to [buffer]
__attribute__((always_inline)) inline size_t _format(void* buffer, const char* format, va_list args) {
	char* str = (char*)buffer;
	size_t size = 0;	// buffer index
	size_t i = 0;		// format index
	while(format[i] != '\0') {
		if(format[i] != STRING_FORMAT_TOKEN) {
			__PUTC(format[i], str, size);
			i++;
			continue;
		}
		i++;	// move past %
		ulong_t uvalue64;
		slong_t svalue64;
		double value_ieee754;
		size_t l_size;	// local size
		switch(format[i]) {
			case 'd':	// signed digits
				svalue64 = (slong_t)va_arg(args, slong_t);
				l_size = _from_int_size(svalue64);
				if(buffer != nullptr) { _from_int(str + size, l_size, svalue64); }
				size += l_size;
				break;
			case 'x':	// hex
				uvalue64 = (ulong_t)va_arg(args, ulong_t);
				l_size = _from_hex_size(uvalue64);
				if(buffer != nullptr) { _from_hex(str + size, l_size, uvalue64); }
				size += l_size;
				break;
			default:	// %% or invalid specifier
				__PUTC(format[i], str, size);
				break;
		}
		i++;
	}
	__PUTC('\0', str, size);
	return size;
}

// Calculates the size of a format in advance
__attribute__((always_inline)) inline size_t _format_size(const char* format, va_list args) {
	va_list args_preliminary;
	va_copy(args_preliminary, args);
	return _format(nullptr, format, args_preliminary);
}

// Writes the formatted [format] to [buffer]
__attribute__((always_inline)) inline size_t _format_dynamic(void** out, const char* format, va_list args) {
	size_t size = _format_size(format, args);
	void* buffer = malloc(size);
	_format(buffer, format, args);
	*out = buffer;
	return size;
}



}
#endif