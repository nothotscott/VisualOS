/*
 * File:		string.cpp
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "string.h"

using namespace libvos;

String::String() : m_str { NULL }, m_size { 0 } {
}

// TODO find size
String::String(char* str) : m_str { str }, m_size { 0 } {
}

String::String(char* str, size_t size) : m_str { str }, m_size { size } {
}


char from_int_buffer[128];	// TODO Dynamic memory
String temp_return;			// TODO Dynamic memory
template<typename T>
String* String::from_int(T value){
	byte negative = false;
	byte size = 0;
	// Test negativity
	if (value < 0){
		negative = true;
		value *= -1;
		size = 1;
	}
	// Test size of decimal
	ulong temp64 = (ulong)value;
	while(temp64 / 10 > 0) {
		temp64 /= 10;
		size++;
	}
	// Extract each digit
	temp64 = value;
	byte index = 0;
	byte digit;
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
	temp_return = String((char*)&from_int_buffer, (size_t)size);
	return (String*)&temp_return;
}
template String* String::from_int<>(long long);

char* String::get_str(){
	return m_str;
}


// Wrappers
extern "C" {
	struct String* string_from_int(long long value)	{ return String::from_int(value); }
	char* string_get_str(struct String* string)		{ return string->get_str(); }
}
