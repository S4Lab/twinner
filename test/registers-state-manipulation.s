	.file	"registers-state-manipulation.c"
	.section	.rodata
	.align 8
.LC0:
	.string	"Old values =>\n\trsi: 0x%llX\n\trsp: 0x%llX\n\trbp: 0x%llX\n\tr11: 0x%llX\n"
	.align 8
.LC1:
	.string	"New values =>\n\trsi: 0x%llX\n\trsp: 0x%llX\n\trbp: 0x%llX\n\tr11: 0x%llX\n"
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
	subq	$96, %rsp
	movabsq	$-6799692559826901080, %rax
	movq	%rax, -56(%rbp)
	movabsq	$-5642249794417674312, %rax
	movq	%rax, -64(%rbp)
	movabsq	$-4484807029008447544, %rax
	movq	%rax, -72(%rbp)
	movabsq	$-3327364263599220776, %rax
	movq	%rax, -80(%rbp)
	movabsq	$-2169921498189994008, %rax
	movq	%rax, -88(%rbp)
	movabsq	$-6799692559826901080, %rax
	movq	%rax, -8(%rbp)
	movabsq	$-5642249794417674312, %rax
	movq	%rax, -16(%rbp)
	movabsq	$-4484807029008447544, %rax
	movq	%rax, -24(%rbp)
	movabsq	$-3327364263599220776, %rax
	movq	%rax, -32(%rbp)
	movabsq	$-2169921498189994008, %rax
	movq	%rax, -40(%rbp)
	leaq	-56(%rbp), %rax
	movq	%rax, -48(%rbp)
#APP
# 16 "registers-state-manipulation.c" 1
	movq	%rsi, -8(%rax)
	movq	%rsp, -16(%rax)
	movq	%rbp, -24(%rax)
	movq	%r11, -32(%rax)
	addq	$8, %rax
	movq	8(%rax), %rsi
	movq	16(%rax), %rsp
	movq	24(%rax), %rbp
	movq	32(%rax), %r11
	subq	$8, %rax
	movq	-8(%rax), %rsi
	movq	-16(%rax), %rsp
	movq	-24(%rax), %rbp
	movq	-32(%rax), %r11
	
# 0 "" 2
#NO_APP
	movq	-88(%rbp), %rsi
	movq	-80(%rbp), %rcx
	movq	-72(%rbp), %rdx
	movq	-64(%rbp), %rax
	movq	%rsi, %r8
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movq	-40(%rbp), %rsi
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rsi, %r8
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.2 20131219 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
