
#include "twincode.h"

int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe290_0_64, UINT16 m7fffffffe298_0_16, UINT32 m7fffffffe29c_0_32, UINT64 n_v_argv_1, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_1_3, UINT8 n_v_argv_1_4, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 m7fffffffe290_0_64, UINT16 m7fffffffe298_0_16, UINT32 m7fffffffe29c_0_32, UINT64 n_v_argv_1, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_1_3, UINT8 n_v_argv_1_4, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int func_2 (struct RegistersSet &regs, UINT64 m7fffffffe290_0_64, UINT16 m7fffffffe298_0_16, UINT32 m7fffffffe29c_0_32, UINT64 n_v_argv_1, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_1_3, UINT8 n_v_argv_1_4, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ == /*unsigned {*/n_v_argv_1_4 /*0x52*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
		*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
		*((UINT64 *) 0x7fffffffe298) = UINT64 (((((((n_v_argv_1_4 << 0x8) | (logicalShiftToRight (m7fffffffe29c_0_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff)) << 0x10) | (((n_v_argv_1_3 << 0x8) | n_v_argv_1_2) & 0xffff)) << 0x20) | (((((n_v_argv_1_1 << 0x8) | n_v_argv_1_0) << 0x10) | m7fffffffe298_0_16) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe390) = UINT64 (n_v_argv_1 /*0x7fffffffe5b7*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_1_4 /*0x52*/);
		regs.rdi = UINT64 (0x400864);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
		regs.rax = puts (/*"right-path last diamond"*/ (const char *) 0x400864);
		const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
		const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				func_3 (regs, rbp_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
		*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
		*((UINT64 *) 0x7fffffffe298) = UINT64 (((((((n_v_argv_1_4 << 0x8) | (logicalShiftToRight (m7fffffffe29c_0_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff)) << 0x10) | (((n_v_argv_1_3 << 0x8) | n_v_argv_1_2) & 0xffff)) << 0x20) | (((((n_v_argv_1_1 << 0x8) | n_v_argv_1_0) << 0x10) | m7fffffffe298_0_16) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe390) = UINT64 (n_v_argv_1 /*0x7fffffffe5b7*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_1_4 /*0x52*/);
		regs.rdi = UINT64 (0x40087c);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
		regs.rax = puts (/*"left-path last diamond"*/ (const char *) 0x40087c);
		const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
		const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				func_3 (regs, rbp_0, rsp_0);
	}
}
int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe290_0_64, UINT16 m7fffffffe298_0_16, UINT32 m7fffffffe29c_0_32, UINT64 n_v_argv_1, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_1_3, UINT8 n_v_argv_1_4, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ == /*unsigned {*/n_v_argv_1_1 /*0x52*//*}*/)) {
		if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ == /*unsigned {*/n_v_argv_1_2 /*0x52*//*}*/)) {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ == /*unsigned {*/n_v_argv_1_3 /*0x52*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
				*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
				*((UINT64 *) 0x7fffffffe298) = UINT64 (((((m7fffffffe29c_0_32 & 0xffff0000) | ((n_v_argv_1_3 << 0x8) | n_v_argv_1_2)) << 0x20) | ((((n_v_argv_1_1 << 0x8) | n_v_argv_1_0) << 0x10) | m7fffffffe298_0_16)) /*0x0*/);
				*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe390) = UINT64 (n_v_argv_1 /*0x7fffffffe5b7*/);
				/*Registers Changes*/
				regs.rax = UINT64 (n_v_argv_1_3 /*0x52*/);
				regs.rdi = UINT64 (0x4007e4);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
				regs.rax = puts (/*"right-path in the RR diamond"*/ (const char *) 0x4007e4);
				const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
				const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
								func_2 (regs, m7fffffffe290_0_64, m7fffffffe298_0_16, m7fffffffe29c_0_32, n_v_argv_1, n_v_argv_1_0, n_v_argv_1_1, n_v_argv_1_2, n_v_argv_1_3, n_v_argv_1_4, rbp_0, rsi_0, rsp_0);
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
				*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
				*((UINT64 *) 0x7fffffffe298) = UINT64 (((((m7fffffffe29c_0_32 & 0xffff0000) | ((n_v_argv_1_3 << 0x8) | n_v_argv_1_2)) << 0x20) | ((((n_v_argv_1_1 << 0x8) | n_v_argv_1_0) << 0x10) | m7fffffffe298_0_16)) /*0x0*/);
				*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe390) = UINT64 (n_v_argv_1 /*0x7fffffffe5b7*/);
				/*Registers Changes*/
				regs.rax = UINT64 (n_v_argv_1_3 /*0x52*/);
				regs.rdi = UINT64 (0x400801);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
				regs.rax = puts (/*"left-path in the RR diamond"*/ (const char *) 0x400801);
				const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
				const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
								func_2 (regs, m7fffffffe290_0_64, m7fffffffe298_0_16, m7fffffffe29c_0_32, n_v_argv_1, n_v_argv_1_0, n_v_argv_1_1, n_v_argv_1_2, n_v_argv_1_3, n_v_argv_1_4, rbp_0, rsi_0, rsp_0);
			}
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (((((m7fffffffe29c_0_32 & 0xffff0000) | ((n_v_argv_1_3 << 0x8) | n_v_argv_1_2)) << 0x20) | ((((n_v_argv_1_1 << 0x8) | n_v_argv_1_0) << 0x10) | m7fffffffe298_0_16)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe390) = UINT64 (n_v_argv_1 /*0x7fffffffe5b7*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_1_3 /*0x52*/);
			regs.rdi = UINT64 (0x40081d);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
			regs.rax = puts (/*"the RL scenario"*/ (const char *) 0x40081d);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
						func_2 (regs, m7fffffffe290_0_64, m7fffffffe298_0_16, m7fffffffe29c_0_32, n_v_argv_1, n_v_argv_1_0, n_v_argv_1_1, n_v_argv_1_2, n_v_argv_1_3, n_v_argv_1_4, rbp_0, rsi_0, rsp_0);
		}
	} else {
		if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ == /*unsigned {*/n_v_argv_1_2 /*0x52*//*}*/)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (((((((logicalShiftToRight (m7fffffffe29c_0_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff00) | n_v_argv_1_2) << 0x10) | (m7fffffffe29c_0_32 & 0xffff)) << 0x20) | ((((n_v_argv_1_1 << 0x8) | n_v_argv_1_0) << 0x10) | m7fffffffe298_0_16)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe390) = UINT64 (n_v_argv_1 /*0x7fffffffe5b7*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_1_2 /*0x52*/);
			regs.rdi = UINT64 (0x40082d);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
			regs.rax = puts (/*"right-path in the L diamond"*/ (const char *) 0x40082d);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
						func_2 (regs, m7fffffffe290_0_64, m7fffffffe298_0_16, m7fffffffe29c_0_32, n_v_argv_1, n_v_argv_1_0, n_v_argv_1_1, n_v_argv_1_2, n_v_argv_1_3, n_v_argv_1_4, rbp_0, rsi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (((((((logicalShiftToRight (m7fffffffe29c_0_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff00) | n_v_argv_1_2) << 0x10) | (m7fffffffe29c_0_32 & 0xffff)) << 0x20) | ((((n_v_argv_1_1 << 0x8) | n_v_argv_1_0) << 0x10) | m7fffffffe298_0_16)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe390) = UINT64 (n_v_argv_1 /*0x7fffffffe5b7*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_1_2 /*0x52*/);
			regs.rdi = UINT64 (0x400849);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
			regs.rax = puts (/*"left-path in the L diamond"*/ (const char *) 0x400849);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
						func_2 (regs, m7fffffffe290_0_64, m7fffffffe298_0_16, m7fffffffe29c_0_32, n_v_argv_1, n_v_argv_1_0, n_v_argv_1_1, n_v_argv_1_2, n_v_argv_1_3, n_v_argv_1_4, rbp_0, rsi_0, rsp_0);
		}
	}
}
int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	const UINT32 m7fffffffe288_0_32 = *((UINT32 *) 0x7fffffffe288);
	const UINT64 m7fffffffe290_0_64 = *((UINT64 *) 0x7fffffffe290);
	const UINT16 m7fffffffe298_0_16 = *((UINT16 *) 0x7fffffffe298);
	const UINT32 m7fffffffe29c_0_32 = *((UINT32 *) 0x7fffffffe29c);
	const UINT64 m7fffffffe2a8_0_64 = *((UINT64 *) 0x7fffffffe2a8);
	if ((/*unsigned {*/(UINT64 (rdi_0) & 0xffffffff) /*0x2*//*}*/ == /*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x2)/*}*/)) {
		if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ == /*unsigned {*/UINT64 (argv[1][0]) /*0x52*//*}*/)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x200400440*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (((m7fffffffe29c_0_32 << 0x20) | ((((argv[1][1] << 0x8) | argv[1][0]) << 0x10) | m7fffffffe298_0_16)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b7*/);
			/*Registers Changes*/
			regs.rax = UINT64 (argv[1][1] /*0x52*/);
			regs.rdi = UINT64 (rdi_0 /*0x2*/);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
			regs.rax = puts (/*"right-path first diamond"*/ (const char *) 0x4007b3);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
						func_1 (regs, UINT64 (m7fffffffe290_0_64), UINT64 (m7fffffffe298_0_16), UINT64 (m7fffffffe29c_0_32), UINT64 (argv[1]), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[1][3]), UINT64 (argv[1][4]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x200400440*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (m7fffffffe290_0_64 /*0x7fffffffe380*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (((m7fffffffe29c_0_32 << 0x20) | ((((argv[1][1] << 0x8) | argv[1][0]) << 0x10) | m7fffffffe298_0_16)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b7*/);
			/*Registers Changes*/
			regs.rax = UINT64 (argv[1][1] /*0x52*/);
			regs.rdi = UINT64 (rdi_0 /*0x2*/);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
			regs.rax = puts (/*"left-path first diamond"*/ (const char *) 0x4007cc);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
						func_1 (regs, UINT64 (m7fffffffe290_0_64), UINT64 (m7fffffffe298_0_16), UINT64 (m7fffffffe29c_0_32), UINT64 (argv[1]), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[1][3]), UINT64 (argv[1][4]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
		}
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe280) = UINT64 (rsi_0 /*0x7fffffffe388*/);
		*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x400440*/);
		*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5feaf45*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (rdi_0 /*0x0*/);
		regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
		regs.rax = puts (/*"Usage: program xyzuw"*/ (const char *) 0x400718);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
		const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
		{
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (0x40072d);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
			regs.rax = puts (/*"\tx=R|L first diamond"*/ (const char *) 0x40072d);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
			{
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
				/*Registers Changes*/
				regs.rdi = UINT64 (0x400748);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
				regs.rax = puts (/*"\t\tyz=RR > u=R|L the RR diamond"*/ (const char *) 0x400748);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				{
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
					/*Registers Changes*/
					regs.rdi = UINT64 (0x400767);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
					regs.rax = puts (/*"\t\tyz=RL > RL scenario"*/ (const char *) 0x400767);
					const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
					const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
					{
						/*Memory Changes*/
						*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
						/*Registers Changes*/
						regs.rdi = UINT64 (0x40077d);
						regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
						regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
						regs.rax = puts (/*"\t\ty=L > z=R|L the L diamond"*/ (const char *) 0x40077d);
						const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
						const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
						{
							/*Memory Changes*/
							*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
							/*Registers Changes*/
							regs.rdi = UINT64 (0x400799);
							regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
							regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
							regs.rax = puts (/*"\t\t\tw=R|L the last diamond"*/ (const char *) 0x400799);
							const UINT64 rax_6 = regs.rax, rbx_6 = regs.rbx, rcx_6 = regs.rcx, rdx_6 = regs.rdx, rdi_6 = regs.rdi, rsi_6 = regs.rsi, rsp_6 = regs.rsp, rbp_6 = regs.rbp, r8_6 = regs.r8, r9_6 = regs.r9, r10_6 = regs.r10, r11_6 = regs.r11, r12_6 = regs.r12, r13_6 = regs.r13, r14_6 = regs.r14, r15_6 = regs.r15;
							const UINT128 xmm0_6 = UINT128 (regs.xmm0), xmm1_6 = UINT128 (regs.xmm1), xmm2_6 = UINT128 (regs.xmm2), xmm3_6 = UINT128 (regs.xmm3), xmm4_6 = UINT128 (regs.xmm4), xmm5_6 = UINT128 (regs.xmm5), xmm6_6 = UINT128 (regs.xmm6), xmm7_6 = UINT128 (regs.xmm7), xmm8_6 = UINT128 (regs.xmm8), xmm9_6 = UINT128 (regs.xmm9), xmm10_6 = UINT128 (regs.xmm10), xmm11_6 = UINT128 (regs.xmm11), xmm12_6 = UINT128 (regs.xmm12), xmm13_6 = UINT128 (regs.xmm13), xmm14_6 = UINT128 (regs.xmm14), xmm15_6 = UINT128 (regs.xmm15);
														func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
						}
					}
				}
			}
		}
	}
}
