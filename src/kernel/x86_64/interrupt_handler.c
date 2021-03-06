/*
 * File:		interrupt_handler.c
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <stdio.h>
#include "log.h"
#include "symbols/symbol.h"
#include "symbols/unwind.h"
#include "x86_64/io.h"
#include "x86_64/cpuid.h"
#include "pit.h"
#include "isr.h"
#include "interrupt_handler.h"

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

static void _stack_trace_callbackee(size_t frame_num, uint64_t rip) {
	char* symbol_str;
	char* section_str;
	symbol_lookup((void*)rip, &symbol_str, &section_str);
	log_options((struct LogOptions){LOG_TYPE_ERROR, true}, "  %s:%s\n", section_str, symbol_str);
}

static inline void _general_handler(struct InterruptErrorStack* stack, size_t num, char* fault_type) {
	char* symbol_str;
	char* section_str;
	symbol_lookup((void*)stack->rip, &symbol_str, &section_str);
	log_options((struct LogOptions){LOG_TYPE_ERROR, true}, "\n%s:\n  LAPIC ID: %d\n  ERROR CODE: 0x%x\n  RIP: 0x%x\n  RSP: 0x%x\n  RBP: 0x%x\n", 
		fault_type, cpuid_get_local_apic_id(), stack->error_code, stack->rip, stack->rsp, stack->general.rbp, section_str
	);
	log_options((struct LogOptions){LOG_TYPE_ERROR, true}, "STACK TRACE:\n  %s:%s\n", section_str, symbol_str);
	unwind(stack->general.rbp, _stack_trace_callbackee);
}

void double_fault_handler(struct InterruptStack* stack, size_t num) {
	_general_handler((struct InterruptErrorStack*)stack, num, "DOUBLE FAULT");
	while(true);
}

void tss_fault_handler(struct InterruptStack* stack, size_t num) {
	_general_handler((struct InterruptErrorStack*)stack, num, "INVALID TASK STATE SEGMENT");
	while(true);
}

void segment_fault_handler(struct InterruptStack* stack, size_t num) {
	_general_handler((struct InterruptErrorStack*)stack, num, "SEGMENT NOT PRESENT EXCEPTION");
	while(true);
}

void general_protection_fault_handler(struct InterruptStack* stack, size_t num) {
	_general_handler((struct InterruptErrorStack*)stack, num, "GENERAL PROTECTION FAULT");
	while(true);
}

void paging_fault_handler(struct InterruptStack* stack, size_t num) {
	_general_handler((struct InterruptErrorStack*)stack, num, "PAGE FAULT");
	while(true);
}


// *** IRQs *** //

void pit_handler(struct InterruptStack* stack, size_t num) {
	pit_on_interrupt();
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
}

void spurious_interrupt_handler(struct InterruptStack* stack, size_t num) {
	log_default("Spurious interrupt\n");
}