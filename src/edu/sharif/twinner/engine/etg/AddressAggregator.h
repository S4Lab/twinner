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

#ifndef ADDRESS_AGGREGATOR_H
#define ADDRESS_AGGREGATOR_H

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
}
}
namespace engine {
namespace etg {

class AddressAggregator {
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

private:
  const std::list < ConstTrace * > &traces;
  std::map < Index, std::set < AddrToSize > > &addressToSize;

public:
  AddressAggregator (const std::list < ConstTrace * > &traces,
      std::map < Index, std::set < AddrToSize > > &addressToSize);

  void gatherSymbolsAddressToSize ();

private:
  void gatherSymbolsAddressToSize (ConstTracePtr trace);
  void gatherSymbolsAddressToSize (TraceSegmentPtr segment);
  void gatherSymbolsAddressToSize (ConstraintPtr constraint);
  template <typename Key>
  void gatherSymbolsAddressToSize (const Key &key, ExpressionPtr exp);
  void gatherSymbolsAddressToSize (ExpressionPtr exp);
  void gatherSymbolsAddressToSize (TokenPtr token);
};

}
}
}
}
}

#endif /* AddressAggregator.h */
