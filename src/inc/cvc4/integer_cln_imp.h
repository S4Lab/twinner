/*********************                                                        */
/*! \file integer_cln_imp.h
 ** \verbatim
 ** Original author: Tim King
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): Dejan Jovanovic, Liana Hadarean
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief A multiprecision integer constant; wraps a CLN multiprecision
 ** integer.
 **
 ** A multiprecision integer constant; wraps a CLN multiprecision integer.
 **/

#include "cvc4_public.h"

#ifndef __CVC4__INTEGER_H
#define __CVC4__INTEGER_H

#include <string>
#include <sstream>
#include <iostream>

#include <cln/integer.h>
#include <cln/input.h>
#include <cln/integer_io.h>
#include <limits>

#include "util/exception.h"

namespace CVC4 {

class Rational;

class CVC4_PUBLIC Integer {
private:
  /**
   * Stores the value of the rational is stored in a C++ GMP integer class.
   * Using this instead of mpz_t allows for easier destruction.
   */
  cln::cl_I d_value;

  /**
   * Gets a reference to the gmp data that backs up the integer.
   * Only accessible to friend classes.
   */
  //const mpz_class& get_mpz() const { return d_value; }
  const cln::cl_I& get_cl_I() const { return d_value; }

  /**
   * Constructs an Integer by copying a GMP C++ primitive.
   */
  //Integer(const mpz_class& val) : d_value(val) {}
  Integer(const cln::cl_I& val) : d_value(val) {}

  void readInt(const cln::cl_read_flags& flags, const std::string& s, unsigned base) throw(std::invalid_argument) {
    try {
      if(s.find_first_not_of('0') == std::string::npos) {
        // String of all zeroes, CLN has a bug for these inputs up to and
        // including CLN v1.3.2.
        // See http://www.ginac.de/CLN/cln.git/?a=commit;h=4a477b0cc3dd7fbfb23b25090ff8c8869c8fa21a for details.
        d_value = read_integer(flags, "0", NULL, NULL);
      } else {
        d_value = read_integer(flags, s.c_str(), NULL, NULL);
      }
    } catch(...) {
      std::stringstream ss;
      ss << "Integer() failed to parse value \"" << s << "\" in base " << base;
      throw std::invalid_argument(ss.str());
    }
  }

  void parseInt(const std::string& s, unsigned base) throw(std::invalid_argument) {
    cln::cl_read_flags flags;
    flags.syntax = cln::syntax_integer;
    flags.lsyntax = cln::lsyntax_standard;
    flags.rational_base = base;
    if(base == 0) {
      // infer base in a manner consistent with GMP
      if(s[0] == '0') {
        flags.lsyntax = cln::lsyntax_commonlisp;
        std::string st = s;
        if(s[1] == 'X' || s[1] == 'x') {
          st.replace(0, 2, "#x");
        } else if(s[1] == 'B' || s[1] == 'b') {
          st.replace(0, 2, "#b");
        } else {
          st.replace(0, 1, "#o");
        }
        readInt(flags, st, base);
        return;
      } else {
        flags.rational_base = 10;
      }
    }
    readInt(flags, s, base);
  }

public:

  /** Constructs a rational with the value 0. */
  Integer() : d_value(0){}

  /**
   * Constructs a Integer from a C string.
   * Throws std::invalid_argument if the string is not a valid rational.
   * For more information about what is a valid rational string,
   * see GMP's documentation for mpq_set_str().
   */
  explicit Integer(const char* sp, unsigned base = 10) throw (std::invalid_argument) {
    parseInt(std::string(sp), base);
  }

  explicit Integer(const std::string& s, unsigned base = 10) throw (std::invalid_argument) {
    parseInt(s, base);
  }

  Integer(const Integer& q) : d_value(q.d_value) {}

  Integer(  signed int z) : d_value((signed long int)z) {}
  Integer(unsigned int z) : d_value((unsigned long int)z) {}
  Integer(  signed long int z) : d_value(z) {}
  Integer(unsigned long int z) : d_value(z) {}

#ifdef CVC4_NEED_INT64_T_OVERLOADS
  Integer( int64_t z) : d_value(static_cast<long>(z)) {}
  Integer(uint64_t z) : d_value(static_cast<unsigned long>(z)) {}
#endif /* CVC4_NEED_INT64_T_OVERLOADS */

