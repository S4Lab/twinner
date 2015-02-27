//                   In the name of GOD
/*
 * Twinner; An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TC_REGISTERS_H
#define TC_REGISTERS_H

struct RegistersSet {

  UINT64 rax;
  UINT64 rbx;
  UINT64 rcx;
  UINT64 rdx;
  UINT64 rdi;
  UINT64 rsi;
  UINT64 rsp;
  UINT64 rbp;
  UINT64 r8;
  UINT64 r9;
  UINT64 r10;
  UINT64 r11;
  UINT64 r12;
  UINT64 r13;
  UINT64 r14;
  UINT64 r15;
  UINT64 xmm0[2];
  UINT64 xmm1[2];
  UINT64 xmm2[2];
  UINT64 xmm3[2];
  UINT64 xmm4[2];
  UINT64 xmm5[2];
  UINT64 xmm6[2];
  UINT64 xmm7[2];
  UINT64 xmm8[2];
  UINT64 xmm9[2];
  UINT64 xmm10[2];
  UINT64 xmm11[2];
  UINT64 xmm12[2];
  UINT64 xmm13[2];
  UINT64 xmm14[2];
  UINT64 xmm15[2];
};

#define SAVING_REGISTERS_COMMANDS(REGID) \
    "movq   %%rax,  %"      #REGID "\n\t" \
    "movq   %%rbx,  8+%"    #REGID "\n\t" \
    "movq   %%rcx,  16+%"   #REGID "\n\t" \
    "movq   %%rdx,  24+%"   #REGID "\n\t" \
    "movq   %%rdi,  32+%"   #REGID "\n\t" \
    "movq   %%rsi,  40+%"   #REGID "\n\t" \
    "movq   %%rsp,  48+%"   #REGID "\n\t" \
    "movq   %%rbp,  56+%"   #REGID "\n\t" \
    "movq   %%r8,   64+%"   #REGID "\n\t" \
    "movq   %%r9,   72+%"   #REGID "\n\t" \
    "movq   %%r10,  80+%"   #REGID "\n\t" \
    "movq   %%r11,  88+%"   #REGID "\n\t" \
    "movq   %%r12,  96+%"   #REGID "\n\t" \
    "movq   %%r13,  104+%"  #REGID "\n\t" \
    "movq   %%r14,  112+%"  #REGID "\n\t" \
    "movq   %%r15,  120+%"  #REGID "\n\t" \
    "movdqa %%xmm0, 128+%"  #REGID "\n\t" \
    "movdqa %%xmm1, 144+%"  #REGID "\n\t" \
    "movdqa %%xmm2, 160+%"  #REGID "\n\t" \
    "movdqa %%xmm3, 176+%"  #REGID "\n\t" \
    "movdqa %%xmm4, 192+%"  #REGID "\n\t" \
    "movdqa %%xmm5, 208+%"  #REGID "\n\t" \
    "movdqa %%xmm6, 224+%"  #REGID "\n\t" \
    "movdqa %%xmm7, 240+%"  #REGID "\n\t" \
    "movdqa %%xmm8, 256+%"  #REGID "\n\t" \
    "movdqa %%xmm9, 272+%"  #REGID "\n\t" \
    "movdqa %%xmm10,288+%"  #REGID "\n\t" \
    "movdqa %%xmm11,304+%"  #REGID "\n\t" \
    "movdqa %%xmm12,320+%"  #REGID "\n\t" \
    "movdqa %%xmm13,336+%"  #REGID "\n\t" \
    "movdqa %%xmm14,352+%"  #REGID "\n\t" \
    "movdqa %%xmm15,368+%"  #REGID "\n\t"

#define LOADING_REGISTERS_COMMANDS(REGID) \
    "movq   64+%"   #REGID ", %%r8\n\t" \
    "movq   72+%"   #REGID ", %%r9\n\t" \
    "movq   80+%"   #REGID ", %%r10\n\t" \
    "movq   88+%"   #REGID ", %%r11\n\t" \
    "movq   96+%"   #REGID ", %%r12\n\t" \
    "movq   104+%"  #REGID ", %%r13\n\t" \
    "movq   112+%"  #REGID ", %%r14\n\t" \
    "movq   120+%"  #REGID ", %%r15\n\t" \
    "movq   %"      #REGID ", %%rax\n\t" \
    "movq   8+%"    #REGID ", %%rbx\n\t" \
    "movq   16+%"   #REGID ", %%rcx\n\t" \
    "movq   24+%"   #REGID ", %%rdx\n\t" \
    "movq   32+%"   #REGID ", %%rdi\n\t" \
    "movq   40+%"   #REGID ", %%rsi\n\t" \
    "movq   48+%"   #REGID ", %%rsp\n\t" \
    "movdqa 128+%"  #REGID ", %%xmm0\n\t" \
    "movdqa 144+%"  #REGID ", %%xmm1\n\t" \
    "movdqa 160+%"  #REGID ", %%xmm2\n\t" \
    "movdqa 176+%"  #REGID ", %%xmm3\n\t" \
    "movdqa 192+%"  #REGID ", %%xmm4\n\t" \
    "movdqa 208+%"  #REGID ", %%xmm5\n\t" \
    "movdqa 224+%"  #REGID ", %%xmm6\n\t" \
    "movdqa 240+%"  #REGID ", %%xmm7\n\t" \
    "movdqa 256+%"  #REGID ", %%xmm8\n\t" \
    "movdqa 272+%"  #REGID ", %%xmm9\n\t" \
    "movdqa 288+%"  #REGID ", %%xmm10\n\t" \
    "movdqa 304+%"  #REGID ", %%xmm11\n\t" \
    "movdqa 320+%"  #REGID ", %%xmm12\n\t" \
    "movdqa 336+%"  #REGID ", %%xmm13\n\t" \
    "movdqa 352+%"  #REGID ", %%xmm14\n\t" \
    "movdqa 368+%"  #REGID ", %%xmm15\n\t" \
    "movq   56+%"   #REGID ", %%rbp\n\t"

#define SAVE_REGISTERS(REGS) \
  asm ( \
    SAVING_REGISTERS_COMMANDS (0) \
    : "=o" (REGS) \
  )

static struct RegistersSet setRegistersValuesAndInvokeSyscall (struct RegistersSet regs) {
  static struct RegistersSet currentRegs; // static => to be coded independent of rbp
  SAVE_REGISTERS (currentRegs);
  static struct RegistersSet resultingRegs;
  asm (
        /* loading registers' values from regs (the %1) variable */
        LOADING_REGISTERS_COMMANDS (1)
        /* the syscall itself */
        "syscall\n\t"
        /* saving resulting registers' values within resultingRegs (the %0) variable */
        SAVING_REGISTERS_COMMANDS (0)
        /* loading registers' old values from currentRegs (the %2) variable */
        LOADING_REGISTERS_COMMANDS (2)

        : "=o" (resultingRegs)
        : "o" (regs), "o" (currentRegs)
        : "cc" /* FIXME: check whether syscall requires clobbering flags or not */
        );
  return resultingRegs;
}

#endif	/* tc-registers.h */

