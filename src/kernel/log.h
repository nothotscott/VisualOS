/*
 * File:		log.h
 * Description: Used for logging the kernel
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include <stdio.h>

#define LOG_FORCE_TO_SCREEN	0

#define LOG_PORT	0x03f8	// COM 1

typedef uint32_t	log_option_t;

enum LogType {
	LOG_TYPE_NORMAL,
	LOG_TYPE_INFO,
	LOG_TYPE_WARNING,
	LOG_TYPE_ERROR,
};
struct LogOptions {
	enum LogType	type : 2;
	log_option_t	to_screen : 1;
};


// Writes [format] with the log information with optional [options] to stderr
int log_default(char* restrict format, ...);
int log_to_screen(char* restrict format, ...);
int log_options(struct LogOptions options, char* restrict format, ...);

// Actually sends buffer [str] onto the serial debug port
void log_output_size(char* str, size_t size);