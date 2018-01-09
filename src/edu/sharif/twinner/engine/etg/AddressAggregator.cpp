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

#include "AddressAggregator.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/trace/exptoken/ExpressionToken.h"
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

AddressAggregator::AddressAggregator (const std::list < ConstTrace * > &_traces,
    std::map < Index, std::set < AddrToSize > > &_addressToSize) :
    traces (_traces), addressToSize (_addressToSize) {
}

void AddressAggregator::gatherSymbolsAddressToSize () {
  edu::sharif::twinner::util::foreach
      (traces, &AddressAggregator::gatherSymbolsAddressToSize, this);
}

void AddressAggregator::gatherSymbolsAddressToSize (ConstTracePtr trace) {
  edu::sharif::twinner::util::foreach
      (trace->getTraceSegments (),
       &AddressAggregator::gatherSymbolsAddressToSize, this);
}

void AddressAggregator::gatherSymbolsAddressToSize (TraceSegmentPtr segment) {
  edu::sharif::twinner::util::foreach
      (segment->getMemoryAddressTo64BitsExpression (),
       &AddressAggregator::gatherSymbolsAddressToSize, this);
  edu::sharif::twinner::util::foreach
      (segment->getRegisterToExpression (),
       &AddressAggregator::gatherSymbolsAddressToSize, this);
  edu::sharif::twinner::util::foreach
      (segment->getPathConstraints (),
       &AddressAggregator::gatherSymbolsAddressToSize, this);
}

void AddressAggregator::gatherSymbolsAddressToSize (ConstraintPtr constraint) {
  gatherSymbolsAddressToSize (constraint->getMainExpression ());
  if (constraint->getAuxExpression ()) {
    gatherSymbolsAddressToSize (constraint->getAuxExpression ());
  }
}

template <typename Key>
void AddressAggregator::gatherSymbolsAddressToSize (const Key &key,
    ExpressionPtr exp) {
  gatherSymbolsAddressToSize (exp);
}

void AddressAggregator::gatherSymbolsAddressToSize (ExpressionPtr exp) {
  edu::sharif::twinner::util::foreach
      (exp->getStack (), &AddressAggregator::gatherSymbolsAddressToSize, this);
}

void AddressAggregator::gatherSymbolsAddressToSize (TokenPtr token) {
  typedef edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol Symbol;
  const Symbol *symbol = dynamic_cast<Symbol *> (token);
  if (symbol) {
    addressToSize[symbol->getGenerationIndex ()].insert
        (make_pair (symbol->getAddress (), symbol->getValue ().getSize ()));
  }
}

}
}
}
}
}
