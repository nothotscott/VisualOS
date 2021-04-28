/*
 * File:		_vos_sys.c
 * Note:		This file should contain any external calls
 * *****************************************************************************
 * This file was made as part of VisualOS's integration with this library
 * Copyright 2021 Scott Maday
 * Code is partially from syscall_arch.h from musl
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "stdint.h"
#include "stdio.h"
#include "_vos_syscall.h"
#include "_vos_sys.h"


ssize_t read(int fd, char* buf, size_t count) {
	return (ssize_t)_VOS_SYSCALL(SYSCALL_NUM_READ, fd, buf, count);
}

ssize_t write(int fd, char* buf, size_t count) {
	return (ssize_t)_VOS_SYSCALL(SYSCALL_NUM_WRITE, fd, buf, count);
}