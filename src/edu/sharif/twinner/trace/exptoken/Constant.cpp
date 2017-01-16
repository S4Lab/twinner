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

#include <sstream>

#include "Constant.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

Constant::Constant (const Constant &c) :
    Operand (c) {
}

Constant::Constant (UINT64 val) :
    Operand (edu::sharif::twinner::trace::cv::ConcreteValue64Bits (val)) {
}

Constant::Constant (const edu::sharif::twinner::trace::cv::ConcreteValue &val) :
    Operand (val) {
}

Constant::Constant (edu::sharif::twinner::trace::cv::ConcreteValue *val) :
    Operand (val) {
}

void Constant::setValue (const edu::sharif::twinner::trace::cv::ConcreteValue &value) {
  (*concreteValue) = value;
}

void Constant::setValue (edu::sharif::twinner::trace::cv::ConcreteValue *value) {
  delete concreteValue;
  concreteValue = value;
}

Constant *Constant::clone () const {
  return new Constant (*this);
}

void Constant::saveToBinaryStream (std::ofstream &out) const {
  out.write ("C", 1);
  concreteValue->saveToBinaryStream (out);
}

Constant *Constant::loadFromBinaryStream (std::ifstream &in) {
  return new Constant
      (edu::sharif::twinner::trace::cv::ConcreteValue::loadFromBinaryStream (in));
}

std::string Constant::toString () const {
  std::stringstream ss;
  ss << *concreteValue;
  return ss.str ();
}

bool Constant::operator== (const ExpressionToken &token) const {
  return dynamic_cast<const Constant *> (&token)
      && (*static_cast<const Constant *> (&token)->concreteValue) == (*concreteValue);
}

bool Constant::isConstant () const {
  return true;
}

}
}
}
}
}
