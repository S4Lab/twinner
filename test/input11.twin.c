
#include "twincode.h"

bool cond_1 (UINT8 n_v_argv_1_0) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_0) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x1)*//*}*/);
}

bool cond_2 (UINT8 n_v_argv_1_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x6)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/);
}

int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe288_0_64, UINT64 m7fffffffe388_1_64, UINT8 n_v_argv_1_1, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_3_0, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
bool cond_3 (UINT8 n_v_argv_2_0) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_0) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x3)*//*}*/);
}

bool cond_4 (UINT8 n_v_argv_2_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x6)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x4)*//*}*/);
}

int func_2 (struct RegistersSet &regs, UINT64 m7fffffffe390_2_64, UINT8 n_v_argv_1_1, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_3_0, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe298*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int func_2 (struct RegistersSet &regs, UINT64 m7fffffffe390_2_64, UINT8 n_v_argv_1_1, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_3_0, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*unsigned {*/((signExtend_0x80_0x8 (n_v_argv_3_0) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff) /*UINT128 (0x0, 0x0, 0x0, 0x5)*//*}*/ != /*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
		*((UINT64 *) 0x7fffffffe280) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_1_1) - 0x30) & 0xffffffff)) /*0x2*/);
		*((UINT64 *) 0x7fffffffe288) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_3_0) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff)) /*0x4*/);
		*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe390) = UINT64 (m7fffffffe390_2_64 /*0x7fffffffe5b5*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_3_0) - 0x30) & 0xffffffff) /*0x5*/);
		regs.rdi = UINT64 (0x400763);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
		regs.rax = puts (/*"u != 3"*/ (const char *) 0x400763);
		const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
		const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				func_3 (regs, rbp_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
		*((UINT64 *) 0x7fffffffe280) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_1_1) - 0x30) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe288) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_3_0) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff)) /*0x4*/);
		*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe390) = UINT64 (m7fffffffe390_2_64 /*0x7fffffffe5b5*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_3_0) - 0x30) & 0xffffffff) /*0x5*/);
		regs.rdi = UINT64 (0x40075c);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
		regs.rax = puts (/*"u == 3"*/ (const char *) 0x40075c);
		const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
		const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				func_3 (regs, rbp_0, rsp_0);
	}
}
bool cond_11 (UINT8 n_v_argv_2_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x6)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x4)*//*}*/);
}

int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe288_0_64, UINT64 m7fffffffe388_1_64, UINT8 n_v_argv_1_1, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_3_0, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if (cond_3 (n_v_argv_2_0)) {
		if (cond_4 (n_v_argv_2_1)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
			*((UINT64 *) 0x7fffffffe280) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_1_1) - 0x30) & 0xffffffff)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 (((m7fffffffe288_0_64 & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff)) /*0x4*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe388) = UINT64 (m7fffffffe388_1_64 /*0x7fffffffe5b2*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff) /*0x4*/);
			regs.rdi = UINT64 (0x400720);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
			regs.rax = puts (/*"z < 4 | w < 7"*/ (const char *) 0x400720);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe390_2_64 = *((UINT64 *) 0x7fffffffe390);
						func_2 (regs, m7fffffffe390_2_64, n_v_argv_1_1, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_3_0, rbp_0, rsi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
			*((UINT64 *) 0x7fffffffe280) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_1_1) - 0x30) & 0xffffffff)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 (((m7fffffffe288_0_64 & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff)) /*0x4*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe388) = UINT64 (m7fffffffe388_1_64 /*0x7fffffffe5b2*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff) /*0x4*/);
			regs.rdi = UINT64 (0x40072e);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
			regs.rax = puts (/*"z < 4 | w >= 7"*/ (const char *) 0x40072e);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe390_2_64 = *((UINT64 *) 0x7fffffffe390);
						func_2 (regs, m7fffffffe390_2_64, n_v_argv_1_1, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_3_0, rbp_0, rsi_0, rsp_0);
		}
	} else {
		if (cond_11 (n_v_argv_2_1)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
			*((UINT64 *) 0x7fffffffe280) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_1_1) - 0x30) & 0xffffffff)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 (((m7fffffffe288_0_64 & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff)) /*0x4*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe388) = UINT64 (m7fffffffe388_1_64 /*0x7fffffffe5b2*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff) /*0x4*/);
			regs.rdi = UINT64 (0x40073d);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
			regs.rax = puts (/*"z >= 4 | w < 7"*/ (const char *) 0x40073d);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe390_2_64 = *((UINT64 *) 0x7fffffffe390);
						func_2 (regs, m7fffffffe390_2_64, n_v_argv_1_1, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_3_0, rbp_0, rsi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
			*((UINT64 *) 0x7fffffffe280) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_1_1) - 0x30) & 0xffffffff)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 (((m7fffffffe288_0_64 & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff)) /*0x4*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe388) = UINT64 (m7fffffffe388_1_64 /*0x7fffffffe5b2*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff) /*0x4*/);
			regs.rdi = UINT64 (0x40074c);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
			regs.rax = puts (/*"z >= 4 | w >= 7"*/ (const char *) 0x40074c);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe390_2_64 = *((UINT64 *) 0x7fffffffe390);
						func_2 (regs, m7fffffffe390_2_64, n_v_argv_1_1, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_3_0, rbp_0, rsi_0, rsp_0);
		}
	}
}
bool cond_15 (UINT8 n_v_argv_1_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x6)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/);
}

