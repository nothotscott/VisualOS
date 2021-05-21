;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		scheduler.inc
;;
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include	"x86_64/cpu.inc"

%define	SCHEDULER_CONTEXT_FRAME_GENERAL_NUM		15
%define	SCHEDULER_CONTEXT_FRAME_INTERRUPT_NUM	5
%define SCHEDULER_CONTEXT_FRAME_COPY_NUM		(SCHEDULER_CONTEXT_FRAME_GENERAL_NUM + SCHEDULER_CONTEXT_FRAME_INTERRUPT_NUM)
%define	SCHEDULER_KERNEL_PML4_OFFSET			(SchedulerContextFrame_size + 0x00)
%define	SCHEDULER_CONTEXT_FRAME_OFFSET			(SchedulerNode.context + SchedulerContext.context_frame)

struc	Thread
	.tid:		resq	1
	.process:	resq	1
	.next:		resq	1
endstruc
struc	SchedulerContextFrame
	.general:		resq	SCHEDULER_CONTEXT_FRAME_GENERAL_NUM
	.interrupt:		resq	SCHEDULER_CONTEXT_FRAME_INTERRUPT_NUM
	.self:			resq	1
endstruc
struc	SchedulerContext
	.context_frame:	resb	SchedulerContextFrame_size
	.flags:			resq	1
	.error_code:	resq	1
	.task:			resb	Thread_size
endstruc
struc	SchedulerNode
	.context:		resb	SchedulerContext_size
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