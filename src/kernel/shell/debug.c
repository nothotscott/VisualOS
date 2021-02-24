/*
 * File:		debug.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "x86_64/io.h"
#include "text.h"
#include "debug.h"

static const char* s_debug_color_reset = "\e[0m";
static const char* s_debug_color_red = "\e[0;31m";
static const char* s_debug_color_green = "\e[0;32m";
static const char* s_debug_color_yellow = "\e[0;33m";
static const char* s_debug_color_blue = "\e[0;34m";
static const char* s_debug_color_magenta = "\e[0;35m";
static const char* s_debug_color_cyan = "\e[0;36m";


void debug_output(char* str) {
	char* chr = str;
	while(*chr != 0){
		outb(DEBUG_PORT, *chr);
		chr++;
	}
}

void debug_output_info(char* str, bool to_screen) {
	debug_output((char*)s_debug_color_blue);
	debug_output(str);
	debug_output((char*)s_debug_color_reset);
	if(to_screen){
		text_output_color(str, TEXT_COLOR_BLUE);
	}
}
void debug_output_warning(char* str, bool to_screen) {
	debug_output((char*)s_debug_color_yellow);
	debug_output(str);
	debug_output((char*)s_debug_color_reset);
	if(to_screen){
		text_output_color(str, TEXT_COLOR_YELLOW);
	}
}
void debug_output_error(char* str, bool to_screen) {
	debug_output((char*)s_debug_color_red);
	debug_output(str);
	debug_output((char*)s_debug_color_reset);
	if(to_screen){
		text_output_color(str, TEXT_COLOR_RED);
	}
}