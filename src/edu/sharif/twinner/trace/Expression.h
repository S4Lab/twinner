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

class Expression {

private:
  int stack;
  int lastConcreteValue;

public:
  void toString ();

  void unaryOperation (edu::sharif::twinner::trace::Operator op,
      edu::sharif::twinner::trace::Expression exp);

  void binaryOperation (edu::sharif::twinner::trace::Operator op,
      edu::sharif::twinner::trace::Expression exp);

  edu::sharif::twinner::trace::Expression clone ();
};

}
}
}
}
