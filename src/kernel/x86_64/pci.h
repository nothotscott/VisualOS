/*
 * File:		pci.h
 * Description:	PCI related headers and functions
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "acpi.h"


struct PCIDeviceConfigurationDescriptor {
	uint64_t	base;
	uint16_t	pci_segment_group;
	uint8_t		bus_start;
	uint8_t		bus_end;
	uint8_t		reserved[4];
} __attribute__((packed));


// Initializes PCI using the ACPI [mcfg]
void pci_init(struct MCFGHeader* mcfg);