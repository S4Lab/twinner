//                   In the name of GOD
/*
 * Twinner; An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TC_REGISTERS_H
#define TC_REGISTERS_H

#ifdef ARCHITECTURE_32
#include "regs-arch32.h"
#else
#include "regs-arch64.h"
#endif

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

#endif /* tc-registers.h */

