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

#include "Constraint.h"

#include "ExecutionState.h"
#include "ExpressionImp.h"

#include "edu/sharif/twinner/util/Logger.h"

#include "edu/sharif/twinner/trace/Expression.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

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

Constraint::Constraint (const Expression *_exp,
    ComparisonType _type, uint32_t instr, bool needsSignExtension) :
    mainExp (needsSignExtension ? _exp->signExtended (128) : _exp->clone ()), auxExp (0),
    type (_type), instruction (instr) {
}

Constraint::Constraint (const Expression *_mainExp, const Expression *_auxExp,
    ComparisonType _type, uint32_t instr, bool needsSignExtension) :
    mainExp (needsSignExtension ? _mainExp->signExtended (128) : _mainExp->clone ()),
    auxExp (needsSignExtension ? _auxExp->signExtended (128) : _auxExp->clone ()),
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

Constraint *Constraint::clone () const {
  if (auxExp) {
    return new Constraint (mainExp, auxExp, type, instruction, false);
  } else {
    return new Constraint (mainExp, type, instruction, false);
  }
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
    ss << "/*unsigned {*/" << auxExp->toString () << "/*}*/";
  } else {
    ss << "/*signed {*/" << auxExp->toString () << "/*}*/";
  }
  return ss.str ();
}

const Expression *Constraint::getMainExpression () const {
  return mainExp;
}

Expression *Constraint::getMainExpression () {
  return mainExp;
}

const Expression *Constraint::getAuxExpression () const {
  return auxExp;
}

Expression *Constraint::getAuxExpression () {
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
    return new Constraint (mainExp, POSITIVE, instruction, true);
  case BELOW_OR_EQUAL:
    return new Constraint (mainExp, auxExp, ABOVE, instruction, false);
  case LESS_OR_EQUAL:
    return new Constraint (mainExp, auxExp, GREATER, instruction, true);
  case NON_NEGATIVE:
    return new Constraint (mainExp, NEGATIVE, instruction, true);
  case ABOVE_OR_EQUAL:
    return new Constraint (mainExp, auxExp, BELOW, instruction, false);
  case GREATER_OR_EQUAL:
    return new Constraint (mainExp, auxExp, LESS, instruction, true);
  case POSITIVE:
    return new Constraint (mainExp, NON_POSITIVE, instruction, true);
  case ABOVE:
    return new Constraint (mainExp, auxExp, BELOW_OR_EQUAL, instruction, false);
  case GREATER:
    return new Constraint (mainExp, auxExp, LESS_OR_EQUAL, instruction, true);
  case NEGATIVE:
    return new Constraint (mainExp, NON_NEGATIVE, instruction, true);
  case BELOW:
    return new Constraint (mainExp, auxExp, ABOVE_OR_EQUAL, instruction, false);
  case LESS:
    return new Constraint (mainExp, auxExp, GREATER_OR_EQUAL, instruction, true);
  case ZERO:
    return new Constraint (mainExp, NON_ZERO, instruction, false);
  case EQUAL:
    return new Constraint (mainExp, auxExp, NON_EQUAL, instruction, false);
  case NON_ZERO:
    return new Constraint (mainExp, ZERO, instruction, false);
  case NON_EQUAL:
    return new Constraint (mainExp, auxExp, EQUAL, instruction, false);
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown ComparisonType: " << std::dec << type << '\n';
    abort ();
  }
}

bool Constraint::operator== (const Constraint &constraint) const {
  return (*mainExp) == (*constraint.mainExp) && type == constraint.type
      && ((auxExp == 0 && constraint.auxExp == 0)
          || (auxExp != 0 && constraint.auxExp != 0 && (*auxExp) == (*constraint.auxExp)));
}

bool Constraint::isTrivial (bool requiresValidConcreteValue) const {
  return mainExp->isTrivial (requiresValidConcreteValue)
      && (auxExp == 0 || auxExp->isTrivial (requiresValidConcreteValue));
}

Constraint *Constraint::instantiateBelowConstraint (bool &below,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  below = mainExp->getLastConcreteValue () < auxExp->getLastConcreteValue ();
  return new Constraint (mainExp, auxExp, below ? BELOW : ABOVE_OR_EQUAL,
                         instruction, false);
}

Constraint *Constraint::instantiateBelowOrEqualConstraint (bool &belowOrEqual,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  belowOrEqual = mainExp->getLastConcreteValue () <= auxExp->getLastConcreteValue ();
  return new Constraint (mainExp, auxExp, belowOrEqual ? BELOW_OR_EQUAL : ABOVE,
                         instruction, false);
}

Constraint *Constraint::instantiateBelowOrEqualConstraint (bool &belowOrEqual,
    const Expression *mainExp, uint32_t instruction) {
  // check for mainExp == 0 instead
  return Constraint::instantiateEqualConstraint (belowOrEqual, mainExp, instruction);
}

Constraint *Constraint::instantiateLessConstraint (bool &less,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  less = mainExp->getLastConcreteValue ().lessThan (auxExp->getLastConcreteValue ());
  return new Constraint (mainExp, auxExp, less ? LESS : GREATER_OR_EQUAL,
                         instruction, true);
}

Constraint *Constraint::instantiateLessConstraint (bool &less,
    const Expression *mainExp, uint32_t instruction) {
  less = mainExp->getLastConcreteValue ().lessThan (0);
  return new Constraint (mainExp, less ? NEGATIVE : NON_NEGATIVE,
                         instruction, true);
}

Constraint *Constraint::instantiateLessOrEqualConstraint (bool &lessOrEqual,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  lessOrEqual = mainExp->getLastConcreteValue ().lessThanOrEqualTo
      (auxExp->getLastConcreteValue ());
  return new Constraint (mainExp, auxExp, lessOrEqual ? LESS_OR_EQUAL : GREATER,
                         instruction, true);
}

Constraint *Constraint::instantiateLessOrEqualConstraint (bool &lessOrEqual,
    const Expression *mainExp, uint32_t instruction) {
  lessOrEqual = mainExp->getLastConcreteValue ().lessThanOrEqualTo (0);
  return new Constraint (mainExp, lessOrEqual ? NON_POSITIVE : POSITIVE,
                         instruction, true);
}

Constraint *Constraint::instantiateEqualConstraint (bool &equal,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  equal = mainExp->getLastConcreteValue () == auxExp->getLastConcreteValue ();
  return new Constraint (mainExp, auxExp, equal ? EQUAL : NON_EQUAL,
                         instruction, false);
}

Constraint *Constraint::instantiateEqualConstraint (bool &equal,
    const Expression *mainExp, uint32_t instruction) {
  equal = mainExp->getLastConcreteValue ().isZero ();
  return new Constraint (mainExp, equal ? ZERO : NON_ZERO, instruction, false);
}

Constraint *Constraint::instantiateSignConstraint (bool &sign,
    const Expression *mainExp, const Expression *auxExp, uint32_t instruction) {
  Expression *exp = mainExp->clone ();
  exp->minus (auxExp);
  Constraint *c = Constraint::instantiateLessConstraint (sign, exp, instruction);
  delete exp;
  return c;
}

Constraint *Constraint::instantiateSignConstraint (bool &sign,
    const Expression *mainExp, uint32_t instruction) {
  return Constraint::instantiateLessConstraint (sign, mainExp, instruction);
}

Constraint *Constraint::instantiateTautology (bool valid) {
  const Expression *zero = new ExpressionImp ();
  Constraint *res = new Constraint (zero, valid ? ZERO : NON_ZERO, 0, false);
  delete zero;
  return res;
}

}
}
}
}
