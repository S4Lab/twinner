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

#include "TraceImp.h"

#include <stdexcept>
#include <set>
#include <fstream>
#include <sstream>

#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/MarInfo.h"

#include "edu/sharif/twinner/trace/exptoken/Symbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue32Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

#include "UnexpectedChange.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

TraceImp::TraceImp () :
    Trace () {
}

TraceImp::TraceImp (std::stringstream &symbolsInputStream,
    edu::sharif::twinner::util::MemoryManager *_memoryManager) :
    Trace (1 /* Invoking dummy constructor of parent class to stop adding segments there*/) {
  memoryManager = _memoryManager;
  loadInitializedSymbolsFromBinaryStream (symbolsInputStream);
  currentSegmentIterator = segments.end ();
  if (currentSegmentIterator != segments.begin ()) {
    currentSegmentIterator--;
  }
  currentSegmentIndex = 0;
}

TraceImp::~TraceImp () {
}

Expression *TraceImp::tryToGetSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state) {
  return tryToGetSymbolicExpressionImplementation
      (size, reg, regval,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister, state);
}

Expression *TraceImp::tryToGetSymbolicExpressionByRegister (int size, REG reg) {
  return tryToGetSymbolicExpressionImplementation
      (size, reg, &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister);
}

Expression *TraceImp::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    StateSummary &state) {
  return tryToGetSymbolicExpressionImplementation
      (size, memoryEa, memval,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress,
       state);
}

Expression *TraceImp::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa) {
  return tryToGetSymbolicExpressionImplementation
      (size, memoryEa, &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress);
}

template < typename T >
Expression *TraceImp::tryToGetSymbolicExpressionImplementation (int size, T address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method,
    StateSummary &state) {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    Expression *exp = (seg->*method) (size, address, val, state);
    if (exp) {
      return exp;
    }
    if (state.isWrongState ()) {
      if (it == currentSegmentIterator) {
        getCurrentTraceSegment ()->printRegistersValues
            (edu::sharif::twinner::util::Logger::loquacious ());
        UnexpectedChange::adoptStateSummary (state, address);
      }
      break;
    }
  }
  return 0;
}

template < typename T >
Expression *TraceImp::tryToGetSymbolicExpressionImplementation (int size, T address,
    typename TryToGetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue method) {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    Expression *exp = (seg->*method) (size, address);
    if (exp) {
      return exp;
    }
  }
  return 0;
}

Expression *TraceImp::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state) {
  return getSymbolicExpressionImplementation
      (size, reg, regval, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByRegister,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister, state);
}

Expression *TraceImp::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (size, reg, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByRegister,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister);
}

Expression *TraceImp::getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    Expression *newExpression, StateSummary &state) {
  return getSymbolicExpressionImplementation
      (size, memoryEa, memval, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByMemoryAddress,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress, state);
}

Expression *TraceImp::getSymbolicExpressionByMemoryAddress (int size, ADDRINT memoryEa,
    Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (size, memoryEa, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByMemoryAddress,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress);
}

template < typename T >
Expression *TraceImp::getSymbolicExpressionImplementation (int size, T address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &val, Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
    typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod,
    StateSummary &state) {
  needsPropagation = false;
  Expression *exp = (this->*tryToGetMethod) (size, address, val, state);
  if (exp) { // exp exists and its val matches with expected value
    return exp;
  } // exp does not exist at all, so it's OK to create a new one

  if (state.isUnexpectedChangeState ()) {
    return 0;
  }
  if (state.isWrongState ()) {
    const edu::sharif::twinner::trace::cv::ConcreteValue &currentValue =
        state.getCurrentStateValue ();
    edu::sharif::twinner::util::Logger::loquacious () << "Unexpected value ("
        << std::hex << currentValue
        << ") was found (instead of " << val << "). "
        "A new symbol is required to describe it.\n";
    state.clear ();
  }
  // instantiate and set a new expression in the current segment
  if (!newExpression) {
    newExpression = instantiateExpression (address, val, currentSegmentIndex);
  }
  needsPropagation = true;
  return (getCurrentTraceSegment ()->*getMethod) (size, address, val,
                                                  newExpression, state);
}

