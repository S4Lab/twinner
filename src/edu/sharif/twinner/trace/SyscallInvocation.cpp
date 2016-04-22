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

#include "SyscallInvocation.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

SyscallInvocation::SyscallInvocation (Syscall _syscall) :
    TraceSegmentTerminator (), syscall (_syscall) {
}

SyscallInvocation::~SyscallInvocation () {
}

Syscall SyscallInvocation::getSyscall () const {
  return syscall;
}

std::string SyscallInvocation::toString () const {
  return "SyscallInvocation ()";
}


}
}
}
}
