/*
 * File:		kernel.c
 * Description:	Entry point of the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "bootloader.h"
#include "shell/shell.h"
#include "shell/tools.h"

// From setup.c
extern void setup();

// Global interface
struct KernelEntryInterface* g_interface;


void _start(struct KernelEntryInterface* interface) {
	g_interface = interface;
	setup();

	while(1);
}

//print_memory((void*)pagemap.buffer, pagemap.size, SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);