/*
 * File:		timer.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "log.h"
#include "x86_64/isr.h"
#include "timer.h"

void timer_handler(struct InterruptStack* stack, size_t num) {
	log("a\n");
}