/*
 * File:		main.c
 * Description:	Kernel entry point
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
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