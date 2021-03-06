//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2018 Behnam Momeni
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
#include "edu/sharif/twinner/trace/cv/ConcreteValue8Bits.h"

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

void *MarInfo::initialArgv = 0;

MarInfo::MarInfo (int _argc, char **_argv) :
    Savable (), argc (_argc), argv (_argv), inspectionMode (false) {
  if (MarInfo::initialArgv == 0) {
    MarInfo::initialArgv = _argv;
  }
  for (int i = 0; i < argc; ++i) {
    char *argvi = argv[i];
    const int argvilen = strlen (argvi);
    char *argvicontent = new char[argvilen + 1];
    memcpy (argvicontent, argvi, argvilen + 1);

    argvis.push_back (argvi);
    argvilens.push_back (argvilen);
    argvicontents.push_back (argvicontent);
  }
}

MarInfo::MarInfo (int _argc, char **_argv, std::vector<char *> _argvis,
    std::vector<int> _argvilens, std::vector<char *> _argvicontents) :
    Savable (), argc (_argc), argv (_argv), argvis (_argvis),
    argvilens (_argvilens), argvicontents (_argvicontents),
    inspectionMode (true) {
  if (MarInfo::initialArgv == 0) {
    MarInfo::initialArgv = _argv;
  }
}

MarInfo::~MarInfo () {
  for (std::vector<char *>::const_iterator it = argvicontents.begin ();
      it != argvicontents.end (); ++it) {
    const char *argvicontent = *it;
    delete[] argvicontent;
  }
  argvicontents.clear ();
}

bool MarInfo::isConsistent () const {
  if (!inspectionMode) {
    edu::sharif::twinner::util::Logger::error () << "MarInfo::isConsistent ():"
        " Method is only available in inspection mode\n";
    abort ();
  }
  return MarInfo::initialArgv == argv;
}

void MarInfo::simplifyTrace (Trace *trace) const {
  if (!inspectionMode) {
    edu::sharif::twinner::util::Logger::error ()
        << "MarInfo::simplifyTrace (...):"
        " Method is only available in inspection mode\n";
    abort ();
  }
  std::list < ExecutionTraceSegment * > &segments = trace->getTraceSegments ();
  for (std::list < ExecutionTraceSegment * >::const_reverse_iterator it =
      segments.rbegin (); it != segments.rend (); ++it) {
    const std::list < Snapshot * > &snapshots = (*it)->getSnapshots ();
    for (std::list < Snapshot * >::const_iterator it2 = snapshots.begin ();
        it2 != snapshots.end (); ++it2) {
      Snapshot *snapshot = *it2;
      snapshot->simplify (this);
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
      if (reg->getAddress () == REG_RSI
          && reg->getGenerationIndex () == 0
          && reg->getSnapshotIndex () == 0) {
        token = new edu::sharif::twinner::trace::exptoken::NamedSymbol
            ("argv", "n_c_argv", true,
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
              std::stringstream ss, ss2;
              ss << "argv[" << i << "]";
              ss2 << "n_v_argv_" << i;
              token = new edu::sharif::twinner::trace::exptoken::NamedSymbol
                  (ss.str (), ss2.str (), false,
                   edu::sharif::twinner::trace::cv::ConcreteValue64Bits
                   (UINT64 (argvis.at (i))), 0);
              delete mem;
              continue;
            }
          }
        }
        if (mem->getValue ().getSize () != 8) {
          continue;
        }
        for (int i = 0; i < argc; ++i) {
          const ADDRINT argvi0 = ADDRINT (argvis.at (i));
          const int argvilen = argvilens.at (i);
          if (argvi0 <= addr && addr <= argvi0 + argvilen) {
            const int offset = addr - argvi0;
            const char *argvicontent = argvicontents.at (i);
            std::stringstream ss, ss2;
            ss << "argv[" << i << "][" << offset << "]";
            ss2 << "n_v_argv_" << i << "_" << offset;
            token = new edu::sharif::twinner::trace::exptoken::NamedSymbol
                (ss.str (), ss2.str (), false,
                 edu::sharif::twinner::trace::cv::ConcreteValue8Bits
                 (UINT8 (argvicontent[offset])), 0);
            delete mem;
            break;
          }
        }
      }
    }
