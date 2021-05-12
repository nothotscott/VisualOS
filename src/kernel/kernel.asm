;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		kernel.asm
;; Note: 		Defines the entry points for the kernel
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN	kernel_common
EXTERN	stivale2_init

%macro ENTRY_SETUP	0
	cli
	mov		rbp, 0
%endmacro

%macro GOTO_KERNEL_COMMON	0
	push	QWORD 0
	jmp		kernel_common
%endmacro


GLOBAL	kernel_start_default
kernel_start_default:
	ENTRY_SETUP
	GOTO_KERNEL_COMMON

GLOBAL	kernel_start_stivale2
kernel_start_stivale2:
	ENTRY_SETUP
	call	stivale2_init
	GOTO_KERNEL_COMMON
