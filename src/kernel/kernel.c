/*
 * File:		main.c
 * Description:	Kernel entry point
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include <stdio.h>
//#include <stdlib.h>
#include "x86_64/cpu.h"
#include "sys/sys.h"

// From setup.c
extern void setup_pre();
extern void setup_post();

struct KernelEntryInterface* g_interface;

void _start(struct KernelEntryInterface* interface) {
	g_interface = interface;
	
	setup_pre();
	cpu_init_bsp();
	setup_post();

	while (true);
}