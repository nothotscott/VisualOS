/*
 * File:		keyboard.c
 * Description:	Unknown
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "io.h"
#include "pic.h"
#include "keyboard.h"

//////////////
#include <string.h>
#include "shell/shell.h"
#include "shell/tools.h"
//////////////

const char g_keyboard_scancodes[] ={
	0, 0, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', 0, 0,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', 0, 0, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};


void keyboard_handler(struct InterruptStack* stack, size_t num) {
	byte_t scancode = inb(0x60);
	if(scancode < sizeof(g_keyboard_scancodes)) {
		print_char(g_keyboard_scancodes[scancode], SHELL_COLOR_GREEN);
	}
	outb(IO_PIC1_COMMAND, IO_PIC_EOI);
	outb(IO_PIC2_COMMAND, IO_PIC_EOI);
}
