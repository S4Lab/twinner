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

class Expression;
class Constraint;
class Syscall;

class Trace {

private:
  void getCurrentTraceSegment ();

public:
  Expression getSymbolicExpression (int address);

  void addPathConstraint (Constraint c);

  void setSymbolicExpression (int address, Expression exp);

  void syscallInvoked (Syscall s);

  void saveToFile ();

  void loadFromFile ();
};

}
}
}
}
