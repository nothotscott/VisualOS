/*
 * File:		pci.h
 * Description:	PCI related headers and functions
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "acpi.h"

#define PCI_UNKNOWN_IDENTIFIER	"?"

#define PCI_MAX_LANES	32

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
	uint8_t		revision_id, program_if, subclass, class;
	uint8_t		cache_line_size, latency_timer, header_type, bist;
} __attribute__((packed));

struct PCIClassIdentifier {
	uint8_t		class;
	const char*	name;
};
struct PCISublassIdentifier {
	uint8_t		class;
	uint8_t		subclass;
	const char*	name;
};
struct PCIProgrammingInterfaceIdentifier {
	uint8_t		class;
	uint8_t		subclass;
	uint8_t		program_if;
	const char*	name;
};

struct PCIVendorIdentifier {
	uint16_t	vendor_id;
	const char*	name;
};


// List of PCI identifiers
extern struct PCIClassIdentifier g_pci_classes[];
extern struct PCISublassIdentifier g_pci_subclasses[];
extern struct PCIProgrammingInterfaceIdentifier g_pci_progifs[];
extern struct PCIVendorIdentifier g_pci_vendors[];


// Initializes PCI using the ACPI [mcfg]
void pci_init(struct MCFGHeader* mcfg);

// Looks up the PCI string based on [pci_header]
const char* pci_get_class_str(struct PCIDeviceHeader* pci_header);
const char* pci_get_subclass_str(struct PCIDeviceHeader* pci_header);
const char* pci_get_progif_str(struct PCIDeviceHeader* pci_header);
const char* pci_get_vendor_str(struct PCIDeviceHeader* pci_header);


// Prints the PCI info
void pci_print();