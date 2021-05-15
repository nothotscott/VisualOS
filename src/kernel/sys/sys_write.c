/*
 * File:		sys_write.c
 * Description:	Writes a buffer to the corresponding descriptor
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#define __NEED_struct_iovec

#include <stdio.h>
#include <bits/alltypes.h>
#include "shell/text.h"
#include "shell/color.h"
#include "log.h"
#include "sys.h"

uint64_t sys_write(int fd, char* buf, size_t count) {
	switch(fd) {
		case SYS_FD_STDOUT: {
			text_output_size(buf, count);
			return count;
		}
		case SYS_FD_STDERR: {
			log_output_size(buf, count);
			return count;
		}
	}
	return 0;
}

uint64_t sys_writev(int fd, struct iovec* vec, size_t vlen) {
	size_t total = 0;
	for(size_t i = 0; i < vlen; i++) {
		struct iovec* target = vec + i;
		total += sys_write(fd, target->iov_base, target->iov_len);
	}
	return total;
}