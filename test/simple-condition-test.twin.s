	.file	"simple-condition-test.twin.c"
	.text
.Ltext0:
	.globl	setRegistersValuesAndInvokeSyscall
	.type	setRegistersValuesAndInvokeSyscall, @function
setRegistersValuesAndInvokeSyscall:
.LFB0:
	.file 1 "../src/res/twincode.h"
	.loc 1 80 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	.loc 1 82 0
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
	.loc 1 84 0
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
	.loc 1 98 0
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
	.loc 1 99 0
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
	.file 2 "simple-condition-test.twin.c"
	.loc 2 4 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$168, %rsp
	.loc 2 6 0
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
	.loc 2 7 0
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
	.loc 2 8 0
	movabsq	$140737488347360, %rax
	movq	(%rax), %rax
	movq	%rax, -136(%rbp)
	.loc 2 9 0
	movabsq	$140737488347368, %rax
	movq	(%rax), %rax
	movq	%rax, -144(%rbp)
	.loc 2 10 0
	movabsq	$140737488347966, %rax
	movq	(%rax), %rax
	movq	%rax, -152(%rbp)
	.loc 2 11 0
	movabsq	$140737488347971, %rax
	movq	(%rax), %rax
	movq	%rax, -160(%rbp)
	.loc 2 84 0
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
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x2c4
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF24
	.byte	0x1
	.long	.LASF25
	.long	.LASF26
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF27
	.byte	0x1
	.byte	0x10
	.long	0x38
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF23
	.uleb128 0x4
	.long	.LASF28
	.byte	0x80
	.byte	0x1
	.byte	0x12
	.long	0x10a
	.uleb128 0x5
	.string	"rax"
	.byte	0x1
	.byte	0x14
	.long	0x2d
	.byte	0
	.uleb128 0x5
	.string	"rbx"
	.byte	0x1
	.byte	0x15
	.long	0x2d
	.byte	0x8
	.uleb128 0x5
	.string	"rcx"
	.byte	0x1
	.byte	0x16
	.long	0x2d
	.byte	0x10
	.uleb128 0x5
	.string	"rdx"
	.byte	0x1
	.byte	0x17
	.long	0x2d
	.byte	0x18
	.uleb128 0x5
	.string	"rdi"
	.byte	0x1
	.byte	0x18
	.long	0x2d
	.byte	0x20
	.uleb128 0x5
	.string	"rsi"
	.byte	0x1
	.byte	0x19
	.long	0x2d
	.byte	0x28
	.uleb128 0x5
	.string	"rsp"
	.byte	0x1
	.byte	0x1a
	.long	0x2d
	.byte	0x30
	.uleb128 0x5
	.string	"rbp"
	.byte	0x1
	.byte	0x1b
	.long	0x2d
	.byte	0x38
	.uleb128 0x5
	.string	"r8"
	.byte	0x1
	.byte	0x1c
	.long	0x2d
	.byte	0x40
	.uleb128 0x5
	.string	"r9"
	.byte	0x1
	.byte	0x1d
	.long	0x2d
	.byte	0x48
	.uleb128 0x5
	.string	"r10"
	.byte	0x1
	.byte	0x1e
	.long	0x2d
	.byte	0x50
	.uleb128 0x5
	.string	"r11"
	.byte	0x1
	.byte	0x1f
	.long	0x2d
	.byte	0x58
	.uleb128 0x5
	.string	"r12"
	.byte	0x1
	.byte	0x20
	.long	0x2d
	.byte	0x60
	.uleb128 0x5
	.string	"r13"
	.byte	0x1
	.byte	0x21
	.long	0x2d
	.byte	0x68
	.uleb128 0x5
	.string	"r14"
	.byte	0x1
	.byte	0x22
	.long	0x2d
	.byte	0x70
	.uleb128 0x5
	.string	"r15"
	.byte	0x1
	.byte	0x23
	.long	0x2d
	.byte	0x78
	.byte	0
	.uleb128 0x6
	.long	.LASF29
	.byte	0x1
	.byte	0x50
	.long	0x3f
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x164
	.uleb128 0x7
	.long	.LASF2
	.byte	0x1
	.byte	0x50
	.long	0x3f
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x8
	.long	.LASF0
	.byte	0x1
	.byte	0x51
	.long	0x3f
	.uleb128 0x9
	.byte	0x3
	.quad	currentRegs.1739
	.uleb128 0x8
	.long	.LASF1
	.byte	0x1
	.byte	0x53
	.long	0x3f
	.uleb128 0x9
	.byte	0x3
	.quad	resultingRegs.1740
	.byte	0
	.uleb128 0x9
	.long	.LASF30
	.byte	0x2
	.byte	0x4
	.long	0x2bb
	.quad	.LFB1
	.quad	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x2bb
	.uleb128 0x8
	.long	.LASF2
	.byte	0x2
	.byte	0x5
	.long	0x3f
	.uleb128 0x3
	.byte	0x91
	.sleb128 -304
	.uleb128 0x8
	.long	.LASF3
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x8
	.long	.LASF4
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x8
	.long	.LASF5
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x8
	.long	.LASF6
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x8
	.long	.LASF7
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x8
	.long	.LASF8
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x8
	.long	.LASF9
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x8
	.long	.LASF10
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -80
	.uleb128 0x8
	.long	.LASF11
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x8
	.long	.LASF12
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x8
	.long	.LASF13
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -104
	.uleb128 0x8
	.long	.LASF14
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -112
	.uleb128 0x8
	.long	.LASF15
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -120
	.uleb128 0x8
	.long	.LASF16
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -128
	.uleb128 0x8
	.long	.LASF17
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -136
	.uleb128 0x8
	.long	.LASF18
	.byte	0x2
	.byte	0x7
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -144
	.uleb128 0x8
	.long	.LASF19
	.byte	0x2
	.byte	0x8
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -152
	.uleb128 0x8
	.long	.LASF20
	.byte	0x2
	.byte	0x9
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -160
	.uleb128 0x8
	.long	.LASF21
	.byte	0x2
	.byte	0xa
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -168
	.uleb128 0x8
	.long	.LASF22
	.byte	0x2
	.byte	0xb
	.long	0x2c2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -176
	.byte	0
	.uleb128 0xa
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0xb
	.long	0x2d
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF10:
	.string	"rbp_0"
