;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		syscall.asm
;; 
;; Copyright 2021 Scott Maday
;; You should have received a copy of the GNU General Public License along with this program. 
;; If not, see https://www.gnu.org/licenses/gpl-2.0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

EXTERN	gdt_set_ring0_stack
EXTERN	gdt_get_ring0_stack
EXTERN	syshandler_get
EXTERN	sys_exit


GLOBAL	syscall_enable_sce
syscall_enable_sce:
	; Load handler RIP into LSTAR MSR
	mov		rax, syscall_entry
	mov		rdx, rax
	shr		rdx, 0x20
	mov		rcx, 0xc0000082
	wrmsr
	; Enable syscall / sysret instruction
	mov		rcx, 0xc0000080
	rdmsr
	or		eax, 1
	wrmsr
	; Load segments into STAR MSR
	mov		rcx, 0xc0000081
	rdmsr
	mov		edx, 0x00180008
	wrmsr
	ret

GLOBAL	syscall_goto_userspace
syscall_goto_userspace:	; rdi=[code], rsi=[stack]
	mov		rcx, rdi			; This will be used to locate the code in userspace later
	; Quickly save our stack pointer
	mov		rdi, rsp
	call	gdt_set_ring0_stack
	; Enter into userspace
	mov		rsp, rsi			; stack
	mov		r11, 0x0202			; RFLAGS
	o64 sysret					; NASM weirdness, equivalent to sysretq

syscall_entry:
	; Save and switch context
	; TODO switch stacks
	SYSCALL_SAVE
	push	r11
	push	rcx
	; Get syscall handler and call the routine
	mov		rdi, rax
	call	syshandler_get
	mov		rbx, rax		; save the function pointer
	mov		rcx, r10		; syscall's 4th param and gcc's 4th param are the only misaligned parameters
	call	rbx				; call the returned function pointer
	; Restore state-sensitive information and exit
	pop		rcx
	pop		r11
	cmp		rbx, sys_exit
	je		.kernel_exit
	.sysret_exit:
		SYSCALL_RESTORE
		o64	sysret
	.kernel_exit:
		SYSCALL_RESTORE
		call	gdt_get_ring0_stack
		mov		rsp, rax
		ret