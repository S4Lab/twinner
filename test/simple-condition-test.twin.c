
#include "twincode.h"

int main () {
	struct RegistersSet regs;
	SAVE_REGISTERS (regs);
	const UINT64 rax_0 = regs.rax, rbx_0 = regs.rbx, rcx_0 = regs.rcx, rdx_0 = regs.rdx, rdi_0 = regs.rdi, rsi_0 = regs.rsi, rsp_0 = regs.rsp, rbp_0 = regs.rbp, r8_0 = regs.r8, r9_0 = regs.r9, r10_0 = regs.r10, r11_0 = regs.r11, r12_0 = regs.r12, r13_0 = regs.r13, r14_0 = regs.r14, r15_0 = regs.r15;
	const UINT64 m7fffffffe0e0_0 = *((UINT64 *) 0x7fffffffe0e0);
	const UINT64 m7fffffffe0e8_0 = *((UINT64 *) 0x7fffffffe0e8);
	const UINT64 m7fffffffe33e_0 = *((UINT64 *) 0x7fffffffe33e);
	const UINT64 m7fffffffe343_0 = *((UINT64 *) 0x7fffffffe343);
	if (((m7fffffffe33e_0 & 0xff) - 0x80) /*0xffffffffffffffe1*/ < 0) {
		if (((m7fffffffe343_0 & 0xff) - 0x80) /*0xffffffffffffffe5*/ < 0) {
			if ((((((m7fffffffe343_0 & 0xff) - 0x30) - 0x3) + 0x4) - ((((m7fffffffe33e_0 & 0xff) - 0x30) + ((m7fffffffe33e_0 & 0xff) - 0x30)) + 0x3)) /*0xffffffffffffffd1*/ != 0) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffdfd0) = rsi_0 /*0x7fffffffe0d8*/;
				*((UINT64 *) 0x7fffffffdfdc) = (rdi_0 & 0xffffffff) /*0x3*/;
				*((UINT64 *) 0x7fffffffdfe0) = (((m7fffffffe343_0 & 0xff) - 0x30) - 0x3) /*0x32*/;
				*((UINT64 *) 0x7fffffffdfe4) = 0x20 /*0x20*/;
				*((UINT64 *) 0x7fffffffdfe8) = ((m7fffffffe343_0 & 0xff) - 0x30) /*0x35*/;
				*((UINT64 *) 0x7fffffffdfec) = ((m7fffffffe33e_0 & 0xff) - 0x30) /*0x31*/;
				*((UINT64 *) 0x7fffffffdff0) = rbp_0 /*0x0*/;
				*((UINT64 *) 0x7fffffffe0e0) = m7fffffffe0e0_0 /*0x7fffffffe33e*/;
				*((UINT64 *) 0x7fffffffe0e8) = m7fffffffe0e8_0 /*0x7fffffffe343*/;
				*((UINT64 *) 0x7fffffffe33e) = m7fffffffe33e_0 /*0x61*/;
				*((UINT64 *) 0x7fffffffe343) = m7fffffffe343_0 /*0x65*/;
				/*Registers Changes*/
				regs.rax = 0x0 /*0x0*/;
				regs.rbx = rbx_0;
				regs.rcx = rcx_0;
				regs.rdx = rdx_0;
				regs.rdi = rdi_0;
				regs.rsi = rsi_0 /*0x7fffffffe0d8*/;
				regs.rsp = (rsp_0 + 0x8) /*0x7fffffffdff8*/;
				regs.rbp = rbp_0 /*0x0*/;
				regs.r8 = r8_0;
				regs.r9 = r9_0;
				regs.r10 = r10_0;
				regs.r11 = r11_0;
				regs.r12 = r12_0;
				regs.r13 = r13_0;
				regs.r14 = r14_0;
				regs.r15 = r15_0;
				regs = setRegistersValuesAndInvokeSyscall (regs);
			}
		}
	}
	if (((m7fffffffe33e_0 & 0xff) - 0x80) /*0xffffffffffffffc1*/ < 0) {
		if (((m7fffffffe343_0 & 0xff) - 0x80) /*0xffffffffffffffd4*/ < 0) {
			if ((((((m7fffffffe343_0 & 0xff) - 0x30) - 0x3) + 0x4) - ((((m7fffffffe33e_0 & 0xff) - 0x30) + ((m7fffffffe33e_0 & 0xff) - 0x30)) + 0x3)) /*0x0*/ == 0) {
				/*Memory Changes*/
				*((UINT64 *) 0x7fffffffdfd0) = rsi_0 /*0x7fffffffe0d8*/;
				*((UINT64 *) 0x7fffffffdfdc) = (rdi_0 & 0xffffffff) /*0x3*/;
				*((UINT64 *) 0x7fffffffdfe0) = (((m7fffffffe343_0 & 0xff) - 0x30) - 0x3) /*0x21*/;
				*((UINT64 *) 0x7fffffffdfe4) = 0x0 /*0x0*/;
				*((UINT64 *) 0x7fffffffdfe8) = ((m7fffffffe343_0 & 0xff) - 0x30) /*0x24*/;
				*((UINT64 *) 0x7fffffffdfec) = ((m7fffffffe33e_0 & 0xff) - 0x30) /*0x11*/;
				*((UINT64 *) 0x7fffffffdff0) = rbp_0 /*0x0*/;
				*((UINT64 *) 0x7fffffffe0e0) = m7fffffffe0e0_0 /*0x7fffffffe33e*/;
				*((UINT64 *) 0x7fffffffe0e8) = m7fffffffe0e8_0 /*0x7fffffffe343*/;
				*((UINT64 *) 0x7fffffffe33e) = m7fffffffe33e_0 /*0x41*/;
				*((UINT64 *) 0x7fffffffe343) = m7fffffffe343_0 /*0x54*/;
				/*Registers Changes*/
				regs.rax = 0x0 /*0x0*/;
				regs.rbx = rbx_0;
				regs.rcx = rcx_0;
				regs.rdx = rdx_0;
				regs.rdi = rdi_0;
				regs.rsi = rsi_0 /*0x7fffffffe0d8*/;
				regs.rsp = (rsp_0 + 0x8) /*0x7fffffffdff8*/;
				regs.rbp = rbp_0 /*0x0*/;
				regs.r8 = r8_0;
				regs.r9 = r9_0;
				regs.r10 = r10_0;
				regs.r11 = r11_0;
				regs.r12 = r12_0;
				regs.r13 = r13_0;
				regs.r14 = r14_0;
				regs.r15 = r15_0;
				regs = setRegistersValuesAndInvokeSyscall (regs);
			}
		}
	}
}
