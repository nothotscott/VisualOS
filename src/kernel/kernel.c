/*
 * File:		kernel.c
 * Description:	Entry point of the kernel
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include <string.h>
#include <conversion.h>
#include "bootloader.h"
#include "shell/shell.h"
#include "shell/tools.h"
#include "memory/heap.h"
// From setup.c
extern void setup();
// From info.c
extern void print_info();
// From below
//extern void kernel_loop();

// Global interface
struct KernelEntryInterface* g_interface;


void _start(struct KernelEntryInterface* interface) {
	g_interface = interface;
	setup();
	print_info();

	//byte_t* pageFaulter = (byte_t*)0x8000000000;
	//*pageFaulter = 69;

	double a = 13.14;
	char* poop = string_str_from_decimal(a, 5);
	print(poop, SHELL_COLOR_FOREGROUND);
	print_newline();
	print_memory(poop, _from_decimal_size(a, 5));
	print_newline();
	//char* test = string_str_from_format("---------------%x this is a pretty cool string longer than it should %d be", 0xaaaabbbb12345678, -123456789123456789);
	//print(test, SHELL_COLOR_FOREGROUND);
	printf_color("---------------%x this is a pretty cool string longer than it should %d be", SHELL_COLOR_FOREGROUND, 0xaaaabbbb12345678, -123456789123456789);
	print_newline();
	printf("This number is green: %GREEN %d%DEFAULT while this one is yellow: %YELLOW %x", 123, 123);
	print_newline();
	printf("This number is green: %GREEN %d ok %BLUE poop", 123);

	print_newline();
	//print_memory(test, _from_int_size(a), SHELL_COLOR_MEMORY_CONTENT, SHELL_COLOR_MEMORY_FADE);

	
	while(true);
}