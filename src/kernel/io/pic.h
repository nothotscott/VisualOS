/*
 * File:		pic.h
 * Description:	Programmable Interface Controller chip functions
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

#define IO_PIC1_COMMAND	0x20
#define IO_PIC1_DATA	0x21
#define IO_PIC2_COMMAND	0xa0
#define IO_PIC2_DATA	0xa1

#define IO_ICW1_INIT	0x10
#define IO_ICW1_ICW4	0x01
#define IO_ICW4_8086	0x01

#define IO_PIC_EOI		0x20


// Sets the interrupt mask register
void pic_mask();

// Remap to receive proper interrupt vectors
void pic_remap();
