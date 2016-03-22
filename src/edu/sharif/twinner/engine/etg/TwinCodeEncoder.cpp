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

#include "TwinCodeEncoder.h"

#include "edu/sharif/twinner/pin-wrapper.h"

#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/trace/exptoken/Symbol.h"

#include "edu/sharif/twinner/engine/smt/Cvc4SmtSolver.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

#include "TreeNode.h"
#include "AddressAggregator.h"

#include <fstream>
#include <stdexcept>

#ifdef TARGET_IA32E
#define VAR_TYPE "UINT64"
#else
#define VAR_TYPE "UINT32"
#endif

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

struct Variable {
  std::string type;
  std::string technicalName;
  std::string name;

  Variable (std::string _type, std::string _technicalName, std::string _name)
      : type (_type), technicalName (_technicalName), name (_name) {
  }

  bool operator< (const Variable &v) const {
    return technicalName < v.technicalName;
  }
};

TwinCodeEncoder::TwinCodeEncoder (const std::list < ConstTrace * > &traces,
    const std::map < AddrToSize, Value > &_initialValues,
    ConstTreeNode *_root) :
    conditionIndex (0),
    initialValues (_initialValues), root (_root) {
  AddressAggregator (traces, addressToSize).gatherSymbolsAddressToSize ();
}

void TwinCodeEncoder::encodeToFile (std::ofstream &code) {
  code << '\n';
  code << "#include \"twincode.h\"\n";
  code << '\n';
  out << "int main (int argc, char *argv[]) {\n";
  out << "\tstruct RegistersSet regs;\n";
  out << "\tSAVE_REGISTERS (regs);\n";
  out.setDepth (1);
  declareRegisterSymbols (0);
  initializeMemory ();
  if (root->hasAnyChild ()) {
    declareMemorySymbols (addressToSize[0], 1, 0);
    out.indented ();
    encodeChildren (root, 1, 0);
  }
  out << "}\n";
  code << conout.str () << out.str ();
}

void TwinCodeEncoder::declareRegisterSymbols (int index) {
  const char *registersNames[] = {
#ifdef TARGET_IA32E
    "rax", "rbx", "rcx", "rdx", "rdi", "rsi", "rsp", "rbp",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
#else
    "eax", "ebx", "ecx", "edx", "edi", "esi", "esp", "ebp"
#endif
  };
  out.indented () << std::dec << "const " VAR_TYPE " "
      << registersNames[0] << "_" << index << " = regs." << registersNames[0];
  for (int i = 1;
#ifdef TARGET_IA32E
      i < 16;
#else
      i < 8;
#endif
      ++i) {
    out << ", " << registersNames[i] << "_" << index
        << " = regs." << registersNames[i];
  }
  out << ";\n";
  out.indented () << std::dec << "const UINT128 xmm0_" << index
      << " = UINT128 (regs.xmm0)";
  for (int i = 1;
#ifdef TARGET_IA32E
      i < 16;
#else
      i < 8;
#endif
      ++i) {
    out << ", xmm" << i << "_" << index << " = UINT128 (regs.xmm" << i << ")";
  }
  out << ";\n";
}

void TwinCodeEncoder::initializeMemory () {
  edu::sharif::twinner::util::foreach
      (initialValues, &TwinCodeEncoder::initializeMemory, this);
}

void TwinCodeEncoder::initializeMemory (const AddrToSize &address,
    const Value &value) {
  out << "\t*((UINT" << std::dec << address.second << " *) 0x"
      << std::hex << address.first << ") = 0x" << value << ";\n";
}

void TwinCodeEncoder::declareMemorySymbols (
    const std::set < AddrToSize > &addrToSize, int depth, int index) {
  out.setDepth (depth);
  for (std::set < AddrToSize >::const_iterator it = addrToSize.begin ();
      it != addrToSize.end (); ++it) {
    out.indented () << "const UINT" << std::dec << it->second
        << " m" << std::hex << it->first << "_" << index
        << "_" << std::dec << it->second
        << " = *((UINT" << it->second << " *) 0x"
        << std::hex << it->first << ");\n";
  }
}

void TwinCodeEncoder::encodeConstraintAndChildren (ConstTreeNode *node,
    int depth, int index) {
  encodeConstraint (node->getConstraint (), depth);
  const TraceSegment *segment = node->getSegment ();
  if (segment) {
    encodeTransformations (segment, depth + 1, index++);
  }
  if (node->hasAnyChild ()) {
    out.setDepth (depth + 1);
    out.indented ();
    encodeChildren (node, depth + 1, index);
  }
  out.setDepth (depth);
  out.indented () << '}';
}

