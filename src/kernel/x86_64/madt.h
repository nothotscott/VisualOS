/*
 * File:		madt.h
 * Description:	Parse's the MADT (APIC signature) table
 * 				MADT stands for Multiple APIC Description Table
 * 				The file is named this instead of apic so that it's not
 * 				lexically confused with acpi.
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "acpi.h"

#define MADT_MAX_PROCESSORS	256
#define MADT_IO_APICS		256

struct MADTRecord {
	uint8_t	type;
	uint8_t	length;
} __attribute__((packed));

// Type 0: Single physical processor and its local interrupt controller
struct MADTLocalProcessor {
	struct MADTRecord	record;
	uint8_t				processor_id;
	uint8_t				apic_id;
	uint32_t			flags;
} __attribute__((packed));
// Type 1: IO APIC, the global system interrupt base is the first interrupt number that this IO APIC handles
struct MADTIOAPIC {
	struct MADTRecord	record;
	uint8_t				apic_id;
	uint8_t				reserved;
	uint32_t			apic_address;
	uint32_t			global_interrupt_base;
} __attribute__((packed));
// Type 2: Interrupt Source Override explaining how IRQ sources are mapped to global system interrupts
struct MADTInterruptSourceOverride {
	struct MADTRecord	record;
	uint8_t				bus;				// Always 0 (ISA)
	uint8_t				source;
	uint32_t			global_interrupt;
	uint16_t			flags;
} __attribute__((packed));
// Type 4: Non-maskable interrupt(NMI), which is connected to each processor where it exists
struct MADTNonMaskableInterrupt {
	struct MADTRecord	record;
	uint8_t				apic_id;
	uint16_t			flags;
	uint8_t				lint;
} __attribute__((packed));
// Type 5: Local APIC Address Override which provides 64 bit systems with an override of the physical address of the local APIC
// There should only be 1 of these
struct MADTLocalAPICAddressOverride {
	struct MADTRecord	record;
	uint8_t				reserved[2];
	uint64_t			local_apic_address;
} __attribute__((packed));


enum MADTType {
	MADT_TYPE_LOCAL_PROCESSOR				= 0,
	MADT_TYPE_IOAPIC						= 1,
	MADT_TYPE_INTERRUPT_SOURCE_OVERRIDE		= 2,
	MADT_TYPE_NONMASKABLE_INTERRUPT			= 4,
	MADT_TYPE_LOCAL_APICADDRESS_OVERRIDE	= 5
};

enum MADTIOAPICTriggerMode {
	MADT_IOAPIC_TRIGGER_MODE_EDGE,
	MADT_IOAPIC_TRIGGER_MODE_LEVEL
};
enum MADTIOAPICPolarity {
	MADT_IOAPIC_POLARITY_ACTIVEHIGH,
	MADT_IOAPIC_POLARITY_ACTIVELOW
};
enum MADTIOAPICDestinationMode {
	MADT_IOAPIC_DESTINATIONMODE_PHYSICAL,
	MADT_IOAPIC_DESTINATIONMODE_LOGICAL
};
enum MADTIOAPICDeliveryMode {
	MADT_IOAPIC_DELIVERY_MODE_FIXED,
	MADT_IOAPIC_DELIVERY_MODE_LOWESTPRIORITY,
	MADT_IOAPIC_DELIVERY_MODE_SMI,
	MADT_IOAPIC_DELIVERY_MODE_NMI				= 0b100,
	MADT_IOAPIC_DELIVERY_MODE_INIT,
	MADT_IOAPIC_DELIVERY_MODE_EXTINT			= 0b111,
};


// Initializes ACPI from [madt]
void madt_init(struct MADTHeader* madt);