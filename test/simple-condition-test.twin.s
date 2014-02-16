	.file	"simple-condition-test.twin.c"
	.text
	.globl	setRegistersValuesAndInvokeSyscall
	.type	setRegistersValuesAndInvokeSyscall, @function
setRegistersValuesAndInvokeSyscall:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
#APP
# 82 "../src/res/twincode.h" 1
	movq   %rax,  currentRegs.1739(%rip)
	movq   %rbx,  8+currentRegs.1739(%rip)
	movq   %rcx,  16+currentRegs.1739(%rip)
	movq   %rdx,  24+currentRegs.1739(%rip)
	movq   %rdi,  32+currentRegs.1739(%rip)
	movq   %rsi,  40+currentRegs.1739(%rip)
	movq   %rsp,  48+currentRegs.1739(%rip)
	movq   %rbp,  56+currentRegs.1739(%rip)
	movq   %r8,   64+currentRegs.1739(%rip)
	movq   %r9,   72+currentRegs.1739(%rip)
	movq   %r10,  80+currentRegs.1739(%rip)
	movq   %r11,  88+currentRegs.1739(%rip)
	movq   %r12,  96+currentRegs.1739(%rip)
	movq   %r13,  104+currentRegs.1739(%rip)
	movq   %r14,  112+currentRegs.1739(%rip)
	movq   %r15,  120+currentRegs.1739(%rip)
	
# 0 "" 2
# 84 "../src/res/twincode.h" 1
	movq   64+16(%rbp), %r8
	movq   72+16(%rbp), %r9
	movq   80+16(%rbp), %r10
	movq   88+16(%rbp), %r11
	movq   96+16(%rbp), %r12
	movq   104+16(%rbp), %r13
	movq   112+16(%rbp), %r14
	movq   120+16(%rbp), %r15
	movq   16(%rbp), %rax
	movq   8+16(%rbp), %rbx
	movq   16+16(%rbp), %rcx
	movq   24+16(%rbp), %rdx
	movq   32+16(%rbp), %rdi
	movq   40+16(%rbp), %rsi
	movq   48+16(%rbp), %rsp
	movq   56+16(%rbp), %rbp
	syscall
	movq   %rax,  resultingRegs.1740(%rip)
	movq   %rbx,  8+resultingRegs.1740(%rip)
	movq   %rcx,  16+resultingRegs.1740(%rip)
	movq   %rdx,  24+resultingRegs.1740(%rip)
	movq   %rdi,  32+resultingRegs.1740(%rip)
	movq   %rsi,  40+resultingRegs.1740(%rip)
	movq   %rsp,  48+resultingRegs.1740(%rip)
	movq   %rbp,  56+resultingRegs.1740(%rip)
	movq   %r8,   64+resultingRegs.1740(%rip)
	movq   %r9,   72+resultingRegs.1740(%rip)
	movq   %r10,  80+resultingRegs.1740(%rip)
	movq   %r11,  88+resultingRegs.1740(%rip)
	movq   %r12,  96+resultingRegs.1740(%rip)
	movq   %r13,  104+resultingRegs.1740(%rip)
	movq   %r14,  112+resultingRegs.1740(%rip)
	movq   %r15,  120+resultingRegs.1740(%rip)
	movq   64+currentRegs.1739(%rip), %r8
	movq   72+currentRegs.1739(%rip), %r9
	movq   80+currentRegs.1739(%rip), %r10
	movq   88+currentRegs.1739(%rip), %r11
	movq   96+currentRegs.1739(%rip), %r12
	movq   104+currentRegs.1739(%rip), %r13
	movq   112+currentRegs.1739(%rip), %r14
	movq   120+currentRegs.1739(%rip), %r15
	movq   currentRegs.1739(%rip), %rax
	movq   8+currentRegs.1739(%rip), %rbx
	movq   16+currentRegs.1739(%rip), %rcx
	movq   24+currentRegs.1739(%rip), %rdx
	movq   32+currentRegs.1739(%rip), %rdi
	movq   40+currentRegs.1739(%rip), %rsi
	movq   48+currentRegs.1739(%rip), %rsp
	movq   56+currentRegs.1739(%rip), %rbp
	
