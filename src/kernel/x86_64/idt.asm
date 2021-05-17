;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		idt.asm
;; Description:	x86_64 Interrupt Descriptor Table functions
;; 
;; Copyright 2020-2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


SECTION .text


GLOBAL	idt_load
idt_load:	; rdi = [idt_descriptor], rsi = [enable_interrupts]
	cli
	lidt	[rdi]
	cmp		rsi, 0
	jz		.exit
	sti
	.exit:
		ret