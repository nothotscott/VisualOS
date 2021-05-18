;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		gdt.asm
;; Copyright 2020-2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include	"gdt.inc"

SECTION	.text

GLOBAL	gdt_load
gdt_load:	; rdi=[gdt_descriptor]
	; Setup GDT
	cli
	lgdt	[rdi]
	; Setup TSS
	mov		ax, GDT_OFFSET_TSS				; TSS descriptor: idt[5]
	ltr		ax
	; This will exploit iretq's ability to set segments in long mode
	; The order iretq will pop is: RIP, CS, RFLAGS, RSP, SS
	; We first need to save data we wish to keep
	pop		rsi								; keep the return address in rsi
	mov		rdi, rsp						; keep the old stack pointer in rdi
	mov		rax, GDT_OFFSET_KERNEL_DATA		; move the data segments
	mov		ds, ax
	mov		es, ax
	; Now to push what iretq needs
	push	rax								; push stack segment: g_idt[2]
	push	rdi								; push old stack pointer
	pushfq									; push flags
	push	QWORD GDT_OFFSET_KERNEL_CODE	; push code segment: g_idt[1]
	push	rsi								; set rip to the return address from earlier
	iretq									; far return in new code segment