//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <iostream>
#include <fstream>
#include <iomanip>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

class LogStream {
private:
  enum VerbosenessLevel {
    QUIET_VERBOSENESS, ERROR_VERBOSENESS,
    WARNING_VERBOSENESS, INFO_VERBOSENESS,
    DEBUG_VERBOSENESS, LOQUACIOUS_VERBOSENESS
  };

  static VerbosenessLevel verbose;
  friend class Logger;

public:
  ~LogStream ();

  static bool setVerbosenessLevel (const std::string &verboseStr);
  static const char *getVerbosenessLevelAsString ();

  void flush ();

  template <typename T>
  LogStream &write (const T &t) {
    std::cout << t;
    return *this;
  }
};


}
}
}
}

#endif /* LogStream.h */
