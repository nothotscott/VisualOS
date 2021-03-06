;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		gdt.asm
;; Copyright 2020-2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include	"scheduler.inc"

EXTERN	local_apic_start_lints

SECTION	.data

s_entry_lock:	db	0

SECTION	.text

GLOBAL	scheduler_entry
scheduler_entry:	; rdi=[should_unlock]
	cli
	mov		rbp, 0
	push	rdi
	mov		rdi, 0
	WRITE_GSBASE
	call	local_apic_start_lints
	pop		rdi
	; Unlock if we have the key, else wait
	cmp		rdi, 0
	je		.spin_wait
	.unlock:
		mov		BYTE [s_entry_lock], 1
		jmp		.release
	.spin_wait:
		pause
		cmp		BYTE [s_entry_lock], 0
		je		.spin_wait
	.release:
	sti
	jmp		scheduler_idle

GLOBAL	scheduler_idle
scheduler_idle:
	hlt
	jmp	scheduler_idle
