//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "RegisterEmergedSymbol.h"

#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue32Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

RegisterEmergedSymbol::RegisterEmergedSymbol (const RegisterEmergedSymbol &s) :
    Symbol (s), address (s.address) {
}

RegisterEmergedSymbol::RegisterEmergedSymbol (REG addr) :
    Symbol (), address (addr) {
}

RegisterEmergedSymbol::RegisterEmergedSymbol (REG _address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex) :
    Symbol (concreteValue, generationIndex), address (_address) {
}

RegisterEmergedSymbol *RegisterEmergedSymbol::clone () const {
  return new RegisterEmergedSymbol (*this);
}

std::pair < int, SymbolRecord > RegisterEmergedSymbol::toSymbolRecord () const {
  SymbolRecord record;
  record.address = address;
#ifdef TARGET_IA32E
  if (dynamic_cast<const edu::sharif::twinner::trace::cv::ConcreteValue64Bits *>
      (concreteValue)) {
    record.type = REGISTER_64_BITS_SYMBOL_TYPE;
    record.concreteValueLsb =
        static_cast<const edu::sharif::twinner::trace::cv::ConcreteValue64Bits *>
        (concreteValue)->getValue ();
    record.concreteValueMsb = 0;
#else
  if (dynamic_cast<const edu::sharif::twinner::trace::cv::ConcreteValue32Bits *>
      (concreteValue)) {
    record.type = REGISTER_32_BITS_SYMBOL_TYPE;
    record.concreteValueLsb = concreteValue->toUint64 ();
    record.concreteValueMsb = 0;
#endif
  } else if (dynamic_cast<const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *>
      (concreteValue)) {
    record.type = REGISTER_128_BITS_SYMBOL_TYPE;
    record.concreteValueLsb =
        static_cast<const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *>
        (concreteValue)->getLsb ();
    record.concreteValueMsb =
        static_cast<const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *>
        (concreteValue)->getMsb ();
  } else {
    edu::sharif::twinner::util::Logger::error ()
        << "RegisterEmergedSymbol::toSymbolRecord () method: "
        "Unsupported concrete value type.\n";
    abort ();
  }
  return make_pair (generationIndex, record);
}

REG RegisterEmergedSymbol::getAddress () const {
  return address;
}

void RegisterEmergedSymbol::saveToBinaryStream (std::ofstream &out) const {
  out.write ("R", 1);
  out.write ((const char *) &address, sizeof (address));
  Symbol::saveToBinaryStream (out);
}

RegisterEmergedSymbol *RegisterEmergedSymbol::loadFromBinaryStream (std::ifstream &in) {
  REG address;
  in.read ((char *) &address, sizeof (address));
  RegisterEmergedSymbol *symbol = new RegisterEmergedSymbol (address);
  symbol->Symbol::loadFromBinaryStream (in);
  return symbol;
}

RegisterEmergedSymbol *RegisterEmergedSymbol::fromNameAndValue (const std::string &name,
    UINT32 v4, UINT32 v3, UINT32 v2, UINT32 v1) {
  const int separator = name.find ('_');
  const REG reg = getRegisterFromName (name.substr (0, separator));
  std::stringstream ss (name.substr (separator + 1));
  int generationIndex;
  ss >> std::hex >> generationIndex;
  if (is128BitsRegister (reg)) {
    const UINT64 high = (UINT64 (v4) << 32) | v3;
    const UINT64 low = (UINT64 (v2) << 32) | v1;
    return new RegisterEmergedSymbol
        (reg, edu::sharif::twinner::trace::cv::ConcreteValue128Bits (high, low),
         generationIndex);

  } else {
#ifdef TARGET_IA32E
    if (v4 != 0 || v3 != 0) {
      edu::sharif::twinner::util::Logger::error ()
          << "RegisterEmergedSymbol::fromNameAndValue (...): "
          "Illegal value: It has not more than 64 bits.\n";
      abort ();
    }
    const UINT64 value = (UINT64 (v2) << 32) | v1;
    return new RegisterEmergedSymbol
        (reg, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value), generationIndex);
#else
    if (v4 != 0 || v3 != 0 || v2 != 0) {
      edu::sharif::twinner::util::Logger::error ()
          << "RegisterEmergedSymbol::fromNameAndValue (...): "
          "Illegal value: It has not more than 32 bits.\n";
      abort ();
    }
    return new RegisterEmergedSymbol
        (reg, edu::sharif::twinner::trace::cv::ConcreteValue32Bits (v1), generationIndex);
#endif
  }
}