# 0 "" 2
#NO_APP
	movq	-8(%rbp), %rax
	movq	resultingRegs.1740(%rip), %rdx
	movq	%rdx, (%rax)
	movq	resultingRegs.1740+8(%rip), %rdx
	movq	%rdx, 8(%rax)
	movq	resultingRegs.1740+16(%rip), %rdx
	movq	%rdx, 16(%rax)
	movq	resultingRegs.1740+24(%rip), %rdx
	movq	%rdx, 24(%rax)
	movq	resultingRegs.1740+32(%rip), %rdx
	movq	%rdx, 32(%rax)
	movq	resultingRegs.1740+40(%rip), %rdx
	movq	%rdx, 40(%rax)
	movq	resultingRegs.1740+48(%rip), %rdx
	movq	%rdx, 48(%rax)
	movq	resultingRegs.1740+56(%rip), %rdx
	movq	%rdx, 56(%rax)
	movq	resultingRegs.1740+64(%rip), %rdx
	movq	%rdx, 64(%rax)
	movq	resultingRegs.1740+72(%rip), %rdx
	movq	%rdx, 72(%rax)
	movq	resultingRegs.1740+80(%rip), %rdx
	movq	%rdx, 80(%rax)
	movq	resultingRegs.1740+88(%rip), %rdx
	movq	%rdx, 88(%rax)
	movq	resultingRegs.1740+96(%rip), %rdx
	movq	%rdx, 96(%rax)
	movq	resultingRegs.1740+104(%rip), %rdx
	movq	%rdx, 104(%rax)
	movq	resultingRegs.1740+112(%rip), %rdx
	movq	%rdx, 112(%rax)
	movq	resultingRegs.1740+120(%rip), %rdx
	movq	%rdx, 120(%rax)
	movq	-8(%rbp), %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	setRegistersValuesAndInvokeSyscall, .-setRegistersValuesAndInvokeSyscall
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$168, %rsp
#APP
# 6 "simple-condition-test.twin.c" 1
	movq   %rax,  -288(%rbp)
	movq   %rbx,  8+-288(%rbp)
	movq   %rcx,  16+-288(%rbp)
	movq   %rdx,  24+-288(%rbp)
	movq   %rdi,  32+-288(%rbp)
	movq   %rsi,  40+-288(%rbp)
	movq   %rsp,  48+-288(%rbp)
	movq   %rbp,  56+-288(%rbp)
	movq   %r8,   64+-288(%rbp)
	movq   %r9,   72+-288(%rbp)
	movq   %r10,  80+-288(%rbp)
	movq   %r11,  88+-288(%rbp)
	movq   %r12,  96+-288(%rbp)
	movq   %r13,  104+-288(%rbp)
	movq   %r14,  112+-288(%rbp)
	movq   %r15,  120+-288(%rbp)
	
# 0 "" 2
#NO_APP
	movq	-288(%rbp), %rax
	movq	%rax, -8(%rbp)
	movq	-280(%rbp), %rax
	movq	%rax, -16(%rbp)
	movq	-272(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-264(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	-256(%rbp), %rax
	movq	%rax, -40(%rbp)
	movq	-248(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	-240(%rbp), %rax
	movq	%rax, -56(%rbp)
	movq	-232(%rbp), %rax
	movq	%rax, -64(%rbp)
	movq	-224(%rbp), %rax
	movq	%rax, -72(%rbp)
	movq	-216(%rbp), %rax
	movq	%rax, -80(%rbp)
	movq	-208(%rbp), %rax
	movq	%rax, -88(%rbp)
	movq	-200(%rbp), %rax
	movq	%rax, -96(%rbp)
	movq	-192(%rbp), %rax
	movq	%rax, -104(%rbp)
	movq	-184(%rbp), %rax
	movq	%rax, -112(%rbp)
	movq	-176(%rbp), %rax
	movq	%rax, -120(%rbp)
	movq	-168(%rbp), %rax
	movq	%rax, -128(%rbp)
	movabsq	$140737488347360, %rax
	movq	(%rax), %rax
	movq	%rax, -136(%rbp)
	movabsq	$140737488347368, %rax
	movq	(%rax), %rax
	movq	%rax, -144(%rbp)
	movabsq	$140737488347966, %rax
	movq	(%rax), %rax
	movq	%rax, -152(%rbp)
	movabsq	$140737488347971, %rax
	movq	(%rax), %rax
	movq	%rax, -160(%rbp)
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.local	currentRegs.1739
	.comm	currentRegs.1739,128,32
	.local	resultingRegs.1740
	.comm	resultingRegs.1740,128,32
	.ident	"GCC: (GNU) 4.8.2 20140206 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
