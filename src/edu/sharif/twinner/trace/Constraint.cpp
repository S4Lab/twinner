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

#include "Constraint.h"

#include "ExecutionState.h"
#include "Expression.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "ConcreteValue.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

Constraint::Constraint (ComparisonType _type, uint32_t instr) :
    mainExp (0), auxExp (0),
    type (_type), instruction (instr) {
}

Constraint::Constraint (const Expression *_exp, ComparisonType _type, uint32_t instr) :
    mainExp (_exp->clone ()), auxExp (0),
    type (_type), instruction (instr) {
}

Constraint::Constraint (const Expression *_mainExp, const Expression *_auxExp,
    ComparisonType _type, uint32_t instr) :
    mainExp (_mainExp->clone ()), auxExp (_auxExp->clone ()),
    type (_type), instruction (instr) {
}

Constraint::~Constraint () {
  delete mainExp;
  delete auxExp;
}

void Constraint::saveToBinaryStream (std::ofstream &out) const {
  out.write ((const char *) &type, sizeof (type));
  out.write ((const char *) &instruction, sizeof (instruction));
  mainExp->saveToBinaryStream (out);
  if (auxExp) {
    auxExp->saveToBinaryStream (out);
  }
}

Constraint *Constraint::loadFromBinaryStream (std::ifstream &in) {
  ComparisonType type;
  in.read ((char *) &type, sizeof (type));
  uint32_t instruction;
  in.read ((char *) &instruction, sizeof (instruction));
  Constraint *cnrt = new Constraint (type, instruction);
  Expression *mainExp = Expression::loadFromBinaryStream (in);
  cnrt->mainExp = mainExp;
  if (type > MAXIMUM_SINGLE_OPERAND_CODED_CONSTRAINT) {
    Expression *auxExp = Expression::loadFromBinaryStream (in);
    cnrt->auxExp = auxExp;
  }
  return cnrt;
}

std::string Constraint::toString () const {
  std::stringstream ss;
  if (type < MAXIMUM_SINGLE_OPERAND_CODED_CONSTRAINT) {
    ss << mainExp->toString ();
  } else if (type < MAXIMUM_UNSIGNED_TWO_OPERANDS_CODED_CONSTRAINT) {
    ss << "/*unsigned {*/" << mainExp->toString () << "/*}*/";
  } else {
    ss << "/*signed {*/" << mainExp->toString () << "/*}*/";
  }
  switch (type) {
  case NON_POSITIVE:
  case BELOW_OR_EQUAL:
  case LESS_OR_EQUAL:
    ss << " <= ";
    break;
  case NON_NEGATIVE:
  case ABOVE_OR_EQUAL:
  case GREATER_OR_EQUAL:
    ss << " >= ";
    break;
  case POSITIVE:
  case ABOVE:
  case GREATER:
    ss << " > ";
    break;
  case NEGATIVE:
  case BELOW:
  case LESS:
    ss << " < ";
    break;
  case ZERO:
  case EQUAL:
    ss << " == ";
    break;
  case NON_ZERO:
  case NON_EQUAL:
    ss << " != ";
    break;
  default:
    return "Unknown comparison type";
  }
  if (type < MAXIMUM_SINGLE_OPERAND_CODED_CONSTRAINT) {
    ss << "0";
  } else if (type < MAXIMUM_UNSIGNED_TWO_OPERANDS_CODED_CONSTRAINT) {
    ss << "unsigned {" << auxExp->toString () << "}";
  } else {
    ss << "signed {" << auxExp->toString () << "}";
  }
  return ss.str ();
}

const Expression *Constraint::getMainExpression () const {
  return mainExp;
}

const Expression *Constraint::getAuxExpression () const {
  return auxExp;
}

Constraint::ComparisonType Constraint::getComparisonType () const {
  return type;
}

uint32_t Constraint::getCausingInstructionIdentifier () const {
  return instruction;
}

