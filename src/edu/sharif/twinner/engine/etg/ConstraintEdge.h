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

#ifndef CONSTRAINT_EDGE_H
#define CONSTRAINT_EDGE_H

#include "edu/sharif/twinner/engine/etg/encoder/ConstraintEncoder.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class Logger;
}
namespace trace {

class Constraint;
class ExecutionTraceSegment;
}
namespace engine {
namespace etg {

class InstructionNode;

class ConstraintEdge {
private:
  const int debugId;

  const edu::sharif::twinner::trace::Constraint *constraint;
  const edu::sharif::twinner::trace::ExecutionTraceSegment *segment;

  edu::sharif::twinner::engine::etg::encoder::ConstraintEncoder *encoder;

  InstructionNode *parent;
  InstructionNode *child;

public:
  ConstraintEdge (InstructionNode *p,
      const edu::sharif::twinner::trace::Constraint *c);
  virtual ~ConstraintEdge ();

  void setChild (InstructionNode *node);
  void registerCorrespondingSegment (
      const edu::sharif::twinner::trace::ExecutionTraceSegment *segment);

  const edu::sharif::twinner::trace::Constraint *getConstraint () const;
  const edu::sharif::twinner::trace::ExecutionTraceSegment *getSegment () const;
  InstructionNode *getChild ();
  InstructionNode *getParent ();

  edu::sharif::twinner::engine::etg::encoder::ConstraintEncoder *getEncoder (
      const edu::sharif::twinner::engine::etg::encoder
      ::Encoder::AddrToSizeMap &addressToSize, bool bypassConstraint);

  bool areConstraintEdgesCompatible (const ConstraintEdge *edge) const;

  void dumpConstraints (edu::sharif::twinner::util::Logger &logger) const;
  void dumpSubTree (edu::sharif::twinner::util::Logger &logger,
      unsigned int pad) const;
};

}
}
}
}
}

#endif /* ConstraintEdge.h */
