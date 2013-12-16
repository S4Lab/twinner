/*********************                                                        */
/*! \file logic_info.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: none
 ** Minor contributors (to current version): Dejan Jovanovic
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A class giving information about a logic (group a theory modules
 ** and configuration information)
 **
 ** A class giving information about a logic (group of theory modules and
 ** configuration information).
 **/

#include "cvc4_public.h"

#ifndef __CVC4__LOGIC_INFO_H
#define __CVC4__LOGIC_INFO_H

#include <string>
#include <vector>
#include "expr/kind.h"

namespace CVC4 {

/**
 * A LogicInfo instance describes a collection of theory modules and some
 * basic configuration about them.  Conceptually, it provides a background
 * context for all operations in CVC4.  Typically, when CVC4's SmtEngine
 * is created, it is issued a setLogic() command indicating features of the
 * assertions and queries to follow---for example, whether quantifiers are
 * used, whether integers or reals (or both) will be used, etc.
 *
 * Most places in CVC4 will only ever need to access a const reference to an
 * instance of this class.  Such an instance is generally set by the SmtEngine
 * when setLogic() is called.  However, mutating member functions are also
 * provided by this class so that it can be used as a more general mechanism
 * (e.g., for communicating to the SmtEngine which theories should be used,
 * rather than having to provide an SMT-LIB string).
 */
class CVC4_PUBLIC LogicInfo {
  mutable std::string d_logicString; /**< an SMT-LIB-like logic string */
  std::vector<bool> d_theories; /**< set of active theories */
  size_t d_sharingTheories; /**< count of theories that need sharing */

  // for arithmetic
  bool d_integers; /**< are integers used in this logic? */
  bool d_reals; /**< are reals used in this logic? */
  bool d_linear; /**< linear-only arithmetic in this logic? */
  bool d_differenceLogic; /**< difference-only arithmetic in this logic? */

  bool d_locked; /**< is this LogicInfo instance locked (and thus immutable)? */

  /**
   * Returns true iff this is a "true" theory (one that must be worried
   * about for sharing
   */
  static inline bool isTrueTheory(theory::TheoryId theory) {
    switch(theory) {
    case theory::THEORY_BUILTIN:
    case theory::THEORY_BOOL:
    case theory::THEORY_QUANTIFIERS:
    case theory::THEORY_REWRITERULES:
      return false;
    default:
      return true;
    }
  }

public:

  /**
   * Constructs a LogicInfo for the most general logic (quantifiers, all
   * background theory modules, ...).
   */
  LogicInfo();

  /**
   * Construct a LogicInfo from an SMT-LIB-like logic string.
   * Throws an IllegalArgumentException if the logic string cannot
   * be interpreted.
   */
  LogicInfo(std::string logicString) throw(IllegalArgumentException);

  /**
   * Construct a LogicInfo from an SMT-LIB-like logic string.
   * Throws an IllegalArgumentException if the logic string cannot
   * be interpreted.
   */
  LogicInfo(const char* logicString) throw(IllegalArgumentException);

  // ACCESSORS

  /**
   * Get an SMT-LIB-like logic string.  These are only guaranteed to
   * be SMT-LIB-compliant if an SMT-LIB-compliant string was used in
   * the constructor and no mutating functions were called.
   */
  std::string getLogicString() const;

  /** Is sharing enabled for this logic? */
  bool isSharingEnabled() const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    return d_sharingTheories > 1;
  }

