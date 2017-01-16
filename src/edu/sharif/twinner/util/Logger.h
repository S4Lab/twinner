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

#ifndef LOGGER_H_
#define LOGGER_H_

#include "edu/sharif/twinner/pin-wrapper.h"

#include <iostream>
#include <iomanip>
#include <map>
#include <list>

#include "LogStream.h"

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

namespace internal {

extern const char *NORMAL_COLOR;
extern const char *TYPE_COLOR;
extern const char *ERROR_COLOR;
extern const char *WARNING_COLOR;
extern const char *INFO_COLOR;
extern const char *DEBUG_COLOR;
extern const char *LOQUACIOUS_COLOR;
}

class Logger;

extern const Logger &operator<< (const Logger &logger, LEVEL_BASE::REG reg);

class Logger {
private:
  LogStream &stream;
  const bool enabled;
  const char *color;

public:
  Logger (internal::VerbosenessLevel level,
      const char *type, const char *color);
  ~Logger ();

  static Logger error ();
  static Logger warning ();
  static Logger info ();
  static Logger debug ();
  static Logger loquacious ();

  template <typename KEY, typename VALUE>
  const Logger &operator<< (const std::map < KEY, VALUE > &map) const;

  template <typename VALUE>
  const Logger &operator<< (const std::list < VALUE > &list) const;

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

  inline const Logger &operator<< (const long int t) const {
    return actualWrite (t);
  }

  inline const Logger &operator<< (const double t) const {
    return actualWrite (t);
  }

  inline const Logger &operator<< (std::ios_base &(*f)(std::ios_base &)) const {
    return actualWrite (f);
  }
#ifdef REAL_TARGET_IS_WINDOWS

  inline const Logger &operator<< (const UINT64 u) const {
    return actualWrite (u);
  }
#endif
#ifdef TARGET_LINUX

  inline const Logger &operator<< (struct std::_Setw w) const {
    return actualWrite (w);
  }

  template<typename _CharT>
  inline const Logger &operator<< (struct std::_Setfill<_CharT> f) const {
    return actualWrite (f);
  }

  inline const Logger &operator<< (struct std::_Setprecision f) const {
    return actualWrite (f);
  }
#endif
#ifdef TARGET_WINDOWS

  template<class _Arg>
  inline const Logger &operator<< (struct std::_Smanip<_Arg> w) const {
    return actualWrite (w);
  }

  template<class _Elem>
  inline const Logger &operator<< (struct std::_Fillobj<_Elem> f) const {
    return actualWrite (f);
  }

  inline const Logger &operator<< (const UINT8 u) const {
    return actualWrite (u);
  }

  inline const Logger &operator<< (const OPCODE op) const {
    return actualWrite (op);
  }

  inline const Logger &operator<< (const bool b) const {
    return actualWrite (b);
  }
#endif

  template <typename T>
  inline const Logger &actualWrite (const T &value) const {
    return actualWrite (value, color);
  }

  template <typename T>
  const Logger &actualWrite (const T &value, const char *_color) const {
    if (enabled) {
      stream.write (_color).write (value).write (internal::NORMAL_COLOR);
    }
    return *this;
  }
};

template <typename KEY, typename VALUE>
const Logger &Logger::operator<< (const std::map < KEY, VALUE > &map) const {
  if (enabled) {
    (*this) << "Map{\n";
    for (typename std::map < KEY, VALUE >::const_iterator it = map.begin ();
        it != map.end (); ++it) {
      (*this) << std::hex << "Key(" << it->first << ")"
          " => Value(" << it->second << ")\n";
    }
    (*this) << "}\n";
  }
  return *this;
}

template <typename VALUE>
const Logger &Logger::operator<< (const std::list < VALUE > &list) const {
  if (enabled) {
    (*this) << "List{\n";
    for (typename std::list < VALUE >::const_iterator it = list.begin ();
        it != list.end (); ++it) {
      (*this) << std::hex << (*it) << '\n';
    }
    (*this) << "}\n";
  }
  return *this;
}

}
}
}
}

#endif /* Logger.h */
