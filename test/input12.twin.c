
#include "twincode.h"

bool cond_3 (UINT8 n_v_argv_2_0) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_0) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x1)*//*}*/);
}

bool cond_4 (UINT8 n_v_argv_2_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x6)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/);
}

int func_1 (struct RegistersSet &regs, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsp_0);
bool cond_5 (UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ >= /*signed {*/signExtend_0x80_0x20 ((((signExtend_0x80_0x8 (n_v_argv_2_1) + signExtend_0x80_0x8 (n_v_argv_2_0)) + UINT128 (0x0, 0x0, 0x0, 0xffffffa0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x3)*//*}*/);
}

int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe258*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsp_0) {
if ((/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/ < /*signed {*/signExtend_0x80_0x20 ((rdi_0 & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x6)*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4008bd);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"there were unused arguments"*/ (const char *) 0x4008bd);
		const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
		const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe240_4_64 = *((UINT64 *) 0x7fffffffe240);
		const UINT64 m7fffffffe348_4_64 = *((UINT64 *) 0x7fffffffe348);
				func_3 (regs, rbp_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4008d9);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"not too many args"*/ (const char *) 0x4008d9);
		const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
		const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe240_4_64 = *((UINT64 *) 0x7fffffffe240);
		const UINT64 m7fffffffe348_4_64 = *((UINT64 *) 0x7fffffffe348);
				func_3 (regs, rbp_0, rsp_0);
	}
}
int func_1 (struct RegistersSet &regs, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsp_0) {
if (cond_5 (n_v_argv_2_0, n_v_argv_2_1)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 ((((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) + (signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30)) & 0xffffffff) /*0x0*/);
		regs.rdx = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) & 0xffffffff) /*0x0*/);
		regs.rdi = UINT64 (0x40083f);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"x + y <= 5"*/ (const char *) 0x40083f);
		const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
		const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				func_2 (regs, rbp_0, rdi_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 ((((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) + (signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30)) & 0xffffffff) /*0x2*/);
		regs.rdx = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) & 0xffffffff) /*0x2*/);
		regs.rdi = UINT64 (0x400835);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"x + y > 5"*/ (const char *) 0x400835);
		const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
		const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				func_2 (regs, rbp_0, rdi_0, rsp_0);
	}
}
bool cond_13 (UINT8 n_v_argv_2_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x6)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/);
}

int func_4 (struct RegistersSet &regs, UINT64 m7fffffffe240_4_64, UINT64 m7fffffffe348_4_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_4_0, UINT8 n_v_argv_4_1, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0);
bool cond_20 (UINT8 n_v_argv_2_0) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x2)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_0) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x1)*//*}*/);
}

bool cond_21 (UINT8 n_v_argv_2_1) {
	return (/*signed {*/UINT128 (0x0, 0x0, 0x0, 0x3)/*}*/ >= /*signed {*/signExtend_0x80_0x20 (((signExtend_0x80_0x8 (n_v_argv_2_1) + UINT128 (0x0, 0x0, 0x0, 0xffffffd0)) & 0xffffffff)) /*UINT128 (0x0, 0x0, 0x0, 0x2)*//*}*/);
}

