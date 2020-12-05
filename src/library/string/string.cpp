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

// TODO Dynamic memory char* _from_int(T value) C++ function

char from_int_buffer[128];	// TODO Dynamic memory
String from_int_return;		// TODO Dynamic memory
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
	from_int_return = String((char*)&from_int_buffer, (size_t)size);
	return (String*)&from_int_return;
}
template String* String::from_int<>(slong);

// TODO Dynamic memory char* _from_decimal(T value) C++ function

template<typename T>
String* String::from_decimal(T value, byte places) {
	String* int_string = String::from_int((slong)value);
	return int_string;
}
template String* String::from_decimal(double, byte);

// TODO Dynamic memory char* _from_hex(T value) C++ function

char from_hex_buffer[128];	// TODO Dynamic memory
String from_hex_return;		// TODO Dynamic memory
template<typename T>
String* String::from_hex(T value) {
	byte size = sizeof(T) * 2 - 1;
	//char* buffer = new char[size];
	T* val_ptr = &value;
	byte* ptr;
	byte nibble;
	for (byte i = 0; i < size; i++){
		ptr = (byte*)val_ptr + i;
		nibble = (*ptr & 0xF0) >> 4;
		from_hex_buffer[size - (i * 2 + 1)] = nibble + (nibble > 9 ? 55 : 48);
		nibble = *ptr & 0x0F;
		from_hex_buffer[size - (i * 2)] = nibble + (nibble > 9 ? 55 : 48);
	}
	size++;
	from_hex_buffer[size] = 0;
	from_hex_return = String((char*)&from_hex_buffer, (size_t)size);
	return (String*)&from_hex_return;
}
template String* String::from_hex<>(byte);
template String* String::from_hex<>(ushort);
template String* String::from_hex<>(uint);
template String* String::from_hex<>(ulong);


char* String::get_str(){
	return m_str;
}


// Wrappers
extern "C" {
	char* string_get_str(struct String* string)	{ return string->get_str(); }

	// TODO Dynamic memory invokes templated "_from" functions
	char* string_str_from_int(slong value)						{ return String::from_int(value)->get_str(); }
	char* string_str_from_decimal(double value, byte places)	{ return String::from_decimal(value, places)->get_str(); }
	char* string_str_from_byte(byte value)						{ return String::from_hex(value)->get_str(); }
	char* string_str_from_ushort(ushort value)					{ return String::from_hex(value)->get_str(); }
	char* string_str_from_uint(uint value)						{ return String::from_hex(value)->get_str(); }
	char* string_str_from_ulong(ulong value)					{ return String::from_hex(value)->get_str(); }
}
