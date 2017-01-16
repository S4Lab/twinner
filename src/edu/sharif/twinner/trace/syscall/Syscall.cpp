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

#include "Syscall.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace syscall {

Syscall::Syscall () :
    standard (SYSCALL_STANDARD_INVALID) {
}

Syscall::Syscall (SYSCALL_STANDARD _standard) :
    standard (_standard) {
}

Syscall::Syscall (ADDRINT _syscallNumber,
    ADDRINT _arg0, ADDRINT _arg1, ADDRINT _arg2, ADDRINT _arg3,
    ADDRINT _arg4, ADDRINT _arg5) :
    standard (SYSCALL_STANDARD_INVALID),
    syscallNumber (_syscallNumber),
    arg0 (_arg0), arg1 (_arg1), arg2 (_arg2), arg3 (_arg3),
    arg4 (_arg4), arg5 (_arg5) {
}

Syscall::~Syscall () {
}

std::string Syscall::getRepresentation () const {
  std::stringstream ss;
  for (const SyscallInformation *si = begin (); si != end (); ++si) {
    if (si->number == syscallNumber) {
      ss << "/*syscall-number=0x" << std::hex << syscallNumber << "*/ "
          << si->name << " (0x" << arg0
          << ", 0x" << arg1 << ", 0x" << arg2 << ", 0x" << arg3
          << ", 0x" << arg4 << ", 0x" << arg5 << ")";
      return ss.str ();
    }
  }
  ss << "GeneralSyscall (syscall-number=0x" << std::hex << syscallNumber
      << ", arg0=0x" << arg0
      << ", arg1=0x" << arg1 << ", arg2=0x" << arg2 << ", arg3=0x" << arg3
      << ", arg4=0x" << arg4 << ", arg5=0x" << arg5 << ")";
  return ss.str ();
}

ADDRINT Syscall::getSyscallNumber () const {
  return syscallNumber;
}

ADDRINT Syscall::getArg0 () const {
  return arg0;
}

ADDRINT Syscall::getArg1 () const {
  return arg1;
}

ADDRINT Syscall::getArg2 () const {
  return arg2;
}

ADDRINT Syscall::getArg3 () const {
  return arg3;
}

ADDRINT Syscall::getArg4 () const {
  return arg4;
}

ADDRINT Syscall::getArg5 () const {
  return arg5;
}

bool Syscall::isProcessTerminatingSyscall () const {
  return false;
}

ADDRINT Syscall::getExitCodeArgument () const {
  return 0;
}

const Syscall::SyscallInformation *Syscall::begin () const {
  return 0;
}

const Syscall::SyscallInformation *Syscall::end () const {
  return 0;
}

}
}
}
}
}
