/*
 * File:		_vos_syscall.h
 * Description:	VOS-specific syscalls designed for compatibility between
 * 				the kernel version and the userspace version of this library.
 * 				Because we have to make the extra call to the standardized
 * 				syscall() function, we unfortunately lose a slight bit of
 * 				perfomance, but this module is designed to greatly improve
 * 				the overall performance by allowing the kernel syscall module
 * 				to directly call the sys functions while the userspace version
 * 				performs the fast system call.
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "stdint.h"
#include "stddef.h"

typedef	uint64_t	syscallret_t;
typedef	uint64_t	syscallarg_t;

enum SyscallNumber {
	// Linux syscalls
	SYSCALL_NUM_READ			= 0,
	SYSCALL_NUM_WRITE			= 1,
	SYSCALL_NUM_EXIT			= 60,
	// VOS syscalls
	SYSCALL_NUM_VOS_SOMETHING	= 512,
};

// Performs a syscall indexed by [num]
// The way this is performed depends on the module
syscallret_t syscall(enum SyscallNumber num);

syscallret_t write(int fd, const void * buf, size_t count);