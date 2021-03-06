//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
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

#include "edu/sharif/twinner/trace/syscall/Syscall.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class SyscallInvocation : public TraceSegmentTerminator {
private:
  edu::sharif::twinner::trace::syscall::Syscall syscall;

public:
  SyscallInvocation (edu::sharif::twinner::trace::syscall::Syscall syscall);
  virtual ~SyscallInvocation ();

  virtual SyscallInvocation *clone () const;

  edu::sharif::twinner::trace::syscall::Syscall getSyscall () const;

  virtual std::string getCallingLine (bool useNonTechnicalNames) const;
  virtual std::string toString () const;
  virtual void replaceTemporarySymbols (const Snapshot *lastSnapshot);
  virtual bool operator== (const TraceSegmentTerminator &t) const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static SyscallInvocation *loadFromBinaryStream (std::ifstream &in);
};

}
}
}
}

#endif /* SyscallInvocation.h */
