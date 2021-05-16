;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		timer.asm
;; 
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

GLOBAL	scheduler_next_task

SECTION	.text

%include "scheduler.inc"

GLOBAL	timer_handler
timer_handler:
	mov		ax, gs
	cmp		ax, 0
	je		.task_switch
	.task_switch:
	mov		QWORD [gs:SchedulerContext.rsp_task], rsp	; save the task's stack
	lea		rdi, [gs:0]
	.task_receive:
	;call	scheduler_next_task							; load the next task
	WRITE_GSBASE	rax
	mov		rsp, [gs:SchedulerContext.rsp_task]			; switch to the new task's stack
	ret