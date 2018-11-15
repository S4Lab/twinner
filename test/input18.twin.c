
#include "twincode.h"

bool cond_1 (UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1) {
	return (/*signed {*/signExtend_0x80_0x8 (((n_v_argv_1_0 + 0xd0) & 0xff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/ >= /*signed {*/signExtend_0x80_0x8 (((n_v_argv_1_1 + 0xd0) & 0xff)) /*UINT128 (0x0, 0x0, 0x0, 0x5)*//*}*/);
}

bool cond_2 (UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_2) {
	return (/*signed {*/signExtend_0x80_0x8 (((n_v_argv_1_2 + 0xd0) & 0xff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/ < /*signed {*/signExtend_0x80_0x8 (((n_v_argv_1_0 + 0xd0) & 0xff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/);
}

int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe260_0_64, UINT16 m7fffffffe268_0_16, UINT8 m7fffffffe26a_0_8, UINT32 m7fffffffe26c_0_32, UINT64 m7fffffffe368_1_64, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_3_0, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe278*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe260_0_64, UINT16 m7fffffffe268_0_16, UINT8 m7fffffffe26a_0_8, UINT32 m7fffffffe26c_0_32, UINT64 m7fffffffe368_1_64, UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_1, UINT8 n_v_argv_1_2, UINT8 n_v_argv_3_0, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
		*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe368) = UINT64 (m7fffffffe368_1_64 /*0x7fffffffe598*/);
		/*Registers Changes*/
		regs.rax = UINT64 (m7fffffffe368_1_64 /*0x7fffffffe598*/);
		regs.rdi = UINT64 (m7fffffffe368_1_64 /*0x7fffffffe598*/);
		regs.rsi = UINT64 (0x4007bf);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
		regs.rax = strcmp (/*"slow"*/ (const char *) m7fffffffe368_1_64 /*0x7fffffffe598*/, /*"slow"*/ (const char *) 0x4007bf);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe370_2_64 = *((UINT64 *) 0x7fffffffe370);
		if (((rax_2 & 0xffffffff) /*0x0*/ == 0)) {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x55)/*}*/ == /*unsigned {*/n_v_argv_3_0 /*0x55*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (((((((logicalShiftToRight (m7fffffffe26c_0_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff00) | n_v_argv_3_0) << 0x10) | (((((n_v_argv_1_2 - 0x30) << 0x8) & 0xff00) | ((n_v_argv_1_1 - 0x30) & 0xff)) & 0xffff)) << 0x20) | (((((((n_v_argv_1_0 - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16) & 0xffffffff)) /*0x0*/);
				*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe370) = UINT64 (m7fffffffe370_2_64 /*0x7fffffffe59d*/);
				/*Registers Changes*/
				regs.rax = UINT64 (n_v_argv_3_0 /*0x55*/);
				regs.rdi = UINT64 (0x4007c4);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
				regs.rax = puts (/*"slow up"*/ (const char *) 0x4007c4);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
								func_2 (regs, rbp_0, rsp_0);
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (((((((logicalShiftToRight (m7fffffffe26c_0_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff00) | n_v_argv_3_0) << 0x10) | (((((n_v_argv_1_2 - 0x30) << 0x8) & 0xff00) | ((n_v_argv_1_1 - 0x30) & 0xff)) & 0xffff)) << 0x20) | (((((((n_v_argv_1_0 - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16) & 0xffffffff)) /*0x0*/);
				*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe370) = UINT64 (m7fffffffe370_2_64 /*0x7fffffffe59d*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_2 /*0x0*/);
				regs.rdi = UINT64 (0x4007cc);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
				regs.rax = puts (/*"slow down"*/ (const char *) 0x4007cc);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
								func_2 (regs, rbp_0, rsp_0);
			}
		} else {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x55)/*}*/ == /*unsigned {*/n_v_argv_3_0 /*0x55*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (((((((n_v_argv_3_0 << 0x8) | (logicalShiftToRight (m7fffffffe26c_0_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff)) << 0x10) | (((((n_v_argv_1_2 - 0x30) << 0x8) & 0xff00) | ((n_v_argv_1_1 - 0x30) & 0xff)) & 0xffff)) << 0x20) | (((((((n_v_argv_1_0 - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16) & 0xffffffff)) /*0x0*/);
				*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe370) = UINT64 (m7fffffffe370_2_64 /*0x7fffffffe59d*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_2 /*0x1*/);
				regs.rdi = UINT64 (0x4007d6);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
				regs.rax = puts (/*"quick up"*/ (const char *) 0x4007d6);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
								func_2 (regs, rbp_0, rsp_0);
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (((((((n_v_argv_3_0 << 0x8) | (logicalShiftToRight (m7fffffffe26c_0_32, UINT128 (0x0, 0x0, 0x0, 0x10)) & 0xff)) << 0x10) | (((((n_v_argv_1_2 - 0x30) << 0x8) & 0xff00) | ((n_v_argv_1_1 - 0x30) & 0xff)) & 0xffff)) << 0x20) | (((((((n_v_argv_1_0 - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16) & 0xffffffff)) /*0x0*/);
				*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe370) = UINT64 (m7fffffffe370_2_64 /*0x7fffffffe59d*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_2 /*0x1*/);
				regs.rdi = UINT64 (0x4007df);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
				regs.rax = puts (/*"quick down"*/ (const char *) 0x4007df);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
								func_2 (regs, rbp_0, rsp_0);
			}
		}
	}
}
bool cond_16 (UINT8 n_v_argv_1_0, UINT8 n_v_argv_1_2) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x8)/*}*/ < /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (((n_v_argv_1_2 + 0xd0) & 0xff)) + signExtend_0x80_0x8 (((n_v_argv_1_0 + 0xd0) & 0xff))) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x9)*//*}*/);
}

