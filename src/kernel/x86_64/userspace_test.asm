SECTION	.text
ALIGN	4096
GLOBAL	test_userspace
test_userspace:
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
	mov		rsi, test_userspace_data
	mov		rdx, 17
	syscall
	
	jmp		$

	; Exit syscall test
	mov		rax, 60
	mov		rdi, 0
	syscall

	jmp		$

GLOBAL	test_userspace2
test_userspace2:
	jmp	$

test_userspace_data:
	db "Hello, userspace", 10,0