RegisterEmergedSymbol *RegisterEmergedSymbol::fromNameAndValue (const std::string &name,
    const edu::sharif::twinner::trace::cv::ConcreteValue &value) {
  const int separator = name.find ('_');
  const REG reg = getRegisterFromName (name.substr (0, separator));
  std::stringstream ss (name.substr (separator + 1));
  int generationIndex;
  ss >> std::hex >> generationIndex;
  return new RegisterEmergedSymbol (reg, value, generationIndex);
}

std::string RegisterEmergedSymbol::toString () const {
  std::stringstream ss;
  ss << std::hex << getRegisterName () << '_' << generationIndex;
  return ss.str ();
}

bool RegisterEmergedSymbol::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const RegisterEmergedSymbol *> (&token)
      && static_cast<const RegisterEmergedSymbol *> (&token)->generationIndex ==
      generationIndex
      && static_cast<const RegisterEmergedSymbol *> (&token)->address == address;
}

const char *RegisterEmergedSymbol::getRegisterName () const {
#ifdef TARGET_IA32E
  // ASSERT: address is a 16, 64, or 128 bits register
#else
  // ASSERT: address is a 16, 32, or 128 bits register
#endif
  // Above assertion is ensured during instantiation in the ExpressionImp class
  switch (address) {
#ifdef TARGET_IA32E
  case REG_RAX:
    return "rax";
  case REG_RBX:
    return "rbx";
  case REG_RCX:
    return "rcx";
  case REG_RDX:
    return "rdx";
  case REG_RDI:
    return "rdi";
  case REG_RSI:
    return "rsi";
  case REG_RSP:
    return "rsp";
  case REG_RBP:
    return "rbp";
#else
  case REG_EAX:
    return "eax";
  case REG_EBX:
    return "ebx";
  case REG_ECX:
    return "ecx";
  case REG_EDX:
    return "edx";
  case REG_EDI:
    return "edi";
  case REG_ESI:
    return "esi";
  case REG_ESP:
    return "esp";
  case REG_EBP:
    return "ebp";
#endif
#ifdef TARGET_IA32E
  case REG_R8:
    return "r8";
  case REG_R9:
    return "r9";
  case REG_R10:
    return "r10";
  case REG_R11:
    return "r11";
  case REG_R12:
    return "r12";
  case REG_R13:
    return "r13";
  case REG_R14:
    return "r14";
  case REG_R15:
    return "r15";
#endif
  case REG_XMM0:
    return "xmm0";
  case REG_XMM1:
    return "xmm1";
  case REG_XMM2:
    return "xmm2";
  case REG_XMM3:
    return "xmm3";
  case REG_XMM4:
    return "xmm4";
  case REG_XMM5:
    return "xmm5";
  case REG_XMM6:
    return "xmm6";
  case REG_XMM7:
    return "xmm7";
#ifdef TARGET_IA32E
  case REG_XMM8:
    return "xmm8";
  case REG_XMM9:
    return "xmm9";
  case REG_XMM10:
    return "xmm10";
  case REG_XMM11:
    return "xmm11";
  case REG_XMM12:
    return "xmm12";
  case REG_XMM13:
    return "xmm13";
  case REG_XMM14:
    return "xmm14";
  case REG_XMM15:
    return "xmm15";
#endif
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Register emerged symbols must correspond to "
        "16 bits (segment registers) "
#ifdef TARGET_IA32E
        "64 bits "
#else
        "32 bits "
#endif
        "or 128 bits (XMM series) registers\n";
    abort ();
  }
}

