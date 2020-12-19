/*
 * File:		pic.c
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#include "io.h"
#include "pic.h"


void pic_mask(){
	outb(IO_PIC1_DATA, 0xFD);
	outb(IO_PIC2_DATA, 0xFF);
}

void pic_remap(){
	// Original PIC data
	byte d1 = inb(IO_PIC1_DATA);
	byte d2 = inb(IO_PIC2_DATA);
	// Tell PIC's to start the initialization sequence
	outb(IO_PIC1_COMMAND, IO_ICW1_INIT | IO_ICW1_ICW4);
	outb(IO_PIC2_COMMAND, IO_ICW1_INIT | IO_ICW1_ICW4);
	// Tell each PIC their offset
	outb(IO_PIC1_DATA, 0x28);
	outb(IO_PIC2_DATA, 0x20);
	outb(IO_PIC1_DATA, 4);
	outb(IO_PIC2_DATA, 2);
	// Set 8086 mode
	outb(IO_PIC1_DATA, IO_ICW4_8086);
	outb(IO_PIC2_DATA, IO_ICW4_8086);
	// Restore saved masks
	outb(IO_PIC1_DATA, d1);
	outb(IO_PIC2_DATA, d2);
}