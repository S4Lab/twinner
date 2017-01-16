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

#include "Operand.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {
namespace exptoken {

Operand::Operand () :
    ExpressionToken (), concreteValue (0) {
}

Operand::Operand (const edu::sharif::twinner::trace::cv::ConcreteValue &cv) :
    ExpressionToken (), concreteValue (cv.clone ()) {
}

Operand::Operand (edu::sharif::twinner::trace::cv::ConcreteValue *cv) :
    ExpressionToken (), concreteValue (cv) {
}

Operand::Operand (const Operand &op) :
    ExpressionToken (op), concreteValue (op.concreteValue->clone ()) {
}

Operand::~Operand () {
  delete concreteValue;
}

const edu::sharif::twinner::trace::cv::ConcreteValue &Operand::getValue () const {
  return *concreteValue;
}

}
}
}
}
}
