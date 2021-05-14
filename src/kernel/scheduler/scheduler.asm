;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		gdt.asm
;; Copyright 2020-2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EXTERN	local_apic_start_lints

SECTION	.text

GLOBAL	scheduler_entry
scheduler_entry;
	mov		rbp, 0
	call	local_apic_start_lints
	.idle:
		hlt
		jmp	.idle