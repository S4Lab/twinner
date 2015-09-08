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

#include "MarInfo.h"

#include <fstream>
#include <stdexcept>

#include "Trace.h"
#include "Expression.h"
#include "ExecutionTraceSegment.h"

#include "edu/sharif/twinner/trace/exptoken/RegisterEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/NamedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

void *MarInfo::initialArgv = 0;

MarInfo::MarInfo (int _argc, char **_argv) :
    Savable (), argc (_argc), argv (_argv) {
  if (MarInfo::initialArgv == 0) {
    MarInfo::initialArgv = _argv;
  }
}

bool MarInfo::isConsistent () const {
  return MarInfo::initialArgv == argv;
}

void MarInfo::simplifyTrace (Trace *trace) const {
  std::list < ExecutionTraceSegment * > &segments = trace->getTraceSegments ();
  for (std::list < ExecutionTraceSegment * >::const_reverse_iterator it =
      segments.rbegin (); it != segments.rend (); ++it) {
    std::list < Constraint * > &constraints = (*it)->getPathConstraints ();
    for (std::list < Constraint * >::const_iterator it2 = constraints.begin ();
        it2 != constraints.end (); ++it2) {
      Constraint *constraint = *it2;
      simplifyExpression (constraint->getMainExpression ());
      simplifyExpression (constraint->getAuxExpression ());
    }
  }
}

void MarInfo::simplifyExpression (Expression *exp) const {
  if (exp == 0) {
    return;
  }
  Expression::Stack &stack = exp->getStack ();
  for (Expression::Stack::iterator it = stack.begin (); it != stack.end (); ++it) {
    edu::sharif::twinner::trace::exptoken::ExpressionToken *&token = *it;
#ifdef TARGET_IA32E
    // argv == rsi_0
    if (dynamic_cast<edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol *> (token)) {
      edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol *reg =
          static_cast<edu::sharif::twinner::trace::exptoken::RegisterEmergedSymbol *> (token);
      if (reg->getAddress () == REG_RSI && reg->getGenerationIndex () == 0) {
        token = new edu::sharif::twinner::trace::exptoken::NamedSymbol
            ("argv", true,
             edu::sharif::twinner::trace::cv::ConcreteValue64Bits
             (UINT64 (MarInfo::initialArgv)), 0);
        delete reg;
      }
    }
#else
#error "Unsupported architecture"
#endif
  }
}

void MarInfo::saveToFile (const char *path) const {
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "Can not report main() args: Error in open function: "
        << path << '\n';
    throw std::runtime_error ("Error in reporting main() args during opening file");
  }
  saveToBinaryStream (out);
  out.close ();
}

void MarInfo::saveToBinaryStream (std::ofstream &out) const {
  out.write (reinterpret_cast<const char *> (&argc), sizeof (argc));
  out.write (reinterpret_cast<const char *> (&argv), sizeof (argv));
}

MarInfo *MarInfo::readMarInfoFromFile (const char *path) {
  std::ifstream in;
  in.open (path, ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not read mar info:"
        " Error in open function: " << path << '\n';
    throw std::runtime_error ("Error in reading MAR info");
  } else {
    int argc;
    char **argv;
    in.read (reinterpret_cast<char *> (&argc), sizeof (argc));
    in.read (reinterpret_cast<char *> (&argv), sizeof (argv));
    in.close ();
    return new MarInfo (argc, argv);
  }
}

}
}
}
}
