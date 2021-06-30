/*
 * File:		bootloader.h
 * Description:	Interface for abstract bootloader and kernel coupling
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#include "framebuffer.h"
#include "module.h"
#include "x86_64/acpi.h"

struct BootloaderInformation {
	struct Framebuffer*	framebuffer;
	struct MemoryMap*	memorymap;
	struct RSDP2*		rsdp;
	void*				kernel_header;
	long				kernel_offset;
	struct Module*		font;
	struct Module*		image;
	struct Module*		test_module;
};


// Gets the bootloader information
struct BootloaderInformation* bootloader_get_info();