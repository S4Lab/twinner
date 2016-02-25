//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "edu/sharif/twinner/pin-wrapper.h"

#include "Flags.h"

#include "edu/sharif/twinner/twintool/operationgroup/OperationGroup.h"

#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Flags::Flags () :
    op (0),
    of (DEFAULT_FSTATE), df (CLEAR_FSTATE), sf (DEFAULT_FSTATE), zf (DEFAULT_FSTATE),
    pf (DEFAULT_FSTATE), cf (DEFAULT_FSTATE),
    cfexp (0) {
}

Flags::~Flags () {
  delete op;
  delete cfexp;
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
  case MANUAL_FSTATE:
    return cfexp->clone ();
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Flags::getCarryFlag () [cf=" << std::hex << int (cf) << "]: "
        "Unknown CF state\n";
    abort ();
  }
}

void Flags::setFlags (
    const edu::sharif::twinner::twintool::operationgroup::OperationGroup *operation) {
  delete op;
  op = operation;
  of = sf = zf = pf = cf = DEFAULT_FSTATE;
  if (cfexp) {
    delete cfexp;
    cfexp = 0;
  }
}

void Flags::setOverflowFlag (bool set) {
  of = set ? SET_FSTATE : CLEAR_FSTATE;
}

void Flags::setDirectionFlag (bool set) {
  df = set ? SET_FSTATE : CLEAR_FSTATE;
}

void Flags::setCarryFlag (bool set) {
  cf = set ? SET_FSTATE : CLEAR_FSTATE;
  if (cfexp) {
    delete cfexp;
    cfexp = 0;
  }
}

void Flags::setCarryFlag (const edu::sharif::twinner::trace::Expression *exp) {
  cf = MANUAL_FSTATE;
  if (cfexp) {
    delete cfexp;
  }
  cfexp = exp;
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForOverflowCase (bool &overflow,
    uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (of) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using OF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    overflow = false;
    break;
  case SET_FSTATE:
    overflow = true;
    break;
  case DEFAULT_FSTATE:
    list = op->instantiateConstraintForOverflowCase (overflow, instruction);
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for OF (0x" << std::hex << int (of) << ")\n";
  }
  return list;
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
        edu::sharif::twinner::util::Logger::error () << "Not implemented yet\n";
        abort ();
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
      edu::sharif::twinner::util::Logger::error () << "Not implemented yet\n";
      abort ();
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
    case MANUAL_FSTATE:
      list = instantiateConstraintForZeroCase (belowOrEqual, instruction);
      if (!belowOrEqual) {
        bool cfiszero;
        list.push_back (edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
                        (cfiszero, cfexp, instruction));
        belowOrEqual = !cfiszero;
      }
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
  case MANUAL_FSTATE:
    bool cfiszero;
    list.push_back (edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
                    (cfiszero, cfexp, instruction));
    below = !cfiszero;
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
