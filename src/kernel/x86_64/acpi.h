/*
 * File:		acpi.h
 * Description:	ACPI definitions
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define ACPI_XSIZE	8

// Root System Descriptor Table Pointer 2.0
struct RSDP2 {
	char		signature[8];
	uint8_t		checksum;
	uint8_t		oem_id[6];
	uint8_t		revision;
	uint32_t	rsdt_base;		// physical address of the root system descriptor table
	// 2.0
	uint32_t	length;			// length of table
	uint64_t	xsdt_base;		// address of the extended system descriptor table
	uint8_t		xchecksum;		// eXtended checksum
	uint8_t		reserved[3];
} __attribute__((packed));

// System Descriptor Table Header
struct SDTHeader {
	char		signature[4];
	uint32_t	length;				// inclusive of header
	uint8_t		revision;
	uint8_t		checksum;
	uint8_t		oem_id[6];
	uint8_t		oem_table_id[8];
	uint32_t	oem_revision;
	uint32_t	creator_id;
	uint32_t	creator_revision;
} __attribute__((packed));

// Memory Mapped Configuration Space Access Header
struct MCFGHeader {
	struct SDTHeader	header;
	uint8_t				reserved[8];
} __attribute__((packed));

// Multiple APIC Description Table Header
struct MADTHeader {
	struct SDTHeader	header;
	uint32_t			apic_address;
	uint32_t			flags;
} __attribute__((packed));

// Looks up and returns the ACPI table starting at [xsdt] with the [signature]
struct SDTHeader* acpi_get_table(struct SDTHeader* xsdt, char* signature);