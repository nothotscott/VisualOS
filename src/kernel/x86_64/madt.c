/*
 * File:		apic.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "memory/paging.h"
#include "acpi.h"
#include "madt.h"


// Array of processors
static struct MADTLocalProcessor* s_processors[MADT_MAX_PROCESSORS];
static size_t s_processors_num = 0;


#include <stdio.h>
void madt_init(struct MADTHeader* madt) {
	for(size_t t = 0; t < madt->header.length - sizeof(struct MADTHeader); /* Do not increment here */ ){
		struct MADTRecord* record = (struct MADTRecord*)((uint64_t)madt + sizeof(struct MADTHeader) + t);
		paging_identity_map_page(record);
		printf("Record type: %d size: %d\n", record->type, record->length);
		t += record->length;	// increment here
		switch(record->type) {
			case MADT_TYPE_LOCAL_PROCESSOR:
				{
					struct MADTLocalProcessor* processor = (struct MADTLocalProcessor*)record;
					s_processors[s_processors_num] = processor;
					s_processors_num++;
					printf("  Processor ID: %d APIC-ID: %d Flags: %d\n", processor->processor_id, processor->apic_id, processor->flags);
				}
				break;
			case MADT_TYPE_IOAPIC:
				{
					struct MADTIOAPIC* io_apic = (struct MADTIOAPIC*)record;
					void* apic_ptr = (void*)(uint64_t)io_apic->apic_address;
					paging_identity_map_page(apic_ptr);
					printf("  IO APIC: 0x%x\n", apic_ptr);
				}
				break;
			case MADT_TYPE_INTERRUPT_SOURCE_OVERRIDE:
				{
					struct MADTInterruptSourceOverride* iso = (struct MADTInterruptSourceOverride*)record;
					printf("  Interrupt Source Override source: 0x%d\n", iso->source);
				}
				break;
			case MADT_TYPE_NONMASKABLE_INTERRUPT:
				{
					struct MADTNonMaskableInterrupt* nmi = (struct MADTNonMaskableInterrupt*)record;
					printf("  Non-Maskable Interrupt ID: %d\n", nmi->apic_id);
				}
				break;
			case MADT_TYPE_LOCAL_APICADDRESS_OVERRIDE:
				break;
		}
	}
}