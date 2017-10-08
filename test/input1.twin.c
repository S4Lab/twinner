
#include "twincode.h"

bool cond_5 (UINT8 m7fffffffe5b6_0_8) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x8)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (m7fffffffe5b6_0_8) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x4)*//*}*/);
}

int func_1 (struct RegistersSet &regs, UINT8 m7fffffffe5b8_0_8, UINT64 rbp_0, UINT64 rsp_0);
bool cond_6 (UINT8 m7fffffffe5b8_0_8) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (m7fffffffe5b8_0_8) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x5)*//*}*/);
}

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
int func_1 (struct RegistersSet &regs, UINT8 m7fffffffe5b8_0_8, UINT64 rbp_0, UINT64 rsp_0) {
if (cond_6 (m7fffffffe5b8_0_8)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4006e9);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
		puts (/*"second condition; else part"*/ (const char *) 0x4006e9);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
				;
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4006cd);
		regs.rsp = UINT64 (((rsp_0 - 0x28) & 0xffffffffffffffff) /*0x7fffffffe280*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
		puts (/*"second condition; then part"*/ (const char *) 0x4006cd);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
				;
	}
}
bool cond_11 (UINT8 m7fffffffe5b7_0_8) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/ < /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (m7fffffffe5b7_0_8) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x7)*//*}*/);
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
	const UINT64 m7fffffffe390_0_64 = *((UINT64 *) 0x7fffffffe390);
	const UINT8 m7fffffffe5b6_0_8 = *((UINT8 *) 0x7fffffffe5b6);
	const UINT8 m7fffffffe5b7_0_8 = *((UINT8 *) 0x7fffffffe5b7);
	const UINT8 m7fffffffe5b8_0_8 = *((UINT8 *) 0x7fffffffe5b8);
	if ((((UINT64 (rsp_0) + 0xffff800000001d58) & 0xffffffffffffffff) /*0x0*/ == 0)) {
		if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x1)/*}*/ < /*signed {*/signExtend_0x80_0x20 ((UINT64 (rdi_0) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/)) {
			if ((((UINT64 (rsi_0) + 0xffff800000001c78) & 0xffffffffffffffff) /*0x0*/ == 0)) {
				if ((((UINT64 (argv[1]) + 0xffff800000001a4a) & 0xffffffffffffffff) /*0x0*/ == 0)) {
					if (cond_5 (UINT64 (m7fffffffe5b6_0_8))) {
						/*Memory Changes*/
						*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x4005f0*/);
						*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
						*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (m7fffffffe5b6_0_8) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0xffffe380*/);
						*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
						*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
						*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ffbf45*/);
						*((UINT64 *) 0x7fffffffe390) = UINT64 (m7fffffffe390_0_64 /*0x7fffffffe5b6*/);
						/*Registers Changes*/
						regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
						regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
						regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
						regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
						regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
						puts (/*"first condition; else part"*/ (const char *) 0x4006b2);
						const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
						const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
												func_1 (regs, UINT64 (m7fffffffe5b8_0_8), UINT64 (rbp_0), UINT64 (rsp_0));
					} else {
						if (cond_11 (UINT64 (m7fffffffe5b7_0_8))) {
							/*Memory Changes*/
							*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x4005f0*/);
							*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
							*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (m7fffffffe5b6_0_8) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x40ffffe380*/);
							*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
							*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
							*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ffbf45*/);
							*((UINT64 *) 0x7fffffffe390) = UINT64 (m7fffffffe390_0_64 /*0x7fffffffe5b6*/);
							/*Registers Changes*/
							regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
							regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
							regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
							regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
							regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
							puts (/*"first condition; else part"*/ (const char *) 0x4006b2);
							const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
							const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
														func_1 (regs, UINT64 (m7fffffffe5b8_0_8), UINT64 (rbp_0), UINT64 (rsp_0));
						} else {
							/*Memory Changes*/
							*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x4005f0*/);
							*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x4000000000400440*/);
							*((UINT64 *) 0x7fffffffe290) = UINT64 (((((signExtend_0x20_0x8 (m7fffffffe5b6_0_8) - 0x30) << 0x20) & 0xffffffff00000000) | m7fffffffe290_0_32) /*0x40ffffe380*/);
							*((UINT64 *) 0x7fffffffe298) = UINT64 (m7fffffffe298_0_64 /*0x0*/);
							*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
							*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ffbf45*/);
							*((UINT64 *) 0x7fffffffe390) = UINT64 (m7fffffffe390_0_64 /*0x7fffffffe5b6*/);
							/*Registers Changes*/
							regs.rax = UINT64 (((rsi_0 + 0x8) & 0xffffffffffffffff) /*0x7fffffffe390*/);
							regs.rdi = UINT64 (rdi_0 /*0x40000000*/);
							regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
							regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
							regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
							puts (/*"first condition; then part"*/ (const char *) 0x400697);
							const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
							const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
														func_1 (regs, UINT64 (m7fffffffe5b8_0_8), UINT64 (rbp_0), UINT64 (rsp_0));
						}
					}
				} else {
				}
			} else {
			}
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe280) = UINT64 (m7fffffffe280_0_64 /*0x4005f0*/);
			*((UINT64 *) 0x7fffffffe288) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe288_0_32) /*0x400440*/);
			*((UINT64 *) 0x7fffffffe2a0) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe2a8) = UINT64 (m7fffffffe2a8_0_64 /*0x7ffff5ffbf45*/);
			/*Registers Changes*/
			regs.rdi = UINT64 (rdi_0 /*0x0*/);
			regs.rsi = UINT64 (rsi_0 /*0x7fffffffe388*/);
			regs.rsp = UINT64 (rsp_0 /*0x7fffffffe2a8*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe2a0*/);
			puts (/*"Usage: program <command codes>"*/ (const char *) 0x400678);
			const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
			const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
						func_2 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
		}
	} else {
	}
}
