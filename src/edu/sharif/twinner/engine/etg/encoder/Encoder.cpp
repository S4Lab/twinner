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

#include "Encoder.h"

#include <set>

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {

Encoder::Encoder (const AddrToSizeMap &_addressToSize) :
    addressToSize (_addressToSize) {
}

Encoder::~Encoder () {
}

void Encoder::declareMemorySymbols (IndentedStream &body,
    const std::set < AddrToSize > &addrToSize, int index) {
  for (std::set < AddrToSize >::const_iterator it = addrToSize.begin ();
      it != addrToSize.end (); ++it) {
    body.indented () << "const UINT" << std::dec << it->second
        << " m" << std::hex << it->first << "_" << index
        << "_" << std::dec << it->second
        << " = *((UINT" << it->second << " *) 0x"
        << std::hex << it->first << ");\n";
  }
}

void Encoder::declareRegisterSymbols (IndentedStream &body, int index) {
  const char *registersNames[] = {
#ifdef TARGET_IA32E
    "rax", "rbx", "rcx", "rdx", "rdi", "rsi", "rsp", "rbp",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
#else
    "eax", "ebx", "ecx", "edx", "edi", "esi", "esp", "ebp"
#endif
  };
  body.indented () << std::dec << "const " VAR_TYPE " "
      << registersNames[0] << "_" << index << " = regs." << registersNames[0];
  for (int i = 1;
#ifdef TARGET_IA32E
      i < 16;
#else
      i < 8;
#endif
      ++i) {
    body << ", " << registersNames[i] << "_" << index
        << " = regs." << registersNames[i];
  }
  body << ";\n";
  body.indented () << std::dec << "const UINT128 xmm0_" << index
      << " = UINT128 (regs.xmm0)";
  for (int i = 1;
#ifdef TARGET_IA32E
      i < 16;
#else
      i < 8;
#endif
      ++i) {
    body << ", xmm" << i << "_" << index << " = UINT128 (regs.xmm" << i << ")";
  }
  body << ";\n";
}


}
}
}
}
}
}
