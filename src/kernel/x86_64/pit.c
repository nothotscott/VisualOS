/*
 * File:		pit.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "io.h"
#include "pit.h"

#define PIT_BASE_FREQUENCY_MILLI	PIT_BASE_FREQUENCY * 10000

volatile static uint64_t s_ticks_since_boot = 0;
static uint16_t s_divisor = PIT_MAX_DIVISOR;


void pit_init() {
	pit_set_divisor(PIT_MAX_DIVISOR);
}

void pit_set_divisor(uint64_t divisor) {
	s_divisor = divisor;
	if(s_divisor > PIT_MAX_DIVISOR){
		s_divisor = PIT_MAX_DIVISOR;
	}
	outb(PIT_PORT, (uint8_t)(s_divisor & 0x00ff));	// low byte
	io_wait();
	outb(PIT_PORT, (uint8_t)((s_divisor & 0xff00) >> 8));	// high byte
}

uint64_t pit_get_frequency() {
	return PIT_BASE_FREQUENCY / s_divisor;
}

void pit_set_frequency(uint64_t frequency) {
	pit_set_divisor(PIT_BASE_FREQUENCY / frequency);
}

void pit_on_interrupt(){
	s_ticks_since_boot += s_divisor;
}

uint64_t pit_get_ticks_since_boot() {
	return s_ticks_since_boot;
}

void sleep(uint64_t milliseconds) {
	volatile uint64_t start_ticks = s_ticks_since_boot;
	while((s_ticks_since_boot - start_ticks) / PIT_BASE_FREQUENCY_MILLI < milliseconds) {
		__asm__ volatile("hlt"); // TODO something more useful
	}
}