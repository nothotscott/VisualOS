/*
 * File:		pci.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "acpi.h"
#include "pci.h"


void pci_init(struct MCFGHeader* mcfg) {
	for	(uint64_t* table_ptr = (uint64_t*)((uint64_t)mcfg + sizeof(struct MCFGHeader));
		table_ptr < (uint64_t*)((uint64_t)mcfg + mcfg->length);
		table_ptr++
	) {
		struct PCIDeviceConfigurationDescriptor* descriptor = (struct PCIDeviceConfigurationDescriptor*)*table_ptr;
	}
}