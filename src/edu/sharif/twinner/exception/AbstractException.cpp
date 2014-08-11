//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#include "AbstractException.h"

#include "edu/sharif/twinner/util/Logger.h"

#include <execinfo.h>
#include <cstddef>
#include <cxxabi.h>
#include <stdlib.h>

namespace edu {
namespace sharif {
namespace twinner {
namespace exception {

AbstractException::AbstractException (const char *_msg) :
    msg (_msg), backtraceSymbols (fetchBacktraceSymbols ()) {
}

AbstractException::AbstractException (const std::string &_msg) :
    msg (_msg), backtraceSymbols (fetchBacktraceSymbols ()) {
}

AbstractException::~AbstractException () throw () {
  if (backtraceSymbols.first) {
    const void *symbols = reinterpret_cast<const void *> (backtraceSymbols.first);
    free (const_cast<void *> (symbols));
  }
}

const char *AbstractException::what () const throw () {
  return msg.c_str ();
}

std::pair < char * const *, int > AbstractException::fetchBacktraceSymbols () {
  static const int backtraceMaxSteps = 20;
  void *array[backtraceMaxSteps];
  int size = backtrace (array, backtraceMaxSteps);
  char **symbols = backtrace_symbols (array, size);
  return std::pair < char * const *, int > (symbols, size);
}

void AbstractException::printStackBacktrace () const {
  edu::sharif::twinner::util::Logger out = edu::sharif::twinner::util::Logger::error ();
  if (!backtraceSymbols.first) {
    out << "No stack backtrace is available!\n";
    return;
  }
  out << "Stack backtrace:\n";
  // This is required for holding the demangled function name.
  // It must be malloc()ed since used library wishes to realloc() it as required.
  std::size_t functionNameSize = 512;
  char *functionName = (char *) malloc (functionNameSize);

  // two first entries are for fetchBacktraceSymbols and AbstractException constructor.
  for (int i = 2; i < backtraceSymbols.second; i++) {
    char * const line = backtraceSymbols.first[i];
    char *begin, *offset, *end;
    if (findFunctionLineParts (line, begin, offset, end)) {
      int status;
      char *name = abi::__cxa_demangle (begin, functionName, &functionNameSize, &status);
      if (status == 0) {
        functionName = name; // use the (possibly) realloc()-ed function name
        out << line << ":" << functionName << "+" << offset << end;
      } else {
        // error while demangling
        out << line << ":" << begin << "+" << offset << end;
      }
      out << line << ":" << functionName << "+" << offset << end;
    } else {
      // line can not be divided to more fine grained parts
      out << line;
    }
    out << '\n';
  }
  free (functionName);
}

bool AbstractException::findFunctionLineParts (char *line, char *&begin, char *&offset,
    char *&end) const {
  begin = offset = end = 0;
  // one of the followings must be in line:
  // ./program(mangled_function+0x12345) [0x987654321]
  // ./program() [0x987654321]
  while (*line) {
    switch (*line) {
    case '(':
      *line = 0;
      begin = ++line; // "mangled_function"
      break;

    case '+':
      *line = 0;
      offset = ++line; // "0x12345"
      break;

    case ')':
      *line = 0;
      end = ++line; // " [0x987654321]"
      return begin && offset && end && begin < offset && offset < end;

    default:
      ++line;
      break;
    }
  }
  return false;
}

}
}
}
}
