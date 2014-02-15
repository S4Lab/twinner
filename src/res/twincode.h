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

typedef unsigned long long int UINT64;

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
    "movq   %%r15,  120+%"  #REGID "\n\t"

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
    "movq   56+%"   #REGID ", %%rbp\n\t"

#define SAVE_REGISTERS(REGS) \
  asm ( \
    SAVING_REGISTERS_COMMANDS (0) \
    : "=o" (REGS) \
  )

struct RegistersSet setRegistersValuesAndInvokeSyscall (struct RegistersSet regs) {
  static struct RegistersSet currentRegs; // static => to be coded independent of rsp and so on
  SAVE_REGISTERS (currentRegs);
  static struct RegistersSet resultingRegs;
  asm (
        /* loading registers' values from regs (the %1) variable */
        LOADING_REGISTERS_COMMANDS (1)
        /* the syscall itself */
        "syscall\n\t" /* FIXME: the syscall code should specify its args/aux-info */
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

#endif	/* twincode.h */
