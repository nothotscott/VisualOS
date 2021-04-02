;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		isr.asm
;; 
;; Copyright 2020 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


EXTERN	paging_get_pagetable_l4

SECTION	.text

GLOBAL	paging_load
paging_load:
	call	paging_get_pagetable_l4	
	mov		cr3, rax				; should have pagetable_l4 in rax
	ret