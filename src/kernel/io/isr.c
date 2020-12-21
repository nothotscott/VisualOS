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

void isr1_handler() {
	byte scancode = inb(0x60);
	print(string_str_from_ulong((ulong)scancode), SHELL_COLOR_FOREGROUND);
	outb(IO_PIC1_COMMAND, 0x20);
	outb(IO_PIC2_COMMAND, 0x20);
	//asm("leave; iret");
}