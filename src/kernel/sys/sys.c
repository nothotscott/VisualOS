/*
 * File:		sys.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "sys.h"


#include "shell/text.h"
#include "log.h"

static struct SysHandler s_handlers[] = {
	{ SYS_NUM_READ,					sys_read },
	{ SYS_NUM_WRITE,				sys_write },
	{ SYS_NUM_MMAP,					sys_mmap },
	{ SYS_NUM_MUNMAP,				sys_munmap },
	{ SYS_NUM_IOCTL,				sys_ioctl },
	{ SYS_NUM_READV,				sys_readv },
	{ SYS_NUM_WRITEV,				sys_writev },
	{ SYS_NUM_EXECVE,				sys_execve },
	{ SYS_NUM_EXIT,					sys_exit },
	
	{ SYS_NUM_GETRLIMIT,			sys_getrlimit },
	{ SYS_NUM_SYSINFO,				sys_sysinfo },
	{ SYS_NUM_SCHED_GETAFFINITY,	sys_sched_getaffinity },
	{ SYS_NUM_PRLIMIT64,			sys_prlimit64 },
};


// Because this function gets called during precarious situations, we need the special attribute
// so we can save the state of any registers this function uses
__attribute__((no_caller_saved_registers)) uint64_t (*syshandler_get(uint64_t num))() {
	for(size_t i = 0; i < sizeof(s_handlers)/sizeof(struct SysHandler); i++) {
		if(s_handlers[i].num == num) {
			return s_handlers[i].handler;
		}
	}
	log_options((struct LogOptions){LOG_TYPE_WARNING, false}, "Sys stub reached: %d\n", num);
	return syshandler_stub;
}

uint64_t syshandler_stub(){
	return 0;
}
