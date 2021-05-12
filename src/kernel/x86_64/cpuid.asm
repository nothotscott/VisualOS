;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		cpuid.asm
;; 
;; Copyright 2020-2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


SECTION	.text

GLOBAL	cpuid_get_local_apic_id
cpuid_get_local_apic_id:
	push	rbx
	mov		eax, 0x01
	cpuid
	mov		eax, ebx
	shr		eax, 24
	and		eax, DWORD 0xff
	pop		rbx
	ret

GLOBAL	cpuid_get_vendor
cpuid_get_vendor:
	push	rbx
	mov		eax, 0x00
	cpuid
	mov		rax, 0
	mov		eax, ecx
	pop		rbx
	ret

GLOBAL	cpuid_get_vendor_str
cpuid_get_vendor_str:	; rdi=[str]
	mov		eax, 0x00
	cpuid
	mov		[rdi + 0], ebx
	mov		[rdi + 4], edx
	mov		[rdi + 8], ecx
	ret

GLOBAL	cpuid_get_brand_str
cpuid_get_brand_str:	; rdi=[str]
	push	rbx
	mov		eax, 0x80000000	; check if brand feature is implemented
	cpuid
	cmp		eax, 0x80000004
	jl		.return			; not supported
	mov		eax, 0x80000002
	cpuid
	mov		[rdi + 0], eax
	mov		[rdi + 4], ebx
	mov		[rdi + 8], ecx
	mov		[rdi + 12], edx
	mov		eax, 0x80000003
	cpuid
	mov		[rdi + 16], eax
	mov		[rdi + 20], ebx
	mov		[rdi + 24], ecx
	mov		[rdi + 28], edx
	mov		eax, 0x80000004
	cpuid
	mov		[rdi + 32], eax
	mov		[rdi + 36], ebx
	mov		[rdi + 40], ecx
	mov		[rdi + 44], edx
	.return:
		pop	rbx
		ret

GLOBAL cpuid_get_features
cpuid_get_features:		; rdi=[features]
	push	rbx
	mov		eax, 0x01		; set 1 and 2
	cpuid
	mov		[rdi + 0], edx
	mov		[rdi + 4], ebx
	mov		[rdi + 8], ecx
	mov		eax, 0x07		; set 3, 4, and 5
	mov		ecx, 0x00
	cpuid
	mov		[rdi + 12], ebx
	mov		[rdi + 16], ecx
	mov		[rdi + 20], edx
	mov		ecx, 0x01		; set 6
	cpuid
	mov		[rdi + 24], eax
	mov		eax, 0x80000001	; amd1 and amd2
	cpuid
	mov		[rdi + 28], edx
	mov		[rdi + 32], ecx
	; Enable Intel's feature set 1 and AMD feature set 1 that are idempotent 
	and		edx, 0b00000001100000111111111111111111
	or		[rdi + 0], edx
	pop		rbx
	ret