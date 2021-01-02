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
#include "memory/heap.h"
// From setup.c
extern void setup();
// From info.c
extern void print_info();
// From below
extern void kernel_loop();

// Global interface
struct KernelEntryInterface* g_interface;

void _start(struct KernelEntryInterface* interface) {
	g_interface = interface;
	setup();
	print_info();

	byte_t* pageFaulter = (byte_t*)0x8000000000;
	*pageFaulter = 69;

	kernel_loop();
}

void kernel_loop() {
	print("This is normal", SHELL_COLOR_FOREGROUND);
	while(true);
}