/*
 * File:		cpu.c
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#include "cpu.h"


static struct CPUContext s_cpu_bsp;


struct CPUContext* cpu_get_bsp() {
	return &s_cpu_bsp;
}