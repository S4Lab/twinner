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

#include <stdexcept>
#include <list>

#include "OperationGroup.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

OperationGroup::OperationGroup () {
}

OperationGroup::~OperationGroup () {
}

DummyOperationGroup::DummyOperationGroup (const char *_name) :
    NaryOperationGroup (), name (_name) {
}

OperationGroup::ExpressionPtr DummyOperationGroup::getCarryExpression () const {
  edu::sharif::twinner::util::Logger::error ()
      << "DummyOperationGroup::getCarryExpression (...): "
      << name << " case is not implemented yet\n";
  throw std::runtime_error (name + " case is not implemented yet");
}

std::list <OperationGroup::ConstraintPtr>
DummyOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "DummyOperationGroup::instantiateConstraintForZeroCase (...): "
      << name << " case is not implemented yet\n";
  throw std::runtime_error (name + " case is not implemented yet");
}

std::list <OperationGroup::ConstraintPtr>
DummyOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "DummyOperationGroup::instantiateConstraintForLessCase (...): "
      << name << " case is not implemented yet\n";
  throw std::runtime_error (name + " case is not implemented yet");
}

std::list <OperationGroup::ConstraintPtr>
DummyOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "DummyOperationGroup::instantiateConstraintForLessOrEqualCase (...): "
      << name << " case is not implemented yet\n";
  throw std::runtime_error (name + " case is not implemented yet");
}

std::list <OperationGroup::ConstraintPtr>
DummyOperationGroup::instantiateConstraintForBelowCase (bool &below,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "DummyOperationGroup::instantiateConstraintForBelowCase (...): "
      << name << " case is not implemented yet\n";
  throw std::runtime_error (name + " case is not implemented yet");
}

std::list <OperationGroup::ConstraintPtr>
DummyOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "DummyOperationGroup::instantiateConstraintForBelowOrEqualCase (...): "
      << name << " case is not implemented yet\n";
  throw std::runtime_error (name + " case is not implemented yet");
}

std::list <OperationGroup::ConstraintPtr>
DummyOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "DummyOperationGroup::operationResultIsLessOrEqualWithZero (...): "
      << name << " case is not implemented yet\n";
  throw std::runtime_error (name + " case is not implemented yet");
}

std::list <OperationGroup::ConstraintPtr>
DummyOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "DummyOperationGroup::operationResultIsLessThanZero (...): "
      << name << " case is not implemented yet\n";
  throw std::runtime_error (name + " case is not implemented yet");
}

SubtractOperationGroup::SubtractOperationGroup (ConstExpressionPtr mainExp,
    ConstExpressionPtr auxExp) :
    NaryOperationGroup (mainExp, auxExp) {
}

OperationGroup::ExpressionPtr SubtractOperationGroup::getCarryExpression () const {
  const int size = exp[0]->getLastConcreteValue ().getSize ();
  if (size > 64) {
    throw std::runtime_error ("SubtractOperationGroup::getCarryExpression (...):"
                              " Too large bit-length for expression");
  }
  OperationGroup::ExpressionPtr doublePrecision = exp[0]->clone (size * 2);
  doublePrecision->minus (exp[1]);
  doublePrecision->shiftToRight (size);
  OperationGroup::ExpressionPtr truncexp = doublePrecision->clone (size);
  delete doublePrecision;
  truncexp->bitwiseAnd (0x1);
  return truncexp;
}

