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

#include "LogStream.h"

#include "Logger.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <stdlib.h>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

LogStream::VerbosenessLevel LogStream::verbose = LogStream::WARNING_VERBOSENESS;

LogStream::~LogStream () {
}

bool LogStream::setVerbosenessLevel (const std::string &verboseStr) {
  if (verboseStr == "quiet") {
    verbose = QUIET_VERBOSENESS;
  } else if (verboseStr == "error") {
    verbose = ERROR_VERBOSENESS;
  } else if (verboseStr == "warning") {
    verbose = WARNING_VERBOSENESS;
  } else if (verboseStr == "info") {
    verbose = INFO_VERBOSENESS;
  } else if (verboseStr == "debug") {
    verbose = DEBUG_VERBOSENESS;
  } else if (verboseStr == "loquacious") {
    verbose = LOQUACIOUS_VERBOSENESS;
  } else {
    return false;
  }
  return true;
}

const char *LogStream::getVerbosenessLevelAsString () {
  switch (verbose) {
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
        << "Logger::getVerbosenessLevelAsString ()"
        " [verbose=" << std::dec << verbose << "]: "
        "corrupted verboseness level\n";
    abort ();
  }
}

void LogStream::flush () {
  std::cout.flush ();
}

}
}
}
}
