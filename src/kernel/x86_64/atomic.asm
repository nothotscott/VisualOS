;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; File:		atomic.asm
;; Copyright 2021 Scott Maday
;; Check the LICENSE file that came with this program for licensing terms
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


GLOBAL	atomic_lock
atomic_lock:	; rdi=[mutex], rsi=[bit]
	lock bts	QWORD [rdi], rsi
	mov			rcx, 1
	mov			rax, 0
	cmovnc		rax, rcx			; conditional mov if not carry
	ret

GLOBAL	atomic_unlock
atomic_unlock:	; rdi=[mutex], rsi=[bit]
	btr		QWORD [rdi], rsi
	mov		rcx, 1
	mov		rax, 1
	cmovnc	rax, rcx
	ret