/*
 * File:		pci.h
 * Description:	PCI related headers and functions
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "acpi.h"


#define PCI_MAX_DEVICES		32	// per bus
#define PCI_MAX_FUNCTIONS	8	// per device


struct PCIDeviceConfigurationDescriptor {
	uint64_t	base;
	uint16_t	pci_segment_group;
	uint8_t		bus_start;
	uint8_t		bus_end;
	uint8_t		reserved[4];
} __attribute__((packed));

struct PCIDeviceHeader {
	uint16_t	vendor_id, device_id;
	uint16_t	command, status;
	uint8_t		revision_id, program_if, subclass, class_code;
	uint8_t		cache_line_size, latency_timer, header_type, bist;
	/*uint32_t	base_address[6];
	uint32_t	cardbus_cis_pointer;
	uint16_t	subsystem_vendor_id, subsystem_id;
	uint32_t	expansion_rom_base_address;
	uint8_t		capability_pointer;
	uint32_t	reserved : 24;
	uint32_t	reserved2;*/

	uint8_t interrupt_line, interrupt_pin, min_grant, max_latency;
} __attribute__((packed));



// Initializes PCI using the ACPI [mcfg]
void pci_init(struct MCFGHeader* mcfg);