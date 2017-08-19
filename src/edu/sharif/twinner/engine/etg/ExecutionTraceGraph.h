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

#ifndef EXECUTION_TRACE_GRAPH_H
#define EXECUTION_TRACE_GRAPH_H

#include "Vertex.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class Constraint;
}
namespace engine {
namespace etg {

class InstructionNode;

class ExecutionTraceGraph {
private:
  InstructionNode *root;
  InstructionNode *tail;
  InstructionNode *iterator;
  const edu::sharif::twinner::trace::Constraint * const alwaysTrue;

public:
  ExecutionTraceGraph ();
  ~ExecutionTraceGraph ();

  void addConstraints (const edu::sharif::twinner::trace::Trace *trace);
  bool getNextConstraintsList (
      std::list < const edu::sharif::twinner::trace::Constraint * > &clist);

  void dumpTree () const;

  InstructionNode *getRoot () const;

  Graph *getEtg () const;

private:
  void mergePath (InstructionNode *node);
  bool tryToMergePath (InstructionNode *node, InstructionNode *target) const;

  typedef std::pair<InstructionNode *, InstructionNode *> NodePair;
  bool tryToMergePath (const NodePair lowerBound) const;
  bool checkSnapshotsSatisfiability (const NodePair lowerBound) const;

  void mergePath (InstructionNode *node, InstructionNode *target) const;
};

}
}
}
}
}

#endif /* ExecutionTraceGraph.h */
