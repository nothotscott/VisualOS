/*
 * File:		setup.c
 * Description:	Setup functions for initializing the kernel
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

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
	/*struct FrameBuffer* frame_buffer = g_interface->frame_buffer;
	pageframe_init(g_interface->mem_map, g_interface->mem_map_size, g_interface->mem_map_descriptor_size);
	pageframe_reserve(frame_buffer->base, NEAREST_PAGE(frame_buffer->size));
	paging_init();
	paging_identity_map(frame_buffer->base, frame_buffer->size);
	paging_load();*/
	//paging_setup_pat();
}

void setup_pit() {
	pit_init();
}

// *** POST BSP INIT *** //

void setup_acpi() {
	/*struct Stivale2StructureRSDP* rsdp_descriptor = (struct Stivale2StructureRSDP*)stivale2_get_structure(g_stivale2_structure, STIVALE2_STRUCTURE_TAG_IDENTIFIER_RSDP);
	struct RSDP2* rsdp = (struct RSDP2*)rsdp_descriptor->rsdp;
	struct SDTHeader* xsdt = (struct SDTHeader*)rsdp->xsdt_base;
	struct MCFGHeader* mcfg = (struct MCFGHeader*)acpi_get_table(xsdt, "MCFG");
	struct MADTHeader* madt = (struct MADTHeader*)acpi_get_table(xsdt, "APIC");
	pci_init(mcfg);
	io_enable_apic();
	madt_init(madt);
	//pci_print();*/
}

void setup_apic() {
	apic_init();
	apic_start_smp();
}

void setup_pre() {
	setup_shell();
	/*debug("Setup shell\n");
	setup_memory();
	debug("Setup memory\n");
	setup_pit();
	debug("Setup PIT\n");*/
}

extern void test_userspace();

void setup_post() {
	setup_acpi();
	debug("Setup ACPI/PCI\n");
	setup_apic();
	debug("Setup APIC/MP\n");
	
	/*
	paging_donate_to_userspace(&test_userspace);
	void* userspace_stack = pageframe_request();
	paging_donate_to_userspace(userspace_stack);
	debug_options((struct DebugOptions){DEBUG_TYPE_INFO, true}, "Entering test_userspace\n");
	syscall_goto_userspace(&test_userspace, userspace_stack + 4096 - 8);
	debug_options((struct DebugOptions){DEBUG_TYPE_INFO, true}, "Back to kernel\n");*/
}