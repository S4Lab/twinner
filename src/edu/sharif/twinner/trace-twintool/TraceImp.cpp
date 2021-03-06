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

#include "TraceImp.h"

#include <stdexcept>
#include <set>
#include <fstream>
#include <sstream>

#include "edu/sharif/twinner/trace/ExpressionImp.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/MarInfo.h"

#include "edu/sharif/twinner/trace/exptoken/Symbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue8Bits.h"
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
  std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator
      = segments.end ();
  if (currentSegmentIterator != segments.begin ()) {
    currentSegmentIterator--;
  }
  current = TimedTrace (this, currentSegmentIterator);
  currentSegmentIndex = 0;
}

TraceImp::TraceImp (const TraceImp &trace) :
    Trace (trace) {
}

TraceImp::~TraceImp () {
}

TraceImp *TraceImp::clone () const {
  return new TraceImp (*this);
}

Expression *TraceImp::tryToGetSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return tryToGetSymbolicExpressionImplementation
      (size, reg, regval,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister, state,
       currentSegmentIterator);
}

Expression *TraceImp::tryToGetSymbolicExpressionByRegister (int size, REG reg,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return tryToGetSymbolicExpressionImplementation
      (size, reg, &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister,
       currentSegmentIterator);
}

Expression *TraceImp::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return tryToGetSymbolicExpressionImplementation
      (size, memoryEa, memval,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress,
       state,
       currentSegmentIterator);
}

Expression *TraceImp::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return tryToGetSymbolicExpressionImplementation
      (size, memoryEa,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress,
       currentSegmentIterator);
}

bool TraceImp::isSymbolicExpressionAvailableInRegister (int size, REG reg,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const {
  const ExecutionTraceSegment *seg = *currentSegmentIterator;
  return seg->isSymbolicExpressionAvailableInRegister (size, reg);
}

bool TraceImp::isSymbolicExpressionAvailableInMemoryAddress (int size,
    ADDRINT memoryEa,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) const {
  const ExecutionTraceSegment *seg = *currentSegmentIterator;
  return seg->isSymbolicExpressionAvailableInMemoryAddress (size, memoryEa);
}

template < typename T >
Expression *TraceImp::tryToGetSymbolicExpressionImplementation (int size, T address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method,
    StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    Expression *exp = (seg->*method) (size, address, val, state);
    if (exp) {
      if (it != currentSegmentIterator) {
        ExecutionTraceSegment *currentSegment = *currentSegmentIterator;
        addTemporaryExpressions (it, currentSegmentIterator, address, size);
        exp = (currentSegment->*method) (size, address, val, state);
      }
      return exp;
    }
    if (state.isWrongState ()) {
      if (it == currentSegmentIterator) {
        (*currentSegmentIterator)->printRegistersValues
            (edu::sharif::twinner::util::Logger::loquacious ());
        UnexpectedChange::adoptStateSummary (state, address);
      } else {
        state.setChangeAsExpected ();
      }
      break;
    }
  }
  return 0;
}

void TraceImp::addTemporaryExpressions (
    std::list < ExecutionTraceSegment * >::iterator past,
    std::list < ExecutionTraceSegment * >::iterator now,
    ADDRINT address, int size) {
  ExecutionTraceSegment *src = 0;
  for (std::list < ExecutionTraceSegment * >::iterator it = past;
      it != now;) {
    if (src == 0) {
      src = *it;
    }
    --it;
    ExecutionTraceSegment *dst = *it;
    dst->addTemporaryExpressions (src, address, size);
    src = dst;
  }
}

void TraceImp::addTemporaryExpressions (
    std::list < ExecutionTraceSegment * >::iterator past,
    std::list < ExecutionTraceSegment * >::iterator now,
    REG address, int size) {
  const REG fullReg = REG_FullRegName (address);
  if (fullReg != address) {
    size = REG_Size (fullReg) * 8;
  }
  ExecutionTraceSegment *src = 0;
  for (std::list < ExecutionTraceSegment * >::iterator it = past;
      it != now;) {
    if (src == 0) {
      src = *it;
    }
    --it;
    ExecutionTraceSegment *dst = *it;
    dst->addTemporaryExpressions (src, fullReg, size);
    src = dst;
  }
}

template < typename T >
Expression *TraceImp::tryToGetSymbolicExpressionImplementation (int size,
    T address,
    typename TryToGetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue method,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    Expression *exp = (seg->*method) (size, address);
    if (exp) {
      if (it != currentSegmentIterator) {
        ExecutionTraceSegment *currentSegment = *currentSegmentIterator;
        addTemporaryExpressions (it, currentSegmentIterator, address, size);
        exp = (currentSegment->*method) (size, address);
      }
      return exp;
    }
  }
  return 0;
}

