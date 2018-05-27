/* Specialized "checked" functions for GMP's mpq_class numbers.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://bugseng.com/products/ppl/ . */

#ifndef PPL_checked_mpq_inlines_hh
#define PPL_checked_mpq_inlines_hh 1

#include <sstream>
#include <climits>
#include <stdexcept>

namespace Parma_Polyhedra_Library {

namespace Checked {

template <typename Policy>
inline Result
classify_mpq(const mpq_class& v, bool nan, bool inf, bool sign) {
  if ((Policy::has_nan || Policy::has_infinity)
      && ::sgn(v.get_den()) == 0) {
    int s = ::sgn(v.get_num());
    if (Policy::has_nan && (nan || sign) && s == 0)
      return V_NAN;
    if (!inf && !sign)
      return V_LGE;
    if (Policy::has_infinity) {
      if (s < 0)
        return inf ? V_EQ_MINUS_INFINITY : V_LT;
      if (s > 0)
        return inf ? V_EQ_PLUS_INFINITY : V_GT;
    }
  }
  if (sign)
    return static_cast<Result>(sgn<Policy>(v));
  return V_LGE;
}

PPL_SPECIALIZE_CLASSIFY(classify_mpq, mpq_class)

template <typename Policy>
inline bool
is_nan_mpq(const mpq_class& v) {
  return Policy::has_nan
    && ::sgn(v.get_den()) == 0
    && ::sgn(v.get_num()) == 0;
}

PPL_SPECIALIZE_IS_NAN(is_nan_mpq, mpq_class)

template <typename Policy>
inline bool
is_minf_mpq(const mpq_class& v) {
  return Policy::has_infinity
    && ::sgn(v.get_den()) == 0
    && ::sgn(v.get_num()) < 0;
}

PPL_SPECIALIZE_IS_MINF(is_minf_mpq, mpq_class)

template <typename Policy>
inline bool
is_pinf_mpq(const mpq_class& v) {
  return Policy::has_infinity
    && ::sgn(v.get_den()) == 0
    && ::sgn(v.get_num()) > 0;
}

PPL_SPECIALIZE_IS_PINF(is_pinf_mpq, mpq_class)

template <typename Policy>
inline bool
is_int_mpq(const mpq_class& v) {
  if ((Policy::has_infinity || Policy::has_nan)
      && ::sgn(v.get_den()) == 0)
    return !(Policy::has_nan && ::sgn(v.get_num()) == 0);
  else
    return v.get_den() == 1;
}

PPL_SPECIALIZE_IS_INT(is_int_mpq, mpq_class)

template <typename Policy>
inline Result
assign_special_mpq(mpq_class& v, Result_Class c, Rounding_Dir) {
  switch (c) {
  case VC_NAN:
    if (Policy::has_nan) {
      v.get_num() = 0;
      v.get_den() = 0;
      return V_NAN | V_UNREPRESENTABLE;
    }
    return V_NAN;
  case VC_MINUS_INFINITY:
    if (Policy::has_infinity) {
      v.get_num() = -1;
      v.get_den() = 0;
      return V_EQ_MINUS_INFINITY;
    }
    return V_EQ_MINUS_INFINITY | V_UNREPRESENTABLE;
  case VC_PLUS_INFINITY:
    if (Policy::has_infinity) {
      v.get_num() = 1;
      v.get_den() = 0;
      return V_EQ_PLUS_INFINITY;
    }
    return V_EQ_PLUS_INFINITY | V_UNREPRESENTABLE;
  default:
    PPL_UNREACHABLE;
    return V_NAN | V_UNREPRESENTABLE;
  }
}

PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_mpq, mpq_class)

PPL_SPECIALIZE_COPY(copy_generic, mpq_class)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
construct_mpq_base(mpq_class& to, const From& from, Rounding_Dir) {
  new (&to) mpq_class(from);
  return V_EQ;
}

PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, mpz_class)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, char)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, signed char)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, signed short)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, signed int)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, signed long)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, unsigned char)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, unsigned short)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, unsigned int)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_base, mpq_class, unsigned long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
construct_mpq_float(mpq_class& to, const From& from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
  new (&to) mpq_class(from);
  return V_EQ;
}

PPL_SPECIALIZE_CONSTRUCT(construct_mpq_float, mpq_class, float)
PPL_SPECIALIZE_CONSTRUCT(construct_mpq_float, mpq_class, double)

PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, char)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, signed char)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, signed short)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, signed int)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, signed long)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, unsigned char)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, unsigned short)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, unsigned int)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpq_class, unsigned long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpq_float(mpq_class& to, const From& from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
  assign_mpq_numeric_float(to, from);
  return V_EQ;
}

