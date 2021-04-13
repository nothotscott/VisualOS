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

#include "x86_64/acpi.h"

#define MADT_MAX_PROCESSORS	256
#define MADT_IO_APICS		8	// I have no clue


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
	uint8_t				io_apic_id;
	uint8_t				reserved;
	uint32_t			io_apic_address;
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
	MADT_TYPE_LOCAL_APICADDRESS_OVERRIDE	= 5,
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

// Getters
struct MADTLocalProcessor** get_processors();
size_t get_processors_num();
struct MADTIOAPIC** get_ioapics();
size_t get_ioapics_num();