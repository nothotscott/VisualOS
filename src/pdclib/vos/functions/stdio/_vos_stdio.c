/*
 * This file was made as part of VisualOS's integration with this library
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with VisualOS for licensing terms
 */

#include <stdio.h>
#include "_vos_stdio.h"

extern void text_output_color_size(char* str, text_color_t color, size_t size);


// I have no idea what I'm doing
ssize_t write( int fd, const void * buf, size_t count ) {
	if(fd == 1) {
		text_output_color_size((char*)buf, TEXT_COLOR_WHITE, count);
		return count;
	}
	return 0;
}