#elif defined(TARGET_IA32) && defined(REAL_TARGET_IS_WINDOWS)
    // In Windows 7, with ASLR disabled, argv is placed at 0x12ff4c
    // This code can be generalized by getting the argv's address
    // in addition to the argc and the argv values too.
    if (dynamic_cast<edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *> (token)) {
      edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *mem =
          static_cast<edu::sharif::twinner::trace::exptoken::MemoryEmergedSymbol *> (token);
      if (mem->getGenerationIndex () == 0) {
        const ADDRINT addr = mem->getAddress ();
        if (addr == 0x12ff4c) {
          token = new edu::sharif::twinner::trace::exptoken::NamedSymbol
              ("argv", "n_c_argv", true,
               edu::sharif::twinner::trace::cv::ConcreteValue64Bits
               (UINT64 (MarInfo::initialArgv)), 0);
          delete mem;
        } else {
          const UINT64 argv0 = UINT64 (argv);
          if (argv0 <= addr) {
            const UINT64 diff = addr - argv0;
            if (diff % 8 == 0) {
              const int i = diff / 8;
              if (i < argc) {
                std::stringstream ss, ss2;
                ss << "argv[" << i << "]";
                ss2 << "n_v_argv_" << i;
                token = new edu::sharif::twinner::trace::exptoken::NamedSymbol
                    (ss.str (), ss2.str (), false,
                     edu::sharif::twinner::trace::cv::ConcreteValue64Bits
                     (UINT64 (argvis.at (i))), 0);
                delete mem;
              }
            }
          }
        }
      }
    }
#else
#error "Unsupported architecture and/or OS"
#endif
  }
}

void MarInfo::saveToFile (const char *path) const {
  if (inspectionMode) {
    edu::sharif::twinner::util::Logger::error ()
        << "MarInfo::saveToFile (...):"
        " Method is not available in inspection mode\n";
    abort ();
  }
  std::ofstream out;
  out.open (path, ios_base::out | ios_base::trunc | ios_base::binary);
  if (!out.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "MarInfo::saveToFile (path=" << path << "): "
        << "Can not report main() args due to error in open function\n";
    abort ();
  }
  saveToBinaryStream (out);
  out.close ();
}

void MarInfo::saveToBinaryStream (std::ofstream &out) const {
  saveToOutputStream (out);
}

void MarInfo::saveToOutputStream (std::ostream &out) const {
  edu::sharif::twinner::util::Logger::loquacious ()
      << "argc=0x" << std::hex << argc << ", argv=0x" << ADDRINT (argv) << "\n";
  out.write (reinterpret_cast<const char *> (&argc), sizeof (argc));
  out.write (reinterpret_cast<const char *> (&argv), sizeof (argv));
  for (int i = 0; i < argc; ++i) {
    char *argvi = argvis.at (i);
    out.write (reinterpret_cast<const char *> (&argvi), sizeof (argvi));
    const int argvilen = argvilens.at (i);
    out.write (reinterpret_cast<const char *> (&argvilen), sizeof (argvilen));
    out.write (argvicontents.at (i), argvilen + 1);
  }
}

MarInfo *MarInfo::readMarInfoFromFile (std::string path) {
  std::ifstream in;
  in.open (path.c_str (), ios_base::in | ios_base::binary);
  if (!in.is_open ()) {
    edu::sharif::twinner::util::Logger::error ()
        << "MarInfo::readMarInfoFromFile (path=" << path << "): "
        << "Can not read mar info due to error in open function\n";
    abort ();
  } else {
    MarInfo *res = MarInfo::loadFromBinaryStream (in);
    in.close ();
    return res;
  }
}

void **MarInfo::getInitialArgv () {
  if (MarInfo::initialArgv == 0) {
    edu::sharif::twinner::util::Logger::error ()
        << "MarInfo::getInitialArgv (): "
        "The MarInfo::initialArgv is not initialized yet\n";
    abort ();
  }
  return reinterpret_cast<void **> (MarInfo::initialArgv);
}

MarInfo *MarInfo::loadFromBinaryStream (std::ifstream &in) {
  int argc;
  char **argv;
  in.read (reinterpret_cast<char *> (&argc), sizeof (argc));
  in.read (reinterpret_cast<char *> (&argv), sizeof (argv));
  std::vector<char *> argvis;
  std::vector<int> argvilens;
  std::vector<char *> argvicontents;
  for (int i = 0; i < argc; ++i) {
    char *argvi;
    in.read (reinterpret_cast<char *> (&argvi), sizeof (argvi));
    int argvilen;
    in.read (reinterpret_cast<char *> (&argvilen), sizeof (argvilen));
    char *argvicontent = new char[argvilen + 1];
    in.read (argvicontent, argvilen + 1);
    argvis.push_back (argvi);
    argvilens.push_back (argvilen);
    argvicontents.push_back (argvicontent);
  }
  return new MarInfo (argc, argv, argvis, argvilens, argvicontents);
}

}
}
}
}
