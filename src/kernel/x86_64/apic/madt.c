/*
 * File:		apic.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <debug/debug.h>
#include "memory/paging.h"
#include "x86_64/acpi.h"
#include "apic.h"
#include "madt.h"


// Static arrays
static struct MADTLocalProcessor* s_processors[MADT_MAX_PROCESSORS];
static size_t s_processors_num = 0;
static struct MADTIOAPIC* s_ioapics[MADT_IO_APICS];
static size_t s_ioapics_num = 0;


void madt_init(struct MADTHeader* madt) {
	void* apic_ptr = (void*)(uint64_t)madt->local_apic_address;
	paging_identity_map_page(apic_ptr);
	debug("Bootstrap processor APIC Address: 0x%x\n", apic_ptr);
	for(size_t t = 0; t < madt->header.length - sizeof(struct MADTHeader); /* Do not increment here */ ){
		struct MADTRecord* record = (struct MADTRecord*)((uint64_t)madt + sizeof(struct MADTHeader) + t);
		paging_identity_map_page(record);
		debug("Record type: %d size: %d\n", record->type, record->length);
		t += record->length;	// increment here
		switch(record->type) {
			case MADT_TYPE_LOCAL_PROCESSOR:
				{
					struct MADTLocalProcessor* processor = (struct MADTLocalProcessor*)record;
					s_processors[s_processors_num] = processor;
					s_processors_num++;
					debug("  Processor ID: %d APIC-ID: %d Flags: %d\n", processor->processor_id, processor->local_apic_id, processor->flags);
				}
				break;
			case MADT_TYPE_IOAPIC:
				{
					struct MADTIOAPIC* io_apic = (struct MADTIOAPIC*)record;
					void* apic_ptr = (void*)(uint64_t)io_apic->io_apic_address;
					paging_identity_map_page(apic_ptr);
					s_ioapics[s_ioapics_num] = io_apic;
					s_ioapics_num++;
					debug("  IO APIC: 0x%x\n", apic_ptr);
				}
				break;
			case MADT_TYPE_INTERRUPT_SOURCE_OVERRIDE:
				{
					struct MADTInterruptSourceOverride* iso = (struct MADTInterruptSourceOverride*)record;
					debug("  Interrupt Source Override source: 0x%d\n", iso->source);
				}
				break;
			case MADT_TYPE_NONMASKABLE_INTERRUPT:
				{
					struct MADTNonMaskableInterrupt* nmi = (struct MADTNonMaskableInterrupt*)record;
					debug("  Non-Maskable Interrupt ID: %d\n", nmi->local_apic_id);
				}
				break;
			case MADT_TYPE_LOCAL_APICADDRESS_OVERRIDE:
				break;
		}
	}
}

struct MADTLocalProcessor** get_processors() {
	return s_processors;
}
size_t get_processors_num() {
	return s_processors_num;
}
struct MADTIOAPIC** get_ioapics() {
	return s_ioapics;
}
size_t get_ioapics_num() {
	return s_ioapics_num;
}