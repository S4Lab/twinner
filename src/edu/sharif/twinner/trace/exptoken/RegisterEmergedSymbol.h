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

#ifndef REGISTEREMERGEDSYMBOL_H_
#define REGISTEREMERGEDSYMBOL_H_

#include "Symbol.h"

namespace edu {
namespace sharif {
namespace twinner {
namespace trace {

class Snapshot;
class Expression;

namespace exptoken {

class RegisterEmergedSymbol : public Symbol {
private:
  REG address;

  RegisterEmergedSymbol (const RegisterEmergedSymbol &s);

  RegisterEmergedSymbol (REG address);

public:
  /// Creates a temporary register symbol iff snapshotIndex != -1
  RegisterEmergedSymbol (REG address,
      const edu::sharif::twinner::trace::cv::ConcreteValue &concreteValue,
      int generationIndex, int snapshotIndex = -1);

  virtual RegisterEmergedSymbol *clone () const;
  virtual std::pair < int, SymbolRecord > toSymbolRecord () const;
  REG getAddress () const;

  virtual void saveToBinaryStream (std::ofstream &out) const;
  static RegisterEmergedSymbol *loadFromBinaryStream (std::ifstream &in);
  static RegisterEmergedSymbol *fromNameAndValue (const std::string &name,
      UINT32 v4, UINT32 v3, UINT32 v2, UINT32 v1);
  static RegisterEmergedSymbol *fromNameAndValue (const std::string &name,
      const edu::sharif::twinner::trace::cv::ConcreteValue &value);

  virtual std::string toString () const;
  virtual bool operator== (const ExpressionToken &token) const;
  virtual bool operator< (const Symbol &symbol) const;

private:
  const char *getRegisterName () const;
  static REG getRegisterFromName (const std::string &name);
  static bool is128BitsRegister (REG reg);

public:

  enum RegisterType {
    REG_64_BITS_TYPE = 8,
    REG_32_BITS_TYPE = 4,
    REG_16_BITS_TYPE = 2,
    REG_8_BITS_LOWER_HALF_TYPE = 1,
    REG_8_BITS_UPPER_HALF_TYPE = 3,
  };

  /**
   * Returns type of the given reg.
   * ASSERT: sizeInBytes == REG_Size (reg).
   *
   * @param reg The register which its type is queried.
   * @param registerSize The size of the given reg in bytes.
   * @return The type of the queried register.
   */
  static RegisterType getRegisterType (REG reg, int sizeInBytes);

  /**
   * Gives an index specifying type of the reg. Overlapping registers share one id.
   * The given reg must be a full register; if another register encloses the given
   * register, -1 will be returned instead of the real id.
   * ASSERT: The given reg must be a full register: reg == REG_FullRegName (reg)
   *
   * @param reg The register which index of its enclosing register is asked.
   * @return Index of the enclosing register of the given register.
   */
  static int getRegisterIndex (REG reg);

  /**
   * Gets register index (external; see getRegisterIndex method) and returns the
   * specific register based on the internal index.
   * The intra-family index of register indicates which one of overlapping registers is
   * queried. Values 0, 1, 2, 3, 4 are used for 64, 32, 16, higher-8, and lower-8 bits
   * registers same as the coding scheme which is used in the RegisterType enum type.
   *
   * @param external The family index of the asked register.
   * @param internal The intra-family index of register.
   * @return The register with given family and intra-family indices.
   */
  static REG getOverlappingRegisterByIndex (int external, int internal);

  /**
   * Initializes all overlapping sub-registers of the given register, assuming the
   * register is already initialized with the given expression itself.
   * ASSERT: The given reg must be a full register: reg == REG_FullRegName (reg)
   *
   * @param reg The register which its sub-registers must be initialized.
   * @param snapshot The snapshot which initialization should take place in it.
   * @param expression The current value of register which should be cloned for subregs.
   */
  static void initializeSubRegisters (REG reg,
      Snapshot *snapshot, const Expression &expression);

  /**
   * Iterating on all full registers of the given registerToExpression map,
   * instantiates a new temporary symbol for them with the same concrete value
   * and requested segment and snapshot indices.
   * ASSERT: All given expressions are in the non-overwriting mode.
   *
   * @param registerToExpression The map of reg exps to be converted to temp exps.
   * @param segmentIndex The segment index of newly instantiated temp symbols.
   * @param snapshotIndex The snapshot index of newly instantiated temp symbols.
   * @return A map containing a temp exp for each full reg in the initial map.
   */
  static std::map < REG, Expression * > instantiateTemporarySymbols (
      const std::map < REG, Expression * > &registerToExpression,
      int segmentIndex, int snapshotIndex);
};

}
}
}
}
}

#endif /* REGISTEREMERGEDSYMBOL_H_ */
