/*
 * File:		main.c
 * Description:	Kernel entry points
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "x86_64/cpu.h"
#include "bootloader.h"

void kernel_common() __attribute__((noreturn));
// From setup.c
extern void setup_pre();
extern void setup_post();


void kernel_common() {
	setup_pre();

	cpu_init_bsp();
	setup_post();

	while (true);
}