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

#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <set>
#include <map>

namespace edu {
namespace sharif {
namespace twinner {
namespace engine {
namespace etg {
namespace encoder {

class Variable {
public:
  const std::string type;
  const std::string technicalName;
  const std::string name;
  const int index;

  Variable (std::string _type,
      std::string _technicalName, std::string _name, int _index);

  bool operator< (const Variable &v) const;
};

class VariableContainer {
private:
  std::map< std::string, std::map< int, Variable > * > nameToIndexToVar;

public:
  VariableContainer ();
  VariableContainer (const VariableContainer &vc);
  VariableContainer (const std::set< Variable > &vars);
  ~VariableContainer ();

  void addVariable (std::string name, Variable var);
  const Variable &provideVariable (const Variable &var,
      bool &shouldDefine) const;
};

}
}
}
}
}
}

#endif /* Variable.h */
