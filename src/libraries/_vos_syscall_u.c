/*
 * File:		_vos_syscall_u.c
 * Description:	The userspace module for VOS syscalls
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Code is partially from syscall_arch.h from musl
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "pdclib/_PDCLIB_config.h"

#include "stdint.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"
#include "_vos_sys.h"
#include "_vos_syscall.h"


// Inline assembly for fast syscalls
static inline syscallret_t _vos_syscall0(syscallnum_t n);
static inline syscallret_t _vos_syscall1(syscallnum_t n, syscallarg_t a1);
static inline syscallret_t _vos_syscall2(syscallnum_t n, syscallarg_t a1, syscallarg_t a2);
static inline syscallret_t _vos_syscall3(syscallnum_t n, syscallarg_t a1, syscallarg_t a2, syscallarg_t a3);
static inline syscallret_t _vos_syscall4(syscallnum_t n, syscallarg_t a1, syscallarg_t a2, syscallarg_t a3, syscallarg_t a4);
static inline syscallret_t _vos_syscall5(syscallnum_t n, syscallarg_t a1, syscallarg_t a2, syscallarg_t a3, syscallarg_t a4, syscallarg_t a5);
static inline syscallret_t _vos_syscall6(syscallnum_t n, syscallarg_t a1, syscallarg_t a2, syscallarg_t a3, syscallarg_t a4, syscallarg_t a5, syscallarg_t a6);

void vos_noreturn_stub() _PDCLIB_NORETURN;

syscallret_t _vos_syscall(enum SyscallNumber num, size_t argc, ...) {
	va_list args;
	syscallarg_t a1, a2, a3, a4, a5, a6;
	va_start(args, argc);
	a1 = va_arg(args, syscallarg_t);
	a2 = va_arg(args, syscallarg_t);
	a3 = va_arg(args, syscallarg_t);
	a4 = va_arg(args, syscallarg_t);
	a5 = va_arg(args, syscallarg_t);
	a6 = va_arg(args, syscallarg_t);
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

__attribute__((always_inline)) static inline syscallret_t _vos_syscall0(syscallnum_t n) {
	syscallret_t ret;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline syscallret_t _vos_syscall1(syscallnum_t n, syscallarg_t a1) {
	syscallret_t ret;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline syscallret_t _vos_syscall2(syscallnum_t n, syscallarg_t a1, syscallarg_t a2) {
	syscallret_t ret;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline syscallret_t _vos_syscall3(syscallnum_t n, syscallarg_t a1, syscallarg_t a2, syscallarg_t a3) {
	syscallret_t ret;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline syscallret_t _vos_syscall4(syscallnum_t n, syscallarg_t a1, syscallarg_t a2, syscallarg_t a3, syscallarg_t a4) {
	syscallret_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline syscallret_t _vos_syscall5(syscallnum_t n, syscallarg_t a1, syscallarg_t a2, syscallarg_t a3, syscallarg_t a4, syscallarg_t a5) {
	syscallret_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	register uint64_t r8 __asm__("r8") = a5;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return ret;
}
__attribute__((always_inline)) static inline syscallret_t _vos_syscall6(syscallnum_t n, syscallarg_t a1, syscallarg_t a2, syscallarg_t a3, syscallarg_t a4, syscallarg_t a5, syscallarg_t a6) {
	syscallret_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	register uint64_t r8 __asm__("r8") = a5;
	register uint64_t r9 __asm__("r9") = a6;
	__asm__ volatile ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return ret;
}


// *** SPECIAL CASES *** //

// We need to directly establish this syscall here, since it does not return
void vos_exit(int status) _PDCLIB_NORETURN;
void vos_exit(int status){
	_vos_syscall1(SYSCALL_NUM_EXIT, status);
	vos_noreturn_stub();
}