/*
 * File:		sys.h
 * Description:	Enumeration and implementation of all system handlers
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define SYS_WRITE	1
#define SYS_EXIT	60


struct SysHandler {
	uint64_t	num;
	void*		handler;
};


// *** Handler functions  *** //

// Gets the sys handler function for [num]
uint64_t (*syshandler_get(uint64_t num))();
// Stub sys handler
uint64_t syshandler_stub();


// *** Implementation functions  *** //
uint64_t sys_write(uint64_t descriptor, char* buffer, size_t size);
uint64_t sys_exit(uint64_t error_code);