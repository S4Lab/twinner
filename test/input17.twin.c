
#include "twincode.h"

int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe338_1_64, UINT64 m7fffffffe340_2_64, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_3_0, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rax_5, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe338_1_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe348_4_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_4 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rax_5, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_5 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_6 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_7 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_7 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe248*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int func_6 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ != /*unsigned {*/n_v_argv_5_2 /*0x53*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((((n_v_argv_5_2 << 0x8) | n_v_argv_5_1) << 0x10) | (((n_v_argv_5_0 << 0x8) | ((n_v_argv_2_4 - 0x30) & 0xff)) & 0xffff)) << 0x20) | (((((n_v_argv_2_2 << 0x8) | n_v_argv_2_1) << 0x10) | (((n_v_argv_2_0 << 0x8) | m7fffffffe238_1_8) & 0xffff)) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (m7fffffffe350_5_64 /*0x7fffffffe587*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_5_2 /*0x53*/);
		regs.rdi = UINT64 (0x40082d);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"S3"*/ (const char *) 0x40082d);
		const UINT64 rax_8 = regs.rax, rbx_8 = regs.rbx, rcx_8 = regs.rcx, rdx_8 = regs.rdx, rdi_8 = regs.rdi, rsi_8 = regs.rsi, rsp_8 = regs.rsp, rbp_8 = regs.rbp, r8_8 = regs.r8, r9_8 = regs.r9, r10_8 = regs.r10, r11_8 = regs.r11, r12_8 = regs.r12, r13_8 = regs.r13, r14_8 = regs.r14, r15_8 = regs.r15;
		const UINT128 xmm0_8 = UINT128 (regs.xmm0), xmm1_8 = UINT128 (regs.xmm1), xmm2_8 = UINT128 (regs.xmm2), xmm3_8 = UINT128 (regs.xmm3), xmm4_8 = UINT128 (regs.xmm4), xmm5_8 = UINT128 (regs.xmm5), xmm6_8 = UINT128 (regs.xmm6), xmm7_8 = UINT128 (regs.xmm7), xmm8_8 = UINT128 (regs.xmm8), xmm9_8 = UINT128 (regs.xmm9), xmm10_8 = UINT128 (regs.xmm10), xmm11_8 = UINT128 (regs.xmm11), xmm12_8 = UINT128 (regs.xmm12), xmm13_8 = UINT128 (regs.xmm13), xmm14_8 = UINT128 (regs.xmm14), xmm15_8 = UINT128 (regs.xmm15);
				func_7 (regs, rbp_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((((n_v_argv_5_2 << 0x8) | n_v_argv_5_1) << 0x10) | (((n_v_argv_5_0 << 0x8) | ((n_v_argv_2_4 - 0x30) & 0xff)) & 0xffff)) << 0x20) | (((((n_v_argv_2_2 << 0x8) | n_v_argv_2_1) << 0x10) | (((n_v_argv_2_0 << 0x8) | m7fffffffe238_1_8) & 0xffff)) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (m7fffffffe350_5_64 /*0x7fffffffe587*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_5_2 /*0x46*/);
		regs.rdi = UINT64 (0x400830);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"F3"*/ (const char *) 0x400830);
		const UINT64 rax_8 = regs.rax, rbx_8 = regs.rbx, rcx_8 = regs.rcx, rdx_8 = regs.rdx, rdi_8 = regs.rdi, rsi_8 = regs.rsi, rsp_8 = regs.rsp, rbp_8 = regs.rbp, r8_8 = regs.r8, r9_8 = regs.r9, r10_8 = regs.r10, r11_8 = regs.r11, r12_8 = regs.r12, r13_8 = regs.r13, r14_8 = regs.r14, r15_8 = regs.r15;
		const UINT128 xmm0_8 = UINT128 (regs.xmm0), xmm1_8 = UINT128 (regs.xmm1), xmm2_8 = UINT128 (regs.xmm2), xmm3_8 = UINT128 (regs.xmm3), xmm4_8 = UINT128 (regs.xmm4), xmm5_8 = UINT128 (regs.xmm5), xmm6_8 = UINT128 (regs.xmm6), xmm7_8 = UINT128 (regs.xmm7), xmm8_8 = UINT128 (regs.xmm8), xmm9_8 = UINT128 (regs.xmm9), xmm10_8 = UINT128 (regs.xmm10), xmm11_8 = UINT128 (regs.xmm11), xmm12_8 = UINT128 (regs.xmm12), xmm13_8 = UINT128 (regs.xmm13), xmm14_8 = UINT128 (regs.xmm14), xmm15_8 = UINT128 (regs.xmm15);
				func_7 (regs, rbp_0, rsp_0);
	}
}
int func_5 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x53)/*}*/ == /*unsigned {*/n_v_argv_5_1 /*0x53*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((((logicalShiftToRight (m7fffffffe23c_1_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff00) | n_v_argv_5_1) << 0x10) | (((n_v_argv_5_0 << 0x8) | ((n_v_argv_2_4 - 0x30) & 0xff)) & 0xffff)) << 0x20) | (((((n_v_argv_2_2 << 0x8) | n_v_argv_2_1) << 0x10) | (((n_v_argv_2_0 << 0x8) | m7fffffffe238_1_8) & 0xffff)) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (m7fffffffe350_5_64 /*0x7fffffffe587*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_5_1 /*0x53*/);
		regs.rdi = UINT64 (0x400827);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"S2"*/ (const char *) 0x400827);
		const UINT64 rax_7 = regs.rax, rbx_7 = regs.rbx, rcx_7 = regs.rcx, rdx_7 = regs.rdx, rdi_7 = regs.rdi, rsi_7 = regs.rsi, rsp_7 = regs.rsp, rbp_7 = regs.rbp, r8_7 = regs.r8, r9_7 = regs.r9, r10_7 = regs.r10, r11_7 = regs.r11, r12_7 = regs.r12, r13_7 = regs.r13, r14_7 = regs.r14, r15_7 = regs.r15;
		const UINT128 xmm0_7 = UINT128 (regs.xmm0), xmm1_7 = UINT128 (regs.xmm1), xmm2_7 = UINT128 (regs.xmm2), xmm3_7 = UINT128 (regs.xmm3), xmm4_7 = UINT128 (regs.xmm4), xmm5_7 = UINT128 (regs.xmm5), xmm6_7 = UINT128 (regs.xmm6), xmm7_7 = UINT128 (regs.xmm7), xmm8_7 = UINT128 (regs.xmm8), xmm9_7 = UINT128 (regs.xmm9), xmm10_7 = UINT128 (regs.xmm10), xmm11_7 = UINT128 (regs.xmm11), xmm12_7 = UINT128 (regs.xmm12), xmm13_7 = UINT128 (regs.xmm13), xmm14_7 = UINT128 (regs.xmm14), xmm15_7 = UINT128 (regs.xmm15);
				func_6 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((((logicalShiftToRight (m7fffffffe23c_1_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff00) | n_v_argv_5_1) << 0x10) | (((n_v_argv_5_0 << 0x8) | ((n_v_argv_2_4 - 0x30) & 0xff)) & 0xffff)) << 0x20) | (((((n_v_argv_2_2 << 0x8) | n_v_argv_2_1) << 0x10) | (((n_v_argv_2_0 << 0x8) | m7fffffffe238_1_8) & 0xffff)) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (m7fffffffe350_5_64 /*0x7fffffffe587*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_5_1 /*0x0*/);
		regs.rdi = UINT64 (0x40082a);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"F2"*/ (const char *) 0x40082a);
		const UINT64 rax_7 = regs.rax, rbx_7 = regs.rbx, rcx_7 = regs.rcx, rdx_7 = regs.rdx, rdi_7 = regs.rdi, rsi_7 = regs.rsi, rsp_7 = regs.rsp, rbp_7 = regs.rbp, r8_7 = regs.r8, r9_7 = regs.r9, r10_7 = regs.r10, r11_7 = regs.r11, r12_7 = regs.r12, r13_7 = regs.r13, r14_7 = regs.r14, r15_7 = regs.r15;
		const UINT128 xmm0_7 = UINT128 (regs.xmm0), xmm1_7 = UINT128 (regs.xmm1), xmm2_7 = UINT128 (regs.xmm2), xmm3_7 = UINT128 (regs.xmm3), xmm4_7 = UINT128 (regs.xmm4), xmm5_7 = UINT128 (regs.xmm5), xmm6_7 = UINT128 (regs.xmm6), xmm7_7 = UINT128 (regs.xmm7), xmm8_7 = UINT128 (regs.xmm8), xmm9_7 = UINT128 (regs.xmm9), xmm10_7 = UINT128 (regs.xmm10), xmm11_7 = UINT128 (regs.xmm11), xmm12_7 = UINT128 (regs.xmm12), xmm13_7 = UINT128 (regs.xmm13), xmm14_7 = UINT128 (regs.xmm14), xmm15_7 = UINT128 (regs.xmm15);
				func_6 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
	}
}
int func_4 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rax_5, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x53)/*}*/ == /*unsigned {*/n_v_argv_5_0 /*0x53*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((m7fffffffe23c_1_32 & 0xffff0000) | ((n_v_argv_5_0 << 0x8) | ((n_v_argv_2_4 - 0x30) & 0xff))) << 0x20) | (((((n_v_argv_2_2 << 0x8) | n_v_argv_2_1) << 0x10) | (((n_v_argv_2_0 << 0x8) | m7fffffffe238_1_8) & 0xffff)) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (m7fffffffe350_5_64 /*0x7fffffffe587*/);
		/*Registers Changes*/
		regs.rax = UINT64 (rax_5 /*0x0*/);
		regs.rdi = UINT64 (0x400809);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"slow"*/ (const char *) 0x400809);
		const UINT64 rax_6 = regs.rax, rbx_6 = regs.rbx, rcx_6 = regs.rcx, rdx_6 = regs.rdx, rdi_6 = regs.rdi, rsi_6 = regs.rsi, rsp_6 = regs.rsp, rbp_6 = regs.rbp, r8_6 = regs.r8, r9_6 = regs.r9, r10_6 = regs.r10, r11_6 = regs.r11, r12_6 = regs.r12, r13_6 = regs.r13, r14_6 = regs.r14, r15_6 = regs.r15;
		const UINT128 xmm0_6 = UINT128 (regs.xmm0), xmm1_6 = UINT128 (regs.xmm1), xmm2_6 = UINT128 (regs.xmm2), xmm3_6 = UINT128 (regs.xmm3), xmm4_6 = UINT128 (regs.xmm4), xmm5_6 = UINT128 (regs.xmm5), xmm6_6 = UINT128 (regs.xmm6), xmm7_6 = UINT128 (regs.xmm7), xmm8_6 = UINT128 (regs.xmm8), xmm9_6 = UINT128 (regs.xmm9), xmm10_6 = UINT128 (regs.xmm10), xmm11_6 = UINT128 (regs.xmm11), xmm12_6 = UINT128 (regs.xmm12), xmm13_6 = UINT128 (regs.xmm13), xmm14_6 = UINT128 (regs.xmm14), xmm15_6 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe350_6_64 = *((UINT64 *) 0x7fffffffe350);
				func_5 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((m7fffffffe23c_1_32 & 0xffff0000) | ((n_v_argv_5_0 << 0x8) | ((n_v_argv_2_4 - 0x30) & 0xff))) << 0x20) | (((((n_v_argv_2_2 << 0x8) | n_v_argv_2_1) << 0x10) | (((n_v_argv_2_0 << 0x8) | m7fffffffe238_1_8) & 0xffff)) & 0xffffffff)) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe350) = UINT64 (m7fffffffe350_5_64 /*0x7fffffffe587*/);
		/*Registers Changes*/
		regs.rax = UINT64 (rax_5 /*0x0*/);
		regs.rdi = UINT64 (0x40080e);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"fast"*/ (const char *) 0x40080e);
		const UINT64 rax_6 = regs.rax, rbx_6 = regs.rbx, rcx_6 = regs.rcx, rdx_6 = regs.rdx, rdi_6 = regs.rdi, rsi_6 = regs.rsi, rsp_6 = regs.rsp, rbp_6 = regs.rbp, r8_6 = regs.r8, r9_6 = regs.r9, r10_6 = regs.r10, r11_6 = regs.r11, r12_6 = regs.r12, r13_6 = regs.r13, r14_6 = regs.r14, r15_6 = regs.r15;
		const UINT128 xmm0_6 = UINT128 (regs.xmm0), xmm1_6 = UINT128 (regs.xmm1), xmm2_6 = UINT128 (regs.xmm2), xmm3_6 = UINT128 (regs.xmm3), xmm4_6 = UINT128 (regs.xmm4), xmm5_6 = UINT128 (regs.xmm5), xmm6_6 = UINT128 (regs.xmm6), xmm7_6 = UINT128 (regs.xmm7), xmm8_6 = UINT128 (regs.xmm8), xmm9_6 = UINT128 (regs.xmm9), xmm10_6 = UINT128 (regs.xmm10), xmm11_6 = UINT128 (regs.xmm11), xmm12_6 = UINT128 (regs.xmm12), xmm13_6 = UINT128 (regs.xmm13), xmm14_6 = UINT128 (regs.xmm14), xmm15_6 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe350_6_64 = *((UINT64 *) 0x7fffffffe350);
				func_5 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
	}
}
int func_3 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe348_4_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_4_64 /*0x7fffffffe57c*/);
		/*Registers Changes*/
		regs.rax = UINT64 (m7fffffffe348_4_64 /*0x7fffffffe57c*/);
		regs.rdi = UINT64 (m7fffffffe348_4_64 /*0x7fffffffe57c*/);
		regs.rsi = UINT64 (0x4007fe);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = strcmp (/*"magicvalue"*/ (const char *) m7fffffffe348_4_64 /*0x7fffffffe57c*/, /*"magicvalue"*/ (const char *) 0x4007fe);
		const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
		const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe350_5_64 = *((UINT64 *) 0x7fffffffe350);
		if (((rax_5 & 0xffffffff) /*0x0*/ == 0)) {
						func_4 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rax_5, rbp_0, rsi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
			*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_5 /*0x1*/);
			regs.rdi = UINT64 (0x400813);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"magic code is wrong"*/ (const char *) 0x400813);
			const UINT64 rax_6 = regs.rax, rbx_6 = regs.rbx, rcx_6 = regs.rcx, rdx_6 = regs.rdx, rdi_6 = regs.rdi, rsi_6 = regs.rsi, rsp_6 = regs.rsp, rbp_6 = regs.rbp, r8_6 = regs.r8, r9_6 = regs.r9, r10_6 = regs.r10, r11_6 = regs.r11, r12_6 = regs.r12, r13_6 = regs.r13, r14_6 = regs.r14, r15_6 = regs.r15;
			const UINT128 xmm0_6 = UINT128 (regs.xmm0), xmm1_6 = UINT128 (regs.xmm1), xmm2_6 = UINT128 (regs.xmm2), xmm3_6 = UINT128 (regs.xmm3), xmm4_6 = UINT128 (regs.xmm4), xmm5_6 = UINT128 (regs.xmm5), xmm6_6 = UINT128 (regs.xmm6), xmm7_6 = UINT128 (regs.xmm7), xmm8_6 = UINT128 (regs.xmm8), xmm9_6 = UINT128 (regs.xmm9), xmm10_6 = UINT128 (regs.xmm10), xmm11_6 = UINT128 (regs.xmm11), xmm12_6 = UINT128 (regs.xmm12), xmm13_6 = UINT128 (regs.xmm13), xmm14_6 = UINT128 (regs.xmm14), xmm15_6 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe350_6_64 = *((UINT64 *) 0x7fffffffe350);
						func_5 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
		}
	}
}
int func_2 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe338_1_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x2)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (((n_v_argv_2_4 + 0xd0) & 0xff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((m7fffffffe23c_1_32 & 0xffff0000) | ((m7fffffffe23c_1_32 & 0xff00) | ((n_v_argv_2_4 - 0x30) & 0xff))) << 0x20) | ((((n_v_argv_2_2 << 0x8) | n_v_argv_2_1) << 0x10) | (((n_v_argv_2_0 << 0x8) | m7fffffffe238_1_8) & 0xffff))) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe574*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((n_v_argv_2_4 - 0x30) & 0xffffffff) /*0x0*/);
		regs.rdi = UINT64 (0x4007ea);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"x <= 2"*/ (const char *) 0x4007ea);
		const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
		const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe230_4_64 = *((UINT64 *) 0x7fffffffe230);
		const UINT32 m7fffffffe238_4_32 = *((UINT32 *) 0x7fffffffe238);
		const UINT16 m7fffffffe23c_4_16 = *((UINT16 *) 0x7fffffffe23c);
		const UINT8 m7fffffffe23f_4_8 = *((UINT8 *) 0x7fffffffe23f);
		const UINT64 m7fffffffe348_4_64 = *((UINT64 *) 0x7fffffffe348);
		const UINT64 m7fffffffe350_4_64 = *((UINT64 *) 0x7fffffffe350);
				func_3 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe348_4_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((m7fffffffe23c_1_32 & 0xffff0000) | ((m7fffffffe23c_1_32 & 0xff00) | ((n_v_argv_2_4 - 0x30) & 0xff))) << 0x20) | ((((n_v_argv_2_2 << 0x8) | n_v_argv_2_1) << 0x10) | (((n_v_argv_2_0 << 0x8) | m7fffffffe238_1_8) & 0xffff))) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe574*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((n_v_argv_2_4 - 0x30) & 0xffffffff) /*0x40*/);
		regs.rdi = UINT64 (0x4007e4);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"x > 2"*/ (const char *) 0x4007e4);
		const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
		const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe230_4_64 = *((UINT64 *) 0x7fffffffe230);
		const UINT32 m7fffffffe238_4_32 = *((UINT32 *) 0x7fffffffe238);
		const UINT16 m7fffffffe23c_4_16 = *((UINT16 *) 0x7fffffffe23c);
		const UINT8 m7fffffffe23f_4_8 = *((UINT8 *) 0x7fffffffe23f);
		const UINT64 m7fffffffe348_4_64 = *((UINT64 *) 0x7fffffffe348);
		const UINT64 m7fffffffe350_4_64 = *((UINT64 *) 0x7fffffffe350);
				func_3 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe348_4_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
	}
}
int func_8 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe348_3_64, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rax_5, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_8 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe348_3_64, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rax_5, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_3_64 /*0x7fffffffe57c*/);
		/*Registers Changes*/
		regs.rax = UINT64 (m7fffffffe348_3_64 /*0x7fffffffe57c*/);
		regs.rdi = UINT64 (m7fffffffe348_3_64 /*0x7fffffffe57c*/);
		regs.rsi = UINT64 (0x4007fe);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = strcmp (/*"magicvalue"*/ (const char *) m7fffffffe348_3_64 /*0x7fffffffe57c*/, /*"magicvalue"*/ (const char *) 0x4007fe);
		const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
		const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe230_4_64 = *((UINT64 *) 0x7fffffffe230);
		const UINT32 m7fffffffe238_4_32 = *((UINT32 *) 0x7fffffffe238);
		const UINT16 m7fffffffe23c_4_16 = *((UINT16 *) 0x7fffffffe23c);
		const UINT8 m7fffffffe23f_4_8 = *((UINT8 *) 0x7fffffffe23f);
		const UINT64 m7fffffffe348_4_64 = *((UINT64 *) 0x7fffffffe348);
		const UINT64 m7fffffffe350_4_64 = *((UINT64 *) 0x7fffffffe350);
		if (((rax_4 & 0xffffffff) /*0x0*/ == 0)) {
						func_4 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rax_5, rbp_0, rsi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
			*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_4 /*0x1*/);
			regs.rdi = UINT64 (0x400813);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"magic code is wrong"*/ (const char *) 0x400813);
			const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
			const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe350_5_64 = *((UINT64 *) 0x7fffffffe350);
						func_5 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
		}
	}
}
int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe230_1_64, UINT8 m7fffffffe238_1_8, UINT32 m7fffffffe23c_1_32, UINT64 m7fffffffe338_1_64, UINT64 m7fffffffe340_2_64, UINT64 m7fffffffe350_5_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_2_2, UINT8 n_v_argv_2_4, UINT8 n_v_argv_3_0, UINT8 n_v_argv_5_0, UINT8 n_v_argv_5_1, UINT8 n_v_argv_5_2, UINT64 rax_5, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ != /*unsigned {*/n_v_argv_3_0 /*0x4c*//*}*/)) {
		if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x55)/*}*/ == /*unsigned {*/n_v_argv_2_2 /*0x55*//*}*/)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
			*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe574*/);
			*((UINT64 *) 0x7fffffffe340) = UINT64 (m7fffffffe340_2_64 /*0x7fffffffe57a*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_3_0 /*0x0*/);
			regs.rdi = UINT64 (0x4007dc);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"up"*/ (const char *) 0x4007dc);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe230_3_64 = *((UINT64 *) 0x7fffffffe230);
			const UINT32 m7fffffffe238_3_32 = *((UINT32 *) 0x7fffffffe238);
			const UINT8 m7fffffffe23c_3_8 = *((UINT8 *) 0x7fffffffe23c);
			const UINT16 m7fffffffe23e_3_16 = *((UINT16 *) 0x7fffffffe23e);
			const UINT64 m7fffffffe348_3_64 = *((UINT64 *) 0x7fffffffe348);
			const UINT64 m7fffffffe350_3_64 = *((UINT64 *) 0x7fffffffe350);
						func_2 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe338_1_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
			*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe574*/);
			*((UINT64 *) 0x7fffffffe340) = UINT64 (m7fffffffe340_2_64 /*0x7fffffffe57a*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_3_0 /*0x0*/);
			regs.rdi = UINT64 (0x4007df);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"down"*/ (const char *) 0x4007df);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe230_3_64 = *((UINT64 *) 0x7fffffffe230);
			const UINT32 m7fffffffe238_3_32 = *((UINT32 *) 0x7fffffffe238);
			const UINT8 m7fffffffe23c_3_8 = *((UINT8 *) 0x7fffffffe23c);
			const UINT16 m7fffffffe23e_3_16 = *((UINT16 *) 0x7fffffffe23e);
			const UINT64 m7fffffffe348_3_64 = *((UINT64 *) 0x7fffffffe348);
			const UINT64 m7fffffffe350_3_64 = *((UINT64 *) 0x7fffffffe350);
						func_2 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe338_1_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rbp_0, rsi_0, rsp_0);
		}
	} else {
		if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x55)/*}*/ == /*unsigned {*/n_v_argv_2_1 /*0x55*//*}*/)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
			*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe340) = UINT64 (m7fffffffe340_2_64 /*0x7fffffffe57a*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_3_0 /*0x52*/);
			regs.rdi = UINT64 (0x4007dc);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"up"*/ (const char *) 0x4007dc);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe230_3_64 = *((UINT64 *) 0x7fffffffe230);
			const UINT32 m7fffffffe238_3_32 = *((UINT32 *) 0x7fffffffe238);
			const UINT8 m7fffffffe23c_3_8 = *((UINT8 *) 0x7fffffffe23c);
			const UINT16 m7fffffffe23e_3_16 = *((UINT16 *) 0x7fffffffe23e);
			const UINT64 m7fffffffe348_3_64 = *((UINT64 *) 0x7fffffffe348);
			const UINT64 m7fffffffe350_3_64 = *((UINT64 *) 0x7fffffffe350);
						func_8 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe348_3_64, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rax_5, rbp_0, rsi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
			*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe340) = UINT64 (m7fffffffe340_2_64 /*0x7fffffffe57a*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_3_0 /*0x52*/);
			regs.rdi = UINT64 (0x4007df);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"down"*/ (const char *) 0x4007df);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe230_3_64 = *((UINT64 *) 0x7fffffffe230);
			const UINT32 m7fffffffe238_3_32 = *((UINT32 *) 0x7fffffffe238);
			const UINT8 m7fffffffe23c_3_8 = *((UINT8 *) 0x7fffffffe23c);
			const UINT16 m7fffffffe23e_3_16 = *((UINT16 *) 0x7fffffffe23e);
			const UINT64 m7fffffffe348_3_64 = *((UINT64 *) 0x7fffffffe348);
			const UINT64 m7fffffffe350_3_64 = *((UINT64 *) 0x7fffffffe350);
						func_8 (regs, m7fffffffe230_1_64, m7fffffffe238_1_8, m7fffffffe23c_1_32, m7fffffffe348_3_64, m7fffffffe350_5_64, n_v_argv_2_0, n_v_argv_2_1, n_v_argv_2_2, n_v_argv_2_4, n_v_argv_5_0, n_v_argv_5_1, n_v_argv_5_2, rax_5, rbp_0, rsi_0, rsp_0);
		}
	}
}
int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	*((UINT16 *) 0x7fffffffe23e) = 0x0;
	*((UINT8 *) 0x7fffffffe23f) = 0x0;
	const UINT32 m7fffffffe228_0_32 = *((UINT32 *) 0x7fffffffe228);
	const UINT64 m7fffffffe248_0_64 = *((UINT64 *) 0x7fffffffe248);
	const UINT64 m7fffffffe330_0_64 = *((UINT64 *) 0x7fffffffe330);
	{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe228) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe228_0_32) /*0x600400490*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_0_64 /*0x7ffff5feaf45*/);
		*((UINT64 *) 0x7fffffffe330) = UINT64 (argv[1] /*0x7fffffffe56c*/);
		/*Registers Changes*/
		regs.rax = UINT64 (argv[1] /*0x7fffffffe56c*/);
		regs.rdi = UINT64 (argv[1] /*0x7fffffffe56c*/);
		regs.rsi = UINT64 (0x4007d4);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = strcmp (/*"verbose"*/ (const char *) m7fffffffe330_0_64 /*0x7fffffffe56c*/, /*"verbose"*/ (const char *) 0x4007d4);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
		const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe230_1_64 = *((UINT64 *) 0x7fffffffe230);
		const UINT8 m7fffffffe238_1_8 = *((UINT8 *) 0x7fffffffe238);
		const UINT32 m7fffffffe23c_1_32 = *((UINT32 *) 0x7fffffffe23c);
		const UINT64 m7fffffffe338_1_64 = *((UINT64 *) 0x7fffffffe338);
		if (((UINT64 (rax_1) & 0xffffffff) /*0x0*/ == 0)) {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x55)/*}*/ == /*unsigned {*/UINT64 (argv[2][0]) /*0x55*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
				*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
				*((UINT64 *) 0x7fffffffe238) = UINT64 (((m7fffffffe23c_1_32 << 0x20) | ((((argv[2][2] << 0x8) | argv[2][1]) << 0x10) | ((argv[2][0] << 0x8) | m7fffffffe238_1_8))) /*0x0*/);
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe574*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_1 /*0x0*/);
				regs.rdi = UINT64 (0x4007dc);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rax = puts (/*"up"*/ (const char *) 0x4007dc);
				const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
				const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe340_2_64 = *((UINT64 *) 0x7fffffffe340);
				const UINT64 m7fffffffe348_2_64 = *((UINT64 *) 0x7fffffffe348);
								const UINT64 m7fffffffe350_5_64 = *((UINT64 *) 0x7fffffffe350);
