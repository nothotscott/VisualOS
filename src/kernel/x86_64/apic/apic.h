/*
 * File:		apic.h
 * Description:	Executes APIC-scoped commands
 * 				Not to be confused with the lexically similar acpi
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 * Thanks to borrrden for supplying some of the APIC datatype values
 */

#pragma once


#define APIC_TRAMPOLINE_TARGET		0x8000
#define APIC_TRAMPOLINE_TARGET_SIZE	4096

#define APIC_SLEEP_DELAY_INIT		10	// miliseconds to sleep after INIT sequence
#define APIC_SLEEP_DELAY_AP_STARTUP	1	// miliseconds to sleep after each AP STARTUP

// IO APIC selector registers
#define	IOAPIC_REG_SELECT_OFFSET	0x00
#define	IOAPIC_REG_WIN_OFFSET		0x10

enum IOAPICTriggerMode {
	IOAPIC_TRIGGER_MODE_EDGE,
	IOAPIC_TRIGGER_MODE_LEVEL
};
enum IOAPICPolarity {
	IOAPIC_POLARITY_ACTIVEHIGH,
	IOAPIC_POLARITY_ACTIVELOW
};
enum IOAPICDestinationMode {
	IOAPIC_DESTINATIONMODE_PHYSICAL,
	IOAPIC_DESTINATIONMODE_LOGICAL
};
enum IOAPICDeliveryMode {
	IOAPIC_DELIVERY_MODE_FIXED,
	IOAPIC_DELIVERY_MODE_LOWESTPRIORITY,
	IOAPIC_DELIVERY_MODE_SMI,
	IOAPIC_DELIVERY_MODE_NMI				= 0b100,
	IOAPIC_DELIVERY_MODE_INIT,
	IOAPIC_DELIVERY_MODE_EXTINT				= 0b111,
};

// Intel SDM Volume 3 Table 10-1
enum LocalAPICRegisterOffset {
	LOCAL_APIC_REG_OFFSET_ID					= 0x0020,
	LOCAL_APIC_REG_OFFSET_VERSION				= 0x0030,
	LOCAL_APIC_REG_OFFSET_TASK_PRIORITY			= 0x0080,
	LOCAL_APIC_REG_OFFSET_ARBITRATION_PRIORITY	= 0x0090,
	LOCAL_APIC_REG_OFFSET_PROCESSOR_PRIORITY	= 0x00a0,
	LOCAL_APIC_REG_OFFSET_EOI					= 0x00b0,
	LOCAL_APIC_REG_OFFSET_REMOTE_READ			= 0x00c0,
	LOCAL_APIC_REG_OFFSET_LOGICAL_DESTINATION	= 0x00d0,
	LOCAL_APIC_REG_OFFSET_DESTINATION_FORMAT	= 0x00e0,
	LOCAL_APIC_REG_OFFSET_SPURIOUS_INT_VECTOR	= 0x00f0,
	LOCAL_APIC_REG_OFFSET_IN_SERVICE			= 0x0100,
	LOCAL_APIC_REG_OFFSET_TRIGGER_MODE			= 0x0180,
	LOCAL_APIC_REG_OFFSET_INTERRUPT_REQUEST		= 0x0200,
	LOCAL_APIC_REG_OFFSET_ERROR_STATUS			= 0x0280,
	LOCAL_APIC_REG_OFFSET_CMCI					= 0x02f0,
	LOCAL_APIC_REG_OFFSET_INTERRUPT_COMMAND		= 0x0300,
	LOCAL_APIC_REG_OFFSET_LVT_TIMER				= 0x0320,
	LOCAL_APIC_REG_OFFSET_LVT_THERMAL_SENSOR	= 0x0330,
	LOCAL_APIC_REG_OFFSET_LVT_PERF_MONITOR		= 0x0340,
	LOCAL_APIC_REG_OFFSET_LVT_LINT0				= 0x0350,
	LOCAL_APIC_REG_OFFSET_LVT_LINT1				= 0x0360,
	LOCAL_APIC_REG_OFFSET_LVT_ERROR				= 0x0370,
	LOCAL_APIC_REG_OFFSET_INITIAL_COUNT			= 0x0380,
	LOCAL_APIC_REG_OFFSET_CURRENT_COUNT			= 0x0390,
	LOCAL_APIC_REG_OFFSET_DIVIDE_CONFIG			= 0x03e0,
};

// Intel 82093AA data sheet
enum IOAPICRegisterOffset {	
    IOAPIC_REG_OFFSET_IOAPICID		= 0x00,
    IOAPIC_REG_OFFSET_IOAPICVER		= 0x01,
    IOAPIC_REG_OFFSET_IOAPICARB 	= 0x02,
    IOAPIC_REG_OFFSET_IOREBTBL_BASE	= 0x10,
};

// VOS specifc data structure to communicate with the AP
struct ApplicationProcessorCommunication {
	uint8_t		ap_status;
	uint8_t		bsp_status;
	uint32_t	pagetable_l4;
	uint64_t	stack_ptr;
	uint32_t	stack_size;
	uint64_t	ap_context;
	uint64_t	cpu_init_ap;
} __attribute__((packed));

// Initializes the bootstrap processor
void apic_init();

// Launches all application processors to be ready for symmetric multiprocessing
void apic_start_smp();

// Gets the interprocessor interrupt for at [local_apic_ptr] and stores it at [command_low] and [command_high]
void local_apic_ipi_get_command(void* local_apic_ptr, uint32_t* command_low, uint32_t* command_high);
// Issues an interprocessor interrupt [command_low] and [command_high] to [local_apic_ptr]
void local_apic_ipi_set_command(void* local_apic_ptr, uint32_t command_low, uint32_t command_high);