.LASF28:
	.string	"RegistersSet"
.LASF21:
	.string	"m7fffffffe33e_0"
.LASF16:
	.string	"r13_0"
.LASF15:
	.string	"r12_0"
.LASF14:
	.string	"r11_0"
.LASF3:
	.string	"rax_0"
.LASF20:
	.string	"m7fffffffe0e8_0"
.LASF9:
	.string	"rsp_0"
.LASF26:
	.string	"/home/behnam/thesis/tools/Twinner/test"
.LASF27:
	.string	"UINT64"
.LASF22:
	.string	"m7fffffffe343_0"
.LASF29:
	.string	"setRegistersValuesAndInvokeSyscall"
.LASF11:
	.string	"r8_0"
.LASF4:
	.string	"rbx_0"
.LASF17:
	.string	"r14_0"
.LASF24:
	.string	"GNU C 4.8.2 20140206 (prerelease) -mtune=generic -march=x86-64 -g"
.LASF23:
	.string	"long long unsigned int"
.LASF1:
	.string	"resultingRegs"
.LASF30:
	.string	"main"
.LASF19:
	.string	"m7fffffffe0e0_0"
.LASF5:
	.string	"rcx_0"
.LASF7:
	.string	"rdi_0"
.LASF25:
	.string	"simple-condition-test.twin.c"
.LASF0:
	.string	"currentRegs"
.LASF2:
	.string	"regs"
.LASF12:
	.string	"r9_0"
.LASF18:
	.string	"r15_0"
.LASF8:
	.string	"rsi_0"
.LASF6:
	.string	"rdx_0"
.LASF13:
	.string	"r10_0"
	.ident	"GCC: (GNU) 4.8.2 20140206 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
