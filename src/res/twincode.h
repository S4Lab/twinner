//                   In the name of GOD
/*
 * Twinner; An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TWINCODE_H
#define	TWINCODE_H

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
};

#define SAVE_REGISTERS(REGS) \
  asm ( \
    "movq   %%rax,  %0\n\t" \
    "movq   %%rbx,  8+%0\n\t" \
    "movq   %%rcx,  16+%0\n\t" \
    "movq   %%rdx,  24+%0\n\t" \
    "movq   %%rdi,  32+%0\n\t" \
    "movq   %%rsi,  40+%0\n\t" \
    "movq   %%rsp,  48+%0\n\t" \
    "movq   %%rbp,  56+%0\n\t" \
    "movq   %%r8,   64+%0\n\t" \
    "movq   %%r9,   72+%0\n\t" \
    "movq   %%r10,  80+%0\n\t" \
    "movq   %%r11,  88+%0\n\t" \
    "movq   %%r12,  96+%0\n\t" \
    "movq   %%r13,  104+%0\n\t" \
    "movq   %%r14,  112+%0\n\t" \
    "movq   %%r15,  120+%0\n\t" \
    : "=o" (REGS) \
  )

#endif	/* twincode.h */
