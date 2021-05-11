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

void test_fault() {
	*(uint8_t*)(0x12341234abc) = 123;
}

void lets_go_deeper() {
	test_fault();
}

void kernel_common() {
	setup_pre();

	cpu_init_bsp();
	setup_post();

	lets_go_deeper();

	while (true);
}