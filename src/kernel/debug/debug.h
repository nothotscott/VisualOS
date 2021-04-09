/*
 * File:		debug.h
 * Description: Used for debugging the kernel
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include <stdio.h>

#define DEBUG_FORCE_TO_SCREEN	0

#define DEBUG_PORT	0x03f8	// COM 1

typedef uint32_t	debug_option_t;

enum DebugType {
	DEBUG_TYPE_NORMAL,
	DEBUG_TYPE_INFO,
	DEBUG_TYPE_WARNING,
	DEBUG_TYPE_ERROR,
};
struct DebugOptions {
	enum DebugType	type : 2;
	debug_option_t	to_screen : 1;
};

// Standard debug output stream
extern FILE* stddbg;


// Writes [format] with the debug information with optional [options] to stddbg
int debug(char* restrict format, ...);
int debug_options(struct DebugOptions options, char* restrict format, ...);

// Actually sends buffer [str] onto the serial debug port
void debug_output(char* str, size_t count);