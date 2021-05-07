/*
 * File:		apic.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "logging/log.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/pageframe.h"
#include "x86_64/pit.h"
#include "x86_64/cpu.h"
#include "madt.h"
#include "apic.h"

#define PAUSE()	__asm__ __volatile__ ("pause" : : : "memory")


// From trampoline.asm
// Pretend they're functions to trick the compiler and avoid errors
extern void trampoline_code();
extern void trampoline_data();

// Inline functions to get/set Local APIC registers
static inline uint32_t local_apic_get_register(void* local_apic_ptr, size_t reg_offset);
static inline void local_apic_set_register(void* local_apic_ptr, size_t reg_offset, uint32_t value);
// Waits for the APIC to complete its IPI
static inline void local_apic_wait(void* local_apic_ptr, uint32_t* command_low);
// Inline functions to get/set IOAPIC registers
static inline uint32_t ioapic_get_register(void* ioapic_ptr, uint8_t reg_offset);
static inline void ioapic_set_register(void* ioapic_ptr, uint8_t reg_offset, uint32_t value);

static void* s_trampoline_code_ptr = &trampoline_code;
static void* s_trampoline_data_ptr = &trampoline_data;


void apic_init() {
	void* trampoline_target = (void*)APIC_TRAMPOLINE_TARGET;
	paging_identity_map(trampoline_target, APIC_TRAMPOLINE_TARGET_SIZE);
}

void apic_start_smp() {
	uint32_t command_low, command_high;
	uint8_t bsp_local_apic_id = cpu_get_bsp()->local_apic_id;
	void* local_apic_ptr = (void*)(uint64_t)get_madt()->local_apic_address;
	void* trampoline_target = (void*)APIC_TRAMPOLINE_TARGET;
	uint8_t trampoline_target_page = (uint8_t)(((uint64_t)trampoline_target >> 12) & 0xff);
	volatile struct ApplicationProcessorCommunication* communicator = trampoline_target + (s_trampoline_data_ptr - s_trampoline_code_ptr);
	// INIT each AP
	//debug_options((struct DebugOptions){DEBUG_TYPE_WARNING, true}, "BSP processor %d\n", bsp_local_apic_id);
	size_t processors_num = get_processors_num();
	struct CPUContext* ap_context = pageframe_request_pages(NEAREST_PAGE(sizeof(struct CPUContext) * (processors_num - 1)));
	for(size_t i = 0; i < processors_num; i++) {
		struct ApplicationProcessor* processor = get_processor(i);
		uint8_t local_apic_id = processor->local_processor->local_apic_id;
		// Don't execute AP startup code on the BSP
		if(local_apic_id == bsp_local_apic_id){
			continue;
		}
		//debug_options((struct DebugOptions){DEBUG_TYPE_WARNING, true}, "Signaling processor %d\n", local_apic_id);
		// Store the trampoline code at the target. Will also clear any data from previous AP
		memcpy(trampoline_target, s_trampoline_code_ptr, APIC_TRAMPOLINE_TARGET_SIZE);
		// Clear APIC errors
		local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_ERROR_STATUS, 0);
		// Send INIT to the AP
		local_apic_ipi_get_command(local_apic_ptr, &command_low, &command_high);
		local_apic_ipi_set_command(local_apic_ptr,
			(command_low & 0xfff32000) | 0xc500,				// trigger INIT
			(command_high & 0x00ffffff) | (local_apic_id << 24)	// select AP in DES bits
		);
		local_apic_wait(local_apic_ptr, &command_low);
		// Deassert
		local_apic_ipi_get_command(local_apic_ptr, &command_low, &command_high);
		local_apic_ipi_set_command(local_apic_ptr,
			(command_low & 0xfff00000) | 0x08500,				// deassert (clear Level bit)
			(command_high & 0x00ffffff) | (local_apic_id << 24)	// select AP
		);
		local_apic_wait(local_apic_ptr, &command_low);
		sleep(APIC_SLEEP_DELAY_INIT);
		// AP STARTUP
		for(size_t j = 0; j < 2; j++) {
			// Clear APIC errors
			local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_ERROR_STATUS, 0);
			local_apic_ipi_set_command(local_apic_ptr,
				(command_low & 0xfff0f800) | 0x600 | trampoline_target_page,	// STARTUP page in Vector
				(command_high & 0x00ffffff) | (local_apic_id << 24)				// select AP
			);
			sleep(APIC_SLEEP_DELAY_AP_STARTUP);
			local_apic_wait(local_apic_ptr, &command_low);
		}
		// Check on the status of the AP
		uint8_t ap_status = communicator->ap_status;
		while(communicator->ap_status == 0) {
			sleep(APIC_SLEEP_DELAY_AP_STARTUP);
		}
		// Configure the AP, knowing it's responded and it's ready
		void* ap_stack = pageframe_request();
		size_t ap_stack_size = MEMORY_PAGE_SIZE;
		processor->stack_ptr = ap_stack;
		processor->stack_size = ap_stack_size;
		// Give the AP the tools it needs to successfully start
		communicator->stack_ptr = (uint64_t)ap_stack;
		communicator->stack_size = (uint32_t)ap_stack_size;
		communicator->ap_context = (uint64_t)ap_context;
		communicator->cpu_init_ap = (uint64_t)&cpu_init_ap;
		//paging_identity_map_page
		communicator->pagetable_l4 = (uint32_t)((uint64_t)paging_get_pagetable_l4() & 0xffffffff);
		// Give the AP the ok signal
		communicator->bsp_status = 1;
		// Wait for another response
		do {
			sleep(APIC_SLEEP_DELAY_AP_STARTUP);
		} while(communicator->ap_status == 0);
		log_options((struct LogOptions){LOG_TYPE_WARNING, true}, "Processor %d (context at 0x%x) responded with %d\n",
			ap_context->local_apic_id, communicator->ap_context, communicator->ap_status
		);
		// Finish this iteration
		ap_context++;
	}

}

// *** Local APIC Register Functions *** //

static inline uint32_t local_apic_get_register(void* local_apic_ptr, size_t reg_offset) {
	return *((volatile uint32_t*)(local_apic_ptr + reg_offset));
}
static inline void local_apic_set_register(void* local_apic_ptr, size_t reg_offset, uint32_t value) {
	*((volatile uint32_t*)(local_apic_ptr + reg_offset)) = value;
}

__attribute__((always_inline)) static inline void local_apic_wait(void* local_apic_ptr, uint32_t* command_low) {
	do {
		PAUSE();
		local_apic_ipi_get_command(local_apic_ptr, command_low, NULL);
	} while(*command_low & (1 << 12));	// delivery status bit
}


void local_apic_ipi_get_command(void* local_apic_ptr, uint32_t* command_low, uint32_t* command_high) {
	if(command_low != NULL) {
		*command_low = local_apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x00);
	}
	if(command_high != NULL) {
		*command_high = local_apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x10);
	}
}
void local_apic_ipi_set_command(void* local_apic_ptr, uint32_t command_low, uint32_t command_high) {
	local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x10, command_high);
	local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND + 0x00, command_low);
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