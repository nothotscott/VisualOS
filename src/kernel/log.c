/*
 * File:		log.c
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
#include "log.h"


// Internal variable log
static inline int vlog(struct LogOptions options, char* restrict format, va_list args);
static inline int vlog_write(struct LogOptions options, char* restrict format, va_list args);

static struct LogOptions s_log_options_default = {
	.type = LOG_TYPE_NORMAL,
	.to_screen = false
};


int log(char* restrict format, ...) {
	int ret;
	va_list args;
	va_start(args, format);
	ret = vlog(s_log_options_default, format, args);
	va_end(args);
	return ret;
}
int log_to_screen(char* restrict format, ...) {
	int ret;
	va_list args;
	va_start(args, format);
	struct LogOptions defaults = s_log_options_default;
	defaults.to_screen = true;
	ret = vlog(defaults, format, args);
	va_end(args);
	return ret;
}
int log_options(struct LogOptions options, char* restrict format, ...) {
	int ret;
	va_list args;
	va_start(args, format);
	ret = vlog(options, format, args);
	va_end(args);
	return ret;
}

static inline int vlog(struct LogOptions options, char* restrict format, va_list args) {
	int ret;
	va_list empty_args;
	if(options.type != LOG_TYPE_NORMAL) {
		struct ColorInterface* color = color_from_log(options.type);
		vlog_write(options, color->ansi, empty_args);
	}
	vlog_write(options, format, args);
	if(options.type != LOG_TYPE_NORMAL) {
		vlog_write(options, COLOR_RESET, empty_args);
	}
	return ret;
}
static inline int vlog_write(struct LogOptions options, char* restrict format, va_list args) {
	int ret = vfprintf(stderr, format, args);
	if(options.to_screen == true || LOG_FORCE_TO_SCREEN) {
		ret |= vfprintf(stdout, format, args);
	}
	return ret;
}


void log_output_size(char* str, size_t size) {
	for(size_t i = 0; i < size; i++) {
		outb(LOG_PORT, str[i]);
	}
}