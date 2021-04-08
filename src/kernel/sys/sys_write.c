/*
 * File:		sys_write.c
 * Description:	Writes a buffer to the corresponding descriptor
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */


#include "shell/text.h"
#include "sys.h"


uint64_t sys_write(uint64_t descriptor, char* buffer, size_t size) {
	// TODO file descriptor and all that linuxy crap
	for(size_t i = 0; i < size && buffer[i] != 0; i++){
		text_output_char(buffer[i], TEXT_COLOR_FOREGROUND);
	}
	return 0;
}