/*
 * File:		main.c
 * Description:	Kernel entry point
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include <stdio.h>
#include "sys/sys.h"
#include "stdlib.h"

// From setup.c
extern void setup();

struct KernelEntryInterface* g_interface;

void sleep(uint64_t milliseconds);

void _start(struct KernelEntryInterface* interface) {
	g_interface = interface;
	
	setup();
	puts("Welcome to VisualOS!");
	//void* test = malloc(4096);

	/*for(int i = 0; i < 20; i++){
		text_output_char('g', TEXT_COLOR_FOREGROUND);
		sleep(50);
	}*/

	while (true);
}