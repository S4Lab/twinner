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

#include "Variable.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {

Variable::Variable (std::string _type,
    std::string _technicalName, std::string _name, int _index)
    : type (_type),
    technicalName (_technicalName), name (_name),
    index (_index) {
}

bool Variable::operator< (const Variable &v) const {
  return technicalName < v.technicalName;
}

VariableContainer::VariableContainer () {
}

VariableContainer::VariableContainer (const VariableContainer &vc) {
  for (std::map< std::string, std::map< int, Variable > * >::const_iterator it =
      vc.nameToIndexToVar.begin (); it != vc.nameToIndexToVar.end (); ++it) {
    const std::map< int, Variable > *vci2v = it->second;
    std::map< int, Variable > *indexToVar = new std::map< int, Variable > ();
    for (std::map< int, Variable >::const_iterator it2 = vci2v->begin ();
        it2 != vci2v->end (); ++it2) {
      indexToVar->insert (std::make_pair (it2->first, it2->second));
    }
    nameToIndexToVar.insert (std::make_pair (it->first, indexToVar));
  }
}

VariableContainer::VariableContainer (const std::set< Variable > &vars) {
  for (std::set< Variable >::const_iterator it = vars.begin ();
      it != vars.end (); ++it) {
    if (it->technicalName.substr (0, 2) == "n_") { // named symbol
      // named symbols are always available and do not need translation
    } else {
      addVariable (it->technicalName.substr (0, it->technicalName.find ("_")),
                   *it);
    }
  }
}

VariableContainer::~VariableContainer () {
  for (std::map< std::string, std::map< int, Variable > * >::iterator it =
      nameToIndexToVar.begin (); it != nameToIndexToVar.end (); ++it) {
    delete it->second;
  }
  nameToIndexToVar.clear ();
}

void VariableContainer::addVariable (std::string name, Variable var) {
  std::map< std::string, std::map< int, Variable > * >::iterator it =
      nameToIndexToVar.find (name);
  std::map< int, Variable > *indexToVar;
  if (it == nameToIndexToVar.end ()) {
    indexToVar = new std::map< int, Variable > ();
    nameToIndexToVar.insert (std::make_pair (name, indexToVar));
  } else {
    indexToVar = it->second;
  }
  indexToVar->insert (std::make_pair (var.index, var));
}

const Variable &VariableContainer::provideVariable (const Variable &var,
    bool &shouldDefine) const {
  if (var.technicalName.substr (0, 2) == "n_") { // named symbol
    shouldDefine = false;
    return var;
  }
  const std::string name = var.technicalName.substr (0, var.technicalName.find ("_"));
  std::map< std::string, std::map< int, Variable > * >::const_iterator it =
      nameToIndexToVar.find (name);
  if (it == nameToIndexToVar.end ()) {
    shouldDefine = (name[0] == 'm');
    return var;
  }
  const std::map< int, Variable > *indexToVar = it->second;
  std::map< int, Variable >::const_iterator it2 = indexToVar->upper_bound (var.index);
  if (it2 == indexToVar->begin ()) {
    shouldDefine = (name[0] == 'm');
    return var;
  }
  shouldDefine = false;
  return (--it2)->second;
}

}
}
}
}
}
}
