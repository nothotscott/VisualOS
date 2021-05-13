/*
 * File:		main.c
 * Description:	Kernel entry points
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "x86_64/cpu.h"
#include "x86_64/apic/local_apic.h"
#include "bootloader.h"

void kernel_common() __attribute__((noreturn));
// From setup.c
extern void setup_pre();


void kernel_common() {
	setup_pre();
	cpu_init_bsp();
	local_apic_start_smp();

	while (true);
}