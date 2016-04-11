	.file	"runtime-simplification.c"
	.section	.rodata
.LC0:
	.string	"overflow"
.LC1:
	.string	"underflow"
.LC2:
	.string	"%d\n"
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
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	movb	%al, -9(%rbp)
	cmpb	$47, -9(%rbp)
	jle	.L2
	cmpb	$57, -9(%rbp)
	jg	.L3
	movsbl	-9(%rbp), %eax
	subl	$48, %eax
	movl	%eax, -4(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L4
.L5:
	movl	-4(%rbp), %eax
	addl	%eax, %eax
	movl	%eax, -4(%rbp)
	addl	$1, -8(%rbp)
.L4:
	cmpl	$9, -8(%rbp)
	jle	.L5
	jmp	.L8
.L3:
	movl	$.LC0, %edi
	call	puts
	movl	$-2, %eax
	jmp	.L7
.L2:
	movl	$.LC1, %edi
	call	puts
	movl	$-1, %eax
	jmp	.L7
.L8:
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
.L7:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 5.3.0"
	.section	.note.GNU-stack,"",@progbits
