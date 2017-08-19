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

#include "FunctionEncoder.h"

#include <set>

#include "edu/sharif/twinner/util/Logger.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {
namespace {

int lastFunctionIndex = 0;
}

FunctionEncoder::FunctionEncoder (const AddrToSizeMap &addressToSize) :
    NodeEncoder (addressToSize),
    functionIndex (++lastFunctionIndex),
    firstVisit (true) {
}

FunctionEncoder::~FunctionEncoder () {
}

void FunctionEncoder::finalizeInitialization () {
  std::stringstream functionName;
  functionName << "func_" << functionIndex;
  std::stringstream typedArguments, argumentsFromMain, argumentsFromFunc;
  for (std::set< Variable >::const_iterator it = aggregatedVariables.begin ();
      it != aggregatedVariables.end (); ++it) {
    typedArguments << ", " << it->type << ' ' << it->technicalName;
    argumentsFromMain << ", " << VAR_TYPE " (" << it->name << ")";
    argumentsFromFunc << ", " << it->technicalName;
  }
  std::stringstream signature;
  signature << "int " << functionName.str ()
      << " (struct RegistersSet &regs" << typedArguments.str () << ")";
  functionSignatureLine = signature.str ();

  std::stringstream invocationFromMain;
  invocationFromMain << functionName.str ()
      << " (regs" << argumentsFromMain.str () << ")";
  functionInvocationLineFromMain = invocationFromMain.str ();

  std::stringstream invocationFromFunc;
  invocationFromMain << functionName.str ()
      << " (regs" << argumentsFromFunc.str () << ")";
  functionInvocationLineFromFunc = invocationFromFunc.str ();
}

void FunctionEncoder::encode (IndentedStream &body, IndentedStream &preamble,
    int index, bool inMain) {
  if (firstVisit) {
    firstVisit = false;

    preamble << functionSignatureLine << ";\n";

    IndentedStream funcBody;
    funcBody.incrementDepth ();
    NodeEncoder::encode (funcBody, preamble, index, false);

    preamble << functionSignatureLine << " {\n";
    preamble << funcBody.str ();
    preamble << "}\n";
  }
  if (inMain) {
    body.indented () << functionInvocationLineFromMain << ";\n";
  } else {
    body.indented () << functionInvocationLineFromFunc << ";\n";
  }
}


}
}
}
}
}
}
