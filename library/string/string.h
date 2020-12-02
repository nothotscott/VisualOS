/*
 * File:		string.h
 * Desciption:	An all-purpose string class and wrapper
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

		// Returns the decimal value of value as a String
		template<typename T>
		static String* from_int(T value);

		// Returns the pointer to the string
		char* get_str();
};

}

#else
struct String;

struct String* string_from_int(long long);
char* string_get_str(struct String*);

#endif