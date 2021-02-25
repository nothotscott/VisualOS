/*
 * File:		interrupt_handlers.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "x86_64/io.h"
#include "shell/text.h"
#include "interrupt.h"
#include "interrupt_handlers.h"

static const char s_keyboard_scancodes[] ={
	0, 0, '1', '2', '3', '4', '5', '6',
	'7', '8', '9', '0', '-', '=', 0, 0,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', 0, 0, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};



void keyboard_handler(struct InterruptStack* stack, size_t num) {
	uint8_t scancode = inb(0x60);
	if(scancode < sizeof(s_keyboard_scancodes)) {
		text_output_char(s_keyboard_scancodes[scancode], TEXT_COLOR_GREEN);
	}
	outb(IO_PIC1_COMMAND, IO_PIC_EOI);
	outb(IO_PIC2_COMMAND, IO_PIC_EOI);
}


void double_fault_handler(struct InterruptStack* stack, size_t num) {
	text_output_newline();
	text_output_color("DOUBLE FAULT DETECTED", TEXT_COLOR_RED); text_output_newline();
	while(true);
}

void general_protection_fault_handler(struct InterruptStack* stack, size_t num) {
	text_output_newline();
	text_output_color("GENERAL PROTECTION FAULT DETECTED", TEXT_COLOR_RED); text_output_newline();
	while(true);
}

void paging_fault_handler(struct InterruptStack* stack, size_t num) {
	text_output_newline();
	text_output_color("PAGE FAULT DETECTED", TEXT_COLOR_RED); text_output_newline();
	while(true);
}
