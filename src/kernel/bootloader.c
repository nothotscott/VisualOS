/*
 * File:		bootloader.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "framebuffer.h"
#include "module.h"
#include "bootloader.h"
#include "x86_64/acpi.h"

static struct Framebuffer s_framebuffer;
static struct Module s_module_font;
static struct Module s_module_image;

static struct BootloaderInformation s_bootloader_info = {
	.framebuffer = &s_framebuffer,
	.font = &s_module_font,
	.image = &s_module_image,
	.rsdp = NULL
};


struct BootloaderInformation* bootloader_get_info() {
	return &s_bootloader_info;
}