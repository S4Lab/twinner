//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "ConstraintEncoder.h"

#include <list>
#include <set>
#include <map>

#include "edu/sharif/twinner/engine/etg/ConstraintEdge.h"
#include "edu/sharif/twinner/engine/etg/InstructionNode.h"

#include "edu/sharif/twinner/engine/smt/Cvc4SmtSolver.h"

#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/TraceSegmentTerminator.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/FunctionInvocation.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/trace/exptoken/Symbol.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {
namespace {

int lastConditionIndex = 0;
}

ConstraintEncoder::ConstraintEncoder (
    edu::sharif::twinner::engine::etg::ConstraintEdge *edge,
    const AddrToSizeMap &addressToSize, bool bypassConstraint) :
    Encoder (addressToSize),
    conditionIndex (++lastConditionIndex),
    segment (edge->getSegment ()) {
  std::list < ConstConstraintPtr > constraints;
  InsNode *node = edge->getChild ();
  while (edge->getSegment () == 0
      && node->getChildren ().size () == 1
      && node->getParents ().size () == 1) {
    constraints.push_back (edge->getConstraint ());
    edge = node->getChildren ().front ();
    node = edge->getChild ();
  }
  constraints.push_back (edge->getConstraint ());
  if (bypassConstraint) {
    constraints.pop_front ();
  }

  std::stringstream ss;
  mustConstraintBeEncoded = simplifyConstraints (constraints, ss);
  constraintString = ss.str ();

  if (segment) {
    gatherOldVariablesOfSegment ();
    gatherNewVariablesOfSegment ();
  }
  child = node->getEncoder (addressToSize);
}

ConstraintEncoder::~ConstraintEncoder () {
}

void ConstraintEncoder::initializeSecondPass () {
  child->initializeSecondPass ();
}

std::set< Variable > ConstraintEncoder::getAggregatedVariables () const {
  std::set< Variable > vars = child->getAggregatedVariables ();
  for (std::set< Variable >::const_iterator it =
      newVariablesCreatedInSegment.begin ();
      it != newVariablesCreatedInSegment.end (); ++it) {
    vars.erase (*it);
  }
  vars.insert (typesAndNames.begin (), typesAndNames.end ());
  vars.insert (oldVariablesUsedBySegment.begin (),
               oldVariablesUsedBySegment.end ());
  return vars;
}

bool ConstraintEncoder::encode (IndentedStream &body, IndentedStream &preamble,
    int index, bool inMain, VariableContainer vc) {
  const bool constraintIsEncoded = encodeConstraint (body, preamble, inMain);
  body.incrementDepth ();
  if (segment) {
    Output out = {
      body,
      inMain
    };
    encodeTransformations (out, preamble, segment, index++, vc);
  }
  if (child->hasAnyChild ()) {
    body.indented ();
    child->encode (body, preamble, index, inMain, vc);
  }
  body.decrementDepth ();
  body.indented () << '}';
  return constraintIsEncoded;
}

void ConstraintEncoder::gatherOldVariablesOfSegment () {
  edu::sharif::twinner::util::foreach
      (segment->getMemoryAddressTo64BitsExpression (),
       &ConstraintEncoder::gatherOldVariablesOfSegment, this);
  edu::sharif::twinner::util::foreach
      (segment->getRegisterToExpression (),
       &ConstraintEncoder::gatherOldVariablesOfSegment, this);
  if (dynamic_cast<const edu::sharif::twinner::trace
      ::FunctionInvocation *> (segment->getTerminator ())) {
    const std::list<ConstExpressionPtr> argumentExpressions =
        static_cast<const edu::sharif::twinner::trace
        ::FunctionInvocation *> (segment->getTerminator ())
        ->getArgumentExpressions ();
    edu::sharif::twinner::util::foreach
        (argumentExpressions,
         &ConstraintEncoder::gatherOldVariablesOfSegment, this);
  }
}

void ConstraintEncoder::gatherNewVariablesOfSegment () {
  if (!segment->getTerminator ()) { // this must be the last segment
    return;
  }
  const int index = segment->getSegmentIndex () + 1;
  gatherNewRegisterVariablesOfSegment (index);
  segment->printCompleteState (edu::sharif::twinner::util::Logger::debug ());
  AddrToSizeMap::const_iterator it = addressToSize.find (index);
  if (it != addressToSize.end ()) {
    gatherNewMemoryVariablesOfSegment (it->second, index);
  }
}

