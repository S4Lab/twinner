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

#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <iostream>
#include <fstream>
#include <iomanip>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

namespace internal {

enum VerbosenessLevel {
  QUIET_VERBOSENESS, ERROR_VERBOSENESS,
  WARNING_VERBOSENESS, INFO_VERBOSENESS,
  DEBUG_VERBOSENESS, LOQUACIOUS_VERBOSENESS
};

VerbosenessLevel stringToVerbosenessLevel (std::string verboseStr);
std::string verbosenessLevelToString (VerbosenessLevel verboseness);
}

class LogStream {
private:
  static LogStream *me;

  const internal::VerbosenessLevel verboseness;
  const std::string logfileName;
  std::ofstream out;

  LogStream (internal::VerbosenessLevel level, const std::string &logfile);

public:
  ~LogStream ();

  static bool init (std::string verboseStr, std::string logfile);
  static LogStream *getInstance ();
  static void destroy ();

  std::string getVerbosenessLevelAsString () const;
  internal::VerbosenessLevel getVerbosenessLevel () const;
  std::string getLogfileName () const;

  void flush ();

  template <typename T>
  LogStream &write (const T &t) {
    out << t;
    return *this;
  }
};


}
}
}
}

#endif /* LogStream.h */