  ~Integer() {}

  Integer& operator=(const Integer& x){
    if(this == &x) return *this;
    d_value = x.d_value;
    return *this;
  }

  bool operator==(const Integer& y) const {
    return d_value == y.d_value;
  }

  Integer operator-() const{
    return Integer(-(d_value));
  }


  bool operator!=(const Integer& y) const {
    return d_value != y.d_value;
  }

  bool operator< (const Integer& y) const {
    return d_value < y.d_value;
  }

  bool operator<=(const Integer& y) const {
    return d_value <= y.d_value;
  }

  bool operator> (const Integer& y) const {
    return d_value > y.d_value;
  }

  bool operator>=(const Integer& y) const {
    return d_value >= y.d_value;
  }


  Integer operator+(const Integer& y) const {
    return Integer( d_value + y.d_value );
  }
  Integer& operator+=(const Integer& y) {
    d_value += y.d_value;
    return *this;
  }

  Integer operator-(const Integer& y) const {
    return Integer( d_value - y.d_value );
  }
  Integer& operator-=(const Integer& y) {
    d_value -= y.d_value;
    return *this;
  }

  Integer operator*(const Integer& y) const {
    return Integer( d_value * y.d_value );
  }
  Integer& operator*=(const Integer& y) {
    d_value *= y.d_value;
    return *this;
  }


  Integer bitwiseOr(const Integer& y) const {
    return Integer(cln::logior(d_value, y.d_value));
  }

  Integer bitwiseAnd(const Integer& y) const {
    return Integer(cln::logand(d_value, y.d_value));
  }

  Integer bitwiseXor(const Integer& y) const {
    return Integer(cln::logxor(d_value, y.d_value));
  }

  Integer bitwiseNot() const {
    return Integer(cln::lognot(d_value));
  }


  /**
   * Return this*(2^pow).
   */
  Integer multiplyByPow2(uint32_t pow) const {
    cln::cl_I ipow(pow);
    return Integer( d_value << ipow);
  }

  bool isBitSet(uint32_t i) const {
    return !extractBitRange(1, i).isZero(); 
  }
  
  Integer setBit(uint32_t i) const {
    cln::cl_I mask(1);
    mask = mask << i; 
    return Integer(cln::logior(d_value, mask)); 
  }
  
  Integer oneExtend(uint32_t size, uint32_t amount) const {
    DebugCheckArgument((*this) < Integer(1).multiplyByPow2(size), size);
    cln::cl_byte range(amount, size);
    cln::cl_I allones = (cln::cl_I(1) << (size + amount))- 1; // 2^size - 1
    Integer temp(allones);

    return Integer(cln::deposit_field(allones, d_value, range));
  }

  uint32_t toUnsignedInt() const {
    return cln::cl_I_to_uint(d_value);
  }


  /** See CLN Documentation. */
  Integer extractBitRange(uint32_t bitCount, uint32_t low) const {
    cln::cl_byte range(bitCount, low);
    return Integer(cln::ldb(d_value, range));
  }

  /**
   * Returns the floor(this / y)
   */
  Integer floorDivideQuotient(const Integer& y) const {
    return Integer( cln::floor1(d_value, y.d_value) );
  }

  /**
   * Returns r == this - floor(this/y)*y
   */
  Integer floorDivideRemainder(const Integer& y) const {
    return Integer( cln::floor2(d_value, y.d_value).remainder );
  }
   /**
   * Computes a floor quoient and remainder for x divided by y.
   */
  static void floorQR(Integer& q, Integer& r, const Integer& x, const Integer& y) {
    cln::cl_I_div_t res = cln::floor2(x.d_value, y.d_value);
    q.d_value = res.quotient;
    r.d_value = res.remainder;
  }

  /**
   * Returns the ceil(this / y)
   */
  Integer ceilingDivideQuotient(const Integer& y) const {
    return Integer( cln::ceiling1(d_value, y.d_value) );
  }

  /**
   * Returns the ceil(this / y)
   */
  Integer ceilingDivideRemainder(const Integer& y) const {
    return Integer( cln::ceiling2(d_value, y.d_value).remainder );
  }