int func_5 (struct RegistersSet &regs, UINT64 m7fffffffe230_5_64, UINT32 m7fffffffe238_5_32, UINT16 m7fffffffe23c_5_16, UINT64 m7fffffffe358_5_64, UINT8 n_v_argv_4_0, UINT8 n_v_argv_4_1, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0);
int func_6 (struct RegistersSet &regs, UINT8 n_v_argv_4_1, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsp_0);
int func_6 (struct RegistersSet &regs, UINT8 n_v_argv_4_1, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsp_0) {
if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ == /*unsigned {*/n_v_argv_4_1 /*0x52*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4008b1);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"w = R"*/ (const char *) 0x4008b1);
		const UINT64 rax_7 = regs.rax, rbx_7 = regs.rbx, rcx_7 = regs.rcx, rdx_7 = regs.rdx, rdi_7 = regs.rdi, rsi_7 = regs.rsi, rsp_7 = regs.rsp, rbp_7 = regs.rbp, r8_7 = regs.r8, r9_7 = regs.r9, r10_7 = regs.r10, r11_7 = regs.r11, r12_7 = regs.r12, r13_7 = regs.r13, r14_7 = regs.r14, r15_7 = regs.r15;
		const UINT128 xmm0_7 = UINT128 (regs.xmm0), xmm1_7 = UINT128 (regs.xmm1), xmm2_7 = UINT128 (regs.xmm2), xmm3_7 = UINT128 (regs.xmm3), xmm4_7 = UINT128 (regs.xmm4), xmm5_7 = UINT128 (regs.xmm5), xmm6_7 = UINT128 (regs.xmm6), xmm7_7 = UINT128 (regs.xmm7), xmm8_7 = UINT128 (regs.xmm8), xmm9_7 = UINT128 (regs.xmm9), xmm10_7 = UINT128 (regs.xmm10), xmm11_7 = UINT128 (regs.xmm11), xmm12_7 = UINT128 (regs.xmm12), xmm13_7 = UINT128 (regs.xmm13), xmm14_7 = UINT128 (regs.xmm14), xmm15_7 = UINT128 (regs.xmm15);
				func_2 (regs, rbp_0, rdi_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (0x4008b7);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"w = L"*/ (const char *) 0x4008b7);
		const UINT64 rax_7 = regs.rax, rbx_7 = regs.rbx, rcx_7 = regs.rcx, rdx_7 = regs.rdx, rdi_7 = regs.rdi, rsi_7 = regs.rsi, rsp_7 = regs.rsp, rbp_7 = regs.rbp, r8_7 = regs.r8, r9_7 = regs.r9, r10_7 = regs.r10, r11_7 = regs.r11, r12_7 = regs.r12, r13_7 = regs.r13, r14_7 = regs.r14, r15_7 = regs.r15;
		const UINT128 xmm0_7 = UINT128 (regs.xmm0), xmm1_7 = UINT128 (regs.xmm1), xmm2_7 = UINT128 (regs.xmm2), xmm3_7 = UINT128 (regs.xmm3), xmm4_7 = UINT128 (regs.xmm4), xmm5_7 = UINT128 (regs.xmm5), xmm6_7 = UINT128 (regs.xmm6), xmm7_7 = UINT128 (regs.xmm7), xmm8_7 = UINT128 (regs.xmm8), xmm9_7 = UINT128 (regs.xmm9), xmm10_7 = UINT128 (regs.xmm10), xmm11_7 = UINT128 (regs.xmm11), xmm12_7 = UINT128 (regs.xmm12), xmm13_7 = UINT128 (regs.xmm13), xmm14_7 = UINT128 (regs.xmm14), xmm15_7 = UINT128 (regs.xmm15);
				func_2 (regs, rbp_0, rdi_0, rsp_0);
	}
}
int func_5 (struct RegistersSet &regs, UINT64 m7fffffffe230_5_64, UINT32 m7fffffffe238_5_32, UINT16 m7fffffffe23c_5_16, UINT64 m7fffffffe358_5_64, UINT8 n_v_argv_4_0, UINT8 n_v_argv_4_1, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0) {
if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x52)/*}*/ != /*unsigned {*/n_v_argv_4_0 /*0x4c*//*}*/)) {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_5_64 /*0x400770*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((((n_v_argv_4_1 << 0x8) | n_v_argv_4_0) << 0x10) | m7fffffffe23c_5_16) << 0x20) | m7fffffffe238_5_32) /*0x90*/);
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe358) = UINT64 (m7fffffffe358_5_64 /*0x7fffffffe58a*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_4_1 /*0x0*/);
		regs.rdi = UINT64 (0x4008ab);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"z = L"*/ (const char *) 0x4008ab);
		const UINT64 rax_6 = regs.rax, rbx_6 = regs.rbx, rcx_6 = regs.rcx, rdx_6 = regs.rdx, rdi_6 = regs.rdi, rsi_6 = regs.rsi, rsp_6 = regs.rsp, rbp_6 = regs.rbp, r8_6 = regs.r8, r9_6 = regs.r9, r10_6 = regs.r10, r11_6 = regs.r11, r12_6 = regs.r12, r13_6 = regs.r13, r14_6 = regs.r14, r15_6 = regs.r15;
		const UINT128 xmm0_6 = UINT128 (regs.xmm0), xmm1_6 = UINT128 (regs.xmm1), xmm2_6 = UINT128 (regs.xmm2), xmm3_6 = UINT128 (regs.xmm3), xmm4_6 = UINT128 (regs.xmm4), xmm5_6 = UINT128 (regs.xmm5), xmm6_6 = UINT128 (regs.xmm6), xmm7_6 = UINT128 (regs.xmm7), xmm8_6 = UINT128 (regs.xmm8), xmm9_6 = UINT128 (regs.xmm9), xmm10_6 = UINT128 (regs.xmm10), xmm11_6 = UINT128 (regs.xmm11), xmm12_6 = UINT128 (regs.xmm12), xmm13_6 = UINT128 (regs.xmm13), xmm14_6 = UINT128 (regs.xmm14), xmm15_6 = UINT128 (regs.xmm15);
				func_6 (regs, n_v_argv_4_1, rbp_0, rdi_0, rsp_0);
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
		*((UINT64 *) 0x7fffffffe230) = UINT64 (m7fffffffe230_5_64 /*0x400770*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 (((((((n_v_argv_4_1 << 0x8) | n_v_argv_4_0) << 0x10) | m7fffffffe23c_5_16) << 0x20) | m7fffffffe238_5_32) /*0x90*/);
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe358) = UINT64 (m7fffffffe358_5_64 /*0x7fffffffe58a*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_4_1 /*0x52*/);
		regs.rdi = UINT64 (0x4008a5);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"z = R"*/ (const char *) 0x4008a5);
		const UINT64 rax_6 = regs.rax, rbx_6 = regs.rbx, rcx_6 = regs.rcx, rdx_6 = regs.rdx, rdi_6 = regs.rdi, rsi_6 = regs.rsi, rsp_6 = regs.rsp, rbp_6 = regs.rbp, r8_6 = regs.r8, r9_6 = regs.r9, r10_6 = regs.r10, r11_6 = regs.r11, r12_6 = regs.r12, r13_6 = regs.r13, r14_6 = regs.r14, r15_6 = regs.r15;
		const UINT128 xmm0_6 = UINT128 (regs.xmm0), xmm1_6 = UINT128 (regs.xmm1), xmm2_6 = UINT128 (regs.xmm2), xmm3_6 = UINT128 (regs.xmm3), xmm4_6 = UINT128 (regs.xmm4), xmm5_6 = UINT128 (regs.xmm5), xmm6_6 = UINT128 (regs.xmm6), xmm7_6 = UINT128 (regs.xmm7), xmm8_6 = UINT128 (regs.xmm8), xmm9_6 = UINT128 (regs.xmm9), xmm10_6 = UINT128 (regs.xmm10), xmm11_6 = UINT128 (regs.xmm11), xmm12_6 = UINT128 (regs.xmm12), xmm13_6 = UINT128 (regs.xmm13), xmm14_6 = UINT128 (regs.xmm14), xmm15_6 = UINT128 (regs.xmm15);
				func_6 (regs, n_v_argv_4_1, rbp_0, rdi_0, rsp_0);
	}
}
int func_4 (struct RegistersSet &regs, UINT64 m7fffffffe240_4_64, UINT64 m7fffffffe348_4_64, UINT8 n_v_argv_2_0, UINT8 n_v_argv_2_1, UINT8 n_v_argv_4_0, UINT8 n_v_argv_4_1, UINT64 rbp_0, UINT64 rdi_0, UINT64 rsi_0, UINT64 rsp_0) {
if (cond_20 (n_v_argv_2_0)) {
		if (cond_21 (n_v_argv_2_1)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_4_64 /*0x7fffffffe330*/);
			*((UINT64 *) 0x7fffffffe248) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) & 0xffffffff)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_4_64 /*0x7fffffffe581*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff) /*0x0*/);
			regs.rdi = UINT64 (0x400879);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
			regs.rax = puts (/*"x < 3 | y < 4"*/ (const char *) 0x400879);
			const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
			const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe230_5_64 = *((UINT64 *) 0x7fffffffe230);
			const UINT32 m7fffffffe238_5_32 = *((UINT32 *) 0x7fffffffe238);
			const UINT16 m7fffffffe23c_5_16 = *((UINT16 *) 0x7fffffffe23c);
			const UINT64 m7fffffffe358_5_64 = *((UINT64 *) 0x7fffffffe358);
						func_5 (regs, m7fffffffe230_5_64, m7fffffffe238_5_32, m7fffffffe23c_5_16, m7fffffffe358_5_64, n_v_argv_4_0, n_v_argv_4_1, rbp_0, rdi_0, rsi_0, rsp_0);
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
			*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_4_64 /*0x7fffffffe330*/);
			*((UINT64 *) 0x7fffffffe248) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) & 0xffffffff)) /*0x0*/);
			*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
			*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_4_64 /*0x7fffffffe581*/);
			/*Registers Changes*/
			regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff) /*0x40*/);
			regs.rdi = UINT64 (0x400887);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
			regs.rax = puts (/*"x < 3 | y >= 4"*/ (const char *) 0x400887);
			const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
			const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe230_5_64 = *((UINT64 *) 0x7fffffffe230);
			const UINT32 m7fffffffe238_5_32 = *((UINT32 *) 0x7fffffffe238);
			const UINT16 m7fffffffe23c_5_16 = *((UINT16 *) 0x7fffffffe23c);
			const UINT64 m7fffffffe358_5_64 = *((UINT64 *) 0x7fffffffe358);
						func_5 (regs, m7fffffffe230_5_64, m7fffffffe238_5_32, m7fffffffe23c_5_16, m7fffffffe358_5_64, n_v_argv_4_0, n_v_argv_4_1, rbp_0, rdi_0, rsi_0, rsp_0);
		}
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
		*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_4_64 /*0x7fffffffe330*/);
		*((UINT64 *) 0x7fffffffe248) = UINT64 (((((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (n_v_argv_2_0) - 0x30) & 0xffffffff)) /*0x40*/);
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_4_64 /*0x7fffffffe581*/);
		/*Registers Changes*/
		regs.rax = UINT64 (((signExtend_0x20_0x8 (n_v_argv_2_1) - 0x30) & 0xffffffff) /*0x2*/);
		regs.rdi = UINT64 (0x400896);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = puts (/*"x >= 3 | y = ?"*/ (const char *) 0x400896);
		const UINT64 rax_5 = regs.rax, rbx_5 = regs.rbx, rcx_5 = regs.rcx, rdx_5 = regs.rdx, rdi_5 = regs.rdi, rsi_5 = regs.rsi, rsp_5 = regs.rsp, rbp_5 = regs.rbp, r8_5 = regs.r8, r9_5 = regs.r9, r10_5 = regs.r10, r11_5 = regs.r11, r12_5 = regs.r12, r13_5 = regs.r13, r14_5 = regs.r14, r15_5 = regs.r15;
		const UINT128 xmm0_5 = UINT128 (regs.xmm0), xmm1_5 = UINT128 (regs.xmm1), xmm2_5 = UINT128 (regs.xmm2), xmm3_5 = UINT128 (regs.xmm3), xmm4_5 = UINT128 (regs.xmm4), xmm5_5 = UINT128 (regs.xmm5), xmm6_5 = UINT128 (regs.xmm6), xmm7_5 = UINT128 (regs.xmm7), xmm8_5 = UINT128 (regs.xmm8), xmm9_5 = UINT128 (regs.xmm9), xmm10_5 = UINT128 (regs.xmm10), xmm11_5 = UINT128 (regs.xmm11), xmm12_5 = UINT128 (regs.xmm12), xmm13_5 = UINT128 (regs.xmm13), xmm14_5 = UINT128 (regs.xmm14), xmm15_5 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe230_5_64 = *((UINT64 *) 0x7fffffffe230);
		const UINT32 m7fffffffe238_5_32 = *((UINT32 *) 0x7fffffffe238);
		const UINT16 m7fffffffe23c_5_16 = *((UINT16 *) 0x7fffffffe23c);
		const UINT64 m7fffffffe358_5_64 = *((UINT64 *) 0x7fffffffe358);
				func_5 (regs, m7fffffffe230_5_64, m7fffffffe238_5_32, m7fffffffe23c_5_16, m7fffffffe358_5_64, n_v_argv_4_0, n_v_argv_4_1, rbp_0, rdi_0, rsi_0, rsp_0);
	}
}
int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	*((UINT32 *) 0x7fffffffe228) = 0x0;
	const UINT32 m7fffffffe228_0_32 = *((UINT32 *) 0x7fffffffe228);
	const UINT64 m7fffffffe258_0_64 = *((UINT64 *) 0x7fffffffe258);
	const UINT64 m7fffffffe340_0_64 = *((UINT64 *) 0x7fffffffe340);
	{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
		*((UINT64 *) 0x7fffffffe228) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe228_0_32) /*0x600000000*/);
		*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe258) = UINT64 (m7fffffffe258_0_64 /*0x7ffff5feaf45*/);
		*((UINT64 *) 0x7fffffffe340) = UINT64 (argv[1] /*0x7fffffffe57c*/);
		/*Registers Changes*/
		regs.rax = UINT64 (argv[1] /*0x7fffffffe57c*/);
		regs.rdi = UINT64 (argv[1] /*0x7fffffffe57c*/);
		regs.rsi = UINT64 (0x4007f4);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
		regs.rax = strcmp (/*"left"*/ (const char *) m7fffffffe340_0_64 /*0x7fffffffe57c*/, /*"left"*/ (const char *) 0x4007f4);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
		const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
		const UINT64 m7fffffffe248_1_64 = *((UINT64 *) 0x7fffffffe248);
		const UINT64 m7fffffffe348_1_64 = *((UINT64 *) 0x7fffffffe348);
		if (((UINT64 (rax_1) & 0xffffffff) /*0x0*/ == 0)) {
			if (cond_3 (UINT64 (argv[2][0]))) {
				if (cond_4 (UINT64 (argv[2][1]))) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (((((signExtend_0x20_0x8 (argv[2][1]) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (argv[2][0]) - 0x30) & 0xffffffff)) /*0x2*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_1_64 /*0x0*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_1_64 /*0x7fffffffe581*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_1 /*0x0*/);
					regs.rdi = UINT64 (0x4007f9);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
					regs.rax = puts (/*"x < 4 | y < 7"*/ (const char *) 0x4007f9);
					const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
					const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
					const UINT64 m7fffffffe350_2_64 = *((UINT64 *) 0x7fffffffe350);
										func_1 (regs, UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsp_0));
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (((((signExtend_0x20_0x8 (argv[2][1]) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (argv[2][0]) - 0x30) & 0xffffffff)) /*0x0*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_1_64 /*0x0*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_1_64 /*0x7fffffffe581*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_1 /*0x0*/);
					regs.rdi = UINT64 (0x400807);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
					regs.rax = puts (/*"x < 4 | y >= 7"*/ (const char *) 0x400807);
					const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
					const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
					const UINT64 m7fffffffe350_2_64 = *((UINT64 *) 0x7fffffffe350);
										func_1 (regs, UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsp_0));
				}
			} else {
				if (cond_13 (UINT64 (argv[2][1]))) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (((((signExtend_0x20_0x8 (argv[2][1]) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (argv[2][0]) - 0x30) & 0xffffffff)) /*0x40*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_1_64 /*0x0*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_1_64 /*0x7fffffffe581*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_1 /*0x0*/);
					regs.rdi = UINT64 (0x400816);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
					regs.rax = puts (/*"x >= 4 | y < 7"*/ (const char *) 0x400816);
					const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
					const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
					const UINT64 m7fffffffe350_2_64 = *((UINT64 *) 0x7fffffffe350);
										func_1 (regs, UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsp_0));
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (((((signExtend_0x20_0x8 (argv[2][1]) - 0x30) << 0x20) & 0xffffffff00000000) | ((signExtend_0x20_0x8 (argv[2][0]) - 0x30) & 0xffffffff)) /*0x40*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (m7fffffffe248_1_64 /*0x0*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe348) = UINT64 (m7fffffffe348_1_64 /*0x7fffffffe581*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_1 /*0x0*/);
					regs.rdi = UINT64 (0x400825);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
					regs.rax = puts (/*"x >= 4 | y >= 7"*/ (const char *) 0x400825);
					const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
					const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
					const UINT64 m7fffffffe350_2_64 = *((UINT64 *) 0x7fffffffe350);
					{
						/*Memory Changes*/
						*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
						/*Registers Changes*/
						regs.rax = UINT64 ((((signExtend_0x20_0x8 (argv[2][1]) - 0x30) + (signExtend_0x20_0x8 (argv[2][0]) - 0x30)) & 0xffffffff) /*0x40*/);
						regs.rdx = UINT64 (((signExtend_0x20_0x8 (argv[2][0]) - 0x30) & 0xffffffff) /*0x40*/);
						regs.rdi = UINT64 (0x400835);
						regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
						regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
						regs.rax = puts (/*"x + y > 5"*/ (const char *) 0x400835);
						const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
						const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
												func_2 (regs, UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsp_0));
					}
				}
			}
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
			*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_1 /*0x1*/);
			regs.rdi = UINT64 (0x40084a);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
			regs.rax = puts (/*"right"*/ (const char *) 0x40084a);
			const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
			const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
			const UINT64 m7fffffffe350_2_64 = *((UINT64 *) 0x7fffffffe350);
			{
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
				*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
				*((UINT64 *) 0x7fffffffe350) = UINT64 (m7fffffffe350_2_64 /*0x7fffffffe584*/);
				/*Registers Changes*/
				regs.rax = UINT64 (m7fffffffe350_2_64 /*0x7fffffffe584*/);
				regs.rdi = UINT64 (m7fffffffe350_2_64 /*0x7fffffffe584*/);
				regs.rsi = UINT64 (0x400850);
				regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
				regs.rax = strcmp (/*"enter"*/ (const char *) m7fffffffe350_2_64 /*0x7fffffffe584*/, /*"enter"*/ (const char *) 0x400850);
				const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
				const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
				if (((UINT64 (rax_3) & 0xffffffff) /*0x0*/ == 0)) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_3 /*0x0*/);
					regs.rdi = UINT64 (0x400856);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
					regs.rax = puts (/*"entering the room"*/ (const char *) 0x400856);
					const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
					const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
					const UINT64 m7fffffffe240_4_64 = *((UINT64 *) 0x7fffffffe240);
					const UINT64 m7fffffffe348_4_64 = *((UINT64 *) 0x7fffffffe348);
										func_4 (regs, UINT64 (m7fffffffe240_4_64), UINT64 (m7fffffffe348_4_64), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[4][0]), UINT64 (argv[4][1]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe220) = UINT64 (rsi_0 /*0x7fffffffe338*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (rbp_0 /*0x0*/);
					/*Registers Changes*/
					regs.rax = UINT64 (rax_3 /*0x1*/);
					regs.rdi = UINT64 (0x400868);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe220*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe250*/);
					regs.rax = puts (/*"leaving the room"*/ (const char *) 0x400868);
					const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
					const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
					const UINT64 m7fffffffe240_4_64 = *((UINT64 *) 0x7fffffffe240);
					const UINT64 m7fffffffe348_4_64 = *((UINT64 *) 0x7fffffffe348);
										func_4 (regs, UINT64 (m7fffffffe240_4_64), UINT64 (m7fffffffe348_4_64), UINT64 (argv[2][0]), UINT64 (argv[2][1]), UINT64 (argv[4][0]), UINT64 (argv[4][1]), UINT64 (rbp_0), UINT64 (rdi_0), UINT64 (rsi_0), UINT64 (rsp_0));
				}
			}
		}
	}
}
