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

#include <sstream>

#include "RegisterEmergedSymbol.h"

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

std::string RegisterEmergedSymbol::toString () const {
  std::stringstream ss;
  ss << std::hex << getRegisterName () << '_' << generationIndex;
  return ss.str ();
}

const char *RegisterEmergedSymbol::getRegisterName () const {
  // ASSERT: address is a 64 bits register
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
  default:
    throw std::runtime_error ("Register emerged symbols must correspond to 64 bits regs");
  }
}

}
}
}
}
