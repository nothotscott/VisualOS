/*
 * File:		shell.h
 * Desciption:	Provide a temporary, full resoluton, text interface 
 * Isn't it ironic to have a shell in an OS named VisualOS?
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

struct Point {
	uint	x;
	uint	y;
};

// Sets the cursor to the x and y
void set_cursor(uint, uint);

// Puts a character at the location in the FrameBuffer using the font
void put_char(struct FrameBuffer*, struct PSF1Font*, char, uint, uint, uint);

// Prints a string at the cursor in the FrameBuffer using the font
void print(struct FrameBuffer*, struct PSF1Font*, char*, uint);

// Draws image img on the FrameBuffer
void draw_tga(struct FrameBuffer*, struct TGAImage*, uint, uint);