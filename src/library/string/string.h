/*
 * File:		string.h
 * Description:	An all-purpose string class and wrapper
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#ifdef __cplusplus
namespace libvos {

class String {
	private:
		char*	m_str;
		size_t	m_size;
	public:
		// Empty string
		String();
		// String with an unknown length
		String(char* str);
		// String with a known length
		String(char* str, size_t size);

		// Returns the int, decimal, or hexidecimal value of value as a String respectivley
		template<typename T>
		static String* from_int(T value);
		template<typename T>
		static String* from_decimal(T value, byte_t places);
		template<typename T>
		static String* from_hex(T value);

		// Returns the pointer to the string
		char* get_str();
};

}

#else
struct String;

// Class wrappers
char* string_get_str(struct String*);

// Convenience wrappers
char* string_str_from_int(slong_t);
char* string_str_from_decimal(double, byte_t);
char* string_str_from_byte(byte_t);
char* string_str_from_ushort(ushort_t);
char* string_str_from_uint(uint_t);
char* string_str_from_ulong(ulong_t);
#endif