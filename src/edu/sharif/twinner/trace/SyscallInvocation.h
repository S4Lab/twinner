//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef SYSCALL_INVOCATION_H
#define SYSCALL_INVOCATION_H

#include "TraceSegmentTerminator.h"

#include "Syscall.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class SyscallInvocation : public TraceSegmentTerminator {
private:
  Syscall syscall;

public:
  SyscallInvocation (Syscall syscall);
  virtual ~SyscallInvocation ();

  Syscall getSyscall () const;

  virtual std::string getCallingLine () const;
  virtual std::string toString () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static SyscallInvocation *loadFromBinaryStream (std::ifstream &in);
};

}
}
}
}

#endif /* SyscallInvocation.h */
