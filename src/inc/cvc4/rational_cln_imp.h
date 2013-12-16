/*********************                                                        */
/*! \file rational_cln_imp.h
 ** \verbatim
 ** Original author: Tim King
 ** Major contributors: Morgan Deters
 ** Minor contributors (to current version): Dejan Jovanovic
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2013  New York University and The University of Iowa
 ** See the file COPYING in the top-level source directory for licensing
 ** information.\endverbatim
 **
 ** \brief Multiprecision rational constants; wraps a CLN multiprecision
 ** rational.
 **
 ** Multiprecision rational constants; wraps a CLN multiprecision rational.
 **/

#include "cvc4_public.h"

#ifndef __CVC4__RATIONAL_H
#define __CVC4__RATIONAL_H

#include <gmp.h>
#include <string>
#include <sstream>
#include <cassert>
#include <cln/rational.h>
#include <cln/input.h>
#include <cln/io.h>
#include <cln/output.h>
#include <cln/rational_io.h>
#include <cln/number_io.h>
#include <cln/dfloat.h>
#include <cln/real.h>

#include "util/exception.h"
#include "util/integer.h"

namespace CVC4 {

/**
 ** A multi-precision rational constant.
 ** This stores the rational as a pair of multi-precision integers,
 ** one for the numerator and one for the denominator.
 ** The number is always stored so that the gcd of the numerator and denominator
 ** is 1.  (This is referred to as referred to as canonical form in GMP's
 ** literature.) A consequence is that that the numerator and denominator may be
 ** different than the values used to construct the Rational.
 **
 ** NOTE: The correct way to create a Rational from an int is to use one of the
 ** int numerator/int denominator constructors with the denominator 1.  Trying
 ** to construct a Rational with a single int, e.g., Rational(0), will put you
 ** in danger of invoking the char* constructor, from whence you will segfault.
 **/

class CVC4_PUBLIC Rational {
private:
  /**
   * Stores the value of the rational is stored in a C++ GMP rational class.
   * Using this instead of mpq_t allows for easier destruction.
   */
  cln::cl_RA d_value;

  /**
   * Constructs a Rational from a mpq_class object.
   * Does a deep copy.
   * Assumes that the value is in canonical form, and thus does not
   * have to call canonicalize() on the value.
   */
  //Rational(const mpq_class& val) : d_value(val) {  }
  Rational(const cln::cl_RA& val) : d_value(val) {  }

public:

  /**
   * Creates a rational from a decimal string (e.g., <code>"1.5"</code>).
   *
   * @param dec a string encoding a decimal number in the format
   * <code>[0-9]*\.[0-9]*</code>
   */
  static Rational fromDecimal(const std::string& dec);

  /** Constructs a rational with the value 0/1. */
  Rational() : d_value(0){
  }

  /**
   * Constructs a Rational from a C string in a given base (defaults to 10).
   * Throws std::invalid_argument if the string is not a valid rational.
   * For more information about what is a valid rational string,
   * see GMP's documentation for mpq_set_str().
   */
  explicit Rational(const char* s, unsigned base = 10) throw (std::invalid_argument){
    cln::cl_read_flags flags;

    flags.syntax = cln::syntax_rational;
    flags.lsyntax = cln::lsyntax_standard;
    flags.rational_base = base;
    try{
      d_value = read_rational(flags, s, NULL, NULL);
    }catch(...){
      std::stringstream ss;
      ss << "Rational() failed to parse value \"" <<s << "\" in base=" <<base;
      throw std::invalid_argument(ss.str());
    }
  }
  Rational(const std::string& s, unsigned base = 10) throw (std::invalid_argument){
    cln::cl_read_flags flags;

    flags.syntax = cln::syntax_rational;
    flags.lsyntax = cln::lsyntax_standard;
    flags.rational_base = base;
    try{
      d_value = read_rational(flags, s.c_str(), NULL, NULL);
    }catch(...){
      std::stringstream ss;
      ss << "Rational() failed to parse value \"" <<s << "\" in base=" <<base;
      throw std::invalid_argument(ss.str());
    }
  }

  /**
   * Creates a Rational from another Rational, q, by performing a deep copy.
   */
  Rational(const Rational& q) : d_value(q.d_value) { }

  /**
   * Constructs a canonical Rational from a numerator.
   */
  Rational(signed int n) : d_value((signed long int)n) { }
  Rational(unsigned int n) : d_value((unsigned long int)n) { }
  Rational(signed long int n) : d_value(n) { }
  Rational(unsigned long int n) : d_value(n) { }

#ifdef CVC4_NEED_INT64_T_OVERLOADS
  Rational(int64_t n) : d_value(static_cast<long>(n)) { }
  Rational(uint64_t n) : d_value(static_cast<unsigned long>(n)) { }
#endif /* CVC4_NEED_INT64_T_OVERLOADS */

