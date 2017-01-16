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

#include "LogStream.h"

#include "Logger.h"

#include <stdexcept>
#include <sstream>
#include <stdlib.h>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

LogStream *LogStream::me = 0;

LogStream::LogStream (internal::VerbosenessLevel level, const std::string &logfile) :
    verboseness (level), logfileName (logfile),
    out ((logfile + ".log").c_str ()) {
  if (!out.is_open ()) {
    std::cerr << "Cannot open the logfile: " << logfile << std::endl;
    abort ();
  }
}

LogStream::~LogStream () {
  out.close ();
}

bool LogStream::init (std::string verboseStr, std::string logfile) {
  if (LogStream::me) {
    return false;
  }
  LogStream::me =
      new LogStream (internal::stringToVerbosenessLevel (verboseStr), logfile);
  return true;
}

LogStream *LogStream::getInstance () {
  return LogStream::me;
}

void LogStream::destroy () {
  delete LogStream::me;
  LogStream::me = 0;
}

std::string LogStream::getVerbosenessLevelAsString () const {
  return internal::verbosenessLevelToString (verboseness);
}

internal::VerbosenessLevel LogStream::getVerbosenessLevel () const {
  return verboseness;
}

std::string LogStream::getLogfileName () const {
  return logfileName;
}

void LogStream::flush () {
  out.flush ();
}

namespace internal {

VerbosenessLevel stringToVerbosenessLevel (std::string verboseStr) {
  if (verboseStr == "quiet") {
    return QUIET_VERBOSENESS;
  } else if (verboseStr == "error") {
    return ERROR_VERBOSENESS;
  } else if (verboseStr == "warning") {
    return WARNING_VERBOSENESS;
  } else if (verboseStr == "info") {
    return INFO_VERBOSENESS;
  } else if (verboseStr == "debug") {
    return DEBUG_VERBOSENESS;
  } else if (verboseStr == "loquacious") {
    return LOQUACIOUS_VERBOSENESS;
  } else {
    edu::sharif::twinner::util::Logger::error ()
        << "stringToVerbosenessLevel (...)"
        " [verboseStr=" << std::dec << verboseStr << "]: "
        "corrupted verboseness level\n";
    abort ();
  }
}

std::string verbosenessLevelToString (VerbosenessLevel verboseness) {
  switch (verboseness) {
  case QUIET_VERBOSENESS:
    return "quiet";
  case ERROR_VERBOSENESS:
    return "error";
  case WARNING_VERBOSENESS:
    return "warning";
  case INFO_VERBOSENESS:
    return "info";
  case DEBUG_VERBOSENESS:
    return "debug";
  case LOQUACIOUS_VERBOSENESS:
    return "loquacious";
  default:
    edu::sharif::twinner::util::Logger::error ()
        << "verbosenessLevelToString (...)"
        " [verbose=" << std::dec << verboseness << "]: "
        "corrupted verboseness level\n";
    abort ();
  }
}

}

}
}
}
}
