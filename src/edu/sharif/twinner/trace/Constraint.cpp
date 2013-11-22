//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "Constraint.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "ExecutionState.h"

#include <stdexcept>
#include <fstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Constraint::Constraint (ComparisonType _type) :
type (_type) {
}

Constraint::Constraint (const Expression *_exp, ComparisonType _type) :
exp (_exp->clone ()), type (_type) {
}

Constraint::~Constraint () {
  delete exp;
}

void Constraint::saveToBinaryStream (std::ofstream &out) const {
  exp->saveToBinaryStream (out);
  out.write ((const char *) &type, sizeof (type));
}

Constraint *Constraint::loadFromBinaryStream (std::ifstream &in) {
  Expression *exp = Expression::loadFromBinaryStream (in);
  ComparisonType type;
  in.read ((char *) &type, sizeof (type));
  Constraint *cnrt = new Constraint (type);
  cnrt->exp = exp;
  return cnrt;
}

std::string Constraint::toString () const {
  const char *t;
  switch (type) {
  case POSITIVE:
    t = " > 0";
  case NEGATIVE:
    t = " < 0";
  case ZERO:
    t = " = 0";
  case NON_ZERO:
    t = " != 0";
  default:
    return "Unknown comparison type";
  }
  std::string str = exp->toString () + t;
  return str;
}

}
}
}
}
