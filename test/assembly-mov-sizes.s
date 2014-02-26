	.file	"assembly-mov-sizes.c"
	.text
.Ltext0:
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
	.file 1 "assembly-mov-sizes.c"
	.loc 1 3 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	.cfi_offset 3, -24
	.loc 1 4 0
	movabsq	$72623859790382856, %rax
	movq	%rax, -24(%rbp)
	.loc 1 5 0
	movl	$-1583176796, -28(%rbp)
	.loc 1 6 0
	movl	$45490, -32(%rbp)
	.loc 1 7 0
	movl	$193, -36(%rbp)
	.loc 1 8 0
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
	.loc 1 16 0
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	.loc 1 17 0
	movabsq	$72623859790382856, %rax
	movq	%rax, -24(%rbp)
	.loc 1 18 0
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
	.loc 1 26 0
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	.loc 1 27 0
	movabsq	$72623859790382856, %rax
	movq	%rax, -24(%rbp)
	.loc 1 28 0
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
	.loc 1 34 0
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	.loc 1 35 0
	movl	$0, %eax
	.loc 1 36 0
	addq	$40, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0xd7
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF10
	.byte	0x1
	.long	.LASF11
	.long	.LASF12
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
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF8
	.uleb128 0x4
	.long	.LASF13
	.byte	0x1
	.byte	0x3
	.long	0x57
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0xc9
	.uleb128 0x5
	.string	"tt"
	.byte	0x1
	.byte	0x4
	.long	0xc9
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x5
	.string	"dd"
	.byte	0x1
	.byte	0x5
	.long	0xd5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x5
	.string	"ee"
	.byte	0x1
	.byte	0x6
	.long	0xd5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x5
	.string	"ff"
	.byte	0x1
	.byte	0x7
	.long	0xd5
	.uleb128 0x2
	.byte	0x91
	.sleb128 -52
	.byte	0
	.uleb128 0x6
	.long	0xce
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x7
	.long	0x57
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
	.uleb128 0x5
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
	.uleb128 0x6
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
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
.LASF9:
	.string	"long long int"
.LASF3:
	.string	"unsigned int"
.LASF12:
	.string	"/home/behnam/thesis/tools/Twinner/test"
.LASF10:
	.string	"GNU C 4.8.2 20140206 (prerelease) -mtune=generic -march=x86-64 -g"
.LASF0:
	.string	"long unsigned int"
.LASF11:
	.string	"assembly-mov-sizes.c"
.LASF8:
	.string	"char"
.LASF1:
	.string	"unsigned char"
.LASF13:
	.string	"main"
.LASF6:
	.string	"long int"
.LASF2:
	.string	"short unsigned int"
.LASF4:
	.string	"signed char"
.LASF5:
	.string	"short int"
.LASF7:
	.string	"sizetype"
	.ident	"GCC: (GNU) 4.8.2 20140206 (prerelease)"
	.section	.note.GNU-stack,"",@progbits
