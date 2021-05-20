/*
 * File:		sys_system.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#define __NEED_pid_t
#define _LARGEFILE64_SOURCE

#include <sys/sysinfo.h>
#include <sys/resource.h>
#include <bits/alltypes.h>

#include "memory/memory.h"
#include "scheduler/scheduler.h"
#include "sys.h"


uint64_t sys_getrlimit(int resource, struct rlimit* rlim) {
	*rlim = (struct rlimit){
		.rlim_cur = RLIM_INFINITY,
		.rlim_max = RLIM_INFINITY,
	};
	return 0;
}

uint64_t sys_sysinfo(struct sysinfo* info) {
	*info = (struct sysinfo){
		.uptime = 0,	// TODO
		.loads[0] = 0,	// TODO
		.loads[1] = 0,	// TODO
		.loads[2] = 0,	// TODO
		.totalram = memory_get_total_size(),
		.freeram = memory_get_free(),
		.sharedram = 0,	// TODO
		.totalswap = 0,	// TODO
		.procs = 0,	// TODO
		.totalhigh = 0,	// TODO
		.freehigh = 0,	// TODO
		.mem_unit = 1
	};
	return 0;
}

uint64_t sys_sched_getaffinity(pid_t pid, size_t len, long* user_mask_ptr) {
	return 0;
}

uint64_t sys_prlimit64(pid_t pid, int resource, const struct rlimit64* new_rlim, struct rlimit64* old_rlim) {
	return 0;
}