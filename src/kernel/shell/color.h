/*
 * File:		color.h
 * Description:	Responsible for the standardized ANSI color codes
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "text.h"
#include "debug/debug.h"

#define COLOR_MATCH_FAIL	-1

#define COLOR_ANSI_MAX_SIZE			8
#define COLOR_ANSI_MAGIC_CHAR_END	'm'

#define COLOR_RESET		"\e[0m"
#define COLOR_BLACK		"\e[0;30m"
#define COLOR_RED		"\e[0;31m"
#define COLOR_GREEN		"\e[0;32m"
#define COLOR_YELLOW	"\e[0;33m"
#define COLOR_BLUE		"\e[0;34m"
#define COLOR_MAGENTA	"\e[0;35m"
#define COLOR_CYAN		"\e[0;36m"
#define COLOR_WHITE		"\e[0;37m"


// Common color interface
struct ColorInterface {
	char			ansi[COLOR_ANSI_MAX_SIZE];
	text_color_t	shell;
	enum DebugType	debug;
};


extern struct ColorInterface g_color_default;

// If ansi colors [str1] and [str2] match, this will return the index at the end
// of the string after the ansi color, else COLOR_MATCH_FAIL
int color_ansi_compare(char* str1, char* str2);

// Gets the color interface for the respective [color]
// Will write the index found to [index] if it's not null
struct ColorInterface* color_from_ansi(char* color, int* index);
struct ColorInterface* color_from_shell(text_color_t color);
struct ColorInterface* color_from_debug(enum DebugType color);