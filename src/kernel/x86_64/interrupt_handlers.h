/*
 * File:		interrupt_handlers.h
 * Description:	Handles interrupts and dispatches events to userspace
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


#include "interrupt.h"


// *** Exceptions *** //

// ISR 8
void double_fault_handler(struct InterruptStack* stack, size_t num);
// ISR 13
void general_protection_fault_handler(struct InterruptStack* stack, size_t num);
// ISR 14
void paging_fault_handler(struct InterruptStack* stack, size_t num);

// *** IRQs *** //

// ISR 32
void pit_handler(struct InterruptStack* stack, size_t num);
// ISR 33
void keyboard_handler(struct InterruptStack* stack, size_t num);