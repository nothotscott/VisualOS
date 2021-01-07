/*
 * File:		tools.h
 * Description:	Provides shell tools for debugging
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "shell.h"

#define SHELL_COLOR_MEMORY_HEADER	SHELL_COLOR_CYAN
#define SHELL_COLOR_MEMORY_LOCATION	SHELL_COLOR_GREEN
#define SHELL_COLOR_MEMORY_CONTENT	SHELL_COLOR_FOREGROUND
#define SHELL_COLOR_MEMORY_FADE		SHELL_COLOR_GRAY

#define SHELL_PRINT_MEMORY_COLUMNS	16
#define SHELL_PRINT_MEMORY_FILLER	'.'

#define SHELL_PRINTF_COLOR_TOKEN_BEGIN	'%'
#define SHELL_PRINTF_COLOR_TOKEN_END	' '


// Returns the color for the [name]
SHELL_COLOR shell_color_lookup(const char* name);

// Prints the [format]ted string to the screen colored [color] and
// returns the size of the string written.
// This version uses a fixed size buffer
size_t printf_color(const char* restrict format, SHELL_COLOR color, ...);
size_t printf(const char* restrict format, ...);

// Prints the contents at [address] (rounded down to nearest 16 bytes) for
// [size] bytes (rounded up to the nearest 16 bytes)
void print_memory_color(void* address, size_t size, SHELL_COLOR content_color, SHELL_COLOR fade_color);
void print_memory(void* address, size_t size);

// Prints "false" in SHELL_COLOR_RED if [value] is false,
// else "true" in SHELL_COLOR_GREEN
void print_bool(bool value);
