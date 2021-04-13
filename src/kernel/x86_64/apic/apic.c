/*
 * File:		apic.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "debug/debug.h"
#include "memory/pageframe.h"
#include "x86_64/cpuid.h"
#include "madt.h"
#include "apic.h"


void apic_init() {
	struct CPUIDFeatures features;
	cpuid_get_features(&features);
	uint8_t bsp_local_apic_id = (uint8_t)((features.values >> CPUID_VALUE_LOCAL_APIC_ID) & 0xff);
	debug("LAPIC ID: %d\n", bsp_local_apic_id);
	void* trampoline_target = (void*)APIC_TRAMPOLINE_TARGET;
	size_t processors_num = get_processors_num();
	for(size_t i = 0; i < processors_num; i++) {
		struct MADTLocalProcessor* processor = get_processors()[i];
		// Don't execute AP startup code on the BSP
		if(processor->local_apic_id == bsp_local_apic_id){
			continue;
		}
		debug("Proc %d\n", i);
	}

}

void apic_ipi_get_command(void* local_apic_ptr, uint32_t* command_low, uint32_t* command_high) {
	*command_low = *((volatile uint32_t*)local_apic_ptr + LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x00);
	*command_high = *((volatile uint32_t*)local_apic_ptr + LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x10);
}
void apic_ipi_set_command(void* local_apic_ptr, uint32_t command_low, uint32_t command_high) {
	*((volatile uint32_t*)local_apic_ptr + LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x00) = command_low;
	*((volatile uint32_t*)local_apic_ptr + LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x10) = command_high;
}