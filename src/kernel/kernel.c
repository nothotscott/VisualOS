/*
 * File:		main.c
 * Description:	Kernel entry point
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdio.h>
#include "shell/text.h"

// From setup.c
extern void setup();

struct KernelEntryInterface* g_interface;

void _start(struct KernelEntryInterface *interface) {
	g_interface = interface;
	
	setup();
	text_output("Hello, kernel!\n");
	printf("test");
	
	while (true);
}