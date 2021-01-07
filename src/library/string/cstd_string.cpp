/*
 * File:		cstd_string.cpp
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "cstd_string.h"

namespace cstdlib {

extern "C" int strcmp(const char* str1, const char* str2) {
	size_t i = 0;
	while(str1[i] != '\0') {
		if(str1[i] != str2[i]) {
			return str1[i] < str2[i] ? -1 : 1;
		}
		i++;
	}
	return 0;
}

extern "C" int strncmp(const char* str1, const char* str2, size_t n) {
	size_t i = 0;
	for(size_t i = 0; i < n && str1[i] != '\0'; i++) {
		if(str1[i] != str2[i]) {
			return str1[i] < str2[i] ? -1 : 1;
		}
	}
	return 0;
}

extern "C" size_t strlen(const char* str) {
	size_t i = 0;
	while(str[i] != '\0'){
		i++;
	}
	return i;
}



}