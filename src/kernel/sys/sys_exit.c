/*
 * File:		sys_exit.c
 * Description:	Exits userspace
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "sys.h"
#include "logging/log.h"

uint64_t sys_exit(uint64_t error_code) {
	log_options((struct LogOptions){LOG_TYPE_INFO, true}, "Exiting userspace\n");
	return error_code;
}