/*
 * File:		interrupt.h
 * Description:	Simple x86_64 interrupt data types
 * *****************************************************************************
 * Copyright 2020-2021 Scott Maday
 * Check the LICENSE file that came with this program for licensing terms
 */

#pragma once


struct InterruptStack
{
	uint64_t	rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t	error_code, rip, cs, rflags, rsp, ss;
};