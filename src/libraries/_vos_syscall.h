/*
 * File:		_vos_syscall.h
 * Description:	VOS-specific syscalls designed for compatibility between
 * 				the kernel version and the userspace versions of libraries.
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

#include <stdint.h>
#include <stddef.h>

#define SYSCALL_VOS_OFFSET	512

#define _VOS_SYSCALL_NARGS_SELECTOR()					6,5,4,3,2,1,0
#define _VOS_SYSCALL_NARGS_N(_1,_2,_3,_4,_5,_6,n,...)	n
#define	_VOS_SYSCALL_NARGS_(...)						_VOS_SYSCALL_NARGS_N(__VA_ARGS__)
#define	_VOS_SYSCALL_NARGS(...)							_VOS_SYSCALL_NARGS_(__VA_ARGS__,_VOS_SYSCALL_NARGS_SELECTOR())

#define	_VOS_SYSCALL(num,...)	(_vos_syscall(num,_VOS_SYSCALL_NARGS(__VA_ARGS__),##__VA_ARGS__))

typedef	uint64_t	_vos_syscallret_t;
typedef	uint64_t	_vos_syscallarg_t;
typedef	uint64_t	_vos_syscallnum_t;



// Performs a syscall indexed by [num] with [...] as args and [argc] count
// This is so there's an actual symbol to link to for things that may need it
// The implementation should just call the inline function _vos_syscall
_vos_syscallret_t _vos_syscall(_vos_syscallnum_t num, size_t argc, ...);
