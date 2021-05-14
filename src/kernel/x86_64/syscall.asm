;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		syscall.asm
;; 
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "cpu.inc"

EXTERN	gdt_set_tss_ring
EXTERN	syshandler_get
EXTERN	sys_exit

%define SYSCALL_EXIT	60

%macro	SYSCALL_SAVE	0
	push	r15
	push	r14
	push	r13
	push	r12
	push	r10
	push	r9
	push	r8
	push	rbp
	push	rdi
	push	rsi
	push	rdx
	push	rbx
%endmacro
%macro	SYSCALL_RESTORE	0
	pop	rbx
	pop	rdx
	pop	rsi
	pop	rdi
	pop	rbp
	pop	r8
	pop	r9
	pop	r10
	pop	r12
	pop	r13
	pop	r14
	pop	r15
%endmacro


GLOBAL	syscall_enable
syscall_enable:
	; Load handler RIP into LSTAR MSR
	mov		rax, syscall_entry
	mov		rdx, rax
	shr		rdx, 0x20
	mov		rcx, 0xc0000082
	wrmsr
	; Enable syscall / sysret instruction
	mov		rcx, 0xc0000080
	rdmsr
	or		rax, 1
	wrmsr
	; Load segments into STAR MSR
	mov		rcx, 0xc0000081
	rdmsr
	mov		edx, 0x00180008
	wrmsr
	ret

GLOBAL	syscall_goto_userspace
syscall_goto_userspace:	; rdi=[code], rsi=[stack]
	cli
	; Save kernel stack
	swapgs											; load in the CPU context
	mov		QWORD [gs:CPUContext.stack_kernel], rsp	; save the kernel stack
	push	rsi										; save the soon-to-be userspace stack
	push 	rdi										; save the soon-to-be userspace code
	mov		rdi, [gs:CPUContext.gdt_block]
	mov		rsi, 0
	mov		rdx, [gs:CPUContext.stack_kernel]
	call	gdt_set_tss_ring
	swapgs											; restore GS state
	; Enter into userspace
	pop		rcx										; (formerly rdi on the stack) locate the code in userspace
	pop		rsp										; (formerly rsi on the stack) userspace stack
	mov		r11, 0x0202								; RFLAGS
	sti												; assume interrupts used to be enabled
	o64 sysret										; NASM weirdness, equivalent to sysretq

syscall_entry:
	; Save and switch context
	swapgs												; load in the CPU context from the kernel GS
	mov		QWORD [gs:CPUContext.stack_userspace], rsp	; save userspace stack
	mov		rsp, [gs:CPUContext.stack_kernel]			; load kernel stack
	SYSCALL_SAVE
	push	r11
	push	rcx
	; Get syscall handler and call the routine
	mov		rdi, rax
	call	syshandler_get
	mov		rbx, rax									; save the function pointer
	mov		rcx, r10									; syscall's 4th param and System V abi's 4th param are the only misaligned parameters
	call	rbx											; call the returned function pointer
	; Restore state-sensitive information and exit
	pop		rcx
	pop		r11
	lea		rdx, [REL sys_exit]
	cmp		rbx, rdx
	je		.kernel_exit
	.sysret_exit:
		SYSCALL_RESTORE
		mov		rsp, [gs:CPUContext.stack_userspace]	; load userspace stack (no need to save the kernel stack)
		swapgs											; restore userspace GS
		o64	sysret
	.kernel_exit:
		SYSCALL_RESTORE
		swapgs
		ret