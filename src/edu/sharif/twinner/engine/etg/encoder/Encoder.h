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

#ifndef ENCODER_H
#define ENCODER_H

#include "edu/sharif/twinner/util/IndentedStringStream.h"

#include <stdint.h>
#include <map>
#include <set>

#ifdef TARGET_IA32E
#define VAR_TYPE "UINT64"
#else
#define VAR_TYPE "UINT32"
#endif

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class ExecutionTraceSegment;
class Constraint;
}
namespace engine {
namespace etg {

class InstructionNode;

namespace encoder {

class VariableContainer;

class Encoder {
protected:
  typedef edu::sharif::twinner::trace::Expression Expression;
  typedef Expression * const &ExpressionPtr;
  typedef const Expression *ConstExpressionPtr;
  typedef edu::sharif::twinner::trace::ExecutionTraceSegment TraceSegment;
  typedef TraceSegment * const &TraceSegmentPtr;
  typedef edu::sharif::twinner::util::IndentedStringStream IndentedStream;
  typedef edu::sharif::twinner::trace::Constraint Constraint;
  typedef Constraint * const &ConstraintPtr;
  typedef const Constraint *ConstConstraintPtr;
  typedef edu::sharif::twinner::engine::etg::InstructionNode InsNode;

  typedef int Size;
  typedef int Index;
  typedef uint64_t Value;

#ifdef TARGET_IA32E
  typedef uint64_t Address;
#else
  typedef uint32_t Address;
#endif

  typedef std::pair < Address, Size > AddrToSize;

public:
  typedef std::map < Index, std::set < AddrToSize > > AddrToSizeMap;
  typedef std::map < AddrToSize, Value > MemoryValueMap;

protected:
  const AddrToSizeMap &addressToSize;

  Encoder (const AddrToSizeMap &addressToSize);

public:
  virtual ~Encoder ();

protected:
  void declareMemorySymbols (IndentedStream &body,
      const std::set < AddrToSize > &addrToSize, int index,
      VariableContainer &vc);
  void declareRegisterSymbols (IndentedStream &body, int index,
      VariableContainer &vc);
};

}
}
}
}
}
}

#endif /* Encoder.h */
