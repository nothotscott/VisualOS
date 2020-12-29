/*
 * File:		tools.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>

#include "shell.h"
#include "tools.h"

#define SHELL_PRINT_MEMORY_COLUMNS	16
#define SHELL_PRINT_MEMORY_FILLER	'.'

void print_memory(void* address, size_t size, SHELL_COLOR content_color, SHELL_COLOR fade_color) {
	size_t mod = size % SHELL_PRINT_MEMORY_COLUMNS;
	ulong_t start = (ulong_t)address - (ulong_t)address % SHELL_PRINT_MEMORY_COLUMNS;
	ulong_t end = (ulong_t)address + size - mod;
	if(mod != 0){
		end += SHELL_PRINT_MEMORY_COLUMNS;
	}
	// Print header
	print("Offset            ", SHELL_COLOR_MEMORY_HEADER);
	for(byte_t i = 0; i < SHELL_PRINT_MEMORY_COLUMNS; i++){
		print(string_str_from_byte(i), SHELL_COLOR_MEMORY_HEADER);
		print_char(' ', SHELL_COLOR_MEMORY_HEADER);
	}
	print(" Decoded text", SHELL_COLOR_MEMORY_HEADER);
	print_newline();
	// Print content
	for(ulong_t row = (ulong_t)start; row < end; row += SHELL_PRINT_MEMORY_COLUMNS){
		print(string_str_from_ulong(row), SHELL_COLOR_MEMORY_LOCATION);
		print("  ", SHELL_COLOR_MEMORY_LOCATION);
		for(byte_t* ptr = (byte_t*)row; ptr < (byte_t*)(row + SHELL_PRINT_MEMORY_COLUMNS); ptr++){
			SHELL_COLOR color = ((ulong_t)ptr < (ulong_t)address || (ulong_t)ptr >= (ulong_t)address + size) ? fade_color : content_color;
			print(string_str_from_byte(*ptr), color);
			print_char(' ', color);
		}
		print_char(' ', content_color);
		for(byte_t* ptr = (byte_t*)row; ptr < (byte_t*)(row + SHELL_PRINT_MEMORY_COLUMNS); ptr++){
			SHELL_COLOR color = ((ulong_t)ptr < (ulong_t)address || (ulong_t)ptr >= (ulong_t)address + size) ? fade_color : content_color;
			char chr = *ptr >= '!' && *ptr <= '~' ? *ptr : SHELL_PRINT_MEMORY_FILLER;
			print_char(chr, color);
		}
		print_newline();
	}
}

void print_bool(bool value) {
	if(value == 0){
		print("false", SHELL_COLOR_RED);
	} else {
		print("true", SHELL_COLOR_GREEN);
	}
}