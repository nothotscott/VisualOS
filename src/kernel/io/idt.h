/*
 * File:		idt.h
 * Description:	Provides functions for the Interrupt Descriptor Table
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once


struct IDT {
	ushort	offset_low;
	ushort	selector;
	byte	ist;
	byte	types_attr;
	ushort	offset_mid;
	uint	offset_high;
	uint	zero;
} __attribute__((__packed__));

struct IDTDescriptor {
	ushort		size;	// max size of 64 bit idt
	struct IDT*	idt;	// where the IDT is
} __attribute__((__packed__));


extern void idt_load(struct IDTDescriptor* idt);