PPL_SPECIALIZE_ASSIGN(assign_mpq_float, mpq_class, float)
PPL_SPECIALIZE_ASSIGN(assign_mpq_float, mpq_class, double)
PPL_SPECIALIZE_ASSIGN(assign_mpq_float, mpq_class, long double)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpq_signed_int(mpq_class& to, const From from, Rounding_Dir) {
  if (sizeof(From) <= sizeof(signed long))
    to = static_cast<signed long>(from);
  else {
    mpz_ptr m = to.get_num().get_mpz_t();
    if (from >= 0)
      mpz_import(m, 1, 1, sizeof(From), 0, 0, &from);
    else {
      From n = -from;
      mpz_import(m, 1, 1, sizeof(From), 0, 0, &n);
      mpz_neg(m, m);
    }
    to.get_den() = 1;
  }
  return V_EQ;
}

PPL_SPECIALIZE_ASSIGN(assign_mpq_signed_int, mpq_class, signed long long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpq_unsigned_int(mpq_class& to, const From from, Rounding_Dir) {
  if (sizeof(From) <= sizeof(unsigned long))
    to = static_cast<unsigned long>(from);
  else {
    mpz_import(to.get_num().get_mpz_t(), 1, 1, sizeof(From), 0, 0, &from);
    to.get_den() = 1;
  }
  return V_EQ;
}

PPL_SPECIALIZE_ASSIGN(assign_mpq_unsigned_int, mpq_class, unsigned long long)

template <typename To_Policy, typename From_Policy>
inline Result
floor_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  mpz_fdiv_q(to.get_num().get_mpz_t(),
             from.get_num().get_mpz_t(), from.get_den().get_mpz_t());
  to.get_den() = 1;
  return V_EQ;
}

PPL_SPECIALIZE_FLOOR(floor_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
ceil_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  mpz_cdiv_q(to.get_num().get_mpz_t(),
             from.get_num().get_mpz_t(), from.get_den().get_mpz_t());
  to.get_den() = 1;
  return V_EQ;
}

PPL_SPECIALIZE_CEIL(ceil_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
trunc_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  mpz_tdiv_q(to.get_num().get_mpz_t(),
             from.get_num().get_mpz_t(), from.get_den().get_mpz_t());
  to.get_den() = 1;
  return V_EQ;
}

PPL_SPECIALIZE_TRUNC(trunc_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
neg_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  mpq_neg(to.get_mpq_t(), from.get_mpq_t());
  return V_EQ;
}

PPL_SPECIALIZE_NEG(neg_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
add_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  to = x + y;
  return V_EQ;
}

PPL_SPECIALIZE_ADD(add_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
sub_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  to = x - y;
  return V_EQ;
}

PPL_SPECIALIZE_SUB(sub_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
mul_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  to = x * y;
  return V_EQ;
}

PPL_SPECIALIZE_MUL(mul_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
div_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, sgn(y) == 0)) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  to = x / y;
  return V_EQ;
}

PPL_SPECIALIZE_DIV(div_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
idiv_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_div_zero, sgn(y) == 0)) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  to = x / y;
  return trunc<To_Policy, To_Policy>(to, to, dir);
}

PPL_SPECIALIZE_IDIV(idiv_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
rem_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y, Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, sgn(y) == 0)) {
    return assign_nan<To_Policy>(to, V_MOD_ZERO);
  }
  PPL_DIRTY_TEMP(mpq_class, tmp);
  tmp = x / y;
  tmp.get_num() %= tmp.get_den();
  to = tmp * y;
  return V_EQ;
}

