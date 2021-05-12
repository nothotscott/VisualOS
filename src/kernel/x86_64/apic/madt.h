/*
 * File:		madt.h
 * Description:	Parse's the MADT (APIC signature) table
 * 				MADT stands for Multiple APIC Description Table
 * Note:		The MADTHeader struct is in acpi.h
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "local_apic.h"
#include "ioapic.h"
#include "x86_64/acpi.h"

// I have no clue with these values
// Please let me know if you do
#define MADT_MAX_PROCESSORS					128
#define MADT_MAX_IO_APICS					8
#define MADT_MAX_INTERRUPT_SOURCE_OVERRIDES	16


struct MADTInformation {
	void*								local_apic_ptr;
	struct LocalAPICProcessor			processors[MADT_MAX_PROCESSORS];
	size_t								processors_num;
	struct IOAPIC						ioapics[MADT_MAX_IO_APICS];
	size_t								ioapics_num;
	struct MADTInterruptSourceOverride*	isos[MADT_MAX_INTERRUPT_SOURCE_OVERRIDES];
	size_t								isos_num;
};

struct MADTRecord {
	uint8_t	type;
	uint8_t	length;
} __attribute__((packed));

// Type 0: Single physical processor and its local interrupt controller
struct MADTLocalProcessor {
	struct MADTRecord	record;
	uint8_t				processor_id;
	uint8_t				local_apic_id;
	uint32_t			flags;
} __attribute__((packed));
// Type 1: IO APIC, the global system interrupt base is the first interrupt number that this IO APIC handles
struct MADTIOAPIC {
	struct MADTRecord	record;
	uint8_t				ioapic_id;
	uint8_t				reserved;
	uint32_t			ioapic_address;
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
	uint8_t				local_apic_id;
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
	MADT_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE	= 5,
	MADT_TYPE_IO_SAPIC						= 6,
	MADT_TYPE_LOCAL_SAPIC					= 7,
	MADT_TYPE_PLAT_INTERRUPT_SRC			= 8,
	MADT_TYPE_PROCESSOR_LOCAL_X2APIC		= 9,
	MADT_TYPE_LOCAL_X2APIC_NMI				= 10,
	MADT_TYPE_GIC_CPU_INTERFACE				= 11,
	MADT_TYPE_GIC_DISTRIBUTOR				= 12,
	MADT_TYPE_GIC_MSI_FRAME					= 13,
	MADT_TYPE_GIC_REDISTRIBUTOR				= 14,
	MADT_TYPE_GIC_INT_TRANSLATION			= 15,
	MADT_TYPE_MULTIPROCESSOR_WAKEUP			= 16,
};

// Initializes ACPI from [madt]
void madt_init(struct MADTHeader* madt);

// *** Getters *** //

struct MADTHeader* madt_get();
struct MADTInformation* madt_get_info();