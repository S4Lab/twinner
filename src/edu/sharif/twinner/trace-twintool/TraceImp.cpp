//                   In the name of GOD
/*
 * Twinner: An unpacker which utilizes concolic execution.
 * Copyright © 2013-2014  Behnam Momeni
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

#include "ExpressionImp.h"
#include "edu/sharif/twinner/trace/ExecutionTraceSegment.h"
#include "edu/sharif/twinner/trace/Symbol.h"
#include "edu/sharif/twinner/trace/ConcreteValue64Bits.h"
#include "edu/sharif/twinner/trace/ConcreteValue128Bits.h"

#include "edu/sharif/twinner/util/Logger.h"
#include "edu/sharif/twinner/util/iterationtools.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

TraceImp::TraceImp () :
Trace () {
}

TraceImp::TraceImp (std::ifstream &symbolsFileInputStream) :
Trace (1 /* Invoking dummy constructor of parent class to stop adding segments there*/) {
  loadInitializedSymbolsFromBinaryStream (symbolsFileInputStream);
  currentSegmentIterator = segments.end ();
  if (currentSegmentIterator != segments.begin ()) {
    currentSegmentIterator--;
  }
  currentSegmentIndex = 0;
}

TraceImp::~TraceImp () {
}

Expression *TraceImp::tryToGetSymbolicExpressionByRegister (REG reg,
    const ConcreteValue &regval) const throw (WrongStateException) {
  return tryToGetSymbolicExpressionImplementation
      (reg, regval, &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister);
}

Expression *TraceImp::tryToGetSymbolicExpressionByRegister (REG reg) const {
  return tryToGetSymbolicExpressionImplementation
      (reg, &ExecutionTraceSegment::tryToGetSymbolicExpressionByRegister);
}

Expression *TraceImp::tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const ConcreteValue &memval) const throw (WrongStateException) {
  return tryToGetSymbolicExpressionImplementation
      (memoryEa, memval,
       &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress);
}

Expression *TraceImp::tryToGetSymbolicExpressionByMemoryAddress (ADDRINT memoryEa) const {
  return tryToGetSymbolicExpressionImplementation
      (memoryEa, &ExecutionTraceSegment::tryToGetSymbolicExpressionByMemoryAddress);
}

void throw_exception_about_unexpected_change_in_memory_or_register_address
(REG reg, const ConcreteValue &expectedVal, const ConcreteValue &currentVal);
void throw_exception_about_unexpected_change_in_memory_or_register_address
(ADDRINT address, const ConcreteValue &expectedVal, const ConcreteValue &currentVal);

template < typename T >
Expression *TraceImp::tryToGetSymbolicExpressionImplementation (T address,
    const ConcreteValue &val,
    typename TryToGetSymbolicExpressionMethod < T >::TraceSegmentType method) const
throw (WrongStateException) {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    try {
      Expression *exp = (seg->*method) (address, val);
      if (exp) {
        return exp;
      }
    } catch (const WrongStateException &e) {
      if (it == currentSegmentIterator) {
        const ConcreteValue &currentValue = e.getCurrentStateValue ();
        getCurrentTraceSegment ()->printRegistersValues
            (edu::sharif::twinner::util::Logger::loquacious ());
        throw_exception_about_unexpected_change_in_memory_or_register_address
            (address, val, currentValue);
      } else {
        throw e;
      }
    }
  }
  return 0;
}

template < typename T >
Expression *TraceImp::tryToGetSymbolicExpressionImplementation (T address,
    typename TryToGetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue method) const {
  for (std::list < ExecutionTraceSegment * >::iterator it = currentSegmentIterator;
      it != segments.end (); ++it) {
    // searches segments starting from the current towards the oldest one
    ExecutionTraceSegment *seg = *it;
    Expression *exp = (seg->*method) (address);
    if (exp) {
      return exp;
    }
  }
  return 0;
}

Expression *TraceImp::getSymbolicExpressionByRegister (REG reg,
    const ConcreteValue &regval, Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (reg, regval, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByRegister,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister);
}

Expression *TraceImp::getSymbolicExpressionByRegister (REG reg,
    Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (reg, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByRegister,
       &ExecutionTraceSegment::getSymbolicExpressionByRegister);
}

Expression *TraceImp::getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    const ConcreteValue &memval, Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (memoryEa, memval, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByMemoryAddress,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress);
}

Expression *TraceImp::getSymbolicExpressionByMemoryAddress (ADDRINT memoryEa,
    Expression *newExpression) {
  return getSymbolicExpressionImplementation
      (memoryEa, newExpression,
       &TraceImp::tryToGetSymbolicExpressionByMemoryAddress,
       &ExecutionTraceSegment::getSymbolicExpressionByMemoryAddress);
}

