;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		timer.asm
;; Description:	Provide timer services for context switching directly in assembly
;; 				to be as fast as possible 
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


%include "scheduler.inc"

EXTERN	scheduler_next_task
EXTERN	local_apic_eoi
EXTERN	log_default

SECTION	.data

string:	db "data: 0x%x",10,0

SECTION	.text

GLOBAL	timer_handler
timer_handler:
	PUSH_REG
	mov		rbp, rsp
	; Check this context
	mov		rbx, [rbp + SCHEDULER_CONTEXT_OFFSET(SchedulerContext.context_frame_self)]				; get self
	cmp		rbx, 0
	je		.next
	; Copy context
	std																								; move backwards
	mov		rdi, [rbp + SCHEDULER_CONTEXT_OFFSET(SchedulerContext.context_frame_interrupt_end)]		; the destination is the context frame
	mov		rax, rbp																				; the source is the current stack
	mov		rcx, SCHEDULER_CONTEXT_FRAME_COPY_NUM													; number of times to repeat
	rep stosq
	cld
	.next:
		call	log_default
		mov		rdi, rbx
		call	scheduler_next_task
		cmp		rax, 0
		je		.exit
		mov		QWORD [rbp + SCHEDULER_CONTEXT_OFFSET(SchedulerContext.context_frame_self)], rax	; save the context
		lea		rbp, [rax + SchedulerContext.context_frame]											; save the context frame's stack
	.exit:
		call	local_apic_eoi
		mov		rsp, rbp
		POP_REG
		iretq