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

#include "edu/sharif/twinner/pin-wrapper.h"

#include "Flags.h"

#include "edu/sharif/twinner/operationgroup/OperationGroup.h"

#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace operationgroup {

Flags::Flags () :
    op (0),
    df (CLEAR_FSTATE),
    cfexp (0) {
  of = sf = zf = pf = cf = UNDEFINED_FSTATE;
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
    const edu::sharif::twinner::operationgroup::OperationGroup *operation) {
  delete op;
  op = operation;
  of = sf = zf = pf = cf = DEFAULT_FSTATE;
  if (cfexp) {
    delete cfexp;
    cfexp = 0;
  }
}

void append (std::list <edu::sharif::twinner::trace::Constraint *> &list,
    std::list <edu::sharif::twinner::trace::Constraint *> tempList) {
  list.splice (list.end (), tempList);
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::getFlagsExpression (uint32_t &flags, uint32_t instruction) const {
  // TODO: Set undefined bits based on their concrete state
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  bool overflow = (flags >> 11) & 0x1;
  append (list, instantiateConstraintForOverflowCase (overflow, instruction));
  bool direction = getDirectionFlag ();
  bool sign = (flags >> 7) & 0x1;
  append (list, instantiateConstraintForSignCase (sign, instruction));
  bool zero = (flags >> 6) & 0x1;
  append (list, instantiateConstraintForZeroCase (zero, instruction));
  bool parity = (flags >> 2) & 0x1;
  append (list, instantiateConstraintForParityCase (parity, instruction));
  bool carry = flags & 0x1;
  append (list, instantiateConstraintForCarryCase (carry, instruction));
  flags = ((overflow ? 1u : 0) << 11) | ((direction ? 1u : 0) << 10)
      | (1u << 9) | ((sign ? 1u : 0) << 7) | ((zero ? 1u : 0) << 6)
      | ((parity ? 1u : 0) << 2) | (1u << 1) | (carry ? 1u : 0);
  return list;
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
        << "Using OF while is in undefined state (assuming that it is ";
    if (overflow) {
      edu::sharif::twinner::util::Logger::warning () << "SET)\n";
    } else {
      edu::sharif::twinner::util::Logger::warning () << "CLEAR)\n";
    }
    break;
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
        << "Using ZF while is in undefined state (assuming that it is ";
    if (zero) {
      edu::sharif::twinner::util::Logger::warning () << "SET)\n";
    } else {
      edu::sharif::twinner::util::Logger::warning () << "CLEAR)\n";
    }
    break;
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
Flags::instantiateConstraintForParityCase (bool &parity, uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (pf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using PF while is in undefined state (assuming that it is ";
    if (parity) {
      edu::sharif::twinner::util::Logger::warning () << "SET)\n";
    } else {
      edu::sharif::twinner::util::Logger::warning () << "CLEAR)\n";
    }
    break;
  case CLEAR_FSTATE:
    parity = false;
    break;
  case SET_FSTATE:
    parity = true;
    break;
  case DEFAULT_FSTATE:
  {
    edu::sharif::twinner::trace::Expression *exp = op->getOperationResult ();
    exp = calculateParity (exp);
    list.push_back
        (edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
         (parity, exp, instruction));
    delete exp;
    break;
  }
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for PF (0x" << std::hex << int (pf) << ")\n";
  }
  return list;
}

edu::sharif::twinner::trace::Expression *Flags::calculateParity (
    edu::sharif::twinner::trace::Expression *exp) const {
  exp->truncate (8);
  edu::sharif::twinner::trace::Expression *xorOfBits = exp->clone (8);
  xorOfBits->bitwiseAnd (0x1);
  for (int i = 1; i < 8; ++i) {
    edu::sharif::twinner::trace::Expression *bit = exp->clone (8);
    bit->shiftToRight (i);
    bit->bitwiseAnd (0x1);
    xorOfBits->bitwiseXor (bit);
    delete bit;
  }
  delete exp;
  return xorOfBits;
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForCarryCase (bool &carry, uint32_t instruction) const {
  return instantiateConstraintForBelowCase (carry, instruction);
}

std::list <edu::sharif::twinner::trace::Constraint *>
Flags::instantiateConstraintForBelowCase (bool &below, uint32_t instruction) const {
  std::list <edu::sharif::twinner::trace::Constraint *> list;
  switch (cf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using CF while is in undefined state (assuming that it is ";
    if (below) {
      edu::sharif::twinner::util::Logger::warning () << "SET)\n";
    } else {
      edu::sharif::twinner::util::Logger::warning () << "CLEAR)\n";
    }
    break;
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
        << "Using SF while is in undefined state (assuming that it is ";
    if (sign) {
      edu::sharif::twinner::util::Logger::warning () << "SET)\n";
    } else {
      edu::sharif::twinner::util::Logger::warning () << "CLEAR)\n";
    }
    break;
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
