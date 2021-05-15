/*
 * File:		_vos_syscall_u.c
 * Description:	The userspace module for VOS syscalls
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Code is partially from syscall_arch.h from musl
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

#include "_vos_syscall.h"


// Inline assembly for fast syscalls
static inline _vos_syscallret_t _vos_syscall0(_vos_syscallnum_t n);
static inline _vos_syscallret_t _vos_syscall1(_vos_syscallnum_t n, _vos_syscallarg_t a1);
static inline _vos_syscallret_t _vos_syscall2(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2);
static inline _vos_syscallret_t _vos_syscall3(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2, _vos_syscallarg_t a3);
static inline _vos_syscallret_t _vos_syscall4(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2, _vos_syscallarg_t a3, _vos_syscallarg_t a4);
static inline _vos_syscallret_t _vos_syscall5(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2, _vos_syscallarg_t a3, _vos_syscallarg_t a4, _vos_syscallarg_t a5);
static inline _vos_syscallret_t _vos_syscall6(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2, _vos_syscallarg_t a3, _vos_syscallarg_t a4, _vos_syscallarg_t a5, _vos_syscallarg_t a6);

_vos_syscallret_t _vos_syscall(_vos_syscallnum_t num, size_t argc, ...) {
	va_list args;
	_vos_syscallarg_t a1, a2, a3, a4, a5, a6;
	va_start(args, argc);
	a1 = va_arg(args, _vos_syscallarg_t);
	a2 = va_arg(args, _vos_syscallarg_t);
	a3 = va_arg(args, _vos_syscallarg_t);
	a4 = va_arg(args, _vos_syscallarg_t);
	a5 = va_arg(args, _vos_syscallarg_t);
	a6 = va_arg(args, _vos_syscallarg_t);
	va_end(args);
	switch(argc) {
		case 0:
			return _vos_syscall0(num);
		case 1:
			return _vos_syscall1(num, a1);
		case 2:
			return _vos_syscall2(num, a1, a2);
		case 3:
			return _vos_syscall3(num, a1, a2, a3);
		case 4:
			return _vos_syscall4(num, a1, a2, a3, a4);
		case 5:
			return _vos_syscall5(num, a1, a2, a3, a4, a5);
		case 6:
			return _vos_syscall6(num, a1, a2, a3, a4, a5, a6);
	}
	return -1;
}

__attribute__((always_inline)) static inline _vos_syscallret_t _vos_syscall0(_vos_syscallnum_t n) {
	_vos_syscallret_t ret;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline _vos_syscallret_t _vos_syscall1(_vos_syscallnum_t n, _vos_syscallarg_t a1) {
	_vos_syscallret_t ret;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline _vos_syscallret_t _vos_syscall2(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2) {
	_vos_syscallret_t ret;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline _vos_syscallret_t _vos_syscall3(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2, _vos_syscallarg_t a3) {
	_vos_syscallret_t ret;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline _vos_syscallret_t _vos_syscall4(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2, _vos_syscallarg_t a3, _vos_syscallarg_t a4) {
	_vos_syscallret_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline _vos_syscallret_t _vos_syscall5(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2, _vos_syscallarg_t a3, _vos_syscallarg_t a4, _vos_syscallarg_t a5) {
	_vos_syscallret_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	register uint64_t r8 __asm__("r8") = a5;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline _vos_syscallret_t _vos_syscall6(_vos_syscallnum_t n, _vos_syscallarg_t a1, _vos_syscallarg_t a2, _vos_syscallarg_t a3, _vos_syscallarg_t a4, _vos_syscallarg_t a5, _vos_syscallarg_t a6) {
	_vos_syscallret_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	register uint64_t r8 __asm__("r8") = a5;
	register uint64_t r9 __asm__("r9") = a6;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return ret;
}