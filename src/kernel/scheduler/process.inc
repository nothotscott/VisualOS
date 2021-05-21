;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		process.inc
;;
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

struc Process
	.pid:				resq	1
	.pml4:				resq	1
	.flags:				resq	1
	.thread_contexts:	resq	1
	; Not necessary past here
	.parent:			resq	1
	.next:				resq	1
	.child_list:		resq	1
endstruc