/*
 * File:		kernel.h
 * Description:	Kernel-specific and auto-included definitions
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#ifndef KERNEL
#define KERNEL
#endif

#define RFLAGS	0x0202

// Rounds up [n] to the nearest [multiple]
#define ROUND_UP(n, multiple)	((n) % (multiple) == 0 ? (n) : (n) - (n) % (multiple) + (multiple))

// Returns [n]'s [bit] set to [value]
#define SET_BIT(n, bit, value)	((n) ^ (-(value) ^ (n)) & 1 << (bit))

// Returns [n]'s value of [bit]
#define GET_BIT(n, bit)			((1 << (bit) & (n)) > 0)

// Returns the physical address of the kernel [virtual_address] from link time
#define KERNEL_PHYSICAL_ADDRESS(virtual_address)	((void*)((unsigned long long)(virtual_address) - ((unsigned long long)&_virtual_base)))

// From kernel.ld linker script
extern void** _virtual_base;

extern void** _kernel_start;
extern void** _kernel_end;

extern void** _kernel_writable_start;
extern void** _kernel_writable_end;