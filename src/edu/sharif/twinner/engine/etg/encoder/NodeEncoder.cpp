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

#include "NodeEncoder.h"

#include <list>
#include <set>

#include "edu/sharif/twinner/engine/smt/Cvc4SmtSolver.h"

#include "edu/sharif/twinner/engine/etg/ConstraintEdge.h"
#include "edu/sharif/twinner/engine/etg/InstructionNode.h"

#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/util/iterationtools.h"

#include "ConstraintEncoder.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {

NodeEncoder::NodeEncoder (const AddrToSizeMap &addressToSize) :
    Encoder (addressToSize),
    secondPass (false),
    aggregatingSetOfVariables (false) {
}

NodeEncoder::~NodeEncoder () {
}

void NodeEncoder::initializeFirstPass (
    edu::sharif::twinner::engine::etg::InstructionNode *node) {
  const std::list < edu::sharif::twinner::engine::etg::ConstraintEdge * > &
      children = node->getChildren ();
  switch (children.size ()) {
  case 0:
    leftChild = rightChild = 0;
    break;
  case 1:
    leftChild = children.front ()->getEncoder (addressToSize, false);
    rightChild = 0;
    break;
  case 2:
  {
    ConstConstraintPtr c1not =
        children.front ()->getConstraint ()->instantiateNegatedConstraint ();
    edu::sharif::twinner::engine::smt::SmtSolver *solver =
        edu::sharif::twinner::engine::smt::SmtSolver::getInstance ();
    solver->clearState ();
    solver->assertConstraint (c1not);
    const bool valid =
        solver->checkValidity (children.back ()->getConstraint ());
    solver->clearState ();
    delete c1not;
    if (!valid) {
      edu::sharif::twinner::util::Logger::error ()
          << "Encoder::Encoder (...): Assertion failed.\n";
      abort ();
    }
    leftChild = children.front ()->getEncoder (addressToSize, false);
    rightChild = children.back ()->getEncoder (addressToSize, true);
    break;
  }
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "Encoder::Encoder (...):"
        " There are more than 2 children. Assertion failed.\n";
    abort ();
  }
}

void NodeEncoder::initializeSecondPass () {
  if (secondPass) {
    return;
  }
  secondPass = true;
  if (leftChild) {
    leftChild->initializeSecondPass ();
    if (rightChild) {
      rightChild->initializeSecondPass ();
    }
  }
  aggregatedVariables = getAggregatedVariables ();
  finalizeInitialization ();
}

void NodeEncoder::finalizeInitialization () {
  // no further initialization is required in the NodeEncoder
}

std::set< Variable > NodeEncoder::getAggregatedVariables () {
  std::set< Variable > vars;
  if (aggregatingSetOfVariables) {
    return vars;
  }
  if (aggregatedVariables.size () > 0) {
    return aggregatedVariables;
  }
  aggregatingSetOfVariables = true;
  if (leftChild) {
    vars = leftChild->getAggregatedVariables ();
    if (rightChild) {
      const std::set< Variable > right = rightChild->getAggregatedVariables ();
      vars.insert (right.begin (), right.end ());
    }
  }
  aggregatingSetOfVariables = false;
  return vars;
}

void NodeEncoder::encodeMain (IndentedStream &body, IndentedStream &preamble,
    const MemoryValueMap &initialValues) {
  body << "int main (int argc, char *argv[]) {\n";
  body.incrementDepth ();
  body << "struct RegistersSet regs;\n";
  body << "SAVE_REGISTERS (regs);\n";

  declareRegisterSymbols (body, 0);
  initializeMemory (body, initialValues);
  if (hasAnyChild ()) {
    AddrToSizeMap::const_iterator it = addressToSize.find (0);
    if (it != addressToSize.end ()) {
      declareMemorySymbols (body, it->second, 0);
    }
    body.indented ();
    encode (body, preamble, 0, true); // depth=1, index=0, inMain=true
  }
  body.decrementDepth ();
  body << "}\n";
}

bool NodeEncoder::hasAnyChild () const {
  return leftChild != 0;
}

void NodeEncoder::initializeMemory (IndentedStream &body,
    const MemoryValueMap &initialValues) {
  edu::sharif::twinner::util::foreach
      (initialValues, &NodeEncoder::initializeMemory, this, body);
}

void NodeEncoder::initializeMemory (IndentedStream &body,
    const AddrToSize &address, const Value &value) {
  body << "\t*((UINT" << std::dec << address.second << " *) 0x"
      << std::hex << address.first << ") = 0x" << value << ";\n";
}

void NodeEncoder::encode (IndentedStream &body, IndentedStream &preamble,
    int index, bool inMain) {
  const bool hasEncodedConstraint =
      leftChild->encode (body, preamble, index, inMain);
  if (rightChild && hasEncodedConstraint) {
    body << " else ";
    rightChild->encode (body, preamble, index, inMain);
  }
  body << '\n';
}


}
}
}
}
}
}
