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

#include "Symbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

Symbol::Symbol (const edu::sharif::twinner::trace::cv::ConcreteValue &_concreteValue,
    int _generationIndex, int _snapshotIndex) :
    Operand (_concreteValue),
    generationIndex (_generationIndex), snapshotIndex (_snapshotIndex) {
}

Symbol::Symbol (const Symbol &s) :
    Operand (s),
    generationIndex (s.generationIndex), snapshotIndex (s.snapshotIndex) {
}

Symbol::Symbol () :
    Operand (), generationIndex (0), snapshotIndex (-1) {
}

void Symbol::saveToBinaryStream (std::ofstream &out) const {
  concreteValue->saveToBinaryStream (out);
  out.write ((const char *) &generationIndex, sizeof (generationIndex));
  out.write ((const char *) &snapshotIndex, sizeof (snapshotIndex));
}

void Symbol::loadFromBinaryStream (std::ifstream &in) {
  delete concreteValue;
  concreteValue = edu::sharif::twinner::trace::cv::ConcreteValue::loadFromBinaryStream (in);
  in.read ((char *) &generationIndex, sizeof (generationIndex));
  in.read ((char *) &snapshotIndex, sizeof (snapshotIndex));
}

int Symbol::getGenerationIndex () const {
  return generationIndex;
}

int Symbol::getSnapshotIndex () const {
  return snapshotIndex;
}

bool Symbol::isConstant () const {
  return false;
}

bool Symbol::isTemporary () const {
  return snapshotIndex != -1;
}

}
}
}
}
}
