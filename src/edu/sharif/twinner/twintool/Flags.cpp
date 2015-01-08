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

#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Flags::Flags () :
    op (0),
    of (DEFAULT_FSTATE), df (CLEAR_FSTATE), sf (DEFAULT_FSTATE), zf (DEFAULT_FSTATE),
    pf (DEFAULT_FSTATE), cf (DEFAULT_FSTATE) {
}

Flags::~Flags () {
  delete op;
}

bool Flags::getDirectionFlag () const {
  return df == SET_FSTATE;
}

edu::sharif::twinner::trace::Expression *Flags::getCarryFlag () const {
  switch (cf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using CF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    return new edu::sharif::twinner::trace::ExpressionImp ();
  case SET_FSTATE:
    return new edu::sharif::twinner::trace::ExpressionImp (1);
  case DEFAULT_FSTATE:
    return op->getCarryExpression ();
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for CF (0x" << std::hex << int (cf) << ")\n";
    throw std::runtime_error ("Unknown CF state");
  }
}

void Flags::setFlags (const OperationGroup *operation) {
  delete op;
  op = operation;
  of = sf = zf = pf = cf = DEFAULT_FSTATE;
}

void Flags::setOverflowFlag (bool set) {
  of = set ? SET_FSTATE : CLEAR_FSTATE;
}

void Flags::setCarryFlag (bool set) {
  cf = set ? SET_FSTATE : CLEAR_FSTATE;
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
    list = op->instantiateConstraintForZeroCase (zero, instruction);
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
      list = op->operationResultIsLessOrEqualWithZero (lessOrEqual, instruction);
      break;
    case SET_FSTATE:
      list = op->operationResultIsLessThanZero (lessOrEqual, instruction);
      lessOrEqual = !lessOrEqual;
      break;
    case DEFAULT_FSTATE:
      if (sf != DEFAULT_FSTATE) {
        throw std::runtime_error ("Not implemented yet");
      }
      list = op->instantiateConstraintForLessOrEqualCase (lessOrEqual, instruction);
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
    list = op->instantiateConstraintForLessCase (less, instruction);
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
      list = op->instantiateConstraintForBelowOrEqualCase (belowOrEqual, instruction);
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
    list = op->instantiateConstraintForBelowCase (below, instruction);
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
    list = op->operationResultIsLessThanZero (sign, instruction);
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
