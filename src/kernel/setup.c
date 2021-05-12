/*
 * File:		setup.c
 * Description:	Setup functions for initializing the kernel
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "bootloader.h"
#include "log.h"
#include "framebuffer.h"
#include "module.h"
#include "shell/shell.h"
#include "shell/text.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "memory/paging.h"
#include "memory/paging.h"
#include "symbols/symbol.h"
#include "x86_64/io.h"
#include "x86_64/isr.h"
#include "x86_64/pit.h"
#include "x86_64/acpi.h"
#include "x86_64/pci.h"
#include "x86_64/apic/madt.h"
#include "x86_64/apic/local_apic.h"
#include "x86_64/apic/ioapic.h"
#include "x86_64/syscall.h"


// *** PRE BSP INIT *** //

void setup_shell() {
	shell_clear();
	shell_init();
}

void setup_memory() {
	struct Framebuffer* framebuffer = bootloader_get_info()->framebuffer;
	size_t framebuffer_size = framebuffer->pitch * framebuffer->height;
	pageframe_init();
	pageframe_reserve_size(framebuffer->base, framebuffer_size);
	pageframe_lock((void*)LOCAL_APIC_TRAMPOLINE_TARGET, LOCAL_APIC_TRAMPOLINE_TARGET_SIZE);	// Lock the APIC trampoline code
	// TODO reserve framebuffer shadow buffer here
	paging_init();
	paging_load();
	paging_identity_map(framebuffer->base, framebuffer_size);
	//paging_setup_pat();
}

void setup_debugging() {
	symbol_init();
}

void setup_acpi() {
	struct RSDP2* rsdp = (struct RSDP2*)bootloader_get_info()->rsdp;
	struct SDTHeader* xsdt = (struct SDTHeader*)rsdp->xsdt_base;
	struct MCFGHeader* mcfg = (struct MCFGHeader*)acpi_get_table(xsdt, "MCFG");
	struct MADTHeader* madt = (struct MADTHeader*)acpi_get_table(xsdt, "APIC");
	pci_init(mcfg);
	io_enable_apic();
	madt_init(madt);
	//pci_print();
}

void setup_interrupt_prep() {
	// NOTE interrupts should be cleared at this point
	isr_init();
	pit_init();
	io_enable_apic();
	ioapic_init();
	ioapic_set_from_isrs();
}


// *** POST BSP INIT *** //



void setup_local_apic() {
	local_apic_init();
	local_apic_start_smp();
}

// *** ORDERED SETUP FUNCTIONS *** //

void setup_pre() {
	setup_shell();
	log("Setup shell\n");
	setup_memory();
	log("Setup memory\n");
	setup_debugging();
	log("Setup debugging\n");
	setup_acpi();
	log("Setup ACPI/PCI\n");
	setup_interrupt_prep();
	log("Setup interrupt preperation\n");
}

void setup_post() {
	//setup_local_apic();
	//log("Setup Local APIC/SMP\n");
}