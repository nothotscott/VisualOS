/*
 * File:		ioapic.c
 * Description:	IO redirections and interrupts through the IOAPIC
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "log.h"
#include "x86_64/isr.h"
#include "x86_64/io.h"
#include "madt.h"
#include "ioapic.h"


// Inline functions to get/set IOAPIC registers
static inline volatile uint32_t ioapic_get_register(void* ioapic_ptr, uint8_t reg_offset);
static inline void ioapic_set_register(void* ioapic_ptr, uint8_t reg_offset, uint32_t value);

void ioapic_init() {
	size_t ioapics_num = madt_get_info()->ioapics_num;
	size_t isos_num = madt_get_info()->isos_num;
	for(size_t i = 0; i < ioapics_num; i++) {
		// Configure IOAPIC
		struct IOAPIC* ioapic = madt_get_info()->ioapics + i;
		void* ioapic_ptr = (void*)(uint64_t)ioapic->ioapic->ioapic_address;
		uint8_t max_interrupts = ((ioapic_get_register(ioapic_ptr, IOAPIC_REG_OFFSET_VERSION) >> 16) & 0xff) + 1;
		ioapic->max_interrupts = max_interrupts;
		// Set the entries
		uint32_t base = ioapic->ioapic->global_interrupt_base;
		for(size_t j = 0; j < isos_num; j++) {
			struct MADTInterruptSourceOverride* iso = madt_get_info()->isos[j];
			ioapic_set_redirection_entry(ioapic_ptr, iso->global_interrupt, (struct IOAPICRedirectionEntry){
				.vector = iso->source + ISR_IRQ_START,
				.delivery_mode = IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_FIXED,
				.destination_mode = IOAPIC_REDIRECTION_ENTRY_DESTINATION_MODE_PHYSICAL,
				.pin_polarity = (iso->flags & 0x03) == 0x03 ? IOAPIC_REDIRECTION_ENTRY_PIN_POLARITY_ACTIVELOW : IOAPIC_REDIRECTION_ENTRY_PIN_POLARITY_ACTIVEHIGH,
				.trigger_mode = (iso->flags & 0x0c) == 0x0c ? IOAPIC_REDIRECTION_ENTRY_TRIGGER_MODE_LEVEL : IOAPIC_REDIRECTION_ENTRY_TRIGGER_MODE_EDGE,
				.mask = IOAPIC_REDIRECTION_ENTRY_MASK_ENABLE,
			});
		}
		log("IOAPIC %d:%d Initialized\n", ioapic->ioapic->ioapic_id, max_interrupts);
	}
}

void ioapic_set_from_isrs() {
	size_t ioapics_num = madt_get_info()->ioapics_num;
	size_t isos_num = madt_get_info()->isos_num;
	for(size_t i = 0; i < ioapics_num; i++) {
		struct IOAPIC* ioapic = madt_get_info()->ioapics + i;
		void* ioapic_ptr = (void*)(uint64_t)ioapic->ioapic->ioapic_address;
		for(size_t isr_num = ISR_IRQ_START; isr_num < ISR_MAX; isr_num++) {
			if(!isr_exists(isr_num)) {
				continue;
			}
			for(size_t j = 0; j < isos_num; j++) {
				struct MADTInterruptSourceOverride* iso = madt_get_info()->isos[j];
				if(iso->source + ISR_IRQ_START == isr_num) {	// already redirected
					goto next;
				}
			}
			ioapic_set_redirection_entry(ioapic_ptr, isr_num - ISR_IRQ_START, (struct IOAPICRedirectionEntry){
				.vector = isr_num,
				.delivery_mode = IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_LOW_PRIORITY,
				.destination_mode = IOAPIC_REDIRECTION_ENTRY_DESTINATION_MODE_PHYSICAL,
				.pin_polarity = IOAPIC_REDIRECTION_ENTRY_PIN_POLARITY_ACTIVEHIGH,
				.trigger_mode = IOAPIC_REDIRECTION_ENTRY_TRIGGER_MODE_EDGE,
				.mask = IOAPIC_REDIRECTION_ENTRY_MASK_ENABLE
			});
			next:
				continue;
		}
	}
}


// *** IOAPIC Register Functions *** //

static inline volatile uint32_t ioapic_get_register(void* ioapic_ptr, uint8_t reg_offset) {
	*(volatile uint32_t*)(ioapic_ptr + IOAPIC_REG_SELECT_OFFSET) = reg_offset;
	return *(volatile uint32_t*)(ioapic_ptr + IOAPIC_REG_WINDOW_OFFSET);
}
static inline void ioapic_set_register(void* ioapic_ptr, uint8_t reg_offset, uint32_t value) {
	*(volatile uint32_t*)(ioapic_ptr + IOAPIC_REG_SELECT_OFFSET) = reg_offset;
	*(volatile uint32_t*)(ioapic_ptr + IOAPIC_REG_WINDOW_OFFSET) = value;
}


void ioapic_get_redirection_entry(void* ioapic_ptr, size_t index, struct IOAPICRedirectionEntry* entry) {
	volatile uint32_t low = ioapic_get_register(ioapic_ptr, IOAPIC_REG_OFFSET_REDIRECTION_TABLE + 2 * index + 0);
	volatile uint32_t high = ioapic_get_register(ioapic_ptr, IOAPIC_REG_OFFSET_REDIRECTION_TABLE + 2 * index + 1);
	*entry = (struct IOAPICRedirectionEntry){
		.vector = (uint8_t)(low >> IOAPIC_REDIRECTION_BITS_LOW_VECTOR),
		.delivery_mode = (low >> IOAPIC_REDIRECTION_BITS_LOW_DELIVERY_MODE),
		.destination_mode = (low >> IOAPIC_REDIRECTION_BITS_LOW_DESTINATION_MODE),
		.delivery_status = (low >> IOAPIC_REDIRECTION_BITS_LOW_DELIVERY_STATUS),
		.pin_polarity = (low >> IOAPIC_REDIRECTION_BITS_LOW_PIN_POLARITY),
		.remote_irr = (low >> IOAPIC_REDIRECTION_BITS_LOW_REMOTE_IRR),
		.trigger_mode = (low >> IOAPIC_REDIRECTION_BITS_LOW_TRIGGER_MODE),
		.mask = (low >> IOAPIC_REDIRECTION_BITS_LOW_MASK),
		.destination = (uint8_t)(high >> IOAPIC_REDIRECTION_BITS_HIGH_DESTINATION)
	};

}

void ioapic_set_redirection_entry(void* ioapic_ptr, size_t index, struct IOAPICRedirectionEntry entry) {
	volatile uint32_t low = (
		(entry.vector << IOAPIC_REDIRECTION_BITS_LOW_VECTOR) |
		(entry.delivery_mode << IOAPIC_REDIRECTION_BITS_LOW_DELIVERY_MODE) |
		(entry.destination_mode << IOAPIC_REDIRECTION_BITS_LOW_DESTINATION_MODE) |
		(entry.delivery_status << IOAPIC_REDIRECTION_BITS_LOW_DELIVERY_STATUS) |
		(entry.pin_polarity << IOAPIC_REDIRECTION_BITS_LOW_PIN_POLARITY) |
		(entry.remote_irr << IOAPIC_REDIRECTION_BITS_LOW_REMOTE_IRR) |
		(entry.trigger_mode << IOAPIC_REDIRECTION_BITS_LOW_TRIGGER_MODE) |
		(entry.mask << IOAPIC_REDIRECTION_BITS_LOW_MASK)
	);
	volatile uint32_t high = (
		(entry.destination << IOAPIC_REDIRECTION_BITS_HIGH_DESTINATION)
	);
	ioapic_set_register(ioapic_ptr, IOAPIC_REG_OFFSET_REDIRECTION_TABLE + 2 * index + 0, low);
	ioapic_set_register(ioapic_ptr, IOAPIC_REG_OFFSET_REDIRECTION_TABLE + 2 * index + 1, high);
}