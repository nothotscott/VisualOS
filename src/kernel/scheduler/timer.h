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

// Local APIC timer handler
void timer_handler(struct InterruptStack* stack, size_t num);