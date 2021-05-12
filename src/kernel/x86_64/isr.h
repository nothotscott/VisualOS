/*
 * File:		isr.h
 * Description:	Interrupt service routines
 * *****************************************************************************
 * Copyright 2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once

#define ISR_IRQ_START	32
#define	ISR_MAX			256


struct InterruptStack
{
	uint64_t	rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t	error_code, rip, cs, rflags, rsp, ss;
};

// Global ISR handler table
extern void (*g_isr_handlers[ISR_MAX])(struct InterruptStack*, size_t);

// Registers all default isr handlers in g_isr_handlers
void isr_init();

// Adds isr [handler] for interrupt [num] in g_isr_handlers
void isr_register_handler(size_t num, void (*handler)(struct InterruptStack*, size_t));

// Returns true the isr for [num] has been registered in the handler table
bool isr_exists(size_t num);


// Interrupt service routines defined in isr.asm
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr30();

extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();