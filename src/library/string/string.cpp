/*
 * File:		string.cpp
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "string.h"
#include "libvos.h"

namespace libvos {

String::String() : m_str { NULL }, m_size { 0 } {
}

// TODO find size
String::String(char* str) : m_str { str }, m_size { 0 } {
}

String::String(char* str, size_t size) : m_str { str }, m_size { size } {
}

// TODO Dynamic memory char* _from_int(T value) C++ function

char from_int_buffer[128];	// TODO Dynamic memory
String from_int_return;		// TODO Dynamic memory
template<typename T>
String* String::from_int(T value){
	byte_t negative = false;
	byte_t size = 0;
	// Test negativity
	if (value < 0){
		negative = true;
		value *= -1;
		size = 1;
	}
	// Test size of decimal
	ulong_t temp64 = (ulong_t)value;
	while(temp64 / 10 > 0) {
		temp64 /= 10;
		size++;
	}
	// Extract each digit
	temp64 = value;
	byte_t index = 0;
	byte_t digit;
	while(temp64 / 10 > 0){
		digit = temp64 % 10;
		temp64 /= 10;
		from_int_buffer[size - index] = digit + '0';	// ASCII offset
		index++;
	}
	digit = temp64 % 10;
	from_int_buffer[size - index] = digit + '0';
	if(negative) {
		from_int_buffer[0] = '-';
	}
	// Finalize
	size++;
	from_int_buffer[size] = 0;	// null terminate
	from_int_return = String((char*)&from_int_buffer, (size_t)size);
	return (String*)&from_int_return;
}
template String* String::from_int<>(slong_t);

// TODO Dynamic memory char* _from_decimal(T value) C++ function

template<typename T>
String* String::from_decimal(T value, byte_t places) {
	String* int_string = String::from_int((slong_t)value);
	return int_string;
}
template String* String::from_decimal(double, byte_t);


template<typename T>
char* _from_hex(T value) {
	byte_t size = sizeof(T) * 2 - 1;
	char* buffer = new char[size];
	T* val_ptr = &value;
	byte_t* ptr;
	byte_t nibble;
	for (byte_t i = 0; i < size; i++){
		ptr = (byte_t*)val_ptr + i;
		nibble = (*ptr & 0xF0) >> 4;
		buffer[size - (i * 2 + 1)] = nibble + (nibble > 9 ? 55 : 48);
		nibble = *ptr & 0x0F;
		buffer[size - (i * 2)] = nibble + (nibble > 9 ? 55 : 48);
	}
	size++;
	buffer[size] = 0;
	return buffer;
}
template<typename T>
String* String::from_hex(T value) {
	char* str = _from_hex(value);
	return new String(str);
}
template String* String::from_hex(byte_t);
template String* String::from_hex(ushort_t);
template String* String::from_hex(uint_t);
template String* String::from_hex(ulong_t);


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

	// TODO Dynamic memory invokes templated "_from" functions
	char* string_str_from_int(slong_t value)						{ return String::from_int(value)->get_str(); }
	char* string_str_from_decimal(double value, byte_t places)		{ return String::from_decimal(value, places)->get_str(); }
	char* string_str_from_byte(byte_t value)						{ return _from_hex(value); }
	char* string_str_from_ushort(ushort_t value)					{ return _from_hex(value); }
	char* string_str_from_uint(uint_t value)						{ return _from_hex(value); }
	char* string_str_from_ulong(ulong_t value)						{ return _from_hex(value); }
}

}