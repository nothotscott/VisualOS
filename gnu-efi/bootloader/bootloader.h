/*
 * File:		bootloader.h
 * Desciption:	Interface for bootloader and kernel coupling
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

struct FrameBuffer {
	void*				base_ptr;
	unsigned long long	size;		// size of buffer
	unsigned int		width;
	unsigned int		height;
	unsigned int		ppsl;		// pixels per scanline
};

struct UefiKernelInterface {
	struct FrameBuffer*	frame_buffer_ptr;
	void* (*malloc_ptr)(unsigned long int);
	void (*mfree_ptr)(void*);
	void* (*load_file_ptr)(void*, unsigned short int*);
};