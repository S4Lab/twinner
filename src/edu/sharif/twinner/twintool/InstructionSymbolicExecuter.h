//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef INSTRUCTION_SYMBOLIC_EXECUTER_H
#define INSTRUCTION_SYMBOLIC_EXECUTER_H

#include "pin.H"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
}
namespace twintool {

class InstructionSymbolicExecuter {
private:
  edu::sharif::twinner::trace::Trace *trace;

public:
  InstructionSymbolicExecuter ();

  edu::sharif::twinner::trace::Trace *getTrace () const;

  void movToRegisterFromMemoryAddress (REG reg, ADDRINT memoryEa);
  void movToMemoryAddressFromRegister (ADDRINT memoryEa, REG reg, UINT64 regval);
  void movToMemoryAddressFromImmediateValue (ADDRINT memoryEa, ADDRINT immediate);
  void movToRegisterFromImmediateValue (REG reg, ADDRINT immediate);
  void movToRegisterFromRegister (REG dreg, REG sreg, UINT64 regsrcval);

  void pushToStackFromRegister (ADDRINT stackEa, REG reg, UINT64 regval);
  void pushToStackFromImmediateValue (ADDRINT stackEa, ADDRINT immediate);
  void pushToStackFromMemoryAddress (ADDRINT stackEa, ADDRINT memoryEa);

private:
  UINT64 readMemoryContent (ADDRINT memoryEa) const;
};

VOID movToRegisterFromMemoryAddress (VOID *iseptr, UINT32 regi32, ADDRINT memoryEa);
VOID movToMemoryAddressFromRegister (VOID *iseptr, ADDRINT memoryEa, UINT32 regi32,
    ADDRINT regval);
VOID movToMemoryAddressFromImmediateValue (VOID *iseptr, ADDRINT memoryEa,
    ADDRINT immediate);
VOID movToRegisterFromImmediateValue (VOID *iseptr, UINT32 regi32, ADDRINT immediate);
VOID movToRegisterFromRegister (VOID *iseptr, UINT32 regdsti32, UINT32 regsrci32,
    ADDRINT regsrcval);

VOID pushToStackFromRegister (VOID *iseptr, ADDRINT stackEa, UINT32 regi32,
    ADDRINT regval);
VOID pushToStackFromImmediateValue (VOID *iseptr, ADDRINT stackEa, ADDRINT immediate);
VOID pushToStackFromMemoryAddress (VOID *iseptr, ADDRINT stackEa, ADDRINT memoryEa);

}
}
}
}

#endif /* InstructionSymbolicExecuter.h */
