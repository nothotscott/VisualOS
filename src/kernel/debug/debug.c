/*
 * File:		debug.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdio.h>
#include <stdarg.h>
#include "x86_64/io.h"
#include "shell/text.h"
#include "shell/color.h"
#include "sys/sys.h"
#include "debug.h"


// Internal variable debug
static inline int vdebug(struct DebugOptions options, char* restrict format, va_list args);
static inline int vdebug_write(struct DebugOptions options, char* restrict format, va_list args);

static struct DebugOptions s_debug_options_default = {
	.type = DEBUG_TYPE_NORMAL,
	.to_screen = false
};

static char _stddbg_buffer[BUFSIZ];
static FILE _stddbg = { SYS_FD_STDDBG, _stddbg_buffer, BUFSIZ, 0, 0, { 0, 0 }, 0, { 0 }, _IOLBF | _PDCLIB_FWRITE,
#ifndef __STDC_NO_THREADS__
    _PDCLIB_MTX_RECURSIVE_INIT,
#endif
    NULL, NULL
};
FILE* stddbg = &_stddbg;


int debug(char* restrict format, ...) {
	int ret;
	va_list args;
	va_start(args, format);
	ret = vdebug(s_debug_options_default, format, args);
	va_end(args);
	return ret;
}
int debug_options(struct DebugOptions options, char* restrict format, ...) {
	int ret;
	va_list args;
	va_start(args, format);
	ret = vdebug(options, format, args);
	va_end(args);
	return ret;
}

static inline int vdebug(struct DebugOptions options, char* restrict format, va_list args) {
	int ret;
	va_list empty_args;
	if(options.type != DEBUG_TYPE_NORMAL) {
		struct ColorInterface* color = color_from_debug(options.type);
		vdebug_write(options, color->ansi, empty_args);
	}
	vdebug_write(options, format, args);
	if(options.type != DEBUG_TYPE_NORMAL) {
		vdebug_write(options, COLOR_RESET, empty_args);
	}
	return ret;
}
static inline int vdebug_write(struct DebugOptions options, char* restrict format, va_list args) {
	int ret = vfprintf(stddbg, format, args);
	if(options.to_screen == true || DEBUG_FORCE_TO_SCREEN) {
		// TODO figure out why color won't work on stderr
		ret |= vfprintf(stdout, format, args);
	}
	return ret;
}


void debug_output_size(char* str, size_t size) {
	for(size_t i = 0; i < size; i++) {
		outb(DEBUG_PORT, str[i]);
	}
}