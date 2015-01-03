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

#include "Flags.h"

#include "OperationGroup.h"

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Flags::Flags () :
    leftExp (0), rightExp (0), op (0),
    of (DEFAULT_FSTATE), df (CLEAR_FSTATE), sf (DEFAULT_FSTATE), zf (DEFAULT_FSTATE),
    pf (DEFAULT_FSTATE), cf (DEFAULT_FSTATE) {
}

Flags::~Flags () {
  delete leftExp;
  delete rightExp;
}

bool Flags::getDirectionFlag () const {
  return df == SET_FSTATE;
}

void Flags::setFlags (const OperationGroup *operation,
    const edu::sharif::twinner::trace::Expression *exp1,
    const edu::sharif::twinner::trace::Expression *exp2) {
  delete leftExp;
  leftExp = exp1;
  delete rightExp;
  rightExp = exp2;

  op = operation;
  of = sf = zf = pf = cf = DEFAULT_FSTATE;
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForZeroCase (bool &zero, uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (zf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using ZF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    zero = false;
    break;
  case SET_FSTATE:
    zero = true;
    break;
  case DEFAULT_FSTATE:
    list = op->instantiateConstraintForZeroCase (zero, leftExp, rightExp, instruction);
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for ZF (0x" << std::hex << int (zf) << ")\n";
  }
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (zf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using ZF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    list = instantiateConstraintForLessCase (lessOrEqual, instruction);
    break;
  case SET_FSTATE:
    lessOrEqual = true;
    break;
  case DEFAULT_FSTATE:
    switch (of) {
    case UNDEFINED_FSTATE:
      edu::sharif::twinner::util::Logger::warning ()
          << "Using OF while is in undefined state (assuming that it is CLEAR)\n";
    case CLEAR_FSTATE:
      list = op->operationResultIsLessOrEqualWithZero
          (lessOrEqual, leftExp, rightExp, instruction);
      break;
    case SET_FSTATE:
      list = op->operationResultIsLessThanZero
          (lessOrEqual, leftExp, rightExp, instruction);
      lessOrEqual = !lessOrEqual;
      break;
    case DEFAULT_FSTATE:
      if (sf != DEFAULT_FSTATE) {
        throw std::runtime_error ("Not implemented yet");
      }
      list = op->instantiateConstraintForLessOrEqualCase
          (lessOrEqual, leftExp, rightExp, instruction);
      break;
    default:
      edu::sharif::twinner::util::Logger::error ()
          << "Unknown state for OF (0x" << std::hex << int (of) << ")\n";
    }
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for ZF (0x" << std::hex << int (zf) << ")\n";
  }
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForLessCase (bool &less, uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (of) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using OF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    list = instantiateConstraintForSignCase (less, instruction);
    break;
  case SET_FSTATE:
    list = instantiateConstraintForSignCase (less, instruction);
    less = !less;
    break;
  case DEFAULT_FSTATE:
    if (sf != DEFAULT_FSTATE) {
      throw std::runtime_error ("Not implemented yet");
    }
    list = op->instantiateConstraintForLessCase (less, leftExp, rightExp, instruction);
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for OF (0x" << std::hex << int (of) << ")\n";
  }
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (zf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using ZF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    list = instantiateConstraintForBelowCase (belowOrEqual, instruction);
    break;
  case SET_FSTATE:
    belowOrEqual = true;
    break;
  case DEFAULT_FSTATE:
    switch (cf) {
    case UNDEFINED_FSTATE:
      edu::sharif::twinner::util::Logger::warning ()
          << "Using CF while is in undefined state (assuming that it is CLEAR)\n";
    case CLEAR_FSTATE:
      list = instantiateConstraintForZeroCase (belowOrEqual, instruction);
      break;
    case SET_FSTATE:
      belowOrEqual = true;
      break;
    case DEFAULT_FSTATE:
      list = op->instantiateConstraintForBelowOrEqualCase
          (belowOrEqual, leftExp, rightExp, instruction);
      break;
    default:
      edu::sharif::twinner::util::Logger::error ()
          << "Unknown state for CF (0x" << std::hex << int (cf) << ")\n";
    }
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for ZF (0x" << std::hex << int (zf) << ")\n";
  }
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForBelowCase (bool &below, uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (cf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using CF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    below = false;
    break;
  case SET_FSTATE:
    below = true;
    break;
  case DEFAULT_FSTATE:
    list = op->instantiateConstraintForBelowCase (below, leftExp, rightExp, instruction);
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for CF (0x" << std::hex << int (cf) << ")\n";
  }
  return list;
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForSignCase (bool &sign, uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (sf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using SF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    sign = false;
    break;
  case SET_FSTATE:
    sign = true;
    break;
  case DEFAULT_FSTATE:
    list = op->operationResultIsLessThanZero (sign, leftExp, rightExp, instruction);
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for SF (0x" << std::hex << int (sf) << ")\n";
  }
  return list;
}

}
}
}
}
