/*
 * File:		cstd_string.h
 * Description:	Functions that implement C standard lib string functions
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#ifdef __cplusplus
namespace cstdlib {
#endif


// Comapres [str1] to [str2] and returns 0 if equal
#ifdef __cplusplus
extern "C"
#endif
int strcmp(const char* str1, const char* str2);

// Comapres [str1] to [str2] and returns 0 if equal for [n] length
#ifdef __cplusplus
extern "C"
#endif
int strncmp(const char* str1, const char* str2, size_t n);

// Returns size of [str]
#ifdef __cplusplus
extern "C"
#endif
size_t strlen(const char* str);


#ifdef __cplusplus
}
#endif