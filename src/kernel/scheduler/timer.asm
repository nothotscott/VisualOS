;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		timer.asm
;; Description:	Provide timer services for context switching directly in assembly
;; 				to be as fast as possible 
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


%include	"x86_64/gdt.inc"
%include	"scheduler.inc"

EXTERN	scheduler_next_task
EXTERN	local_apic_eoi
EXTERN	log_default

SECTION	.data

string:	db "hi",10,0

SECTION	.text

GLOBAL	timer_handler
timer_handler:
	PUSH_REG
	mov		rbp, rsp
	; Set kernel data segments
	mov		ax, GDT_OFFSET_KERNEL_DATA
	mov		ds, ax
	mov		es, ax
	mov		ss, ax
	; Check this context
	mov		rbx, [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET(SchedulerNode.context_frame_self)]					; get self
	cmp		rbx, 0
	je		.next
	; Copy context
	std																										; move backwards
	mov		rdi, [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET(SchedulerNode.context_frame_interrupt_end)]			; the destination is the context frame
	mov		rax, rbp																						; the source is the current stack
	mov		rcx, SCHEDULER_CONTEXT_FRAME_COPY_NUM															; number of times to repeat
	rep stosq
	cld
	.next:
		mov		rdi, rbx
		call	scheduler_next_task
		cmp		rax, 0
		je		.exit
		mov		QWORD [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET(SchedulerNode.context_frame_self)], rax			; save the context
		lea		rbp, [rax + SchedulerNode.context_frame]													; save the context frame's stack
		WRITE_GSBASE	rax																					; save the context to GSBase
	.exit:
		call	local_apic_eoi
		mov		rax, [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET(SchedulerNode.context_frame_interrupt + 0x20)]	; get ss
		mov		ds, ax																						; set other segments to ss
		mov		es, ax
		mov		rsp, rbp
		POP_REG
		iretq