template < typename T >
Expression *TraceImp::getSymbolicExpressionImplementation (int size, T address,
    Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >
    ::TraceTypeWithoutConcreteValue tryToGetMethod,
    typename GetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue getMethod) {
  needsPropagation = false;
  Expression *exp = (this->*tryToGetMethod) (size, address);
  if (exp) {
    return exp;
  }
  // instantiate and set a new expression in the current segment
  if (!newExpression) {
    /*
     * This getter, which ignores concrete value, is only called when the returned
     * expression is going to be set immediately. So previous value was random and
     * not significant. Thus we can use 0 as concrete value of instantiated expression.
     *
     * Callers who care about concrete value of the created expression, are not allowed
     * to pass null for the newExpression argument.
     */
    newExpression = instantiateExpression
        (address, edu::sharif::twinner::trace::cv::ConcreteValue64Bits (0),
         currentSegmentIndex);
  }
  needsPropagation = true;
  return (getCurrentTraceSegment ()->*getMethod) (size, address, newExpression);
}

void TraceImp::loadInitializedSymbolsFromBinaryStream (std::stringstream &in) {
  std::map < int, std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord > >
      ::size_type s;
  in.read ((char *) &s, sizeof (s));
  int index = 0;
  if (s == 0) {
    segments.push_front (new ExecutionTraceSegment (index));
  }

  repeat (s) {
    int segmentIndex;
    in.read ((char *) &segmentIndex, sizeof (segmentIndex));
    ExecutionTraceSegment *segment =
        loadSingleSegmentSymbolsRecordsFromBinaryStream (segmentIndex, in);
    while (index < segmentIndex) {
      segments.push_front (new ExecutionTraceSegment (index++));
    }
    segments.push_front (segment);
    ++index;
  }
}