  /** Is the given theory module active in this logic? */
  bool isTheoryEnabled(theory::TheoryId theory) const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    return d_theories[theory];
  }

  /** Is this a quantified logic? */
  bool isQuantified() const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    return isTheoryEnabled(theory::THEORY_QUANTIFIERS) || isTheoryEnabled(theory::THEORY_REWRITERULES);
  }

  /** Is this the all-inclusive logic? */
  bool hasEverything() const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    LogicInfo everything;
    everything.lock();
    return *this == everything;
  }

  /** Is this the all-exclusive logic?  (Here, that means propositional logic) */
  bool hasNothing() const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    LogicInfo nothing("");
    nothing.lock();
    return *this == nothing;
  }

  /**
   * Is this a pure logic (only one "true" background theory).  Quantifiers
   * can exist in such logics though; to test for quantifier-free purity,
   * use "isPure(theory) && !isQuantified()".
   */
  bool isPure(theory::TheoryId theory) const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    // the third and fourth conjucts are really just to rule out the misleading
    // case where you ask isPure(THEORY_BOOL) and get true even in e.g. QF_LIA
    return isTheoryEnabled(theory) && !isSharingEnabled() &&
      ( !isTrueTheory(theory) || d_sharingTheories == 1 ) &&
      ( isTrueTheory(theory) || d_sharingTheories == 0 );
  }

  // these are for arithmetic

  /** Are integers in this logic? */
  bool areIntegersUsed() const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    CheckArgument(isTheoryEnabled(theory::THEORY_ARITH), *this, "Arithmetic not used in this LogicInfo; cannot ask whether integers are used");
    return d_integers;
  }
  /** Are reals in this logic? */
  bool areRealsUsed() const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    CheckArgument(isTheoryEnabled(theory::THEORY_ARITH), *this, "Arithmetic not used in this LogicInfo; cannot ask whether reals are used");
    return d_reals;
  }
  /** Does this logic only linear arithmetic? */
  bool isLinear() const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    CheckArgument(isTheoryEnabled(theory::THEORY_ARITH), *this, "Arithmetic not used in this LogicInfo; cannot ask whether it's linear");
    return d_linear || d_differenceLogic;
  }
  /** Does this logic only permit difference reasoning? (implies linear) */
  bool isDifferenceLogic() const {
    CheckArgument(d_locked, *this, "This LogicInfo isn't locked yet, and cannot be queried");
    CheckArgument(isTheoryEnabled(theory::THEORY_ARITH), *this, "Arithmetic not used in this LogicInfo; cannot ask whether it's difference logic");
    return d_differenceLogic;
  }

  // MUTATORS

  /**
   * Initialize the LogicInfo with an SMT-LIB-like logic string.
   * Throws an IllegalArgumentException if the string can't be
   * interpreted.
   */
  void setLogicString(std::string logicString) throw(IllegalArgumentException);

  /**
   * Enable all functionality.  All theories, plus quantifiers, will be
   * enabled.
   */
  void enableEverything();

  /**
   * Disable all functionality.  The result will be a LogicInfo with
   * the BUILTIN and BOOLEAN theories enabled only ("QF_SAT").
   */
  void disableEverything();

  /**
   * Enable the given theory module.
   */
  void enableTheory(theory::TheoryId theory);

  /**
   * Disable the given theory module.  THEORY_BUILTIN and THEORY_BOOL cannot
   * be disabled (and if given here, the request will be silently ignored).
   */
  void disableTheory(theory::TheoryId theory);

  /**
   * Quantifiers are a special case, since two theory modules handle them.
   */
  void enableQuantifiers() {
    enableTheory(theory::THEORY_QUANTIFIERS);
    enableTheory(theory::THEORY_REWRITERULES);
  }

  /**
   * Quantifiers are a special case, since two theory modules handle them.
   */
  void disableQuantifiers() {
    disableTheory(theory::THEORY_QUANTIFIERS);
    disableTheory(theory::THEORY_REWRITERULES);
  }

  // these are for arithmetic

  /** Enable the use of integers in this logic. */
  void enableIntegers();
  /** Disable the use of integers in this logic. */
  void disableIntegers();
  /** Enable the use of reals in this logic. */
  void enableReals();
  /** Disable the use of reals in this logic. */
  void disableReals();
  /** Only permit difference arithmetic in this logic. */
  void arithOnlyDifference();
  /** Only permit linear arithmetic in this logic. */
  void arithOnlyLinear();
  /** Permit nonlinear arithmetic in this logic. */
  void arithNonLinear();

  // LOCKING FUNCTIONALITY

  /** Lock this LogicInfo, disabling further mutation and allowing queries */
  void lock() { d_locked = true; }
  /** Check whether this LogicInfo is locked, disallowing further mutation */
  bool isLocked() const { return d_locked; }
  /** Get a copy of this LogicInfo that is identical, but unlocked */
  LogicInfo getUnlockedCopy() const;

  // COMPARISON

  /** Are these two LogicInfos equal? */
  bool operator==(const LogicInfo& other) const {
    CheckArgument(isLocked() && other.isLocked(), *this, "This LogicInfo isn't locked yet, and cannot be queried");
    for(theory::TheoryId id = theory::THEORY_FIRST; id < theory::THEORY_LAST; ++id) {
      if(d_theories[id] != other.d_theories[id]) {
        return false;
      }
    }
    CheckArgument(d_sharingTheories == other.d_sharingTheories, *this, "LogicInfo internal inconsistency");
    if(isTheoryEnabled(theory::THEORY_ARITH)) {
      return
        d_integers == other.d_integers &&
        d_reals == other.d_reals &&
        d_linear == other.d_linear &&
        d_differenceLogic == other.d_differenceLogic;
    } else {
      return true;
    }
  }
  /** Are these two LogicInfos disequal? */
  bool operator!=(const LogicInfo& other) const {
    return !(*this == other);
  }
  /** Is this LogicInfo "greater than" (does it contain everything and more) the other? */
  bool operator>(const LogicInfo& other) const {
    return *this >= other && *this != other;
  }
  /** Is this LogicInfo "less than" (does it contain strictly less) the other? */
  bool operator<(const LogicInfo& other) const {
    return *this <= other && *this != other;
  }
  /** Is this LogicInfo "less than or equal" the other? */
  bool operator<=(const LogicInfo& other) const {
    CheckArgument(isLocked() && other.isLocked(), *this, "This LogicInfo isn't locked yet, and cannot be queried");
    for(theory::TheoryId id = theory::THEORY_FIRST; id < theory::THEORY_LAST; ++id) {
      if(d_theories[id] && !other.d_theories[id]) {
        return false;
      }
    }
    CheckArgument(d_sharingTheories <= other.d_sharingTheories, *this, "LogicInfo internal inconsistency");
    if(isTheoryEnabled(theory::THEORY_ARITH) && other.isTheoryEnabled(theory::THEORY_ARITH)) {
      return
        (!d_integers || other.d_integers) &&
        (!d_reals || other.d_reals) &&
        (d_linear || !other.d_linear) &&
        (d_differenceLogic || !other.d_differenceLogic);
    } else {
      return true;
    }
  }
  /** Is this LogicInfo "greater than or equal" the other? */
  bool operator>=(const LogicInfo& other) const {
    CheckArgument(isLocked() && other.isLocked(), *this, "This LogicInfo isn't locked yet, and cannot be queried");
    for(theory::TheoryId id = theory::THEORY_FIRST; id < theory::THEORY_LAST; ++id) {
      if(!d_theories[id] && other.d_theories[id]) {
        return false;
      }
    }
    CheckArgument(d_sharingTheories >= other.d_sharingTheories, *this, "LogicInfo internal inconsistency");
    if(isTheoryEnabled(theory::THEORY_ARITH) && other.isTheoryEnabled(theory::THEORY_ARITH)) {
      return
        (d_integers || !other.d_integers) &&
        (d_reals || !other.d_reals) &&
        (!d_linear || other.d_linear) &&
        (!d_differenceLogic || other.d_differenceLogic);
    } else {
      return true;
    }
  }

  /** Are two LogicInfos comparable?  That is, is one of <= or > true? */
  bool isComparableTo(const LogicInfo& other) const {
    return *this <= other || *this >= other;
  }

};/* class LogicInfo */

std::ostream& operator<<(std::ostream& out, const LogicInfo& logic) CVC4_PUBLIC;

}/* CVC4 namespace */

#endif /* __CVC4__LOGIC_INFO_H */

