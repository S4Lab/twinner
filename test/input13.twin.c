
#include "twincode.h"

int func_1 (struct RegistersSet &regs, UINT8 m7fffffffe250_0_8, UINT32 m7fffffffe254_0_32, UINT64 m7fffffffe258_0_64, UINT64 m7fffffffe358_1_64, UINT64 n_v_argv_1, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_1_4, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0);
bool cond_6 (UINT8 n_v_argv_1_4) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x7)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_4) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/);
}

bool cond_7 (UINT8 n_v_argv_1_4) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x2)/*}*/ < /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_4) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/);
}

int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe268*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsp_0) {
if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/ < /*signed {*/signExtend_0x80_0x20 ((rdi_0 & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x4)*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x400892);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = puts (/*"there were unused arguments"*/ (const char *) 0x400892);
		const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
		const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
				func_3 (regs, rbp_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4008ae);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = puts (/*"not too many args"*/ (const char *) 0x4008ae);
		const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
		const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
				func_3 (regs, rbp_0, rsp_0);
	}
}
bool cond_17 (UINT8 n_v_argv_1_4) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/ < /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_4) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/);
}

int func_1 (struct RegistersSet &regs, UINT8 m7fffffffe250_0_8, UINT32 m7fffffffe254_0_32, UINT64 m7fffffffe258_0_64, UINT64 m7fffffffe358_1_64, UINT64 n_v_argv_1, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_1_4, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
		*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((n_v_argv_1_2 << 0x8) | n_v_argv_1_1) << 0x10) | ((n_v_argv_1_0 << 0x8) | m7fffffffe250_0_8))) /*0x7fff46000040*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (n_v_argv_1 /*0x7fffffffe586*/);
		*((UINT64 *) 0x7fffffffe358) = UINT64 (m7fffffffe358_1_64 /*0x7fffffffe58c*/);
		/*Registers Changes*/
		regs.rax = UINT64 (m7fffffffe358_1_64 /*0x7fffffffe58c*/);
		regs.rdi = UINT64 (m7fffffffe358_1_64 /*0x7fffffffe58c*/);
		regs.rsi = UINT64 (0x400839);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = strcmp (/*"right"*/ (const char *) m7fffffffe358_1_64 /*0x7fffffffe58c*/, /*"left"*/ (const char *) 0x400839);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe358_2_64 = *((UINT64 *) 0x7fffffffe358);
		if (((rax_2 & 0xffffffff) /*0x6*/ != 0)) {
			if (cond_6 (n_v_argv_1_4)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((n_v_argv_1_2 << 0x8) | n_v_argv_1_1) << 0x10) | ((n_v_argv_1_0 << 0x8) | m7fffffffe250_0_8))) /*0x7fff46000040*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_1_4) - 0x30) << 0x20) & 0xffffffff00000000) | (m7fffffffe258_0_64 & 0xffffffff)) /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (n_v_argv_1 /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_2 /*0x1*/);
				regs.rdi = UINT64 (0x400878);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"y < 8"*/ (const char *) 0x400878);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				if (cond_7 (n_v_argv_1_4)) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					/*Registers Changes*/
					regs.rdi = UINT64 (0x400885);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"y > 2"*/ (const char *) 0x400885);
					const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
					const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
										func_2 (regs, rbp_0, rdi_0, rsp_0);
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					/*Registers Changes*/
					regs.rdi = UINT64 (0x40088b);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"y <= 2"*/ (const char *) 0x40088b);
					const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
					const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
										func_2 (regs, rbp_0, rdi_0, rsp_0);
				}
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((n_v_argv_1_2 << 0x8) | n_v_argv_1_1) << 0x10) | ((n_v_argv_1_0 << 0x8) | m7fffffffe250_0_8))) /*0x7fff46000040*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_1_4) - 0x30) << 0x20) & 0xffffffff00000000) | (m7fffffffe258_0_64 & 0xffffffff)) /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (n_v_argv_1 /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_2 /*0x1*/);
				regs.rdi = UINT64 (0x40087e);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"y >= 8"*/ (const char *) 0x40087e);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				{
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					/*Registers Changes*/
					regs.rdi = UINT64 (0x400885);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"y > 2"*/ (const char *) 0x400885);
					const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
					const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
										func_2 (regs, rbp_0, rdi_0, rsp_0);
				}
			}
		} else {
			if (cond_17 (n_v_argv_1_4)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((n_v_argv_1_2 << 0x8) | n_v_argv_1_1) << 0x10) | ((n_v_argv_1_0 << 0x8) | m7fffffffe250_0_8))) /*0x7fff46000040*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (((m7fffffffe258_0_64 & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_1_4) - 0x30) & 0xffffffff)) /*0x7*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (n_v_argv_1 /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_2 /*0x0*/);
				regs.rdi = UINT64 (0x400871);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"y >= 4"*/ (const char *) 0x400871);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
								func_2 (regs, rbp_0, rdi_0, rsp_0);
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((n_v_argv_1_2 << 0x8) | n_v_argv_1_1) << 0x10) | ((n_v_argv_1_0 << 0x8) | m7fffffffe250_0_8))) /*0x7fff46000040*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (((m7fffffffe258_0_64 & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_1_4) - 0x30) & 0xffffffff)) /*0x7*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (n_v_argv_1 /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_2 /*0x0*/);
				regs.rdi = UINT64 (0x40086b);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"y < 4"*/ (const char *) 0x40086b);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
								func_2 (regs, rbp_0, rdi_0, rsp_0);
			}
		}
	}
}
int func_4 (struct RegistersSet &regs, UINT8 m7fffffffe250_0_8, UINT64 m7fffffffe258_0_64, UINT64 n_v_argv_1, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_1_3, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0);
bool cond_27 (UINT8 n_v_argv_1_3) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ < /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_3) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x6)*//*}*/);
}