  /**
   * Computes a quoitent and remainder according to Boute's Euclidean definition.
   * euclidianDivideQuotient, euclidianDivideRemainder.
   *
   * Boute, Raymond T. (April 1992).
   * The Euclidean definition of the functions div and mod.
   * ACM Transactions on Programming Languages and Systems (TOPLAS)
   * ACM Press. 14 (2): 127 - 144. doi:10.1145/128861.128862.
   */
  static void euclidianQR(Integer& q, Integer& r, const Integer& x, const Integer& y) {
    // compute the floor and then fix the value up if needed.
    floorQR(q,r,x,y);

    if(r.strictlyNegative()){
      // if r < 0
      // abs(r) < abs(y)
      // - abs(y) < r < 0, then 0 < r + abs(y) < abs(y)
      // n = y * q + r
      // n = y * q - abs(y) + r + abs(y)
      if(r.sgn() >= 0){
        // y = abs(y)
        // n = y * q - y + r + y
        // n = y * (q-1) + (r+y)
        q -= 1;
        r += y;
      }else{
        // y = -abs(y)
        // n = y * q + y + r - y
        // n = y * (q+1) + (r-y)
        q += 1;
        r -= y;
      }
    }
  }

  /**
   * Returns the quoitent according to Boute's Euclidean definition.
   * See the documentation for euclidianQR.
   */
  Integer euclidianDivideQuotient(const Integer& y) const {
    Integer q,r;
    euclidianQR(q,r, *this, y);
    return q;
  }

  /**
   * Returns the remainfing according to Boute's Euclidean definition.
   * See the documentation for euclidianQR.
   */
  Integer euclidianDivideRemainder(const Integer& y) const {
    Integer q,r;
    euclidianQR(q,r, *this, y);
    return r;
  }

  /**
   * If y divides *this, then exactQuotient returns (this/y)
   */
  Integer exactQuotient(const Integer& y) const {
    DebugCheckArgument(y.divides(*this), y);
    return Integer( cln::exquo(d_value, y.d_value) );
  }

  Integer modByPow2(uint32_t exp) const {
    cln::cl_byte range(exp, 0);
    return Integer(cln::ldb(d_value, range));
  }

  Integer divByPow2(uint32_t exp) const {
    return d_value >> exp;
  }

  /**
   * Raise this Integer to the power <code>exp</code>.
   *
   * @param exp the exponent
   */
  Integer pow(unsigned long int exp) const {
    if(exp > 0){
      cln::cl_I result= cln::expt_pos(d_value,exp);
      return Integer( result );
    }else if(exp == 0){
      return Integer( 1 );
    }else{
      throw Exception("Negative exponent in Integer::pow()");
    }
  }

  /**
   * Return the greatest common divisor of this integer with another.
   */
  Integer gcd(const Integer& y) const {
    cln::cl_I result = cln::gcd(d_value, y.d_value);
    return Integer(result);
  }

  /**
   * Return the least common multiple of this integer with another.
   */
  Integer lcm(const Integer& y) const {
    cln::cl_I result = cln::lcm(d_value, y.d_value);
    return Integer(result);
  }

  /**
   * Return true if *this exactly divides y.
   */
  bool divides(const Integer& y) const {
    cln::cl_I result = cln::rem(y.d_value, d_value);
    return cln::zerop(result);
  }

  /**
   * Return the absolute value of this integer.
   */
  Integer abs() const {
    return d_value >= 0 ? *this : -*this;
  }

  std::string toString(int base = 10) const{
    std::stringstream ss;
    switch(base){
    case 2:
      fprintbinary(ss,d_value);
      break;
    case 8:
      fprintoctal(ss,d_value);
      break;
    case 10:
      fprintdecimal(ss,d_value);
      break;
    case 16:
      fprinthexadecimal(ss,d_value);
      break;
    default:
      throw Exception("Unhandled base in Integer::toString()");
    }
    std::string output = ss.str();
    for( unsigned i = 0; i <= output.length(); ++i){
      if(isalpha(output[i])){
        output.replace(i, 1, 1, tolower(output[i]));
      }
    }

    return output;
  }

  int sgn() const {
    cln::cl_I sgn = cln::signum(d_value);
    return cln::cl_I_to_int(sgn);
  }


