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
#include "stdarg.h"
#include "_vos_syscall.h"

extern syscallret_t (*syshandler_get(syscallnum_t num))();


syscallret_t _vos_syscall(enum SyscallNumber num, size_t argc, ...) {
	va_list args;
	syscallarg_t a1, a2, a3, a4, a5, a6;
	syscallret_t (*syshandler)() = syshandler_get((syscallnum_t)num);
	va_start(args, argc);
	a1 = va_arg(args, syscallarg_t);
	a2 = va_arg(args, syscallarg_t);
	a3 = va_arg(args, syscallarg_t);
	a4 = va_arg(args, syscallarg_t);
	a5 = va_arg(args, syscallarg_t);
	a6 = va_arg(args, syscallarg_t);
	va_end(args);
	return syshandler(a1, a2, a3, a4, a5, a6);
}
