/*
 * File:		cpu.h
 * Description:	Functions that should initialize either the BSP or APs
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


struct CPUContext {
	uint8_t		local_apic_id;
} __attribute__((packed));


// Initializes the bootstrap processor
void cpu_init_bsp();
// Initializes the application processor
void cpu_init_ap(struct CPUContext* cpu_context);

// Getters
struct CPUContext* cpu_get_bsp();