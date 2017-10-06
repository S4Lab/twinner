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

#ifndef TRACE_SEGMENT_TERMINATOR_H
#define TRACE_SEGMENT_TERMINATOR_H

#include "Savable.h"


namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Snapshot;

class TraceSegmentTerminator : public Savable {
protected:
  TraceSegmentTerminator ();

public:
  virtual ~TraceSegmentTerminator ();

  virtual TraceSegmentTerminator *clone () const = 0;

  static TraceSegmentTerminator *loadFromBinaryStream (std::ifstream &in);

  virtual std::string getCallingLine () const = 0;
  virtual std::string toString () const = 0;
  virtual void replaceTemporarySymbols (const Snapshot *lastSnapshot) = 0;
};

}
}
}
}

#endif /* TraceSegmentTerminator.h */