  inline bool strictlyPositive() const {
    return sgn() > 0;
  }

  inline bool strictlyNegative() const {
    return sgn() < 0;
  }

  inline bool isZero() const {
    return sgn() == 0;
  }

  inline bool isOne() const {
    return d_value == 1;
  }

  inline bool isNegativeOne() const {
    return d_value == -1;
  }

  long getLong() const {
    // ensure there isn't overflow
    CheckArgument(d_value <= std::numeric_limits<long>::max(), this,
                  "Overflow detected in Integer::getLong()");
    CheckArgument(d_value >= std::numeric_limits<long>::min(), this,
                  "Overflow detected in Integer::getLong()");
    return cln::cl_I_to_long(d_value);
  }

  unsigned long getUnsignedLong() const {
    // ensure there isn't overflow
    CheckArgument(d_value <= std::numeric_limits<unsigned long>::max(), this,
                  "Overflow detected in Integer::getUnsignedLong()");
    CheckArgument(d_value >= std::numeric_limits<unsigned long>::min(), this,
                  "Overflow detected in Integer::getUnsignedLong()");
    return cln::cl_I_to_ulong(d_value);
  }

  /**
   * Computes the hash of the node from the first word of the
   * numerator, the denominator.
   */
  size_t hash() const {
    return equal_hashcode(d_value);
  }

  /**
   * Returns true iff bit n is set.
   *
   * @param n the bit to test (0 == least significant bit)
   * @return true if bit n is set in this integer; false otherwise
   */
  bool testBit(unsigned n) const {
    return cln::logbitp(n, d_value);
  }

  /**
   * Returns k if the integer is equal to 2^(k-1)
   * @return k if the integer is equal to 2^(k-1) and 0 otherwise
   */
  unsigned isPow2() const {
    if (d_value <= 0) return 0;
    // power2p returns n such that d_value = 2^(n-1)
    return cln::power2p(d_value);
  }

  /**
   * If x != 0, returns the unique n s.t. 2^{n-1} <= abs(x) < 2^{n}.
   * If x == 0, returns 1.
   */
  size_t length() const {
    int s = sgn();
    if(s == 0){
      return 1;
    }else if(s < 0){
      size_t len = cln::integer_length(d_value);
      /*If this is -2^n, return len+1 not len to stay consistent with the definition above!
       * From CLN's documentation of integer_length:
       *   This is the smallest n >= 0 such that -2^n <= x < 2^n.
       *   If x > 0, this is the unique n > 0 such that 2^(n-1) <= x < 2^n.
       */
      size_t ord2 = cln::ord2(d_value);
      return (len == ord2) ? (len + 1) : len;
    }else{
      return cln::integer_length(d_value);
    }
  }

/*   cl_I xgcd (const cl_I& a, const cl_I& b, cl_I* u, cl_I* v) */
/* This function ("extended gcd") returns the greatest common divisor g of a and b and at the same time the representation of g as an integral linear combination of a and b: u and v with u*a+v*b = g, g >= 0. u and v will be normalized to be of smallest possible absolute value, in the following sense: If a and b are non-zero, and abs(a) != abs(b), u and v will satisfy the inequalities abs(u) <= abs(b)/(2*g), abs(v) <= abs(a)/(2*g). */
  static void extendedGcd(Integer& g, Integer& s, Integer& t, const Integer& a, const Integer& b){
    g.d_value = cln::xgcd(a.d_value, b.d_value, &s.d_value, &t.d_value);
  }

  /** Returns a reference to the minimum of two integers. */
  static const Integer& min(const Integer& a, const Integer& b){
    return (a <=b ) ? a : b;
  }

  /** Returns a reference to the maximum of two integers. */
  static const Integer& max(const Integer& a, const Integer& b){
    return (a >= b ) ? a : b;
  }

  friend class CVC4::Rational;
};/* class Integer */

struct IntegerHashFunction {
  inline size_t operator()(const CVC4::Integer& i) const {
    return i.hash();
  }
};/* struct IntegerHashFunction */

inline std::ostream& operator<<(std::ostream& os, const Integer& n) {
  return os << n.toString();
}

}/* CVC4 namespace */

#endif /* __CVC4__INTEGER_H */
