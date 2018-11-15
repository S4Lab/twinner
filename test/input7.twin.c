
#include "twincode.h"

int func_1 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_1 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0xfffffffe);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe238*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int func_2 (struct RegistersSet &regs, UINT64 m7fffffffe338_3_64, UINT64 m7fffffffe340_3_64, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 m7fffffffe338_3_64, UINT64 m7fffffffe340_3_64, UINT64 rbp_0, UINT64 rsi_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe338) = UINT64 (m7fffffffe338_3_64 /*0x7fffffffe57f*/);
		*((UINT64 *) 0x7fffffffe340) = UINT64 (m7fffffffe340_3_64 /*0x7fffffffe585*/);
		/*Registers Changes*/
		regs.rax = UINT64 (m7fffffffe338_3_64 /*0x7fffffffe57f*/);
		regs.rdx = UINT64 (m7fffffffe340_3_64 /*0x7fffffffe585*/);
		regs.rdi = UINT64 (m7fffffffe338_3_64 /*0x7fffffffe57f*/);
		regs.rsi = UINT64 (m7fffffffe340_3_64 /*0x7fffffffe585*/);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
		regs.rax = strcmp (/*"first"*/ (const char *) m7fffffffe338_3_64 /*0x7fffffffe57f*/, /*"second"*/ (const char *) m7fffffffe340_3_64 /*0x7fffffffe585*/);
		const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
		const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
		if (((rax_4 & 0xffffffff) /*0xfffffff3*/ != 0)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (0x400888);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
			regs.rax = puts (/*"last args are different"*/ (const char *) 0x400888);
			const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
			const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
						func_1 (regs, rbp_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_4 /*0x0*/);
			regs.rdi = UINT64 (0x400860);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
			regs.rax = puts (/*"fourth and fifth arguments are the same"*/ (const char *) 0x400860);
			const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
			const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
						func_1 (regs, rbp_0, rsp_0);
		}
	}
}
int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	*((UINT32 *) 0x7fffffffe218) = 0x400490;
	*((UINT64 *) 0x7fffffffe220) = 0x7fffffffe310;
	*((UINT32 *) 0x7fffffffe228) = 0x0;
	*((UINT16 *) 0x7fffffffe22c) = 0x0;
	*((UINT64 *) 0x7fffffffe238) = 0x7ffff5feaf45;
	const UINT32 m7fffffffe218_0_32 = *((UINT32 *) 0x7fffffffe218);
	const UINT64 m7fffffffe238_0_64 = *((UINT64 *) 0x7fffffffe238);
	if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/ < /*signed {*/signExtend_0x80_0x20 ((UINT64 (rdi_0) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/)) {
		if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x6)/*}*/ < /*signed {*/signExtend_0x80_0x20 ((UINT64 (rdi_0) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
			*((UINT64 *) 0x7fffffffe218) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe218_0_32) /*0x700400490*/);
			*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe238) = UINT64 (m7fffffffe238_0_64 /*0x7ffff5feaf45*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (0x400790);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe318*/);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
			regs.rax = puts (/*"At most five arguments are allowed"*/ (const char *) 0x400790);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe220_1_64 = *((UINT64 *) 0x7fffffffe220);
			const UINT32 m7fffffffe228_1_32 = *((UINT32 *) 0x7fffffffe228);
			const UINT16 m7fffffffe22c_1_16 = *((UINT16 *) 0x7fffffffe22c);
			const UINT64 m7fffffffe328_1_64 = *((UINT64 *) 0x7fffffffe328);
			const UINT64 m7fffffffe330_1_64 = *((UINT64 *) 0x7fffffffe330);
						func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
			*((UINT64 *) 0x7fffffffe218) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe218_0_32) /*0x400400490*/);
			*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe238) = UINT64 (m7fffffffe238_0_64 /*0x7ffff5feaf45*/);
			*((UINT64 *) 0x7fffffffe320) = UINT64 (argv[1] /*0x7fffffffe570*/);
			/*Registers Changes*/
			regs.rax = UINT64 (argv[1] /*0x7fffffffe570*/);
			regs.rdi = UINT64 (rdi_0 /*0x4*/);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe318*/);
			regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
			const UINT64 m7fffffffe320_0_64 = *((UINT64 *) 0x7fffffffe320);
			regs.rax = strcmp (/*"random"*/ (const char *) m7fffffffe320_0_64 /*0x7fffffffe570*/, /*"magicvalue"*/ (const char *) 0x4007b3);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe220_1_64 = *((UINT64 *) 0x7fffffffe220);
			const UINT32 m7fffffffe228_1_32 = *((UINT32 *) 0x7fffffffe228);
			const UINT16 m7fffffffe22c_1_16 = *((UINT16 *) 0x7fffffffe22c);
			const UINT64 m7fffffffe328_1_64 = *((UINT64 *) 0x7fffffffe328);
			const UINT64 m7fffffffe330_1_64 = *((UINT64 *) 0x7fffffffe330);
			if (((UINT64 (rax_1) & 0xffffffff) /*0x5*/ != 0)) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
				/*Registers Changes*/
				regs.rdi = UINT64 (0x4007c0);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
				regs.rax = puts (/*"You are not allowed to run this program!"*/ (const char *) 0x4007c0);
				const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
				const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
								func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
			} else {
				if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ != /*unsigned {*/UINT64 (argv[2][0]) /*0x4c*//*}*/)) {
					if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ != /*unsigned {*/UINT64 (argv[2][1]) /*0x4c*//*}*/)) {
						/*Memory Changes*/
						*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
						*((UINT64 *) 0x7fffffffe220) = UINT64 (m7fffffffe220_1_64 /*0x7fffffffe310*/);
						*((UINT64 *) 0x7fffffffe228) = UINT64 (((((((argv[2][1] << 0x8) | argv[2][0]) << 0x10) | m7fffffffe22c_1_16) << 0x20) | m7fffffffe228_1_32) /*0x0*/);
						*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
						*((UINT64 *) 0x7fffffffe328) = UINT64 (m7fffffffe328_1_64 /*0x7fffffffe577*/);
						*((UINT64 *) 0x7fffffffe330) = UINT64 (m7fffffffe330_1_64 /*0x7fffffffe57a*/);
						/*Registers Changes*/
						regs.rax = UINT64 (rax_1 /*0x0*/);
						regs.rdi = UINT64 (m7fffffffe330_1_64 /*0x7fffffffe57a*/);
						regs.rsi = UINT64 (0x4007e9);
						regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
						regs.rax = strcmp (/*"left"*/ (const char *) m7fffffffe330_1_64 /*0x7fffffffe57a*/, /*"left"*/ (const char *) 0x4007e9);
						const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
						const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
						if (((UINT64 (rax_2) & 0xffffffff) /*0x0*/ == 0)) {
							/*Memory Changes*/
							*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
							*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
							/*Registers Changes*/
							regs.rax = UINT64 (rax_2 /*0x0*/);
							regs.rdi = UINT64 (0x40082b);
							regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
							regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
							regs.rax = puts (/*"left-path of LL scenario"*/ (const char *) 0x40082b);
							const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
							const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
							const UINT64 m7fffffffe338_3_64 = *((UINT64 *) 0x7fffffffe338);
							const UINT64 m7fffffffe340_3_64 = *((UINT64 *) 0x7fffffffe340);
														func_2 (regs, UINT64 (m7fffffffe338_3_64), UINT64 (m7fffffffe340_3_64), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
						} else {
							/*Memory Changes*/
							*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
							*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
							/*Registers Changes*/
							regs.rax = UINT64 (rax_2 /*0x1*/);
							regs.rdi = UINT64 (0x400844);
							regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
							regs.rax = puts (/*"right-path of LL scenario"*/ (const char *) 0x400844);
							const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
							const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
							const UINT64 m7fffffffe338_3_64 = *((UINT64 *) 0x7fffffffe338);
							const UINT64 m7fffffffe340_3_64 = *((UINT64 *) 0x7fffffffe340);
														func_2 (regs, UINT64 (m7fffffffe338_3_64), UINT64 (m7fffffffe340_3_64), UINT64 (rbp_0), UINT64 (rsi_0), UINT64 (rsp_0));
						}
					} else {
						/*Memory Changes*/
						*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
						*((UINT64 *) 0x7fffffffe220) = UINT64 (m7fffffffe220_1_64 /*0x7fffffffe310*/);
						*((UINT64 *) 0x7fffffffe228) = UINT64 (((((((argv[2][1] << 0x8) | argv[2][0]) << 0x10) | m7fffffffe22c_1_16) << 0x20) | m7fffffffe228_1_32) /*0x0*/);
						*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
						*((UINT64 *) 0x7fffffffe328) = UINT64 (m7fffffffe328_1_64 /*0x7fffffffe577*/);
						/*Registers Changes*/
						regs.rax = UINT64 (rax_1 /*0x0*/);
						regs.rdi = UINT64 (0x40081f);
						regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
						regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
						regs.rax = puts (/*"LR scenario"*/ (const char *) 0x40081f);
						const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
						const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
												func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
					}
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
					*((UINT64 *) 0x7fffffffe220) = UINT64 (m7fffffffe220_1_64 /*0x7fffffffe310*/);
					*((UINT64 *) 0x7fffffffe228) = UINT64 (((((((argv[2][1] << 0x8) | argv[2][0]) << 0x10) | m7fffffffe22c_1_16) << 0x20) | m7fffffffe228_1_32) /*0x0*/);
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe328) = UINT64 (m7fffffffe328_1_64 /*0x7fffffffe577*/);
					*((UINT64 *) 0x7fffffffe330) = UINT64 (m7fffffffe330_1_64 /*0x7fffffffe57a*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_1 /*0x0*/);
					regs.rdi = UINT64 (m7fffffffe330_1_64 /*0x7fffffffe57a*/);
					regs.rsi = UINT64 (0x4007e9);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rax = strcmp (/*"left"*/ (const char *) m7fffffffe330_1_64 /*0x7fffffffe57a*/, /*"left"*/ (const char *) 0x4007e9);
					const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
					const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
					if (((UINT64 (rax_2) & 0xffffffff) /*0x0*/ == 0)) {
						/*Memory Changes*/
						*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
						/*Registers Changes*/
						regs.rdi = UINT64 (0x400806);
						regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
						regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
						regs.rax = puts (/*"right-path of R scenario"*/ (const char *) 0x400806);
						const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
						const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
						const UINT64 m7fffffffe338_3_64 = *((UINT64 *) 0x7fffffffe338);
						const UINT64 m7fffffffe340_3_64 = *((UINT64 *) 0x7fffffffe340);
												func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
					} else {
						/*Memory Changes*/
						*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
						/*Registers Changes*/
						regs.rax = UINT64 (rax_2 /*0x1*/);
						regs.rdi = UINT64 (0x4007ee);
						regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
						regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
						regs.rax = puts (/*"left-path of R scenario"*/ (const char *) 0x4007ee);
						const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
						const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
						const UINT64 m7fffffffe338_3_64 = *((UINT64 *) 0x7fffffffe338);
						const UINT64 m7fffffffe340_3_64 = *((UINT64 *) 0x7fffffffe340);
												func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
					}
				}
			}
		}
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe210) = UINT64 (rsi_0 /*0x7fffffffe318*/);
		*((UINT64 *) 0x7fffffffe218) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe218_0_32) /*0x400490*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (m7fffffffe238_0_64 /*0x7ffff5feaf45*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (rdi_0 /*0x0*/);
		regs.rsi = UINT64 (rsi_0 /*0x7fffffffe318*/);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe210*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe230*/);
		regs.rax = puts (/*"At least three arguments are required"*/ (const char *) 0x400768);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
		const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe220_1_64 = *((UINT64 *) 0x7fffffffe220);
		const UINT32 m7fffffffe228_1_32 = *((UINT32 *) 0x7fffffffe228);
		const UINT16 m7fffffffe22c_1_16 = *((UINT16 *) 0x7fffffffe22c);
		const UINT64 m7fffffffe328_1_64 = *((UINT64 *) 0x7fffffffe328);
		const UINT64 m7fffffffe330_1_64 = *((UINT64 *) 0x7fffffffe330);
				func_1 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
	}
}
