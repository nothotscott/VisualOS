/*
 * File:		_vos_sys.h
 * Description:	The unified implementation of syscall functions. This is
 * 				agnostic to which module it's being called from.
 * *****************************************************************************
 * This file was made as part of VisualOS's integration with this library
 * Copyright 2021 Scott Maday
 * Code is partially from syscall_arch.h from musl
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "pdclib/_PDCLIB_glue.h"

#include "stdint.h"
#include "stdio.h"
#include "time.h"

#include "vos_time.h"
#include "_vos_syscall.h"

typedef	signed long	ssize_t;

ssize_t read(int fd, char* buffer, size_t count);
ssize_t write(int fd, char* buffer, size_t count);

// TODO sys implementations
int open(const char* filename, int flags, int mode);
int close(int fd);
int gettimeofday(struct timeval* tv, struct timezone* tz);
clock_t times(struct tms* tbuf);