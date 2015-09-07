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

#include "NamedSymbol.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

NamedSymbol::NamedSymbol (const NamedSymbol &symbol) :
    Symbol (symbol), name (symbol.name), constant (symbol.constant) {
}

NamedSymbol::NamedSymbol (std::string _name, bool _constant) :
    Symbol (), name (_name), constant (_constant) {
}

NamedSymbol::NamedSymbol (std::string _name, bool _constant,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex) :
    Symbol (concreteValue, generationIndex),
    name (_name), constant (_constant) {
}

NamedSymbol *NamedSymbol::clone () const {
  return new NamedSymbol (*this);
}

std::pair < int, SymbolRecord > NamedSymbol::toSymbolRecord () const {
  throw std::runtime_error ("NamedSymbol::toSymbolRecord () is called");
}

void NamedSymbol::saveToBinaryStream (std::ofstream &out) const {
  out.write ("N", 1);
  UINT32 size = name.length ();
  out.write ((const char *) &size, sizeof (size));
  out.write (name.c_str (), name.length ());
  out.write (constant ? "C" : "V", 1);
  Symbol::saveToBinaryStream (out);
}

NamedSymbol *NamedSymbol::loadFromBinaryStream (std::ifstream &in) {
  UINT32 size;
  in.read ((char *) &size, sizeof (size));
  char *str = new char [size];
  in.read (str, size);
  std::string name = str;
  delete[] str;
  char c;
  in.read (&c, 1);
  bool constant;
  switch (c) {
  case 'C':
    constant = true;
    break;
  case 'V':
    constant = false;
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown NamedSymbol constant value;"
        " C or V was expected but got " << c << '\n';
    throw std::runtime_error ("Unknown NamedSymbol constant value");
  }
  NamedSymbol *symbol = new NamedSymbol (name, constant);
  symbol->Symbol::loadFromBinaryStream (in);
  return symbol;
}

std::string NamedSymbol::toString () const {
  return name;
}

bool NamedSymbol::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const NamedSymbol *> (&token)
      && static_cast<const NamedSymbol *> (&token)->generationIndex ==
      generationIndex
      && static_cast<const NamedSymbol *> (&token)->name == name
      && static_cast<const NamedSymbol *> (&token)->constant == constant;
}

bool NamedSymbol::isConstant () const {
  return constant;
}

}
}
}
}
}
