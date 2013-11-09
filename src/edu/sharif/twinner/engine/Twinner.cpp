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

#include "Twinner.h"

#include <iostream>
#include <stdexcept>

#include "Executer.h"

#include "edu/sharif/twinner/trace/Trace.h"
#include "edu/sharif/twinner/trace/Symbol.h"

#include "edu/sharif/twinner/util/Logger.h"

using namespace std;

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {

Twinner::Twinner () {
}

void Twinner::setInputBinaryPath (string input) {
  this->input = input;
}

void Twinner::setTwinToolPath (string twintool) {
  this->twintool = twintool;
}

void Twinner::setPinLauncherPath (string pin) {
  this->pin = pin;
}

void Twinner::setTwinBinaryPath (string twin) {
  this->twin = twin;
}

void Twinner::generateTwinBinary () {
  Executer ex (pin, twintool, input);
  set < edu::sharif::twinner::trace::Symbol * > symbols;
  bool somePathsAreNotCovered = true;
  int i = 1;
  while (somePathsAreNotCovered) {
    edu::sharif::twinner::util::Logger::debug () << "Executing trace # " << i++ << '\n';
    ex.setSymbolsValues (symbols);

    edu::sharif::twinner::trace::Trace *trace = ex.executeSingleTrace ();
    addExecutionTrace (trace);

    for (set < edu::sharif::twinner::trace::Symbol * >::iterator it = symbols.begin ();
        it != symbols.end (); ++it) {
      delete *it;
    }
    symbols.clear ();
    somePathsAreNotCovered = calculateSymbolsValuesForCoveringNextPath (*trace, symbols);
  }
  codeTracesIntoTwinBinary ();
}

void Twinner::addExecutionTrace (const edu::sharif::twinner::trace::Trace *trace) {
  throw std::runtime_error ("Twinner::addExecutionTrace: Not yet implemented");
}

bool Twinner::calculateSymbolsValuesForCoveringNextPath (
    const edu::sharif::twinner::trace::Trace &trace,
    set < edu::sharif::twinner::trace::Symbol * > &symbols) {
  return false;
}

void Twinner::codeTracesIntoTwinBinary () {
  throw std::runtime_error ("Twinner::codeTracesIntoTwinBinary: Not yet implemented");
}

}
}
}
}
