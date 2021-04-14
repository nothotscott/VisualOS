/*
 * File:		apic.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "debug/debug.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "x86_64/cpuid.h"
#include "x86_64/pit.h"
#include "madt.h"
#include "apic.h"

#define PAUSE()													\
				__asm__ __volatile__ ("pause" : : : "memory")	\


// Inline functions to get/set APIC registers
static inline uint32_t apic_get_register(void* local_apic_ptr, size_t reg_offset);
static inline void apic_set_register(void* local_apic_ptr, size_t reg_offset, uint32_t value);
// Waits for the APIC to complete its IPI
static inline void apic_wait(void* local_apic_ptr, uint32_t* command_low);


void apic_init() {
	void* trampoline_target = (void*)APIC_TRAMPOLINE_TARGET;
	paging_identity_map(trampoline_target, APIC_TRAMPOLINE_TARGET_SIZE);
	memcpy(trampoline_target, &apic_trampoline, APIC_TRAMPOLINE_TARGET_SIZE);
}

void apic_start_smp() {
	uint32_t command_low, command_high;
	struct CPUIDFeatures features;
	cpuid_get_features(&features);
	uint8_t bsp_local_apic_id = (uint8_t)((features.values >> CPUID_VALUE_LOCAL_APIC_ID) & 0xff);
	void* local_apic_ptr = (void*)(uint64_t)get_madt()->local_apic_address;
	void* trampoline_target = (void*)APIC_TRAMPOLINE_TARGET;
	uint8_t trampoline_target_page = (uint8_t)(((uint64_t)trampoline_target >> 12) & 0xff);
	//uint8_t trampoline_target_segment =
	// INIT each AP
	size_t processors_num = get_processors_num();
	for(size_t i = 0; i < processors_num; i++) {
		struct MADTLocalProcessor* processor = get_processors()[i];
		// Don't execute AP startup code on the BSP
		if(processor->local_apic_id == bsp_local_apic_id){
			continue;
		}
		// Clear APIC errors
		apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_ERROR_STATUS, 0);
		// Send INIT to the AP
		apic_ipi_get_command(local_apic_ptr, &command_low, &command_high);
		apic_ipi_set_command(local_apic_ptr,
			(command_low & 0xfff32000) | 0xc500,							// trigger INIT
			(command_high & 0x00ffffff) | (processor->local_apic_id << 24)	// select AP in DES bits
		);
		apic_wait(local_apic_ptr, &command_low);
		// Deassert
		apic_ipi_get_command(local_apic_ptr, &command_low, &command_high);
		apic_ipi_set_command(local_apic_ptr,
			(command_low & 0xfff00000) | 0x08500,							// deassert (clear Level bit)
			(command_high & 0x00ffffff) | (processor->local_apic_id << 24)	// select AP
		);
		apic_wait(local_apic_ptr, &command_low);
		sleep(APIC_SLEEP_DELAY_INIT);
		// AP STARTUP
		for(size_t j = 0; j < 2; j++) {
			// Clear APIC errors
			apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_ERROR_STATUS, 0);
			apic_ipi_set_command(local_apic_ptr,
				(command_low & 0xfff0f800) | 0x600 | (trampoline_target_page),	// STARTUP page in Vector
				(command_high & 0x00ffffff) | (processor->local_apic_id << 24)	// select AP
			);
			sleep(APIC_SLEEP_DELAY_AP_STARTUP);
			apic_wait(local_apic_ptr, &command_low);
		}
		
		debug("Proc %d\n", i);
	}

}

static inline uint32_t apic_get_register(void* local_apic_ptr, size_t reg_offset) {
	return *((volatile uint32_t*)(local_apic_ptr + reg_offset));
}
static inline void apic_set_register(void* local_apic_ptr, size_t reg_offset, uint32_t value) {
	*((volatile uint32_t*)(local_apic_ptr + reg_offset)) = value;
}

__attribute__((always_inline)) static inline void apic_wait(void* local_apic_ptr, uint32_t* command_low) {
	do {
		PAUSE();
		apic_ipi_get_command(local_apic_ptr, command_low, NULL);
	} while(*command_low & (1 << 12));	// delivery status bit
}


void apic_ipi_get_command(void* local_apic_ptr, uint32_t* command_low, uint32_t* command_high) {
	if(command_low != NULL) {
		*command_low = apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x00);
	}
	if(command_high != NULL) {
		*command_high = apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x10);
	}
}
void apic_ipi_set_command(void* local_apic_ptr, uint32_t command_low, uint32_t command_high) {
	apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x10, command_high);
	apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x00, command_low);
}