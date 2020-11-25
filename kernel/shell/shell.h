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

// Puts a character at the location in the FrameBuffer
void put_char(struct FrameBuffer*, struct PSF1Font*, char, uint, uint, uint);

void print(struct FrameBuffer*, struct PSF1Font*, char*, uint);