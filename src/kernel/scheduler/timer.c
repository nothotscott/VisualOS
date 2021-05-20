/*
 * File:		timer.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "x86_64/apic/local_apic.h"
#include "timer.h"


void timer_set_ticks(timer_ticks_t ticks) {
	local_apic_set_timer_count(ticks);
}