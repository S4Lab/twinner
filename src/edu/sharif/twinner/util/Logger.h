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

#ifndef LOGGER_H_
#define LOGGER_H_

#include "pin.H"

#include <iostream>
#include <map>
#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class Constraint;
}
namespace util {

class Logger;

extern const Logger &operator<< (const Logger &logger, REG reg);

class Logger {

private:

  enum VerbosenessLevel {

    QUIET, ERROR, WARNING, INFO, DEBUG, LOQUACIOUS
  };

  static VerbosenessLevel verbose;

  const bool enabled;

public:
  Logger (bool enabled);
  ~Logger ();

  template <typename KEY, typename VALUE>
  const Logger &operator<< (const std::map < KEY, VALUE > map) const;

  template <typename VALUE>
  const Logger &operator<< (const std::list < VALUE > map) const;

  const Logger &operator<< (const edu::sharif::twinner::trace::Expression *exp) const;
  const Logger &operator<< (const edu::sharif::twinner::trace::Constraint *c) const;

  inline const Logger &operator<< (const std::string &t) const {
    return actualWrite (t);
  }

  inline const Logger &operator<< (const char t) const {
    return actualWrite (t);
  }

  inline const Logger &operator<< (const char *t) const {
    return actualWrite (t);
  }

  inline const Logger &operator<< (const int t) const {
    return actualWrite (t);
  }

  inline const Logger &operator<< (const unsigned int t) const {
    return actualWrite (t);
  }

  inline const Logger &operator<< (const long unsigned int t) const {
    return actualWrite (t);
  }

  inline const Logger &operator<< (const double t) const {
    return actualWrite (t);
  }

  template <typename T>
  const Logger &actualWrite (const T &t) const {
    if (enabled) {
      std::cout << t;
    }
    return *this;
  }

  operator bool () const {
    return enabled;
  }

  static Logger error ();
  static Logger warning ();
  static Logger info ();
  static Logger debug ();
  static Logger loquacious ();

  static bool setVerbosenessLevel (const std::string &verboseStr);
  static const char *getVerbosenessLevelAsString ();
};

template <typename KEY, typename VALUE>
const edu::sharif::twinner::util::Logger &Logger::operator<< (
    const std::map < KEY, VALUE > map) const {
  if (enabled) {
    for (typename std::map < KEY, VALUE >::const_iterator it = map.begin ();
        it != map.end (); ++it) {
      const KEY k = it->first;
      const VALUE v = it->second;

      (*this) << "Key(" << k << ") => Value(" << v << ")\n";
    }
  }
  return *this;
}

template <typename VALUE>
const edu::sharif::twinner::util::Logger &Logger::operator<< (
    const std::list < VALUE > map) const {
  if (enabled) {
    for (typename std::list < VALUE >::const_iterator it = map.begin ();
        it != map.end (); ++it) {
      const VALUE v = *it;
      (*this) << v;
    }
  }
  return *this;
}

}
}
}
}

#endif /* LOGGER_H_ */
