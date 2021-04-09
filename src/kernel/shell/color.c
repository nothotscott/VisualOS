/*
 * File:		color.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "text.h"
#include "debug/debug.h"
#include "color.h"

#define COLORS_SIZE	(sizeof(s_colors)/sizeof(struct ColorInterface))

struct ColorInterface g_color_default = {COLOR_WHITE, TEXT_COLOR_FOREGROUND, DEBUG_TYPE_NORMAL};

static struct ColorInterface s_colors[] = {
	{ COLOR_BLACK,		TEXT_COLOR_BLACK },
	{ COLOR_RED,		TEXT_COLOR_RED,		DEBUG_TYPE_ERROR },
	{ COLOR_GREEN,		TEXT_COLOR_GREEN },
	{ COLOR_YELLOW,		TEXT_COLOR_YELLOW,	DEBUG_TYPE_WARNING },
	{ COLOR_BLUE,		TEXT_COLOR_BLUE,	DEBUG_TYPE_INFO },
	{ COLOR_MAGENTA,	TEXT_COLOR_MAGENTA },
	{ COLOR_CYAN,		TEXT_COLOR_CYAN },
	{ COLOR_WHITE,	 	TEXT_COLOR_WHITE,	DEBUG_TYPE_NORMAL },
};


int color_ansi_compare(char* str1, char* str2) {
	for(size_t i = 0; i < COLOR_ANSI_MAX_SIZE; i++){
		if(str1[i] != str2[i]) {
			return COLOR_MATCH_FAIL;
		}
		if(str1[i] == '\0' && str2[i] == '\0') {
			return i;
		}
	}
	return COLOR_MATCH_FAIL;
}


struct ColorInterface* color_from_ansi(char* color, int* index) {
	for(size_t i = 0; i < COLORS_SIZE; i++) {
		int compare = color_ansi_compare(s_colors[i].ansi, color);
		if(compare != COLOR_MATCH_FAIL) {
			if(index != NULL){
				*index = i;
			}
			return &s_colors[i];
		}
	}
	if(index != NULL){
		*index = COLOR_MATCH_FAIL;
	}
	return &g_color_default;
}

struct ColorInterface* color_from_shell(text_color_t color) {
	for(size_t i = 0; i < COLORS_SIZE; i++) {
		if(s_colors[i].shell == color) {
			return &s_colors[i];
		}
	}
	return &g_color_default;
}

struct ColorInterface* color_from_debug(enum DebugType color) {
	for(size_t i = 0; i < COLORS_SIZE; i++) {
		if(s_colors[i].debug == color) {
			return &s_colors[i];
		}
	}
	return &g_color_default;
}