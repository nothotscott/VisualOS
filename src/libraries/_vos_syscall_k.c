/*
 * File:		_vos_syscall_k.c
 * Description:	The kernel module for VOS syscalls.
 * 				This is designed to directly link against the kernel
 * 				as a static library.
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdint.h>
#include <stdarg.h>

#include "_vos_syscall.h"

extern _vos_syscallret_t (*syshandler_get(_vos_syscallnum_t num))();


_vos_syscallret_t _vos_syscall(_vos_syscallnum_t num, size_t argc, ...) {
	va_list args;
	_vos_syscallarg_t a1, a2, a3, a4, a5, a6;
	_vos_syscallret_t (*syshandler)() = syshandler_get((_vos_syscallnum_t)num);
	va_start(args, argc);
	a1 = va_arg(args, _vos_syscallarg_t);
	a2 = va_arg(args, _vos_syscallarg_t);
	a3 = va_arg(args, _vos_syscallarg_t);
	a4 = va_arg(args, _vos_syscallarg_t);
	a5 = va_arg(args, _vos_syscallarg_t);
	a6 = va_arg(args, _vos_syscallarg_t);
	va_end(args);
	return syshandler(a1, a2, a3, a4, a5, a6);
}
