;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		gdt.inc
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

%define GDT_OFFSET_KERNEL_CODE	0x08
%define GDT_OFFSET_KERNEL_DATA	0x10
%define GDT_OFFSET_USER_DATA	0x20
%define GDT_OFFSET_USER_CODE	0x28
%define GDT_OFFSET_TSS			0x30