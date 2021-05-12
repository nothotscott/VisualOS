;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		isr.asm
;; Description:	Entry point for interrupt service routines
;; 
;; Copyright 2020-2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


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
%macro	ISR_IRQ_DEFINE	1
	%assign		ISR_NUM	32 + %1
	ISR_DEFINE	ISR_NUM, 0
%endmacro

SECTION .text

; Exceptions
;ISR_DEFINE	0, 0	; Divide By Zero Exception
;ISR_DEFINE	1, 0	; Debug Exception
;ISR_DEFINE	2, 0	; Non Maskable Interrupt Exception
;ISR_DEFINE	3, 0	; Breakpoint Exception
;ISR_DEFINE	4, 0	; Into Detected Overflow Exception
;ISR_DEFINE	5, 0	; Out of Bounds Exception
;ISR_DEFINE	6, 0	; Invalid Opcode Exception
;ISR_DEFINE	7, 0	; No Coprocessor Exception
ISR_DEFINE	8, 1	; Double Fault Exception
;ISR_DEFINE	9, 0	; Coprocessor Segment Overrun Exception
;ISR_DEFINE	10, 1	; Invalid Task State Segment
;ISR_DEFINE	11, 1	; Segment Not Present Exception
;ISR_DEFINE	12, 1	; Stack Fault Exception
ISR_DEFINE	13, 1	; General Protection Fault Exception
ISR_DEFINE	14, 1	; Page Fault Exception
;ISR_DEFINE	16, 0	; Coprocessor Fault Exception
;ISR_DEFINE	17, 1	; Alignment Check Exception (486+)
;ISR_DEFINE	18, 0	; Machine Check Exception (Pentium/586+)
;ISR_DEFINE	19, 0	; SIMD Floating-Point Exception
;ISR_DEFINE	20, 0	; Virtualization Exception
;ISR_DEFINE	30, 1	; Security Exception
; Interrupt requests
ISR_IRQ_DEFINE	0	; PIT
ISR_IRQ_DEFINE	1	; Keyboard Interrupt
;ISR_IRQ_DEFINE	2	; Cascade
ISR_IRQ_DEFINE	3	; COM2
ISR_IRQ_DEFINE	4	; COM1
ISR_IRQ_DEFINE	5	; LPT2
ISR_IRQ_DEFINE	6	; Floppy disk
ISR_IRQ_DEFINE	7	; LPT (spurious interrupt)
ISR_IRQ_DEFINE	8	; CMOS
ISR_IRQ_DEFINE	9	; Peripherals 1 (legacy SCSI/NIC)
ISR_IRQ_DEFINE	10	; Peripherals 2 (SCSI/NIC)
ISR_IRQ_DEFINE	11	; Peripherals 3 (SCSI/NIC)
ISR_IRQ_DEFINE	12	; PS2 Mouse
ISR_IRQ_DEFINE	13	; Coprocessor/interprocessor
ISR_IRQ_DEFINE	14	; Primary ATA Hard disk
ISR_IRQ_DEFINE	15	; Secondary ATA Hard disk