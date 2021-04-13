/*
 * File:		interrupt_handlers.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdio.h>
#include "x86_64/io.h"
#include "debug/debug.h"
#include "pit.h"
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


// *** Exceptions *** //

void double_fault_handler(struct InterruptStack* stack, size_t num) {
	debug_options((struct DebugOptions){DEBUG_TYPE_ERROR, true}, "\nDOUBLE FAULT DETECTED: %d\n", stack->error_code);
	while(true);
}

void general_protection_fault_handler(struct InterruptStack* stack, size_t num) {
	debug_options((struct DebugOptions){DEBUG_TYPE_ERROR, true}, "\nGENERAL PROTECTION FAULT DETECTED: %d\n", stack->error_code);
	while(true);
}

void paging_fault_handler(struct InterruptStack* stack, size_t num) {
	debug_options((struct DebugOptions){DEBUG_TYPE_ERROR, true}, "\nPAGE FAULT DETECTED: %d\n", stack->error_code);
	while(true);
}


// *** IRQs *** //

void pit_handler(struct InterruptStack* stack, size_t num) {
	pit_on_interrupt();
	io_pic_end_master();
}

void keyboard_handler(struct InterruptStack* stack, size_t num) {
	uint8_t scancode = inb(0x60);
	if(scancode < sizeof(s_keyboard_scancodes)) {
		char chr = s_keyboard_scancodes[scancode];
		ungetc(chr, stdin);
		// For now, get back the char and output to stdout
		putchar((char)getchar());
		fflush(stdout);

	}
	io_pic_end_slave();
}