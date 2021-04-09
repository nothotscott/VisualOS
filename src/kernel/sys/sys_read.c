/*
 * File:		sys_read.c
 * Description:	Reads in the corresponding buffer
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdio.h>
#include "shell/text.h"
#include "sys.h"

uint64_t sys_read(int fd, char* buf, size_t count) {
	switch(fd) {
		case SYS_FD_STDIN:
			//text_output_size(buf, count);
			return count;
	}
	return 0;
}