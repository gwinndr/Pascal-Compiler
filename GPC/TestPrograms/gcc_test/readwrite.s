	.file	"readwrite.c"
	.text
	.section	.rodata
.LC0:
	.string	"%d"
	.text
	.globl	read
	.type	read, @function
read:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	leaq	-12(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	nop
	movq	-8(%rbp), %rax
	jmp	.L2
	call	__stack_chk_fail@PLT
.L2:
	leave
	ret
	.size	read, .-read
	.section	.rodata
.LC1:
	.string	"%d\n"
	.text
	.globl	write
	.type	write, @function
write:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	$5, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC1(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	ret
	.size	write, .-write
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, %eax
	call	read
	movl	$0, %eax
	call	write
	movl	$0, %eax
	popq	%rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