int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	const UINT32 m7fffffffe258_0_32 = *((UINT32 *) 0x7fffffffe258);
	const UINT64 m7fffffffe260_0_64 = *((UINT64 *) 0x7fffffffe260);
	const UINT16 m7fffffffe268_0_16 = *((UINT16 *) 0x7fffffffe268);
	const UINT8 m7fffffffe26a_0_8 = *((UINT8 *) 0x7fffffffe26a);
	const UINT32 m7fffffffe26c_0_32 = *((UINT32 *) 0x7fffffffe26c);
	const UINT64 m7fffffffe278_0_64 = *((UINT64 *) 0x7fffffffe278);
	if (cond_1 (UINT64 (argv[1][0]), UINT64 (argv[1][1]))) {
		if (cond_2 (UINT64 (argv[1][0]), UINT64 (argv[1][2]))) {
			if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x6)/*}*/ < /*signed {*/signExtend_0x80_0x8 (((UINT64 (argv[1][0]) + 0xd0) & 0xff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe258_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (((((m7fffffffe26c_0_32 & 0xffff0000) | ((((argv[1][2] - 0x30) << 0x8) & 0xff00) | ((argv[1][1] - 0x30) & 0xff))) << 0x20) | ((((((argv[1][0] - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16)) /*0x5e005f000000*/);
				*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe278) = UINT64 (m7fffffffe278_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[1] /*0x7fffffffe594*/);
				/*Registers Changes*/
				regs.rax = UINT64 (((argv[1][0] - 0x30) & 0xff) /*0x5f*/);
				regs.rdi = UINT64 (0x400786);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe358*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
				regs.rax = puts (/*"z is min | x >= 7"*/ (const char *) 0x400786);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe368_1_64 = *((UINT64 *) 0x7fffffffe368);
								func_1 (regs, UINT64 (m7fffffffe260_0_64), UINT64 (m7fffffffe268_0_16), UINT64 (m7fffffffe26a_0_8), UINT64 (m7fffffffe26c_0_32), UINT64 (m7fffffffe368_1_64), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe258_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (((((m7fffffffe26c_0_32 & 0xffff0000) | ((((argv[1][2] - 0x30) << 0x8) & 0xff00) | ((argv[1][1] - 0x30) & 0xff))) << 0x20) | ((((((argv[1][0] - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16)) /*0xfe0006000000*/);
				*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe278) = UINT64 (m7fffffffe278_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[1] /*0x7fffffffe594*/);
				/*Registers Changes*/
				regs.rax = UINT64 (((argv[1][0] - 0x30) & 0xff) /*0x6*/);
				regs.rdi = UINT64 (0x400775);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe358*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
				regs.rax = puts (/*"z is min | x < 7"*/ (const char *) 0x400775);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe368_1_64 = *((UINT64 *) 0x7fffffffe368);
								func_1 (regs, UINT64 (m7fffffffe260_0_64), UINT64 (m7fffffffe268_0_16), UINT64 (m7fffffffe26a_0_8), UINT64 (m7fffffffe26c_0_32), UINT64 (m7fffffffe368_1_64), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
			}
		} else {
			if (cond_16 (UINT64 (argv[1][0]), UINT64 (argv[1][2]))) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe258_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (((((m7fffffffe26c_0_32 & 0xffff0000) | ((((argv[1][2] - 0x30) << 0x8) & 0xff00) | ((argv[1][1] - 0x30) & 0xff))) << 0x20) | ((((((argv[1][0] - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16)) /*0x76005e000000*/);
				*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe278) = UINT64 (m7fffffffe278_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[1] /*0x7fffffffe594*/);
				/*Registers Changes*/
				regs.rax = UINT64 (((argv[1][0] - 0x30) & 0xff) /*0x5e*/);
				regs.rdx = UINT64 ((signExtend_0x20_0x8 (((argv[1][0] - 0x30) & 0xff)) & 0xffffffff) /*0x5e*/);
				regs.rdi = UINT64 (0x400798);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe358*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
				regs.rax = puts (/*"x is min | x+z > 8"*/ (const char *) 0x400798);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe368_1_64 = *((UINT64 *) 0x7fffffffe368);
								func_1 (regs, UINT64 (m7fffffffe260_0_64), UINT64 (m7fffffffe268_0_16), UINT64 (m7fffffffe26a_0_8), UINT64 (m7fffffffe26c_0_32), UINT64 (m7fffffffe368_1_64), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
				*((UINT64 *) 0x7fffffffe258) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe258_0_32) /*0x400400490*/);
				*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
				*((UINT64 *) 0x7fffffffe268) = UINT64 (((((m7fffffffe26c_0_32 & 0xffff0000) | ((((argv[1][2] - 0x30) << 0x8) & 0xff00) | ((argv[1][1] - 0x30) & 0xff))) << 0x20) | ((((((argv[1][0] - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16)) /*0x70001000000*/);
				*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe278) = UINT64 (m7fffffffe278_0_64 /*0x7ffff5feaf45*/);
				*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[1] /*0x7fffffffe594*/);
				/*Registers Changes*/
				regs.rax = UINT64 (((argv[1][0] - 0x30) & 0xff) /*0x1*/);
				regs.rdx = UINT64 ((signExtend_0x20_0x8 (((argv[1][0] - 0x30) & 0xff)) & 0xffffffff) /*0x1*/);
				regs.rdi = UINT64 (0x4007ab);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe358*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
				regs.rax = puts (/*"x is min | x+z <= 8"*/ (const char *) 0x4007ab);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe368_1_64 = *((UINT64 *) 0x7fffffffe368);
								func_1 (regs, UINT64 (m7fffffffe260_0_64), UINT64 (m7fffffffe268_0_16), UINT64 (m7fffffffe26a_0_8), UINT64 (m7fffffffe26c_0_32), UINT64 (m7fffffffe368_1_64), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
			}
		}
	} else {
		if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (((UINT64 (argv[1][2]) + 0xd0) & 0xff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
			*((UINT64 *) 0x7fffffffe258) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe258_0_32) /*0x400400490*/);
			*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
			*((UINT64 *) 0x7fffffffe268) = UINT64 (((((m7fffffffe26c_0_32 & 0xffff0000) | ((((argv[1][2] - 0x30) << 0x8) & 0xff00) | ((argv[1][1] - 0x30) & 0xff))) << 0x20) | ((((((argv[1][0] - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16)) /*0x28886000000*/);
			*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe278) = UINT64 (m7fffffffe278_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[1] /*0x7fffffffe594*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((argv[1][0] - 0x30) & 0xff) /*0x86*/);
			regs.rdi = UINT64 (0x400764);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe358*/);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
			regs.rax = puts (/*"small x | z <= 5"*/ (const char *) 0x400764);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe368_1_64 = *((UINT64 *) 0x7fffffffe368);
						func_1 (regs, UINT64 (m7fffffffe260_0_64), UINT64 (m7fffffffe268_0_16), UINT64 (m7fffffffe26a_0_8), UINT64 (m7fffffffe26c_0_32), UINT64 (m7fffffffe368_1_64), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe250) = UINT64 (rsi_0 /*0x7fffffffe358*/);
			*((UINT64 *) 0x7fffffffe258) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe258_0_32) /*0x400400490*/);
			*((UINT64 *) 0x7fffffffe260) = UINT64 (m7fffffffe260_0_64 /*0x7fffffffe350*/);
			*((UINT64 *) 0x7fffffffe268) = UINT64 (((((m7fffffffe26c_0_32 & 0xffff0000) | ((((argv[1][2] - 0x30) << 0x8) & 0xff00) | ((argv[1][1] - 0x30) & 0xff))) << 0x20) | ((((((argv[1][0] - 0x30) << 0x8) & 0xff00) | m7fffffffe26a_0_8) << 0x10) | m7fffffffe268_0_16)) /*0x509190000000*/);
			*((UINT64 *) 0x7fffffffe270) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe278) = UINT64 (m7fffffffe278_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[1] /*0x7fffffffe594*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((argv[1][0] - 0x30) & 0xff) /*0x90*/);
			regs.rdi = UINT64 (0x400754);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe358*/);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe250*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe270*/);
			regs.rax = puts (/*"small x | z > 5"*/ (const char *) 0x400754);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe368_1_64 = *((UINT64 *) 0x7fffffffe368);
						func_1 (regs, UINT64 (m7fffffffe260_0_64), UINT64 (m7fffffffe268_0_16), UINT64 (m7fffffffe26a_0_8), UINT64 (m7fffffffe26c_0_32), UINT64 (m7fffffffe368_1_64), UINT64 (argv[1][0]), UINT64 (argv[1][1]), UINT64 (argv[1][2]), UINT64 (argv[3][0]), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
		}
	}
}
