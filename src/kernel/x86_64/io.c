/*
 * File:		io.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "io.h"


void io_pic_override() {
	outb(IO_PIC1_DATA, 0xff);
	outb(IO_PIC2_DATA, 0xff);
	io_wait();
	outb(IO_CHIPSET_ADDRESS_REGISTER, IO_IMCR_REGISTER_ADDRESS);
	outb(IO_CHIPSET_DATA_REGISTER, IO_IMCR_VIA_APIC);
}

void io_pic_mask(){
	outb(IO_PIC1_DATA, 0b11111000);
	outb(IO_PIC2_DATA, 0b11101111);
}

void io_pic_remap(){
	// Original PIC data
	uint8_t d1 = inb(IO_PIC1_DATA);
	io_wait();
	uint8_t d2 = inb(IO_PIC2_DATA);
	io_wait();
	// Tell PIC's to start the initialization sequence
	outb(IO_PIC1_COMMAND, IO_ICW1_INIT | IO_ICW1_ICW4);
	io_wait();
	outb(IO_PIC2_COMMAND, IO_ICW1_INIT | IO_ICW1_ICW4);
	io_wait();
	// Tell each PIC their offset
	outb(IO_PIC1_DATA, 0x20);
	io_wait();
	outb(IO_PIC2_DATA, 0x28);
	io_wait();
	outb(IO_PIC1_DATA, 0x04);
	io_wait();
	outb(IO_PIC2_DATA, 0x02);
	io_wait();
	outb(IO_PIC1_DATA, IO_ICW4_8086);
	io_wait();
	outb(IO_PIC2_DATA, IO_ICW4_8086);
	io_wait();
	// Restore saved data
	outb(IO_PIC1_DATA, d1);
	io_wait();
	outb(IO_PIC2_DATA, d2);
}

void io_pic_end_master() {
	outb(IO_PIC1_COMMAND, IO_PIC_EOI);
}

void io_pic_end_slave() {
	outb(IO_PIC2_COMMAND, IO_PIC_EOI);
	outb(IO_PIC1_COMMAND, IO_PIC_EOI);
}