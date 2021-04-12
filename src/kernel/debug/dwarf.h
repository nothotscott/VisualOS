/*
 * File:		dwarf.h
 * Description:	Gives the kernel awareness of its own debug symbols
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


// From kernel.ld linker script
extern void** _debug_start;
extern void** _debug_end;