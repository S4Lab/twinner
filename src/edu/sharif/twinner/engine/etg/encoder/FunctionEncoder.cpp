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
  std::stringstream typedArguments;
  for (std::set< Variable >::const_iterator it = aggregatedVariables.begin ();
      it != aggregatedVariables.end (); ++it) {
    typedArguments << ", " << it->type << ' ' << it->technicalName;
  }
  std::stringstream signature;
  signature << "int " << functionName.str ()
      << " (struct RegistersSet &regs" << typedArguments.str () << ")";
  functionSignatureLine = signature.str ();
}

void FunctionEncoder::encode (IndentedStream &body, IndentedStream &preamble,
    int index, bool inMain, const VariableContainer &vc) {
  if (firstVisit) {
    firstVisit = false;

    preamble << functionSignatureLine << ";\n";

    IndentedStream funcBody;
    funcBody.incrementDepth ();
    VariableContainer subVc (aggregatedVariables);
    NodeEncoder::encode (funcBody, preamble, index, false, subVc);

    preamble << functionSignatureLine << " {\n";
    preamble << funcBody.str ();
    preamble << "}\n";
  }
  // TODO: encode untouched variables
  std::stringstream functionName;
  functionName << "func_" << functionIndex;
  std::stringstream arguments;
  std::stringstream definedVars;
  if (inMain) {
    for (std::set< Variable >::const_iterator it = aggregatedVariables.begin ();
        it != aggregatedVariables.end (); ++it) {
      bool shouldDefine;
      arguments << ", " << VAR_TYPE " ("
          << vc.provideVariable (*it, shouldDefine).name << ")";
      if (shouldDefine) {
        const std::string hexAddress = it->name.substr (1, it->name.find ("_") - 1);
        definedVars << "const " << it->type << " " << it->name
            << " = *((" << it->type << " *) 0x" << hexAddress << ");\n";
      }
    }
  } else {
    for (std::set< Variable >::const_iterator it = aggregatedVariables.begin ();
        it != aggregatedVariables.end (); ++it) {
      bool shouldDefine;
      arguments << ", " << vc.provideVariable (*it, shouldDefine).technicalName;
      if (shouldDefine) {
        const std::string hexAddress = it->name.substr (1, it->name.find ("_") - 1);
        definedVars << "const " << it->type << " " << it->name
            << " = *((" << it->type << " *) 0x" << hexAddress << ");\n";
      }
    }
  }
  std::stringstream invocation;
  invocation << functionName.str () << " (regs" << arguments.str () << ");\n";

  body.indented () << definedVars.str () << invocation.str ();
}


}
}
}
}
}
}
