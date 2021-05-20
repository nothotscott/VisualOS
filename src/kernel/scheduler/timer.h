/*
 * File:		timer.h
 * Description:	Timer interface, with the LAPIC timer, for scheduling
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "x86_64/isr.h"

#define TIMER_TICKS_DEFAULT	0x100000

typedef	uint64_t	timer_ticks_t;


// Sets the current timer to countdown [ticks] times until the next interrupt
void timer_set_ticks(timer_ticks_t ticks);

// Local APIC timer handler
void timer_handler(struct InterruptStack* stack, size_t num);