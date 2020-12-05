/*
 * File:		tools.h
 * Desciption:	Provides shell tools for debugging
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#include "shell.h"

#define SHELL_COLOR_MEMORY_HEADER	SHELL_COLOR_CYAN
#define SHELL_COLOR_MEMORY_LOCATION	SHELL_COLOR_GREEN


// Prints the contents at address (rounded down to nearest 16 bytes) for size
// bytes (rounded up to the nearest 16 bytes)
void print_memory(void* address, size_t size, SHELL_COLOR content_color);