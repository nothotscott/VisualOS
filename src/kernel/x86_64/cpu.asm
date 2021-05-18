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
	mov		QWORD [rdi + CPUContext.stack_kernel], rsi
	jmp		cpu_init_common

cpu_init_common:	; rdi=[cpu_context]
	mov		r12, rdi
	; Set some of the context struct
	call	cpuid_get_local_apic_id
	mov		BYTE [rdi + CPUContext.local_apic_id], al
	; Set gs/fs base MSRs
	mov		rax, cr4
	or		rax, 0x10000
	mov		cr4, rax
	mov		rdi, r12
	mov		ecx, CPU_MSR_KERNELGSBASE	
	mov		eax, edi					; low
	shr		rdi, 32
	mov		edx, edi					; high
	wrmsr
	; Finish initialization in C
	mov		rdi, r12
	call	cpu_init
	pop		rbp
	ret


GLOBAL	cpu_enable_features
cpu_enable_features:
	push	rbp
	mov		rbp, rsp
	push	rbx
	; Enable Coprocessors (FPU and SSE)
	mov		rax, cr0
	and		rax, 0xfffffffffffffffb	; disable FPU emulation
	or		rax, 0x22				; enable monitoring coprocessor and numeric error
	mov		cr0, rax
	mov		rax, cr4
	; TODO enable XSAVE here
	or		rax, 0x0406b0			; enable OSFXSR, OSXMMEXCPT and others
	mov		cr4, rax	
	fninit
	; Finish
	pop		rbx
	pop		rbp
	ret