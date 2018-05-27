/* Specialized "checked" functions for GMP's mpz_class numbers.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_checked_mpz_inlines_hh
#define PPL_checked_mpz_inlines_hh 1

#include <sstream>

namespace Parma_Polyhedra_Library {

namespace Checked {

template <typename Policy>
inline Result
round_lt_mpz(mpz_class& to, Rounding_Dir dir) {
  if (round_down(dir)) {
    --to;
    return V_GT;
  }
  return V_LT;
}

template <typename Policy>
inline Result
round_gt_mpz(mpz_class& to, Rounding_Dir dir) {
  if (round_up(dir)) {
    ++to;
    return V_LT;
  }
  return V_GT;
}

#ifdef PPL_HAVE_TYPEOF
//! Type of the _mp_size field of GMP's __mpz_struct.
typedef typeof(__mpz_struct()._mp_size) mp_size_field_t;
#else
//! This is assumed to be the type of the _mp_size field of GMP's __mpz_struct.
typedef int mp_size_field_t;
#endif

inline mp_size_field_t
get_mp_size(const mpz_class &v) {
  return v.get_mpz_t()->_mp_size;
}

inline void
set_mp_size(mpz_class &v, mp_size_field_t size) {
  v.get_mpz_t()->_mp_size = size;
}

template <typename Policy>
inline Result
classify_mpz(const mpz_class& v, bool nan, bool inf, bool sign) {
  if (Policy::has_nan || Policy::has_infinity) {
    mp_size_field_t s = get_mp_size(v);
    if (Policy::has_nan
	&& (nan || sign)
	&& s == C_Integer<mp_size_field_t>::min + 1)
      return V_NAN;
    if (!inf && !sign)
      return V_LGE;
    if (Policy::has_infinity) {
      if (s == C_Integer<mp_size_field_t>::min)
	return inf ? V_EQ_MINUS_INFINITY : V_LT;
      if (s == C_Integer<mp_size_field_t>::max)
	return inf ? V_EQ_PLUS_INFINITY : V_GT;
    }
  }
  if (sign)
    return static_cast<Result>(sgn<Policy>(v));
  return V_LGE;
}

PPL_SPECIALIZE_CLASSIFY(classify_mpz, mpz_class)

template <typename Policy>
inline bool
is_nan_mpz(const mpz_class& v) {
  return Policy::has_nan
    && get_mp_size(v) == C_Integer<mp_size_field_t>::min + 1;
}

PPL_SPECIALIZE_IS_NAN(is_nan_mpz, mpz_class)

template <typename Policy>
inline bool
is_minf_mpz(const mpz_class& v) {
  return Policy::has_infinity
    && get_mp_size(v) == C_Integer<mp_size_field_t>::min;
}

PPL_SPECIALIZE_IS_MINF(is_minf_mpz, mpz_class)

template <typename Policy>
inline bool
is_pinf_mpz(const mpz_class& v) {
  return Policy::has_infinity
    && get_mp_size(v) == C_Integer<mp_size_field_t>::max;
}

PPL_SPECIALIZE_IS_PINF(is_pinf_mpz, mpz_class)

template <typename Policy>
inline bool
is_int_mpz(const mpz_class& v) {
  return !is_nan<Policy>(v);
}

PPL_SPECIALIZE_IS_INT(is_int_mpz, mpz_class)

template <typename Policy>
inline Result
assign_special_mpz(mpz_class& v, Result_Class c, Rounding_Dir) {
  switch (c) {
  case VC_NAN:
    if (Policy::has_nan)
      set_mp_size(v, C_Integer<mp_size_field_t>::min + 1);
    return V_NAN;
  case VC_MINUS_INFINITY:
    if (Policy::has_infinity) {
      set_mp_size(v, C_Integer<mp_size_field_t>::min);
      return V_EQ_MINUS_INFINITY;
    }
    return V_EQ_MINUS_INFINITY | V_UNREPRESENTABLE;
  case VC_PLUS_INFINITY:
    if (Policy::has_infinity) {
      set_mp_size(v, C_Integer<mp_size_field_t>::max);
      return V_EQ_PLUS_INFINITY;
    }
    return V_EQ_PLUS_INFINITY | V_UNREPRESENTABLE;
  default:
    PPL_UNREACHABLE;
    return V_NAN;
  }
}

PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_mpz, mpz_class)

template <typename To_Policy, typename From_Policy>
inline void
copy_mpz(mpz_class& to, const mpz_class& from) {
  if (is_nan_mpz<From_Policy>(from))
    PPL_ASSERT(To_Policy::has_nan);
  else if (is_minf_mpz<From_Policy>(from) || is_pinf_mpz<From_Policy>(from))
    PPL_ASSERT(To_Policy::has_infinity);
  else {
    to = from;
    return;
  }
  set_mp_size(to, get_mp_size(from));
}

PPL_SPECIALIZE_COPY(copy_mpz, mpz_class)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
construct_mpz_base(mpz_class& to, const From from, Rounding_Dir) {
    new (&to) mpz_class(from);
    return V_EQ;
}

PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, char)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, signed char)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, signed short)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, signed int)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, signed long)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, unsigned char)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, unsigned short)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, unsigned int)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_base, mpz_class, unsigned long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
construct_mpz_float(mpz_class& to, const From& from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return construct_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
  if (round_not_requested(dir)) {
    new (&to) mpz_class(from);
    return V_LGE;
  }
  From n = rint(from);
  new (&to) mpz_class(n);
  if (from == n)
    return V_EQ;
  if (from < 0)
    return round_lt_mpz<To_Policy>(to, dir);
  else
    return round_gt_mpz<To_Policy>(to, dir);
}

PPL_SPECIALIZE_CONSTRUCT(construct_mpz_float, mpz_class, float)
PPL_SPECIALIZE_CONSTRUCT(construct_mpz_float, mpz_class, double)

PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, char)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, signed char)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, signed short)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, signed int)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, signed long)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, unsigned char)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, unsigned short)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, unsigned int)
PPL_SPECIALIZE_ASSIGN(assign_exact, mpz_class, unsigned long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpz_signed_int(mpz_class& to, const From from, Rounding_Dir) {
  if (sizeof(From) <= sizeof(signed long))
    to = static_cast<signed long>(from);
  else {
    mpz_ptr m = to.get_mpz_t();
    if (from >= 0)
      mpz_import(m, 1, 1, sizeof(From), 0, 0, &from);
    else {
      From n = -from;
      mpz_import(m, 1, 1, sizeof(From), 0, 0, &n);
      mpz_neg(m, m);
    }
  }
  return V_EQ;
}

PPL_SPECIALIZE_ASSIGN(assign_mpz_signed_int, mpz_class, signed long long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpz_unsigned_int(mpz_class& to, const From from, Rounding_Dir) {
  if (sizeof(From) <= sizeof(unsigned long))
    to = static_cast<unsigned long>(from);
  else
    mpz_import(to.get_mpz_t(), 1, 1, sizeof(From), 0, 0, &from);
  return V_EQ;
}

PPL_SPECIALIZE_ASSIGN(assign_mpz_unsigned_int, mpz_class, unsigned long long)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpz_float(mpz_class& to, const From from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
  if (round_not_requested(dir)) {
    to = from;
    return V_LGE;
  }
  From i_from = rint(from);
  to = i_from;
  if (from == i_from)
    return V_EQ;
  if (round_direct(ROUND_UP))
    return round_lt_mpz<To_Policy>(to, dir);
  if (round_direct(ROUND_DOWN))
    return round_gt_mpz<To_Policy>(to, dir);
  if (from < i_from)
    return round_lt_mpz<To_Policy>(to, dir);
  if (from > i_from)
    return round_gt_mpz<To_Policy>(to, dir);
  PPL_UNREACHABLE;
  return V_NAN;
}

PPL_SPECIALIZE_ASSIGN(assign_mpz_float, mpz_class, float)
PPL_SPECIALIZE_ASSIGN(assign_mpz_float, mpz_class, double)

template <typename To_Policy, typename From_Policy, typename From>
inline Result
assign_mpz_long_double(mpz_class& to, const From& from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
  // FIXME: this is an incredibly inefficient implementation!
  std::stringstream ss;
  output<From_Policy>(ss, from, Numeric_Format(), dir);
  PPL_DIRTY_TEMP(mpq_class, tmp);
#ifndef NDEBUG
  Result r =
#endif
    input_mpq(tmp, ss);
  PPL_ASSERT(r == V_EQ);
  return assign<To_Policy, From_Policy>(to, tmp, dir);
}

PPL_SPECIALIZE_ASSIGN(assign_mpz_long_double, mpz_class, long double)

template <typename To_Policy, typename From_Policy>
inline Result
assign_mpz_mpq(mpz_class& to, const mpq_class& from, Rounding_Dir dir) {
  if (round_not_needed(dir)) {
    to = from.get_num();
    return V_LGE;
  }
  if (round_ignore(dir)) {
    to = from;
    return V_LGE;
  }
  mpz_srcptr n = from.get_num().get_mpz_t();
  mpz_srcptr d = from.get_den().get_mpz_t();
  if (round_down(dir)) {
    mpz_fdiv_q(to.get_mpz_t(), n, d);
    if (round_strict_relation(dir))
      return (mpz_divisible_p(n, d) != 0) ? V_EQ : V_GT;
    return V_GE;
  }
  else {
    PPL_ASSERT(round_up(dir));
    mpz_cdiv_q(to.get_mpz_t(), n, d);
    if (round_strict_relation(dir))
      return (mpz_divisible_p(n, d) != 0) ? V_EQ : V_LT;
    return V_LE;
  }
}

PPL_SPECIALIZE_ASSIGN(assign_mpz_mpq, mpz_class, mpq_class)

PPL_SPECIALIZE_FLOOR(assign_exact, mpz_class, mpz_class)
PPL_SPECIALIZE_CEIL(assign_exact, mpz_class, mpz_class)
PPL_SPECIALIZE_TRUNC(assign_exact, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
neg_mpz(mpz_class& to, const mpz_class& from, Rounding_Dir) {
  mpz_neg(to.get_mpz_t(), from.get_mpz_t());
  return V_EQ;
}

PPL_SPECIALIZE_NEG(neg_mpz, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
add_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y, Rounding_Dir) {
  to = x + y;
  return V_EQ;
}

PPL_SPECIALIZE_ADD(add_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
sub_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y, Rounding_Dir) {
  to = x - y;
  return V_EQ;
}

PPL_SPECIALIZE_SUB(sub_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
mul_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y, Rounding_Dir) {
  to = x * y;
  return V_EQ;
}

PPL_SPECIALIZE_MUL(mul_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
div_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y,
	Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_div_zero, ::sgn(y) == 0)) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  mpz_srcptr n = x.get_mpz_t();
  mpz_srcptr d = y.get_mpz_t();
  if (round_not_needed(dir)) {
    mpz_divexact(to.get_mpz_t(), n, d);
    return V_LGE;
  }
  if (round_ignore(dir)) {
    mpz_cdiv_q(to.get_mpz_t(), n, d);
    return V_LE;
  }
  if (round_down(dir)) {
    mpz_fdiv_q(to.get_mpz_t(), n, d);
    if (round_strict_relation(dir))
      return (mpz_divisible_p(n, d) != 0) ? V_EQ : V_GT;
    return V_GE;
  }
  else {
    PPL_ASSERT(round_up(dir));
    mpz_cdiv_q(to.get_mpz_t(), n, d);
    if (round_strict_relation(dir))
      return (mpz_divisible_p(n, d) != 0) ? V_EQ : V_LT;
    return V_LE;
  }
}

PPL_SPECIALIZE_DIV(div_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
idiv_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y,
	Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, ::sgn(y) == 0)) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  mpz_srcptr n = x.get_mpz_t();
  mpz_srcptr d = y.get_mpz_t();
  mpz_tdiv_q(to.get_mpz_t(), n, d);
  return V_EQ;
}

PPL_SPECIALIZE_IDIV(idiv_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
rem_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y, Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, ::sgn(y) == 0)) {
    return assign_nan<To_Policy>(to, V_MOD_ZERO);
  }
  to = x % y;
  return V_EQ;
}

PPL_SPECIALIZE_REM(rem_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
add_2exp_mpz(mpz_class& to, const mpz_class& x, unsigned int exp,
             Rounding_Dir) {
  PPL_DIRTY_TEMP(mpz_class, v);
  v = 1;
  mpz_mul_2exp(v.get_mpz_t(), v.get_mpz_t(), exp);
  to = x + v;
  return V_EQ;
}

PPL_SPECIALIZE_ADD_2EXP(add_2exp_mpz, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
sub_2exp_mpz(mpz_class& to, const mpz_class& x, unsigned int exp,
             Rounding_Dir) {
  PPL_DIRTY_TEMP(mpz_class, v);
  v = 1;
  mpz_mul_2exp(v.get_mpz_t(), v.get_mpz_t(), exp);
  to = x - v;
  return V_EQ;
}

PPL_SPECIALIZE_SUB_2EXP(sub_2exp_mpz, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
mul_2exp_mpz(mpz_class& to, const mpz_class& x, unsigned int exp,
             Rounding_Dir) {
  mpz_mul_2exp(to.get_mpz_t(), x.get_mpz_t(), exp);
  return V_EQ;
}

PPL_SPECIALIZE_MUL_2EXP(mul_2exp_mpz, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
div_2exp_mpz(mpz_class& to, const mpz_class& x, unsigned int exp,
             Rounding_Dir dir) {
  mpz_srcptr n = x.get_mpz_t();
  if (round_not_requested(dir)) {
    mpz_tdiv_q_2exp(to.get_mpz_t(), x.get_mpz_t(), exp);
    return V_LGE;
  }
  if (round_down(dir)) {
    mpz_fdiv_q_2exp(to.get_mpz_t(), n, exp);
    if (round_strict_relation(dir))
      return (mpz_divisible_2exp_p(n, exp) != 0) ? V_EQ : V_GT;
    return V_GE;
  }
  else {
    PPL_ASSERT(round_up(dir));
    mpz_cdiv_q_2exp(to.get_mpz_t(), n, exp);
    if (round_strict_relation(dir))
      return (mpz_divisible_2exp_p(n, exp) != 0) ? V_EQ : V_LT;
    return V_LE;
  }
}

PPL_SPECIALIZE_DIV_2EXP(div_2exp_mpz, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
smod_2exp_mpz(mpz_class& to, const mpz_class& x, unsigned int exp,
	      Rounding_Dir) {
  if (mpz_tstbit(x.get_mpz_t(), exp - 1) != 0)
    mpz_cdiv_r_2exp(to.get_mpz_t(), x.get_mpz_t(), exp);
  else
    mpz_fdiv_r_2exp(to.get_mpz_t(), x.get_mpz_t(), exp);
  return V_EQ;
}

PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_mpz, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
umod_2exp_mpz(mpz_class& to, const mpz_class& x, unsigned int exp,
	      Rounding_Dir) {
  mpz_fdiv_r_2exp(to.get_mpz_t(), x.get_mpz_t(), exp);
  return V_EQ;
}

PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_mpz, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
abs_mpz(mpz_class& to, const mpz_class& from, Rounding_Dir) {
  to = abs(from);
  return V_EQ;
}

PPL_SPECIALIZE_ABS(abs_mpz, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
add_mul_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y,
	    Rounding_Dir) {
  mpz_addmul(to.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t());
  return V_EQ;
}

PPL_SPECIALIZE_ADD_MUL(add_mul_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
sub_mul_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y,
	    Rounding_Dir) {
  mpz_submul(to.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t());
  return V_EQ;
}

PPL_SPECIALIZE_SUB_MUL(sub_mul_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
gcd_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y, Rounding_Dir) {
  mpz_gcd(to.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t());
  return V_EQ;
}

PPL_SPECIALIZE_GCD(gcd_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
gcdext_mpz(mpz_class& to, mpz_class& s, mpz_class& t,
	   const mpz_class& x, const mpz_class& y,
	   Rounding_Dir) {
  mpz_gcdext(to.get_mpz_t(), s.get_mpz_t(), t.get_mpz_t(),
	     x.get_mpz_t(), y.get_mpz_t());
  return V_EQ;
}

PPL_SPECIALIZE_GCDEXT(gcdext_mpz, mpz_class, mpz_class, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From1_Policy, typename From2_Policy>
inline Result
lcm_mpz(mpz_class& to, const mpz_class& x, const mpz_class& y, Rounding_Dir) {
  mpz_lcm(to.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t());
  return V_EQ;
}

PPL_SPECIALIZE_LCM(lcm_mpz, mpz_class, mpz_class, mpz_class)

template <typename To_Policy, typename From_Policy>
inline Result
sqrt_mpz(mpz_class& to, const mpz_class& from, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_sqrt_neg, from < 0)) {
    return assign_nan<To_Policy>(to, V_SQRT_NEG);
  }
  if (round_not_requested(dir)) {
    to = sqrt(from);
    return V_GE;
  }
  PPL_DIRTY_TEMP(mpz_class, r);
  mpz_sqrtrem(to.get_mpz_t(), r.get_mpz_t(), from.get_mpz_t());
  if (r == 0)
    return V_EQ;
  return round_gt_mpz<To_Policy>(to, dir);
}

PPL_SPECIALIZE_SQRT(sqrt_mpz, mpz_class, mpz_class)

template <typename Policy, typename Type>
inline Result_Relation
sgn_mp(const Type& x) {
  int i = ::sgn(x);
  return (i > 0) ? VR_GT : ((i < 0) ? VR_LT : VR_EQ);
}

PPL_SPECIALIZE_SGN(sgn_mp, mpz_class)
PPL_SPECIALIZE_SGN(sgn_mp, mpq_class)

template <typename Policy1, typename Policy2, typename Type>
inline Result_Relation
cmp_mp(const Type& x, const Type& y) {
  int i = ::cmp(x, y);
  return (i > 0) ? VR_GT : ((i < 0) ? VR_LT : VR_EQ);
}

PPL_SPECIALIZE_CMP(cmp_mp, mpz_class, mpz_class)
PPL_SPECIALIZE_CMP(cmp_mp, mpq_class, mpq_class)

template <typename Policy>
inline Result
output_mpz(std::ostream& os, const mpz_class& from, const Numeric_Format&,
	   Rounding_Dir) {
  os << from;
  return V_EQ;
}

PPL_SPECIALIZE_INPUT(input_generic, mpz_class)
PPL_SPECIALIZE_OUTPUT(output_mpz, mpz_class)

} // namespace Checked

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_checked_mpz_inlines_hh)
