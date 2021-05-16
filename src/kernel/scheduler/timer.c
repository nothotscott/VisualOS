/*
 * File:		timer.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "log.h"
#include "x86_64/isr.h"
#include "x86_64/apic/madt.h"
#include "x86_64/apic/local_apic.h"
#include "timer.h"

/*void timer_handler(struct InterruptStack* stack, size_t num) {
	void* local_apic_ptr = madt_get_info()->local_apic_ptr;
	uint8_t local_apic_id = local_apic_get_id(local_apic_ptr);
	log_to_screen("%d\n", local_apic_id);
}*/