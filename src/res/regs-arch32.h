//                   In the name of GOD
/*
 * Twinner; An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TC_REGISTERS_ARCH32_H
#define TC_REGISTERS_ARCH32_H

struct RegistersSet {
  UINT32 eax;
  UINT32 ebx;
  UINT32 ecx;
  UINT32 edx;
  UINT32 edi;
  UINT32 esi;
  UINT32 esp;
  UINT32 ebp;
  UINT64 xmm0[2];
  UINT64 xmm1[2];
  UINT64 xmm2[2];
  UINT64 xmm3[2];
  UINT64 xmm4[2];
  UINT64 xmm5[2];
  UINT64 xmm6[2];
  UINT64 xmm7[2];
};

#define SAVING_REGISTERS_COMMANDS(REGID) \
    "movl   %%eax,  %"      #REGID "\n\t" \
    "movl   %%ebx,  4+%"    #REGID "\n\t" \
    "movl   %%ecx,  8+%"    #REGID "\n\t" \
    "movl   %%edx,  12+%"   #REGID "\n\t" \
    "movl   %%edi,  16+%"   #REGID "\n\t" \
    "movl   %%esi,  20+%"   #REGID "\n\t" \
    "movl   %%esp,  24+%"   #REGID "\n\t" \
    "movl   %%ebp,  28+%"   #REGID "\n\t" \
    "movdqa %%xmm0, 44+%"   #REGID "\n\t" \
    "movdqa %%xmm1, 60+%"   #REGID "\n\t" \
    "movdqa %%xmm2, 76+%"   #REGID "\n\t" \
    "movdqa %%xmm3, 92+%"   #REGID "\n\t" \
    "movdqa %%xmm4, 108+%"  #REGID "\n\t" \
    "movdqa %%xmm5, 124+%"  #REGID "\n\t" \
    "movdqa %%xmm6, 140+%"  #REGID "\n\t" \
    "movdqa %%xmm7, 156+%"  #REGID "\n\t"

#define LOADING_REGISTERS_COMMANDS(REGID) \
    "movl   %"      #REGID ", %%eax\n\t" \
    "movl   4+%"    #REGID ", %%ebx\n\t" \
    "movl   8+%"    #REGID ", %%ecx\n\t" \
    "movl   12+%"   #REGID ", %%edx\n\t" \
    "movl   16+%"   #REGID ", %%edi\n\t" \
    "movl   20+%"   #REGID ", %%esi\n\t" \
    "movl   24+%"   #REGID ", %%esp\n\t" \
    "movdqa 44+%"   #REGID ", %%xmm0\n\t" \
    "movdqa 60+%"   #REGID ", %%xmm1\n\t" \
    "movdqa 76+%"   #REGID ", %%xmm2\n\t" \
    "movdqa 92+%"   #REGID ", %%xmm3\n\t" \
    "movdqa 108+%"  #REGID ", %%xmm4\n\t" \
    "movdqa 124+%"  #REGID ", %%xmm5\n\t" \
    "movdqa 140+%"  #REGID ", %%xmm6\n\t" \
    "movdqa 156+%"  #REGID ", %%xmm7\n\t" \
    "movl   28+%"   #REGID ", %%ebp\n\t"

#define SAVE_REGISTERS(REGS) \
  asm ( \
    SAVING_REGISTERS_COMMANDS (0) \
    : "=o" (REGS) \
  )

#endif /* regs-arch32.h */

