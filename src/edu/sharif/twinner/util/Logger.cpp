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

#include "edu/sharif/twinner/pin-wrapper.h"

#include "Logger.h"

#include "edu/sharif/twinner/trace/Expression.h"
#include "edu/sharif/twinner/trace/Constraint.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

#include <stdexcept>
#include <sstream>

namespace edu {
namespace sharif {
namespace twinner {
namespace util {

namespace internal {

const char *NORMAL_COLOR = "\x1B[0m";
const char *TYPE_COLOR = "\x1B[37;45m"; // BLACK on WHITE
const char *ERROR_COLOR = "\x1B[31m"; // RED
const char *WARNING_COLOR = "\x1B[33;1m"; // BOLD YELLOW
const char *INFO_COLOR = "\x1B[33m"; // YELLOW
const char *DEBUG_COLOR = "\x1B[32m"; // GREEN
const char *LOQUACIOUS_COLOR = "\x1B[34m"; // BLUE

}

Logger::Logger (internal::VerbosenessLevel level,
    const char *type, const char *_color) :
    stream (*LogStream::getInstance ()),
    enabled (level <= stream.getVerbosenessLevel ()),
    color (_color) {
  if (enabled) {
    stream.write ('[');
    actualWrite (type, internal::TYPE_COLOR);
    stream.write ("]: ");
  }
}

Logger::~Logger () {
  stream.flush ();
}

Logger Logger::error () {
  return Logger (internal::ERROR_VERBOSENESS,
                 "ERROR", internal::ERROR_COLOR);
}

Logger Logger::warning () {
  return Logger (internal::WARNING_VERBOSENESS,
                 "WARNING", internal::WARNING_COLOR);
}

Logger Logger::info () {
  return Logger (internal::INFO_VERBOSENESS,
                 "INFO", internal::INFO_COLOR);
}

Logger Logger::debug () {
  return Logger (internal::DEBUG_VERBOSENESS,
                 "DEBUG", internal::DEBUG_COLOR);
}

Logger Logger::loquacious () {
  return Logger (internal::LOQUACIOUS_VERBOSENESS,
                 "LOQUACIOUS", internal::LOQUACIOUS_COLOR);
}

const Logger &Logger::operator<<
(const edu::sharif::twinner::trace::Expression *exp) const {
  return (*this) << "Expression(" << exp->toString () << ')';
}

const Logger &Logger::operator<<
(const edu::sharif::twinner::trace::Constraint *c) const {
  return (*this) << "Constraint(" << c->toString () << ')';
}

const Logger &Logger::operator<<
(const edu::sharif::twinner::trace::exptoken::ExpressionToken *token) const {
  return (*this) << "Token(" << token->toString () << ')';
}

const Logger &Logger::operator<<
(const edu::sharif::twinner::trace::cv::ConcreteValue &value) const {
  if (dynamic_cast<const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *> (&value)) {
    const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *value128imp =
        static_cast<const edu::sharif::twinner::trace::cv::ConcreteValue128Bits *> (&value);
    std::stringstream ss;
    ss << "ConcreteValue128Bits(0x" << std::hex
        << value128imp->getMsb () << std::setw (16) << std::setfill ('0')
        << value128imp->getLsb () << ')';
    return (*this) << ss.str ();
  } else {
    return (*this) << "ConcreteValue" << std::dec << value.getSize () << "Bits(0x"
        << std::hex << value.toUint64 () << ')';
  }
}

}
}
}
}
