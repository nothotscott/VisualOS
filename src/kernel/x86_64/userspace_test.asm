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
	mov		rsi, test_userspace_data1
	mov		rdx, 17
	syscall

	jmp		$

	mov		rax, 1
	mov		rdi, 1
	mov		rsi, test_userspace_data2
	mov		rdx, 18
	syscall
	; Exit syscall test
	mov		rax, 60
	mov		rdi, 0
	syscall

	jmp		$


test_userspace_data1:	db "Hello, userspace", 10,0
test_userspace_data2:	db "Hello, userspace2", 10,0