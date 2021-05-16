;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		cpu.inc
;;
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


%define	CPU_MSR_FSBASE			0xc0000100
%define	CPU_MSR_GSBASE			0xc0000101
%define	CPU_MSR_KERNELGSBASE	0xc0000102

struc	CPUContext
	.local_apic_id:		resb	1
	.reserved:			resb	3
	.rsp_kernel:		resq	1
	.rsp_userspace:		resq	1
	.gdt_block:			resq	1
	.idt_block:			resq	1
	.stack_irq:			resq	1
endstruc
