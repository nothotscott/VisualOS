SECTION	.rodata

string1:	db "Hello, world!", 10,0

SECTION	.text

GLOBAL	_start
_start:
	; Stack test
	push	0x80
	push	0x123
	pop		rax
	pop		rax
	cmp		rax, 0x80
	jne		0x0

	; Text output syscall test
	mov		rax, 1
	mov		rdi, 1
	mov		rsi, string1
	mov		rdx, 13
	syscall

	jmp		$
	; Exit syscall test
	mov		rax, 60
	mov		rdi, 0
	syscall
