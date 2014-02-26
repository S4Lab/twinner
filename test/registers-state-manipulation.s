	.file	"registers-state-manipulation.c"
	.text
.Ltext0:
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
	.file 1 "registers-state-manipulation.c"
	.loc 1 3 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	.loc 1 4 0
	movabsq	$-6799692559826901080, %rax
	movq	%rax, -56(%rbp)
	.loc 1 5 0
	movabsq	$-5642249794417674312, %rax
	movq	%rax, -64(%rbp)
	.loc 1 6 0
	movabsq	$-4484807029008447544, %rax
	movq	%rax, -72(%rbp)
	.loc 1 7 0
	movabsq	$-3327364263599220776, %rax
	movq	%rax, -80(%rbp)
	.loc 1 8 0
	movabsq	$-2169921498189994008, %rax
	movq	%rax, -88(%rbp)
	.loc 1 10 0
	movabsq	$-6799692559826901080, %rax
	movq	%rax, -8(%rbp)
	.loc 1 11 0
	movabsq	$-5642249794417674312, %rax
	movq	%rax, -16(%rbp)
	.loc 1 12 0
	movabsq	$-4484807029008447544, %rax
	movq	%rax, -24(%rbp)
	.loc 1 13 0
	movabsq	$-3327364263599220776, %rax
	movq	%rax, -32(%rbp)
	.loc 1 14 0
	movabsq	$-2169921498189994008, %rax
	movq	%rax, -40(%rbp)
	.loc 1 15 0
	leaq	-56(%rbp), %rax
	movq	%rax, -48(%rbp)
	.loc 1 16 0
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
	.loc 1 39 0
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
	.loc 1 44 0
	movq	-40(%rbp), %rsi
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rax
	movq	%rsi, %r8
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	.loc 1 49 0
	movl	$0, %eax
	.loc 1 50 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x13f
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF16
	.byte	0x1
	.long	.LASF17
	.long	.LASF18
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF7
	.uleb128 0x4
	.byte	0x8
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF8
	.uleb128 0x5
	.long	.LASF19
	.byte	0x1
	.byte	0x3
	.long	0x57
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x136
	.uleb128 0x6
	.string	"rax"
	.byte	0x1
	.byte	0x4
	.long	0x136
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0x6
	.string	"rsi"
	.byte	0x1
	.byte	0x5
	.long	0x136
	.uleb128 0x3
	.byte	0x91
	.sleb128 -80
	.uleb128 0x6
	.string	"rsp"
	.byte	0x1
	.byte	0x6
	.long	0x136
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x6
	.string	"rbp"
	.byte	0x1
	.byte	0x7
	.long	0x136
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.uleb128 0x6
	.string	"r11"
	.byte	0x1
	.byte	0x8
	.long	0x136
	.uleb128 0x3
	.byte	0x91
	.sleb128 -104
	.uleb128 0x7
	.long	.LASF9
	.byte	0x1
	.byte	0xa
	.long	0x13d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x7
	.long	.LASF10
	.byte	0x1
	.byte	0xb
	.long	0x13d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x7
	.long	.LASF11
	.byte	0x1
	.byte	0xc
	.long	0x13d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x7
	.long	.LASF12
	.byte	0x1
	.byte	0xd
	.long	0x13d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x7
	.long	.LASF13
	.byte	0x1
	.byte	0xe
	.long	0x13d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x7
	.long	.LASF14
	.byte	0x1
	.byte	0xf
	.long	0x6c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.byte	0
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF15
	.uleb128 0x8
	.long	0x136
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
	.uleb128 0x3
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
	.uleb128 0x4
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x5
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
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0x7
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
	.uleb128 0x8
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
.LASF15:
	.string	"long long int"
.LASF17:
	.string	"registers-state-manipulation.c"
.LASF3:
	.string	"unsigned int"
.LASF13:
	.string	"newr11"
.LASF1:
	.string	"unsigned char"
.LASF14:
	.string	"pointerToRax"
.LASF18:
	.string	"/home/behnam/thesis/tools/Twinner/test"
.LASF16:
	.string	"GNU C 4.8.2 20140206 (prerelease) -mtune=generic -march=x86-64 -g"
.LASF0:
	.string	"long unsigned int"
.LASF8:
	.string	"char"
.LASF10:
	.string	"newrsi"
.LASF19:
	.string	"main"
.LASF6:
	.string	"long int"
.LASF11:
	.string	"newrsp"
.LASF2:
	.string	"short unsigned int"
.LASF4:
	.string	"signed char"
.LASF5:
	.string	"short int"
.LASF12:
	.string	"newrbp"
.LASF9:
	.string	"newrax"
.LASF7:
	.string	"sizetype"
	.ident	"GCC: (GNU) 4.8.2 20140206 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
