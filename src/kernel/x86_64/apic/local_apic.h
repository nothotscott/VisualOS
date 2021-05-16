/*
 * File:		local_apic.h
 * Description:	Executes Local APIC-scoped commands
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 * Thanks to borrrden for supplying some of the APIC datatype values
 */

#pragma once


#define LOCAL_APIC_TRAMPOLINE_TARGET		0x8000
#define LOCAL_APIC_TRAMPOLINE_TARGET_SIZE	4096

#define LOCAL_APIC_AP_CPU_CONTEXT_ALIGNMENT	16
#define LOCAL_APIC_AP_STACK_PAGES			2

#define LOCAL_APIC_SLEEP_DELAY_INIT			10	// miliseconds to sleep after INIT sequence
#define LOCAL_APIC_SLEEP_DELAY_AP_STARTUP	1	// miliseconds to sleep after each AP STARTUP

enum LocalAPICInterruptRegisterMessageType {
	LOCAL_APIC_INTERRUPT_REGISTER_MESSAGE_TYPE_FIXED		= 0b000,
	LOCAL_APIC_INTERRUPT_REGISTER_MESSAGE_TYPE_SMI			= 0b010,
	LOCAL_APIC_INTERRUPT_REGISTER_MESSAGE_TYPE_NMI			= 0b100,
	LOCAL_APIC_INTERRUPT_REGISTER_MESSAGE_TYPE_EXTINT		= 0b111,
};
enum LocalAPICInterruptRegisterDeliveryStatus {
	LOCAL_APIC_INTERRUPT_REGISTER_DELIVERY_STATUS_IDLE		= 0,
	LOCAL_APIC_INTERRUPT_REGISTER_DELIVERY_STATUS_PENDING	= 1
};
enum LocalAPICInterruptRegisterRemoteIRR {
	LOCAL_APIC_INTERRUPT_REGISTER_REMOTE_COMPLETED	= 0,
	LOCAL_APIC_INTERRUPT_REGISTER_REMOTE_ACCEPTED	= 1,
};
enum LocalAPICInterruptRegisterTriggerMode {
	LOCAL_APIC_INTERRUPT_REGISTER_TRIGGER_MODE_EDGE		= 0,
	LOCAL_APIC_INTERRUPT_REGISTER_TRIGGER_MODE_LEVEL	= 1
};
enum LocalAPICInterruptRegisterMask {
	LOCAL_APIC_INTERRUPT_REGISTER_MASK_ENABLE	= 0,
	LOCAL_APIC_INTERRUPT_REGISTER_MASK_DISABLE	= 1
};
enum LocalAPICInterruptTimerMode {
	LOCAL_APIC_INTERRUPT_REGISTER_TIMER_MODE_ONESHOT	= 0,
	LOCAL_APIC_INTERRUPT_REGISTER_TIMER_MODE_PERIODIC	= 1
};

enum LocalAPICInterruptBits {
	LOCAL_APIC_INTERRUPT_REGISTER_VECTOR			= 0,
	LOCAL_APIC_INTERRUPT_REGISTER_MESSAGE_TYPE		= 8,
	LOCAL_APIC_INTERRUPT_REGISTER_DELIVERY_STATUS	= 12,
	LOCAL_APIC_INTERRUPT_REGISTER_TRIGGER_MODE		= 15,
	LOCAL_APIC_INTERRUPT_REGISTER_MASK				= 16,
	LOCAL_APIC_INTERRUPT_REGISTER_TIMER_MODE		= 17,
};

struct LocalAPICInterruptRegister {
	uint8_t											vector : 8;
	enum LocalAPICInterruptRegisterMessageType		message_type : 3;
	enum LocalAPICInterruptRegisterDeliveryStatus	delivery_status : 1;
	enum LocalAPICInterruptRegisterRemoteIRR		remote_irr : 1;
	enum LocalAPICInterruptRegisterTriggerMode		trigger_mode : 1;
	enum LocalAPICInterruptRegisterMask				mask : 1;
	enum LocalAPICInterruptTimerMode				timer_mode : 1;
};

enum LocalAPICInterruptVectors {
	LOCAL_APIC_INTERRUPT_VECTOR_SPURIOUS	= 39,
	LOCAL_APIC_INTERRUPT_VECTOR_LINT		= 255,
	LOCAL_APIC_INTERRUPT_VECTOR_TIMER		= 48
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

// VOS specifc data structure to communicate with the AP
struct LocalAPICApplicationProcessorCommunication {
	uint8_t		ap_status;
	uint8_t		bsp_status;
	uint32_t	pagetable_l4;
	uint64_t	stack_ptr;
	uint32_t	stack_size;
	uint64_t	ap_context;
	uint64_t	cpu_init_ap;
	uint64_t	scheduler_entry;
} __attribute__((packed));

struct LocalAPICProcessor {
	struct MADTLocalProcessor*	local_processor;
	void*						stack_ptr;
	size_t						stack_size;
};


// Initializes the bootstrap processor
void local_apic_init();

// Launches all application processors to be ready for symmetric multiprocessing
void local_apic_start_smp();

// Starts local interrupts for the calling processor(Spurious, NMIs and timer)
void local_apic_start_lints();

// Sets the initial count register to [value]
void local_apic_set_timer_count(uint32_t value);

// Sends the end of interrupt signal for the calling APIC
void local_apic_eoi();

// Returns the register value for general register [reg]
uint32_t local_apic_create_register_value(struct LocalAPICInterruptRegister reg);

// Gets the interprocessor interrupt for at [local_apic_ptr] and stores it at [command_low] and [command_high]
void local_apic_ipi_get_command(void* local_apic_ptr, uint32_t* command_low, uint32_t* command_high);
// Issues an interprocessor interrupt [command_low] and [command_high] to [local_apic_ptr]
void local_apic_ipi_set_command(void* local_apic_ptr, uint32_t command_low, uint32_t command_high);

// Gets the APIC ID of the processor calling this function
uint8_t local_apic_get_id(void* local_apic_ptr);