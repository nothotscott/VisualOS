/*
 * File:		main.c
 * Description:	Kernel entry points
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "log.h"
#include "x86_64/cpu.h"
#include "bootloader.h"

#include <stdlib.h>

void kernel_common();
// From setup.c
extern void setup_pre();
extern void setup_post();

void kernel_common() {
	setup_pre();
	cpu_init_bsp();
	setup_post();

	log_options((struct LogOptions){LOG_TYPE_NORMAL, true}, "Welcome to VisualOS!\n");
}