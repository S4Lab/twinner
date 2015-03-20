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

#include "edu/sharif/twinner/trace/exptoken/Symbol.h"

#include "edu/sharif/twinner/trace/cv/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/cv/ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"
#include "edu/sharif/twinner/util/MemoryManager.h"

#include "UnexpectedChangeException.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

TraceImp::TraceImp () :
    Trace () {
}

TraceImp::TraceImp (std::ifstream &symbolsFileInputStream) :
    Trace (1 /* Invoking dummy constructor of parent class to stop adding segments there*/) {
  memoryManager = edu::sharif::twinner::util::MemoryManager::allocateInstance ();
  loadInitializedSymbolsFromBinaryStream (symbolsFileInputStream);
  currentSegmentIterator = segments.end ();
  if (currentSegmentIterator != segments.begin ()) {
    currentSegmentIterator--;
  }
  currentSegmentIndex = 0;
}

TraceImp::~TraceImp () {
}

Expression *TraceImp::tryToGetSymbolicExpressionByRegister (int size, REG reg,
    const edu::sharif::twinner::trace::cv::ConcreteValue &regval) const
/* @throw (WrongStateException) */ {
  return tryToGetSymbolicExpressionImplementation
      (size, reg, regval, &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister);
}

Expression *TraceImp::tryToGetSymbolicExpressionByRegister (int size, REG reg) const {
  return tryToGetSymbolicExpressionImplementation
      (size, reg, &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister);
}

Expression *TraceImp::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa, const edu::sharif::twinner::trace::cv::ConcreteValue &memval) const
/* throw (WrongStateException) */ {
  return tryToGetSymbolicExpressionImplementation
      (size, memoryEa, memval,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress);
}

Expression *TraceImp::tryToGetSymbolicExpressionByMemoryAddress (int size,
    ADDRINT memoryEa) const {
  return tryToGetSymbolicExpressionImplementation
      (size, memoryEa, &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress);
}

template < typename T >
Expression *TraceImp::tryToGetSymbolicExpressionImplementation (int size, T address,
    const edu::sharif::twinner::trace::cv::ConcreteValue &val,
    typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method) const
/* @throw (WrongStateException) */ {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    try {
      Expression *exp = (seg->*method) (size, address, val);
      if (exp) {
        return exp;
      }
    } catch (const WrongStateException &e) {
      if (it == currentSegmentIterator) {
        const edu::sharif::twinner::trace::cv::ConcreteValue &currentValue =
            e.getCurrentStateValue ();
        getCurrentTraceSegment ()->printRegistersValues
            (edu::sharif::twinner::util::Logger::loquacious ());
        throw UnexpectedChangeException (address, val, currentValue);
      } else {
        throw;
      }
    }
  }
  return 0;
}

template < typename T >
Expression *TraceImp::tryToGetSymbolicExpressionImplementation (int size, T address,
    typename TryToGetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue method) const {
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
    Expression *newExpression) /* @throw (UnexpectedChangeException) */ {
  return getSymbolicExpressionImplementation
      (size, reg, regval, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByRegister,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister);
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
    Expression *newExpression) /* @throw (UnexpectedChangeException) */ {
  return getSymbolicExpressionImplementation
      (size, memoryEa, memval, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByMemoryAddress,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress);
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
    typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod)
/* @throw (UnexpectedChangeException) */ {
  try {
    Expression *exp = (this->*tryToGetMethod) (size, address, val);
    if (exp) { // exp exists and its val matches with expected value
      return exp;
    } // exp does not exist at all, so it's OK to create a new one

  } catch (const UnexpectedChangeException &e) {
    throw;

  } catch (const WrongStateException &e) {
    const edu::sharif::twinner::trace::cv::ConcreteValue &currentValue =
        e.getCurrentStateValue ();
    edu::sharif::twinner::util::Logger::loquacious () << "Unexpected value ("
        << std::hex << currentValue
        << ") was found (instead of " << val << "). "
        "Probably, a new symbol is required to describe it.\n";
  }
  // instantiate and set a new expression in the current segment
  if (!newExpression) {
    newExpression = instantiateExpression (address, val, currentSegmentIndex);
  }
  return (getCurrentTraceSegment ()->*getMethod) (size, address, val, newExpression);
}

