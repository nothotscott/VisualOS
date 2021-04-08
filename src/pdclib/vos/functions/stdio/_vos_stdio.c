/*
 * This file was made as part of VisualOS's integration with this library
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with VisualOS for licensing terms
 */

#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "_vos_syscall.h"

syscallret_t write(int fd, const void * buf, size_t count) {
	return syscall(SYSCALL_NUM_WRITE);
}