/*
 * File:		sys_write.c
 * Description:	Writes a buffer to the corresponding descriptor
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdio.h>
#include "shell/text.h"
#include "shell/color.h"
#include "log.h"
#include "sys.h"

uint64_t sys_write(int fd, char* buf, size_t count) {
	switch(fd) {
		case SYS_FD_STDOUT:
			text_output_size(buf, count);
			return count;
		case SYS_FD_STDERR:
			log_output_size(buf, count);
			return count;
	}
	return 0;
}