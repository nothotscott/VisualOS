/*
 * File:		ioapic.h
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

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

// Intel 82093AA data sheet
enum IOAPICRegisterOffset {	
	IOAPIC_REG_OFFSET_ID			= 0x00,
	IOAPIC_REG_OFFSET_VERSION		= 0x01,
	IOAPIC_REG_OFFSET_ARBITRATION 	= 0x02,
	IOAPIC_REG_OFFSET_REBTBL_BASE	= 0x10,
};

struct IOAPIC {
	struct MADTIOAPIC*	ioapic;
	uint8_t				num_pins;
};


// Initializes each IOAPIC
void ioapic_init();