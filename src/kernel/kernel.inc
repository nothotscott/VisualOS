;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		kernel.inc
;; Description:	Kernel level macros for nasm
;; 
;; Copyright 2020-2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

DEFAULT	REL

; Saves/restores all general purpose registers
%macro	PUSH_REG	0
	push	r15
	push	r14
	push	r13
	push	r12
	push	r11
	push	r10
	push	r9
	push	r8
	push	rbp
	push	rdi
	push	rsi
	push	rdx
	push	rcx
	push	rbx
	push	rax
%endmacro
%macro	POP_REG		0
	pop		rax
	pop		rbx
	pop		rcx
	pop		rdx
	pop		rsi
	pop		rdi
	pop		rbp
	pop		r8
	pop		r9
	pop		r10
	pop		r11
	pop		r12
	pop		r13
	pop		r14
	pop		r15
%endmacro