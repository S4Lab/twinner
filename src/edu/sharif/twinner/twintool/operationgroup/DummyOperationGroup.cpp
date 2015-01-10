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

#include "DummyOperationGroup.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {
namespace operationgroup {

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

}
}
}
}
}