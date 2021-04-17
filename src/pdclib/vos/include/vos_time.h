/*
 * File:		vos_time.h
 * Description:	VOS-specific time definitions
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include <time.h>

struct timeval {
	time_t	tv_sec;
    long	tv_usec;
};

struct timezone {
	int	tz_minuteswest;
	int	tz_dsttime;
};

struct tms {
	clock_t	tms_utime;	// user time
	clock_t	tms_stime;	// system time
	clock_t	tms_cutime;	// user time of children
	clock_t	tms_cstime;	// system time of children
};