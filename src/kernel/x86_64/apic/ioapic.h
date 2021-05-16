/*
 * File:		ioapic.h
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

// IO APIC selector registers
#define	IOAPIC_REG_SELECT_OFFSET	0x00
#define	IOAPIC_REG_WINDOW_OFFSET	0x10


enum IOAPICRedirectionEntryDeliveryMode {
	IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_FIXED			= 0b000,
	IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_LOW_PRIORITY		= 0b001,
	IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_SMI				= 0b010,
	IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_NMI				= 0b100,
	IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_INIT				= 0b101,
	IOAPIC_REDIRECTION_ENTRY_DELIVERY_MODE_EXTINT			= 0b111,
};
enum IOAPICRedirectionEntryDestinationMode {
	IOAPIC_REDIRECTION_ENTRY_DESTINATION_MODE_PHYSICAL	= 0,
	IOAPIC_REDIRECTION_ENTRY_DESTINATION_MODE_LOGICAL	= 1
};
enum IOAPICRedirectionEntryDeliveryStatus {
	IOAPIC_REDIRECTION_ENTRY_DELIVERY_STATUS_IDLE		= 0,
	IOAPIC_REDIRECTION_ENTRY_DELIVERY_STATUS_PENDING	= 1
};
enum IOAPICRedirectionEntryPinPolarity {
	IOAPIC_REDIRECTION_ENTRY_PIN_POLARITY_ACTIVEHIGH	= 0,
	IOAPIC_REDIRECTION_ENTRY_PIN_POLARITY_ACTIVELOW		= 1
};
enum IOAPICRedirectionEntryRemoteIRR {
	IOAPIC_REDIRECTION_ENTRY_REMOTE_IRR_NONE		= 0,
	IOAPIC_REDIRECTION_ENTRY_REMOTE_IRR_INFLIGHT	= 1,
};
enum IOAPICRedirectionEntryTriggerMode {
	IOAPIC_REDIRECTION_ENTRY_TRIGGER_MODE_EDGE	= 0,
	IOAPIC_REDIRECTION_ENTRY_TRIGGER_MODE_LEVEL	= 1
};
enum IOAPICRedirectionEntryMask {
	IOAPIC_REDIRECTION_ENTRY_MASK_ENABLE	= 0,	// Masks the interrupt through
	IOAPIC_REDIRECTION_ENTRY_MASK_DISABLE	= 1		// Masks the so it doesn't go through
};

enum IOAPICRedirectionBitsLow {
	IOAPIC_REDIRECTION_BITS_LOW_VECTOR				= 0,
	IOAPIC_REDIRECTION_BITS_LOW_DELIVERY_MODE		= 8,
	IOAPIC_REDIRECTION_BITS_LOW_DESTINATION_MODE	= 11,
	IOAPIC_REDIRECTION_BITS_LOW_DELIVERY_STATUS		= 12,
	IOAPIC_REDIRECTION_BITS_LOW_PIN_POLARITY		= 13,
	IOAPIC_REDIRECTION_BITS_LOW_REMOTE_IRR			= 14,
	IOAPIC_REDIRECTION_BITS_LOW_TRIGGER_MODE		= 15,
	IOAPIC_REDIRECTION_BITS_LOW_MASK				= 16,
};
enum IOAPICRedirectionBitsHigh {
	IOAPIC_REDIRECTION_BITS_HIGH_DESTINATION	= 24
};

// This is NOT directly casted, this is only to transfer redirection information
struct IOAPICRedirectionEntry {
	uint8_t										vector : 8;
	enum IOAPICRedirectionEntryDeliveryMode		delivery_mode : 3;
	enum IOAPICRedirectionEntryDestinationMode	destination_mode : 1;
	enum IOAPICRedirectionEntryDeliveryStatus	delivery_status : 1;
	enum IOAPICRedirectionEntryPinPolarity		pin_polarity : 1;
	enum IOAPICRedirectionEntryRemoteIRR		remote_irr : 1;
	enum IOAPICRedirectionEntryTriggerMode		trigger_mode : 1;
	enum IOAPICRedirectionEntryMask				mask : 1;
	uint8_t										destination : 8;
};

// Intel 82093AA data sheet
enum IOAPICRegisterOffset {
	IOAPIC_REG_OFFSET_ID				= 0x00,
	IOAPIC_REG_OFFSET_VERSION			= 0x01,
	IOAPIC_REG_OFFSET_ARBITRATION 		= 0x02,
	IOAPIC_REG_OFFSET_REDIRECTION_TABLE	= 0x10,
};

struct IOAPIC {
	struct MADTIOAPIC*	ioapic;
	uint8_t				max_interrupts;
};

// Initializes each IOAPIC
void ioapic_init();

// Sets the the ISRs from the IDT
void ioapic_set_from_isrs();

// Sets the [mask] the interrupt of [isr_num] (also known as the source plus 32)
void ioapic_entry_set_mask(size_t isr_num, enum IOAPICRedirectionEntryMask mask);

// Sets the redirection entry in [ioapic_ptr] to [entry] at [index]
void ioapic_set_redirection_entry(void* ioapic_ptr, size_t index, struct IOAPICRedirectionEntry entry);
// Transforms and writes the redirection [entry] in [ioapic_ptr] at [index] 
void ioapic_get_redirection_entry(void* ioapic_ptr, size_t index, struct IOAPICRedirectionEntry* entry);