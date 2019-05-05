	.file	"non_local.c"
	.text
	.comm	x,4,4
	.section	.rodata
.LC0:
	.string	"%d\n"
	.text
	.globl	test
	.type	test, @function
test:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, x(%rip)
	jmp	.L2
.L3:
	movl	x(%rip), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	x(%rip), %eax
	addl	$1, %eax
	movl	%eax, x(%rip)
.L2:
	movl	x(%rip), %eax
	cmpl	$9, %eax
	jle	.L3
	nop
	popq	%rbp
	ret
	.size	test, .-test
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, %eax
	call	test
	movl	$0, %eax
	popq	%rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0"
	.section	.note.GNU-stack,"",@progbits
