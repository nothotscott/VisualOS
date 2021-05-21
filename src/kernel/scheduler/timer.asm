;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		timer.asm
;; Description:	Provide timer services for context switching directly in assembly
;; 				to be as fast as possible 
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


%include	"x86_64/gdt.inc"
%include	"process.inc"
%include	"scheduler.inc"

EXTERN	scheduler_next_task
EXTERN	local_apic_eoi
EXTERN	log_default

SECTION	.data

string:	db "test: 0x%x",10,0

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
	; Switch to the kernel PML4
	mov		rax, [rbp + SCHEDULER_KERNEL_PML4_OFFSET]
	mov		cr3, rax
	; Check this context
	mov		rbx, [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET + SchedulerContextFrame.self]					; get self
	cmp		rbx, 0
	je		.next
	; Copy context
	lea		rsi, [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET + SchedulerContextFrame]							; the source is the stack
	lea		rdi, [rbx + SCHEDULER_CONTEXT_FRAME_OFFSET + SchedulerContextFrame]														; the destination is the self's context frame
	mov		rcx, SCHEDULER_CONTEXT_FRAME_COPY_NUM														; number of times to repeat
	.copy:
		lodsq
		stosq
		loop	.copy
	
	; Get next task
	.next:
		mov		rdi, rbx
		call	scheduler_next_task
		cmp		rax, 0
		je		.exit
		mov		QWORD [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET + SchedulerContextFrame.self], rax			; save the context
		lea		rbp, [rax + SCHEDULER_CONTEXT_FRAME_OFFSET]												; save the context frame's stack
		mov		rsi, rax
		WRITE_GSBASE	rax																				; save the context to GSBase
		; Switch PML4 if applicable
		mov		rsi, [rsi + SchedulerNode.context + SchedulerContext.task + Thread.process]				; load the context's process pointer
		cmp		rsi, 0
		je		.exit
		mov		rax, [rsi + Process.pml4]																; load the PML4
		cmp		rax, 0
		je		.exit
		mov		cr3, rax
	.exit:
		mov		rax, [rbp + SCHEDULER_CONTEXT_FRAME_OFFSET + SchedulerContextFrame.interrupt + 0x20]	; get ss
		mov		ds, ax																					; set other segments to ss
		mov		es, ax
		call	local_apic_eoi
		mov		rsp, rbp
		POP_REG
		iretq