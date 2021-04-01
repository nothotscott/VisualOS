/*
 * File:		main.c
 * Description:	Kernel entry point
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include <stdio.h>
#include "shell/text.h"
#include "x86_64/cpuid.h"
#include "stdlib.h"

// From setup.c
extern void setup();

struct KernelEntryInterface* g_interface;

void _start(struct KernelEntryInterface* interface) {
	g_interface = interface;
	
	setup();
	text_output("Hello, kernel!\n");
	
	printf ("Characters: %c %c \n", 'a', 65);
	printf ("Decimals: %d %ld\n", 1977, 650000L);
	printf ("Preceding with blanks: %10d \n", 1977);
	printf ("Preceding with zeros: %010d \n", 1977);
	printf ("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
	printf ("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
	printf ("Width trick: %*d \n", 5, 10);
	printf ("%s \n", "A string");

	while (true);
}