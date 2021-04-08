/*
 * File:		_vos_syscall_u.c
 * Description:	The userspace module for VOS syscalls
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Code is partially from syscall_arch.h from musl
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "stdint.h"
#include "_vos_syscall.h"

__attribute__((always_inline)) static inline uint64_t __syscall0(uint64_t n)
{
	uint64_t ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n) : "rcx", "r11", "memory");
	return ret;
}

__attribute__((always_inline)) static inline uint64_t __syscall1(uint64_t n, uint64_t a1)
{
	uint64_t ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1) : "rcx", "r11", "memory");
	return ret;
}

__attribute__((always_inline)) static inline uint64_t __syscall2(uint64_t n, uint64_t a1, uint64_t a2)
{
	uint64_t ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2)
						  : "rcx", "r11", "memory");
	return ret;
}

__attribute__((always_inline)) static inline uint64_t __syscall3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3)
{
	uint64_t ret;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3) : "rcx", "r11", "memory");
	return ret;
}

__attribute__((always_inline)) static inline uint64_t __syscall4(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4)
{
	uint64_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10): "rcx", "r11", "memory");
	return ret;
}

__attribute__((always_inline)) static inline uint64_t __syscall5(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5)
{
	uint64_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	register uint64_t r8 __asm__("r8") = a5;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8) : "rcx", "r11", "memory");
	return ret;
}

__attribute__((always_inline)) static inline uint64_t __syscall6(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6)
{
	uint64_t ret;
	register uint64_t r10 __asm__("r10") = a4;
	register uint64_t r8 __asm__("r8") = a5;
	register uint64_t r9 __asm__("r9") = a6;
	__asm__ __volatile__ ("syscall" : "=a"(ret) : "a"(n), "D"(a1), "S"(a2),
						  "d"(a3), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11", "memory");
	return ret;
}


syscallret_t syscall(enum SyscallNumber num) {
	return 0;	// test
}