int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	const UINT32 m7fffffffe268_0_32 = *((UINT32 *) 0x7fffffffe268);
	const UINT64 m7fffffffe270_0_64 = *((UINT64 *) 0x7fffffffe270);
	const UINT32 m7fffffffe278_0_32 = *((UINT32 *) 0x7fffffffe278);
	const UINT32 m7fffffffe284_0_32 = *((UINT32 *) 0x7fffffffe284);
	const UINT64 m7fffffffe288_0_64 = *((UINT64 *) 0x7fffffffe288);
	const UINT64 m7fffffffe298_0_64 = *((UINT64 *) 0x7fffffffe298);
	if (cond_1 (UINT64 (argv[1][0]))) {
		if (cond_2 (UINT64 (argv[1][1]))) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
			*((UINT64 *) 0x7fffffffe268) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe268_0_32) /*0x400000000*/);
			*((UINT64 *) 0x7fffffffe270) = UINT64 (m7fffffffe270_0_64 /*0x400660*/);
			*((UINT64 *) 0x7fffffffe278) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe278_0_32) /*0x400440*/);
			*((UINT64 *) 0x7fffffffe280) = UINT64 (((m7fffffffe284_0_32 << 0x20) | ((signExtend_0x20_0x8 (argv[1][1]) - 0x30) & 0xffffffff)) /*0x7fff00000000*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 (m7fffffffe288_0_64 /*0x0*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe380) = UINT64 (argv[1] /*0x7fffffffe5af*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (argv[1][1]) - 0x30) & 0xffffffff) /*0x0*/);
			regs.rdi = UINT64 (0x4006e4);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe378*/);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
			regs.rax = puts (/*"x < 4 | y < 7"*/ (const char *) 0x4006e4);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe388_1_64 = *((UINT64 *) 0x7fffffffe388);
						func_1 (regs, UINT64 (m7fffffffe288_0_64), UINT64 (m7fffffffe388_1_64), UINT64 (argv[1][1]), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
			*((UINT64 *) 0x7fffffffe268) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe268_0_32) /*0x400000000*/);
			*((UINT64 *) 0x7fffffffe270) = UINT64 (m7fffffffe270_0_64 /*0x400660*/);
			*((UINT64 *) 0x7fffffffe278) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe278_0_32) /*0x400440*/);
			*((UINT64 *) 0x7fffffffe280) = UINT64 (((m7fffffffe284_0_32 << 0x20) | ((signExtend_0x20_0x8 (argv[1][1]) - 0x30) & 0xffffffff)) /*0x7fff00000040*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 (m7fffffffe288_0_64 /*0x0*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe380) = UINT64 (argv[1] /*0x7fffffffe5af*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (argv[1][1]) - 0x30) & 0xffffffff) /*0x40*/);
			regs.rdi = UINT64 (0x4006f2);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe378*/);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
			regs.rax = puts (/*"x < 4 | y >= 7"*/ (const char *) 0x4006f2);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe388_1_64 = *((UINT64 *) 0x7fffffffe388);
						func_1 (regs, UINT64 (m7fffffffe288_0_64), UINT64 (m7fffffffe388_1_64), UINT64 (argv[1][1]), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
		}
	} else {
		if (cond_15 (UINT64 (argv[1][1]))) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
			*((UINT64 *) 0x7fffffffe268) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe268_0_32) /*0x400000000*/);
			*((UINT64 *) 0x7fffffffe270) = UINT64 (m7fffffffe270_0_64 /*0x400660*/);
			*((UINT64 *) 0x7fffffffe278) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe278_0_32) /*0x4000400440*/);
			*((UINT64 *) 0x7fffffffe280) = UINT64 (((m7fffffffe284_0_32 << 0x20) | ((signExtend_0x20_0x8 (argv[1][1]) - 0x30) & 0xffffffff)) /*0x7fff00000002*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 (m7fffffffe288_0_64 /*0x0*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe380) = UINT64 (argv[1] /*0x7fffffffe5af*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (argv[1][1]) - 0x30) & 0xffffffff) /*0x2*/);
			regs.rdi = UINT64 (0x400701);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe378*/);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
			regs.rax = puts (/*"x >= 4 | y < 7"*/ (const char *) 0x400701);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe388_1_64 = *((UINT64 *) 0x7fffffffe388);
						func_1 (regs, UINT64 (m7fffffffe288_0_64), UINT64 (m7fffffffe388_1_64), UINT64 (argv[1][1]), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rsi_0 /*0x7fffffffe378*/);
			*((UINT64 *) 0x7fffffffe268) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe268_0_32) /*0x400000000*/);
			*((UINT64 *) 0x7fffffffe270) = UINT64 (m7fffffffe270_0_64 /*0x400660*/);
			*((UINT64 *) 0x7fffffffe278) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe278_0_32) /*0x4000400440*/);
			*((UINT64 *) 0x7fffffffe280) = UINT64 (((m7fffffffe284_0_32 << 0x20) | ((signExtend_0x20_0x8 (argv[1][1]) - 0x30) & 0xffffffff)) /*0x7fff00000040*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 (m7fffffffe288_0_64 /*0x0*/);
			*((UINT64 *) 0x7fffffffe290) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe380) = UINT64 (argv[1] /*0x7fffffffe5af*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (argv[1][1]) - 0x30) & 0xffffffff) /*0x40*/);
			regs.rdi = UINT64 (0x400710);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe378*/);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe290*/);
			regs.rax = puts (/*"x >= 4 | y >= 7"*/ (const char *) 0x400710);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe388_1_64 = *((UINT64 *) 0x7fffffffe388);
						func_1 (regs, UINT64 (m7fffffffe288_0_64), UINT64 (m7fffffffe388_1_64), UINT64 (argv[1][1]), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
		}
	}
}
