//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2015  Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef ABSTRACT_EXCEPTION_H_
#define ABSTRACT_EXCEPTION_H_

#include <stdexcept>

namespace edu {
namespace sharif {
namespace twinner {
namespace exception {

/**
 * Name:              AbstractException
 * Parent:            std::exception
 * Description:       Parent of all exceptions which are thrown from
 *                    the 'edu.sharif.twinner' package and its subpackages.
 *
 * Package Access:    Public
 * Package:           edu.sharif.twinner.exception
 * Tags:              Exception Class;
 */
class AbstractException : public std::exception {

private:
  const std::string msg;
  std::pair < char * const *, int > backtraceSymbols;

public:

  AbstractException (const char *_msg);
  AbstractException (const std::string &_msg);
  virtual ~AbstractException () throw ();

  virtual const char *what () const throw ();

  void printStackBacktrace () const;

private:
  static std::pair < char * const *, int > fetchBacktraceSymbols ();
  bool findFunctionLineParts (char *line, char *&begin, char *&offset, char *&end) const;
};

}
}
}
}

#endif /* AbstractException.h */
