/*
 * File:		apic.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include <string.h>
#include "log.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/pageframe.h"
#include "x86_64/acpi.h"
#include "local_apic.h"
#include "ioapic.h"
#include "madt.h"


static struct MADTHeader* s_madt;
static struct MADTInformation s_madt_info = (struct MADTInformation){
	.processors_num = 0,
	.ioapics_num = 0,
	.isos_num = 0,
};


void madt_init(struct MADTHeader* madt) {
	s_madt = madt;
	pageframe_reserve_size(madt, madt->header.length);
	void* local_apic_ptr = (void*)(uint64_t)madt->local_apic_address;
	size_t t = 0;
	while(t < madt->header.length - sizeof(struct MADTHeader)) {
		struct MADTRecord* record = (struct MADTRecord*)((uint64_t)madt + sizeof(struct MADTHeader) + t);
		t += record->length;
		switch(record->type) {
			case MADT_TYPE_LOCAL_PROCESSOR: {
				struct MADTLocalProcessor* local_processor = (struct MADTLocalProcessor*)record;
				s_madt_info.processors[s_madt_info.processors_num] = (struct LocalAPICProcessor){
					.local_processor = local_processor
				};
				s_madt_info.processors_num++;
				break;
			}
			case MADT_TYPE_IOAPIC: {
				struct MADTIOAPIC* ioapic = (struct MADTIOAPIC*)record;
				void* ioapic_ptr = (void*)(uint64_t)ioapic->ioapic_address;
				pageframe_reserve(ioapic_ptr, 1);
				paging_identity_map(ioapic_ptr, 1);
				paging_set_writable(ioapic_ptr, 1);
				s_madt_info.ioapics[s_madt_info.ioapics_num] = (struct IOAPIC){
					.ioapic = ioapic
				};
				s_madt_info.ioapics_num++;
				break;
			}
			case MADT_TYPE_INTERRUPT_SOURCE_OVERRIDE: {
				struct MADTInterruptSourceOverride* iso = (struct MADTInterruptSourceOverride*)record;
				s_madt_info.isos[s_madt_info.isos_num] = iso;
				s_madt_info.isos_num++;
				break;
			}
			case MADT_TYPE_NONMASKABLE_INTERRUPT: {
				struct MADTNonMaskableInterrupt* nmi = (struct MADTNonMaskableInterrupt*)record;
				s_madt_info.nmis[s_madt_info.nmis_num] = nmi;
				s_madt_info.nmis_num++;
				log_default("Non-Maskable Interrupt ID: %d\n", nmi->local_apic_id);
				break;
			}
			case MADT_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE: {
				struct MADTLocalAPICAddressOverride* local_apic_override = (struct MADTLocalAPICAddressOverride*)record;
				local_apic_ptr = (void*)local_apic_override->local_apic_address;
				log_options((struct LogOptions){ LOG_TYPE_WARNING }, "Local PIC Address changed");
				break;
			}
		}
	}
	pageframe_reserve(local_apic_ptr, 1);
	paging_identity_map(local_apic_ptr, 1);
	paging_set_writable(local_apic_ptr, 1);
	s_madt_info.local_apic_ptr = local_apic_ptr;
	log_default("Bootstrap processor APIC Address: 0x%x\n", local_apic_ptr);
}

struct MADTHeader* madt_get() {
	return s_madt;
}
struct MADTInformation* madt_get_info() {
	return &s_madt_info;
}