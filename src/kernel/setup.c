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
#include "x86_64/apic/ioapic.h"
#include "x86_64/isr.h"
#include "x86_64/pit.h"
#include "x86_64/acpi.h"
#include "x86_64/pci.h"
#include "x86_64/apic/madt.h"
#include "x86_64/apic/local_apic.h"
#include "x86_64/syscall.h"
#include "scheduler/scheduler.h"


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
	paging_identity_map_size(framebuffer->base, framebuffer_size);
	paging_set_writable_size(framebuffer->base, framebuffer_size);
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
	io_apic_enable();
	madt_init(madt);
	local_apic_init();
	//pci_print();
}

void setup_interrupt_prep() {
	// NOTE interrupts should be cleared at this point
	isr_init();
	pit_init();
	io_pic_disable();
	ioapic_init();
	ioapic_set_from_isrs();
}

// *** POST BSP INIT *** //

void setup_smp() {
	local_apic_start_smp();
	ioapic_entry_set_mask(32, IOAPIC_REDIRECTION_ENTRY_MASK_DISABLE);
}

extern void test_userspace();
extern void test_userspace2();

void setup_tasks() {
	scheduler_init();
	// TODO load up some modules for the scheduler
	scheduler_add_task_default(test_userspace, 1, SCHEDULER_QUEUE_PRIORITY);
	scheduler_add_task_default(test_userspace2, 1, SCHEDULER_QUEUE_PRIORITY);
}

// *** ORDERED SETUP FUNCTIONS *** //

void setup_pre() {
	setup_shell();
	log_default("Setup shell\n");
	setup_memory();
	log_default("Setup memory\n");
	setup_debugging();
	log_default("Setup debugging\n");
	setup_acpi();
	log_default("Setup ACPI/PCI\n");
	setup_interrupt_prep();
	log_default("Setup interrupt preperation\n");
}

void setup_post() {
	setup_smp();
	log_default("Setup SMP\n");
	setup_tasks();
	//log_default("Setup tasks\n");
}