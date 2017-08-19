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

#ifndef INSTRUCTION_NODE_H
#define INSTRUCTION_NODE_H

#include <list>
#include <string>
#include <stdint.h>

#include "edu/sharif/twinner/engine/etg/encoder/Encoder.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class Logger;
class MemoryManager;
}
namespace trace {

class Constraint;
class Snapshot;
class ExecutionTraceSegment;
}
namespace engine {
namespace etg {
namespace encoder {

class NodeEncoder;
}

class ConstraintEdge;

class InstructionNode {
private:
  const int debugId;

  // front of list: left-most | back of list: right-most
  std::list < ConstraintEdge * > parents;
  std::list < ConstraintEdge * > children;

  uint32_t insId;
  const edu::sharif::twinner::util::MemoryManager *memoryManager;

  edu::sharif::twinner::engine::etg::encoder::NodeEncoder *encoder;

  edu::sharif::twinner::trace::Snapshot *snapshot;

public:
  InstructionNode ();
  InstructionNode (ConstraintEdge *parent);
  ~InstructionNode ();

  /**
   * Adds constraint as right-most child of this node and returns the newly instantiated
   * node. However, if the given constraint was already handled by current right-most
   * child of this node, the responsible node will be returned.
   * @param constraint The constraint which should be added.
   * @param memoryManager The memory manager which contains causing instruction.
   * @param performValidityCheck Ignore constraint if it is tautological.
   *
   * @return The node (possibly newly instantiated) which is responsible for constraint.
   */
  InstructionNode *addConstraint (const edu::sharif::twinner::trace::Constraint *constraint,
      const edu::sharif::twinner::util::MemoryManager *memoryManager,
      bool performValidityCheck,
      ConstraintEdge *&parentEdge);

  /**
   * Moves downward, traversing right-most children until a leaf is visited. During this
   * traversing, visited constraints are added to the given list.
   * @param clist List of constraints which are visited during the requested traversal.
   * @return A pointer to the right-most and deepest grandchild of this node.
   */
  InstructionNode *getRightMostDeepestGrandChild (
      std::list < const edu::sharif::twinner::trace::Constraint * > &clist);

  /**
   * Get next node of the current node assuming the infix visiting order among nodes.
   * Upon visiting leafs, a negated constraint (if not exist currently) is instantiated
   * and added to the tree and the target next node. If the negated node existed,
   * continues DFS and negation to find next node.
   * Constraints are removed upon moving upward and added upon moving downward.
   * If it isn't possible to instantiate a negated node (reaching the root node), null is
   * returned.
   * @param clist Constraints which are in correspondence to root-to-returned-node path.
   * @return The next node which should be visited or null if there was no such node.
   */
  InstructionNode *getNextNode (
      std::list < const edu::sharif::twinner::trace::Constraint * > &clist);

  const edu::sharif::twinner::util::MemoryManager *getMemoryManager () const;

  void dumpConstraints (edu::sharif::twinner::util::Logger &logger) const;
  void dumpSubTree (edu::sharif::twinner::util::Logger &logger,
      unsigned int pad = 0) const;

  bool hasAnyChild () const;
  void mergeCriticalAddresses (edu::sharif::twinner::trace::Snapshot *snapshot);
  const edu::sharif::twinner::trace::Snapshot *getSnapshot () const;

  const std::list < ConstraintEdge * > &getChildren () const;
  void addParent (ConstraintEdge *newParent);
  bool areInstructionsTheSame (const InstructionNode *tn) const;

  ConstraintEdge *getRightMostParent ();
  const std::list < ConstraintEdge * > &getParents () const;

  std::string toString () const;

  edu::sharif::twinner::engine::etg::encoder::NodeEncoder *getEncoder (
      const edu::sharif::twinner::engine::etg::encoder
      ::Encoder::AddrToSizeMap &addressToSize);

private:
  void registerInstructionIdIfRequired (
      const edu::sharif::twinner::trace::Constraint *c,
      const edu::sharif::twinner::util::MemoryManager *m);
  InstructionNode *addConstraint (InstructionNode *parent,
      unsigned int depth,
      const edu::sharif::twinner::trace::Constraint *c,
      const edu::sharif::twinner::util::MemoryManager *m,
      bool performValidityCheck,
      ConstraintEdge *&parentEdge);
};

}
}
}
}
}

#endif /* InstructionNode.h */
