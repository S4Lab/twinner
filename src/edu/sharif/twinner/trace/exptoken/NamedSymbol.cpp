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

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

NamedSymbol::NamedSymbol (const NamedSymbol &symbol) :
    Symbol (symbol), name (symbol.name), technicalName (symbol.technicalName),
    constant (symbol.constant) {
}

NamedSymbol::NamedSymbol (std::string _name, std::string techName,
    bool _constant) :
    Symbol (), name (_name), technicalName (techName), constant (_constant) {
}

NamedSymbol::NamedSymbol (std::string _name, std::string techName,
    bool _constant,
    const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
    int generationIndex) :
    Symbol (concreteValue, generationIndex),
    name (_name), technicalName (techName), constant (_constant) {
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
  size = technicalName.length ();
  out.write ((const char *) &size, sizeof (size));
  out.write (technicalName.c_str (), technicalName.length ());
  out.write (constant ? "C" : "V", 1);
  Symbol::saveToBinaryStream (out);
}

NamedSymbol *NamedSymbol::loadFromBinaryStream (std::ifstream &in) {
  UINT32 size;
  in.read ((char *) &size, sizeof (size));
  char *str = new char [size];
  in.read (str, size);
  const std::string name = str;
  delete[] str;
  in.read ((char *) &size, sizeof (size));
  str = new char [size];
  const std::string techName = str;
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
  NamedSymbol *symbol = new NamedSymbol (name, techName, constant);
  symbol->Symbol::loadFromBinaryStream (in);
  return symbol;
}

std::string NamedSymbol::toString () const {
  return name;
}

std::string NamedSymbol::getTechnicalName () const {
  return technicalName;
}

bool NamedSymbol::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const NamedSymbol *> (&token)
      && static_cast<const NamedSymbol *> (&token)->generationIndex ==
      generationIndex
      && static_cast<const NamedSymbol *> (&token)->name == name
      && static_cast<const NamedSymbol *> (&token)->technicalName ==
      technicalName
      && static_cast<const NamedSymbol *> (&token)->constant == constant;
}

bool NamedSymbol::isConstant () const {
  return constant;
}

NamedSymbol *NamedSymbol::fromTechnicalName (const std::string &name,
    const edu::sharif::twinner::trace::cv::ConcreteValue &value) {
  std::stringstream ss (name);
  char dummy;
  char constant_variable;
  std::string rest;
  ss >> dummy >> dummy >> constant_variable >> dummy >> rest;
  if (rest.size () >= 4 && rest.substr (0, 4) == "argv") {
    if (rest.size () == 4 && constant_variable == 'c') {
      return new edu::sharif::twinner::trace::exptoken::NamedSymbol
          ("argv", "n_c_argv", true, value, 0);
    } else if (rest.size () > 5 && rest.at (4) == '_' && constant_variable == 'v') {
      std::stringstream ss, ss2;
      ss << rest.substr (5);
      int i;
      ss >> i;
      ss2 << "argv[" << i << "]";
      return new edu::sharif::twinner::trace::exptoken::NamedSymbol
          (ss2.str (), name, false, value, 0);
    }
  }
  edu::sharif::twinner::util::Logger::error ()
      << "NamedSymbol::fromTechnicalName (name=" << name
      << ", value=" << value << ") failed\n";
  throw std::runtime_error ("NamedSymbol::fromTechnicalName (...) failed");
}

NamedSymbol *NamedSymbol::fromTechnicalName (const std::string &name,
    UINT32 v4, UINT32 v3, UINT32 v2, UINT32 v1) {
  UNUSED_VARIABLE (v4);
  UNUSED_VARIABLE (v3);
  const UINT64 value = (UINT64 (v2) << 32) | v1;
  return NamedSymbol::fromTechnicalName
      (name, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (value));
}

}
}
}
}
}
