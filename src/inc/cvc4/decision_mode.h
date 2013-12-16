/*********************                                                        */
/*! \file decision_mode.h
 ** \verbatim
 ** Original author: Morgan Deters
 ** Major contributors: Kshitij Bansal
 ** Minor contributors (to current version): none
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include "cvc4_private.h"

#ifndef __CVC4__SMT__DECISION_MODE_H
#define __CVC4__SMT__DECISION_MODE_H

#include <iostream>

namespace CVC4 {
namespace decision {

/** Enumeration of decision strategies */
enum DecisionMode {

  /**
   * Decision engine doesn't do anything. Use sat solver's internal
   * heuristics
   */
  DECISION_STRATEGY_INTERNAL,

  /**
   * Use the justification heuristic
   */
  DECISION_STRATEGY_JUSTIFICATION,

  /**
   * Use may-relevancy.
   */
  DECISION_STRATEGY_RELEVANCY

};/* enum DecisionMode */


/** Enumeration of combining functions for computing internal weights */
enum DecisionWeightInternal {
  DECISION_WEIGHT_INTERNAL_OFF,
  DECISION_WEIGHT_INTERNAL_MAX,
  DECISION_WEIGHT_INTERNAL_SUM,
  DECISION_WEIGHT_INTERNAL_USR1
};/* enum DecisionInternalWeight */

}/* CVC4::decision namespace */

std::ostream& operator<<(std::ostream& out, decision::DecisionMode mode) CVC4_PUBLIC;

}/* CVC4 namespace */

#endif /* __CVC4__SMT__DECISION_MODE_H */
