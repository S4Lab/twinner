	.file	"simple-condition-test.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, -4(%rbp)
	movq	-32(%rbp), %rax
	addq	$16, %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, -8(%rbp)
	movl	-4(%rbp), %eax
	addl	%eax, %eax
	addl	$3, %eax
	movl	%eax, -12(%rbp)
	movl	-8(%rbp), %eax
	subl	$3, %eax
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	addl	$4, %eax
	cmpl	-12(%rbp), %eax
	jne	.L2
	movl	$0, -12(%rbp)
	jmp	.L3
.L2:
	movl	$32, -12(%rbp)
.L3:
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.1 20130725 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
