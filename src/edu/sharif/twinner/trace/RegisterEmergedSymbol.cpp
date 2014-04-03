//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information 
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "RegisterEmergedSymbol.h"

#include "ConcreteValue64Bits.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

RegisterEmergedSymbol::RegisterEmergedSymbol (const RegisterEmergedSymbol &s) :
Symbol (s), address (s.address) {
}

RegisterEmergedSymbol::RegisterEmergedSymbol (REG addr) :
Symbol (), address (addr) {
}

RegisterEmergedSymbol::RegisterEmergedSymbol (REG _address,
    const ConcreteValue &concreteValue, int generationIndex) :
Symbol (concreteValue, generationIndex), address (_address) {
}

RegisterEmergedSymbol *RegisterEmergedSymbol::clone () const {
  return new RegisterEmergedSymbol (*this);
}

std::pair < int, SymbolRecord > RegisterEmergedSymbol::toSymbolRecord () const {
  throw std::runtime_error ("Not implemented yet");
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
    UINT64 value) {
  const int separator = name.find ('_');
  const REG reg = getRegisterFromName (name.substr (0, separator));
  std::stringstream ss (name.substr (separator + 1));
  int generationIndex;
  ss >> std::hex >> generationIndex;
  return new RegisterEmergedSymbol (reg, ConcreteValue64Bits (value), generationIndex);
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
  // ASSERT: address is a 64 bits or 128 bits register
  // Above assertion is ensured during instantiation in the ExpressionImp class
  switch (address) {
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
  default:
    throw std::runtime_error ("Register emerged symbols must correspond to 64 bits "
                              "or 128 bits (XMM series) regs");
  }
}

REG RegisterEmergedSymbol::getRegisterFromName (const std::string &name) {
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
  } else {
    const std::string msg = "Unknown Register Name";
    throw std::runtime_error (msg + name);
  }
}

}
}
}
}
