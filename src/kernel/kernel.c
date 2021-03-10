/*
 * File:		main.c
 * Description:	Kernel entry point
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdio.h>
#include "shell/text.h"
#include "x86_64/cpuid.h"
#include "stdlib.h"
#include "string.h"

// From setup.c
extern void setup();

struct KernelEntryInterface* g_interface;

void _start(struct KernelEntryInterface *interface) {
	g_interface = interface;
	
	setup();
	text_output("Hello, kernel!\n");
	if(cpuid_get_vendor() == CPUID_VENDOR_AMD) {
		text_output("You're AMD!\n");
	}
	//void* test = malloc(4096);
	
	while (true);
}