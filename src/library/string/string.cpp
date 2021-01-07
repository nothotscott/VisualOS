/*
 * File:		string.cpp
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "libvos.h"
#include "conversion.h"
#include "format.h"
#include "string.h"


namespace libvos {

String::String() : m_str { NULL }, m_size { 0 } {
}

// TODO find size
String::String(char* str) : m_str { str }, m_size { 0 } {
}

String::String(char* str, size_t size) : m_str { str }, m_size { size } {
}

// TODO the fixed version will be removed when malloc is official
// TODO _from_int_dynamic
char from_int_buffer[128];	// TODO Dynamic memory
String from_int_return;		// TODO Dynamic memory
template<typename T>
String* String::from_int(T value){
	size_t size = _from_int_size(value);
	_from_int(&from_int_buffer, size, value);
	((char*)&from_int_buffer)[size] = 0;
	from_int_return = String((char*)&from_int_buffer, size);
	return (String*)&from_int_return;
}
/*String* String::from_int(T value){
	char* ptr;
	size_t size = _from_int_dynamic(value, (void**)&ptr);
	return new String(ptr, size);
}
template String* String::from_int(slong_t);*/

template<typename T>
String* String::from_decimal(T value, size_t places) {
	char* ptr;
	size_t size = _from_decimal_dynamic(value, places, (void**)&ptr);
	return new String(ptr, size);
}
template String* String::from_decimal(float, size_t);
template String* String::from_decimal(double, size_t);

template<typename T>
String* String::from_hex(T value) {
	char* ptr;
	size_t size = _from_hex_dynamic(value, (void**)&ptr);
	return new String(ptr, size);
}
template String* String::from_hex(byte_t);
template String* String::from_hex(ushort_t);
template String* String::from_hex(uint_t);
template String* String::from_hex(ulong_t);

String* String::from_format(const char* format, ...) {
	va_list args;
	va_start(args, format);
	char* ptr;
	size_t size = _format_dynamic((void**)&ptr, format, args);
	va_end(args);
	return new String(ptr, size);
}


void String::set_str(char* str) {
	m_str = str;
	// TODO find size
}
void String::set_str(char* str, size_t size) {
	m_str = str;
	m_size = size;
}

char* String::get_str(){
	return m_str;
}



// Wrappers
extern "C" {
	char* string_get_str(struct String* string)	{ return string->get_str(); }

	char* string_str_from_int(slong_t value)					{ return String::from_int(value)->get_str(); }
	char* string_str_from_decimal(double value, size_t places)	{ return String::from_decimal(value, places)->get_str(); }
	//char* string_str_from_decimal(double value, size_t places)	{ char* ptr; _from_decimal_dynamic(value, places, (void**)&ptr); return ptr; }
	char* string_str_from_byte(byte_t value)					{ char* ptr; _from_hex_dynamic(value, (void**)&ptr); return ptr; }
	char* string_str_from_ushort(ushort_t value)				{ char* ptr; _from_hex_dynamic(value, (void**)&ptr); return ptr; }
	char* string_str_from_uint(uint_t value)					{ char* ptr; _from_hex_dynamic(value, (void**)&ptr); return ptr; }
	char* string_str_from_ulong(ulong_t value)					{ char* ptr; _from_hex_dynamic(value, (void**)&ptr); return ptr; }

	char* string_str_from_format(const char* format, va_list args)	{ char* ptr; _format_dynamic((void**)&ptr, format, args); return ptr; }

	size_t string_str_format(char* str, const char* format, size_t limit, va_list args)	{ return _format(str, format, limit, args); }
}

}