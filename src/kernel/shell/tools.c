/*
 * File:		tools.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include <memory.h>
#include "cstd_string.h"
#include "shell.h"
#include "tools.h"

// Static buffer for printf
static char s_buffer[0x200];


SHELL_COLOR shell_color_lookup(const char* name) {
	struct _ColorItem { const char* name; SHELL_COLOR color; };
	struct _ColorItem _table[] = {
		{ "DEFAULT",	SHELL_COLOR_FOREGROUND },
		{ "WHITE",		SHELL_COLOR_WHITE },
		{ "BLACK",		SHELL_COLOR_BLACK },
		{ "GRAY",		SHELL_COLOR_GRAY },
		{ "RED",		SHELL_COLOR_RED },
		{ "GREEN",		SHELL_COLOR_GREEN },
		{ "BLUE",		SHELL_COLOR_BLUE },
		{ "CYAN",		SHELL_COLOR_CYAN },
		{ "YELLOW",		SHELL_COLOR_YELLOW },
		{ "MAGENTA",	SHELL_COLOR_MAGENTA }
	};
	for(struct _ColorItem* item = _table; item < _table + sizeof(_table)/sizeof(struct _ColorItem); item++) {
		if(strcmp(item->name, name) == 0) {
			return item->color;
		}
	}
	return NULL;
}

size_t printf_color(const char* restrict format, SHELL_COLOR color, ...) {
	va_list args;
	va_start(args, color);
	size_t size = string_str_format(s_buffer, format, NULL, args);
	print_color(s_buffer, color);
	va_end(args);
	return size;
}

size_t printf(const char* restrict format, ...) {
	va_list args;
	va_start(args, format);
	size_t size = 0;	// total buffered size
	size_t s = 0;		// start index
	size_t i = 0;		// format index
	size_t j = 0;		// lookahead offset
	SHELL_COLOR color = SHELL_COLOR_FOREGROUND;
	while(format[i] != '\0') {
		if(format[i] != SHELL_PRINTF_COLOR_TOKEN_BEGIN) {
			i++;
			continue;
		}
		i++;	// move past beginning token
		while(format[i + j] != '\0' && format[i + j] != SHELL_PRINTF_COLOR_TOKEN_END) {
			j++;
		}
		SHELL_COLOR l_color = shell_color_lookup(format + i);
		if(l_color == NULL){	// invalid color
			i++;
			j = 0;
			continue;
		}
		size += string_str_format(s_buffer, format + s, i - s - 1, args);	// print the last set before the color
		print_color(s_buffer, color);
		color = l_color;
		i += j + 1;
		s = i;
		j = 0;
	}
	size += string_str_format(s_buffer, format + s, NULL, args);	// print the final set
	print_color(s_buffer, color);
	va_end(args);
	return size;
}

void print_memory_color(void* address, size_t size, SHELL_COLOR content_color, SHELL_COLOR fade_color) {
	size_t mod = size % SHELL_PRINT_MEMORY_COLUMNS;
	ulong_t start = (ulong_t)address - (ulong_t)address % SHELL_PRINT_MEMORY_COLUMNS;
	ulong_t end = (ulong_t)address + size - mod;
	if(mod != 0){
		end += SHELL_PRINT_MEMORY_COLUMNS;
	}
	// Print header
	print_color("Offset            ", SHELL_COLOR_MEMORY_HEADER);
	for(byte_t i = 0; i < SHELL_PRINT_MEMORY_COLUMNS; i++){
		print_color(string_str_from_byte(i), SHELL_COLOR_MEMORY_HEADER);
		print_char(' ', SHELL_COLOR_MEMORY_HEADER);
	}
	print_color(" Decoded text", SHELL_COLOR_MEMORY_HEADER);
	print_newline();
	// Print content
	for(ulong_t row = (ulong_t)start; row < end; row += SHELL_PRINT_MEMORY_COLUMNS){
		print_color(string_str_from_ulong(row), SHELL_COLOR_MEMORY_LOCATION);
		print_color("  ", SHELL_COLOR_MEMORY_LOCATION);
		for(byte_t* ptr = (byte_t*)row; ptr < (byte_t*)(row + SHELL_PRINT_MEMORY_COLUMNS); ptr++){
			SHELL_COLOR color = ((ulong_t)ptr < (ulong_t)address || (ulong_t)ptr >= (ulong_t)address + size) ? fade_color : content_color;
			print_color(string_str_from_byte(*ptr), color);
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
void print_memory(void* address, size_t size) {
	print_memory_color(address, size, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);
}

void print_bool(bool value) {
	if(value == 0){
		print_color("false", SHELL_COLOR_RED);
	} else {
		print_color("true", SHELL_COLOR_GREEN);
	}
}