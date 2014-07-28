//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "ConcreteValueAbstractImp.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

template <>
bool ConcreteValueAbstractImp<128>::isNegative () const {
  // ASSERT: bits <= 64 OTHERWISE: specialize this method or override it in child class
  throw std::runtime_error ("Bit length > 64;"
                            " override the isNegative () in the child class");
}

template <>
bool ConcreteValueAbstractImp<128>::isZero () const {
  // ASSERT: bits <= 64 OTHERWISE: specialize this method or override it in child class
  throw std::runtime_error ("Bit length > 64;"
                            " override the isZero () in the child class");
}

}
}
}
}
