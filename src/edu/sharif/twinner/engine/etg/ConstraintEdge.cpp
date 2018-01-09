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

#include "ConstraintEdge.h"

#include "InstructionNode.h"

#include "edu/sharif/twinner/trace/Constraint.h"
#include "edu/sharif/twinner/trace/TraceSegmentTerminator.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"

#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

static int lastDebugId = 0;

ConstraintEdge::ConstraintEdge (InstructionNode *p,
    const edu::sharif::twinner::trace::Constraint *c) :
    debugId (++lastDebugId),
    constraint (c),
    segment (0),
    encoder (0),
    parent (p),
    child (0) {
}

ConstraintEdge::~ConstraintEdge () {
  delete child;
  delete encoder;
}

void ConstraintEdge::setChild (InstructionNode *node) {
  child = node;
}

void ConstraintEdge::registerCorrespondingSegment (
    const edu::sharif::twinner::trace::ExecutionTraceSegment *_segment) {
  segment = _segment;
}

const edu::sharif::twinner::trace::Constraint *
ConstraintEdge::getConstraint () const {
  return constraint;
}

const edu::sharif::twinner::trace::ExecutionTraceSegment *
ConstraintEdge::getSegment () const {
  return segment;
}

InstructionNode *ConstraintEdge::getChild () {
  return child;
}

InstructionNode *ConstraintEdge::getParent () {
  return parent;
}

edu::sharif::twinner::engine::etg::encoder::ConstraintEncoder *
ConstraintEdge::getEncoder (const edu::sharif::twinner::engine::etg::encoder
    ::Encoder::AddrToSizeMap &addressToSize, bool bypassConstraint) {
  if (encoder) {
    return encoder;
  }
  encoder = new edu::sharif::twinner::engine::etg::encoder
      ::ConstraintEncoder (this, addressToSize, bypassConstraint);
  return encoder;
}

bool ConstraintEdge::areConstraintEdgesCompatible (const ConstraintEdge *edge) const {
  if ((segment == 0) ^ (edge->segment == 0)) {
    return false;
  }
  if (segment && edge->segment) {
    const edu::sharif::twinner::trace::TraceSegmentTerminator *me =
        segment->getTerminator ();
    const edu::sharif::twinner::trace::TraceSegmentTerminator *that =
        edge->segment->getTerminator ();
    if ((me == 0) ^ (that == 0)) {
      return false;
    }
    if (me && that) {
      if ((*me) != (*that)) {
        return false;
      }
    }
  }
  return (*constraint) == (*edge->constraint);
}

void ConstraintEdge::dumpConstraints (
    edu::sharif::twinner::util::Logger &logger) const {
  if (constraint) {
    logger << debugId << " -> " << constraint << '\n';
  }
  child->dumpConstraints (logger);
}

void ConstraintEdge::dumpSubTree (edu::sharif::twinner::util::Logger &logger,
    unsigned int pad) const {

  repeat (pad) {
    logger << "  ";
  }
  logger << "Edge(" << debugId << ") -> " << constraint << "\n";
  child->dumpSubTree (logger, pad + 1);
}

}
}
}
}
}
