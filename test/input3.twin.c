
#include "twincode.h"

bool cond_14 (UINT8 n_v_argv_2_0, UINT8 n_v_argv_4_0) {
	return (/*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_0) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x3)*//*}*/ < /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_4_0) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x5)*//*}*/);
}

int func_1 (struct RegistersSet &regs, UINT8 n_v_argv_2_0, UINT8 n_v_argv_4_0, UINT64 rbp_0, UINT64 rsp_0);
bool cond_15 (UINT8 n_v_argv_2_0, UINT8 n_v_argv_4_0) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0xf)/*}*/ >= /*signed {*/signExtend_0x80_0x20 ((((signExtend_0x80_0x8 (n_v_argv_4_0) + signExtend_0x80_0x8 (n_v_argv_2_0)) + UINT128 (0x0, 0x0, 0x0, 0xffffffa0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x8)*//*}*/);
}

int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 ((rsp_0 & 0xffffffffffffffff) /*0x7fffffffe248*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		return int (regs.rax);
	}
}
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4008c7);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"last line!"*/ (const char *) 0x4008c7);
		const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
		const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
				func_3 (regs, rbp_0, rsp_0);
	}
}
int func_1 (struct RegistersSet &regs, UINT8 n_v_argv_2_0, UINT8 n_v_argv_4_0, UINT64 rbp_0, UINT64 rsp_0) {
if (cond_15 (n_v_argv_2_0, n_v_argv_4_0)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_4_0) - 0x30) & 0xffffffff) /*0x5*/);
		regs.rdx = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) & 0xffffffff) /*0x3*/);
		regs.rdi = UINT64 (0x4008a0);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"sum of two numbers is NOT more than 15"*/ (const char *) 0x4008a0);
		const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
		const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
				func_2 (regs, rbp_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_4_0) - 0x30) & 0xffffffff) /*0x5*/);
		regs.rdx = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) & 0xffffffff) /*0x3*/);
		regs.rdi = UINT64 (0x400878);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"sum of two numbers is more than 15"*/ (const char *) 0x400878);
		const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
		const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
				func_2 (regs, rbp_0, rsp_0);
	}
}
int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	const UINT64 m7fffffffe220_0_64 = *((UINT64 *) 0x7fffffffe220);
	const UINT32 m7fffffffe228_0_32 = *((UINT32 *) 0x7fffffffe228);
	const UINT64 m7fffffffe248_0_64 = *((UINT64 *) 0x7fffffffe248);
	if ((((UINT64 (rsp_0) + 0xffff800000001db8) & 0xffffffffffffffff) /*0x0*/ == 0)) {
		if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x4)/*}*/ < /*signed {*/signExtend_0x80_0x20 ((UINT64 (rdi_0) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x5)*//*}*/)) {
			if ((((UINT64 (rsi_0) + 0xffff800000001cd8) & 0xffffffffffffffff) /*0x0*/ == 0)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe220) = UINT64 (m7fffffffe220_0_64 /*0x400720*/);
				*((UINT64 *) 0x7fffffffe228) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe228_0_32) /*0x4000000000400490*/);
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_0_64 /*0x7ffff5ff3f45*/);
				*((UINT64 *) 0x7fffffffe330) = UINT64 (argv[1] /*0x7fffffffe569*/);
				/*Registers Changes*/
				regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe330*/);
				regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe328*/);
				regs.rsp = UINT64 (rsp_0 /*0x7fffffffe248*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rax = strcmp (/*"--left"*/ (const char *) argv[1] /*0x7fffffffe569*/, /*"--left"*/ (const char *) 0x4007d4);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				const UINT64 m7fffffffe230_1_64 = *((UINT64 *) 0x7fffffffe230);
				const UINT32 m7fffffffe23c_1_32 = *((UINT32 *) 0x7fffffffe23c);
				const UINT64 m7fffffffe338_1_64 = *((UINT64 *) 0x7fffffffe338);
				const UINT64 m7fffffffe340_1_64 = *((UINT64 *) 0x7fffffffe340);
				if (((UINT64 (rax_1) & 0xffffffff) /*0x0*/ == 0)) {
					if ((((UINT64 (m7fffffffe338_1_64) + 0xffff800000001a90) & 0xffffffffffffffff) /*0x0*/ == 0)) {
						if ((UINT64 (argv[2][1]) /*0x0*/ == 0)) {
							if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x2f)/*}*/ < /*signed {*/signExtend_0x80_0x8 (UINT64 (argv[2][0])) /*UINT128 (0x0, 0x0, 0x0, 0x33)*//*}*/)) {
								if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x39)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (UINT64 (argv[2][0])) /*UINT128 (0x0, 0x0, 0x0, 0x33)*//*}*/)) {
									/*Memory Changes*/
									*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
									*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
									*((UINT64 *) 0x7fffffffe238) = UINT64 (((m7fffffffe23c_1_32 << 0x20) | ((signExtend_0x20_0x8 (argv[2][0]) - 0x30) & 0xffffffff)) /*0x3*/);
									*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
									*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe570*/);
									*((UINT64 *) 0x7fffffffe340) = UINT64 (m7fffffffe340_1_64 /*0x7fffffffe572*/);
									/*Registers Changes*/
									regs.rax = UINT64 (rax_1 /*0x0*/);
									regs.rdi = UINT64 (m7fffffffe340_1_64 /*0x7fffffffe572*/);
									regs.rsi = UINT64 (0x400807);
									regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
									regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
									regs.rax = strcmp (/*"--right"*/ (const char *) m7fffffffe340_1_64 /*0x7fffffffe572*/, /*"--right"*/ (const char *) 0x400807);
									const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
									const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
									const UINT64 m7fffffffe348_2_64 = *((UINT64 *) 0x7fffffffe348);
									if (((UINT64 (rax_2) & 0xffffffff) /*0x0*/ == 0)) {
										if ((((UINT64 (m7fffffffe348_2_64) + 0xffff800000001a86) & 0xffffffffffffffff) /*0x0*/ == 0)) {
											if ((UINT64 (argv[4][1]) /*0x0*/ == 0)) {
												if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x2f)/*}*/ < /*signed {*/signExtend_0x80_0x8 (UINT64 (argv[4][0])) /*UINT128 (0x0, 0x0, 0x0, 0x35)*//*}*/)) {
													if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x39)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (UINT64 (argv[4][0])) /*UINT128 (0x0, 0x0, 0x0, 0x35)*//*}*/)) {
														if (cond_14 (UINT64 (argv[2][0]), UINT64 (argv[4][0]))) {
															/*Memory Changes*/
															*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
															*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
															*((UINT64 *) 0x7fffffffe238) = UINT64 (((((signExtend_0x20_0x8 (argv[4][0]) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (argv[2][0]) - 0x30) & 0xffffffff)) /*0x500000003*/);
															*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
															*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_2_64 /*0x7fffffffe57a*/);
															/*Registers Changes*/
															regs.rax = UINT64 (rax_2 /*0x0*/);
															regs.rdi = UINT64 (0x40083d);
															regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
															regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
															regs.rax = puts (/*"num1 is smaller than num2"*/ (const char *) 0x40083d);
															const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
															const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
																														func_1 (regs, UINT64 (argv[2][0]), UINT64 (argv[4][0]), UINT64 (rbp_0), UINT64 (rsp_0));
														} else {
															/*Memory Changes*/
															*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
															*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_1_64 /*0x7fffffffe320*/);
															*((UINT64 *) 0x7fffffffe238) = UINT64 (((((signExtend_0x20_0x8 (argv[4][0]) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (argv[2][0]) - 0x30) & 0xffffffff)) /*0x500000003*/);
															*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
															*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_2_64 /*0x7fffffffe57a*/);
															/*Registers Changes*/
															regs.rax = UINT64 (rax_2 /*0x0*/);
															regs.rdi = UINT64 (0x400857);
															regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
															regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
															regs.rax = puts (/*"num1 is NOT smaller than num2"*/ (const char *) 0x400857);
															const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
															const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
																														func_1 (regs, UINT64 (argv[2][0]), UINT64 (argv[4][0]), UINT64 (rbp_0), UINT64 (rsp_0));
														}
													} else {
														/*Memory Changes*/
														*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
														*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
														*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_2_64 /*0x7fffffffe57a*/);
														/*Registers Changes*/
														regs.rax = UINT64 (rax_2 /*0x0*/);
														regs.rdi = UINT64 (0x400828);
														regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
														regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
														regs.rax = puts (/*"invalid right value!"*/ (const char *) 0x400828);
														const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
														const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
																												func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
													}
												} else {
													/*Memory Changes*/
													*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
													*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
													*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_2_64 /*0x7fffffffe57a*/);
													/*Registers Changes*/
													regs.rax = UINT64 (rax_2 /*0x0*/);
													regs.rdi = UINT64 (0x400828);
													regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
													regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
													regs.rax = puts (/*"invalid right value!"*/ (const char *) 0x400828);
													const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
													const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
																										func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
												}
											} else {
												/*Memory Changes*/
												*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
												*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
												*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_2_64 /*0x7fffffffe57a*/);
												/*Registers Changes*/
												regs.rax = UINT64 (rax_2 /*0x0*/);
												regs.rdi = UINT64 (0x400828);
												regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
												regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
												regs.rax = puts (/*"invalid right value!"*/ (const char *) 0x400828);
												const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
												const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
																								func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
											}
										} else {
										}
									} else {
										/*Memory Changes*/
										*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
										/*Registers Changes*/
										regs.rax = UINT64 (rax_2 /*0x1*/);
										regs.rdi = UINT64 (0x40080f);
										regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
										regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
										regs.rax = puts (/*"right option is missing!"*/ (const char *) 0x40080f);
										const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
										const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
																				func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
									}
								} else {
									/*Memory Changes*/
									*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
									*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
									*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe570*/);
									/*Registers Changes*/
									regs.rax = UINT64 (rax_1 /*0x0*/);
									regs.rdi = UINT64 (0x4007f3);
									regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
									regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
									regs.rax = puts (/*"invalid left value!"*/ (const char *) 0x4007f3);
									const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
									const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
									const UINT64 m7fffffffe348_2_64 = *((UINT64 *) 0x7fffffffe348);
																		func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
								}
							} else {
								/*Memory Changes*/
								*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
								*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
								*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe570*/);
								/*Registers Changes*/
								regs.rax = UINT64 (rax_1 /*0x0*/);
								regs.rdi = UINT64 (0x4007f3);
								regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
								regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
								regs.rax = puts (/*"invalid left value!"*/ (const char *) 0x4007f3);
								const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
								const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
								const UINT64 m7fffffffe348_2_64 = *((UINT64 *) 0x7fffffffe348);
																func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
							}
						} else {
							/*Memory Changes*/
							*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe328*/);
							*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
							*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe570*/);
							/*Registers Changes*/
							regs.rax = UINT64 (rax_1 /*0x0*/);
							regs.rdi = UINT64 (0x4007f3);
							regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
							regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
							regs.rax = puts (/*"invalid left value!"*/ (const char *) 0x4007f3);
							const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
							const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
							const UINT64 m7fffffffe348_2_64 = *((UINT64 *) 0x7fffffffe348);
														func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
						}
					} else {
					}
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_1 /*0x1*/);
					regs.rdi = UINT64 (0x4007db);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe220*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
					regs.rax = puts (/*"left option is missing!"*/ (const char *) 0x4007db);
					const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
					const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
					const UINT64 m7fffffffe348_2_64 = *((UINT64 *) 0x7fffffffe348);
										func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
				}
			} else {
			}
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (m7fffffffe220_0_64 /*0x400720*/);
			*((UINT64 *) 0x7fffffffe228) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe228_0_32) /*0x400490*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_0_64 /*0x7ffff5ff3f45*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (rdi_0 /*0x0*/);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe328*/);
			regs.rsp = UINT64 (rsp_0 /*0x7fffffffe248*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"Usage: program --left <num1> --right <num2>"*/ (const char *) 0x4007a8);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe230_1_64 = *((UINT64 *) 0x7fffffffe230);
			const UINT32 m7fffffffe23c_1_32 = *((UINT32 *) 0x7fffffffe23c);
			const UINT64 m7fffffffe338_1_64 = *((UINT64 *) 0x7fffffffe338);
			const UINT64 m7fffffffe340_1_64 = *((UINT64 *) 0x7fffffffe340);
						func_3 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
		}
	} else {
	}
}