void TwinCodeEncoder::encodeConstraint (ConstConstraintPtr constraint,
    int depth) {
  bool ok;
  std::list < ConstConstraintPtr > simplifiedConstraints =
      edu::sharif::twinner::engine::smt::SmtSolver::getInstance ()
      ->simplifyConstraint (ok, constraint);
  if (!ok) {
    edu::sharif::twinner::util::Logger::error ()
        << "TwinCodeEncoder::encodeConstraint (...):"
        " Error in constraints simplification\n";
    abort ();
  }
  std::stringstream ss;
  std::set< Variable > typesAndNames;
  bool first = true;
  for (std::list < ConstConstraintPtr >
      ::const_reverse_iterator it = simplifiedConstraints.rbegin ();
      it != simplifiedConstraints.rend (); ++it) {
    ConstConstraintPtr simplifiedConstraint = *it;
    if (simplifiedConstraint->isTrivial ()) {
      delete simplifiedConstraint;
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
    delete simplifiedConstraint;
  }
  if (!first) {
    const std::string constraintString = ss.str ();
    std::stringstream conditionName;
    conditionName << "cond_" << (++conditionIndex);

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
      arguments << VAR_TYPE " (" << it->name << ")";
    }

    conout << "bool " << conditionName.str () << " ("
        << typedArguments.str () << ") {\n";
    conout << "\treturn " << constraintString << ";\n";
    edu::sharif::twinner::util::Logger::loquacious () << constraintString << '\n';
    conout << "}\n\n";
    out << "if (" << conditionName.str () << " ("
        << arguments.str () << ")) {\n";
  } else {
    out << "if (true) {\n";
  }
}

void TwinCodeEncoder::extractTypesAndNames (std::set< Variable > &typesAndNames,
    ConstConstraintPtr simplifiedConstraint) {
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
                                    symbol->toString ()));
  }
}

void TwinCodeEncoder::extractTypesAndNames (std::set< Variable > &typesAndNames,
    ConstExpressionPtr exp) {
  edu::sharif::twinner::util::foreach
      (exp->getStack (), &extract_type_and_name, typesAndNames);
}

void TwinCodeEncoder::encodeTransformations (const TraceSegment *segment,
    int depth, int index) {
  out.setDepth (depth);
  out.indented () << "/*Memory Changes*/\n";
  const std::map < ADDRINT, Expression * > &m = segment->getMemoryAddressTo64BitsExpression ();
  edu::sharif::twinner::util::foreach
      (segment->getMemoryAddressTo64BitsExpression (),
       &TwinCodeEncoder::codeMemoryChanges, this);
  codeRegisterChanges (segment, index);
  declareRegisterSymbols (index + 1);
  declareMemorySymbols (addressToSize[index + 1], depth, index + 1);
}

void TwinCodeEncoder::codeMemoryChanges (const Address &memoryEa,
    ExpressionPtr exp) {
  out.indented () << "*((" VAR_TYPE " *) 0x" << std::hex << memoryEa
      << ") = " VAR_TYPE " (" << exp->toString () << ");\n";
}

void TwinCodeEncoder::codeRegisterChanges (const TraceSegment *segment,
    int index) {

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
  out.indented () << "/*Registers Changes*/\n";
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
    out.indented () << "regs." << regs[i].name << " = ";
    if (it != regToExp.end ()) {
      out << VAR_TYPE " (" << it->second->toString () << ");\n";
    } else {
      out << regs[i].name << "_" << index << ";\n";
    }
  }
  out.indented () << "regs = setRegistersValuesAndInvokeSyscall (regs);\n";
}

void TwinCodeEncoder::encodeChildren (ConstTreeNode *node,
    int depth, int index) {
  const std::list < TreeNode * > &children = node->getChildren ();
  bool first = true;
  for (std::list < TreeNode * >::const_iterator it = children.begin ();
      it != children.end (); ++it) {
    ConstTreeNode *child = *it;
    if (first) {
      first = false;
    } else {
      out << " else ";
    }
    encodeConstraintAndChildren (child, depth, index);
  }
  out << '\n';
}

}
}
}
}
}
