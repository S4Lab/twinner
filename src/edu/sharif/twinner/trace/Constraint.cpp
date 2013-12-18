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
  case NON_POSITIVE:
    t = " <= 0";
    break;
  case NON_NEGATIVE:
    t = " >= 0";
    break;
  case POSITIVE:
    t = " > 0";
    break;
  case NEGATIVE:
    t = " < 0";
    break;
  case ZERO:
    t = " == 0";
    break;
  case NON_ZERO:
    t = " != 0";
    break;
  default:
    return "Unknown comparison type";
  }
  std::string str = exp->toString () + t;
  return str;
}

const Expression *Constraint::getExpression () const {
  return exp;
}

Constraint::ComparisonType Constraint::getComparisonType () const {
  return type;
}

Constraint *Constraint::instantiateNegatedConstraint () const {
  switch (type) {
  case NON_POSITIVE:
    return new Constraint (exp, POSITIVE);
  case NON_NEGATIVE:
    return new Constraint (exp, NEGATIVE);
  case POSITIVE:
    return new Constraint (exp, NON_POSITIVE);
  case NEGATIVE:
    return new Constraint (exp, NON_NEGATIVE);
  case ZERO:
    return new Constraint (exp, NON_ZERO);
  case NON_ZERO:
    return new Constraint (exp, ZERO);
  default:
    throw std::runtime_error ("Unknown ComparisonType");
  }
}

}
}
}
}
