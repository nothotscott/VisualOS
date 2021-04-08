/*
 * File:		sys.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "sys.h"


#include "shell/text.h"
#include "shell/debug.h"

static struct SysHandler s_handlers[] = {
	{ 1,	sys_write },
	{ 60,	sys_exit }
};

uint64_t (*syshandler_get(uint64_t num))() {
	for(size_t i = 0; i < sizeof(s_handlers)/sizeof(struct SysHandler); i++) {
		if(s_handlers[i].num == num) {
			return s_handlers[i].handler;
		}
	}
	return syshandler_stub;
}

uint64_t syshandler_stub(){
	debug_output_warning("sys stub reached\n", false);
	return 0;
}
