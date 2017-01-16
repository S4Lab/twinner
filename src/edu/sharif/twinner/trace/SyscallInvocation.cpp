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

#include "SyscallInvocation.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

SyscallInvocation::SyscallInvocation (
    edu::sharif::twinner::trace::syscall::Syscall _syscall) :
    TraceSegmentTerminator (), syscall (_syscall) {
}

SyscallInvocation::~SyscallInvocation () {
}

edu::sharif::twinner::trace::syscall::Syscall
SyscallInvocation::getSyscall () const {
  return syscall;
}

std::string SyscallInvocation::getCallingLine () const {
  return "regs = setRegistersValuesAndInvokeSyscall (regs);";
}

std::string SyscallInvocation::toString () const {
  return "SyscallInvocation ()";
}

void SyscallInvocation::saveToBinaryStream (std::ofstream &out) const {
  const char *terminatorMagicString = "SYS";
  out.write (terminatorMagicString, 3);
  out.write (reinterpret_cast<const char *> (&syscall), sizeof (syscall));
}

SyscallInvocation *SyscallInvocation::loadFromBinaryStream (std::ifstream &in) {
  edu::sharif::twinner::trace::syscall::Syscall syscall;
  in.read (reinterpret_cast<char *> (&syscall), sizeof (syscall));
  return new SyscallInvocation (syscall);
}


}
}
}
}
