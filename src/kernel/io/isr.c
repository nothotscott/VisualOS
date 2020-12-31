/*
 * File:		isr.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "io.h"
#include "pic.h"
#include "isr.h"




#include <string.h>
#include "shell/shell.h"
#include "shell/tools.h"


void (*g_isr_handlers[IO_ISR_MAX])(ulong_t);

const char kb_scan_codes[] ={
	0, 0, '1', '2',
	'3', '4', '5', '6',
	'7', '8', '9', '0',
	'-', '=', 0, 0,
	'q', 'w', 'e', 'r',
	't', 'y', 'u', 'i',
	'o', 'p', '[', ']',
	0, 0, 'a', 's',
	'd', 'f', 'g', 'h',
	'j', 'k', 'l', ';',
	'\'', '`', 0, '\\',
	'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',',
	'.', '/', 0, '*',
	0, ' '
};


void isr33_handler() {
	byte_t scancode = inb(0x60);
	if(scancode < sizeof(kb_scan_codes)) {
		print_char(kb_scan_codes[scancode], SHELL_COLOR_GREEN);
	}
	outb(IO_PIC1_COMMAND, IO_PIC_EOI);
	outb(IO_PIC2_COMMAND, IO_PIC_EOI);
}

void isr_register_handler(size_t num, void (*handler)(ulong_t)) {
	g_isr_handlers[num] = handler;
}