REG RegisterEmergedSymbol::getRegisterFromName (const std::string &name) {
#ifdef TARGET_IA32E
  if (name == "rax") {
    return REG_RAX;
  } else if (name == "rbx") {
    return REG_RBX;
  } else if (name == "rcx") {
    return REG_RCX;
  } else if (name == "rdx") {
    return REG_RDX;
  } else if (name == "rdi") {
    return REG_RDI;
  } else if (name == "rsi") {
    return REG_RSI;
  } else if (name == "rsp") {
    return REG_RSP;
  } else if (name == "rbp") {
    return REG_RBP;
#else
  if (name == "eax") {
    return REG_EAX;
  } else if (name == "ebx") {
    return REG_EBX;
  } else if (name == "ecx") {
    return REG_ECX;
  } else if (name == "edx") {
    return REG_EDX;
  } else if (name == "edi") {
    return REG_EDI;
  } else if (name == "esi") {
    return REG_ESI;
  } else if (name == "esp") {
    return REG_ESP;
  } else if (name == "ebp") {
    return REG_EBP;
#endif
#ifdef TARGET_IA32E
  } else if (name == "r8") {
    return REG_R8;
  } else if (name == "r9") {
    return REG_R9;
  } else if (name == "r10") {
    return REG_R10;
  } else if (name == "r11") {
    return REG_R11;
  } else if (name == "r12") {
    return REG_R12;
  } else if (name == "r13") {
    return REG_R13;
  } else if (name == "r14") {
    return REG_R14;
  } else if (name == "r15") {
    return REG_R15;
#endif
  } else if (name == "xmm0") {
    return REG_XMM0;
  } else if (name == "xmm1") {
    return REG_XMM1;
  } else if (name == "xmm2") {
    return REG_XMM2;
  } else if (name == "xmm3") {
    return REG_XMM3;
  } else if (name == "xmm4") {
    return REG_XMM4;
  } else if (name == "xmm5") {
    return REG_XMM5;
  } else if (name == "xmm6") {
    return REG_XMM6;
  } else if (name == "xmm7") {
    return REG_XMM7;
#ifdef TARGET_IA32E
  } else if (name == "xmm8") {
    return REG_XMM8;
  } else if (name == "xmm9") {
    return REG_XMM9;
  } else if (name == "xmm10") {
    return REG_XMM10;
  } else if (name == "xmm11") {
    return REG_XMM11;
  } else if (name == "xmm12") {
    return REG_XMM12;
  } else if (name == "xmm13") {
    return REG_XMM13;
  } else if (name == "xmm14") {
    return REG_XMM14;
  } else if (name == "xmm15") {
    return REG_XMM15;
#endif
  } else {
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown Register Name: " << name << '\n';
    abort ();
  }
}

bool RegisterEmergedSymbol::is128BitsRegister (REG reg) {
  switch (reg) {
  case REG_XMM0:
  case REG_XMM1:
  case REG_XMM2:
  case REG_XMM3:
  case REG_XMM4:
  case REG_XMM5:
  case REG_XMM6:
  case REG_XMM7:
#ifdef TARGET_IA32E
  case REG_XMM8:
  case REG_XMM9:
  case REG_XMM10:
  case REG_XMM11:
  case REG_XMM12:
  case REG_XMM13:
  case REG_XMM14:
  case REG_XMM15:
#endif
    return true;
  default:
    return false;
  }
}

RegisterEmergedSymbol::RegisterType RegisterEmergedSymbol::getRegisterType (REG reg,
    int sizeInBytes) {
  RegisterType regType = (RegisterType) sizeInBytes; // 8, 4, 2, 1
  if (regType == REG_8_BITS_LOWER_HALF_TYPE) {
    if (reg == REG_AH || reg == REG_BH || reg == REG_CH || reg == REG_DH) {
      regType = REG_8_BITS_UPPER_HALF_TYPE;
    }
  }
  return regType;
}

int RegisterEmergedSymbol::getRegisterIndex (REG reg) {
  switch (reg) {
#ifdef TARGET_IA32E
  case REG_RAX:
    return 0;
  case REG_RBX:
    return 1;
  case REG_RCX:
    return 2;
  case REG_RDX:
    return 3;
  case REG_RDI:
    return 4;
  case REG_RSI:
    return 5;
  case REG_RSP:
    return 6;
  case REG_RBP:
    return 7;
#else
  case REG_EAX:
    return 0;
  case REG_EBX:
    return 1;
  case REG_ECX:
    return 2;
  case REG_EDX:
    return 3;
  case REG_EDI:
    return 4;
  case REG_ESI:
    return 5;
  case REG_ESP:
    return 6;
  case REG_EBP:
    return 7;
#endif
#ifdef TARGET_IA32E
  case REG_R8:
    return 8;
  case REG_R9:
    return 9;
  case REG_R10:
    return 10;
  case REG_R11:
    return 11;
  case REG_R12:
    return 12;
  case REG_R13:
    return 13;
  case REG_R14:
    return 14;
  case REG_R15:
    return 15;
#endif
  default:
    return -1;
  }
}

REG RegisterEmergedSymbol::getOverlappingRegisterByIndex (int external, int internal) {
  const REG registers[][5] = {
#ifdef TARGET_IA32E
    {REG_RAX, REG_EAX, REG_AX, REG_AH, REG_AL}, // 0
    {REG_RBX, REG_EBX, REG_BX, REG_BH, REG_BL},
    {REG_RCX, REG_ECX, REG_CX, REG_CH, REG_CL},
    {REG_RDX, REG_EDX, REG_DX, REG_DH, REG_DL},
    {REG_RDI, REG_EDI, REG_DI, REG_INVALID_, REG_DIL}, // 4
    {REG_RSI, REG_ESI, REG_SI, REG_INVALID_, REG_SIL},
    {REG_RSP, REG_ESP, REG_SP, REG_INVALID_, REG_SPL},
    {REG_RBP, REG_EBP, REG_BP, REG_INVALID_, REG_BPL},
#else
    {REG_INVALID_, REG_EAX, REG_AX, REG_AH, REG_AL}, // 0
    {REG_INVALID_, REG_EBX, REG_BX, REG_BH, REG_BL},
    {REG_INVALID_, REG_ECX, REG_CX, REG_CH, REG_CL},
    {REG_INVALID_, REG_EDX, REG_DX, REG_DH, REG_DL},
    {REG_INVALID_, REG_EDI, REG_DI, REG_INVALID_, REG_INVALID_}, // 4
    {REG_INVALID_, REG_ESI, REG_SI, REG_INVALID_, REG_INVALID_},
    {REG_INVALID_, REG_ESP, REG_SP, REG_INVALID_, REG_INVALID_},
    {REG_INVALID_, REG_EBP, REG_BP, REG_INVALID_, REG_INVALID_},
#endif
#ifdef TARGET_IA32E
    {REG_R8, REG_R8D, REG_R8W, REG_INVALID_, REG_R8B}, // 8
    {REG_R9, REG_R9D, REG_R9W, REG_INVALID_, REG_R9B},
    {REG_R10, REG_R10D, REG_R10W, REG_INVALID_, REG_R10B},
    {REG_R11, REG_R11D, REG_R11W, REG_INVALID_, REG_R11B},
    {REG_R12, REG_R12D, REG_R12W, REG_INVALID_, REG_R12B}, // 12
    {REG_R13, REG_R13D, REG_R13W, REG_INVALID_, REG_R13B},
    {REG_R14, REG_R14D, REG_R14W, REG_INVALID_, REG_R14B},
    {REG_R15, REG_R15D, REG_R15W, REG_INVALID_, REG_R15B},
#endif
  };
  return registers[external][internal];
}

void RegisterEmergedSymbol::initializeSubRegisters (REG reg,
    ExecutionTraceSegment *segment, const Expression &expression) {
  int regIndex = getRegisterIndex (reg);
  if (regIndex == -1) { // e.g. xmm registers
    return;
  }
#ifdef TARGET_IA32E
  segment->setSymbolicExpressionByRegister
      (32, getOverlappingRegisterByIndex (regIndex, 1), &expression)->truncate (32);
#endif
  Expression *reg16 = segment->setSymbolicExpressionByRegister
      (16, getOverlappingRegisterByIndex (regIndex, 2), &expression);
  reg16->truncate (16);
  if (getOverlappingRegisterByIndex (regIndex, 3) != REG_INVALID_) {
    edu::sharif::twinner::trace::Expression *temp = reg16->clone (16);
    temp->shiftToRight (8);
    segment->setSymbolicExpressionByRegister
        (8, getOverlappingRegisterByIndex (regIndex, 3), temp);
    delete temp;
  }
  const REG lowest8Bits = getOverlappingRegisterByIndex (regIndex, 4);
  if (lowest8Bits != REG_INVALID_) {
    segment->setSymbolicExpressionByRegister
        (8, lowest8Bits, &expression)->truncate (8);
  }
}

}
}
}
}
}
