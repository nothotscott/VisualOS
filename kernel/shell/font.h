/*
 * File:		font.h
 * Desciption:	Uses UEFI interface to load a font
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */


#pragma once

#define PSF1_MAGIC0	0x36
#define PSF1_MAGIC1	0x04

struct PSF1Header {
	byte	magic[2];	// identifier bytes
	byte	mode;
	byte	charsize;	// how large the characters are
};

struct PSF1Font {
	struct PSF1Header*	header_ptr;
	void*				glyph_buffer;
};

struct PSF1Font* font_load_psf1(void* directory, ushort* path, void* (*)(void*, ushort*), void* (*)(size_t));