int func_5 (struct RegistersSet &regs, UINT64 m7fffffffe358_2_64, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0);
int func_5 (struct RegistersSet &regs, UINT64 m7fffffffe358_2_64, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe358) = UINT64 (m7fffffffe358_2_64 /*0x7fffffffe58c*/);
		/*Registers Changes*/
		regs.rax = UINT64 (m7fffffffe358_2_64 /*0x7fffffffe58c*/);
		regs.rdi = UINT64 (m7fffffffe358_2_64 /*0x7fffffffe58c*/);
		regs.rsi = UINT64 (0x400839);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = strcmp (/*"right"*/ (const char *) m7fffffffe358_2_64 /*0x7fffffffe58c*/, /*"left"*/ (const char *) 0x400839);
		const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
		const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
		if (((rax_3 & 0xffffffff) /*0x6*/ != 0)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (0x400848);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rax = puts (/*"right-path"*/ (const char *) 0x400848);
			const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
			const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
						func_2 (regs, rbp_0, rdi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_3 /*0x0*/);
			regs.rdi = UINT64 (0x40083e);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rax = puts (/*"left-path"*/ (const char *) 0x40083e);
			const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
			const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
						func_2 (regs, rbp_0, rdi_0, rsp_0);
		}
	}
}
int func_4 (struct RegistersSet &regs, UINT8 m7fffffffe250_0_8, UINT64 m7fffffffe258_0_64, UINT64 n_v_argv_1, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_1_3, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0) {
if (cond_27 (n_v_argv_1_3)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
		*((UINT64 *) 0x7fffffffe250) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_1_3) - 0x30) << 0x20) & 0xffffffff00000000) | (((((n_v_argv_1_2 << 0x8) | n_v_argv_1_1) << 0x10) | ((n_v_argv_1_0 << 0x8) | m7fffffffe250_0_8)) & 0xffffffff)) /*0x46004c40*/);
		*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (n_v_argv_1 /*0x7fffffffe586*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_1_3) - 0x30) & 0xffffffff) /*0x6*/);
		regs.rdi = UINT64 (0x40082c);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = puts (/*"x > 5"*/ (const char *) 0x40082c);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe358_2_64 = *((UINT64 *) 0x7fffffffe358);
				func_5 (regs, m7fffffffe358_2_64, rbp_0, rdi_0, rsi_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
		*((UINT64 *) 0x7fffffffe250) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_1_3) - 0x30) << 0x20) & 0xffffffff00000000) | (((((n_v_argv_1_2 << 0x8) | n_v_argv_1_1) << 0x10) | ((n_v_argv_1_0 << 0x8) | m7fffffffe250_0_8)) & 0xffffffff)) /*0x46004c40*/);
		*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (n_v_argv_1 /*0x7fffffffe586*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_1_3) - 0x30) & 0xffffffff) /*0x6*/);
		regs.rdi = UINT64 (0x400832);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = puts (/*"x <= 5"*/ (const char *) 0x400832);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe358_2_64 = *((UINT64 *) 0x7fffffffe358);
				func_5 (regs, m7fffffffe358_2_64, rbp_0, rdi_0, rsi_0, rsp_0);
	}
}
int func_6 (struct RegistersSet &regs, UINT64 m7fffffffe358_1_64, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0);
int func_6 (struct RegistersSet &regs, UINT64 m7fffffffe358_1_64, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe358) = UINT64 (m7fffffffe358_1_64 /*0x7fffffffe58c*/);
		/*Registers Changes*/
		regs.rax = UINT64 (m7fffffffe358_1_64 /*0x7fffffffe58c*/);
		regs.rdi = UINT64 (m7fffffffe358_1_64 /*0x7fffffffe58c*/);
		regs.rsi = UINT64 (0x400839);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = strcmp (/*"right"*/ (const char *) m7fffffffe358_1_64 /*0x7fffffffe58c*/, /*"left"*/ (const char *) 0x400839);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe358_2_64 = *((UINT64 *) 0x7fffffffe358);
		if (((rax_2 & 0xffffffff) /*0x6*/ != 0)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (0x400848);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rax = puts (/*"right-path"*/ (const char *) 0x400848);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
						func_2 (regs, rbp_0, rdi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_2 /*0x0*/);
			regs.rdi = UINT64 (0x40083e);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rax = puts (/*"left-path"*/ (const char *) 0x40083e);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
						func_2 (regs, rbp_0, rdi_0, rsp_0);
		}
	}
}
int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	const UINT32 m7fffffffe248_0_32 = *((UINT32 *) 0x7fffffffe248);
	const UINT8 m7fffffffe250_0_8 = *((UINT8 *) 0x7fffffffe250);
	const UINT32 m7fffffffe254_0_32 = *((UINT32 *) 0x7fffffffe254);
	const UINT64 m7fffffffe258_0_64 = *((UINT64 *) 0x7fffffffe258);
	const UINT64 m7fffffffe268_0_64 = *((UINT64 *) 0x7fffffffe268);
	if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x4c)/*}*/ != /*unsigned {*/UINT64 (argv[1][0]) /*0x52*//*}*/)) {
		if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x55)/*}*/ != /*unsigned {*/UINT64 (argv[1][1]) /*0x44*//*}*/)) {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ == /*unsigned {*/UINT64 (argv[1][2]) /*0x46*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe248_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((argv[1][2] << 0x8) | argv[1][1]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe250_0_8))) /*0x7fff46000040*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (argv[1][2] /*0x46*/);
				regs.rdi = UINT64 (0x40085f);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"R/D/F"*/ (const char *) 0x40085f);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe358_1_64 = *((UINT64 *) 0x7fffffffe358);
								func_1 (regs, UINT64 (m7fffffffe250_0_8), UINT64 (m7fffffffe254_0_32), UINT64 (m7fffffffe258_0_64), UINT64 (m7fffffffe358_1_64), UINT64 (argv[1]), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[1][4]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe248_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((argv[1][2] << 0x8) | argv[1][1]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe250_0_8))) /*0x7fff00000040*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (argv[1][2] /*0x0*/);
				regs.rdi = UINT64 (0x400865);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"R/D/B"*/ (const char *) 0x400865);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe358_1_64 = *((UINT64 *) 0x7fffffffe358);
								func_1 (regs, UINT64 (m7fffffffe250_0_8), UINT64 (m7fffffffe254_0_32), UINT64 (m7fffffffe258_0_64), UINT64 (m7fffffffe358_1_64), UINT64 (argv[1]), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[1][4]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
			}
		} else {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ == /*unsigned {*/UINT64 (argv[1][2]) /*0x46*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe248_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((argv[1][2] << 0x8) | argv[1][1]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe250_0_8))) /*0x7fff46550040*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (argv[1][2] /*0x46*/);
				regs.rdi = UINT64 (0x400853);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"R/U/F"*/ (const char *) 0x400853);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe358_1_64 = *((UINT64 *) 0x7fffffffe358);
								func_1 (regs, UINT64 (m7fffffffe250_0_8), UINT64 (m7fffffffe254_0_32), UINT64 (m7fffffffe258_0_64), UINT64 (m7fffffffe358_1_64), UINT64 (argv[1]), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[1][4]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe248_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((argv[1][2] << 0x8) | argv[1][1]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe250_0_8))) /*0x7fff00550040*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (argv[1][2] /*0x0*/);
				regs.rdi = UINT64 (0x400859);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"R/U/B"*/ (const char *) 0x400859);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe358_1_64 = *((UINT64 *) 0x7fffffffe358);
								func_1 (regs, UINT64 (m7fffffffe250_0_8), UINT64 (m7fffffffe254_0_32), UINT64 (m7fffffffe258_0_64), UINT64 (m7fffffffe358_1_64), UINT64 (argv[1]), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[1][4]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
			}
		}
	} else {
		if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x55)/*}*/ != /*unsigned {*/UINT64 (argv[1][1]) /*0x44*//*}*/)) {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ == /*unsigned {*/UINT64 (argv[1][2]) /*0x46*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe248_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((argv[1][2] << 0x8) | argv[1][1]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe250_0_8))) /*0x7fff46004c40*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (argv[1][2] /*0x46*/);
				regs.rdi = UINT64 (0x400820);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"L/D/F"*/ (const char *) 0x400820);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe358_1_64 = *((UINT64 *) 0x7fffffffe358);
								func_4 (regs, UINT64 (m7fffffffe250_0_8), UINT64 (m7fffffffe258_0_64), UINT64 (argv[1]), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[1][3]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe248_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((argv[1][2] << 0x8) | argv[1][1]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe250_0_8))) /*0x7fff00004c40*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (argv[1][2] /*0x0*/);
				regs.rdi = UINT64 (0x400826);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"L/D/B"*/ (const char *) 0x400826);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe358_1_64 = *((UINT64 *) 0x7fffffffe358);
								func_4 (regs, UINT64 (m7fffffffe250_0_8), UINT64 (m7fffffffe258_0_64), UINT64 (argv[1]), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[1][3]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
			}
		} else {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ == /*unsigned {*/UINT64 (argv[1][2]) /*0x46*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe248_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((argv[1][2] << 0x8) | argv[1][1]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe250_0_8))) /*0x7fff46554c40*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (argv[1][2] /*0x46*/);
				regs.rdi = UINT64 (0x400814);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"L/U/F"*/ (const char *) 0x400814);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe358_1_64 = *((UINT64 *) 0x7fffffffe358);
								func_6 (regs, UINT64 (m7fffffffe358_1_64), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rsi_0 /*0x7fffffffe348*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe248_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe254_0_32 << 0x20) | ((((argv[1][2] << 0x8) | argv[1][1]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe250_0_8))) /*0x7fff00554c40*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe586*/);
				/*Registers Changes*/
				regs.rax = UINT64 (argv[1][2] /*0x0*/);
				regs.rdi = UINT64 (0x40081a);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"L/U/B"*/ (const char *) 0x40081a);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe358_1_64 = *((UINT64 *) 0x7fffffffe358);
								func_6 (regs, UINT64 (m7fffffffe358_1_64), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
			}
		}
	}
}
