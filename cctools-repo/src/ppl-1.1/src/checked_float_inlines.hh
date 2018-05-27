/* Specialized "checked" functions for native floating-point numbers.
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

#ifndef PPL_checked_float_inlines_hh
#define PPL_checked_float_inlines_hh 1

#include "compiler.hh"
#include <cmath>

namespace Parma_Polyhedra_Library {

namespace Checked {

inline float
multiply_add(float x, float y, float z) {
#if PPL_HAVE_DECL_FMAF && defined(FP_FAST_FMAF) \
  && !defined(__alpha) && !defined(__FreeBSD__)
  return fmaf(x, y, z);
#else
  return x*y + z;
#endif
}

inline double
multiply_add(double x, double y, double z) {
#if PPL_HAVE_DECL_FMA && defined(FP_FAST_FMA) \
  && !defined(__alpha) && !defined(__FreeBSD__)
  return fma(x, y, z);
#else
  return x*y + z;
#endif
}

inline long double
multiply_add(long double x, long double y, long double z) {
#if PPL_HAVE_DECL_FMAL && defined(FP_FAST_FMAL) \
  && !defined(__alpha) && !defined(__FreeBSD__)
  return fmal(x, y, z);
#else
  return x*y + z;
#endif
}

#if PPL_HAVE_DECL_RINTF
inline float
round_to_integer(float x) {
  return rintf(x);
}
#endif

inline double
round_to_integer(double x) {
  return rint(x);
}

#if PPL_HAVE_DECL_RINTL
inline long double
round_to_integer(long double x) {
  return rintl(x);
}
#elif !PPL_CXX_PROVIDES_PROPER_LONG_DOUBLE
// If proper long doubles are not provided, this is most likely
// because long double and double are the same type: use rint().
inline long double
round_to_integer(long double x) {
  return rint(x);
}
#elif defined(__i386__) && (defined(__GNUC__) || defined(__INTEL_COMPILER))
// On Cygwin, we have proper long doubles but rintl() is not defined:
// luckily, one machine instruction is enough to save the day.
inline long double
round_to_integer(long double x) {
  long double i;
  __asm__ ("frndint" : "=t" (i) : "0" (x));
  return i;
}
#endif

inline bool
fpu_direct_rounding(Rounding_Dir dir) {
  return round_direct(dir) || round_not_requested(dir);
}

inline bool
fpu_inverse_rounding(Rounding_Dir dir) {
  return round_inverse(dir);
}

// The FPU mode is "round down".
//
// The result of the rounded down multiplication is thus computed directly.
//
//   a = 0.3
//   b = 0.1
//   c_i = a * b = 0.03
//   c = c_i = 0.0
//
// To obtain the result of the rounded up multiplication
// we do -(-a * b).
//
//   a = 0.3
//   b = 0.1
//   c_i = -a * b = -0.03
//
// Here c_i should be forced to lose excess precision, otherwise the
// FPU will truncate using the rounding mode in force, which is "round down".
//
//   c_i = -c_i = 0.03
//   c = c_i = 0.0
//
// Wrong result: we should have obtained c = 0.1.

inline void
limit_precision(const float& v) {
  PPL_CC_FLUSH(v);
}

inline void
limit_precision(const double& v) {
  PPL_CC_FLUSH(v);
}

inline void
limit_precision(const long double&) {
}

template <typename Policy, typename T>
inline Result
classify_float(const T v, bool nan, bool inf, bool sign) {
  Float<T> f(v);
  if ((nan || sign) && CHECK_P(Policy::has_nan, f.u.binary.is_nan()))
    return V_NAN;
  if (inf) {
    if (Policy::has_infinity) {
      int sign_inf = f.u.binary.inf_sign();
      if (sign_inf < 0)
        return V_EQ_MINUS_INFINITY;
      if (sign_inf > 0)
        return V_EQ_PLUS_INFINITY;
    }
    else
      PPL_ASSERT(f.u.binary.inf_sign() == 0);
  }
  if (sign) {
    if (v < 0)
      return V_LT;
    if (v > 0)
      return V_GT;
    return V_EQ;
  }
  return V_LGE;
}

template <typename Policy, typename T>
inline bool
is_nan_float(const T v) {
  Float<T> f(v);
  return CHECK_P(Policy::has_nan, f.u.binary.is_nan());
}

template <typename Policy, typename T>
inline bool
is_inf_float(const T v) {
  Float<T> f(v);
  return CHECK_P(Policy::has_infinity, (f.u.binary.inf_sign() != 0));
}
template <typename Policy, typename T>
inline bool
is_minf_float(const T v) {
  Float<T> f(v);
  return CHECK_P(Policy::has_infinity, (f.u.binary.inf_sign() < 0));
}

template <typename Policy, typename T>
inline bool
is_pinf_float(const T v) {
  Float<T> f(v);
  return CHECK_P(Policy::has_infinity, (f.u.binary.inf_sign() > 0));
}


template <typename Policy, typename T>
inline bool
is_int_float(const T v) {
  return round_to_integer(v) == v;
}

template <typename Policy, typename T>
inline Result
assign_special_float(T& v, Result_Class c, Rounding_Dir) {
  PPL_ASSERT(c == VC_MINUS_INFINITY || c == VC_PLUS_INFINITY || c == VC_NAN);
  switch (c) {
  case VC_MINUS_INFINITY:
    v = -HUGE_VAL;
    return V_EQ_MINUS_INFINITY;
  case VC_PLUS_INFINITY:
    v = HUGE_VAL;
    return V_EQ_PLUS_INFINITY;
  case VC_NAN:
    v = PPL_NAN;
    return V_NAN;
  default:
    PPL_UNREACHABLE;
    return V_NAN | V_UNREPRESENTABLE;
  }
}

template <typename T>
inline void
pred_float(T& v) {
  Float<T> f(v);
  PPL_ASSERT(!f.u.binary.is_nan());
  PPL_ASSERT(f.u.binary.inf_sign() >= 0);
  if (f.u.binary.zero_sign() > 0) {
    f.u.binary.negate();
    f.u.binary.inc();
  }
  else if (f.u.binary.sign_bit()) {
    f.u.binary.inc();
  }
  else {
    f.u.binary.dec();
  }
  v = f.value();
}

template <typename T>
inline void
succ_float(T& v) {
  Float<T> f(v);
  PPL_ASSERT(!f.u.binary.is_nan());
  PPL_ASSERT(f.u.binary.inf_sign() <= 0);
  if (f.u.binary.zero_sign() < 0) {
    f.u.binary.negate();
    f.u.binary.inc();
  }
  else if (!f.u.binary.sign_bit()) {
    f.u.binary.inc();
  }
  else {
    f.u.binary.dec();
  }
  v = f.value();
}

template <typename Policy, typename To>
inline Result
round_lt_float(To& to, Rounding_Dir dir) {
  if (round_down(dir)) {
    pred_float(to);
    return V_GT;
  }
  return V_LT;
}

template <typename Policy, typename To>
inline Result
round_gt_float(To& to, Rounding_Dir dir) {
  if (round_up(dir)) {
    succ_float(to);
    return V_LT;
  }
  return V_GT;
}


template <typename Policy>
inline void
prepare_inexact(Rounding_Dir dir) {
  if (Policy::fpu_check_inexact
      && !round_not_needed(dir) && round_strict_relation(dir))
    fpu_reset_inexact();
}

template <typename Policy>
inline Result
result_relation(Rounding_Dir dir) {
  if (Policy::fpu_check_inexact
      && !round_not_needed(dir) && round_strict_relation(dir)) {
    switch (fpu_check_inexact()) {
    case 0:
      return V_EQ;
    case -1:
      goto unknown;
    case 1:
      break;
    }
    switch (round_dir(dir)) {
    case ROUND_DOWN:
      return V_GT;
    case ROUND_UP:
      return V_LT;
    default:
      return V_NE;
    }
  }
  else {
  unknown:
    switch (round_dir(dir)) {
    case ROUND_DOWN:
      return V_GE;
    case ROUND_UP:
      return V_LE;
    default:
      return V_LGE;
    }
  }
}

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_float_float_exact(To& to, const From from, Rounding_Dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  to = from;
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_float_float_inexact(To& to, const From from, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = from;
  else if (fpu_inverse_rounding(dir)) {
    From tmp = -from;
    to = tmp;
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    limit_precision(from);
    to = from;
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  return result_relation<To_Policy>(dir);
}

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_float_float(To& to, const From from, Rounding_Dir dir) {
  if (sizeof(From) > sizeof(To))
    return assign_float_float_inexact<To_Policy, From_Policy>(to, from, dir);
  else
    return assign_float_float_exact<To_Policy, From_Policy>(to, from, dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
floor_float(Type& to, const Type from, Rounding_Dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  if (fpu_direct_rounding(ROUND_DOWN))
    to = round_to_integer(from);
  else if (fpu_inverse_rounding(ROUND_DOWN)) {
    to = round_to_integer(-from);
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(ROUND_DOWN));
    limit_precision(from);
    to = round_to_integer(from);
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
ceil_float(Type& to, const Type from, Rounding_Dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  if (fpu_direct_rounding(ROUND_UP))
    to = round_to_integer(from);
  else if (fpu_inverse_rounding(ROUND_UP)) {
    to = round_to_integer(-from);
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(ROUND_UP));
    limit_precision(from);
    to = round_to_integer(from);
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
trunc_float(Type& to, const Type from, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  if (from >= 0)
    return floor<To_Policy, From_Policy>(to, from, dir);
  else
    return ceil<To_Policy, From_Policy>(to, from, dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
neg_float(Type& to, const Type from, Rounding_Dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  to = -from;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
add_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_inf_add_inf
      && is_inf_float<From1_Policy>(x) && x == -y) {
    return assign_nan<To_Policy>(to, V_INF_ADD_INF);
  }
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = x + y;
  else if (fpu_inverse_rounding(dir)) {
    to = -x - y;
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    limit_precision(x);
    limit_precision(y);
    to = x + y;
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  if (To_Policy::fpu_check_nan_result && is_nan<To_Policy>(to))
    return V_NAN;
  return result_relation<To_Policy>(dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
sub_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_inf_sub_inf
      && is_inf_float<From1_Policy>(x) && x == y) {
    return assign_nan<To_Policy>(to, V_INF_SUB_INF);
  }
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = x - y;
  else if (fpu_inverse_rounding(dir)) {
    to = y - x;
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    limit_precision(x);
    limit_precision(y);
    to = x - y;
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  if (To_Policy::fpu_check_nan_result && is_nan<To_Policy>(to))
    return V_NAN;
  return result_relation<To_Policy>(dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
mul_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_inf_mul_zero
      && ((x == 0 && is_inf_float<From2_Policy>(y))
          ||
          (y == 0 && is_inf_float<From1_Policy>(x)))) {
    return assign_nan<To_Policy>(to, V_INF_MUL_ZERO);
  }
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = x * y;
  else if (fpu_inverse_rounding(dir)) {
    to = x * -y;
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    limit_precision(x);
    limit_precision(y);
    to = x * y;
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  if (To_Policy::fpu_check_nan_result && is_nan<To_Policy>(to))
    return V_NAN;
  return result_relation<To_Policy>(dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
div_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_inf_div_inf
      && is_inf_float<From1_Policy>(x) && is_inf_float<From2_Policy>(y)) {
    return assign_nan<To_Policy>(to, V_INF_DIV_INF);
  }
  if (To_Policy::check_div_zero && y == 0) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = x / y;
  else if (fpu_inverse_rounding(dir)) {
    to = x / -y;
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    limit_precision(x);
    limit_precision(y);
    to = x / y;
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  if (To_Policy::fpu_check_nan_result && is_nan<To_Policy>(to))
    return V_NAN;
  return result_relation<To_Policy>(dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
idiv_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  Type temp;
  // The inexact check is useless
  dir = round_dir(dir);
  Result r = div<To_Policy, From1_Policy, From2_Policy>(temp, x, y, dir);
  if (result_class(r) != VC_NORMAL) {
    to = temp;
    return r;
  }
  Result r1 = trunc<To_Policy, To_Policy>(to, temp, ROUND_NOT_NEEDED);
  PPL_ASSERT(r1 == V_EQ);
  if (r == V_EQ || to != temp)
    return r1;
  // FIXME: Prove that it is impossible to return a strict relation
  return (dir == ROUND_UP) ? V_LE : V_GE;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
rem_float(Type& to, const Type x, const Type y, Rounding_Dir) {
  if (To_Policy::check_inf_mod && is_inf_float<From1_Policy>(x)) {
    return assign_nan<To_Policy>(to, V_INF_MOD);
  }
  if (To_Policy::check_div_zero && y == 0) {
    return assign_nan<To_Policy>(to, V_MOD_ZERO);
  }
  to = std::fmod(x, y);
  if (To_Policy::fpu_check_nan_result && is_nan<To_Policy>(to))
    return V_NAN;
  return V_EQ;
}

struct Float_2exp {
  const_bool_nodef(has_nan, false);
  const_bool_nodef(has_infinity, false);
};

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
add_2exp_float(Type& to, const Type x, unsigned int exp, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(x))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  PPL_ASSERT(exp < sizeof_to_bits(sizeof(unsigned long long)));
  return
    add<To_Policy, From_Policy, Float_2exp>(to,
                                            x,
                                            Type(1ULL << exp),
                                            dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
sub_2exp_float(Type& to, const Type x, unsigned int exp, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(x))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  PPL_ASSERT(exp < sizeof_to_bits(sizeof(unsigned long long)));
  return
    sub<To_Policy, From_Policy, Float_2exp>(to,
                                            x,
                                            Type(1ULL << exp),
                                            dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
mul_2exp_float(Type& to, const Type x, unsigned int exp, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(x))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  PPL_ASSERT(exp < sizeof_to_bits(sizeof(unsigned long long)));
  return
    mul<To_Policy, From_Policy, Float_2exp>(to,
                                            x,
                                            Type(1ULL << exp),
                                            dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
div_2exp_float(Type& to, const Type x, unsigned int exp, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(x))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  PPL_ASSERT(exp < sizeof_to_bits(sizeof(unsigned long long)));
  return
    div<To_Policy, From_Policy, Float_2exp>(to,
                                            x,
                                            Type(1ULL << exp),
                                            dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
smod_2exp_float(Type& to, const Type x, unsigned int exp, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(x))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  if (To_Policy::check_inf_mod && is_inf_float<From_Policy>(x)) {
    return assign_nan<To_Policy>(to, V_INF_MOD);
  }
  PPL_ASSERT(exp < sizeof_to_bits(sizeof(unsigned long long)));
  Type m = 1ULL << exp;
  rem_float<To_Policy, From_Policy, Float_2exp>(to, x, m, ROUND_IGNORE);
  Type m2 = m / 2;
  if (to < -m2)
    return add_float<To_Policy, From_Policy, Float_2exp>(to, to, m, dir);
  else if (to >= m2)
    return sub_float<To_Policy, From_Policy, Float_2exp>(to, to, m, dir);
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
umod_2exp_float(Type& to, const Type x, unsigned int exp, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(x))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  if (To_Policy::check_inf_mod && is_inf_float<From_Policy>(x)) {
    return assign_nan<To_Policy>(to, V_INF_MOD);
  }
  PPL_ASSERT(exp < sizeof_to_bits(sizeof(unsigned long long)));
  Type m = 1ULL << exp;
  rem_float<To_Policy, From_Policy, Float_2exp>(to, x, m, ROUND_IGNORE);
  if (to < 0)
    return add_float<To_Policy, From_Policy, Float_2exp>(to, to, m, dir);
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
abs_float(Type& to, const Type from, Rounding_Dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  to = std::abs(from);
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
sqrt_float(Type& to, const Type from, Rounding_Dir dir) {
  if (To_Policy::fpu_check_nan_result && is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  if (To_Policy::check_sqrt_neg && from < 0) {
    return assign_nan<To_Policy>(to, V_SQRT_NEG);
  }
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = std::sqrt(from);
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    limit_precision(from);
    to = std::sqrt(from);
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  return result_relation<To_Policy>(dir);
}

template <typename Policy, typename Type>
inline Result_Relation
sgn_float(const Type x) {
  if (x > 0)
    return VR_GT;
  if (x < 0)
    return VR_LT;
  if (x == 0)
    return VR_EQ;
  return VR_EMPTY;
}

template <typename Policy1, typename Policy2, typename Type>
inline Result_Relation
cmp_float(const Type x, const Type y) {
  if (x > y)
    return VR_GT;
  if (x < y)
    return VR_LT;
  if (x == y)
    return VR_EQ;
  return VR_EMPTY;
}

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_float_int_inexact(To& to, const From from, Rounding_Dir dir) {
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = from;
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    to = from;
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  return result_relation<To_Policy>(dir);
}

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_float_int(To& to, const From from, Rounding_Dir dir) {
  if (sizeof_to_bits(sizeof(From)) > Float<To>::Binary::MANTISSA_BITS)
    return assign_float_int_inexact<To_Policy, From_Policy>(to, from, dir);
  else
    return assign_exact<To_Policy, From_Policy>(to, from, dir);
}

template <typename Policy, typename T>
inline Result
set_neg_overflow_float(T& to, Rounding_Dir dir) {
  switch (round_dir(dir)) {
  case ROUND_UP:
    {
      Float<T> f;
      f.u.binary.set_max(true);
      to = f.value();
      return V_LT_INF;
    }
  case ROUND_DOWN: // Fall through.
  case ROUND_IGNORE:
    to = -HUGE_VAL;
    return V_GT_MINUS_INFINITY;
  default:
    PPL_UNREACHABLE;
    return V_GT_MINUS_INFINITY;
  }
}

template <typename Policy, typename T>
inline Result
set_pos_overflow_float(T& to, Rounding_Dir dir) {
  switch (round_dir(dir)) {
  case ROUND_DOWN:
    {
      Float<T> f;
      f.u.binary.set_max(false);
      to = f.value();
      return V_GT_SUP;
    }
  case ROUND_UP: // Fall through.
  case ROUND_IGNORE:
    to = HUGE_VAL;
    return V_LT_PLUS_INFINITY;
  default:
    PPL_UNREACHABLE;
    return V_LT_PLUS_INFINITY;
  }
}

template <typename To_Policy, typename From_Policy, typename T>
inline Result
assign_float_mpz(T& to, const mpz_class& from, Rounding_Dir dir) {
  int sign = sgn(from);
  if (sign == 0) {
    to = 0;
    return V_EQ;
  }
  mpz_srcptr from_z = from.get_mpz_t();
  size_t exponent = mpz_sizeinbase(from_z, 2) - 1;
  if (exponent > size_t(Float<T>::Binary::EXPONENT_MAX)) {
    if (sign < 0)
      return set_neg_overflow_float<To_Policy>(to, dir);
    else
      return set_pos_overflow_float<To_Policy>(to, dir);
  }
  unsigned long zeroes = mpn_scan1(from_z->_mp_d, 0);
  size_t meaningful_bits = exponent - zeroes;
  mpz_t mantissa;
  mpz_init(mantissa);
  if (exponent > Float<T>::Binary::MANTISSA_BITS)
    mpz_tdiv_q_2exp(mantissa,
                    from_z,
                    exponent - Float<T>::Binary::MANTISSA_BITS);
  else
    mpz_mul_2exp(mantissa, from_z, Float<T>::Binary::MANTISSA_BITS - exponent);
  Float<T> f;
  f.u.binary.build(sign < 0, mantissa, static_cast<long>(exponent));
  mpz_clear(mantissa);
  to = f.value();
  if (meaningful_bits > Float<T>::Binary::MANTISSA_BITS) {
    if (sign < 0)
      return round_lt_float<To_Policy>(to, dir);
    else
      return round_gt_float<To_Policy>(to, dir);
  }
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename T>
inline Result
assign_float_mpq(T& to, const mpq_class& from, Rounding_Dir dir) {
  const mpz_class& numer = from.get_num();
  const mpz_class& denom = from.get_den();
  if (denom == 1)
    return assign_float_mpz<To_Policy, From_Policy>(to, numer, dir);
  mpz_srcptr numer_z = numer.get_mpz_t();
  mpz_srcptr denom_z = denom.get_mpz_t();
  int sign = sgn(numer);
  long exponent = static_cast<long>(mpz_sizeinbase(numer_z, 2))
    - static_cast<long>(mpz_sizeinbase(denom_z, 2));
  if (exponent < Float<T>::Binary::EXPONENT_MIN_DENORM) {
    to = 0;
  inexact:
    if (sign < 0)
      return round_lt_float<To_Policy>(to, dir);
    else
      return round_gt_float<To_Policy>(to, dir);
  }
  if (exponent > Float<T>::Binary::EXPONENT_MAX + 1) {
  overflow:
    if (sign < 0)
      return set_neg_overflow_float<To_Policy>(to, dir);
    else
      return set_pos_overflow_float<To_Policy>(to, dir);
  }
  unsigned int needed_bits = Float<T>::Binary::MANTISSA_BITS + 1;
  if (exponent < Float<T>::Binary::EXPONENT_MIN) {
    long diff = Float<T>::Binary::EXPONENT_MIN - exponent;
    needed_bits -= static_cast<unsigned int>(diff);
  }
  mpz_t mantissa;
  mpz_init(mantissa);
  {
    long shift = static_cast<long>(needed_bits) - exponent;
    if (shift > 0) {
      mpz_mul_2exp(mantissa, numer_z, static_cast<unsigned long>(shift));
      numer_z = mantissa;
    }
    else if (shift < 0) {
      shift = -shift;
      mpz_mul_2exp(mantissa, denom_z, static_cast<unsigned long>(shift));
      denom_z = mantissa;
    }
  }
  mpz_t r;
  mpz_init(r);
  mpz_tdiv_qr(mantissa, r, numer_z, denom_z);
  size_t bits = mpz_sizeinbase(mantissa, 2);
  bool inexact = (mpz_sgn(r) != 0);
  mpz_clear(r);
  if (bits == needed_bits + 1) {
    inexact = (inexact || mpz_odd_p(mantissa));
    mpz_tdiv_q_2exp(mantissa, mantissa, 1);
  }
  else
    --exponent;
  if (exponent > Float<T>::Binary::EXPONENT_MAX) {
    mpz_clear(mantissa);
    goto overflow;
  }
  else if (exponent < Float<T>::Binary::EXPONENT_MIN - 1) {
    // Denormalized.
    exponent = Float<T>::Binary::EXPONENT_MIN - 1;
  }
  Float<T> f;
  f.u.binary.build(sign < 0, mantissa, exponent);
  mpz_clear(mantissa);
  to = f.value();
  if (inexact)
    goto inexact;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
add_mul_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_inf_mul_zero
      && ((x == 0 && is_inf_float<From2_Policy>(y))
          ||
          (y == 0 && is_inf_float<From1_Policy>(x)))) {
    return assign_nan<To_Policy>(to, V_INF_MUL_ZERO);
  }
  // FIXME: missing check_inf_add_inf
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = multiply_add(x, y, to);
  else if (fpu_inverse_rounding(dir)) {
    to = multiply_add(-x, y, -to);
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    limit_precision(x);
    limit_precision(y);
    limit_precision(to);
    to = multiply_add(x, y, to);
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  if (To_Policy::fpu_check_nan_result && is_nan<To_Policy>(to))
    return V_NAN;
  return result_relation<To_Policy>(dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy, typename Type>
inline Result
sub_mul_float(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_inf_mul_zero
      && ((x == 0 && is_inf_float<From2_Policy>(y))
          ||
          (y == 0 && is_inf_float<From1_Policy>(x)))) {
    return assign_nan<To_Policy>(to, V_INF_MUL_ZERO);
  }
  // FIXME: missing check_inf_add_inf
  prepare_inexact<To_Policy>(dir);
  if (fpu_direct_rounding(dir))
    to = multiply_add(x, -y, to);
  else if (fpu_inverse_rounding(dir)) {
    to = multiply_add(x, y, -to);
    limit_precision(to);
    to = -to;
  }
  else {
    fpu_rounding_control_word_type old
      = fpu_save_rounding_direction(round_fpu_dir(dir));
    limit_precision(x);
    limit_precision(y);
    limit_precision(to);
    to = multiply_add(x, -y, to);
    limit_precision(to);
    fpu_restore_rounding_direction(old);
  }
  if (To_Policy::fpu_check_nan_result && is_nan<To_Policy>(to))
    return V_NAN;
  return result_relation<To_Policy>(dir);
}

template <typename From>
inline void
assign_mpq_numeric_float(mpq_class& to, const From from) {
  to = from;
}

template <>
inline void
assign_mpq_numeric_float(mpq_class& to, const long double from) {
  to = 0;
  if (from == 0.0L)
    return;
  mpz_class& num = to.get_num();
  mpz_class& den = to.get_den();
  int exp;
  long double n = std::frexp(from, &exp);
  bool neg = false;
  if (n < 0.0L) {
    neg = true;
    n = -n;
  }
  const long double mult = static_cast<long double>(ULONG_MAX) + 1.0L;
  const unsigned int bits = sizeof(unsigned long) * CHAR_BIT;
  while (true) {
    n *= mult;
    exp -= bits;
    const long double intpart = std::floor(n);
    num += static_cast<unsigned long>(intpart);
    n -= intpart;
    if (n == 0.0L)
      break;
    num <<= bits;
  }
  if (exp < 0)
    den <<= -exp;
  else
    num <<= exp;
  if (neg)
    to = -to;
  to.canonicalize();
}

template <typename Policy, typename Type>
inline Result
output_float(std::ostream& os, const Type from, const Numeric_Format&,
             Rounding_Dir) {
  if (from == 0)
    os << "0";
  else if (is_minf<Policy>(from))
    os << "-inf";
  else if (is_pinf<Policy>(from))
    os << "+inf";
  else if (is_nan<Policy>(from))
    os << "nan";
  else {
    mpq_class q;
    assign_mpq_numeric_float(q, from);
    std::string s = float_mpq_to_string(q);
    os << s;
  }
  return V_EQ;
}

#if PPL_SUPPORTED_FLOAT
PPL_SPECIALIZE_ASSIGN(assign_float_float_exact, float, float)
#if PPL_SUPPORTED_DOUBLE
PPL_SPECIALIZE_ASSIGN(assign_float_float, float, double)
PPL_SPECIALIZE_ASSIGN(assign_float_float_exact, double, float)
#endif
#if PPL_SUPPORTED_LONG_DOUBLE
PPL_SPECIALIZE_ASSIGN(assign_float_float, float, long double)
PPL_SPECIALIZE_ASSIGN(assign_float_float_exact, long double, float)
#endif
#endif

#if PPL_SUPPORTED_DOUBLE
PPL_SPECIALIZE_ASSIGN(assign_float_float_exact, double, double)
#if PPL_SUPPORTED_LONG_DOUBLE
PPL_SPECIALIZE_ASSIGN(assign_float_float, double, long double)
PPL_SPECIALIZE_ASSIGN(assign_float_float_exact, long double, double)
#endif
#endif

#if PPL_SUPPORTED_LONG_DOUBLE
PPL_SPECIALIZE_ASSIGN(assign_float_float_exact, long double, long double)
#endif

#if PPL_SUPPORTED_FLOAT
PPL_SPECIALIZE_CLASSIFY(classify_float, float)
PPL_SPECIALIZE_IS_NAN(is_nan_float, float)
PPL_SPECIALIZE_IS_MINF(is_minf_float, float)
PPL_SPECIALIZE_IS_PINF(is_pinf_float, float)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_float, float)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, signed char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, signed short)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, signed int)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, signed long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, signed long long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, unsigned char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, unsigned short)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, unsigned int)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, unsigned long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, float, unsigned long long)
PPL_SPECIALIZE_ASSIGN(assign_float_mpz, float, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_float_mpq, float, mpq_class)
PPL_SPECIALIZE_COPY(copy_generic, float)
PPL_SPECIALIZE_IS_INT(is_int_float, float)
PPL_SPECIALIZE_FLOOR(floor_float, float, float)
PPL_SPECIALIZE_CEIL(ceil_float, float, float)
PPL_SPECIALIZE_TRUNC(trunc_float, float, float)
PPL_SPECIALIZE_NEG(neg_float, float, float)
PPL_SPECIALIZE_ABS(abs_float, float, float)
PPL_SPECIALIZE_ADD(add_float, float, float, float)
PPL_SPECIALIZE_SUB(sub_float, float, float, float)
PPL_SPECIALIZE_MUL(mul_float, float, float, float)
PPL_SPECIALIZE_DIV(div_float, float, float, float)
PPL_SPECIALIZE_REM(rem_float, float, float, float)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_float, float, float)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_float, float, float)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_float, float, float)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_float, float, float)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_float, float, float)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_float, float, float)
PPL_SPECIALIZE_SQRT(sqrt_float, float, float)
PPL_SPECIALIZE_GCD(gcd_exact, float, float, float)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, float, float, float, float, float)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, float, float, float)
PPL_SPECIALIZE_SGN(sgn_float, float)
PPL_SPECIALIZE_CMP(cmp_float, float, float)
PPL_SPECIALIZE_ADD_MUL(add_mul_float, float, float, float)
PPL_SPECIALIZE_SUB_MUL(sub_mul_float, float, float, float)
PPL_SPECIALIZE_INPUT(input_generic, float)
PPL_SPECIALIZE_OUTPUT(output_float, float)
#endif

#if PPL_SUPPORTED_DOUBLE
PPL_SPECIALIZE_CLASSIFY(classify_float, double)
PPL_SPECIALIZE_IS_NAN(is_nan_float, double)
PPL_SPECIALIZE_IS_MINF(is_minf_float, double)
PPL_SPECIALIZE_IS_PINF(is_pinf_float, double)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_float, double)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, signed char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, signed short)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, signed int)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, signed long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, signed long long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, unsigned char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, unsigned short)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, unsigned int)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, unsigned long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, double, unsigned long long)
PPL_SPECIALIZE_ASSIGN(assign_float_mpz, double, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_float_mpq, double, mpq_class)
PPL_SPECIALIZE_COPY(copy_generic, double)
PPL_SPECIALIZE_IS_INT(is_int_float, double)
PPL_SPECIALIZE_FLOOR(floor_float, double, double)
PPL_SPECIALIZE_CEIL(ceil_float, double, double)
PPL_SPECIALIZE_TRUNC(trunc_float, double, double)
PPL_SPECIALIZE_NEG(neg_float, double, double)
PPL_SPECIALIZE_ABS(abs_float, double, double)
PPL_SPECIALIZE_ADD(add_float, double, double, double)
PPL_SPECIALIZE_SUB(sub_float, double, double, double)
PPL_SPECIALIZE_MUL(mul_float, double, double, double)
PPL_SPECIALIZE_DIV(div_float, double, double, double)
PPL_SPECIALIZE_REM(rem_float, double, double, double)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_float, double, double)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_float, double, double)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_float, double, double)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_float, double, double)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_float, double, double)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_float, double, double)
PPL_SPECIALIZE_SQRT(sqrt_float, double, double)
PPL_SPECIALIZE_GCD(gcd_exact, double, double, double)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, double, double, double, double, double)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, double, double, double)
PPL_SPECIALIZE_SGN(sgn_float, double)
PPL_SPECIALIZE_CMP(cmp_float, double, double)
PPL_SPECIALIZE_ADD_MUL(add_mul_float, double, double, double)
PPL_SPECIALIZE_SUB_MUL(sub_mul_float, double, double, double)
PPL_SPECIALIZE_INPUT(input_generic, double)
PPL_SPECIALIZE_OUTPUT(output_float, double)
#endif

#if PPL_SUPPORTED_LONG_DOUBLE
PPL_SPECIALIZE_CLASSIFY(classify_float, long double)
PPL_SPECIALIZE_IS_NAN(is_nan_float, long double)
PPL_SPECIALIZE_IS_MINF(is_minf_float, long double)
PPL_SPECIALIZE_IS_PINF(is_pinf_float, long double)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_float, long double)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, signed char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, signed short)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, signed int)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, signed long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, signed long long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, unsigned char)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, unsigned short)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, unsigned int)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, unsigned long)
PPL_SPECIALIZE_ASSIGN(assign_float_int, long double, unsigned long long)
PPL_SPECIALIZE_ASSIGN(assign_float_mpz, long double, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_float_mpq, long double, mpq_class)
PPL_SPECIALIZE_COPY(copy_generic, long double)
PPL_SPECIALIZE_IS_INT(is_int_float, long double)
PPL_SPECIALIZE_FLOOR(floor_float, long double, long double)
PPL_SPECIALIZE_CEIL(ceil_float, long double, long double)
PPL_SPECIALIZE_TRUNC(trunc_float, long double, long double)
PPL_SPECIALIZE_NEG(neg_float, long double, long double)
PPL_SPECIALIZE_ABS(abs_float, long double, long double)
PPL_SPECIALIZE_ADD(add_float, long double, long double, long double)
PPL_SPECIALIZE_SUB(sub_float, long double, long double, long double)
PPL_SPECIALIZE_MUL(mul_float, long double, long double, long double)
PPL_SPECIALIZE_DIV(div_float, long double, long double, long double)
PPL_SPECIALIZE_REM(rem_float, long double, long double, long double)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_float, long double, long double)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_float, long double, long double)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_float, long double, long double)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_float, long double, long double)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_float, long double, long double)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_float, long double, long double)
PPL_SPECIALIZE_SQRT(sqrt_float, long double, long double)
PPL_SPECIALIZE_GCD(gcd_exact, long double, long double, long double)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, long double, long double, long double,
                  long double, long double)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, long double, long double, long double)
PPL_SPECIALIZE_SGN(sgn_float, long double)
PPL_SPECIALIZE_CMP(cmp_float, long double, long double)
PPL_SPECIALIZE_ADD_MUL(add_mul_float, long double, long double, long double)
PPL_SPECIALIZE_SUB_MUL(sub_mul_float, long double, long double, long double)
PPL_SPECIALIZE_INPUT(input_generic, long double)
PPL_SPECIALIZE_OUTPUT(output_float, long double)
#endif

} // namespace Checked

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_checked_int_inlines_hh)
