/*
 * File:		timer.h
 * Description:	Uses the LAPIC timer for scheduling
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

// Local APIC timer handler
void timer_handler(struct InterruptStack* stack, size_t num);