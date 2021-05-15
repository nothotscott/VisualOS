/*
 * File:		sys.h
 * Description:	Enumeration and implementation of all system handlers
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define __NEED_struct_iovec

#include <bits/alltypes.h>

#define	SYS_FD_STDIN	0
#define	SYS_FD_STDOUT	1
#define	SYS_FD_STDERR	2

enum SysNum {
	SYS_NUM_READ	= 0,
	SYS_NUM_WRITE	= 1,
	SYS_NUM_IOCTL	= 16,
	SYS_NUM_READV	= 19,
	SYS_NUM_WRITEV	= 20,
	SYS_NUM_EXIT	= 60
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
uint64_t sys_ioctl(int fd, int cmd, long arg);
uint64_t sys_readv(int fd, struct iovec* vec, size_t vlen);
uint64_t sys_writev(int fd, struct iovec* vec, size_t vlen);

uint64_t sys_exit(uint64_t error_code);