std::list <OperationGroup::ConstraintPtr>
SubtractOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (zero, exp[0], exp[1], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (less, exp[0], exp[1], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, exp[0], exp[1], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractOperationGroup::instantiateConstraintForBelowCase (bool &below,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateBelowConstraint
                  (below, exp[0], exp[1], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateBelowOrEqualConstraint
                  (belowOrEqual, exp[0], exp[1], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr mainExp = exp[0]->clone ();
  mainExp->minus (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, mainExp, instruction));
  delete mainExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
SubtractOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr mainExp = exp[0]->clone ();
  mainExp->minus (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (lessOrEqual, mainExp, instruction));
  delete mainExp;
  return list;
}

AdditionOperationGroup::AdditionOperationGroup (ConstExpressionPtr mainExp,
    ConstExpressionPtr auxExp) :
    NaryOperationGroup (mainExp, auxExp) {
}

OperationGroup::ExpressionPtr AdditionOperationGroup::getCarryExpression () const {
  const int size = exp[0]->getLastConcreteValue ().getSize ();
  if (size > 64) {
    throw std::runtime_error ("AdditionOperationGroup::getCarryExpression (...):"
                              " Too large bit-length for expression");
  }
  OperationGroup::ExpressionPtr doublePrecision = exp[0]->clone (size * 2);
  doublePrecision->add (exp[1]);
  doublePrecision->shiftToRight (size);
  OperationGroup::ExpressionPtr truncexp = doublePrecision->clone (size);
  delete doublePrecision;
  return truncexp;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr negativeOfRightExp = exp[1]->twosComplement ();
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (zero, exp[0], negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr negativeOfRightExp = exp[1]->twosComplement ();
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (less, exp[0], negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr negativeOfRightExp = exp[1]->twosComplement ();
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, exp[0], negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForBelowCase (bool &below,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list =
      instantiateConstraintForBelowOrEqualCase (below, instruction);
  if (below) {
    bool zero;
    list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                    (zero, exp[0], instruction));
    below = below && !zero;
  }
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr negativeOfRightExp = exp[1]->twosComplement ();
  list.push_back (OperationGroup::Constraint::instantiateBelowConstraint
                  (belowOrEqual, exp[0], negativeOfRightExp, instruction));
  delete negativeOfRightExp;
  belowOrEqual = !belowOrEqual;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr mainExp = exp[0]->clone ();
  mainExp->add (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, mainExp, instruction));
  delete mainExp;
  return list;
}

std::list <OperationGroup::ConstraintPtr>
AdditionOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  OperationGroup::ExpressionPtr mainExp = exp[0]->clone ();
  mainExp->add (exp[1]);
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (lessOrEqual, mainExp, instruction));
  delete mainExp;
  return list;
}

BitwiseAndOperationGroup::BitwiseAndOperationGroup (ConstExpressionPtr mainExp) :
    NaryOperationGroup (mainExp) {
}

OperationGroup::ExpressionPtr BitwiseAndOperationGroup::getCarryExpression () const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears CF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::getCarryExpression (): unreachable code");
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForZeroCase (bool &zero,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateEqualConstraint
                  (zero, exp[0], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForLessCase (bool &less,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears OF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForLessCase ():"
       " unreachable code");
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForLessOrEqualCase (bool &lessOrEqual,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears OF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForLessOrEqualCase ():"
       " unreachable code");
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForBelowCase (bool &below,
    uint32_t instruction) const {
  edu::sharif::twinner::util::Logger::error ()
      << "BitwiseAndOperationGroup always clears CF and so this code is unreachable!\n";
  throw std::runtime_error
      ("BitwiseAndOperationGroup::instantiateConstraintForBelowCase ():"
       " unreachable code");
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::instantiateConstraintForBelowOrEqualCase (bool &belowOrEqual,
    uint32_t instruction) const {
  // CF is clear, so below-or-equal should just check for ZF=1
  return instantiateConstraintForZeroCase (belowOrEqual, instruction);
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::operationResultIsLessOrEqualWithZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateLessOrEqualConstraint
                  (lessOrEqual, exp[0], instruction));
  return list;
}

std::list <OperationGroup::ConstraintPtr>
BitwiseAndOperationGroup::operationResultIsLessThanZero (bool &lessOrEqual,
    uint32_t instruction) const {
  std::list <OperationGroup::ConstraintPtr> list;
  list.push_back (OperationGroup::Constraint::instantiateLessConstraint
                  (lessOrEqual, exp[0], instruction));
  return list;
}

}
}
}
}
