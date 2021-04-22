;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		apic.asm
;;
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


%define	CPU_MSR_FS_BASE			0xc0000100
%define	CPU_MSR_GS_BASE			0xc0000101
%define	CPU_MSR_KERNEL_GS_BASE	0xc0000102

struc	CPUContext
	.local_apic_id:	resb	1
	.gdt_block:		resq	1
	.idt_block:		resq	1
endstruc