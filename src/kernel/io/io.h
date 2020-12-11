/*
 * File:		io.h
 * Desciption:	x86_64 processor IO functions
 * *****************************************************************************
 * Copyright 2020 Scott Maday
 * You should have received a copy of the GNU General Public License along with this program. 
 * If not, see https://www.gnu.org/licenses/gpl-2.0
 */

#pragma once

// Sends value out onto the processor bus on port
extern void outb(ushort port, byte value);

// Returns value on the processor bus on port
extern byte inb(ushort port);