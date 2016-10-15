//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2016 Behnam Momeni
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 * See the COPYING file distributed with this work for information
 * regarding copyright ownership.
 *
 * This file is part of Twinner project.
 */

#ifndef TWINNER_H
#define TWINNER_H

#include <string>
#include <set>
#include <map>
#include <list>

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Trace;
class Constraint;
class MarInfo;

namespace exptoken {

class Symbol;
}
}
namespace engine {
namespace etg {

class ConstraintTree;
}

class Executer;

class Twinner {
private:
  std::string etgpath;
  std::string input;
  std::string twintool;
  std::string pin;
  std::string twin;
  std::string arguments;
  std::string endpoints;
  bool newRecord;
  bool replayRecord;
  std::string safeFunctions;
  std::string tmpfolder;
  bool main; // just analyze after the main() function
  string stackOffset; // in number of arguments (not bytes)
  bool naive; // just print about visited instructions with no instrumentation
  bool overheads; // run twintool twice for each trace and measure CPU/Memory overheads

  std::list < const edu::sharif::twinner::trace::Trace * > traces;
  edu::sharif::twinner::engine::etg::ConstraintTree *ctree;

public:
  Twinner ();
  ~Twinner ();

  void setEtgPath (std::string etgpath);
  void setInputBinaryPath (std::string input);
  void setTwinToolPath (std::string twintool);
  void setPinLauncherPath (std::string pin);
  void setTwinBinaryPath (std::string twin);
  void setInputBinaryArguments (std::string arguments);
  void setAnalysisEndpoints (std::string endpoints);
  void setRecord (bool newRecord, bool replayRecord);
  void setSafeFunctions (std::string safeFunctions);
  void setTempFolder (std::string tmpfolder);
  void setJustAnalyzeMainRoutine (bool main);
  void setStackOffset (std::string stackOffset);
  void setNaiveMode (bool naive);
  void setMeasureOverheads (bool measureOverheads);

  bool generateTwinBinary ();

private:
  void addExecutionTrace (edu::sharif::twinner::trace::Trace *trace,
      edu::sharif::twinner::trace::MarInfo *marInfo);
  bool isTraceConsistent (edu::sharif::twinner::trace::Trace *trace) const;
  void addExecutionTrace (edu::sharif::twinner::trace::Trace *trace);

  bool calculateSymbolsValuesForCoveringNextPath (
      std::set < const edu::sharif::twinner::trace::exptoken::Symbol * > &symbols);

  std::map < std::pair < ADDRINT, int >, UINT64 >
  obtainInitializedMemoryValues (Executer &ex) const;
  void codeTracesIntoTwinCode (
      const std::map < std::pair < ADDRINT, int >, UINT64 > &initialValues);
  void codeInitialValuesIntoTwinCode (std::stringstream &out,
      const std::map < std::pair < ADDRINT, int >, UINT64 > &initialValues) const;

  void generateEtg (std::ofstream &out) const;
};

}
}
}
}

#endif /* Twinner.h */
