/*
 * File:		sys_write.c
 * Description:	Writes a buffer to the corresponding descriptor
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */


#include "shell/text.h"
#include "sys.h"


uint64_t sys_write(int fd, char* buffer, size_t count) {
	// TODO file descriptor and all that linuxy crap
	text_output_color_size(buffer, TEXT_COLOR_FOREGROUND, count);
	return count;
}