;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		apic.asm
;; Description:	Assembly code and data for when the application processor first
;;				receives the STARTUP IPI
;; Note:		The beginning of this file ($$) should be the reference point
;;				for when this code gets memcpy'ed into low memory
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

DEFAULT	ABS

%define	OFFSET_REL(abs)	(abs - $$)

%define	GDT_CODE_SEG	(trampoline_gdt_start.code - trampoline_gdt_start)
%define	GDT_DATA_SEG	(trampoline_gdt_start.data - trampoline_gdt_start)
%define	GDT_SYSTEM_SEG	(trampoline_gdt_start.system - trampoline_gdt_start)

;;; 16-bit code ;;;
BITS	16

GLOBAL	trampoline_code
trampoline_code:
	; The startup code should be the first thing here
	cli
	cld
	; Store the code segment given to us during SIPI
	mov		bx, cs
	; Let the other segments also be the same as the code segment, except
	; for the stack segment. We shouldn't have to use the stack at this stage anyway.
	mov		ds, bx
	mov		es, bx
	mov		fs, bx
	mov		gs, bx
	; Needs base address for linear space
	shl		ebx, 4
	; Inform BSP we're awake
	mov		BYTE [ds:OFFSET_REL(trampoline_data.ap_status)], 1
	; Wait for BSP to give the go-ahead
	.spin:
		cmp	BYTE [ds:OFFSET_REL(trampoline_data.bsp_status)], 0
		je	SHORT .spin
	.continue:
	mov		BYTE [ds:OFFSET_REL(trampoline_data.ap_status)], 0	; reset status
	; Figure out the run-time address of the gdt and load it
	mov		eax, ebx
	add		eax, OFFSET_REL(trampoline_gdt_start)
	mov		DWORD [ds:OFFSET_REL(trampoline_gdt_descriptor + 2)], eax
	lgdt	[ds:OFFSET_REL(trampoline_gdt_descriptor)]
	; Enable protected mode
	mov		eax, cr0
	or		eax, 1
	mov		cr0, eax
	; Jump into protected mode
	mov		eax, ebx
	add		eax, OFFSET_REL(trampoline_protected)
	mov		DWORD [OFFSET_REL(trampoline_spring)], eax
	jmp		FAR DWORD [OFFSET_REL(trampoline_spring)]

;;; 32-bit code ;;;
BITS	32
trampoline_protected:
	mov		ax, GDT_DATA_SEG
	mov		ds, ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax
	; Enable PAE
	mov		eax, cr4
	or		eax, 1 << 5
	mov		cr4, eax
	; Enable long mode
	mov		ecx, 0xC0000080
	rdmsr
	or		eax, 0x0900		; enable long mode, no executable
	wrmsr
	; Setup PML4
	mov		eax, DWORD [ebx + OFFSET_REL(trampoline_data.pagetable_l4)]
	mov		cr3, eax
	; Enable paging
	mov		eax, cr0
	or		eax, 0x80010000	; paging, write protect
	mov		cr0, eax
	; Jump into long mode, reusing the old spring
	lea		eax, [ebx + OFFSET_REL(trampoline_longmode)]
	mov		DWORD [ebx + OFFSET_REL(trampoline_spring)], eax
	mov		WORD [ebx + OFFSET_REL(trampoline_spring) + 4], GDT_SYSTEM_SEG
	jmp		FAR DWORD [ebx + OFFSET_REL(trampoline_spring)]

;;; 64-bit code ;;;
BITS	64
trampoline_longmode:
	mov		ax, 0
	mov		ds, ax
	mov		es, ax
	mov		fs, ax
	mov		gs, ax
	; Enable APIC
	mov		ecx, 0x1b
	rdmsr
	or		eax, 0x800
	wrmsr
	; Get our stack
	mov		rsp, QWORD [ebx + OFFSET_REL(trampoline_data.stack_ptr)]
	mov		rbp, 0					; null base pointer for stack unwinding
	; Continue BSP initialization
	mov		rdi, QWORD [ebx + OFFSET_REL(trampoline_data.ap_context)]
	push	rbx
	call	QWORD [ebx + OFFSET_REL(trampoline_data.cpu_init_ap)]
	pop		rbx
	; Finish up and begin scheduling work
	mov		BYTE [ebx + OFFSET_REL(trampoline_data.ap_status)], 2
	jmp		QWORD [ebx + OFFSET_REL(trampoline_data.scheduler_entry)]

;;; Data ;;;
BITS	64
ALIGN	64

GLOBAL	trampoline_data
trampoline_data:
	; The AP communication structure should be the first thing here
	; NOTE: these should reflect the initial values when this gets memcpy'ed
	.ap_status:			db	0
	.bsp_status:		db	0
	.pagetable_l4:		dd	0
	.stack_ptr:			dq	0
	.stack_size:		dd	0
	.ap_context:		dq	0
	.cpu_init_ap:		dq	0
	.scheduler_entry:	dq	0


ALIGN	16
trampoline_gdt_start:
	dq	0
	.code:
	dw	0xffff
	dw	0x0000
	db	0x00
	db	0x9a
	db	0xcf
	db	0x00
	.data:
	dw	0xffff
	dw	0x0000
	db	0x00
	db	0x92
	db	0xcf
	db	0x00
	.system:
	dw	0x0000
	dw	0x0000
	db	0x00
	db	0x98
	db	0xa0
	db	0x00
trampoline_gdt_end:

ALIGN	4
trampoline_gdt_descriptor:
	dw	trampoline_gdt_end - trampoline_gdt_start - 1
	dd	00	; Figure out at runtime

; Used for far jumping
ALIGN	4
trampoline_spring:
	dd	0
	dw	GDT_CODE_SEG

TIMES		4096-($-$$)	db	0