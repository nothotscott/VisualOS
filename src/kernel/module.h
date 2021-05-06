/*
 * File:		module.h
 * Description:	Describes modules loaded by the bootloader, abstracted away
 * 				from any particular boot protocol
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define MODULE_SIGNATURE_FONT	"font"
#define MODULE_SIGNATURE_IMAGE	"image"

#define MODULE_PSF1_WIDTH	8

// *** Miscellaneous datatypes *** //

struct Module {
	void*	header;
	void*	body;
	size_t	size;	// size of, in bytes, starting from the header
};

// *** Module Headers *** //

struct ModulePSF1Header {
	uint8_t	magic[2];
	uint8_t	mode;
	uint8_t	charsize;	// how large the characters are
} __attribute__((packed));

struct ModuleTGAHeader {
	uint8_t		id_length;
	uint8_t		color_map_type;
	uint8_t		image_type;
	uint16_t	color_map_index;
	uint16_t	color_map_length;
	uint8_t		depth;
	uint16_t	xorg;				// x origin
	uint16_t	yorg;				// y origin
	uint16_t	width;
	uint16_t	height;
	uint8_t		bbp;				// bits per pixel
	uint8_t		descriptor;
} __attribute__((packed));