
#include "twincode.h"

int func_1 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_1 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0xfffffffe);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe248*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	*((UINT32 *) 0x7fffffffe238) = 0x0;
	*((UINT64 *) 0x7fffffffe248) = 0x7ffff5feaf45;
	const UINT32 m7fffffffe238_0_32 = *((UINT32 *) 0x7fffffffe238);
	const UINT64 m7fffffffe248_0_64 = *((UINT64 *) 0x7fffffffe248);
	const UINT64 m7fffffffe330_0_64 = *((UINT64 *) 0x7fffffffe330);
	if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x2)/*}*/ < /*signed {*/signExtend_0x80_0x20 ((UINT64 (rdi_0) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x3)*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x4000000000000000*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_0_64 /*0x7ffff5feaf45*/);
		*((UINT64 *) 0x7fffffffe330) = UINT64 (argv[1] /*0x7fffffffe567*/);
		/*Registers Changes*/
		regs.rax = UINT64 (argv[1] /*0x7fffffffe567*/);
		regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
		regs.rsi = UINT64 (rsi_0 /*0x7fffffffe328*/);
		regs.rsp = UINT64 (((rsp_0 - 0x18) & 0xffffffffffffffff) /*0x7fffffffe230*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = strcmp (/*"--wrongopt"*/ (const char *) m7fffffffe330_0_64 /*0x7fffffffe567*/, /*"--option"*/ (const char *) 0x4006c0);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
		const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe338_1_64 = *((UINT64 *) 0x7fffffffe338);
		if (((UINT64 (rax_1) & 0xffffffff) /*0x8*/ != 0)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (0x4006c9);
			regs.rsp = UINT64 (((rsp_0 - 0x18) & 0xffffffffffffffff) /*0x7fffffffe230*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = puts (/*"unknown option!"*/ (const char *) 0x4006c9);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
						func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe328*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe572*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_1 /*0x0*/);
			regs.rdi = UINT64 (m7fffffffe338_1_64 /*0x7fffffffe572*/);
			regs.rsi = UINT64 (0x4006d9);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
			regs.rax = strcmp (/*"wrongvalue"*/ (const char *) m7fffffffe338_1_64 /*0x7fffffffe572*/, /*"optvalue"*/ (const char *) 0x4006d9);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
			if (((UINT64 (rax_2) & 0xffffffff) /*0x8*/ != 0)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
				/*Registers Changes*/
				regs.rdi = UINT64 (0x4006e2);
				regs.rsp = UINT64 (((rsp_0 - 0x18) & 0xffffffffffffffff) /*0x7fffffffe230*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rax = puts (/*"invalid value!"*/ (const char *) 0x4006e2);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
								func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
				/*Registers Changes*/
				regs.rax = UINT64 (rax_2 /*0x0*/);
				regs.rdi = UINT64 (0x4006f8);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
				regs.rax = puts (/*"correct option/value pair is given!"*/ (const char *) 0x4006f8);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
								func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
			}
		}
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe328*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x0*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_0_64 /*0x7ffff5feaf45*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (rdi_0 /*0x0*/);
		regs.rsi = UINT64 (rsi_0 /*0x7fffffffe328*/);
		regs.rsp = UINT64 (((rsp_0 - 0x18) & 0xffffffffffffffff) /*0x7fffffffe230*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe240*/);
		regs.rax = puts (/*"Usage: program <one-option> <its-value>"*/ (const char *) 0x400698);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
		const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe338_1_64 = *((UINT64 *) 0x7fffffffe338);
				func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
	}
}
