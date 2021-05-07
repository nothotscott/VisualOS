/*
 * File:		setup.c
 * Description:	Setup functions for initializing the kernel
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "bootloader.h"
#include "framebuffer.h"
#include "module.h"
#include "shell/shell.h"
#include "shell/text.h"
#include "debug/debug.h"
#include "memory/pageframe.h"
#include "memory/paging.h"
#include "memory/paging.h"
#include "x86_64/io.h"
#include "x86_64/pit.h"
#include "x86_64/acpi.h"
#include "x86_64/pci.h"
#include "x86_64/apic/madt.h"
#include "x86_64/apic/apic.h"
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
	pageframe_reserve(framebuffer->base, NEAREST_PAGE(framebuffer_size));
	pageframe_lock((void*)APIC_TRAMPOLINE_TARGET, APIC_TRAMPOLINE_TARGET_SIZE);	// Lock the APIC trampoline code
	// TODO reserve framebuffer shadow buffer here
	paging_init();
	paging_identity_map(framebuffer->base, framebuffer_size);
	//paging_setup_pat();
}

void setup_pit() {
	pit_init();
}

// *** POST BSP INIT *** //

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

void setup_apic() {
	apic_init();
	apic_start_smp();
}

void setup_pre() {
	setup_shell();
	debug("Setup shell\n");
	setup_memory();
	debug("Setup memory\n");
	setup_pit();
	debug("Setup PIT\n");
}

extern void test_userspace();

void setup_post() {
	setup_acpi();
	debug("Setup ACPI/PCI\n");
	setup_apic();
	debug("Setup APIC/MP\n");
}