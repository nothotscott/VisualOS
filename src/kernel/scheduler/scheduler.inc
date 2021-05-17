;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		scheduler.inc
;;
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "x86_64/cpu.inc"

%define	SCHEDULER_CONTEXT_FRAME_GENERAL_NUM		15
%define	SCHEDULER_CONTEXT_FRAME_INTERRUPT_NUM	5
%define SCHEDULER_CONTEXT_FRAME_COPY_NUM		(SCHEDULER_CONTEXT_FRAME_GENERAL_NUM + SCHEDULER_CONTEXT_FRAME_INTERRUPT_NUM)
%define	SCHEDULER_CONTEXT_OFFSET(n)				(n - SchedulerContext.context_frame)

struc	SchedulerContext
	.context_frame:
		.context_frame_general:			resq	SCHEDULER_CONTEXT_FRAME_GENERAL_NUM
		.context_frame_interrupt:		resq	SCHEDULER_CONTEXT_FRAME_INTERRUPT_NUM
		.context_frame_interrupt_end:
		.context_frame_self:			resq	1
	.context_frame_end:
	; Not important past this point
endstruc

; Write the value [%1] to GSBase MSR. If [%1] is not given, it assumes rdi 
%macro	WRITE_GSBASE	0-1
	%if	%0 == 1
		mov		rdi, %1
	%endif
	mov		ecx, CPU_MSR_GSBASE	
	mov		eax, edi
	shr		rdi, 32
	mov		edx, edi
	wrmsr
%endmacro