
#include "twincode.h"

int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe250_0_64, UINT64 n_v_argv_4, UINT64 rbp_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0);
int func_2 (struct RegistersSet &regs, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (0x0);
		regs.rsp = UINT64 (rsp_0 /*0x7fffffffe268*/);
		regs.rbp = UINT64 (rbp_0 /*0x0*/);
		return int (regs.rax);
	}
}
int func_1 (struct RegistersSet &regs, UINT64 m7fffffffe250_0_64, UINT64 n_v_argv_4, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe258) = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
		regs.rdi = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
		regs.rsi = UINT64 (0x400801);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = strcmp (/*"verbose"*/ (const char *) n_v_argv_4 /*0x7fffffffe598*/, /*"verbose"*/ (const char *) 0x400801);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
		if (((rax_2 & 0xffffffff) /*0x0*/ == 0)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe250_0_64 & 0xffffffff00000000) | 0x1) /*0x1*/);
			*((UINT64 *) 0x7fffffffe258) = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
			regs.rdi = UINT64 (0x400810);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rax = puts (/*"Three coordinates indicate a 3D position"*/ (const char *) 0x400810);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
			{
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				/*Registers Changes*/
				regs.rdi = UINT64 (0x400840);
				regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"We are at top of a plane right now!"*/ (const char *) 0x400840);
				const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
				const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
								func_2 (regs, rbp_0, rsp_0);
			}
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe250) = UINT64 ((m7fffffffe250_0_64 & 0xffffffff00000000) /*0x0*/);
			*((UINT64 *) 0x7fffffffe258) = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_2 /*0x1*/);
			regs.rdi = UINT64 (0x400864);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rax = puts (/*"_quiet output_"*/ (const char *) 0x400864);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
						func_2 (regs, rbp_0, rsp_0);
		}
	}
}
int func_3 (struct RegistersSet &regs, UINT64 m7fffffffe250_0_64, UINT64 n_v_argv_4, UINT64 rbp_0, UINT64 rsp_0);
int func_3 (struct RegistersSet &regs, UINT64 m7fffffffe250_0_64, UINT64 n_v_argv_4, UINT64 rbp_0, UINT64 rsp_0) {
{
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe258) = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		/*Registers Changes*/
		regs.rax = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
		regs.rdi = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
		regs.rsi = UINT64 (0x400801);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = strcmp (/*"verbose"*/ (const char *) n_v_argv_4 /*0x7fffffffe598*/, /*"verbose"*/ (const char *) 0x400801);
		const UINT64 rax_2 = regs.rax, rbx_2 = regs.rbx, rcx_2 = regs.rcx, rdx_2 = regs.rdx, rdi_2 = regs.rdi, rsi_2 = regs.rsi, rsp_2 = regs.rsp, rbp_2 = regs.rbp, r8_2 = regs.r8, r9_2 = regs.r9, r10_2 = regs.r10, r11_2 = regs.r11, r12_2 = regs.r12, r13_2 = regs.r13, r14_2 = regs.r14, r15_2 = regs.r15;
		const UINT128 xmm0_2 = UINT128 (regs.xmm0), xmm1_2 = UINT128 (regs.xmm1), xmm2_2 = UINT128 (regs.xmm2), xmm3_2 = UINT128 (regs.xmm3), xmm4_2 = UINT128 (regs.xmm4), xmm5_2 = UINT128 (regs.xmm5), xmm6_2 = UINT128 (regs.xmm6), xmm7_2 = UINT128 (regs.xmm7), xmm8_2 = UINT128 (regs.xmm8), xmm9_2 = UINT128 (regs.xmm9), xmm10_2 = UINT128 (regs.xmm10), xmm11_2 = UINT128 (regs.xmm11), xmm12_2 = UINT128 (regs.xmm12), xmm13_2 = UINT128 (regs.xmm13), xmm14_2 = UINT128 (regs.xmm14), xmm15_2 = UINT128 (regs.xmm15);
		if (((rax_2 & 0xffffffff) /*0x0*/ == 0)) {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe250) = UINT64 (((m7fffffffe250_0_64 & 0xffffffff) | 0x100000000) /*0xffffe340*/);
			*((UINT64 *) 0x7fffffffe258) = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
			regs.rdi = UINT64 (0x400810);
			regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rax = puts (/*"Three coordinates indicate a 3D position"*/ (const char *) 0x400810);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
			{
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
				/*Registers Changes*/
				regs.rdi = UINT64 (0x4008b8);
				regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
				regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
				regs.rax = puts (/*"We are exploring the downwards plane!"*/ (const char *) 0x4008b8);
				const UINT64 rax_4 = regs.rax, rbx_4 = regs.rbx, rcx_4 = regs.rcx, rdx_4 = regs.rdx, rdi_4 = regs.rdi, rsi_4 = regs.rsi, rsp_4 = regs.rsp, rbp_4 = regs.rbp, r8_4 = regs.r8, r9_4 = regs.r9, r10_4 = regs.r10, r11_4 = regs.r11, r12_4 = regs.r12, r13_4 = regs.r13, r14_4 = regs.r14, r15_4 = regs.r15;
				const UINT128 xmm0_4 = UINT128 (regs.xmm0), xmm1_4 = UINT128 (regs.xmm1), xmm2_4 = UINT128 (regs.xmm2), xmm3_4 = UINT128 (regs.xmm3), xmm4_4 = UINT128 (regs.xmm4), xmm5_4 = UINT128 (regs.xmm5), xmm6_4 = UINT128 (regs.xmm6), xmm7_4 = UINT128 (regs.xmm7), xmm8_4 = UINT128 (regs.xmm8), xmm9_4 = UINT128 (regs.xmm9), xmm10_4 = UINT128 (regs.xmm10), xmm11_4 = UINT128 (regs.xmm11), xmm12_4 = UINT128 (regs.xmm12), xmm13_4 = UINT128 (regs.xmm13), xmm14_4 = UINT128 (regs.xmm14), xmm15_4 = UINT128 (regs.xmm15);
								func_2 (regs, rbp_0, rsp_0);
			}
		} else {
			/*Memory Changes*/
			*((UINT64 *) 0x7fffffffe250) = UINT64 ((m7fffffffe250_0_64 & 0xffffffff) /*0xffffe340*/);
			*((UINT64 *) 0x7fffffffe258) = UINT64 (n_v_argv_4 /*0x7fffffffe598*/);
			*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
			/*Registers Changes*/
			regs.rax = UINT64 (rax_2 /*0x1*/);
			regs.rdi = UINT64 (0x400864);
			regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
			regs.rax = puts (/*"_quiet output_"*/ (const char *) 0x400864);
			const UINT64 rax_3 = regs.rax, rbx_3 = regs.rbx, rcx_3 = regs.rcx, rdx_3 = regs.rdx, rdi_3 = regs.rdi, rsi_3 = regs.rsi, rsp_3 = regs.rsp, rbp_3 = regs.rbp, r8_3 = regs.r8, r9_3 = regs.r9, r10_3 = regs.r10, r11_3 = regs.r11, r12_3 = regs.r12, r13_3 = regs.r13, r14_3 = regs.r14, r15_3 = regs.r15;
			const UINT128 xmm0_3 = UINT128 (regs.xmm0), xmm1_3 = UINT128 (regs.xmm1), xmm2_3 = UINT128 (regs.xmm2), xmm3_3 = UINT128 (regs.xmm3), xmm4_3 = UINT128 (regs.xmm4), xmm5_3 = UINT128 (regs.xmm5), xmm6_3 = UINT128 (regs.xmm6), xmm7_3 = UINT128 (regs.xmm7), xmm8_3 = UINT128 (regs.xmm8), xmm9_3 = UINT128 (regs.xmm9), xmm10_3 = UINT128 (regs.xmm10), xmm11_3 = UINT128 (regs.xmm11), xmm12_3 = UINT128 (regs.xmm12), xmm13_3 = UINT128 (regs.xmm13), xmm14_3 = UINT128 (regs.xmm14), xmm15_3 = UINT128 (regs.xmm15);
						func_2 (regs, rbp_0, rsp_0);
		}
	}
}
int main (int argc, char *argv[]) {
struct RegistersSet regs;
SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT128 xmm0_0 = UINT128 (regs.xmm0), xmm1_0 = UINT128 (regs.xmm1), xmm2_0 = UINT128 (regs.xmm2), xmm3_0 = UINT128 (regs.xmm3), xmm4_0 = UINT128 (regs.xmm4), xmm5_0 = UINT128 (regs.xmm5), xmm6_0 = UINT128 (regs.xmm6), xmm7_0 = UINT128 (regs.xmm7), xmm8_0 = UINT128 (regs.xmm8), xmm9_0 = UINT128 (regs.xmm9), xmm10_0 = UINT128 (regs.xmm10), xmm11_0 = UINT128 (regs.xmm11), xmm12_0 = UINT128 (regs.xmm12), xmm13_0 = UINT128 (regs.xmm13), xmm14_0 = UINT128 (regs.xmm14), xmm15_0 = UINT128 (regs.xmm15);
	*((UINT32 *) 0x7fffffffe238) = 0x0;
	*((UINT64 *) 0x7fffffffe240) = 0x400710;
	*((UINT32 *) 0x7fffffffe248) = 0x400490;
	*((UINT8 *) 0x7fffffffe24c) = 0x0;
	*((UINT64 *) 0x7fffffffe250) = 0x7fffffffe340;
	*((UINT64 *) 0x7fffffffe268) = 0x7ffff5feaf45;
	const UINT32 m7fffffffe238_0_32 = *((UINT32 *) 0x7fffffffe238);
	const UINT64 m7fffffffe240_0_64 = *((UINT64 *) 0x7fffffffe240);
	const UINT32 m7fffffffe248_0_32 = *((UINT32 *) 0x7fffffffe248);
	const UINT8 m7fffffffe24c_0_8 = *((UINT8 *) 0x7fffffffe24c);
	const UINT64 m7fffffffe250_0_64 = *((UINT64 *) 0x7fffffffe250);
	const UINT64 m7fffffffe268_0_64 = *((UINT64 *) 0x7fffffffe268);
	if ((/*unsigned {*/(UINT64 (rdi_0) & 0xffffffff) /*0x5*//*}*/ == /*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x5)/*}*/)) {
		if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x55)/*}*/ == /*unsigned {*/UINT64 (argv[1][0]) /*0x55*//*}*/)) {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x4c)/*}*/ != /*unsigned {*/UINT64 (argv[2][0]) /*0x52*//*}*/)) {
				if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ == /*unsigned {*/UINT64 (argv[3][0]) /*0x46*//*}*/)) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
					*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x500000000*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_0_64 /*0x400710*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (((((((argv[3][0] << 0x8) | argv[2][0]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe24c_0_8)) << 0x20) | m7fffffffe248_0_32) /*0x90*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (m7fffffffe250_0_64 /*0x7fffffffe340*/);
					*((UINT64 *) 0x7fffffffe258) = UINT64 (argv[4] /*0x7fffffffe598*/);
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
					*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe592*/);
					*((UINT64 *) 0x7fffffffe358) = UINT64 (argv[2] /*0x7fffffffe594*/);
					*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[3] /*0x7fffffffe596*/);
					*((UINT64 *) 0x7fffffffe368) = UINT64 (argv[4] /*0x7fffffffe598*/);
					/*Registers Changes*/
					regs.rax = UINT64 (argv[4] /*0x7fffffffe598*/);
					regs.rdi = UINT64 (rdi_0 /*0x5*/);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"Up/Right/Forth"*/ (const char *) 0x4007e4);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
										func_1 (regs, UINT64 (m7fffffffe250_0_64), UINT64 (argv[4]), UINT64 (rbp_0), UINT64 (rsp_0));
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
					*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x500000000*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_0_64 /*0x400710*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (((((((argv[3][0] << 0x8) | argv[2][0]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe24c_0_8)) << 0x20) | m7fffffffe248_0_32) /*0x90*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (m7fffffffe250_0_64 /*0x7fffffffe340*/);
					*((UINT64 *) 0x7fffffffe258) = UINT64 (argv[4] /*0x7fffffffe598*/);
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
					*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe592*/);
					*((UINT64 *) 0x7fffffffe358) = UINT64 (argv[2] /*0x7fffffffe594*/);
					*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[3] /*0x7fffffffe596*/);
					*((UINT64 *) 0x7fffffffe368) = UINT64 (argv[4] /*0x7fffffffe598*/);
					/*Registers Changes*/
					regs.rax = UINT64 (argv[4] /*0x7fffffffe598*/);
					regs.rdi = UINT64 (rdi_0 /*0x5*/);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"Up/Right/Back"*/ (const char *) 0x4007f3);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
										func_1 (regs, UINT64 (m7fffffffe250_0_64), UINT64 (argv[4]), UINT64 (rbp_0), UINT64 (rsp_0));
				}
			} else {
				if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ == /*unsigned {*/UINT64 (argv[3][0]) /*0x46*//*}*/)) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
					*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x500000000*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_0_64 /*0x400710*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (((((((argv[3][0] << 0x8) | argv[2][0]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe24c_0_8)) << 0x20) | m7fffffffe248_0_32) /*0x90*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (m7fffffffe250_0_64 /*0x7fffffffe340*/);
					*((UINT64 *) 0x7fffffffe258) = UINT64 (argv[4] /*0x7fffffffe598*/);
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
					*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe592*/);
					*((UINT64 *) 0x7fffffffe358) = UINT64 (argv[2] /*0x7fffffffe594*/);
					*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[3] /*0x7fffffffe596*/);
					*((UINT64 *) 0x7fffffffe368) = UINT64 (argv[4] /*0x7fffffffe598*/);
					/*Registers Changes*/
					regs.rax = UINT64 (argv[4] /*0x7fffffffe598*/);
					regs.rdi = UINT64 (rdi_0 /*0x5*/);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"Up/Left/Forth"*/ (const char *) 0x4007c9);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
										func_1 (regs, UINT64 (m7fffffffe250_0_64), UINT64 (argv[4]), UINT64 (rbp_0), UINT64 (rsp_0));
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
					*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x500000000*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_0_64 /*0x400710*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (((((((argv[3][0] << 0x8) | argv[2][0]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe24c_0_8)) << 0x20) | m7fffffffe248_0_32) /*0x90*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (m7fffffffe250_0_64 /*0x7fffffffe340*/);
					*((UINT64 *) 0x7fffffffe258) = UINT64 (argv[4] /*0x7fffffffe598*/);
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
					*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe592*/);
					*((UINT64 *) 0x7fffffffe358) = UINT64 (argv[2] /*0x7fffffffe594*/);
					*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[3] /*0x7fffffffe596*/);
					*((UINT64 *) 0x7fffffffe368) = UINT64 (argv[4] /*0x7fffffffe598*/);
					/*Registers Changes*/
					regs.rax = UINT64 (argv[4] /*0x7fffffffe598*/);
					regs.rdi = UINT64 (rdi_0 /*0x5*/);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"Up/Left/Back"*/ (const char *) 0x4007d7);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
										func_1 (regs, UINT64 (m7fffffffe250_0_64), UINT64 (argv[4]), UINT64 (rbp_0), UINT64 (rsp_0));
				}
			}
		} else {
			if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x4c)/*}*/ != /*unsigned {*/UINT64 (argv[2][0]) /*0x52*//*}*/)) {
				if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ == /*unsigned {*/UINT64 (argv[3][0]) /*0x46*//*}*/)) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
					*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x500000000*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_0_64 /*0x400710*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (((((((argv[3][0] << 0x8) | argv[2][0]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe24c_0_8)) << 0x20) | m7fffffffe248_0_32) /*0x90*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (m7fffffffe250_0_64 /*0x7fffffffe340*/);
					*((UINT64 *) 0x7fffffffe258) = UINT64 (argv[4] /*0x7fffffffe598*/);
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
					*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe592*/);
					*((UINT64 *) 0x7fffffffe358) = UINT64 (argv[2] /*0x7fffffffe594*/);
					*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[3] /*0x7fffffffe596*/);
					*((UINT64 *) 0x7fffffffe368) = UINT64 (argv[4] /*0x7fffffffe598*/);
					/*Registers Changes*/
					regs.rax = UINT64 (argv[4] /*0x7fffffffe598*/);
					regs.rdi = UINT64 (rdi_0 /*0x5*/);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"Down/Right/Forth"*/ (const char *) 0x400892);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
										func_3 (regs, UINT64 (m7fffffffe250_0_64), UINT64 (argv[4]), UINT64 (rbp_0), UINT64 (rsp_0));
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
					*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x500000000*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_0_64 /*0x400710*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (((((((argv[3][0] << 0x8) | argv[2][0]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe24c_0_8)) << 0x20) | m7fffffffe248_0_32) /*0x90*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (m7fffffffe250_0_64 /*0x7fffffffe340*/);
					*((UINT64 *) 0x7fffffffe258) = UINT64 (argv[4] /*0x7fffffffe598*/);
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
					*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe592*/);
					*((UINT64 *) 0x7fffffffe358) = UINT64 (argv[2] /*0x7fffffffe594*/);
					*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[3] /*0x7fffffffe596*/);
					*((UINT64 *) 0x7fffffffe368) = UINT64 (argv[4] /*0x7fffffffe598*/);
					/*Registers Changes*/
					regs.rax = UINT64 (argv[4] /*0x7fffffffe598*/);
					regs.rdi = UINT64 (rdi_0 /*0x5*/);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"Down/Right/Back"*/ (const char *) 0x4008a3);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
										func_3 (regs, UINT64 (m7fffffffe250_0_64), UINT64 (argv[4]), UINT64 (rbp_0), UINT64 (rsp_0));
				}
			} else {
				if ((/*unsigned {*/UINT128 (0x0, 0x0, 0x0, 0x46)/*}*/ == /*unsigned {*/UINT64 (argv[3][0]) /*0x46*//*}*/)) {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
					*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x500000000*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_0_64 /*0x400710*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (((((((argv[3][0] << 0x8) | argv[2][0]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe24c_0_8)) << 0x20) | m7fffffffe248_0_32) /*0x90*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (m7fffffffe250_0_64 /*0x7fffffffe340*/);
					*((UINT64 *) 0x7fffffffe258) = UINT64 (argv[4] /*0x7fffffffe598*/);
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
					*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe592*/);
					*((UINT64 *) 0x7fffffffe358) = UINT64 (argv[2] /*0x7fffffffe594*/);
					*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[3] /*0x7fffffffe596*/);
					*((UINT64 *) 0x7fffffffe368) = UINT64 (argv[4] /*0x7fffffffe598*/);
					/*Registers Changes*/
					regs.rax = UINT64 (argv[4] /*0x7fffffffe598*/);
					regs.rdi = UINT64 (rdi_0 /*0x5*/);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"Down/Left/Forth"*/ (const char *) 0x400873);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
										func_3 (regs, UINT64 (m7fffffffe250_0_64), UINT64 (argv[4]), UINT64 (rbp_0), UINT64 (rsp_0));
				} else {
					/*Memory Changes*/
					*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
					*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x500000000*/);
					*((UINT64 *) 0x7fffffffe240) = UINT64 (m7fffffffe240_0_64 /*0x400710*/);
					*((UINT64 *) 0x7fffffffe248) = UINT64 (((((((argv[3][0] << 0x8) | argv[2][0]) << 0x10) | ((argv[1][0] << 0x8) | m7fffffffe24c_0_8)) << 0x20) | m7fffffffe248_0_32) /*0x90*/);
					*((UINT64 *) 0x7fffffffe250) = UINT64 (m7fffffffe250_0_64 /*0x7fffffffe340*/);
					*((UINT64 *) 0x7fffffffe258) = UINT64 (argv[4] /*0x7fffffffe598*/);
					*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
					*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
					*((UINT64 *) 0x7fffffffe350) = UINT64 (argv[1] /*0x7fffffffe592*/);
					*((UINT64 *) 0x7fffffffe358) = UINT64 (argv[2] /*0x7fffffffe594*/);
					*((UINT64 *) 0x7fffffffe360) = UINT64 (argv[3] /*0x7fffffffe596*/);
					*((UINT64 *) 0x7fffffffe368) = UINT64 (argv[4] /*0x7fffffffe598*/);
					/*Registers Changes*/
					regs.rax = UINT64 (argv[4] /*0x7fffffffe598*/);
					regs.rdi = UINT64 (rdi_0 /*0x5*/);
					regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
					regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
					regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
					regs.rax = puts (/*"Down/Left/Back"*/ (const char *) 0x400883);
					const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
					const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
										func_3 (regs, UINT64 (m7fffffffe250_0_64), UINT64 (argv[4]), UINT64 (rbp_0), UINT64 (rsp_0));
				}
			}
		}
	} else {
		/*Memory Changes*/
		*((UINT64 *) 0x7fffffffe230) = UINT64 (rsi_0 /*0x7fffffffe348*/);
		*((UINT64 *) 0x7fffffffe238) = UINT64 ((((rdi_0 & 0xffffffff) << 0x20) | m7fffffffe238_0_32) /*0x0*/);
		*((UINT64 *) 0x7fffffffe260) = UINT64 (rbp_0 /*0x0*/);
		*((UINT64 *) 0x7fffffffe268) = UINT64 (m7fffffffe268_0_64 /*0x7ffff5feaf45*/);
		/*Registers Changes*/
		regs.rdi = UINT64 (rdi_0 /*0x0*/);
		regs.rsi = UINT64 (rsi_0 /*0x7fffffffe348*/);
		regs.rsp = UINT64 (((rsp_0 - 0x38) & 0xffffffffffffffff) /*0x7fffffffe230*/);
		regs.rbp = UINT64 (((rsp_0 - 0x8) & 0xffffffffffffffff) /*0x7fffffffe260*/);
		regs.rax = puts (/*"Usage: program (U|D) (L|R) (F|B) (verbose|quiet)"*/ (const char *) 0x400798);
		const UINT64 rax_1 = regs.rax, rbx_1 = regs.rbx, rcx_1 = regs.rcx, rdx_1 = regs.rdx, rdi_1 = regs.rdi, rsi_1 = regs.rsi, rsp_1 = regs.rsp, rbp_1 = regs.rbp, r8_1 = regs.r8, r9_1 = regs.r9, r10_1 = regs.r10, r11_1 = regs.r11, r12_1 = regs.r12, r13_1 = regs.r13, r14_1 = regs.r14, r15_1 = regs.r15;
		const UINT128 xmm0_1 = UINT128 (regs.xmm0), xmm1_1 = UINT128 (regs.xmm1), xmm2_1 = UINT128 (regs.xmm2), xmm3_1 = UINT128 (regs.xmm3), xmm4_1 = UINT128 (regs.xmm4), xmm5_1 = UINT128 (regs.xmm5), xmm6_1 = UINT128 (regs.xmm6), xmm7_1 = UINT128 (regs.xmm7), xmm8_1 = UINT128 (regs.xmm8), xmm9_1 = UINT128 (regs.xmm9), xmm10_1 = UINT128 (regs.xmm10), xmm11_1 = UINT128 (regs.xmm11), xmm12_1 = UINT128 (regs.xmm12), xmm13_1 = UINT128 (regs.xmm13), xmm14_1 = UINT128 (regs.xmm14), xmm15_1 = UINT128 (regs.xmm15);
				func_2 (regs, UINT64 (rbp_0), UINT64 (rsp_0));
	}
}
