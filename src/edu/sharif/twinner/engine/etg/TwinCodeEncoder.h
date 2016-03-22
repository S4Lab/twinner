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

#ifndef TWIN_CODE_ENCODER_H
#define TWIN_CODE_ENCODER_H

#include "edu/sharif/twinner/util/IndentedStringStream.h"

#include <stdint.h>

#include <map>
#include <list>
#include <set>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class ExecutionTraceSegment;
class Constraint;
class Expression;
namespace exptoken {

class ExpressionToken;
class MemoryEmergedSymbol;
}
}
namespace engine {
namespace etg {

class TreeNode;
class Variable;

class TwinCodeEncoder {
public:
  typedef int Size;
#ifdef TARGET_IA32E
  typedef uint64_t Address;
#else
  typedef uint32_t Address;
#endif
  typedef std::pair < Address, Size > AddrToSize;
  typedef int Index;
  typedef edu::sharif::twinner::trace::Trace Trace;
  typedef const Trace ConstTrace;
  typedef ConstTrace * const &ConstTracePtr;
  typedef edu::sharif::twinner::trace::ExecutionTraceSegment TraceSegment;
  typedef TraceSegment * const &TraceSegmentPtr;
  typedef edu::sharif::twinner::trace::Constraint Constraint;
  typedef Constraint * const &ConstraintPtr;
  typedef const Constraint *ConstConstraintPtr;
  typedef edu::sharif::twinner::trace::Expression Expression;
  typedef Expression * const &ExpressionPtr;
  typedef const Expression *ConstExpressionPtr;
  typedef edu::sharif::twinner::trace::exptoken::ExpressionToken Token;
  typedef Token * const &TokenPtr;
  typedef uint64_t Value;
  typedef edu::sharif::twinner::util::IndentedStringStream IndentedStream;
  typedef const TreeNode ConstTreeNode;

private:
  IndentedStream out;
  IndentedStream conout;

  int conditionIndex;

  std::map < Index, std::set < AddrToSize > > addressToSize;
  const std::map < AddrToSize, Value > &initialValues;
  ConstTreeNode *root;

public:
  TwinCodeEncoder (const std::list < ConstTrace * > &traces,
      const std::map < AddrToSize, Value > &initialValues, ConstTreeNode *root);

  void encodeToFile (std::ofstream &fileout);

private:
  void declareRegisterSymbols (int index);
  void initializeMemory ();
  void initializeMemory (const AddrToSize &address, const Value &value);

  void declareMemorySymbols (const std::set < AddrToSize > &addrToSize,
      int depth, int index);

  void encodeConstraintAndChildren (ConstTreeNode *node, int depth, int index,
      bool bypassConstraint = false);
  void encodeConstraint (std::list < ConstConstraintPtr > constraints,
      int depth);
  void encodeTransformations (const TraceSegment *segment,
      int depth, int index);
  void encodeChildren (ConstTreeNode *node, int depth, int index);

  void extractTypesAndNames (std::set< Variable > &typesAndNames,
      ConstConstraintPtr simplifiedConstraint);
  void extractTypesAndNames (std::set< Variable > &typesAndNames,
      ConstExpressionPtr exp);

  void codeMemoryChanges (const Address &memoryEa, ExpressionPtr exp);
  void codeRegisterChanges (const TraceSegment *segment, int index);
};

}
}
}
}
}

#endif /* TwinCodeEncoder.h */