Expression *TraceImp::getSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval,
    Expression *newExpression, StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return getSymbolicExpressionImplementation
      (size, reg, regval, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByRegister,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister,
       state,
       currentSegmentIterator);
}

Expression *TraceImp::getSymbolicExpressionByRegister (int size, REG reg,
    Expression *newExpression,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return getSymbolicExpressionImplementation
      (size, reg, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByRegister,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister,
       currentSegmentIterator);
}

Expression *TraceImp::getSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    const edu::sharif::twinner::trace::cv::ConcreteValue &memval,
    Expression *newExpression,
    StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return getSymbolicExpressionImplementation
      (size, memoryEa, memval, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByMemoryAddress,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress, state,
       currentSegmentIterator);
}

Expression *TraceImp::getSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa,
    Expression *newExpression,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  return getSymbolicExpressionImplementation
      (size, memoryEa, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByMemoryAddress,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress,
       currentSegmentIterator);
}

template < typename T >
Expression *TraceImp::getSymbolicExpressionImplementation (int size, T address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
    typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod,
    StateSummary &state,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  needsPropagation = false;
  Expression *exp = (this->*tryToGetMethod)
      (size, address, val, state, currentSegmentIterator);
  if (exp) { // exp exists and its val matches with expected value
    return exp;
  }
  if (state.isUnexpectedChangeState ()) {
    return 0;
  }
  // exp does not exist at all, so it's OK to create a new one
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
  return ((*currentSegmentIterator)->*getMethod)
      (size, address, val, newExpression, state);
}

template < typename T >
Expression *TraceImp::getSymbolicExpressionImplementation (int size, T address,
    Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >
    ::TraceTypeWithoutConcreteValue tryToGetMethod,
    typename GetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue getMethod,
    std::list < ExecutionTraceSegment * >::iterator currentSegmentIterator) {
  needsPropagation = false;
  Expression *exp = (this->*tryToGetMethod)
      (size, address, currentSegmentIterator);
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
  return ((*currentSegmentIterator)->*getMethod) (size, address, newExpression);
}

void TraceImp::loadInitializedSymbolsFromBinaryStream (std::stringstream &in) {
  std::map < int, std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord > >
      ::size_type s;
  in.read ((char *) &s, sizeof (s));
  int index = 0;
  if (s == 0) {
    addNewSegment (new ExecutionTraceSegment (index));
  }

  repeat (s) {
    int segmentIndex;
    in.read ((char *) &segmentIndex, sizeof (segmentIndex));
    ExecutionTraceSegment *segment =
        loadSingleSegmentSymbolsRecordsFromBinaryStream (segmentIndex, in);
    while (index < segmentIndex) {
      addNewSegment (new ExecutionTraceSegment (index++));
    }
    addNewSegment (segment);
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
      exp->setOverwriting (true);
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
            " from symbols binary stream (argv[i] case)\n";
        abort ();
      }
      break;
    }
    case edu::sharif::twinner::trace::exptoken::MAIN_ARGV_I_J_TYPE:
    {
      const UINT32 i = UINT32 (record.address);
      const UINT32 j = UINT32 (record.address >> 32);
      const ADDRINT memoryEa = ADDRINT (MarInfo::getInitialArgv () + i);
      UINT64 argvi;
#ifdef TARGET_IA32E
      if (!edu::sharif::twinner::util::readMemoryContent (argvi, memoryEa, 8)) {
#else
      if (!edu::sharif::twinner::util::readMemoryContent (argvi, memoryEa, 4)) {
#endif
        edu::sharif::twinner::util::Logger::error ()
            << "TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream"
            " (...): Cannot dereference argv[i] pointer (argv[i][j] case)\n";
        abort ();
      }
      const ADDRINT argvij = ADDRINT (argvi + j);
      exp = new ExpressionImp
          (argvij,
           edu::sharif::twinner::trace::cv::ConcreteValue8Bits
           (record.concreteValueLsb),
           index, true);
      std::pair < std::map < ADDRINT, Expression * >::iterator, bool > res =
          memMap.insert (make_pair (argvij, exp));
      if (!res.second) {
        edu::sharif::twinner::util::Logger::error ()
            << "TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream"
            " (...): Duplicate symbols are read for one memory address"
            " from symbols binary stream (argv[i][j] case)\n";
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
