	.file	"multi-path-with-printf-leafs-test.c"
	.text
.Ltext0:
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
	.file 1 "multi-path-with-printf-leafs-test.c"
	.loc 1 3 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	.loc 1 4 0
	cmpl	$1, -20(%rbp)
	jg	.L2
	.loc 1 5 0
	movq	-32(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	.loc 1 6 0
	movl	$-1, %eax
	jmp	.L3
.L2:
	.loc 1 8 0
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, -4(%rbp)
	.loc 1 9 0
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	addq	$1, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, -8(%rbp)
	.loc 1 10 0
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	addq	$2, %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$48, %eax
	movl	%eax, -12(%rbp)
	.loc 1 11 0
	cmpl	$5, -4(%rbp)
	jle	.L4
	.loc 1 12 0
	cmpl	$5, -8(%rbp)
	jle	.L5
	.loc 1 13 0
	cmpl	$5, -12(%rbp)
	jle	.L6
	.loc 1 14 0
	movl	$.LC1, %edi
	call	puts
	jmp	.L10
.L6:
	.loc 1 16 0
	movl	$.LC2, %edi
	call	puts
	jmp	.L10
.L5:
	.loc 1 18 0
	cmpl	$5, -12(%rbp)
	jle	.L9
	.loc 1 19 0
	movl	$.LC3, %edi
	call	puts
	jmp	.L10
.L9:
	.loc 1 21 0
	movl	$.LC4, %edi
	call	puts
	jmp	.L10
.L4:
	.loc 1 23 0
	cmpl	$5, -8(%rbp)
	jle	.L11
	.loc 1 24 0
	cmpl	$5, -12(%rbp)
	jle	.L12
	.loc 1 25 0
	movl	$.LC5, %edi
	call	puts
	jmp	.L10
.L12:
	.loc 1 27 0
	movl	$.LC6, %edi
	call	puts
	jmp	.L10
.L11:
	.loc 1 30 0
	cmpl	$5, -12(%rbp)
	jle	.L14
	.loc 1 31 0
	movl	$.LC7, %edi
	call	puts
	jmp	.L10
.L14:
	.loc 1 33 0
	movl	$.LC8, %edi
	call	puts
.L10:
	.loc 1 36 0
	movl	$.LC9, %edi
	call	puts
	.loc 1 37 0
	movl	$0, %eax
.L3:
	.loc 1 38 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xde
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF11
	.byte	0x1
	.long	.LASF12
	.long	.LASF13
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
	.long	0x72
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF8
	.uleb128 0x5
	.long	.LASF14
	.byte	0x1
	.byte	0x3
	.long	0x57
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0xdb
	.uleb128 0x6
	.long	.LASF9
	.byte	0x1
	.byte	0x3
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x6
	.long	.LASF10
	.byte	0x1
	.byte	0x3
	.long	0xdb
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x7
	.string	"a"
	.byte	0x1
	.byte	0x8
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x7
	.string	"b"
	.byte	0x1
	.byte	0x9
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x7
	.string	"c"
	.byte	0x1
	.byte	0xa
	.long	0x57
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x4
	.byte	0x8
	.long	0x6c
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
	.uleb128 0x49
	.uleb128 0x13
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
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
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
	.uleb128 0x7
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
.LASF2:
	.string	"short unsigned int"
.LASF3:
	.string	"unsigned int"
.LASF13:
	.string	"/home/behnam/thesis/tools/Twinner/test"
.LASF11:
	.string	"GNU C 4.8.2 20140206 (prerelease) -mtune=generic -march=x86-64 -g"
.LASF0:
	.string	"long unsigned int"
.LASF8:
	.string	"char"
.LASF1:
	.string	"unsigned char"
.LASF14:
	.string	"main"
.LASF6:
	.string	"long int"
.LASF12:
	.string	"multi-path-with-printf-leafs-test.c"
.LASF9:
	.string	"argc"
.LASF4:
	.string	"signed char"
.LASF5:
	.string	"short int"
.LASF7:
	.string	"sizetype"
.LASF10:
	.string	"argv"
	.ident	"GCC: (GNU) 4.8.2 20140206 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
