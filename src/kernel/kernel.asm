;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		kernel.asm
;; Note: 		Defines the entry points for the kernel
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN	kernel_common
EXTERN	stivale2_init
EXTERN	scheduler_entry

%macro ENTRY_SETUP	0
	cli
	mov		rbp, 0
%endmacro

%macro KERNEL_BEGIN	0
	call	kernel_common
	mov		rdi, 1
	jmp		scheduler_entry
%endmacro


GLOBAL	kernel_start_default
kernel_start_default:
	ENTRY_SETUP
	KERNEL_BEGIN

GLOBAL	kernel_start_stivale2
kernel_start_stivale2:
	ENTRY_SETUP
	call	stivale2_init
	KERNEL_BEGIN
