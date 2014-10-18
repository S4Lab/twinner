
#include "twincode.h"

int main () {
	struct RegistersSet regs;
	SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	*((UINT64 *) 0x7fffffffe038) = 0x0;
	// coding trace #0
	{
	const UINT64 m7fffffffe038_0 = *((UINT64 *) 0x7fffffffe038);
	const UINT64 m7fffffffe058_0 = *((UINT64 *) 0x7fffffffe058);
	const UINT64 m7fffffffe140_0 = *((UINT64 *) 0x7fffffffe140);
	const UINT64 m7fffffffe148_0 = *((UINT64 *) 0x7fffffffe148);
	const UINT64 m7fffffffe3a0_0 = *((UINT64 *) 0x7fffffffe3a0);
	if (/*unsigned {*/((((((((((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff) * 0x100000000) | (((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x100000000000000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff)) & 0xffffffff) - 0x3) & 0xffffffff) + 0x4) & 0xffffffff) /*0x36*//*}*/ != /*unsigned {*/(((((((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff) + (((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff)) & 0xffffffff) + 0x3) & 0xffffffff) /*0x65*//*}*/) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe030) = (rsi_0 & 0xffffffffffffffff) /*0x7fffffffe138*/;
		*((UINT64 *) 0x7fffffffe038) = (((rdi_0 & 0xffffffff) * 0x100000000) | (m7fffffffe038_0 & 0xffffffff)) /*0x3004003c0*/;
		*((UINT64 *) 0x7fffffffe040) = ((0x20 * 0x100000000) | ((((((((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff) * 0x100000000) | (((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x100000000000000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff)) & 0xffffffff) - 0x3) & 0xffffffff)) /*0x2000000032*/;
		*((UINT64 *) 0x7fffffffe048) = (((((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff) * 0x100000000) | (((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x100000000000000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff)) /*0x3100000035*/;
		*((UINT64 *) 0x7fffffffe050) = (rbp_0 & 0xffffffffffffffff) /*0x400520*/;
		*((UINT64 *) 0x7fffffffe058) = m7fffffffe058_0 /*0x7fffe4a20040*/;
		*((UINT64 *) 0x7fffffffe140) = m7fffffffe140_0 /*0x7fffffffe3a2*/;
		*((UINT64 *) 0x7fffffffe148) = m7fffffffe148_0 /*0x7fffffffe3a7*/;
		/*Registers Changes*/
		regs.rax = 0x0 /*0x0*/;
		regs.rbx = rbx_0;
		regs.rcx = rcx_0;
		regs.rdx = rdx_0;
		regs.rdi = rdi_0;
		regs.rsi = rsi_0 /*0x7fffffffe138*/;
		regs.rsp = (rsp_0 + 0x8) /*0x7fffffffe058*/;
		regs.rbp = (rbp_0 & 0xffffffffffffffff) /*0x400520*/;
		regs.r8 = r8_0;
		regs.r9 = r9_0;
		regs.r10 = r10_0;
		regs.r11 = r11_0;
		regs.r12 = r12_0;
		regs.r13 = r13_0;
		regs.r14 = r14_0;
		regs.r15 = r15_0;
		regs = setRegistersValuesAndInvokeSyscall (regs);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
	}
	}
	// end of trace #0
	// coding trace #1
	{
	const UINT64 m7fffffffe038_0 = *((UINT64 *) 0x7fffffffe038);
	const UINT64 m7fffffffe058_0 = *((UINT64 *) 0x7fffffffe058);
	const UINT64 m7fffffffe140_0 = *((UINT64 *) 0x7fffffffe140);
	const UINT64 m7fffffffe148_0 = *((UINT64 *) 0x7fffffffe148);
	const UINT64 m7fffffffe3a0_0 = *((UINT64 *) 0x7fffffffe3a0);
	if (/*unsigned {*/((((((((((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff) * 0x100000000) | (((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x100000000000000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff)) & 0xffffffff) - 0x3) & 0xffffffff) + 0x4) & 0xffffffff) /*0x31*//*}*/ == /*unsigned {*/(((((((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff) + (((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff)) & 0xffffffff) + 0x3) & 0xffffffff) /*0x31*//*}*/) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe030) = (rsi_0 & 0xffffffffffffffff) /*0x7fffffffe138*/;
		*((UINT64 *) 0x7fffffffe038) = (((rdi_0 & 0xffffffff) * 0x100000000) | (m7fffffffe038_0 & 0xffffffff)) /*0x3004003c0*/;
		*((UINT64 *) 0x7fffffffe040) = ((0x0 * 0x100000000) | ((((((((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff) * 0x100000000) | (((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x100000000000000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff)) & 0xffffffff) - 0x3) & 0xffffffff)) /*0x2d*/;
		*((UINT64 *) 0x7fffffffe048) = (((((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x10000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff) * 0x100000000) | (((signExtend_0x20_0x8 (((m7fffffffe3a0_0 / 0x100000000000000) & 0xff)) & 0xffffffff) - 0x30) & 0xffffffff)) /*0x1700000030*/;
		*((UINT64 *) 0x7fffffffe050) = (rbp_0 & 0xffffffffffffffff) /*0x400520*/;
		*((UINT64 *) 0x7fffffffe058) = m7fffffffe058_0 /*0x7fffe4a1a040*/;
		*((UINT64 *) 0x7fffffffe140) = m7fffffffe140_0 /*0x7fffffffe3a2*/;
		*((UINT64 *) 0x7fffffffe148) = m7fffffffe148_0 /*0x7fffffffe3a7*/;
		*((UINT64 *) 0x7fffffffe3a0) = m7fffffffe3a0_0 /*0x6000000000470000*/;
		/*Registers Changes*/
		regs.rax = 0x0 /*0x0*/;
		regs.rbx = rbx_0;
		regs.rcx = rcx_0;
		regs.rdx = rdx_0;
		regs.rdi = rdi_0;
		regs.rsi = rsi_0 /*0x7fffffffe138*/;
		regs.rsp = (rsp_0 + 0x8) /*0x7fffffffe058*/;
		regs.rbp = (rbp_0 & 0xffffffffffffffff) /*0x400520*/;
		regs.r8 = r8_0;
		regs.r9 = r9_0;
		regs.r10 = r10_0;
		regs.r11 = r11_0;
		regs.r12 = r12_0;
		regs.r13 = r13_0;
		regs.r14 = r14_0;
		regs.r15 = r15_0;
		regs = setRegistersValuesAndInvokeSyscall (regs);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
	}
	}
	// end of trace #1
}
