	.file	"TestPrograms/CodeGeneration/t1.p"
	.section	.rodata
	.LC0:
		.string	"%d\n"
		.text
	.LC1:
		.string	"%d"
		.text
.globl	boo
.type	boo, @function
boo:
	pushq	%rbp
	movq	%rsp, %rbp
	popq	%rbp
	ret
	.size	boo, .-boo
.globl	main
.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$0, %eax
	call	boo
	movl	$0, %eax
	popq	%rbp
	ret
	.size	main, .-main
.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
.section	.note.GNU-stack,"",@progbits
