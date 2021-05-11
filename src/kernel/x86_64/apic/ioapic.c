/*
 * File:		ioapic.c
 * Description:	IO redirections and interrupts through the IOAPIC
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "log.h"
#include "madt.h"
#include "ioapic.h"


// Inline functions to get/set IOAPIC registers
static inline uint32_t ioapic_get_register(void* ioapic_ptr, uint8_t reg_offset);
static inline void ioapic_set_register(void* ioapic_ptr, uint8_t reg_offset, uint32_t value);


void ioapic_init() {
	size_t ioapics_num = get_ioapics_num();
	for(size_t i = 0; i < ioapics_num; i++) {
		struct IOAPIC* ioapic = get_ioapic(i);
		void* ioapic_ptr = (void*)(uint64_t)ioapic->ioapic->ioapic_address;
		uint8_t num_pins = ((ioapic_get_register(ioapic_ptr, IOAPIC_REG_OFFSET_VERSION) >> 16) & 0xff) + 1;
		ioapic->num_pins = num_pins;
		log("IOAPIC %d: %d\n", ioapic->ioapic->ioapic_id, num_pins);
	}
}


// *** IOAPIC Register Functions *** //

static inline uint32_t ioapic_get_register(void* ioapic_ptr, uint8_t reg_offset) {
	*(volatile uint32_t*)(ioapic_ptr + IOAPIC_REG_SELECT_OFFSET) = reg_offset;
	return *(volatile uint32_t*)(ioapic_ptr + IOAPIC_REG_WIN_OFFSET);
}
static inline void ioapic_set_register(void* ioapic_ptr, uint8_t reg_offset, uint32_t value) {
	*(volatile uint32_t*)(ioapic_ptr + IOAPIC_REG_SELECT_OFFSET) = reg_offset;
	*(volatile uint32_t*)(ioapic_ptr + IOAPIC_REG_WIN_OFFSET) = value;
}