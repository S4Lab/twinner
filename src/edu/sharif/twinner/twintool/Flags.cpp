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

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/ConcreteValue.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

Flags::Flags () :
    leftExp (0), rightExp (0), op (UNDEFINED_OPGROUP),
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

void Flags::setFlags (OperationGroup operation,
    edu::sharif::twinner::trace::Expression *exp1,
    edu::sharif::twinner::trace::Expression *exp2) {
  delete leftExp;
  leftExp = exp1;
  delete rightExp;
  rightExp = exp2;

  op = operation;
  of = sf = zf = pf = cf = DEFAULT_FSTATE;
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForZeroCase (
    bool &zero, uint32_t instruction) const {
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
    if (rightExp) {
      if (op == SUB_OPGROUP) {
        return edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
            (zero, leftExp, rightExp, instruction);
      } else if (op == ADD_OPGROUP) {
        edu::sharif::twinner::trace::Expression *negativeOfRightExp =
            rightExp->twosComplement ();
        edu::sharif::twinner::trace::Constraint *res =
            edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
            (zero, leftExp, negativeOfRightExp, instruction);
        delete negativeOfRightExp;
        return res;
      } else {
        edu::sharif::twinner::util::Logger::error ()
            << "Unexpected operation group (0x" << std::hex << int (op) << ")\n";
      }
    } else {
      if (op == AND_OPGROUP) {
        return edu::sharif::twinner::trace::Constraint::instantiateEqualConstraint
            (zero, leftExp, instruction);
      } else {
        edu::sharif::twinner::util::Logger::error ()
            << "Unexpected operation group (0x" << std::hex << int (op) << ")\n";
      }
    }
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for ZF (0x" << std::hex << int (zf) << ")\n";
  }
  return 0;
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForLessOrEqualCase (
    bool &lessOrEqual, uint32_t instruction) const {
  switch (zf) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using ZF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    return instantiateConstraintForLessCase (lessOrEqual, instruction);
  case SET_FSTATE:
    lessOrEqual = true;
    break;
  case DEFAULT_FSTATE:
    switch (of) {
    case UNDEFINED_FSTATE:
      edu::sharif::twinner::util::Logger::warning ()
          << "Using OF while is in undefined state (assuming that it is CLEAR)\n";
    case CLEAR_FSTATE:
      if (op == SUB_OPGROUP) {
        edu::sharif::twinner::trace::Expression *exp = leftExp->clone ();
        exp->minus (rightExp);
        edu::sharif::twinner::trace::Constraint *res =
            edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
            (lessOrEqual, exp, instruction);
        delete exp;
        return res;
      } else if (op == ADD_OPGROUP) {
        edu::sharif::twinner::trace::Expression *exp = leftExp->clone ();
        exp->add (rightExp);
        edu::sharif::twinner::trace::Constraint *res =
            edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
            (lessOrEqual, exp, instruction);
        delete exp;
        return res;
      } else if (op == AND_OPGROUP) {
        edu::sharif::twinner::trace::Constraint *res =
            edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
            (lessOrEqual, leftExp, instruction);
        return res;
      } else {
        edu::sharif::twinner::util::Logger::error ()
            << "Unexpected operation group (0x" << std::hex << int (op) << ")\n";
      }
    case SET_FSTATE:
      if (op == SUB_OPGROUP) {
        edu::sharif::twinner::trace::Expression *exp = leftExp->clone ();
        exp->minus (rightExp);
        edu::sharif::twinner::trace::Constraint *res =
            edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
            (lessOrEqual, exp, instruction);
        lessOrEqual = !lessOrEqual;
        delete exp;
        return res;
      } else if (op == ADD_OPGROUP) {
        edu::sharif::twinner::trace::Expression *exp = leftExp->clone ();
        exp->add (rightExp);
        edu::sharif::twinner::trace::Constraint *res =
            edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
            (lessOrEqual, exp, instruction);
        lessOrEqual = !lessOrEqual;
        delete exp;
        return res;
      } else if (op == AND_OPGROUP) {
        edu::sharif::twinner::trace::Constraint *res =
            edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
            (lessOrEqual, leftExp, instruction);
        lessOrEqual = !lessOrEqual;
        return res;
      } else {
        edu::sharif::twinner::util::Logger::error ()
            << "Unexpected operation group (0x" << std::hex << int (op) << ")\n";
      }
    case DEFAULT_FSTATE:
      if (sf != DEFAULT_FSTATE) {
        throw "Not implemented yet";
      }
      if (rightExp) {
        if (op == SUB_OPGROUP) {
          return edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
              (lessOrEqual, leftExp, rightExp, instruction);
        } else if (op == ADD_OPGROUP) {
          edu::sharif::twinner::trace::Expression *negativeOfRightExp =
              rightExp->twosComplement ();
          edu::sharif::twinner::trace::Constraint *res =
              edu::sharif::twinner::trace::Constraint::instantiateLessOrEqualConstraint
              (lessOrEqual, leftExp, negativeOfRightExp, instruction);
          delete negativeOfRightExp;
          return res;
        } else {
          edu::sharif::twinner::util::Logger::error ()
              << "Unexpected operation group (0x" << std::hex << int (op) << ")\n";
        }
      } else {
        // AND_OPGROUP clears OF
        edu::sharif::twinner::util::Logger::error ()
            << "Unexpected operation group (0x" << std::hex << int (op) << ")\n";
      }
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
  return 0;
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForLessCase (
    bool &less, uint32_t instruction) const {
  switch (of) {
  case UNDEFINED_FSTATE:
    edu::sharif::twinner::util::Logger::warning ()
        << "Using OF while is in undefined state (assuming that it is CLEAR)\n";
  case CLEAR_FSTATE:
    return instantiateConstraintForSignCase (less, instruction);
  case SET_FSTATE:
    edu::sharif::twinner::trace::Constraint *res =
        instantiateConstraintForSignCase (less, instruction);
    less = !less;
    return res;
  case DEFAULT_FSTATE:
    if (sf != DEFAULT_FSTATE) {
      throw "Not implemented yet";
    }
    if (rightExp) {
      if (op == SUB_OPGROUP) {
        return edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
            (less, leftExp, rightExp, instruction);
      } else if (op == ADD_OPGROUP) {
        edu::sharif::twinner::trace::Expression *negativeOfRightExp =
            rightExp->twosComplement ();
        edu::sharif::twinner::trace::Constraint *res =
            edu::sharif::twinner::trace::Constraint::instantiateLessConstraint
            (less, leftExp, negativeOfRightExp, instruction);
        delete negativeOfRightExp;
        return res;
      } else {
        edu::sharif::twinner::util::Logger::error ()
            << "Unexpected operation group (0x" << std::hex << int (op) << ")\n";
      }
    } else {
      // TEST_OPGROUP clears OF
      edu::sharif::twinner::util::Logger::error ()
          << "Unexpected operation group (0x" << std::hex << int (op) << ")\n";
    }
    break;
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Unknown state for OF (0x" << std::hex << int (of) << ")\n";
  }
  return 0;
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForBelowOrEqualCase (
    bool &belowOrEqual, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateBelowOrEqualConstraint
      (belowOrEqual, leftExp, rightExp, instruction);
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForBelowCase (
    bool &below, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateBelowConstraint
      (below, leftExp, rightExp, instruction);
}

edu::sharif::twinner::trace::Constraint *Flags::instantiateConstraintForSignCase (
    bool &sign, uint32_t instruction) const {
  return edu::sharif::twinner::trace::Constraint::instantiateSignConstraint
      (sign, leftExp, rightExp, instruction);
}

}
}
}
}
