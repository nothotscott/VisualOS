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
	outb(IO_PIC1_DATA, 0xfd);
	outb(IO_PIC2_DATA, 0xff);
}

void pic_remap(){
	// Original PIC data
	byte d1 = inb(IO_PIC1_DATA);
	byte d2 = inb(IO_PIC2_DATA);
	// Tell PIC's to start the initialization sequence
	outb(IO_PIC1_COMMAND, IO_ICW1_INIT | IO_ICW1_ICW4);
	//io_wait();
	outb(IO_PIC2_COMMAND, IO_ICW1_INIT | IO_ICW1_ICW4);
	//io_wait();
	// Tell each PIC their offset
	outb(IO_PIC1_DATA, 0x20);
	//io_wait();
	outb(IO_PIC2_DATA, 0x28);
	//io_wait();
	outb(IO_PIC1_DATA, 0x04);
	//io_wait();
	outb(IO_PIC2_DATA, 0x02);
	//io_wait();
	outb(IO_PIC1_DATA, IO_ICW4_8086);
	//io_wait();
	outb(IO_PIC2_DATA, IO_ICW4_8086);
	//io_wait();
	// Restore saved data
	//outb(IO_PIC1_DATA, d1);
	//outb(IO_PIC2_DATA, d2);
	outb(IO_PIC1_DATA, 0x0);
	outb(IO_PIC2_DATA, 0x0);
}