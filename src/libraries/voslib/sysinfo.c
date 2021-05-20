/*
 * File:		sysinfo.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <sys/sysinfo.h>
#include "_vos_syscall.h"
#include "syscall.h"

int __lsysinfo(struct sysinfo* info)
{
	return _VOS_SYSCALL(SYS_sysinfo, info);
}

int sysinfo(struct sysinfo* info) __attribute__((weak,alias("__lsysinfo")));