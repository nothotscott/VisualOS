;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		cpu.asm
;; Note: 		The functions should follow Sys V ABI
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN	gdt_init
EXTERN	gdt_load
EXTERN	cpu_get_bsp
EXTERN	cpu_init

%include	"cpu.inc"

;; Function macros

%macro	SET_LOCAL_APIC_ID	1
	push	rbx
	mov		eax, 0x01
	cpuid
	mov		eax, ebx
	pop		rbx
	shr		eax, 24
	and		eax, DWORD 0xff
	mov		BYTE [%1 + CPUContext.local_apic_id], al
%endmacro

%macro	SET_KERNEL_CONTEXT	0
	mov		ecx, CPU_MSR_KERNEL_GS_BASE	
	mov		eax, edi					; low
	shr		rdi, 32
	mov		edx, edi					; high
	wrmsr
%endmacro


;; Global initializors
SECTION	.text

GLOBAL	cpu_init_bsp
cpu_init_bsp:
	call				cpu_get_bsp
	mov					rdi, rax
	jmp					cpu_init_common


GLOBAL	cpu_init_ap
cpu_init_ap:
	jmp	cpu_init_common

cpu_init_common:
	mov					r12, rdi
	; Set context struct
	SET_LOCAL_APIC_ID	rdi
	; Set MSRs
	mov					rdi, r12
	SET_KERNEL_CONTEXT
	; Finish initialization in C
	mov					rdi, r12
	call				cpu_init
	ret