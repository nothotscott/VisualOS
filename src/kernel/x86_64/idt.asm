;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		idt.asm
;; Description:	x86_64 Interrupt Descriptor Table functions
;; 
;; Copyright 2020-2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


EXTERN	g_idt_descriptor
EXTERN	g_isr_handlers

%macro	ISR_DEFINE	2
	GLOBAL	isr%1
	isr%1:
		cli
		%if	%2 == 0
			push	QWORD 0				; push for so the stack size is consistent
		%endif
		PUSH_REG
		mov		rax, [g_isr_handlers + 8 * %1]
		cmp		rax, 0					; hopefully, a valid function doesn't point to 0
		jz		%%.finish
		mov		rdi, rsp				; we'll create a struct out of the stack pointer
		mov		rsi, %1
		call	rax
		%%.finish:
			POP_REG
			%if	%2 == 0
				add		rsp, 8
			%endif
			sti
			iretq
%endmacro

;;; Begin code ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION .text

GLOBAL	idt_load
idt_load:
	cli
	lidt	[g_idt_descriptor]
	sti
	ret


; Exceptions
ISR_DEFINE	0, 0	; Divide By Zero Exception
ISR_DEFINE	1, 0	; Debug Exception
ISR_DEFINE	2, 0	; Non Maskable Interrupt Exception
ISR_DEFINE	3, 0	; Breakpoint Exception
ISR_DEFINE	4, 0	; Into Detected Overflow Exception
ISR_DEFINE	5, 0	; Out of Bounds Exception
ISR_DEFINE	6, 0	; Invalid Opcode Exception
ISR_DEFINE	7, 0	; No Coprocessor Exception
ISR_DEFINE	8, 1	; Double Fault Exception
ISR_DEFINE	9, 0	; Coprocessor Segment Overrun Exception
ISR_DEFINE	10, 1	; Segment Not Present Exception
ISR_DEFINE	11, 1	; Segment Not Present Exception
ISR_DEFINE	12, 1	; Stack Fault Exception
ISR_DEFINE	13, 1	; General Protection Fault Exception
ISR_DEFINE	14, 1	; Page Fault Exception
ISR_DEFINE	16, 0	; Coprocessor Fault Exception
ISR_DEFINE	17, 1	; Alignment Check Exception (486+)
ISR_DEFINE	18, 0	; Machine Check Exception (Pentium/586+)
ISR_DEFINE	19, 0	; SIMD Floating-Point Exception
ISR_DEFINE	20, 0	; SIMD Floating-Point Exception
ISR_DEFINE	30, 1	; Security Exception
; Interrupt requests
ISR_DEFINE	32, 0	; PIT
ISR_DEFINE	33, 0	; Keyboard Interrupt
ISR_DEFINE	35, 0	; COM2
ISR_DEFINE	36, 0	; COM1
ISR_DEFINE	37, 0	; LPT2
ISR_DEFINE	39, 0	; LPT1