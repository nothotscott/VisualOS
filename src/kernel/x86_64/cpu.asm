;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		apic.asm
;; Note: 		The functions should follow Sys V ABI
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN	cpu_get_bsp

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
	mov					r12, rdi
	SET_LOCAL_APIC_ID	rdi
	mov					rdi, r12
	SET_KERNEL_CONTEXT
	mov					rdi, r12
	ret


GLOBAL	cpu_init_ap
cpu_init_ap:
	mov					r12, rdi
	SET_LOCAL_APIC_ID	rdi
	mov					rdi, r12
	SET_KERNEL_CONTEXT
	mov					rdi, r12
	ret