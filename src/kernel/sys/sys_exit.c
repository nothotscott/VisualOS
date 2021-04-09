/*
 * File:		sys_exit.c
 * Description:	Exits userspace
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "sys.h"
#include "debug/debug.h"

uint64_t sys_exit(uint64_t error_code) {
	debug_options((struct DebugOptions){DEBUG_TYPE_INFO, true}, "Exiting userspace\n");
	return error_code;
}