func_1 (regs, UINT64 (m7fffffffe230_1_64), UINT64 (m7fffffffe238_1_8), UINT64 (m7fffffffe23c_1_32), UINT64 (m7fffffffe338_1_64), UINT64 (m7fffffffe340_2_64), UINT64 (m7fffffffe350_5_64), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[2][2]), UINT64 (argv[2][4]), UINT64 (argv[3][0]), UINT64 (argv[5][0]), UINT64 (argv[5][1]), UINT64 (argv[5][2]), UINT64 (rax_2), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
				*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
				*((UINT64 *) 0x7fffffffe238) = UINT64 (((m7fffffffe23c_1_32 << 0x20) | ((((argv[2][2] << 0x8) | argv[2][1]) << 0x10) | ((argv[2][0] << 0x8) | m7fffffffe238_1_8))) /*0x0*/);
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe574*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_1 /*0x0*/);
				regs.rdi = UINT64 (0x4007df);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rax = puts (/*"down"*/ (const char *) 0x4007df);
				const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
				const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe340_2_64 = *((UINT64 *) 0x7fffffffe340);
				const UINT64 m7fffffffe348_2_64 = *((UINT64 *) 0x7fffffffe348);
								const UINT64 m7fffffffe350_5_64 = *((UINT64 *) 0x7fffffffe350);