ExecutionTraceSegment *TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream (
    int index, std::stringstream &in) {
  std::map < ADDRINT, Expression * > memMap;
  std::map < REG, Expression * > regMap;
  std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord >::size_type s;
  in.read ((char *) &s, sizeof (s));

  repeat (s) {
    edu::sharif::twinner::trace::exptoken::SymbolRecord record;
    in.read ((char *) &record.address, sizeof (record.address));
    in.read ((char *) &record.type, sizeof (record.type));
    in.read ((char *) &record.concreteValueLsb, sizeof (record.concreteValueLsb));
    in.read ((char *) &record.concreteValueMsb, sizeof (record.concreteValueMsb));
    Expression *exp = 0;
    int size = 0;
    switch (edu::sharif::twinner::trace::exptoken::SymbolType (record.type)) {
#ifdef TARGET_IA32E
    case edu::sharif::twinner::trace::exptoken::REGISTER_64_BITS_SYMBOL_TYPE:
      exp = instantiateExpression
          (REG (record.address),
           edu::sharif::twinner::trace::cv::ConcreteValue64Bits
           (record.concreteValueLsb), index);
#else
    case edu::sharif::twinner::trace::exptoken::REGISTER_32_BITS_SYMBOL_TYPE:
      exp = instantiateExpression
          (REG (record.address),
           edu::sharif::twinner::trace::cv::ConcreteValue32Bits
           (record.concreteValueLsb), index);
#endif
    case edu::sharif::twinner::trace::exptoken::REGISTER_128_BITS_SYMBOL_TYPE:
    {
      if (exp == 0) {
        exp = instantiateExpression (REG (record.address),
                                     edu::sharif::twinner::trace::cv::ConcreteValue128Bits
                                     (record.concreteValueMsb, record.concreteValueLsb),
                                     index);
      }
      edu::sharif::twinner::util::Logger::loquacious () << "loading symbol: "
          << REG (record.address) << " -> " << exp << '\n';
      std::pair < std::map < REG, Expression * >::iterator, bool > res =
          regMap.insert (make_pair (REG (record.address), exp));
      if (!res.second) {
        edu::sharif::twinner::util::Logger::error ()
            << "TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream"
            " (...): Duplicate symbols are read for one register"
            " from symbols binary stream\n";
        abort ();
      }
      break;
    }
    case edu::sharif::twinner::trace::exptoken::MEMORY_8_BITS_SYMBOL_TYPE:
      if (size == 0) {
        size = 8;
      }
    case edu::sharif::twinner::trace::exptoken::MEMORY_16_BITS_SYMBOL_TYPE:
      if (size == 0) {
        size = 16;
      }
    case edu::sharif::twinner::trace::exptoken::MEMORY_32_BITS_SYMBOL_TYPE:
      if (size == 0) {
        size = 32;
      }
    case edu::sharif::twinner::trace::exptoken::MEMORY_64_BITS_SYMBOL_TYPE:
      if (size == 0) {
        size = 64;
      }
    {
      const edu::sharif::twinner::trace::cv::ConcreteValue *cv =
          edu::sharif::twinner::trace::cv::ConcreteValue64Bits
          (record.concreteValueLsb).clone (size);
      exp = new ExpressionImp (ADDRINT (record.address), *cv, index, true);
      delete cv;
    }
    case edu::sharif::twinner::trace::exptoken::MEMORY_128_BITS_SYMBOL_TYPE:
    {
      if (exp == 0) {
        edu::sharif::twinner::util::Logger::warning () << "A 128-bits memory symbol "
            "is found in the symbols file, while only 64-bits memory symbols are "
            "supposed to be transferred between Twinner and TwinTool.\n";
        exp = new ExpressionImp (ADDRINT (record.address),
                                 edu::sharif::twinner::trace::cv::ConcreteValue128Bits
                                 (record.concreteValueMsb, record.concreteValueLsb),
                                 index, true);
      }
      edu::sharif::twinner::util::Logger::loquacious () << "loading symbol: 0x"
          << std::hex << ADDRINT (record.address) << " -> " << exp << '\n';
      std::pair < std::map < ADDRINT, Expression * >::iterator, bool > res =
          memMap.insert (make_pair (ADDRINT (record.address), exp));
      if (!res.second) {
        edu::sharif::twinner::util::Logger::error ()
            << "TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream"
            " (...): Duplicate symbols are read for one memory address"
            " from symbols binary stream\n";
        abort ();
      }
      break;
    }
    case edu::sharif::twinner::trace::exptoken::MAIN_ARGV_I_TYPE:
    {
      const UINT32 i = UINT32 (record.address);
      const ADDRINT memoryEa = ADDRINT (MarInfo::getInitialArgv () + i);
      exp = new ExpressionImp
          (memoryEa,
           edu::sharif::twinner::trace::cv::ConcreteValue64Bits
           (record.concreteValueLsb),
           index, true);
      edu::sharif::twinner::util::Logger::loquacious () << "loading symbol "
          "argv[" << i << "] == 0x" << std::hex << memoryEa
          << " -> " << exp << '\n';
      std::pair < std::map < ADDRINT, Expression * >::iterator, bool > res =
          memMap.insert (make_pair (memoryEa, exp));
      if (!res.second) {
        edu::sharif::twinner::util::Logger::error ()
            << "TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream"
            " (...): Duplicate symbols are read for one memory address"
            " from symbols binary stream [argv[i] case)\n";
        abort ();
      }
      break;
    }
    default:
      edu::sharif::twinner::util::Logger::error ()
          << "TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream"
          " (...): Unsupported SymbolRecord type: "
          << std::dec << record.type << '\n';
      abort ();
    }
  }
  return new ExecutionTraceSegment (index, regMap, memMap);
}

Expression *TraceImp::instantiateExpression (REG address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &value, int index) {
  const REG enclosingReg = REG_FullRegName (address);
  Expression *exp = new ExpressionImp (enclosingReg, value, index);
  if (enclosingReg != address) {
    exp->truncate (REG_Size (address) * 8);
  }
  return exp;
}

Expression *TraceImp::instantiateExpression (ADDRINT address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &value,
    int index) {
  return new ExpressionImp (address, value, index);
}

}
}
}
}