void ConstraintEncoder::gatherNewRegisterVariablesOfSegment (int index) {
  const char *registersNames[] = {
#ifdef TARGET_IA32E
    "rax", "rbx", "rcx", "rdx", "rdi", "rsi", "rsp", "rbp",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
#else
    "eax", "ebx", "ecx", "edx", "edi", "esi", "esp", "ebp"
#endif
  };
  for (int i = 0;
#ifdef TARGET_IA32E
      i < 16;
#else
      i < 8;
#endif
      ++i) {
    {
      std::stringstream ss;
      ss << registersNames[i] << "_" << index;
      newVariablesCreatedInSegment.insert
          (Variable (VAR_TYPE, ss.str (), ss.str (), index));
    }
    {
      std::stringstream ss;
      ss << "xmm" << i << "_" << index;
      newVariablesCreatedInSegment.insert
          (Variable ("UINT128", ss.str (), ss.str (), index));
    }
  }
}

void ConstraintEncoder::gatherNewMemoryVariablesOfSegment (
    const std::set < AddrToSize > &addrToSize, int index) {
  for (std::set < AddrToSize >::const_iterator it = addrToSize.begin ();
      it != addrToSize.end (); ++it) {
    std::stringstream type;
    type << "UINT" << std::dec << it->second;
    std::stringstream name;
    name << "m" << std::hex << it->first << "_" << index
        << "_" << std::dec << it->second;
    newVariablesCreatedInSegment.insert
        (Variable (type.str (), name.str (), name.str (), index));
  }
}

void ConstraintEncoder::encodeTransformations (Output &out,
    IndentedStream &preamble, const TraceSegment *segment, int index,
    VariableContainer &vc) {
  IndentedStream &body = out.body;
  body.indented () << "/*Memory Changes*/\n";
  edu::sharif::twinner::util::foreach
      (segment->getMemoryAddressTo64BitsExpression (),
       &ConstraintEncoder::codeMemoryChanges, this, out);
  codeRegisterChanges (out, segment);
  if (!segment->getTerminator ()) { // this must be the last segment
#ifdef TARGET_IA32E
    body.indented () << "return int (regs.rax);\n";
#else
    body.indented () << "return int (regs.eax);\n";
#endif
    return;
  }
  body.indented ()
      << segment->getTerminator ()->getCallingLine (out.inMain) << '\n';
  declareRegisterSymbols (body, index + 1, vc);
  AddrToSizeMap::const_iterator it = addressToSize.find (index + 1);
  if (it != addressToSize.end ()) {
    declareMemorySymbols (body, it->second, index + 1, vc);
  }
}

void ConstraintEncoder::codeRegisterChanges (Output &out,
    const TraceSegment *segment) {
  IndentedStream &body = out.body;

  struct RegInfo {
    const char *name;
    REG id;
  } const regs[] = {
#ifdef TARGET_IA32E
    {"rax", REG_RAX},
    {"rbx", REG_RBX},
    {"rcx", REG_RCX},
    {"rdx", REG_RDX},
    {"rdi", REG_RDI},
    {"rsi", REG_RSI},
    {"rsp", REG_RSP},
    {"rbp", REG_RBP},
    {"r8", REG_R8},
    {"r9", REG_R9},
    {"r10", REG_R10},
    {"r11", REG_R11},
    {"r12", REG_R12},
    {"r13", REG_R13},
    {"r14", REG_R14},
    {"r15", REG_R15}
#else
    {"eax", REG_EAX},
    {"ebx", REG_EBX},
    {"ecx", REG_ECX},
    {"edx", REG_EDX},
    {"edi", REG_EDI},
    {"esi", REG_ESI},
    {"esp", REG_ESP},
    {"ebp", REG_EBP}
#endif
  };
  body.indented () << "/*Registers Changes*/\n";
  const std::map < REG, Expression * > &regToExp =
      segment->getRegisterToExpression ();
  for (int i = 0;
#ifdef TARGET_IA32E
      i < 16;
#else
      i < 8;
#endif
      ++i) {
    std::map < REG, Expression * >::const_iterator it =
        regToExp.find (regs[i].id);
    if (it != regToExp.end ()) {
      std::string expStr = it->second->toString (out.inMain);
      body.indented () << "regs." << regs[i].name << " = "
          << VAR_TYPE " (" << expStr << ");\n";
    }
  }
}

void ConstraintEncoder::codeMemoryChanges (Output &out,
    const Address &memoryEa, ExpressionPtr exp) {
  std::string expStr = exp->toString (out.inMain);
  out.body.indented () << "*((" VAR_TYPE " *) 0x" << std::hex << memoryEa
      << ") = " VAR_TYPE " (" << expStr << ");\n";
}

