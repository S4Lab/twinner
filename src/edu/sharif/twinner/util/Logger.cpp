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

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

Logger::VerbosenessLevel Logger::verbose = Logger::WARNING;

const char *Logger::NORMAL_COLOR = "\x1B[0m";
const char *Logger::TYPE_COLOR = "\x1B[37;45m"; // BLACK on WHITE
const char *Logger::ERROR_COLOR = "\x1B[31m"; // RED
const char *Logger::WARNING_COLOR = "\x1B[33;1m"; // BOLD YELLOW
const char *Logger::INFO_COLOR = "\x1B[33m"; // YELLOW
const char *Logger::DEBUG_COLOR = "\x1B[32m"; // GREEN
const char *Logger::LOQUACIOUS_COLOR = "\x1B[34m"; // BLUE

Logger::Logger (bool _enabled, const char *type, const char *_color) :
enabled (_enabled), color (_color) {
  if (enabled) {
    std::cout << '[' << TYPE_COLOR << type << NORMAL_COLOR << "]: ";
  }
}

Logger::~Logger () {
  std::cout.flush ();
}

Logger Logger::error () {
  return Logger (Logger::verbose >= Logger::ERROR, "ERROR", ERROR_COLOR);
}

Logger Logger::warning () {
  return Logger (Logger::verbose >= Logger::WARNING, "WARNING", WARNING_COLOR);
}

Logger Logger::info () {
  return Logger (Logger::verbose >= Logger::INFO, "INFO", INFO_COLOR);
}

Logger Logger::debug () {
  return Logger (Logger::verbose >= Logger::DEBUG, "DEBUG", DEBUG_COLOR);
}

Logger Logger::loquacious () {
  return Logger (Logger::verbose >= Logger::LOQUACIOUS, "LOQUACIOUS", LOQUACIOUS_COLOR);
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
  } else if (verboseStr == "loquacious") {
    verbose = LOQUACIOUS;
  } else {
    return false;
  }
  return true;
}

const char *Logger::getVerbosenessLevelAsString () {
  switch (verbose) {
  case QUIET:
    return "quiet";
  case ERROR:
    return "error";
  case WARNING:
    return "warning";
  case INFO:
    return "info";
  case DEBUG:
    return "debug";
  case LOQUACIOUS:
    return "loquacious";
  default:
    throw std::runtime_error ("Verboseness level is corrupted");
  }
}

const Logger &Logger::operator<< (
    const edu::sharif::twinner::trace::Expression *exp) const {
  return (*this) << "Expression(" << exp->toString () << ')';
}

const Logger &Logger::operator<< (
    const edu::sharif::twinner::trace::Constraint *c) const {
  return (*this) << "Constraint(" << c->toString () << ')';
}

const Logger &Logger::operator<< (
    const edu::sharif::twinner::trace::ExpressionToken *token) const {
  return (*this) << "Token(" << token->toString () << ')';
}


}
}
}
}