  /**
   * Constructs a canonical Rational from a numerator and denominator.
   */
  Rational(signed int n, signed int d) : d_value((signed long int)n) {
    d_value /= cln::cl_I(d);
  }
  Rational(unsigned int n, unsigned int d) : d_value((unsigned long int)n) {
    d_value /= cln::cl_I(d);
  }
  Rational(signed long int n, signed long int d) : d_value(n) {
    d_value /= cln::cl_I(d);
  }
  Rational(unsigned long int n, unsigned long int d) : d_value(n) {
    d_value /= cln::cl_I(d);
  }

#ifdef CVC4_NEED_INT64_T_OVERLOADS
  Rational(int64_t n, int64_t d) : d_value(static_cast<long>(n)) {
    d_value /= cln::cl_I(d);
  }
  Rational(uint64_t n, uint64_t d) : d_value(static_cast<unsigned long>(n)) {
    d_value /= cln::cl_I(d);
  }
#endif /* CVC4_NEED_INT64_T_OVERLOADS */

  Rational(const Integer& n, const Integer& d) :
    d_value(n.get_cl_I())
  {
    d_value /= d.get_cl_I();
  }
  Rational(const Integer& n) : d_value(n.get_cl_I()){  }

  ~Rational() {}


  /**
   * Returns the value of numerator of the Rational.
   * Note that this makes a deep copy of the numerator.
   */
  Integer getNumerator() const {
    return Integer(cln::numerator(d_value));
  }

  /**
   * Returns the value of denominator of the Rational.
   * Note that this makes a deep copy of the denominator.
   */
  Integer getDenominator() const {
    return Integer(cln::denominator(d_value));
  }

  /** Return an exact rational for a double d. */
  static Rational fromDouble(double d){
    cln::cl_DF fromD = d;
    Rational q;
    q.d_value = cln::rationalize(fromD);
    return q;
  }

  /**
   * Get a double representation of this Rational, which is
   * approximate: truncation may occur, overflow may result in
   * infinity, and underflow may result in zero.
   */
  double getDouble() const {
    return cln::double_approx(d_value);
  }

  Rational inverse() const {
    return Rational(cln::recip(d_value));
  }

  int cmp(const Rational& x) const {
    //Don't use mpq_class's cmp() function.
    //The name ends up conflicting with this function.
    return cln::compare(d_value, x.d_value);
  }


  int sgn() const {
    if(cln::zerop(d_value)){
       return 0;
    }else if(cln::minusp(d_value)){
       return -1;
    }else{
      assert(cln::plusp(d_value));
      return 1;
    }
  }

  bool isZero() const {
    return cln::zerop(d_value);
  }

  bool isOne() const {
    return d_value == 1;
  }

  bool isNegativeOne() const {
    return d_value == -1;
  }

  Rational abs() const {
    if(sgn() < 0){
      return -(*this);
    }else{
      return *this;
    }
  }

  bool isIntegral() const{
    return getDenominator() == 1;
  }

  Integer floor() const {
    return Integer(cln::floor1(d_value));
  }

  Integer ceiling() const {
    return Integer(cln::ceiling1(d_value));
  }

  Rational& operator=(const Rational& x){
    if(this == &x) return *this;
    d_value = x.d_value;
    return *this;
  }

  Rational operator-() const{
    return Rational(-(d_value));
  }

  bool operator==(const Rational& y) const {
    return d_value == y.d_value;
  }

  bool operator!=(const Rational& y) const {
    return d_value != y.d_value;
  }

  bool operator< (const Rational& y) const {
    return d_value < y.d_value;
  }

  bool operator<=(const Rational& y) const {
    return d_value <= y.d_value;
  }

  bool operator> (const Rational& y) const {
    return d_value > y.d_value;
  }

  bool operator>=(const Rational& y) const {
    return d_value >= y.d_value;
  }

  Rational operator+(const Rational& y) const{
    return Rational( d_value + y.d_value );
  }
  Rational operator-(const Rational& y) const {
    return Rational( d_value - y.d_value );
  }

  Rational operator*(const Rational& y) const {
    return Rational( d_value * y.d_value );
  }
  Rational operator/(const Rational& y) const {
    return Rational( d_value / y.d_value );
  }

  Rational& operator+=(const Rational& y){
    d_value += y.d_value;
    return (*this);
  }

  Rational& operator-=(const Rational& y){
    d_value -= y.d_value;
    return (*this);
  }

  Rational& operator*=(const Rational& y){
    d_value *= y.d_value;
    return (*this);
  }

  Rational& operator/=(const Rational& y){
    d_value /= y.d_value;
    return (*this);
  }

  /** Returns a string representing the rational in the given base. */
  std::string toString(int base = 10) const {
    cln::cl_print_flags flags;
    flags.rational_base = base;
    flags.rational_readably = false;
    std::stringstream ss;
    print_rational(ss, flags, d_value);
    return ss.str();
  }

  /**
   * Computes the hash of the rational from hashes of the numerator and the
   * denominator.
   */
  size_t hash() const {
    return equal_hashcode(d_value);
  }

  uint32_t complexity() const {
    return getNumerator().length() + getDenominator().length();
  }

};/* class Rational */

struct RationalHashFunction {
  inline size_t operator()(const CVC4::Rational& r) const {
    return r.hash();
  }
};/* struct RationalHashFunction */

CVC4_PUBLIC std::ostream& operator<<(std::ostream& os, const Rational& n);

}/* CVC4 namespace */

#endif /* __CVC4__RATIONAL_H */

