/*
 * File:		bootloader.h
 * Desciption:	Interface for bootloader and kernel coupling
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define PSF1_MAGIC0	0x36
#define PSF1_MAGIC1	0x04
#define PSF1_WIDTH	8

struct FrameBuffer {
	void*				base_ptr;
	unsigned long long	size;		// size of buffer
	unsigned int		width;
	unsigned int		height;
	unsigned int		ppsl;		// pixels per scanline
};

struct PSF1Header {
	unsigned char	magic[2];	// identifier bytes
	unsigned char	mode;
	unsigned char	charsize;	// how large the characters are
};

struct PSF1Font {
	struct PSF1Header*	header_ptr;
	void*				glyph_buffer;
};

struct UefiKernelInterface {
	struct FrameBuffer*	frame_buffer_ptr;
	struct PSF1Font*	font_ptr;
};