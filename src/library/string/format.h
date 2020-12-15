/*
 * File:		format.h
 * Description: Continuation of string.h for formatting
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "string.h"


#ifdef __cplusplus
//libvos::String::format(const char* restrict format, ...);
#else
char* string_str_format(const char* restrict format, ...);
#endif
