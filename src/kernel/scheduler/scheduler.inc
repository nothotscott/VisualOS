;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		scheduler.inc
;;
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%include "x86_64/cpu.inc"

struc	SchedulerContext
	.rsp_task:		resq	1
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