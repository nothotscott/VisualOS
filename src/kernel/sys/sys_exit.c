/*
 * File:		sys_exit.c
 * Description:	Exits userspace
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "sys.h"
#include "shell/debug.h"

uint64_t sys_exit(uint64_t error_code) {
	debug_output_info("Exiting userspace\n", true);
	return error_code;
}