template < typename T >
Expression *TraceImp::getSymbolicExpressionImplementation (int size, T address,
    Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >
    ::TraceTypeWithoutConcreteValue tryToGetMethod,
    typename GetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue getMethod) {
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
  return (getCurrentTraceSegment ()->*getMethod) (size, address, newExpression);
}

void TraceImp::loadInitializedSymbolsFromBinaryStream (std::ifstream &in) {
  std::map < int, std::list < edu::sharif::twinner::trace::exptoken::SymbolRecord > >
      ::size_type s;
  in.read ((char *) &s, sizeof (s));
  int index = 0;
  if (s == 0) {
    segments.push_front (new ExecutionTraceSegment ());
  }

  repeat (s) {
    int segmentIndex;
    in.read ((char *) &segmentIndex, sizeof (segmentIndex));
    ExecutionTraceSegment *segment =
        loadSingleSegmentSymbolsRecordsFromBinaryStream (segmentIndex, in);
    while (index++ < segmentIndex) {
      segments.push_front (new ExecutionTraceSegment ());
    }
    segments.push_front (segment);
  }
}

ExecutionTraceSegment *TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream (
    int index, std::ifstream &in) {
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
    Expression *exp;
    switch (edu::sharif::twinner::trace::exptoken::SymbolType (record.type)) {
    case edu::sharif::twinner::trace::exptoken::REGISTER_64_BITS_SYMBOL_TYPE:
    case edu::sharif::twinner::trace::exptoken::REGISTER_128_BITS_SYMBOL_TYPE:
    {
      if (record.type ==
          edu::sharif::twinner::trace::exptoken::REGISTER_64_BITS_SYMBOL_TYPE) {
        exp = instantiateExpression (REG (record.address),
                                     edu::sharif::twinner::trace::cv::ConcreteValue64Bits
                                     (record.concreteValueLsb), index);
        edu::sharif::twinner::util::Logger::loquacious () << "loading symbol: "
            << REG (record.address) << " -> " << exp << '\n';
      } else {
        exp = instantiateExpression (REG (record.address),
                                     edu::sharif::twinner::trace::cv::ConcreteValue128Bits
                                     (record.concreteValueMsb, record.concreteValueLsb),
                                     index);
        edu::sharif::twinner::util::Logger::loquacious () << "loading symbol: "
            << REG (record.address) << " -> " << exp << '\n';
      }
      std::pair < std::map < REG, Expression * >::iterator, bool > res =
          regMap.insert (make_pair (REG (record.address), exp));
      if (!res.second) {
        throw std::runtime_error
            ("Duplicate symbols are read for one register from symbols binary stream");
      }
      break;
    }
    case edu::sharif::twinner::trace::exptoken::MEMORY_64_BITS_SYMBOL_TYPE:
    case edu::sharif::twinner::trace::exptoken::MEMORY_128_BITS_SYMBOL_TYPE:
    {
      if (record.type ==
          edu::sharif::twinner::trace::exptoken::MEMORY_64_BITS_SYMBOL_TYPE) {
        exp = new ExpressionImp (ADDRINT (record.address),
                                 edu::sharif::twinner::trace::cv::ConcreteValue64Bits
                                 (record.concreteValueLsb),
                                 index, true);
        edu::sharif::twinner::util::Logger::loquacious () << "loading symbol: 0x"
            << std::hex << ADDRINT (record.address) << " -> " << exp << '\n';
      } else {
        edu::sharif::twinner::util::Logger::warning () << "A 128-bits memory symbol "
            "is found in the symbols file, while only 64-bits memory symbols are "
            "supposed to be transferred between Twinner and TwinTool.\n";
        exp = new ExpressionImp (ADDRINT (record.address),
                                 edu::sharif::twinner::trace::cv::ConcreteValue128Bits
                                 (record.concreteValueMsb, record.concreteValueLsb),
                                 index, true);
      }
      std::pair < std::map < ADDRINT, Expression * >::iterator, bool > res =
          memMap.insert (make_pair (ADDRINT (record.address), exp));
      if (!res.second) {
        throw std::runtime_error ("Duplicate symbols are read for one memory address"
                                  " from symbols binary stream");
      }
      break;
    }
    default:
      throw std::runtime_error ("Unsupported SymbolRecord type");
    }
  }
  return new ExecutionTraceSegment (regMap, memMap);
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
