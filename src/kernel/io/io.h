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
extern void outb(ushort_t port, byte_t value);

// Returns value on the processor bus on port
extern byte_t inb(ushort_t port);

// Send byte_t onto port 0x80 to signal a checkpoint
extern void io_wait();