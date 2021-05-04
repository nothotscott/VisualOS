/*
 * File:		stivale2.h
 * Description:	stivale2 boot protocol headers
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define STIVALE2_STACK_PAGES	1

#define STIVALE2_TAG_ID_HEADER_FRAME_BUFFER	0x3ecc1bc43d0f7971
#define STIVALE2_TAG_ID_HEADER_TERMINAL		0xa85d499b1823be72

#define STIVALE2_TAG_ID_STRUCTURE_COMMANDLINE		0xe5e76a1b4597a781
#define STIVALE2_TAG_ID_STRUCTURE_MEMORYMAP			0x2187f79e8612de07

/// Headers (give to bootloader) ///
struct Stivale2Header {
	uint64_t	entry_point;
	uint64_t	stack;
	uint64_t	flags;
	uint64_t	tags;
} __attribute__((packed));

struct Stivale2HeaderTag {
	uint64_t	identifier;
	uint64_t	next;
} __attribute__((packed));

struct Stivale2HeaderTagFrameBuffer {
	struct Stivale2HeaderTag	tag;
	// If all values are set to 0 then the bootloader will pick the best possible video mode automatically.
	uint16_t					width;
	uint16_t					height;
	uint16_t					bpp;
} __attribute__((packed));

struct Stivale2HeaderTagTerminal {
	struct Stivale2HeaderTag	tag;
	uint64_t					flags;
} __attribute__((packed));

/// Structures (received from bootloader) ///

struct Stivale2Structure {
	char		bootloader_brand[64];
	char		bootloader_version[64];
	uint64_t	tags;
} __attribute__((packed));

struct Stivale2StructureTag {
	uint64_t	identifier;
	uint64_t	next;
} __attribute__((packed));

struct Stivale2StructureTagCommandLine {
	struct Stivale2StructureTag	tag;
	uint64_t					cmdline;
} __attribute__((packed));