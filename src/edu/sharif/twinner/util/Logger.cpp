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

#include "Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

Logger::VerbosenessLevel Logger::verbose = Logger::WARNING;

Logger::Logger (bool _enabled) :
enabled (_enabled) {
}

Logger::~Logger () {
  std::cout.flush ();
}

Logger Logger::error () {
  return Logger (Logger::verbose >= Logger::ERROR);
}

Logger Logger::warning () {
  return Logger (Logger::verbose >= Logger::WARNING);
}

Logger Logger::info () {
  return Logger (Logger::verbose >= Logger::INFO);
}

Logger Logger::debug () {
  return Logger (Logger::verbose >= Logger::DEBUG);
}

bool Logger::setVerbosenessLevel (const std::string &verboseStr) {
  if (verboseStr == "quiet") {
    verbose = QUIET;
  } else if (verboseStr == "error") {
    verbose = ERROR;
  } else if (verboseStr == "warning") {
    verbose = WARNING;
  } else if (verboseStr == "info") {
    verbose = INFO;
  } else if (verboseStr == "debug") {
    verbose = DEBUG;
  } else {
    return false;
  }
  return true;
}

}
}
}
}