Constraint *Constraint::instantiateNegatedConstraint () const {
  switch (type) {
  case NON_POSITIVE:
    return new Constraint (mainExp, POSITIVE, instruction);
  case BELOW_OR_EQUAL:
    return new Constraint (mainExp, auxExp, ABOVE, instruction);
  case LESS_OR_EQUAL:
    return new Constraint (mainExp, auxExp, GREATER, instruction);
  case NON_NEGATIVE:
    return new Constraint (mainExp, NEGATIVE, instruction);
  case ABOVE_OR_EQUAL:
    return new Constraint (mainExp, auxExp, BELOW, instruction);
  case GREATER_OR_EQUAL:
    return new Constraint (mainExp, auxExp, LESS, instruction);
  case POSITIVE:
    return new Constraint (mainExp, NON_POSITIVE, instruction);
  case ABOVE:
    return new Constraint (mainExp, auxExp, BELOW_OR_EQUAL, instruction);
  case GREATER:
    return new Constraint (mainExp, auxExp, LESS_OR_EQUAL, instruction);
  case NEGATIVE:
    return new Constraint (mainExp, NON_NEGATIVE, instruction);
  case BELOW:
    return new Constraint (mainExp, auxExp, ABOVE_OR_EQUAL, instruction);
  case LESS:
    return new Constraint (mainExp, auxExp, GREATER_OR_EQUAL, instruction);
  case ZERO:
    return new Constraint (mainExp, NON_ZERO, instruction);
  case EQUAL:
    return new Constraint (mainExp, auxExp, NON_EQUAL, instruction);
  case NON_ZERO:
    return new Constraint (mainExp, ZERO, instruction);
  case NON_EQUAL:
    return new Constraint (mainExp, auxExp, EQUAL, instruction);
  default:
    throw std::runtime_error ("Unknown ComparisonType");
  }
}

bool Constraint::operator== (const Constraint &constraint) const {
  return (*mainExp) == (*constraint.mainExp) && type == constraint.type
      && ((auxExp == 0 && constraint.auxExp == 0)
      || (auxExp != 0 && constraint.auxExp != 0 && (*auxExp) == (*constraint.auxExp)));
}

bool Constraint::isTrivial () const {
  return mainExp->isTrivial () && (auxExp == 0 || auxExp->isTrivial ());
}

Constraint *Constraint::instantiateBelowConstraint (bool &below,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  if (auxExp) {
    below = mainExp->getLastConcreteValue () < auxExp->getLastConcreteValue ();
    return new Constraint (mainExp, auxExp, below ? BELOW : ABOVE_OR_EQUAL, instruction);
  } else {
    below = false; // unsigned value can not be below zero
    return 0;
  }
}

Constraint *Constraint::instantiateBelowOrEqualConstraint (bool &belowOrEqual,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  if (auxExp) {
    belowOrEqual = mainExp->getLastConcreteValue () <= auxExp->getLastConcreteValue ();
    return new Constraint
        (mainExp, auxExp, belowOrEqual ? BELOW_OR_EQUAL : ABOVE, instruction);
  } else {
    return Constraint::instantiateEqualConstraint
        (belowOrEqual, mainExp, auxExp, instruction);
  }
}

Constraint *Constraint::instantiateLessConstraint (bool &less,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  if (auxExp) {
    less = mainExp->getLastConcreteValue ().lessThan (auxExp->getLastConcreteValue ());
    return new Constraint (mainExp, auxExp, less ? LESS : GREATER_OR_EQUAL, instruction);
  } else {
    less = mainExp->getLastConcreteValue ().lessThan (0);
    return new Constraint (mainExp, less ? NEGATIVE : NON_NEGATIVE, instruction);
  }
}

Constraint *Constraint::instantiateLessOrEqualConstraint (bool &lessOrEqual,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  if (auxExp) {
    lessOrEqual = mainExp->getLastConcreteValue ().lessThanOrEqualTo
        (auxExp->getLastConcreteValue ());
    return new Constraint
        (mainExp, auxExp, lessOrEqual ? LESS_OR_EQUAL : GREATER, instruction);
  } else {
    lessOrEqual = mainExp->getLastConcreteValue ().lessThanOrEqualTo (0);
    return new Constraint (mainExp, lessOrEqual ? NON_POSITIVE : POSITIVE, instruction);
  }
}

Constraint *Constraint::instantiateEqualConstraint (bool &equal,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  if (auxExp) {
    equal = mainExp->getLastConcreteValue () == auxExp->getLastConcreteValue ();
    return new Constraint (mainExp, auxExp, equal ? EQUAL : NON_EQUAL, instruction);
  } else {
    equal = mainExp->getLastConcreteValue ().isZero ();
    return new Constraint (mainExp, equal ? ZERO : NON_ZERO, instruction);
  }
}

Constraint *Constraint::instantiateSignConstraint (bool &sign,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  if (auxExp) {
    Expression *exp = mainExp->clone ();
    exp->minus (auxExp);
    Constraint *c = Constraint::instantiateLessConstraint (sign, exp, 0, instruction);
    delete exp;
    return c;
  } else {
    return Constraint::instantiateLessConstraint (sign, mainExp, 0, instruction);
  }
}

}
}
}
}
