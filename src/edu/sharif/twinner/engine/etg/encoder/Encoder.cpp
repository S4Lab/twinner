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

#include "Encoder.h"

#include <set>

#include "Variable.h"

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
    const std::set < AddrToSize > &addrToSize, int index,
    VariableContainer &vc) {
  for (std::set < AddrToSize >::const_iterator it = addrToSize.begin ();
      it != addrToSize.end (); ++it) {
    std::stringstream ss;
    ss << "m" << std::hex << it->first;
    std::stringstream type;
    type << "UINT" << std::dec << it->second;
    std::stringstream name;
    name << ss.str () << std::hex << "_" << index
        << "_" << std::dec << it->second;
    body.indented () << "const " << type.str ()
        << " " << name.str ()
        << " = *((" << type.str () << " *) 0x"
        << std::hex << it->first << ");\n";
    vc.addVariable (ss.str (),
                    Variable (type.str (), name.str (), name.str (), index));
  }
}

void Encoder::declareRegisterSymbols (IndentedStream &body, int index,
    VariableContainer &vc) {
  const char *registersNames[] = {
#ifdef TARGET_IA32E
    "rax", "rbx", "rcx", "rdx", "rdi", "rsi", "rsp", "rbp",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
#else
    "eax", "ebx", "ecx", "edx", "edi", "esi", "esp", "ebp"
#endif
  };
  {
    std::stringstream ss;
    ss << registersNames[0] << "_" << std::hex << index;
    body.indented () << std::dec << "const " VAR_TYPE " "
        << ss.str () << " = regs." << registersNames[0];
    vc.addVariable (registersNames[0],
                    Variable (VAR_TYPE, ss.str (), ss.str (), index));
  }
  for (int i = 1;
#ifdef TARGET_IA32E
      i < 16;
#else
      i < 8;
#endif
      ++i) {
    std::stringstream ss;
    ss << registersNames[i] << "_" << std::hex << index;
    body << ", " << ss.str ()
        << " = regs." << registersNames[i];
    vc.addVariable (registersNames[i],
                    Variable (VAR_TYPE, ss.str (), ss.str (), index));
  }
  body << ";\n";
  {
    std::stringstream name;
    name << "xmm0_" << std::hex << index;
    body.indented () << std::dec << "const UINT128 " << name.str ()
        << " = UINT128 (regs.xmm0)";
    vc.addVariable ("xmm0",
                    Variable ("UINT128", name.str (), name.str (), index));
  }
  for (int i = 1;
#ifdef TARGET_IA32E
      i < 16;
#else
      i < 8;
#endif
      ++i) {
    std::stringstream ss;
    ss << "xmm" << i;
    std::stringstream name;
    name << ss.str () << "_" << std::hex << index;
    body << ", " << name.str () << " = UINT128 (regs." << ss.str () << ")";
    vc.addVariable (ss.str (),
                    Variable ("UINT128", name.str (), name.str (), index));
  }
  body << ";\n";
}


}
}
}
}
}
}