bool ConstraintEncoder::encodeConstraint (IndentedStream &body,
    IndentedStream &preamble, bool inMain) {
  if (!mustConstraintBeEncoded) {
    body << "{\n";
    return false;
  }
  if (constraintString.length () < 200) { // inline the constraint
    std::string inlinedConstraintString = constraintString;
    if (inMain) {
      for (std::set< Variable >::const_iterator it = typesAndNames.begin ();
          it != typesAndNames.end (); ++it) {
        std::stringstream castedName;
        castedName << VAR_TYPE << " (" << it->name << ")";
        inlinedConstraintString = replaceAll
            (inlinedConstraintString, it->technicalName, castedName.str ());
      }
    }
    body << "if (" << inlinedConstraintString << ") {\n";
    return true;
  }
  std::stringstream conditionName;
  conditionName << "cond_" << conditionIndex;
  std::stringstream typedArguments, arguments;
  bool first = true;
  for (std::set< Variable >::const_iterator it = typesAndNames.begin ();
      it != typesAndNames.end (); ++it) {
    if (first) {
      first = false;
    } else {
      typedArguments << ", ";
      arguments << ", ";
    }
    typedArguments << it->type << ' ' << it->technicalName;
    if (inMain) {
      arguments << VAR_TYPE " (" << it->name << ")";
    } else {
      arguments << it->technicalName;
    }
  }
  preamble << "bool " << conditionName.str () << " ("
      << typedArguments.str () << ") {\n";
  preamble << "\treturn " << constraintString << ";\n";
  preamble << "}\n\n";
  body << "if (" << conditionName.str () << " ("
      << arguments.str () << ")) {\n";
  return true;
}

std::string ConstraintEncoder::replaceAll (std::string str,
    std::string part, std::string substitute) const {
  for (std::string::size_type pos = str.find (part); pos != std::string::npos;
      pos = str.find (part, pos + substitute.length ())) {
    str.replace (pos, part.length (), substitute);
  }
  return str;
}

bool ConstraintEncoder::simplifyConstraints (
    std::list < ConstConstraintPtr > constraints,
    std::stringstream &ss) {
  bool ok;
  std::list < ConstConstraintPtr > simplifiedConstraints =
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
      ->simplifyConstraints (ok, constraints);
  bool isAnyConstraintEncoded;
  if (!ok) {
    edu::sharif::twinner::util::Logger::warning ()
        << "ConstraintEncoder::simplifyConstraints (...):"
        " Skipping constraints simplification due to errors\n";
    isAnyConstraintEncoded = encodeConstraints (constraints, ss);
  } else {
    isAnyConstraintEncoded = encodeConstraints (simplifiedConstraints, ss);
  }
  while (!simplifiedConstraints.empty ()) {
    delete simplifiedConstraints.back ();
    simplifiedConstraints.pop_back ();
  }
  return isAnyConstraintEncoded;
}

bool ConstraintEncoder::encodeConstraints (
    std::list < ConstConstraintPtr > simplifiedConstraints,
    std::stringstream &ss) {
  bool first = true;
  for (std::list < ConstConstraintPtr >
      ::const_reverse_iterator it = simplifiedConstraints.rbegin ();
      it != simplifiedConstraints.rend (); ++it) {
    ConstConstraintPtr simplifiedConstraint = *it;
    if (simplifiedConstraint->isTrivial (false)) {
      continue;
    }
    if (first) {
      ss << '(';
      first = false;
    } else {
      ss << "\n\t\t&& (";
    }
    ss << simplifiedConstraint->toString () << ')';
    extractTypesAndNames (typesAndNames, simplifiedConstraint);
  }
  return !first;
}

void ConstraintEncoder::extractTypesAndNames (std::set< Variable > &typesAndNames,
    ConstConstraintPtr simplifiedConstraint) const {
  extractTypesAndNames
      (typesAndNames, simplifiedConstraint->getMainExpression ());
  if (simplifiedConstraint->getAuxExpression ()) {
    extractTypesAndNames
        (typesAndNames, simplifiedConstraint->getAuxExpression ());
  }
}

void extract_type_and_name (std::set< Variable > &typesAndNames,
    edu::sharif::twinner::trace::exptoken::ExpressionToken * const &token) {
  const edu::sharif::twinner::trace::exptoken::Symbol *symbol =
      dynamic_cast<edu::sharif::twinner::trace::exptoken::Symbol *> (token);
  if (symbol) {
    std::stringstream ss;
    ss << "UINT" << std::dec << symbol->getValue ().getSize ();
    typesAndNames.insert (Variable (ss.str (),
                                    symbol->getTechnicalName (),
                                    symbol->toString (),
                                    symbol->getGenerationIndex ()));
  }
}

void ConstraintEncoder::extractTypesAndNames (std::set< Variable > &typesAndNames,
    ConstExpressionPtr exp) const {
  edu::sharif::twinner::util::foreach
      (exp->getStack (), &extract_type_and_name, typesAndNames);
}


}
}
}
}
}
}
