	.file	"multi-path-with-printf-leafs-test.c"
	.section	.rodata
.LC0:
	.string	"Usage: %s <command codes>\n"
.LC1:
	.string	"a > 5, b > 5, c > 5 -- case 1"
	.align 8
.LC2:
	.string	"a > 5, b > 5, c <= 5 -- case 2"
	.align 8
.LC3:
	.string	"a > 5, b <= 5, c > 5 -- case 3"
	.align 8
.LC4:
	.string	"a > 5, b <= 5, c <= 5 -- case 4"
	.align 8
.LC5:
	.string	"a <= 5, b > 5, c > 5 -- case 5"
	.align 8
.LC6:
	.string	"a <= 5, b > 5, c <= 5 -- case 6"
	.align 8
.LC7:
	.string	"a <= 5, b <= 5, c > 5 -- case 7"
	.align 8
.LC8:
	.string	"a <= 5, b <= 5, c <= 5 -- case 8"
	.align 8
.LC9:
	.string	"exiting multi-path test program with printf-leafs"
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
	cmpl	$1, -20(%rbp)
	jg	.L2
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$-1, %eax
	jmp	.L3
.L2:
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, -4(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	addq	$1, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, -8(%rbp)
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	addq	$2, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, -12(%rbp)
	cmpl	$5, -4(%rbp)
	jle	.L4
	cmpl	$5, -8(%rbp)
	jle	.L5
	cmpl	$5, -12(%rbp)
	jle	.L6
	movl	$.LC1, %edi
	call	puts
	jmp	.L10
.L6:
	movl	$.LC2, %edi
	call	puts
	jmp	.L10
.L5:
	cmpl	$5, -12(%rbp)
	jle	.L9
	movl	$.LC3, %edi
	call	puts
	jmp	.L10
.L9:
	movl	$.LC4, %edi
	call	puts
	jmp	.L10
.L4:
	cmpl	$5, -8(%rbp)
	jle	.L11
	cmpl	$5, -12(%rbp)
	jle	.L12
	movl	$.LC5, %edi
	call	puts
	jmp	.L10
.L12:
	movl	$.LC6, %edi
	call	puts
	jmp	.L10
.L11:
	cmpl	$5, -12(%rbp)
	jle	.L14
	movl	$.LC7, %edi
	call	puts
	jmp	.L10
.L14:
	movl	$.LC8, %edi
	call	puts
.L10:
	movl	$.LC9, %edi
	call	puts
	movl	$0, %eax
.L3:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.2 20140206 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
