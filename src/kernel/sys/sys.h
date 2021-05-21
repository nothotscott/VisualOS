/*
 * File:		sys.h
 * Description:	Enumeration and implementation of all system handlers
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define __NEED_struct_iovec
#define __NEED_pid_t
#define _LARGEFILE64_SOURCE

#include <sys/sysinfo.h>
#include <sys/resource.h>
#include <bits/alltypes.h>

#define	SYS_FD_STDIN	0
#define	SYS_FD_STDOUT	1
#define	SYS_FD_STDERR	2

enum SysNum {
	SYS_NUM_READ				= 0,
	SYS_NUM_WRITE				= 1,
	SYS_NUM_MMAP				= 9,
	SYS_NUM_MUNMAP				= 11,
	SYS_NUM_IOCTL				= 16,
	SYS_NUM_READV				= 19,
	SYS_NUM_WRITEV				= 20,
	SYS_NUM_EXECVE				= 59,
	SYS_NUM_EXIT				= 60,

	SYS_NUM_GETRLIMIT			= 97,
	SYS_NUM_SYSINFO				= 99,
	SYS_NUM_SCHED_GETAFFINITY	= 204,
	SYS_NUM_PRLIMIT64			= 302,
};


struct SysHandler {
	enum SysNum	num;
	void*		handler;
};


// *** Handler functions  *** //

// Gets the sys handler function for [num]
__attribute__((no_caller_saved_registers)) uint64_t (*syshandler_get(uint64_t num))();
// Stub sys handler
uint64_t syshandler_stub();


// *** Implementation functions  *** //
uint64_t sys_read(int fd, char* buf, size_t count);
uint64_t sys_write(int fd, char* buf, size_t count);
uint64_t sys_mmap(void* addr, size_t len, int prot, int flags, int fd, size_t offset);
uint64_t sys_munmap(void* addr, size_t len);
uint64_t sys_ioctl(int fd, int cmd, long arg);
uint64_t sys_readv(int fd, struct iovec* vec, size_t vlen);
uint64_t sys_writev(int fd, struct iovec* vec, size_t vlen);
uint64_t sys_execve(const char* filename, const char* const* argv, const char* const* envp);
uint64_t sys_exit(uint64_t error_code);

uint64_t sys_getrlimit(int resource, struct rlimit* rlim);
uint64_t sys_sysinfo(struct sysinfo* info);
uint64_t sys_sched_getaffinity(pid_t pid, size_t len, long* user_mask_ptr);
uint64_t sys_prlimit64(pid_t pid, int resource, const struct rlimit64* new_rlim, struct rlimit64* old_rlim);