template < typename T >
Expression *TraceImp::getSymbolicExpressionImplementation (T address,
    const ConcreteValue &val, Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >::TraceType tryToGetMethod,
    typename GetSymbolicExpressionMethod < T >::TraceSegmentType getMethod) {
  try {
    Expression *exp = (this->*tryToGetMethod) (address, val);
    if (exp) { // exp exists and its val matches with expected value
      return exp;
    } // exp does not exist at all, so it's OK to create a new one

  } catch (const WrongStateException &e) {
    const ConcreteValue &currentValue = e.getCurrentStateValue ();
    edu::sharif::twinner::util::Logger::debug () << "Unexpected value (0x"
        << std::hex << currentValue
        << ") was found (instead of 0x" << val << "). "
        "Probably, a new symbol is required to describe it.\n";
  }
  // instantiate and set a new expression in the current segment
  if (!newExpression) {
    newExpression = new ExpressionImp (address, val, currentSegmentIndex);
  }
  return (getCurrentTraceSegment ()->*getMethod) (address, val, newExpression);
}

template < typename T >
Expression *TraceImp::getSymbolicExpressionImplementation (T address,
    Expression *newExpression,
    typename TryToGetSymbolicExpressionMethod < T >
    ::TraceTypeWithoutConcreteValue tryToGetMethod,
    typename GetSymbolicExpressionMethod < T >::
    TraceSegmentTypeWithoutConcreteValue getMethod) {
  Expression *exp = (this->*tryToGetMethod) (address);
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
    newExpression = new ExpressionImp
        (address, ConcreteValue64Bits (0), currentSegmentIndex);
  }
  return (getCurrentTraceSegment ()->*getMethod) (address, newExpression);
}

void TraceImp::loadInitializedSymbolsFromBinaryStream (std::ifstream &in) {
  std::map < int, std::list < SymbolRecord > >::size_type s;
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

ExecutionTraceSegment *TraceImp::loadSingleSegmentSymbolsRecordsFromBinaryStream (int index,
    std::ifstream &in) {
  std::map < ADDRINT, Expression * > memMap;
  std::map < REG, Expression * > regMap;
  std::list < SymbolRecord >::size_type s;
  in.read ((char *) &s, sizeof (s));

  repeat (s) {
    SymbolRecord record;
    in.read ((char *) &record.address, sizeof (record.address));
    in.read ((char *) &record.type, sizeof (record.type));
    in.read ((char *) &record.concreteValueLsb, sizeof (record.concreteValueLsb));
    in.read ((char *) &record.concreteValueMsb, sizeof (record.concreteValueMsb));
    Expression *exp;
    switch (SymbolType (record.type)) {
    case REGISTER_64_BITS_SYMBOL_TYPE:
    case REGISTER_128_BITS_SYMBOL_TYPE:
    {
      if (record.type == REGISTER_64_BITS_SYMBOL_TYPE) {
        exp = new ExpressionImp
            (REG (record.address), ConcreteValue64Bits (record.concreteValueLsb), index);
      } else {
        exp = new ExpressionImp
            (REG (record.address),
             ConcreteValue128Bits (record.concreteValueMsb, record.concreteValueLsb),
             index);
      }
      std::pair < std::map < ADDRINT, Expression * >::iterator, bool > res =
          memMap.insert (make_pair (ADDRINT (record.address), exp));
      if (!res.second) {
        throw std::runtime_error ("Duplicate symbols are read for one memory address"
                                  " from symbols binary stream");
      }
      break;
    }
    case MEMORY_64_BITS_SYMBOL_TYPE:
    case MEMORY_128_BITS_SYMBOL_TYPE:
    {
      if (record.type == MEMORY_64_BITS_SYMBOL_TYPE) {
        exp = new ExpressionImp
            (ADDRINT (record.address), ConcreteValue64Bits (record.concreteValueLsb),
             index, true);
      } else {
        exp = new ExpressionImp
            (ADDRINT (record.address),
             ConcreteValue128Bits (record.concreteValueMsb, record.concreteValueLsb),
             index, true);
      }
      std::pair < std::map < REG, Expression * >::iterator, bool > res =
          regMap.insert (make_pair (REG (record.address), exp));
      if (!res.second) {
        throw std::runtime_error
            ("Duplicate symbols are read for one register from symbols binary stream");
      }
      break;
    }
    default:
      throw std::runtime_error ("Unsupported SymbolRecord type");
    }
  }
  return new ExecutionTraceSegment (regMap, memMap);
}

void throw_exception_about_unexpected_change_in_memory_or_register_address
(REG reg, const ConcreteValue &expectedVal, const ConcreteValue &currentVal) {
  std::string addr = REG_StringShort (reg);
  std::stringstream ss;
  ss << "Value of an address changed unexpectedly"
      " without any interfering syscall\n"
      "\tExpected " << expectedVal << ", Got " << currentVal
      << "; at register " << addr.c_str () << " (code: 0x" << (unsigned int) reg << ")";
  edu::sharif::twinner::util::Logger::error () << ss.str () << '\n';
  throw std::runtime_error (ss.str ());
}

void throw_exception_about_unexpected_change_in_memory_or_register_address
(ADDRINT address, const ConcreteValue &expectedVal, const ConcreteValue &currentVal) {
  std::stringstream ss;
  ss << "Value of an address changed unexpectedly"
      " without any interfering syscall\n"
      "\tExpected " << expectedVal << ", Got " << currentVal
      << "; at address 0x" << address;
  edu::sharif::twinner::util::Logger::error () << ss.str () << '\n';
  throw std::runtime_error (ss.str ());
}

}
}
}
}
