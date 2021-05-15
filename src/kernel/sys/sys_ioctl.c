/*
 * File:		sys_ioctl.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#define __NEED_struct_winsize
#include <bits/alltypes.h>

#include <bits/ioctl.h>
#include "log.h"
#include "framebuffer.h"
#include "module.h"
#include "bootloader.h"

uint64_t sys_ioctl(int fd, int cmd, long arg) {
	switch(cmd) {
		case TIOCGWINSZ: {
			struct Framebuffer* framebuffer = bootloader_get_info()->framebuffer;
			struct ModulePSF1Header* font = (struct ModulePSF1Header*)bootloader_get_info()->image->header;
			*(struct winsize*)arg = (struct winsize){
				.ws_row = framebuffer->width,
				.ws_col = framebuffer->height,
				.ws_xpixel = MODULE_PSF1_WIDTH,
				.ws_ypixel = font->charsize,
			};
			return false;
		}
	}
}