PPL_SPECIALIZE_REM(rem_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
add_2exp_mpq(mpq_class& to, const mpq_class& x, unsigned int exp,
             Rounding_Dir) {
  PPL_DIRTY_TEMP(mpz_class, v);
  v = 1;
  mpz_mul_2exp(v.get_mpz_t(), v.get_mpz_t(), exp);
  to = x + v;
  return V_EQ;
}

PPL_SPECIALIZE_ADD_2EXP(add_2exp_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
sub_2exp_mpq(mpq_class& to, const mpq_class& x, unsigned int exp,
             Rounding_Dir) {
  PPL_DIRTY_TEMP(mpz_class, v);
  v = 1;
  mpz_mul_2exp(v.get_mpz_t(), v.get_mpz_t(), exp);
  to = x - v;
  return V_EQ;
}

PPL_SPECIALIZE_SUB_2EXP(sub_2exp_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
mul_2exp_mpq(mpq_class& to, const mpq_class& x, unsigned int exp,
             Rounding_Dir) {
  mpz_mul_2exp(to.get_num().get_mpz_t(), x.get_num().get_mpz_t(), exp);
  to.get_den() = x.get_den();
  to.canonicalize();
  return V_EQ;
}

PPL_SPECIALIZE_MUL_2EXP(mul_2exp_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
div_2exp_mpq(mpq_class& to, const mpq_class& x, unsigned int exp,
             Rounding_Dir) {
  to.get_num() = x.get_num();
  mpz_mul_2exp(to.get_den().get_mpz_t(), x.get_den().get_mpz_t(), exp);
  to.canonicalize();
  return V_EQ;
}

PPL_SPECIALIZE_DIV_2EXP(div_2exp_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
smod_2exp_mpq(mpq_class& to, const mpq_class& x, unsigned int exp,
              Rounding_Dir) {
  mpz_mul_2exp(to.get_den().get_mpz_t(), x.get_den().get_mpz_t(), exp);
  mpz_fdiv_r(to.get_num().get_mpz_t(), x.get_num().get_mpz_t(), to.get_den().get_mpz_t());
  mpz_fdiv_q_2exp(to.get_den().get_mpz_t(), to.get_den().get_mpz_t(), 1);
  bool neg = to.get_num() >= to.get_den();
  mpz_mul_2exp(to.get_den().get_mpz_t(), to.get_den().get_mpz_t(), 1);
  if (neg)
    to.get_num() -= to.get_den();
  mpz_mul_2exp(to.get_num().get_mpz_t(), to.get_num().get_mpz_t(), exp);
  to.canonicalize();
  return V_EQ;
}

PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
umod_2exp_mpq(mpq_class& to, const mpq_class& x, unsigned int exp,
              Rounding_Dir) {
  mpz_mul_2exp(to.get_den().get_mpz_t(), x.get_den().get_mpz_t(), exp);
  mpz_fdiv_r(to.get_num().get_mpz_t(), x.get_num().get_mpz_t(), to.get_den().get_mpz_t());
  mpz_mul_2exp(to.get_num().get_mpz_t(), to.get_num().get_mpz_t(), exp);
  to.canonicalize();
  return V_EQ;
}

PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From_Policy>
inline Result
abs_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir) {
  to = abs(from);
  return V_EQ;
}

PPL_SPECIALIZE_ABS(abs_mpq, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
add_mul_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y,
            Rounding_Dir) {
  to += x * y;
  return V_EQ;
}

PPL_SPECIALIZE_ADD_MUL(add_mul_mpq, mpq_class, mpq_class, mpq_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
sub_mul_mpq(mpq_class& to, const mpq_class& x, const mpq_class& y,
            Rounding_Dir) {
  to -= x * y;
  return V_EQ;
}

PPL_SPECIALIZE_SUB_MUL(sub_mul_mpq, mpq_class, mpq_class, mpq_class)

extern unsigned irrational_precision;

template <typename To_Policy, typename From_Policy>
inline Result
sqrt_mpq(mpq_class& to, const mpq_class& from, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_sqrt_neg, from < 0)) {
    return assign_nan<To_Policy>(to, V_SQRT_NEG);
  }
  if (from == 0) {
    to = 0;
    return V_EQ;
  }
  bool gt1 = from.get_num() > from.get_den();
  const mpz_class& from_a = gt1 ? from.get_num() : from.get_den();
  const mpz_class& from_b = gt1 ? from.get_den() : from.get_num();
  mpz_class& to_a = gt1 ? to.get_num() : to.get_den();
  mpz_class& to_b = gt1 ? to.get_den() : to.get_num();
  Rounding_Dir rdir = gt1 ? dir : inverse(dir);
  mul_2exp<To_Policy, From_Policy>(to_a, from_a,
                                   2*irrational_precision, ROUND_IGNORE);
  Result r_div
    = div<To_Policy, To_Policy, To_Policy>(to_a, to_a, from_b, rdir);
  Result r_sqrt = sqrt<To_Policy, To_Policy>(to_a, to_a, rdir);
  to_b = 1;
  mul_2exp<To_Policy, To_Policy>(to_b, to_b,
                                 irrational_precision, ROUND_IGNORE);
  to.canonicalize();
  return (r_div != V_EQ) ? r_div : r_sqrt;
}

PPL_SPECIALIZE_SQRT(sqrt_mpq, mpq_class, mpq_class)

template <typename Policy>
inline Result
input_mpq(mpq_class& to, std::istream& is, Rounding_Dir dir) {
  Result r = input_mpq(to, is);
  Result_Class c = result_class(r);
  switch (c) {
  case VC_MINUS_INFINITY:
  case VC_PLUS_INFINITY:
    return assign_special<Policy>(to, c, dir);
  case VC_NAN:
    return assign_nan<Policy>(to, r);
  default:
    return r;
  }
}

PPL_SPECIALIZE_INPUT(input_mpq, mpq_class)

template <typename Policy>
inline Result
output_mpq(std::ostream& os,
           const mpq_class& from,
           const Numeric_Format&,
           Rounding_Dir) {
  os << from;
  return V_EQ;
}

PPL_SPECIALIZE_OUTPUT(output_mpq, mpq_class)

} // namespace Checked

//! Returns the precision parameter used for irrational calculations.
inline unsigned
irrational_precision() {
  return Checked::irrational_precision;
}

//! Sets the precision parameter used for irrational calculations.
/*! The lesser between numerator and denominator is limited to 2**\p p.

  If \p p is less than or equal to <CODE>INT_MAX</CODE>, sets the
  precision parameter used for irrational calculations to \p p.

  \exception std::invalid_argument
  Thrown if \p p is greater than <CODE>INT_MAX</CODE>.
*/
inline void
set_irrational_precision(const unsigned p) {
  if (p <= INT_MAX)
    Checked::irrational_precision = p;
  else
    throw std::invalid_argument("PPL::set_irrational_precision(p)"
                                " with p > INT_MAX");
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_checked_mpq_inlines_hh)
