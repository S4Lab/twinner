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

#ifndef LOGGER_H_
#define LOGGER_H_

#include "pin.H"

#include <iostream>
#include <iomanip>
#include <map>
#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Expression;
class Constraint;

namespace exptoken {

class ExpressionToken;
}
namespace cv {

class ConcreteValue;
}
}
namespace util {

class Logger;

extern const Logger &operator<< (const Logger &logger, REG reg);

class Logger {

private:

  enum VerbosenessLevel {

    QUIET, ERROR, WARNING, INFO, DEBUG, LOQUACIOUS
  };

  static const char *NORMAL_COLOR;
  static const char *TYPE_COLOR;
  static const char *ERROR_COLOR;
  static const char *WARNING_COLOR;
  static const char *INFO_COLOR;
  static const char *DEBUG_COLOR;
  static const char *LOQUACIOUS_COLOR;

  static VerbosenessLevel verbose;

  const bool enabled;
  const char *color;

public:
  Logger (bool enabled, const char *type, const char *_color);
  ~Logger ();

  template <typename KEY, typename VALUE>
  const Logger &operator<< (const std::map < KEY, VALUE > map) const;

  template <typename VALUE>
  const Logger &operator<< (const std::list < VALUE > map) const;

  const Logger &operator<< (const edu::sharif::twinner::trace::Expression *exp) const;
  const Logger &operator<< (const edu::sharif::twinner::trace::Constraint *c) const;
  const Logger &operator<< (
      const edu::sharif::twinner::trace::exptoken::ExpressionToken *token) const;
  const Logger &operator<< (
      const edu::sharif::twinner::trace::cv::ConcreteValue &value) const;

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

  inline const Logger &operator<< (ios_base &(*func)(ios_base &)) const {
    return actualWrite (func);
  }

  inline const Logger &operator<< (struct std::_Setw w) const {
    return actualWrite (w);
  }

  template<typename _CharT>
  inline const Logger &operator<< (struct std::_Setfill<_CharT> f) const {
    return actualWrite (f);
  }

  template <typename T>
  const Logger &actualWrite (const T &t) const {
    if (enabled) {
      std::cout << color << t << NORMAL_COLOR;
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

      (*this) << std::hex << "Key(" << k << ") => Value(" << v << ")\n";
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
      (*this) << std::hex << v << '\n';
    }
  }
  return *this;
}

}
}
}
}

#endif /* LOGGER_H_ */
