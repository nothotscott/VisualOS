/*
 * File:		pit.h
 * Description:	Programmable Interrupt Timer
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


#define PIT_BASE_FREQUENCY	1193182
#define PIT_MAX_DIVISOR		65535
#define PIT_DEFAULT_DIVISOR	PIT_MAX_DIVISOR / 10

#define PIT_PORT	0x40


// Initialized the PIT
void pit_init();

// Sets the [divisor] in calculating the frequency
void pit_set_divisor(uint64_t divisor);

// Gets the calculated frequency
uint64_t pit_get_frequency();

// Sets the [frequency] of the PIT
void pit_set_frequency(uint64_t frequency);

// Local PIT interrupt handler
void pit_on_interrupt();

// Gets the ticks since boot
uint64_t pit_get_ticks_since_boot();

// TODO remove or modify this for scheduling
// Sleeps for [milliseconds]
void sleep(uint64_t milliseconds);