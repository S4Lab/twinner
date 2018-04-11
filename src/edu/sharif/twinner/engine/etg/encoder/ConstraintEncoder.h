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

#ifndef CONSTRAINT_ENCODER_H
#define CONSTRAINT_ENCODER_H

#include "NodeEncoder.h"

#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

class ConstraintEdge;

namespace encoder {

class ConstraintEncoder : public Encoder {
private:
  const int conditionIndex;
  const TraceSegment *segment;

  std::set< Variable > typesAndNames;
  bool mustConstraintBeEncoded;
  std::string constraintString;

  std::set< Variable > oldVariablesUsedBySegment;
  std::set< Variable > newVariablesCreatedInSegment;

  NodeEncoder *child;

public:
  ConstraintEncoder (edu::sharif::twinner::engine::etg::ConstraintEdge *edge,
      const AddrToSizeMap &addressToSize,
      bool bypassConstraint);
  virtual ~ConstraintEncoder ();

  void initializeSecondPass ();
  std::set< Variable > getAggregatedVariables () const;

  /**
   * @return true iff the subgraph is encoded under an if-construct
   */
  virtual bool encode (IndentedStream &body, IndentedStream &preamble,
      int index, bool inMain);

private:
  void gatherOldVariablesOfSegment ();

  template <typename Address>
  void gatherOldVariablesOfSegment (const Address &key, ExpressionPtr exp) {
    extractTypesAndNames (oldVariablesUsedBySegment, exp);
  }

  void gatherOldVariablesOfSegment (const ConstExpressionPtr &exp) {
    extractTypesAndNames (oldVariablesUsedBySegment, exp);
  }

  void gatherNewVariablesOfSegment ();
  void gatherNewRegisterVariablesOfSegment (int index);
  void gatherNewMemoryVariablesOfSegment (
      const std::set < AddrToSize > &addrToSize, int index);

  struct Output {
    IndentedStream &body;
    bool inMain;
  };

  void encodeTransformations (Output &out,
      IndentedStream &preamble, const TraceSegment *segment, int index);

  void codeRegisterChanges (Output &out, const TraceSegment *segment);
  void codeMemoryChanges (Output &out,
      const Address &memoryEa, ExpressionPtr exp);

  /**
   * @return true iff the constraint is encoded using an if-construct
   */
  bool encodeConstraint (IndentedStream &body, IndentedStream &preamble,
      bool inMain);

  bool simplifyConstraints (std::list < ConstConstraintPtr > constraints,
      std::stringstream &ss);
  bool encodeConstraints (std::list < ConstConstraintPtr > constraints,
      std::stringstream &ss);

  std::string replaceAll (std::string str,
      std::string part, std::string substitute) const;
  void extractTypesAndNames (std::set< Variable > &typesAndNames,
      ConstConstraintPtr simplifiedConstraint) const;
  void extractTypesAndNames (std::set< Variable > &typesAndNames,
      ConstExpressionPtr exp) const;
};

}
}
}
}
}
}

#endif /* ConstraintEncoder.h */
