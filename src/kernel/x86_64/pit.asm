;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		kernel.asm
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN	pit_get_ticks_since_boot

%define	PIT_BASE_FREQUENCY	1193182

GLOBAL	pit_sleep
pit_sleep:	; rdi=[miliseconds]
	call	pit_get_ticks_since_boot
	mov		rsi, rax
	.check:
		call	pit_get_ticks_since_boot
		sub		rax, rsi
		mov		rcx, 1000
		mul		rcx
		mov		rdx, 0
		mov		rcx, PIT_BASE_FREQUENCY
		div		rcx
		cmp		rax, rdi
		jge		.exit
		hlt
		jmp		.check
	.exit:
		ret