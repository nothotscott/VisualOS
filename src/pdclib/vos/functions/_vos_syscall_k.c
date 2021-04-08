/*
 * File:		_vos_syscall_k.c
 * Description:	The kernel module for VOS syscalls.
 * 				This is designed to directly link against the kernel
 * 				as a static library.
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "stdint.h"
#include "_vos_syscall.h"


syscallret_t syscall(enum SyscallNumber num) {
	return 1;	// test
}