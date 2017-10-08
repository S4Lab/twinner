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

#include "Graph.h"

#include <list>
#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class Snapshot;
class Constraint;
}
namespace engine {
namespace etg {

class InstructionNode;

struct TraceCriticalAddressInfo {
  typedef edu::sharif::twinner::trace::Snapshot * SnapshotPtr;
  typedef std::pair < InstructionNode *, SnapshotPtr > NodeSnapshotPair;
  typedef int ConstraintIndex;
  typedef std::list < ConstraintIndex > ConstraintIndices;

  std::list < NodeSnapshotPair > nodeSnapshotPairs;
  std::map < SnapshotPtr, ConstraintIndices > effectiveConstraints;
};

class ExecutionTraceGraph {
private:
  InstructionNode *root;
  InstructionNode *tail;
  InstructionNode *iterator;
  const edu::sharif::twinner::trace::Constraint * const alwaysTrue;

public:
  ExecutionTraceGraph ();
  ~ExecutionTraceGraph ();

  TraceCriticalAddressInfo addConstraints (
      const edu::sharif::twinner::trace::Trace *trace);
  void mergePath (
      const std::list < TraceCriticalAddressInfo::NodeSnapshotPair > &pairs);

  void markAbortedExecution ();
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
