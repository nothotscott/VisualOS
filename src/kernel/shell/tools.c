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

void print_memory(void* address, size_t size, SHELL_COLOR content_color) {
	ulong start = (ulong)address - (ulong)address % SHELL_PRINT_MEMORY_COLUMNS;
	ulong end = start + size + (SHELL_PRINT_MEMORY_COLUMNS - size % SHELL_PRINT_MEMORY_COLUMNS);
	// Print header
	print("Offset            ", SHELL_COLOR_MEMORY_HEADER);
	for(byte i = 0; i < SHELL_PRINT_MEMORY_COLUMNS; i++){
		print(string_str_from_byte(i), SHELL_COLOR_MEMORY_HEADER);
		print_char(' ', SHELL_COLOR_MEMORY_HEADER);
	}
	print(" Decoded text", SHELL_COLOR_MEMORY_HEADER);
	print_newline();
	// Print content
	for(ulong row = (ulong)start; row < end; row += SHELL_PRINT_MEMORY_COLUMNS){
		print(string_str_from_ulong(row), SHELL_COLOR_MEMORY_LOCATION);
		print("  ", SHELL_COLOR_MEMORY_LOCATION);
		for(byte* ptr = (byte*)row; ptr < (byte*)(row + SHELL_PRINT_MEMORY_COLUMNS); ptr++){
			print(string_str_from_byte(*ptr), content_color);
			print_char(' ', content_color);
		}
		print_char(' ', content_color);
		for(byte* ptr = (byte*)row; ptr < (byte*)(row + SHELL_PRINT_MEMORY_COLUMNS); ptr++){
			char chr = *ptr >= '!' && *ptr <= '~' ? *ptr : SHELL_PRINT_MEMORY_FILLER;
			print_char(chr, content_color);
		}
		print_newline();
	}
}