/*
 * File:		io.h
 * Desciption:	x86_64 processor IO functions
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define IO_PIC1_COMMAND	0x20
#define IO_PIC1_DATA	0x21
#define IO_PIC2_COMMAND	0xa0
#define IO_PIC2_DATA	0xa1

#define IO_ICW1_INIT	0x10
#define IO_ICW1_ICW4	0x01
#define IO_ICW4_8086	0x01

#define IO_CHIPSET_ADDRESS_REGISTER	0x22
#define IO_CHIPSET_DATA_REGISTER	0x23
#define IO_IMCR_REGISTER_ADDRESS	0x70
#define IO_IMCR_8259_DIRECT			0x00
#define IO_IMCR_VIA_APIC			0x01

#define IO_PIC_EOI		0x20

// Disables/enables interrupts
void io_interrupts_disable();
void io_interrupts_enable();

// Enables APIC
void io_enable_apic();

// Sends value out onto the processor bus on port
extern void outb(uint16_t port, uint8_t value);
extern void outw(uint16_t port, uint16_t value);
extern void outl(uint16_t port, uint32_t value);

// Returns value on the processor bus on port
extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern uint32_t inl(uint16_t port);

// Send byte onto port 0x80 to signal a checkpoint
extern void io_wait();

// Overrides the PIC in favor of the APIC
void io_pic_override();

// *** DEPRECATED PIC FUNCTIONS *** //

// Sets the interrupt mask register
void io_pic_mask() __attribute__((deprecated));

// Remap to receive proper interrupt vectors
void io_pic_remap() __attribute__((deprecated));

// Ends either the master or slave PIC
void io_pic_end_master() __attribute__((deprecated));
void io_pic_end_slave() __attribute__((deprecated));