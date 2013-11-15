	.file	"assembly-mov-sizes.c"
	.section	.rodata
.LC0:
	.string	"Testing with RAX => 0x%llX\n"
.LC1:
	.string	"Testing with R12 => 0x%llX\n"
.LC2:
	.string	"Testing with RDI => 0x%llX\n"
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
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	movabsq	$72623859790382856, %rax
	movq	%rax, -24(%rbp)
	movl	$-1583176796, -28(%rbp)
	movl	$45490, -32(%rbp)
	movl	$193, -36(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rbx
#APP
# 8 "assembly-mov-sizes.c" 1
	movq %rbx, %rax
	movl -28(%rbp), %eax
	movw -32(%rbp), %ax
	movb -36(%rbp), %ah
	movq %rax, %rbx
	
# 0 "" 2
#NO_APP
	movq	%rbx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movabsq	$72623859790382856, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rbx
#APP
# 18 "assembly-mov-sizes.c" 1
	movq %rbx, %r12
	movl -28(%rbp), %r12d
	movw -32(%rbp), %r12w
	movb -36(%rbp), %r12b
	movq %r12, %rbx
	
# 0 "" 2
#NO_APP
	movq	%rbx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movabsq	$72623859790382856, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rbx
#APP
# 28 "assembly-mov-sizes.c" 1
	movq %rbx, %rdi
	movl -28(%rbp), %edi
	movq %rdi, %rbx
	
# 0 "" 2
#NO_APP
	movq	%rbx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
