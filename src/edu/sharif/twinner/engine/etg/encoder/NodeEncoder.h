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

#ifndef NODE_ENCODER_H
#define NODE_ENCODER_H

#include "Encoder.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {

class InstructionNode;

namespace encoder {

class ConstraintEncoder;

struct Variable {
  std::string type;
  std::string technicalName;
  std::string name;

  Variable (std::string _type, std::string _technicalName, std::string _name)
  : type (_type), technicalName (_technicalName), name (_name) {
  }

  bool operator< (const Variable &v) const {
    return technicalName < v.technicalName;
  }
};

class NodeEncoder : public Encoder {
private:
  bool secondPass;
  bool aggregatingSetOfVariables;

  ConstraintEncoder *leftChild;
  ConstraintEncoder *rightChild;

protected:
  std::set< Variable > aggregatedVariables;

public:
  NodeEncoder (const AddrToSizeMap &addressToSize);
  virtual ~NodeEncoder ();

  void initializeFirstPass (
      edu::sharif::twinner::engine::etg::InstructionNode *node);
  void initializeSecondPass ();
  std::set< Variable > getAggregatedVariables ();

  void encodeMain (IndentedStream &body, IndentedStream &preamble,
      const MemoryValueMap &initialValues);
  bool hasAnyChild () const;

  virtual void encode (IndentedStream &body, IndentedStream &preamble,
      int index, bool inMain);

protected:
  virtual void finalizeInitialization ();

private:
  void initializeMemory (IndentedStream &body,
      const MemoryValueMap &initialValues);
  void initializeMemory (IndentedStream &body,
      const AddrToSize &address, const Value &value);
};

}
}
}
}
}
}

#endif /* NodeEncoder.h */
