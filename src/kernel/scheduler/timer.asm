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

string:	db "rip: 0x%x",10,0

SECTION	.text

GLOBAL	timer_handler
timer_handler:
	PUSH_REG
	mov		rbp, rsp
	cld
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
	lea		rsi, [rbp + SchedulerNode.context_frame]														; the source is the stack
	lea		rdi, [rbx + SchedulerNode.context_frame]														; the destination is the self's context frame
	mov		rcx, SCHEDULER_CONTEXT_FRAME_COPY_NUM															; number of times to repeat
	.copy:
		lodsq
		stosq
		loop	.copy
	.next:
		mov		rdi, rbx
		call	scheduler_next_task
		cmp		rax, 0
		je		.exit
		mov		QWORD [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET(SchedulerNode.context_frame_self)], rax			; save the context
		lea		rbp, [rax + SchedulerNode.context_frame]													; save the context frame's stack
		WRITE_GSBASE	rax																					; save the context to GSBase
	.exit:
		mov		rax, [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET(SchedulerNode.context_frame_interrupt + 0x20)]	; get ss
		mov		ds, ax																						; set other segments to ss
		mov		es, ax
		call	local_apic_eoi
		mov		rsp, rbp
		POP_REG
		iretq