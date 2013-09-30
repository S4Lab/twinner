//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace {

private:
  void getCurrentTraceSegment ();

public:
  edu::sharif::twinner::trace::Expression getSymbolicExpression (int address);

  void addPathConstraint (edu::sharif::twinner::trace::Constraint c);

  void setSymbolicExpression (int address, edu::sharif::twinner::trace::Expression exp);

  void syscallInvoked (edu::sharif::twinner::trace::Syscall s);

  void saveToFile ();

  void loadFromFile ();
};

}
}
}
}
