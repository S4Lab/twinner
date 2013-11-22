//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
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

namespace edu {
namespace sharif {
namespace twinner {
namespace twintool {

class MemoryResidentExpressionValueProxy : public MutableExpressionValueProxy {

public:
  ADDRINT memoryEa;
  int memReadBytes;

  MemoryResidentExpressionValueProxy (ADDRINT memoryEa, int memReadBytes = -1);

  virtual edu::sharif::twinner::trace::Expression *getExpression (
      edu::sharif::twinner::trace::Trace *trace) const;
  virtual edu::sharif::twinner::trace::Expression *
  setExpressionWithoutChangeNotification (
      edu::sharif::twinner::trace::Trace *trace,
      const edu::sharif::twinner::trace::Expression *exp) const;

  virtual void valueIsChanged (
      edu::sharif::twinner::trace::Trace *trace,
      edu::sharif::twinner::trace::Expression *changedExp) const;

  virtual void truncate (edu::sharif::twinner::trace::Expression *exp) const;
  virtual int getSize () const;
};

}
}
}
}

#endif /* MemoryResidentExpressionValueProxy.h */
