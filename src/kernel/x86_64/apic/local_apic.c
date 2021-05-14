/*
 * File:		local_apic.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "log.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/pageframe.h"
#include "scheduler/scheduler.h"
#include "x86_64/pit.h"
#include "x86_64/cpu.h"
#include "madt.h"
#include "local_apic.h"

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

static void* s_trampoline_code_ptr = &trampoline_code;
static void* s_trampoline_data_ptr = &trampoline_data;


void local_apic_init() {
	void* trampoline_target = (void*)LOCAL_APIC_TRAMPOLINE_TARGET;
	paging_identity_map(trampoline_target, LOCAL_APIC_TRAMPOLINE_TARGET_SIZE);
	// Enable APIC software enabled
	void* local_apic_ptr = madt_get_info()->local_apic_ptr;
	uint32_t spurious_reg = local_apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_SPURIOUS_INT_VECTOR);
	local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_SPURIOUS_INT_VECTOR, spurious_reg | 0x100);
}

void local_apic_start_smp() {
	uint32_t command_low, command_high;
	uint8_t bsp_local_apic_id = cpu_get_bsp()->local_apic_id;
	void* local_apic_ptr = madt_get_info()->local_apic_ptr;
	void* trampoline_target = (void*)LOCAL_APIC_TRAMPOLINE_TARGET;
	uint8_t trampoline_target_page = (uint8_t)(((uint64_t)trampoline_target >> 12) & 0xff);
	volatile struct LocalAPICApplicationProcessorCommunication* communicator = trampoline_target + (s_trampoline_data_ptr - s_trampoline_code_ptr);
	// INIT each AP
	size_t processors_num = madt_get_info()->processors_num;
	size_t cpu_context_size = ROUND_UP(sizeof(struct CPUContext), LOCAL_APIC_AP_CPU_CONTEXT_ALIGNMENT);
	void* ap_contexts = pageframe_request_pages(NEAREST_PAGE(cpu_context_size * (processors_num - 1)));
	size_t ai = 0;	// adjusted i
	for(size_t i = 0; i < processors_num; i++) {
		struct LocalAPICProcessor* processor = madt_get_info()->processors + i;
		uint8_t local_apic_id = processor->local_processor->local_apic_id;
		// Don't execute AP startup code on the BSP
		if(local_apic_id == bsp_local_apic_id){
			continue;
		}
		// Store the trampoline code at the target. Will also clear any data from previous AP
		memcpy(trampoline_target, s_trampoline_code_ptr, LOCAL_APIC_TRAMPOLINE_TARGET_SIZE);
		// Clear APIC errors
		local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_ERROR_STATUS, 0);
		// Send INIT to the AP
		local_apic_ipi_get_command(local_apic_ptr, &command_low, &command_high);
		local_apic_ipi_set_command(local_apic_ptr,
			(command_low & 0xfff32000) | 0xc500,
			(command_high & 0x00ffffff) | (local_apic_id << 24)
		);
		local_apic_wait(local_apic_ptr, &command_low);
		// Deassert (clear Level bit)
		local_apic_ipi_get_command(local_apic_ptr, &command_low, &command_high);
		local_apic_ipi_set_command(local_apic_ptr,
			(command_low & 0xfff00000) | 0x8500,
			(command_high & 0x00ffffff) | (local_apic_id << 24)
		);
		local_apic_wait(local_apic_ptr, &command_low);
		pit_sleep(LOCAL_APIC_SLEEP_DELAY_INIT);
		// AP STARTUP
		for(size_t j = 0; j < 2; j++) {
			// Clear APIC errors and set trampoline code page in vector
			local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_ERROR_STATUS, 0);
			local_apic_ipi_set_command(local_apic_ptr,
				(command_low & 0xfff0f800) | 0x600 | trampoline_target_page,
				(command_high & 0x00ffffff) | (local_apic_id << 24)
			);
			pit_sleep(LOCAL_APIC_SLEEP_DELAY_AP_STARTUP);
			local_apic_wait(local_apic_ptr, &command_low);
		}
		// Check on the status of the AP
		uint8_t ap_status = communicator->ap_status;
		while(communicator->ap_status == 0) {
			pit_sleep(LOCAL_APIC_SLEEP_DELAY_AP_STARTUP);
		}
		// Configure the AP, knowing it's responded and it's ready
		struct CPUContext* ap_context = ap_contexts + ai * cpu_context_size;
		size_t ap_stack_size = LOCAL_APIC_AP_STACK_PAGES * MEMORY_PAGE_SIZE;
		void* ap_stack = pageframe_request_pages(LOCAL_APIC_AP_STACK_PAGES) + ap_stack_size;
		processor->stack_ptr = ap_stack;
		processor->stack_size = ap_stack_size;
		// Give the AP the tools it needs to successfully start
		communicator->stack_ptr = (uint64_t)ap_stack;
		communicator->stack_size = (uint32_t)ap_stack_size;
		communicator->ap_context = (uint64_t)ap_context;
		communicator->cpu_init_ap = (uint64_t)&cpu_init_ap;
		communicator->scheduler_entry = (uint64_t)scheduler_entry;
		//paging_identity_map_page
		communicator->pagetable_l4 = (uint32_t)((uint64_t)paging_get_pagetable_l4() & 0xffffffff);
		// Give the AP the ok signal
		communicator->bsp_status = 1;
		// Wait for another response
		do {
			pit_sleep(LOCAL_APIC_SLEEP_DELAY_AP_STARTUP);
		} while(communicator->ap_status == 0);
		log_options((struct LogOptions){LOG_TYPE_WARNING, true}, "Processor %d (context at 0x%x) responded with %d\n",
			ap_context->local_apic_id, communicator->ap_context, communicator->ap_status
		);
		// Finish this iteration
		ai++;
	}
}

void local_apic_start_lints() {
	void* local_apic_ptr = madt_get_info()->local_apic_ptr;
	uint8_t local_apic_id = local_apic_get_id(local_apic_ptr);
	// Set Spurious interrupts and enable APIC software enabled
	uint32_t spurious_reg = local_apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_SPURIOUS_INT_VECTOR);
	local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_SPURIOUS_INT_VECTOR, spurious_reg | LOCAL_APIC_INTERRUPT_VECTOR_SPURIOUS | 0x100);
	// Set NMI from MADT
	size_t nmis_num = madt_get_info()->nmis_num;
	for(size_t i = 0; i < nmis_num; i++) {
		struct MADTNonMaskableInterrupt* nmi = madt_get_info()->nmis[i];
		if(nmi->local_apic_id != 0xff && nmi->local_apic_id != local_apic_id) {
			continue;
		}
		uint32_t nmi_entry = local_apic_create_register_value((struct LocalAPICInterruptRegister){
			.vector = LOCAL_APIC_INTERRUPT_VECTOR_LINT,
			.message_type = LOCAL_APIC_INTERRUPT_REGISTER_MESSAGE_TYPE_NMI,
			.trigger_mode = nmi->flags & 8 ? LOCAL_APIC_INTERRUPT_REGISTER_TRIGGER_MODE_LEVEL : LOCAL_APIC_INTERRUPT_REGISTER_TRIGGER_MODE_EDGE,
			.mask = LOCAL_APIC_INTERRUPT_REGISTER_MASK_DISABLE	// TODO figure the rest of this out
		});
		switch(nmi->lint) {
			case 0:
				local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_LVT_LINT0, nmi_entry);
				break;
			case 1:
				local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_LVT_LINT1, nmi_entry);
				break;
		}
	}
	// Setup Local APIC timer
	local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_INITIAL_COUNT, 0x100000);
	uint32_t divide_reg = local_apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_DIVIDE_CONFIG);
	local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_DIVIDE_CONFIG, (divide_reg & 0xfffffff4) | 0b1010);
	uint32_t timer_reg = local_apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_LVT_TIMER);
	local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_LVT_TIMER, local_apic_create_register_value((struct LocalAPICInterruptRegister){
		.vector = LOCAL_APIC_INTERRUPT_VECTOR_TIMER,
		.mask = LOCAL_APIC_INTERRUPT_REGISTER_MASK_ENABLE,
		.timer_mode = LOCAL_APIC_INTERRUPT_REGISTER_TIMER_MODE_PERIODIC
	}) | (timer_reg & 0xfffcef00));
}

void local_apic_eoi() {
	void* local_apic_ptr = madt_get_info()->local_apic_ptr;
	local_apic_set_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_EOI, 0);
}

// *** Local APIC Register Functions *** //

uint32_t local_apic_create_register_value(struct LocalAPICInterruptRegister reg) {
	return (
		(reg.vector << LOCAL_APIC_INTERRUPT_REGISTER_VECTOR) |
		(reg.message_type << LOCAL_APIC_INTERRUPT_REGISTER_MESSAGE_TYPE) |
		(reg.delivery_status << LOCAL_APIC_INTERRUPT_REGISTER_DELIVERY_STATUS) |
		(reg.trigger_mode << LOCAL_APIC_INTERRUPT_REGISTER_TRIGGER_MODE) |
		(reg.mask << LOCAL_APIC_INTERRUPT_REGISTER_MASK) |
		(reg.timer_mode << LOCAL_APIC_INTERRUPT_REGISTER_TIMER_MODE)
	);
}

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

uint8_t local_apic_get_id(void* local_apic_ptr) {
	return local_apic_get_register(local_apic_ptr, LOCAL_APIC_REG_OFFSET_ID) >> 24;
}