func_1 (regs, UINT64 (m7fffffffe230_1_64), UINT64 (m7fffffffe238_1_8), UINT64 (m7fffffffe23c_1_32), UINT64 (m7fffffffe338_1_64), UINT64 (m7fffffffe340_2_64), UINT64 (m7fffffffe350_5_64), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[2][2]), UINT64 (argv[2][4]), UINT64 (argv[3][0]), UINT64 (argv[5][0]), UINT64 (argv[5][1]), UINT64 (argv[5][2]), UINT64 (rax_2), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
			}
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_1 /*0x1*/);
			regs.rdi = UINT64 (0x4007f1);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"_quiet mode_"*/ (const char *) 0x4007f1);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe340_2_64 = *((UINT64 *) 0x7fffffffe340);
			const UINT64 m7fffffffe348_2_64 = *((UINT64 *) 0x7fffffffe348);
			{
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_2_64 /*0x7fffffffe57c*/);
				/*Registers Changes*/
				regs.rax = UINT64 (m7fffffffe348_2_64 /*0x7fffffffe57c*/);
				regs.rdi = UINT64 (m7fffffffe348_2_64 /*0x7fffffffe57c*/);
				regs.rsi = UINT64 (0x4007fe);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rax = strcmp (/*"magicvalue"*/ (const char *) m7fffffffe348_2_64 /*0x7fffffffe57c*/, /*"magicvalue"*/ (const char *) 0x4007fe);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe230_3_64 = *((UINT64 *) 0x7fffffffe230);
				const UINT32 m7fffffffe238_3_32 = *((UINT32 *) 0x7fffffffe238);
				const UINT8 m7fffffffe23c_3_8 = *((UINT8 *) 0x7fffffffe23c);
				const UINT16 m7fffffffe23e_3_16 = *((UINT16 *) 0x7fffffffe23e);
				const UINT64 m7fffffffe348_3_64 = *((UINT64 *) 0x7fffffffe348);
				const UINT64 m7fffffffe350_3_64 = *((UINT64 *) 0x7fffffffe350);
				if (((UINT64 (rax_3) & 0xffffffff) /*0x0*/ == 0)) {
										func_4 (regs, UINT64 (m7fffffffe230_1_64), UINT64 (m7fffffffe238_1_8), UINT64 (m7fffffffe23c_1_32), UINT64 (m7fffffffe350_3_64), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[2][2]), UINT64 (argv[2][4]), UINT64 (argv[5][0]), UINT64 (argv[5][1]), UINT64 (argv[5][2]), UINT64 (rax_3), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_3 /*0x1*/);
					regs.rdi = UINT64 (0x400813);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
					regs.rax = puts (/*"magic code is wrong"*/ (const char *) 0x400813);
					const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
					const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
					const UINT64 m7fffffffe230_4_64 = *((UINT64 *) 0x7fffffffe230);
					const UINT32 m7fffffffe238_4_32 = *((UINT32 *) 0x7fffffffe238);
					const UINT16 m7fffffffe23c_4_16 = *((UINT16 *) 0x7fffffffe23c);
					const UINT8 m7fffffffe23f_4_8 = *((UINT8 *) 0x7fffffffe23f);
					const UINT64 m7fffffffe348_4_64 = *((UINT64 *) 0x7fffffffe348);
					const UINT64 m7fffffffe350_4_64 = *((UINT64 *) 0x7fffffffe350);
										func_5 (regs, UINT64 (m7fffffffe230_1_64), UINT64 (m7fffffffe238_1_8), UINT64 (m7fffffffe23c_1_32), UINT64 (m7fffffffe350_4_64), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[2][2]), UINT64 (argv[2][4]), UINT64 (argv[5][0]), UINT64 (argv[5][1]), UINT64 (argv[5][2]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
				}
			}
		}
	}
}
