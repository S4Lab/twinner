
#include "twincode.h"

int main (int argc, char *argv[]) {
	struct RegistersSet regs;
	SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	*((UINT8 *) 0x7fffffffe2c6) = 0x0;
	const UINT32 m7fffffffe2b8_0_32 = *((UINT32 *) 0x7fffffffe2b8);
	const UINT32 m7fffffffe2c0_0_32 = *((UINT32 *) 0x7fffffffe2c0);
	const UINT16 m7fffffffe2c4_0_16 = *((UINT16 *) 0x7fffffffe2c4);
	const UINT8 m7fffffffe2c6_0_8 = *((UINT8 *) 0x7fffffffe2c6);
	const UINT64 m7fffffffe2c8_0_64 = *((UINT64 *) 0x7fffffffe2c8);
	const UINT64 m7fffffffe2d8_0_64 = *((UINT64 *) 0x7fffffffe2d8);
	const UINT64 m7fffffffe3c0_0_64 = *((UINT64 *) 0x7fffffffe3c0);
	const UINT8 m7fffffffe5eb_0_8 = *((UINT8 *) 0x7fffffffe5eb);
	if ((((UINT64 (rsp_0) + UINT128 (0xffffffff, 0xffffffff, 0xffff8000, 0x1d28)) & 0xffffffffffffffff) /*0x0*/ == 0)) {
		if ((((UINT64 (argv[1]) + UINT128 (0xffffffff, 0xffffffff, 0xffff8000, 0x1a15)) & 0xffffffffffffffff) /*0x0*/ == 0)) {
			if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x2f)/*}*/ < /*signed {*/signExtend_0x80_0x8 (UINT64 (m7fffffffe5eb_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x32)*//*}*/)) {
				if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x39)/*}*/ >= /*signed {*/signExtend_0x80_0x8 (UINT64 (m7fffffffe5eb_0_8)) /*UINT128 (0x0, 0x0, 0x0, 0x32)*//*}*/)) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe2b0) = UINT64 (rsi_0 /*0x7fffffffe3b8*/);
					*((UINT64 *) 0x7fffffffe2b8) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe2b8_0_32) /*0x200400490*/);
					*((UINT64 *) 0x7fffffffe2c0) = UINT64 (((((((m7fffffffe5eb_0_8 << 0x8) | m7fffffffe2c6_0_8) << 0x10) | m7fffffffe2c4_0_16) << 0x20) | m7fffffffe2c0_0_32) /*0x32007fffffffe3b0*/);
					*((UINT64 *) 0x7fffffffe2c8) = UINT64 (((((signExtend_0x20_0x8 (m7fffffffe5eb_0_8) - 0x30) << 0x2a) & 0xffffffff00000000) | 0xa) /*0x8000000000a*/);
					*((UINT64 *) 0x7fffffffe2d0) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe2d8) = UINT64 (m7fffffffe2d8_0_64 /*0x7ffff6024ec5*/);
					*((UINT64 *) 0x7fffffffe3c0) = UINT64 (m7fffffffe3c0_0_64 /*0x7fffffffe5eb*/);
					/*Registers Changes*/
					regs.rax = UINT64 (0x0);
					regs.rdi = UINT64 (0x4006a7);
					regs.rsi = UINT64 ((((signExtend_0x20_0x8 (m7fffffffe5eb_0_8) - 0x30) * 0x400) & 0xffffffff) /*0x800*/);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe2b0*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2d0*/);
					printf (/*"%d\n"*/ (const char *) 0x4006a7, (UINT64) (((signExtend_0x20_0x8 (m7fffffffe5eb_0_8) - 0x30) * 0x400) & 0xffffffff) /*0x800*/);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
					{
						/*Memory Changes*/
						/*Registers Changes*/
						regs.rax = UINT64 (0x0);
						regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2d8*/);
						regs.rbp = UINT64 (rbp_0 /*0x0*/);
						return int (regs.rax);
					}
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe2b0) = UINT64 (rsi_0 /*0x7fffffffe3b8*/);
					*((UINT64 *) 0x7fffffffe2b8) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe2b8_0_32) /*0x200400490*/);
					*((UINT64 *) 0x7fffffffe2c0) = UINT64 (((((((m7fffffffe5eb_0_8 << 0x8) | m7fffffffe2c6_0_8) << 0x10) | m7fffffffe2c4_0_16) << 0x20) | m7fffffffe2c0_0_32) /*0x40007fffffffe3b0*/);
					*((UINT64 *) 0x7fffffffe2c8) = UINT64 (m7fffffffe2c8_0_64 /*0x0*/);
					*((UINT64 *) 0x7fffffffe2d0) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe2d8) = UINT64 (m7fffffffe2d8_0_64 /*0x7ffff6024ec5*/);
					*((UINT64 *) 0x7fffffffe3c0) = UINT64 (m7fffffffe3c0_0_64 /*0x7fffffffe5eb*/);
					/*Registers Changes*/
					regs.rax = UINT64 (m7fffffffe5eb_0_8 /*0x40*/);
					regs.rdi = UINT64 (0x400694);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe3b8*/);
					regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe2b0*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2d0*/);
					puts (/*"overflow"*/ (const char *) 0x400694);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
					{
						/*Memory Changes*/
						/*Registers Changes*/
						regs.rax = UINT64 (0xfffffffe);
						regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2d8*/);
						regs.rbp = UINT64 (rbp_0 /*0x0*/);
						return int (regs.rax);
					}
				}
			} else {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe2b0) = UINT64 (rsi_0 /*0x7fffffffe3b8*/);
				*((UINT64 *) 0x7fffffffe2b8) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe2b8_0_32) /*0x200400490*/);
				*((UINT64 *) 0x7fffffffe2c0) = UINT64 (((((((m7fffffffe5eb_0_8 << 0x8) | m7fffffffe2c6_0_8) << 0x10) | m7fffffffe2c4_0_16) << 0x20) | m7fffffffe2c0_0_32) /*0x7fffffffe3b0*/);
				*((UINT64 *) 0x7fffffffe2c8) = UINT64 (m7fffffffe2c8_0_64 /*0x0*/);
				*((UINT64 *) 0x7fffffffe2d0) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe2d8) = UINT64 (m7fffffffe2d8_0_64 /*0x7ffff6024ec5*/);
				*((UINT64 *) 0x7fffffffe3c0) = UINT64 (m7fffffffe3c0_0_64 /*0x7fffffffe5eb*/);
				/*Registers Changes*/
				regs.rax = UINT64 (m7fffffffe5eb_0_8 /*0x0*/);
				regs.rdi = UINT64 (0x40069d);
				regs.rsi = UINT64 (rsi_0 /*0x7fffffffe3b8*/);
				regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe2b0*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2d0*/);
				puts (/*"underflow"*/ (const char *) 0x40069d);
				const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
				const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				{
					/*Memory Changes*/
					/*Registers Changes*/
					regs.rax = UINT64 (0xffffffff);
					regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2d8*/);
					regs.rbp = UINT64 (rbp_0 /*0x0*/);
					return int (regs.rax);
				}
			}
		} else {
		}
	}
}
