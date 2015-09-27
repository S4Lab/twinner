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
#include "edu/sharif/twinner/trace/exptoken/MemoryEmergedSymbol.h"
#include "edu/sharif/twinner/trace/exptoken/NamedSymbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

void *MarInfo::initialArgv = 0;

MarInfo::MarInfo (int _argc, char **_argv) :
    Savable (), argc (_argc), argv (_argv), inspectionMode (false) {
}

MarInfo::MarInfo (int _argc, char **_argv, std::vector<char *> _argvis) :
    Savable (), argc (_argc), argv (_argv), argvis (_argvis),
    inspectionMode (true) {
  if (MarInfo::initialArgv == 0) {
    MarInfo::initialArgv = _argv;
  }
}

bool MarInfo::isConsistent () const {
  if (!inspectionMode) {
    throw std::runtime_error ("Method is only available in inspection mode");
  }
  return MarInfo::initialArgv == argv;
}

void MarInfo::simplifyTrace (Trace *trace) const {
  if (!inspectionMode) {
    throw std::runtime_error ("Method is only available in inspection mode");
  }
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
    } else if (dynamic_cast<edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *> (token)) {
      edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *mem =
          static_cast<edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *> (token);
      if (mem->getGenerationIndex () == 0) {
        const ADDRINT addr = mem->getAddress ();
        const UINT64 argv0 = UINT64 (argv);
        if (argv0 <= addr) {
          const UINT64 diff = addr - argv0;
          if (diff % 8 == 0) {
            const int i = diff / 8;
            if (i < argc) {
              std::stringstream ss;
              ss << "argv[" << i << "]";
              token = new edu::sharif::twinner::trace::exptoken::NamedSymbol
                  (ss.str (), false,
                   // storing the address instead of the argv[i] value
                   edu::sharif::twinner::trace::cv::ConcreteValue64Bits (addr),
                   0);
              delete mem;
            }
          }
        }
      }
    }
#else
#error "Unsupported architecture"
#endif
  }
}

void MarInfo::saveToFile (const char *path) const {
  if (inspectionMode) {
    throw std::runtime_error ("Method is not available in inspection mode");
  }
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
  for (int i = 0; i < argc; ++i) {
    char *argvi = argv[i];
    out.write (reinterpret_cast<const char *> (&argvi), sizeof (argvi));
  }
}

MarInfo *MarInfo::readMarInfoFromFile (const char *path) {
  std::ifstream in;
  in.open (path, ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error () << "Can not read mar info:"
        " Error in open function: " << path << '\n';
    throw std::runtime_error ("Error in reading MAR info");
  } else {
    MarInfo *res = MarInfo::loadFromBinaryStream (in);
    in.close ();
    return res;
  }
}

MarInfo *MarInfo::loadFromBinaryStream (std::ifstream &in) {
  int argc;
  char **argv;
  in.read (reinterpret_cast<char *> (&argc), sizeof (argc));
  in.read (reinterpret_cast<char *> (&argv), sizeof (argv));
  std::vector<char *> argvis;
  for (int i = 0; i < argc; ++i) {
    char *argvi;
    in.read (reinterpret_cast<char *> (&argvi), sizeof (argvi));
    argvis.push_back (argvi);
  }
  return new MarInfo (argc, argv, argvis);
}

}
}
}
}
