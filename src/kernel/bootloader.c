/*
 * File:		bootloader.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "framebuffer.h"
#include "module.h"
#include "memory/memory.h"
#include "x86_64/acpi.h"
#include "bootloader.h"

static struct Framebuffer s_framebuffer;
static struct MemoryMap s_memorymap;
static struct Module s_module_font;
static struct Module s_module_image;

static struct BootloaderInformation s_bootloader_info = {
	.framebuffer = &s_framebuffer,
	.memorymap = &s_memorymap,
	.rsdp = NULL,
	.font = &s_module_font,
	.image = &s_module_image,
};


struct BootloaderInformation* bootloader_get_info() {
	return &s_bootloader_info;
}