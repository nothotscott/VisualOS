;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		isr.asm
;; 
;; Copyright 2020 Scott Maday
;; Credit to https://github.com/JoannaHammond for PAT loading
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


EXTERN	paging_get_pagetable_l4

SECTION	.text

GLOBAL	paging_load
paging_load:
	call	paging_get_pagetable_l4	
	mov		cr3, rax				; should have pagetable_l4 in rax
	ret

GLOBAL	paging_load_pat
paging_load_pat:
	push	rbx
	wbinvd
	mov		ecx, 0x277
	; Provide the PAT registers linearly to each encoding
	; Blank encodings are set to the default (write back)
	mov		eax, 0x6060100	; 00000-110-00000-110-00000-001-00000-000
	mov		edx, 0x7060504	; 00000-111-00000-110-00000-101-00000-100
	wrmsr
	wbinvd
	pop		rbx
	ret