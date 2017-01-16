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

#ifndef SYSCALL_H
#define SYSCALL_H

#include "edu/sharif/twinner/pin-wrapper.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace syscall {

class Syscall {
private:
  SYSCALL_STANDARD standard;

  ADDRINT syscallNumber;
  ADDRINT arg0;
  ADDRINT arg1;
  ADDRINT arg2;
  ADDRINT arg3;
  ADDRINT arg4;
  ADDRINT arg5;

public:
  Syscall ();
  Syscall (SYSCALL_STANDARD standard);
  Syscall (ADDRINT syscallNumber,
      ADDRINT arg0, ADDRINT arg1, ADDRINT arg2, ADDRINT arg3,
      ADDRINT arg4, ADDRINT arg5);
  ~Syscall ();

  std::string getRepresentation () const;

  ADDRINT getSyscallNumber () const;
  ADDRINT getArg0 () const;
  ADDRINT getArg1 () const;
  ADDRINT getArg2 () const;
  ADDRINT getArg3 () const;
  ADDRINT getArg4 () const;
  ADDRINT getArg5 () const;

  virtual bool isProcessTerminatingSyscall () const;
  virtual ADDRINT getExitCodeArgument () const;

protected:

  struct SyscallInformation {
    unsigned int number;
    const char *name;
  };

  virtual const SyscallInformation *begin () const;
  virtual const SyscallInformation *end () const;
};

}
}
}
}
}

#endif /* Syscall.h */
