//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef MEMORY_RESIDENT_EXPRESSION_VALUE_PROXY_H
#define MEMORY_RESIDENT_EXPRESSION_VALUE_PROXY_H

#include "MutableExpressionValueProxy.h"

#include "pin.H"

#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class MemoryResidentExpressionValueProxy : public MutableExpressionValueProxy {
private:
  ADDRINT memoryEa;
  int memReadBytes;

public:

  MemoryResidentExpressionValueProxy (ADDRINT memoryEa, int memReadBytes = -1);

  virtual edu::sharif::twinner::trace::Expression *getExpression (
      edu::sharif::twinner::trace::Trace *trace) const;

  virtual edu::sharif::twinner::trace::Expression
  setExpressionWithoutChangeNotification (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const;
  bool isMemoryEaAligned () const;

  virtual void valueIsChanged (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression &changedExp) const;

  virtual int getSize () const;

private:
  void emptyExpressionCache () const;

  void propagateChangeDownwards (int size, ADDRINT memoryEa,
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression &changedExp, bool ownExp) const;
  void propagateChangeUpwards (int size, ADDRINT memoryEa,
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression &changedExp) const;

  void actualPropagateChangeDownwards (int size,
      ADDRINT memoryEa, edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const;

  /// returned expression is linked to the underlying expression (clone it to de-link)
  edu::sharif::twinner::trace::Expression *alignedMemoryRead (int size,
      edu::sharif::twinner::trace::Trace *trace) const;
  edu::sharif::twinner::trace::Expression alignedMemoryWrite (int size,
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const;
  edu::sharif::twinner::trace::Expression unalignedMemoryWrite (int size,
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const;

  /// temporary cache of any used exp during change propagation in valueIsChanged ()
  typedef std::map < std::pair < ADDRINT, int >,
  std::pair < const edu::sharif::twinner::trace::Expression *, bool > >
  AddrSizeToExpMap; // (addr, size) -> (exp*, owned?)
  mutable AddrSizeToExpMap expCache;

  const edu::sharif::twinner::trace::Expression *getNeighborExpression (int size,
      ADDRINT address, edu::sharif::twinner::trace::Trace *trace,
      bool &readFromCache) const;
};

}
}
}
}

#endif /* MemoryResidentExpressionValueProxy.h */
