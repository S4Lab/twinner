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

#ifndef FLAGS_H
#define FLAGS_H

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
}
namespace twintool {

class Flags {

private:
  edu::sharif::twinner::trace::Expression *exp;

public:
  Flags ();
  ~Flags ();

  /**
   * Sets all flags based on given expression. It's assumed that given
   * expression is owned by this object.
   * @param exp The expression which flags are set based on it.
   */
  void setFlags (edu::sharif::twinner::trace::Expression *exp);

  /**
   * Returns the underlying expression which currently, flags are set based on it. The
   * returned expression is still owned by this object and caller must clone it
   * if required.
   * @return The underlying expression of flags object.
   */
  const edu::sharif::twinner::trace::Expression *getFlagsUnderlyingExpression () const;
};

}
}
}
}

#endif /* Flags.h */
