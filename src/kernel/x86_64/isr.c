/*
 * File:		isr.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "interrupt_handler.h"
#include "scheduler/timer.h"
#include "isr.h"

void (*g_isr_handlers[ISR_MAX])(struct InterruptStack*, size_t);


void isr_init() {
	// Internal interrupts
	isr_register_handler(8, double_fault_handler);
	isr_register_handler(13, general_protection_fault_handler);
	isr_register_handler(14, paging_fault_handler);
	// IRQs
	isr_register_handler(32, pit_handler);
	isr_register_handler(33, keyboard_handler);
	isr_register_handler(39, spurious_interrupt_handler);
	// Custom interrupts
	isr_register_handler(48, timer_handler);
}

void isr_register_handler(size_t num, void (*handler)(struct InterruptStack*, size_t)) {
	g_isr_handlers[num] = handler;
}

bool isr_exists(size_t num) {
	return g_isr_handlers[num] != 0;
}