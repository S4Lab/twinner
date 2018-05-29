
#include "twincode.h"

bool cond_5 (UINT8 n_v_argv_1_0) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_0) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x1)*//*}*/);
}

bool cond_6 (UINT8 n_v_argv_1_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/);
}

bool cond_7 (UINT8 n_v_argv_1_2) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_2) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x3)*//*}*/);
}

int func_1 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 ((rsp_0 & 0xffffffffffffffff) /*0x7fffffffe2a8*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
		return int (regs.rax);
	}
}
int func_1 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4007f0);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
		regs.rax = puts (/*"multi-path program with printf-leafs"*/ (const char *) 0x4007f0);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
				func_2 (regs, rbp_0, rsp_0);
	}
}
bool cond_14 (UINT8 n_v_argv_1_2) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_2) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x3)*//*}*/);
}

bool cond_17 (UINT8 n_v_argv_1_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/);
}

bool cond_18 (UINT8 n_v_argv_1_2) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_2) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x3)*//*}*/);
}

bool cond_21 (UINT8 n_v_argv_1_2) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_1_2) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x3)*//*}*/);
}

int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	const UINT64 m7fffffffe280_0_64 = *((UINT64 *) 0x7fffffffe280);
	const UINT32 m7fffffffe288_0_32 = *((UINT32 *) 0x7fffffffe288);
	const UINT32 m7fffffffe290_0_32 = *((UINT32 *) 0x7fffffffe290);
	const UINT64 m7fffffffe298_0_64 = *((UINT64 *) 0x7fffffffe298);
	const UINT64 m7fffffffe2a8_0_64 = *((UINT64 *) 0x7fffffffe2a8);
	if ((((UINT64 (rsp_0) + 0xffff800000001d58) & 0xffffffffffffffff) /*0x0*/ == 0)) {
		if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x1)/*}*/ < /*signed {*/signExtend_0x80_0x20 ((UINT64 (rdi_0) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/)) {
			if ((((UINT64 (rsi_0) + 0xffff800000001c78) & 0xffffffffffffffff) /*0x0*/ == 0)) {
				if ((((UINT64 (argv[1]) + 0xffff800000001a48) & 0xffffffffffffffff) /*0x0*/ == 0)) {
					if (cond_5 (UINT64 (argv[1][0]))) {
						if (cond_6 (UINT64 (argv[1][1]))) {
							if (cond_7 (UINT64 (argv[1][2]))) {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
								*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x200400440*/);
								*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x1ffffe380*/);
								*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
								*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b8*/);
								/*Registers Changes*/
								regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
								regs.rdi = UINT64 (0x4007c8);
								regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
								regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
								regs.rax = puts (/*"a <= 5, b <= 5, c <= 5 -- case 8"*/ (const char *) 0x4007c8);
								const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
																func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							} else {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
								*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
								*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x1ffffe380*/);
								*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
								*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b8*/);
								/*Registers Changes*/
								regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
								regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
								regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
								regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
								regs.rax = puts (/*"a <= 5, b <= 5, c > 5 -- case 7"*/ (const char *) 0x4007a8);
								const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
																func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							}
						} else {
							if (cond_14 (UINT64 (argv[1][2]))) {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
								*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
								*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x1ffffe380*/);
								*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
								*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b8*/);
								/*Registers Changes*/
								regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
								regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
								regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
								regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
								regs.rax = puts (/*"a <= 5, b > 5, c <= 5 -- case 6"*/ (const char *) 0x400788);
								const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
																func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							} else {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
								*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
								*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x1ffffe380*/);
								*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
								*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b8*/);
								/*Registers Changes*/
								regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
								regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
								regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
								regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
								regs.rax = puts (/*"a <= 5, b > 5, c > 5 -- case 5"*/ (const char *) 0x400768);
								const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
																func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							}
						}
					} else {
						if (cond_17 (UINT64 (argv[1][1]))) {
							if (cond_18 (UINT64 (argv[1][2]))) {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
								*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
								*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x1ffffe380*/);
								*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
								*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b8*/);
								/*Registers Changes*/
								regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
								regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
								regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
								regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
								regs.rax = puts (/*"a > 5, b <= 5, c <= 5 -- case 4"*/ (const char *) 0x400748);
								const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
																func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							} else {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
								*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
								*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x1ffffe380*/);
								*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
								*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b8*/);
								/*Registers Changes*/
								regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
								regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
								regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
								regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
								regs.rax = puts (/*"a > 5, b <= 5, c > 5 -- case 3"*/ (const char *) 0x400728);
								const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
																func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							}
						} else {
							if (cond_21 (UINT64 (argv[1][2]))) {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
								*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
								*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x1ffffe380*/);
								*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
								*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b8*/);
								/*Registers Changes*/
								regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
								regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
								regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
								regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
								regs.rax = puts (/*"a > 5, b > 5, c <= 5 -- case 2"*/ (const char *) 0x400708);
								const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
																func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							} else {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
								*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
								*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (argv[1][0]) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x1ffffe380*/);
								*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
								*((UINT64 *) 0x7fffffffe390) = UINT64 (argv[1] /*0x7fffffffe5b8*/);
								/*Registers Changes*/
								regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
								regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
								regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
								regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
								regs.rax = puts (/*"a > 5, b > 5, c > 5 -- case 1"*/ (const char *) 0x4006e7);
								const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
								const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
																func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							}
						}
					}
				} else {
				}
			} else {
			}
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x400640*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x400440*/);
			*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ff3f45*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (rdi_0 /*0x0*/);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
			regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
			regs.rax = puts (/*"Usage: program <command codes>"*/ (const char *) 0x4006c8);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
						func_2 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
		}
	} else {
	}
}