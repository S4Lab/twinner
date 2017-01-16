//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2017 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef X86_WINDOWS7_SP1_SYSCALL_H
#define X86_WINDOWS7_SP1_SYSCALL_H

#include "Syscall.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace syscall {

class X86Windows7Sp1Syscall : public Syscall {
public:
  X86Windows7Sp1Syscall (ADDRINT syscallNumber,
      ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3,
      ADDRINT arg4, ADDRINT arg5);
  virtual ~X86Windows7Sp1Syscall ();

  virtual bool isProcessTerminatingSyscall () const;
  virtual ADDRINT getExitCodeArgument () const;

protected:
  virtual const SyscallInformation *begin () const;
  virtual const SyscallInformation *end () const;

private:
  static const SyscallInformation SYSCALLS_TABLE[406];
};

}
}
}
}
}

#endif /* X86Windows7Sp1Syscall.h */
