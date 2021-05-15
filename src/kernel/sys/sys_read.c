/*
 * File:		sys_read.c
 * Description:	Reads in the corresponding buffer
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#define __NEED_struct_iovec

#include <stdio.h>
#include <bits/alltypes.h>
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

uint64_t sys_readv(int fd, struct iovec* vec, size_t vlen) {
	size_t total = 0;
	for(size_t i = 0; i < vlen; i++) {
		struct iovec* target = vec + i;
		total += sys_read(fd, target->iov_base, target->iov_len);
	}
	return total;
}