;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		cpu.asm
;; Note: 		The functions should follow Sys V ABI
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN	cpu_get_bsp
EXTERN	cpu_init
EXTERN	cpuid_get_local_apic_id

%include	"cpu.inc"

;; Global initializors
SECTION	.text

GLOBAL	cpu_init_bsp
cpu_init_bsp:
	push	rbp
	mov		rbp, rsp
	call	cpu_get_bsp
	mov		rdi, rax
	jmp		cpu_init_common


GLOBAL	cpu_init_ap
cpu_init_ap:
	push	rbp
	mov		rbp, rsp
	jmp		cpu_init_common

cpu_init_common:
	mov		r12, rdi
	; Set context struct
	;SET_LOCAL_APIC_ID	rdi
	call	cpuid_get_local_apic_id
	mov		BYTE [rdi + CPUContext.local_apic_id], al
	; Set MSRs
	mov		ecx, CPU_MSR_KERNEL_GS_BASE	
	mov		eax, edi					; low
	shr		rdi, 32
	mov		edx, edi					; high
	wrmsr
	; Finish initialization in C
	mov		rdi, r12
	call	cpu_init
	pop		rbp
	ret