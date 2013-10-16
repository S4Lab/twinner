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

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class Logger {
private:
  enum VerbosenessLevel {
    QUIET, ERROR, WARNING, INFO, DEBUG
  };

  static VerbosenessLevel verbose;

  const bool enabled;

public:
  Logger (bool enabled);
  ~Logger ();

  template < typename T >
  Logger &operator<< (const T &t) {
    if (enabled) {
      std::cout << t;
    }
    return *this;
  }

  template < typename T >
  const Logger &operator<< (const T &t) const {
    if (enabled) {
      std::cout << t;
    }
    return *this;
  }

  operator bool () const {
    return enabled;
  }

  static Logger error ();
  static Logger warning ();
  static Logger info ();
  static Logger debug ();

  static bool setVerbosenessLevel (const std::string &verboseStr);
};

}
}
}
}

#endif /* LOGGER_H_ */
