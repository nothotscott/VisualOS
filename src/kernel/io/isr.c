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
	byte_t scancode = inb(0x60);
	print(string_str_from_ulong((ulong_t)scancode), SHELL_COLOR_FOREGROUND);
	outb(IO_PIC1_COMMAND, IO_PIC_EOI);
	outb(IO_PIC2_COMMAND, IO_PIC_EOI);
}