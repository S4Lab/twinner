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
  static LogStream *me;

  const VerbosenessLevel verboseness;
  friend class Logger;

  LogStream (VerbosenessLevel level);

public:
  ~LogStream ();

  static bool init (std::string verboseStr, std::string logfile);
  static LogStream *getInstance ();
  static void destroy ();

  std::string getVerbosenessLevelAsString () const;
  VerbosenessLevel getVerbosenessLeivel () const;

  void flush ();

  static VerbosenessLevel stringToVerbosenessLevel (std::string verboseStr);
  static std::string verbosenessLevelToString (VerbosenessLevel verboseness);

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
