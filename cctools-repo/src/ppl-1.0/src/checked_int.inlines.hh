/* Specialized "checked" functions for native integer numbers.
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

#ifndef PPL_checked_int_inlines_hh
#define PPL_checked_int_inlines_hh 1

#include "C_Integer.hh"
#include <cerrno>
#include <cstdlib>
#include <climits>
#include <string>

#if !PPL_HAVE_DECL_STRTOLL
signed long long
strtoll(const char* nptr, char** endptr, int base);
#endif

#if !PPL_HAVE_DECL_STRTOULL
unsigned long long
strtoull(const char* nptr, char** endptr, int base);
#endif

namespace Parma_Polyhedra_Library {

namespace Checked {

#ifndef PPL_HAVE_INT_FAST16_T
typedef int16_t int_fast16_t;
#endif

#ifndef PPL_HAVE_INT_FAST32_T
typedef int32_t int_fast32_t;
#endif

#ifndef PPL_HAVE_INT_FAST64_T
typedef int64_t int_fast64_t;
#endif

#ifndef PPL_HAVE_UINT_FAST16_T
typedef uint16_t uint_fast16_t;
#endif

#ifndef PPL_HAVE_UINT_FAST32_T
typedef uint32_t uint_fast32_t;
#endif

#ifndef PPL_HAVE_UINT_FAST64_T
typedef uint64_t uint_fast64_t;
#endif

template <typename Policy, typename Type>
struct Extended_Int {
  static const Type plus_infinity = C_Integer<Type>::max;
  static const Type minus_infinity = ((C_Integer<Type>::min >= 0)
				      ? (C_Integer<Type>::max - 1)
				      : C_Integer<Type>::min);
  static const Type not_a_number
  = ((C_Integer<Type>::min >= 0)
     ? (C_Integer<Type>::max - 2 * (Policy::has_infinity ? 1 : 0))
     : (C_Integer<Type>::min + (Policy::has_infinity ? 1 : 0)));
  static const Type min
  = (C_Integer<Type>::min
     + ((C_Integer<Type>::min >= 0)
        ? 0
        : ((Policy::has_infinity ? 1 : 0) + (Policy::has_nan ? 1 : 0))));
  static const Type max
  = (C_Integer<Type>::max
     - ((C_Integer<Type>::min >= 0)
        ? (2 * (Policy::has_infinity ? 1 : 0) + (Policy::has_nan ? 1 : 0))
        : (Policy::has_infinity ? 1 : 0)));
};

template <typename Policy, typename To>
inline Result
set_neg_overflow_int(To& to, Rounding_Dir dir) {
  if (round_up(dir)) {
    to = Extended_Int<Policy, To>::min;
    return V_LT_INF;
  }
  else {
    if (Policy::has_infinity) {
      to = Extended_Int<Policy, To>::minus_infinity;
      return V_GT_MINUS_INFINITY;
    }
    return V_GT_MINUS_INFINITY | V_UNREPRESENTABLE;
  }
}

template <typename Policy, typename To>
inline Result
set_pos_overflow_int(To& to, Rounding_Dir dir) {
  if (round_down(dir)) {
    to = Extended_Int<Policy, To>::max;
    return V_GT_SUP;
  }
  else {
    if (Policy::has_infinity) {
      to = Extended_Int<Policy, To>::plus_infinity;
      return V_LT_PLUS_INFINITY;
    }
    return V_LT_PLUS_INFINITY | V_UNREPRESENTABLE;
  }
}

template <typename Policy, typename To>
inline Result
round_lt_int_no_overflow(To& to, Rounding_Dir dir) {
  if (round_down(dir)) {
    --to;
    return V_GT;
  }
  return V_LT;
}

template <typename Policy, typename To>
inline Result
round_gt_int_no_overflow(To& to, Rounding_Dir dir) {
  if (round_up(dir)) {
    ++to;
    return V_LT;
  }
  return V_GT;
}

template <typename Policy, typename To>
inline Result
round_lt_int(To& to, Rounding_Dir dir) {
  if (round_down(dir)) {
    if (to == Extended_Int<Policy, To>::min) {
      if (Policy::has_infinity) {
	to = Extended_Int<Policy, To>::minus_infinity;
	return V_GT_MINUS_INFINITY;
      }
      return V_GT_MINUS_INFINITY | V_UNREPRESENTABLE;
    }
    else {
      --to;
      return V_GT;
    }
  }
  return V_LT;
}

template <typename Policy, typename To>
inline Result
round_gt_int(To& to, Rounding_Dir dir) {
  if (round_up(dir)) {
    if (to == Extended_Int<Policy, To>::max) {
      if (Policy::has_infinity) {
	to = Extended_Int<Policy, To>::plus_infinity;
	return V_LT_PLUS_INFINITY;
      }
      return V_LT_PLUS_INFINITY | V_UNREPRESENTABLE;
    }
    else {
      ++to;
      return V_LT;
    }
  }
  return V_GT;
}

PPL_SPECIALIZE_COPY(copy_generic, char)
PPL_SPECIALIZE_COPY(copy_generic, signed char)
PPL_SPECIALIZE_COPY(copy_generic, signed short)
PPL_SPECIALIZE_COPY(copy_generic, signed int)
PPL_SPECIALIZE_COPY(copy_generic, signed long)
PPL_SPECIALIZE_COPY(copy_generic, signed long long)
PPL_SPECIALIZE_COPY(copy_generic, unsigned char)
PPL_SPECIALIZE_COPY(copy_generic, unsigned short)
PPL_SPECIALIZE_COPY(copy_generic, unsigned int)
PPL_SPECIALIZE_COPY(copy_generic, unsigned long)
PPL_SPECIALIZE_COPY(copy_generic, unsigned long long)

template <typename Policy, typename Type>
inline Result
classify_int(const Type v, bool nan, bool inf, bool sign) {
  if (Policy::has_nan
      && (nan || sign)
      && v == Extended_Int<Policy, Type>::not_a_number)
    return V_NAN;
  if (!inf && !sign)
    return V_LGE;
  if (Policy::has_infinity) {
    if (v == Extended_Int<Policy, Type>::minus_infinity)
      return inf ? V_EQ_MINUS_INFINITY : V_LT;
    if (v == Extended_Int<Policy, Type>::plus_infinity)
      return inf ? V_EQ_PLUS_INFINITY : V_GT;
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

PPL_SPECIALIZE_CLASSIFY(classify_int, char)
PPL_SPECIALIZE_CLASSIFY(classify_int, signed char)
PPL_SPECIALIZE_CLASSIFY(classify_int, signed short)
PPL_SPECIALIZE_CLASSIFY(classify_int, signed int)
PPL_SPECIALIZE_CLASSIFY(classify_int, signed long)
PPL_SPECIALIZE_CLASSIFY(classify_int, signed long long)
PPL_SPECIALIZE_CLASSIFY(classify_int, unsigned char)
PPL_SPECIALIZE_CLASSIFY(classify_int, unsigned short)
PPL_SPECIALIZE_CLASSIFY(classify_int, unsigned int)
PPL_SPECIALIZE_CLASSIFY(classify_int, unsigned long)
PPL_SPECIALIZE_CLASSIFY(classify_int, unsigned long long)

template <typename Policy, typename Type>
inline bool
is_nan_int(const Type v) {
  return Policy::has_nan && v == Extended_Int<Policy, Type>::not_a_number;
}

PPL_SPECIALIZE_IS_NAN(is_nan_int, char)
PPL_SPECIALIZE_IS_NAN(is_nan_int, signed char)
PPL_SPECIALIZE_IS_NAN(is_nan_int, signed short)
PPL_SPECIALIZE_IS_NAN(is_nan_int, signed int)
PPL_SPECIALIZE_IS_NAN(is_nan_int, signed long)
PPL_SPECIALIZE_IS_NAN(is_nan_int, signed long long)
PPL_SPECIALIZE_IS_NAN(is_nan_int, unsigned char)
PPL_SPECIALIZE_IS_NAN(is_nan_int, unsigned short)
PPL_SPECIALIZE_IS_NAN(is_nan_int, unsigned int)
PPL_SPECIALIZE_IS_NAN(is_nan_int, unsigned long)
PPL_SPECIALIZE_IS_NAN(is_nan_int, unsigned long long)

template <typename Policy, typename Type>
inline bool
is_minf_int(const Type v) {
  return Policy::has_infinity
    && v == Extended_Int<Policy, Type>::minus_infinity;
}

PPL_SPECIALIZE_IS_MINF(is_minf_int, char)
PPL_SPECIALIZE_IS_MINF(is_minf_int, signed char)
PPL_SPECIALIZE_IS_MINF(is_minf_int, signed short)
PPL_SPECIALIZE_IS_MINF(is_minf_int, signed int)
PPL_SPECIALIZE_IS_MINF(is_minf_int, signed long)
PPL_SPECIALIZE_IS_MINF(is_minf_int, signed long long)
PPL_SPECIALIZE_IS_MINF(is_minf_int, unsigned char)
PPL_SPECIALIZE_IS_MINF(is_minf_int, unsigned short)
PPL_SPECIALIZE_IS_MINF(is_minf_int, unsigned int)
PPL_SPECIALIZE_IS_MINF(is_minf_int, unsigned long)
PPL_SPECIALIZE_IS_MINF(is_minf_int, unsigned long long)

template <typename Policy, typename Type>
inline bool
is_pinf_int(const Type v) {
  return Policy::has_infinity
    && v == Extended_Int<Policy, Type>::plus_infinity;
}

PPL_SPECIALIZE_IS_PINF(is_pinf_int, char)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, signed char)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, signed short)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, signed int)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, signed long)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, signed long long)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, unsigned char)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, unsigned short)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, unsigned int)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, unsigned long)
PPL_SPECIALIZE_IS_PINF(is_pinf_int, unsigned long long)

template <typename Policy, typename Type>
inline bool
is_int_int(const Type v) {
  return !is_nan<Policy>(v);
}

PPL_SPECIALIZE_IS_INT(is_int_int, char)
PPL_SPECIALIZE_IS_INT(is_int_int, signed char)
PPL_SPECIALIZE_IS_INT(is_int_int, signed short)
PPL_SPECIALIZE_IS_INT(is_int_int, signed int)
PPL_SPECIALIZE_IS_INT(is_int_int, signed long)
PPL_SPECIALIZE_IS_INT(is_int_int, signed long long)
PPL_SPECIALIZE_IS_INT(is_int_int, unsigned char)
PPL_SPECIALIZE_IS_INT(is_int_int, unsigned short)
PPL_SPECIALIZE_IS_INT(is_int_int, unsigned int)
PPL_SPECIALIZE_IS_INT(is_int_int, unsigned long)
PPL_SPECIALIZE_IS_INT(is_int_int, unsigned long long)

template <typename Policy, typename Type>
inline Result
assign_special_int(Type& v, Result_Class c, Rounding_Dir dir) {
  PPL_ASSERT(c == VC_MINUS_INFINITY || c == VC_PLUS_INFINITY || c == VC_NAN);
  switch (c) {
  case VC_NAN:
    if (Policy::has_nan) {
      v = Extended_Int<Policy, Type>::not_a_number;
      return V_NAN;
    }
    return V_NAN | V_UNREPRESENTABLE;
  case VC_MINUS_INFINITY:
    if (Policy::has_infinity) {
      v = Extended_Int<Policy, Type>::minus_infinity;
      return V_EQ_MINUS_INFINITY;
    }
    if (round_up(dir)) {
      v = Extended_Int<Policy, Type>::min;
      return V_LT_INF;
    }
    return V_EQ_MINUS_INFINITY | V_UNREPRESENTABLE;
  case VC_PLUS_INFINITY:
    if (Policy::has_infinity) {
      v = Extended_Int<Policy, Type>::plus_infinity;
      return V_EQ_PLUS_INFINITY;
    }
    if (round_down(dir)) {
      v = Extended_Int<Policy, Type>::max;
      return V_GT_SUP;
    }
    return V_EQ_PLUS_INFINITY | V_UNREPRESENTABLE;
  default:
    PPL_UNREACHABLE;
    return V_NAN | V_UNREPRESENTABLE;
  }
}

PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, char)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, signed char)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, signed short)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, signed int)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, signed long)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, signed long long)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, unsigned char)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, unsigned short)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, unsigned int)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, unsigned long)
PPL_SPECIALIZE_ASSIGN_SPECIAL(assign_special_int, unsigned long long)

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_signed_int_signed_int(To& to, const From from, Rounding_Dir dir) {
  if (sizeof(To) < sizeof(From)
      || (sizeof(To) == sizeof(From)
	  && (Extended_Int<To_Policy, To>::min > Extended_Int<From_Policy, From>::min
	      || Extended_Int<To_Policy, To>::max < Extended_Int<From_Policy, From>::max))) {
    if (CHECK_P(To_Policy::check_overflow,
		PPL_LT_SILENT(from,
                              static_cast<From>(Extended_Int<To_Policy, To>::min))))
      return set_neg_overflow_int<To_Policy>(to, dir);
    if (CHECK_P(To_Policy::check_overflow,
		PPL_GT_SILENT(from,
                              static_cast<From>(Extended_Int<To_Policy, To>::max))))
      return set_pos_overflow_int<To_Policy>(to, dir);
  }
  to = static_cast<To>(from);
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_signed_int_unsigned_int(To& to, const From from, Rounding_Dir dir) {
  if (sizeof(To) <= sizeof(From)) {
    if (CHECK_P(To_Policy::check_overflow,
		from > static_cast<From>(Extended_Int<To_Policy, To>::max)))
      return set_pos_overflow_int<To_Policy>(to, dir);
  }
  to = static_cast<To>(from);
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_unsigned_int_signed_int(To& to, const From from, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_overflow, from < 0))
    return set_neg_overflow_int<To_Policy>(to, dir);
  if (sizeof(To) < sizeof(From)) {
    if (CHECK_P(To_Policy::check_overflow,
		from > static_cast<From>(Extended_Int<To_Policy, To>::max)))
      return set_pos_overflow_int<To_Policy>(to, dir);
  }
  to = static_cast<To>(from);
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_unsigned_int_unsigned_int(To& to, const From from, Rounding_Dir dir) {
  if (sizeof(To) < sizeof(From)
      || (sizeof(To) == sizeof(From)
	  && Extended_Int<To_Policy, To>::max < Extended_Int<From_Policy, From>::max)) {
    if (CHECK_P(To_Policy::check_overflow,
		PPL_GT_SILENT(from,
                              static_cast<From>(Extended_Int<To_Policy, To>::max))))
      return set_pos_overflow_int<To_Policy>(to, dir);
  }
  to = static_cast<To>(from);
  return V_EQ;
}


#define PPL_ASSIGN2_SIGNED_SIGNED(Smaller, Larger) \
PPL_SPECIALIZE_ASSIGN(assign_signed_int_signed_int, Smaller, Larger) \
PPL_SPECIALIZE_ASSIGN(assign_signed_int_signed_int, Larger, Smaller)

#define PPL_ASSIGN2_UNSIGNED_UNSIGNED(Smaller, Larger) \
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_unsigned_int, Smaller, Larger) \
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_unsigned_int, Larger, Smaller)

#define PPL_ASSIGN2_UNSIGNED_SIGNED(Smaller, Larger) \
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_signed_int, Smaller, Larger) \
PPL_SPECIALIZE_ASSIGN(assign_signed_int_unsigned_int, Larger, Smaller)

#define PPL_ASSIGN2_SIGNED_UNSIGNED(Smaller, Larger) \
PPL_SPECIALIZE_ASSIGN(assign_signed_int_unsigned_int, Smaller, Larger) \
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_signed_int, Larger, Smaller)

#define PPL_ASSIGN_SIGNED(Type) \
PPL_SPECIALIZE_ASSIGN(assign_signed_int_signed_int, Type, Type)
#define PPL_ASSIGN_UNSIGNED(Type) \
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_unsigned_int, Type, Type)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_ASSIGN_SIGNED(char)
#endif
PPL_ASSIGN_SIGNED(signed char)
PPL_ASSIGN_SIGNED(signed short)
PPL_ASSIGN_SIGNED(signed int)
PPL_ASSIGN_SIGNED(signed long)
PPL_ASSIGN_SIGNED(signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_ASSIGN_UNSIGNED(char)
#endif
PPL_ASSIGN_UNSIGNED(unsigned char)
PPL_ASSIGN_UNSIGNED(unsigned short)
PPL_ASSIGN_UNSIGNED(unsigned int)
PPL_ASSIGN_UNSIGNED(unsigned long)
PPL_ASSIGN_UNSIGNED(unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_ASSIGN2_SIGNED_SIGNED(char, signed short)
PPL_ASSIGN2_SIGNED_SIGNED(char, signed int)
PPL_ASSIGN2_SIGNED_SIGNED(char, signed long)
PPL_ASSIGN2_SIGNED_SIGNED(char, signed long long)
#endif
PPL_ASSIGN2_SIGNED_SIGNED(signed char, signed short)
PPL_ASSIGN2_SIGNED_SIGNED(signed char, signed int)
PPL_ASSIGN2_SIGNED_SIGNED(signed char, signed long)
PPL_ASSIGN2_SIGNED_SIGNED(signed char, signed long long)
PPL_ASSIGN2_SIGNED_SIGNED(signed short, signed int)
PPL_ASSIGN2_SIGNED_SIGNED(signed short, signed long)
PPL_ASSIGN2_SIGNED_SIGNED(signed short, signed long long)
PPL_ASSIGN2_SIGNED_SIGNED(signed int, signed long)
PPL_ASSIGN2_SIGNED_SIGNED(signed int, signed long long)
PPL_ASSIGN2_SIGNED_SIGNED(signed long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_ASSIGN2_UNSIGNED_UNSIGNED(char, unsigned short)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(char, unsigned int)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(char, unsigned long)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(char, unsigned long long)
#endif
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned char, unsigned short)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned char, unsigned int)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned char, unsigned long)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned char, unsigned long long)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned short, unsigned int)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned short, unsigned long)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned short, unsigned long long)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned int, unsigned long)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned int, unsigned long long)
PPL_ASSIGN2_UNSIGNED_UNSIGNED(unsigned long, unsigned long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_ASSIGN2_UNSIGNED_SIGNED(char, signed short)
PPL_ASSIGN2_UNSIGNED_SIGNED(char, signed int)
PPL_ASSIGN2_UNSIGNED_SIGNED(char, signed long)
PPL_ASSIGN2_UNSIGNED_SIGNED(char, signed long long)
#endif
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned char, signed short)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned char, signed int)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned char, signed long)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned char, signed long long)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned short, signed int)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned short, signed long)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned short, signed long long)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned int, signed long)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned int, signed long long)
PPL_ASSIGN2_UNSIGNED_SIGNED(unsigned long, signed long long)
#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_ASSIGN2_SIGNED_UNSIGNED(char, unsigned char)
PPL_ASSIGN2_SIGNED_UNSIGNED(char, unsigned short)
PPL_ASSIGN2_SIGNED_UNSIGNED(char, unsigned int)
PPL_ASSIGN2_SIGNED_UNSIGNED(char, unsigned long)
PPL_ASSIGN2_SIGNED_UNSIGNED(char, unsigned long long)
#else
PPL_ASSIGN2_SIGNED_UNSIGNED(signed char, char)
#endif
PPL_ASSIGN2_SIGNED_UNSIGNED(signed char, unsigned char)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed char, unsigned short)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed char, unsigned int)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed char, unsigned long)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed char, unsigned long long)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed short, unsigned short)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed short, unsigned int)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed short, unsigned long)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed short, unsigned long long)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed int, unsigned int)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed int, unsigned long)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed int, unsigned long long)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed long, unsigned long)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed long, unsigned long long)
PPL_ASSIGN2_SIGNED_UNSIGNED(signed long long, unsigned long long)

template <typename To_Policy, typename From_Policy, typename To, typename From>
inline Result
assign_int_float(To& to, const From from, Rounding_Dir dir) {
  if (is_nan<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_NAN, ROUND_IGNORE);
  else if (is_minf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_MINUS_INFINITY, dir);
  else if (is_pinf<From_Policy>(from))
    return assign_special<To_Policy>(to, VC_PLUS_INFINITY, dir);
#if 0
  // FIXME: this is correct but it is inefficient and breaks the build
  // for the missing definition of static const members (a problem present
  // also in other areas of the PPL).
  if (CHECK_P(To_Policy::check_overflow, lt(from, Extended_Int<To_Policy, To>::min)))
    return set_neg_overflow_int<To_Policy>(to, dir);
  if (CHECK_P(To_Policy::check_overflow, !le(from, Extended_Int<To_Policy, To>::max)))
    return set_pos_overflow_int<To_Policy>(to, dir);
#else
  if (CHECK_P(To_Policy::check_overflow, (from < Extended_Int<To_Policy, To>::min)))
    return set_neg_overflow_int<To_Policy>(to, dir);
  if (CHECK_P(To_Policy::check_overflow, (from > Extended_Int<To_Policy, To>::max)))
    return set_pos_overflow_int<To_Policy>(to, dir);
#endif
  if (round_not_requested(dir)) {
    to = from;
    return V_LGE;
  }
  From i_from = rint(from);
  to = i_from;
  if (from == i_from)
    return V_EQ;
  if (round_direct(ROUND_UP))
    return round_lt_int<To_Policy>(to, dir);
  if (round_direct(ROUND_DOWN))
    return round_gt_int<To_Policy>(to, dir);
  if (from < i_from)
    return round_lt_int<To_Policy>(to, dir);
  PPL_ASSERT(from > i_from);
  return round_gt_int<To_Policy>(to, dir);
}

PPL_SPECIALIZE_ASSIGN(assign_int_float, char, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed char, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed short, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed int, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed long, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed long long, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned char, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned short, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned int, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned long, float)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned long long, float)

PPL_SPECIALIZE_ASSIGN(assign_int_float, char, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed char, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed short, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed int, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed long, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed long long, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned char, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned short, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned int, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned long, double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned long long, double)

PPL_SPECIALIZE_ASSIGN(assign_int_float, char, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed char, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed short, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed int, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed long, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, signed long long, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned char, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned short, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned int, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned long, long double)
PPL_SPECIALIZE_ASSIGN(assign_int_float, unsigned long long, long double)

#undef PPL_ASSIGN_SIGNED
#undef PPL_ASSIGN_UNSIGNED
#undef PPL_ASSIGN2_SIGNED_SIGNED
#undef PPL_ASSIGN2_UNSIGNED_UNSIGNED
#undef PPL_ASSIGN2_UNSIGNED_SIGNED
#undef PPL_ASSIGN2_SIGNED_UNSIGNED

template <typename To_Policy, typename From_Policy, typename To>
inline Result
assign_signed_int_mpz(To& to, const mpz_class& from, Rounding_Dir dir) {
  if (sizeof(To) <= sizeof(signed long)) {
    if (!To_Policy::check_overflow) {
      to = from.get_si();
      return V_EQ;
    }
    if (from.fits_slong_p()) {
      signed long v = from.get_si();
      if (PPL_LT_SILENT(v, (Extended_Int<To_Policy, To>::min)))
	return set_neg_overflow_int<To_Policy>(to, dir);
      if (PPL_GT_SILENT(v, (Extended_Int<To_Policy, To>::max)))
	return set_pos_overflow_int<To_Policy>(to, dir);
      to = v;
      return V_EQ;
    }
  }
  else {
    mpz_srcptr m = from.get_mpz_t();
    size_t sz = mpz_size(m);
    if (sz <= sizeof(To) / sizeof(mp_limb_t)) {
      if (sz == 0) {
	to = 0;
	return V_EQ;
      }
      To v;
      mpz_export(&v, 0, -1, sizeof(To), 0, 0, m);
      if (v >= 0) {
	if (::sgn(from) < 0)
	  return neg<To_Policy, To_Policy>(to, v, dir);
	to = v;
	return V_EQ;
      }
    }
  }
  return (::sgn(from) < 0)
    ? set_neg_overflow_int<To_Policy>(to, dir)
    : set_pos_overflow_int<To_Policy>(to, dir);
}

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_ASSIGN(assign_signed_int_mpz, char, mpz_class)
#endif
PPL_SPECIALIZE_ASSIGN(assign_signed_int_mpz, signed char, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_signed_int_mpz, signed short, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_signed_int_mpz, signed int, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_signed_int_mpz, signed long, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_signed_int_mpz, signed long long, mpz_class)

template <typename To_Policy, typename From_Policy, typename To>
inline Result
assign_unsigned_int_mpz(To& to, const mpz_class& from, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_overflow, ::sgn(from) < 0))
    return set_neg_overflow_int<To_Policy>(to, dir);
  if (sizeof(To) <= sizeof(unsigned long)) {
    if (!To_Policy::check_overflow) {
      to = static_cast<To>(from.get_ui());
      return V_EQ;
    }
    if (from.fits_ulong_p()) {
      unsigned long v = from.get_ui();
      if (PPL_GT_SILENT(v, (Extended_Int<To_Policy, To>::max)))
	return set_pos_overflow_int<To_Policy>(to, dir);
      to = static_cast<To>(v);
      return V_EQ;
    }
  }
  else {
    mpz_srcptr m = from.get_mpz_t();
    size_t sz = mpz_size(m);
    if (sz <= sizeof(To) / sizeof(mp_limb_t)) {
      if (sz == 0)
	to = 0;
      else
	mpz_export(&to, 0, -1, sizeof(To), 0, 0, m);
      return V_EQ;
    }
  }
  return set_pos_overflow_int<To_Policy>(to, dir);
}

#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_mpz, char, mpz_class)
#endif
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_mpz, unsigned char, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_mpz, unsigned short, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_mpz, unsigned int, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_mpz, unsigned long, mpz_class)
PPL_SPECIALIZE_ASSIGN(assign_unsigned_int_mpz, unsigned long long, mpz_class)

template <typename To_Policy, typename From_Policy, typename To>
inline Result
assign_int_mpq(To& to, const mpq_class& from, Rounding_Dir dir) {
  mpz_srcptr n = from.get_num().get_mpz_t();
  mpz_srcptr d = from.get_den().get_mpz_t();
  PPL_DIRTY_TEMP(mpz_class, q);
  mpz_ptr q_z = q.get_mpz_t();
  if (round_not_requested(dir)) {
    mpz_tdiv_q(q_z, n, d);
    Result r = assign<To_Policy, void>(to, q, dir);
    if (r != V_EQ)
      return r;
    return V_LGE;
  }
  mpz_t rem;
  int sign;
  mpz_init(rem);
  mpz_tdiv_qr(q_z, rem, n, d);
  sign = mpz_sgn(rem);
  mpz_clear(rem);
  Result r = assign<To_Policy, void>(to, q, dir);
  if (r != V_EQ)
    return r;
  switch (sign) {
  case -1:
    return round_lt_int<To_Policy>(to, dir);
  case 1:
    return round_gt_int<To_Policy>(to, dir);
  default:
    return V_EQ;
  }
}

PPL_SPECIALIZE_ASSIGN(assign_int_mpq, char, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, signed char, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, signed short, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, signed int, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, signed long, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, signed long long, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, unsigned char, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, unsigned short, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, unsigned int, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, unsigned long, mpq_class)
PPL_SPECIALIZE_ASSIGN(assign_int_mpq, unsigned long long, mpq_class)

#if ~0 != -1
#error "Only two's complement is supported"
#endif

#if UCHAR_MAX == 0xff
#define CHAR_BITS 8
#else
#error "Unexpected max for unsigned char"
#endif

#if USHRT_MAX == 0xffff
#define SHRT_BITS 16
#else
#error "Unexpected max for unsigned short"
#endif

#if UINT_MAX == 0xffffffff
#define INT_BITS 32
#else
#error "Unexpected max for unsigned int"
#endif

#if ULONG_MAX == 0xffffffffUL
#define LONG_BITS 32
#elif ULONG_MAX == 0xffffffffffffffffULL
#define LONG_BITS 64
#else
#error "Unexpected max for unsigned long"
#endif

#if ULLONG_MAX == 0xffffffffffffffffULL
#define LONG_LONG_BITS 64
#else
#error "Unexpected max for unsigned long long"
#endif


template <typename T>
struct Larger;

// The following may be tuned for performance on specific architectures.
//
// Current guidelines:
//   - avoid division where possible (larger type variant for mul)
//   - use larger type variant for types smaller than architecture bit size

template <>
struct Larger<char> {
  const_bool_nodef(use_for_neg, true);
  const_bool_nodef(use_for_add, true);
  const_bool_nodef(use_for_sub, true);
  const_bool_nodef(use_for_mul, true);
  typedef int_fast16_t type_for_neg;
  typedef int_fast16_t type_for_add;
  typedef int_fast16_t type_for_sub;
  typedef int_fast16_t type_for_mul;
};

template <>
struct Larger<signed char> {
  const_bool_nodef(use_for_neg, true);
  const_bool_nodef(use_for_add, true);
  const_bool_nodef(use_for_sub, true);
  const_bool_nodef(use_for_mul, true);
  typedef int_fast16_t type_for_neg;
  typedef int_fast16_t type_for_add;
  typedef int_fast16_t type_for_sub;
  typedef int_fast16_t type_for_mul;
};

template <>
struct Larger<unsigned char> {
  const_bool_nodef(use_for_neg, true);
  const_bool_nodef(use_for_add, true);
  const_bool_nodef(use_for_sub, true);
  const_bool_nodef(use_for_mul, true);
  typedef int_fast16_t type_for_neg;
  typedef uint_fast16_t type_for_add;
  typedef int_fast16_t type_for_sub;
  typedef uint_fast16_t type_for_mul;
};

template <>
struct Larger<signed short> {
  const_bool_nodef(use_for_neg, true);
  const_bool_nodef(use_for_add, true);
  const_bool_nodef(use_for_sub, true);
  const_bool_nodef(use_for_mul, true);
  typedef int_fast32_t type_for_neg;
  typedef int_fast32_t type_for_add;
  typedef int_fast32_t type_for_sub;
  typedef int_fast32_t type_for_mul;
};

template <>
struct Larger<unsigned short> {
  const_bool_nodef(use_for_neg, true);
  const_bool_nodef(use_for_add, true);
  const_bool_nodef(use_for_sub, true);
  const_bool_nodef(use_for_mul, true);
  typedef int_fast32_t type_for_neg;
  typedef uint_fast32_t type_for_add;
  typedef int_fast32_t type_for_sub;
  typedef uint_fast32_t type_for_mul;
};

template <>
struct Larger<signed int> {
  const_bool_nodef(use_for_neg, (LONG_BITS == 64));
  const_bool_nodef(use_for_add, (LONG_BITS == 64));
  const_bool_nodef(use_for_sub, (LONG_BITS == 64));
  const_bool_nodef(use_for_mul, true);
  typedef int_fast64_t type_for_neg;
  typedef int_fast64_t type_for_add;
  typedef int_fast64_t type_for_sub;
  typedef int_fast64_t type_for_mul;
};

template <>
struct Larger<unsigned int> {
  const_bool_nodef(use_for_neg, (LONG_BITS == 64));
  const_bool_nodef(use_for_add, (LONG_BITS == 64));
  const_bool_nodef(use_for_sub, (LONG_BITS == 64));
  const_bool_nodef(use_for_mul, true);
  typedef int_fast64_t type_for_neg;
  typedef uint_fast64_t type_for_add;
  typedef int_fast64_t type_for_sub;
  typedef uint_fast64_t type_for_mul;
};

template <>
struct Larger<signed long> {
  const_bool_nodef(use_for_neg, false);
  const_bool_nodef(use_for_add, false);
  const_bool_nodef(use_for_sub, false);
  const_bool_nodef(use_for_mul, (LONG_BITS == 32));
  typedef int_fast64_t type_for_neg;
  typedef int_fast64_t type_for_add;
  typedef int_fast64_t type_for_sub;
  typedef int_fast64_t type_for_mul;
};

template <>
struct Larger<unsigned long> {
  const_bool_nodef(use_for_neg, false);
  const_bool_nodef(use_for_add, false);
  const_bool_nodef(use_for_sub, false);
  const_bool_nodef(use_for_mul, (LONG_BITS == 32));
  typedef int_fast64_t type_for_neg;
  typedef uint_fast64_t type_for_add;
  typedef int_fast64_t type_for_sub;
  typedef uint_fast64_t type_for_mul;
};

template <>
struct Larger<signed long long> {
  const_bool_nodef(use_for_neg, false);
  const_bool_nodef(use_for_add, false);
  const_bool_nodef(use_for_sub, false);
  const_bool_nodef(use_for_mul, false);
  typedef int_fast64_t type_for_neg;
  typedef int_fast64_t type_for_add;
  typedef int_fast64_t type_for_sub;
  typedef int_fast64_t type_for_mul;
};

template <>
struct Larger<unsigned long long> {
  const_bool_nodef(use_for_neg, false);
  const_bool_nodef(use_for_add, false);
  const_bool_nodef(use_for_sub, false);
  const_bool_nodef(use_for_mul, false);
  typedef int_fast64_t type_for_neg;
  typedef uint_fast64_t type_for_add;
  typedef int_fast64_t type_for_sub;
  typedef uint_fast64_t type_for_mul;
};

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
neg_int_larger(Type& to, const Type x, Rounding_Dir dir) {
  typename Larger<Type>::type_for_neg l = x;
  l = -l;
  return assign<To_Policy, To_Policy>(to, l, dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
add_int_larger(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  typename Larger<Type>::type_for_add l = x;
  l += y;
  return assign<To_Policy, To_Policy>(to, l, dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
sub_int_larger(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  typename Larger<Type>::type_for_sub l = x;
  l -= y;
  return assign<To_Policy, To_Policy>(to, l, dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
mul_int_larger(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  typename Larger<Type>::type_for_mul l = x;
  l *= y;
  return assign<To_Policy, To_Policy>(to, l, dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
neg_signed_int(Type& to, const Type from, Rounding_Dir dir) {
  if (To_Policy::check_overflow && Larger<Type>::use_for_neg)
    return neg_int_larger<To_Policy, From_Policy>(to, from, dir);
  if (CHECK_P(To_Policy::check_overflow,
	      (from < -Extended_Int<To_Policy, Type>::max)))
    return set_pos_overflow_int<To_Policy>(to, dir);
  to = -from;
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
neg_unsigned_int(Type& to, const Type from, Rounding_Dir dir) {
  if (To_Policy::check_overflow && Larger<Type>::use_for_neg)
    return neg_int_larger<To_Policy, From_Policy>(to, from, dir);
  if (CHECK_P(To_Policy::check_overflow, from != 0))
    return set_neg_overflow_int<To_Policy>(to, dir);
  to = from;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
add_signed_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_overflow && Larger<Type>::use_for_add)
    return add_int_larger<To_Policy, From1_Policy, From2_Policy>(to, x, y, dir);
  if (To_Policy::check_overflow) {
    if (y >= 0) {
      if (x > Extended_Int<To_Policy, Type>::max - y)
	return set_pos_overflow_int<To_Policy>(to, dir);
    }
    else if (x < Extended_Int<To_Policy, Type>::min - y)
      return set_neg_overflow_int<To_Policy>(to, dir);
  }
  to = x + y;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
add_unsigned_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_overflow && Larger<Type>::use_for_add)
    return add_int_larger<To_Policy, From1_Policy, From2_Policy>(to, x, y, dir);
  if (CHECK_P(To_Policy::check_overflow,
	      (x > Extended_Int<To_Policy, Type>::max - y)))
    return set_pos_overflow_int<To_Policy>(to, dir);
  to = x + y;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
sub_signed_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_overflow && Larger<Type>::use_for_sub)
    return sub_int_larger<To_Policy, From1_Policy, From2_Policy>(to, x, y, dir);
  if (To_Policy::check_overflow) {
    if (y >= 0) {
      if (x < Extended_Int<To_Policy, Type>::min + y)
	return set_neg_overflow_int<To_Policy>(to, dir);
    }
    else if (x > Extended_Int<To_Policy, Type>::max + y)
      return set_pos_overflow_int<To_Policy>(to, dir);
  }
  to = x - y;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
sub_unsigned_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_overflow && Larger<Type>::use_for_sub)
    return sub_int_larger<To_Policy, From1_Policy, From2_Policy>(to, x, y, dir);
  if (CHECK_P(To_Policy::check_overflow,
	      (x < Extended_Int<To_Policy, Type>::min + y)))
    return set_neg_overflow_int<To_Policy>(to, dir);
  to = x - y;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
mul_signed_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_overflow && Larger<Type>::use_for_mul)
    return mul_int_larger<To_Policy, From1_Policy, From2_Policy>(to, x, y, dir);
  if (!To_Policy::check_overflow) {
    to = x * y;
    return V_EQ;
  }
  if (y == 0) {
    to = 0;
    return V_EQ;
  }
  if (y == -1)
    return neg_signed_int<To_Policy, From1_Policy>(to, x, dir);
  if (x >= 0) {
    if (y > 0) {
      if (x > Extended_Int<To_Policy, Type>::max / y)
	return set_pos_overflow_int<To_Policy>(to, dir);
    }
    else {
      if (x > Extended_Int<To_Policy, Type>::min / y)
	return set_neg_overflow_int<To_Policy>(to, dir);
    }
  }
  else {
    if (y < 0) {
      if (x < Extended_Int<To_Policy, Type>::max / y)
	return set_pos_overflow_int<To_Policy>(to, dir);
    }
    else {
      if (x < Extended_Int<To_Policy, Type>::min / y)
	return set_neg_overflow_int<To_Policy>(to, dir);
    }
  }
  to = x * y;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
mul_unsigned_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (To_Policy::check_overflow && Larger<Type>::use_for_mul)
    return mul_int_larger<To_Policy, From1_Policy, From2_Policy>(to, x, y, dir);
  if (!To_Policy::check_overflow) {
    to = x * y;
    return V_EQ;
  }
  if (y == 0) {
    to = 0;
    return V_EQ;
  }
  if (x > Extended_Int<To_Policy, Type>::max / y)
    return set_pos_overflow_int<To_Policy>(to, dir);
  to = x * y;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
div_signed_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_div_zero, y == 0)) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  if (To_Policy::check_overflow && y == -1)
    return neg_signed_int<To_Policy, From1_Policy>(to, x, dir);
  to = x / y;
  if (round_not_requested(dir))
    return V_LGE;
  if (y == -1)
    return V_EQ;
  Type m = x % y;
  if (m < 0)
    return round_lt_int_no_overflow<To_Policy>(to, dir);
  else if (m > 0)
    return round_gt_int_no_overflow<To_Policy>(to, dir);
  else
    return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
div_unsigned_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_div_zero, y == 0)) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  to = x / y;
  if (round_not_requested(dir))
    return V_GE;
  Type m = x % y;
  if (m == 0)
    return V_EQ;
  return round_gt_int<To_Policy>(to, dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
idiv_signed_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_div_zero, y == 0)) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  if (To_Policy::check_overflow && y == -1)
    return neg_signed_int<To_Policy, From1_Policy>(to, x, dir);
  to = x / y;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
idiv_unsigned_int(Type& to, const Type x, const Type y, Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, y == 0)) {
    return assign_nan<To_Policy>(to, V_DIV_ZERO);
  }
  to = x / y;
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
rem_signed_int(Type& to, const Type x, const Type y, Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, y == 0)) {
    return assign_nan<To_Policy>(to, V_MOD_ZERO);
  }
  to = (y == -1) ? 0 : (x % y);
  return V_EQ;
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
rem_unsigned_int(Type& to, const Type x, const Type y, Rounding_Dir) {
  if (CHECK_P(To_Policy::check_div_zero, y == 0)) {
    return assign_nan<To_Policy>(to, V_MOD_ZERO);
  }
  to = x % y;
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
div_2exp_unsigned_int(Type& to, const Type x, unsigned int exp,
                      Rounding_Dir dir) {
  if (exp >= sizeof_to_bits(sizeof(Type))) {
    to = 0;
    if (round_not_requested(dir))
      return V_GE;
    if (x == 0)
      return V_EQ;
    return round_gt_int_no_overflow<To_Policy>(to, dir);
  }
  to = x >> exp;
  if (round_not_requested(dir))
    return V_GE;
  if (x & ((Type(1) << exp) - 1))
    return round_gt_int_no_overflow<To_Policy>(to, dir);
  else
    return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
div_2exp_signed_int(Type& to, const Type x, unsigned int exp,
                    Rounding_Dir dir) {
  if (exp > sizeof_to_bits(sizeof(Type)) - 1) {
  zero:
    to = 0;
    if (round_not_requested(dir))
      return V_LGE;
    if (x < 0)
      return round_lt_int_no_overflow<To_Policy>(to, dir);
    else if (x > 0)
      return round_gt_int_no_overflow<To_Policy>(to, dir);
    else
      return V_EQ;
  }
  if (exp == sizeof_to_bits(sizeof(Type)) - 1) {
    if (x == C_Integer<Type>::min) {
      to = -1;
      return V_EQ;
    }
    goto zero;
  }
#if 0
  to = x / (Type(1) << exp);
  if (round_not_requested(dir))
    return V_GE;
  Type r = x % (Type(1) << exp);
  if (r < 0)
    return round_lt_int_no_overflow<To_Policy>(to, dir);
  else if (r > 0)
    return round_gt_int_no_overflow<To_Policy>(to, dir);
  else
    return V_EQ;
#else
  // Faster but compiler implementation dependent (see C++98 5.8.3)
  to = x >> exp;
  if (round_not_requested(dir))
    return V_GE;
  if (x & ((Type(1) << exp) - 1))
    return round_gt_int_no_overflow<To_Policy>(to, dir);
  return V_EQ;
#endif
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
add_2exp_unsigned_int(Type& to, const Type x, unsigned int exp,
                      Rounding_Dir dir) {
  if (!To_Policy::check_overflow) {
    to = x + (Type(1) << exp);
    return V_EQ;
  }
  if (exp >= sizeof_to_bits(sizeof(Type)))
    return set_pos_overflow_int<To_Policy>(to, dir);
  Type n = Type(1) << exp;
  return add_unsigned_int<To_Policy, From_Policy, void>(to, x, n, dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
add_2exp_signed_int(Type& to, const Type x, unsigned int exp,
		    Rounding_Dir dir) {
  if (!To_Policy::check_overflow) {
    to = x + (Type(1) << exp);
    return V_EQ;
  }
  if (exp >= sizeof_to_bits(sizeof(Type)))
    return set_pos_overflow_int<To_Policy>(to, dir);
  if (exp == sizeof_to_bits(sizeof(Type)) - 1) {
    Type n = -2 * (Type(1) << (exp - 1));
    return sub_signed_int<To_Policy, From_Policy, void>(to, x, n, dir);
  }
  else {
    Type n = Type(1) << exp;
    return add_signed_int<To_Policy, From_Policy, void>(to, x, n, dir);
  }
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
sub_2exp_unsigned_int(Type& to, const Type x, unsigned int exp,
                      Rounding_Dir dir) {
  if (!To_Policy::check_overflow) {
    to = x - (Type(1) << exp);
    return V_EQ;
  }
  if (exp >= sizeof_to_bits(sizeof(Type)))
    return set_neg_overflow_int<To_Policy>(to, dir);
  Type n = Type(1) << exp;
  return sub_unsigned_int<To_Policy, From_Policy, void>(to, x, n, dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
sub_2exp_signed_int(Type& to, const Type x, unsigned int exp,
		    Rounding_Dir dir) {
  if (!To_Policy::check_overflow) {
    to = x - (Type(1) << exp);
    return V_EQ;
  }
  if (exp >= sizeof_to_bits(sizeof(Type)))
    return set_neg_overflow_int<To_Policy>(to, dir);
  if (exp == sizeof_to_bits(sizeof(Type)) - 1) {
    Type n = -2 * (Type(1) << (exp - 1));
    return add_signed_int<To_Policy, From_Policy, void>(to, x, n, dir);
  }
  else {
    Type n = Type(1) << exp;
    return sub_signed_int<To_Policy, From_Policy, void>(to, x, n, dir);
  }
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
mul_2exp_unsigned_int(Type& to, const Type x, unsigned int exp,
                      Rounding_Dir dir) {
  if (!To_Policy::check_overflow) {
    to = x << exp;
    return V_EQ;
  }
  if (exp >= sizeof_to_bits(sizeof(Type))) {
    if (x == 0) {
      to = 0;
      return V_EQ;
    }
    return set_pos_overflow_int<To_Policy>(to, dir);
  }
  if (x & (((Type(1) << exp) - 1) << (sizeof_to_bits(sizeof(Type)) - exp)))
    return set_pos_overflow_int<To_Policy>(to, dir);
  Type n = x << exp;
  if (PPL_GT_SILENT(n, (Extended_Int<To_Policy, Type>::max)))
    return set_pos_overflow_int<To_Policy>(to, dir);
  to = n;
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
mul_2exp_signed_int(Type& to, const Type x, unsigned int exp,
                    Rounding_Dir dir) {
  if (!To_Policy::check_overflow) {
    to = x << exp;
    return V_EQ;
  }
  if (exp >= sizeof_to_bits(sizeof(Type)) - 1) {
    if (x < 0)
      return set_neg_overflow_int<To_Policy>(to, dir);
    else if (x > 0)
      return set_pos_overflow_int<To_Policy>(to, dir);
    else {
      to = 0;
      return V_EQ;
    }
  }
  Type mask = ((Type(1) << exp) - 1) << ((sizeof_to_bits(sizeof(Type)) - 1) - exp);
  Type n;
  if (x < 0) {
    if ((x & mask) != mask)
      return set_neg_overflow_int<To_Policy>(to, dir);
    n = x << exp;
    if (PPL_LT_SILENT(n, (Extended_Int<To_Policy, Type>::min)))
      return set_neg_overflow_int<To_Policy>(to, dir);
  }
  else {
    if (x & mask)
      return set_pos_overflow_int<To_Policy>(to, dir);
    n = x << exp;
    if (PPL_GT_SILENT(n, (Extended_Int<To_Policy, Type>::max)))
      return set_pos_overflow_int<To_Policy>(to, dir);
  }
  to = n;
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
smod_2exp_unsigned_int(Type& to, const Type x, unsigned int exp,
		       Rounding_Dir dir) {
  if (exp > sizeof_to_bits(sizeof(Type)))
    to = x;
  else {
    Type v = (exp == sizeof_to_bits(sizeof(Type)) ? x : (x & ((Type(1) << exp) - 1)));
    if (v >= (Type(1) << (exp - 1)))
      return set_neg_overflow_int<To_Policy>(to, dir);
    else
      to = v;
  }
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
smod_2exp_signed_int(Type& to, const Type x, unsigned int exp,
		     Rounding_Dir) {
  if (exp >= sizeof_to_bits(sizeof(Type)))
    to = x;
  else {
    Type m = Type(1) << (exp - 1);
    to = (x & (m - 1)) - (x & m);
  }
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
umod_2exp_unsigned_int(Type& to, const Type x, unsigned int exp,
		       Rounding_Dir) {
  if (exp >= sizeof_to_bits(sizeof(Type)))
    to = x;
  else
    to = x & ((Type(1) << exp) - 1);
  return V_EQ;
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
umod_2exp_signed_int(Type& to, const Type x, unsigned int exp,
		     Rounding_Dir dir) {
  if (exp >= sizeof_to_bits(sizeof(Type))) {
    if (x < 0)
      return set_pos_overflow_int<To_Policy>(to, dir);
    to = x;
  }
  else
    to = x & ((Type(1) << exp) - 1);
  return V_EQ;
}

template <typename Type>
inline void
isqrt_rem(Type& q, Type& r, const Type from) {
  q = 0;
  r = from;
  Type t(1);
  for (t <<= sizeof_to_bits(sizeof(Type)) - 2; t != 0; t >>= 2) {
    Type s = q + t;
    if (s <= r) {
      r -= s;
      q = s + t;
    }
    q >>= 1;
  }
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
sqrt_unsigned_int(Type& to, const Type from, Rounding_Dir dir) {
  Type rem;
  isqrt_rem(to, rem, from);
  if (round_not_requested(dir))
    return V_GE;
  if (rem == 0)
    return V_EQ;
  return round_gt_int<To_Policy>(to, dir);
}

template <typename To_Policy, typename From_Policy, typename Type>
inline Result
sqrt_signed_int(Type& to, const Type from, Rounding_Dir dir) {
  if (CHECK_P(To_Policy::check_sqrt_neg, from < 0)) {
    return assign_nan<To_Policy>(to, V_SQRT_NEG);
  }
  return sqrt_unsigned_int<To_Policy, From_Policy>(to, from, dir);
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
add_mul_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  Type z;
  Result r = mul<To_Policy, From1_Policy, From2_Policy>(z, x, y, dir);
  switch (result_overflow(r)) {
  case 0:
    return add<To_Policy, To_Policy, To_Policy>(to, to, z, dir);
  case -1:
    if (to <= 0)
      return set_neg_overflow_int<To_Policy>(to, dir);
    return assign_nan<To_Policy>(to, V_UNKNOWN_NEG_OVERFLOW);
  case 1:
    if (to >= 0)
      return set_pos_overflow_int<To_Policy>(to, dir);
    return assign_nan<To_Policy>(to, V_UNKNOWN_POS_OVERFLOW);
  default:
    PPL_UNREACHABLE;
    return V_NAN;
  }
}

template <typename To_Policy, typename From1_Policy, typename From2_Policy,
          typename Type>
inline Result
sub_mul_int(Type& to, const Type x, const Type y, Rounding_Dir dir) {
  Type z;
  Result r = mul<To_Policy, From1_Policy, From2_Policy>(z, x, y, dir);
  switch (result_overflow(r)) {
  case 0:
    return sub<To_Policy, To_Policy, To_Policy>(to, to, z, dir);
  case -1:
    if (to >= 0)
      return set_pos_overflow_int<To_Policy>(to, dir);
    return assign_nan<To_Policy>(to, V_UNKNOWN_NEG_OVERFLOW);
  case 1:
    if (to <= 0)
      return set_neg_overflow_int<To_Policy>(to, dir);
    return assign_nan<To_Policy>(to, V_UNKNOWN_POS_OVERFLOW);
  default:
    PPL_UNREACHABLE;
    return V_NAN;
  }
}

template <typename Policy, typename Type>
inline Result
output_char(std::ostream& os, Type& from,
	    const Numeric_Format&, Rounding_Dir) {
  os << int(from);
  return V_EQ;
}

template <typename Policy, typename Type>
inline Result
output_int(std::ostream& os, Type& from, const Numeric_Format&, Rounding_Dir) {
  os << from;
  return V_EQ;
}

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_FLOOR(assign_signed_int_signed_int, char, char)
#endif
PPL_SPECIALIZE_FLOOR(assign_signed_int_signed_int, signed char, signed char)
PPL_SPECIALIZE_FLOOR(assign_signed_int_signed_int, signed short, signed short)
PPL_SPECIALIZE_FLOOR(assign_signed_int_signed_int, signed int, signed int)
PPL_SPECIALIZE_FLOOR(assign_signed_int_signed_int, signed long, signed long)
PPL_SPECIALIZE_FLOOR(assign_signed_int_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_FLOOR(assign_unsigned_int_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_FLOOR(assign_unsigned_int_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_FLOOR(assign_unsigned_int_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_FLOOR(assign_unsigned_int_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_FLOOR(assign_unsigned_int_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_FLOOR(assign_unsigned_int_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_CEIL(assign_signed_int_signed_int, char, char)
#endif
PPL_SPECIALIZE_CEIL(assign_signed_int_signed_int, signed char, signed char)
PPL_SPECIALIZE_CEIL(assign_signed_int_signed_int, signed short, signed short)
PPL_SPECIALIZE_CEIL(assign_signed_int_signed_int, signed int, signed int)
PPL_SPECIALIZE_CEIL(assign_signed_int_signed_int, signed long, signed long)
PPL_SPECIALIZE_CEIL(assign_signed_int_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_CEIL(assign_unsigned_int_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_CEIL(assign_unsigned_int_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_CEIL(assign_unsigned_int_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_CEIL(assign_unsigned_int_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_CEIL(assign_unsigned_int_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_CEIL(assign_unsigned_int_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_TRUNC(assign_signed_int_signed_int, char, char)
#endif
PPL_SPECIALIZE_TRUNC(assign_signed_int_signed_int, signed char, signed char)
PPL_SPECIALIZE_TRUNC(assign_signed_int_signed_int, signed short, signed short)
PPL_SPECIALIZE_TRUNC(assign_signed_int_signed_int, signed int, signed int)
PPL_SPECIALIZE_TRUNC(assign_signed_int_signed_int, signed long, signed long)
PPL_SPECIALIZE_TRUNC(assign_signed_int_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_TRUNC(assign_unsigned_int_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_TRUNC(assign_unsigned_int_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_TRUNC(assign_unsigned_int_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_TRUNC(assign_unsigned_int_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_TRUNC(assign_unsigned_int_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_TRUNC(assign_unsigned_int_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_NEG(neg_signed_int, char, char)
#endif
PPL_SPECIALIZE_NEG(neg_signed_int, signed char, signed char)
PPL_SPECIALIZE_NEG(neg_signed_int, signed short, signed short)
PPL_SPECIALIZE_NEG(neg_signed_int, signed int, signed int)
PPL_SPECIALIZE_NEG(neg_signed_int, signed long, signed long)
PPL_SPECIALIZE_NEG(neg_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_NEG(neg_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_NEG(neg_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_NEG(neg_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_NEG(neg_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_NEG(neg_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_NEG(neg_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_ADD(add_signed_int, char, char, char)
#endif
PPL_SPECIALIZE_ADD(add_signed_int, signed char, signed char, signed char)
PPL_SPECIALIZE_ADD(add_signed_int, signed short, signed short, signed short)
PPL_SPECIALIZE_ADD(add_signed_int, signed int, signed int, signed int)
PPL_SPECIALIZE_ADD(add_signed_int, signed long, signed long, signed long)
PPL_SPECIALIZE_ADD(add_signed_int, signed long long, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_ADD(add_unsigned_int, char, char, char)
#endif
PPL_SPECIALIZE_ADD(add_unsigned_int, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_ADD(add_unsigned_int, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_ADD(add_unsigned_int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_ADD(add_unsigned_int, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_ADD(add_unsigned_int, unsigned long long, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_SUB(sub_signed_int, char, char, char)
#endif
PPL_SPECIALIZE_SUB(sub_signed_int, signed char, signed char, signed char)
PPL_SPECIALIZE_SUB(sub_signed_int, signed short, signed short, signed short)
PPL_SPECIALIZE_SUB(sub_signed_int, signed int, signed int, signed int)
PPL_SPECIALIZE_SUB(sub_signed_int, signed long, signed long, signed long)
PPL_SPECIALIZE_SUB(sub_signed_int, signed long long, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_SUB(sub_unsigned_int, char, char, char)
#endif
PPL_SPECIALIZE_SUB(sub_unsigned_int, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_SUB(sub_unsigned_int, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_SUB(sub_unsigned_int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_SUB(sub_unsigned_int, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_SUB(sub_unsigned_int, unsigned long long, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_MUL(mul_signed_int, char, char, char)
#endif
PPL_SPECIALIZE_MUL(mul_signed_int, signed char, signed char, signed char)
PPL_SPECIALIZE_MUL(mul_signed_int, signed short, signed short, signed short)
PPL_SPECIALIZE_MUL(mul_signed_int, signed int, signed int, signed int)
PPL_SPECIALIZE_MUL(mul_signed_int, signed long, signed long, signed long)
PPL_SPECIALIZE_MUL(mul_signed_int, signed long long, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_MUL(mul_unsigned_int, char, char, char)
#endif
PPL_SPECIALIZE_MUL(mul_unsigned_int, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_MUL(mul_unsigned_int, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_MUL(mul_unsigned_int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_MUL(mul_unsigned_int, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_MUL(mul_unsigned_int, unsigned long long, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_DIV(div_signed_int, char, char, char)
#endif
PPL_SPECIALIZE_DIV(div_signed_int, signed char, signed char, signed char)
PPL_SPECIALIZE_DIV(div_signed_int, signed short, signed short, signed short)
PPL_SPECIALIZE_DIV(div_signed_int, signed int, signed int, signed int)
PPL_SPECIALIZE_DIV(div_signed_int, signed long, signed long, signed long)
PPL_SPECIALIZE_DIV(div_signed_int, signed long long, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_DIV(div_unsigned_int, char, char, char)
#endif
PPL_SPECIALIZE_DIV(div_unsigned_int, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_DIV(div_unsigned_int, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_DIV(div_unsigned_int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_DIV(div_unsigned_int, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_DIV(div_unsigned_int, unsigned long long, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_IDIV(idiv_signed_int, char, char, char)
#endif
PPL_SPECIALIZE_IDIV(idiv_signed_int, signed char, signed char, signed char)
PPL_SPECIALIZE_IDIV(idiv_signed_int, signed short, signed short, signed short)
PPL_SPECIALIZE_IDIV(idiv_signed_int, signed int, signed int, signed int)
PPL_SPECIALIZE_IDIV(idiv_signed_int, signed long, signed long, signed long)
PPL_SPECIALIZE_IDIV(idiv_signed_int, signed long long, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_IDIV(idiv_unsigned_int, char, char, char)
#endif
PPL_SPECIALIZE_IDIV(idiv_unsigned_int, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_IDIV(idiv_unsigned_int, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_IDIV(idiv_unsigned_int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_IDIV(idiv_unsigned_int, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_IDIV(idiv_unsigned_int, unsigned long long, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_REM(rem_signed_int, char, char, char)
#endif
PPL_SPECIALIZE_REM(rem_signed_int, signed char, signed char, signed char)
PPL_SPECIALIZE_REM(rem_signed_int, signed short, signed short, signed short)
PPL_SPECIALIZE_REM(rem_signed_int, signed int, signed int, signed int)
PPL_SPECIALIZE_REM(rem_signed_int, signed long, signed long, signed long)
PPL_SPECIALIZE_REM(rem_signed_int, signed long long, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_REM(rem_unsigned_int, char, char, char)
#endif
PPL_SPECIALIZE_REM(rem_unsigned_int, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_REM(rem_unsigned_int, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_REM(rem_unsigned_int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_REM(rem_unsigned_int, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_REM(rem_unsigned_int, unsigned long long, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_ADD_2EXP(add_2exp_signed_int, char, char)
#endif
PPL_SPECIALIZE_ADD_2EXP(add_2exp_signed_int, signed char, signed char)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_signed_int, signed short, signed short)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_signed_int, signed int, signed int)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_signed_int, signed long, signed long)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_ADD_2EXP(add_2exp_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_ADD_2EXP(add_2exp_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_ADD_2EXP(add_2exp_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_signed_int, char, char)
#endif
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_signed_int, signed char, signed char)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_signed_int, signed short, signed short)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_signed_int, signed int, signed int)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_signed_int, signed long, signed long)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_SUB_2EXP(sub_2exp_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_signed_int, char, char)
#endif
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_signed_int, signed char, signed char)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_signed_int, signed short, signed short)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_signed_int, signed int, signed int)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_signed_int, signed long, signed long)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_MUL_2EXP(mul_2exp_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_DIV_2EXP(div_2exp_signed_int, char, char)
#endif
PPL_SPECIALIZE_DIV_2EXP(div_2exp_signed_int, signed char, signed char)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_signed_int, signed short, signed short)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_signed_int, signed int, signed int)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_signed_int, signed long, signed long)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_DIV_2EXP(div_2exp_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_DIV_2EXP(div_2exp_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_DIV_2EXP(div_2exp_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_signed_int, char, char)
#endif
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_signed_int, signed char, signed char)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_signed_int, signed short, signed short)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_signed_int, signed int, signed int)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_signed_int, signed long, signed long)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_SMOD_2EXP(smod_2exp_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_signed_int, char, char)
#endif
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_signed_int, signed char, signed char)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_signed_int, signed short, signed short)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_signed_int, signed int, signed int)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_signed_int, signed long, signed long)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_UMOD_2EXP(umod_2exp_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_SQRT(sqrt_signed_int, char, char)
#endif
PPL_SPECIALIZE_SQRT(sqrt_signed_int, signed char, signed char)
PPL_SPECIALIZE_SQRT(sqrt_signed_int, signed short, signed short)
PPL_SPECIALIZE_SQRT(sqrt_signed_int, signed int, signed int)
PPL_SPECIALIZE_SQRT(sqrt_signed_int, signed long, signed long)
PPL_SPECIALIZE_SQRT(sqrt_signed_int, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_SQRT(sqrt_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_SQRT(sqrt_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_SQRT(sqrt_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_SQRT(sqrt_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_SQRT(sqrt_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_SQRT(sqrt_unsigned_int, unsigned long long, unsigned long long)

#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_ABS(abs_generic, char, char)
#endif
PPL_SPECIALIZE_ABS(abs_generic, signed char, signed char)
PPL_SPECIALIZE_ABS(abs_generic, signed short, signed short)
PPL_SPECIALIZE_ABS(abs_generic, signed int, signed int)
PPL_SPECIALIZE_ABS(abs_generic, signed long, signed long)
PPL_SPECIALIZE_ABS(abs_generic, signed long long, signed long long)
#if !PPL_CXX_PLAIN_CHAR_IS_SIGNED
PPL_SPECIALIZE_ABS(assign_unsigned_int_unsigned_int, char, char)
#endif
PPL_SPECIALIZE_ABS(assign_unsigned_int_unsigned_int, unsigned char, unsigned char)
PPL_SPECIALIZE_ABS(assign_unsigned_int_unsigned_int, unsigned short, unsigned short)
PPL_SPECIALIZE_ABS(assign_unsigned_int_unsigned_int, unsigned int, unsigned int)
PPL_SPECIALIZE_ABS(assign_unsigned_int_unsigned_int, unsigned long, unsigned long)
PPL_SPECIALIZE_ABS(assign_unsigned_int_unsigned_int, unsigned long long, unsigned long long)

PPL_SPECIALIZE_GCD(gcd_exact, char, char, char)
PPL_SPECIALIZE_GCD(gcd_exact, signed char, signed char, signed char)
PPL_SPECIALIZE_GCD(gcd_exact, signed short, signed short, signed short)
PPL_SPECIALIZE_GCD(gcd_exact, signed int, signed int, signed int)
PPL_SPECIALIZE_GCD(gcd_exact, signed long, signed long, signed long)
PPL_SPECIALIZE_GCD(gcd_exact, signed long long, signed long long, signed long long)
PPL_SPECIALIZE_GCD(gcd_exact, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_GCD(gcd_exact, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_GCD(gcd_exact, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_GCD(gcd_exact, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_GCD(gcd_exact, unsigned long long, unsigned long long, unsigned long long)

PPL_SPECIALIZE_GCDEXT(gcdext_exact, char, char, char, char, char)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, signed char, signed char, signed char, signed char, signed char)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, signed short, signed short, signed short, signed short, signed short)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, signed int, signed int, signed int, signed int, signed int)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, signed long, signed long, signed long, signed long, signed long)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, signed long long, signed long long, signed long long, signed long long, signed long long)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_GCDEXT(gcdext_exact, unsigned long long, unsigned long long, unsigned long long, unsigned long long, unsigned long long)

PPL_SPECIALIZE_LCM(lcm_gcd_exact, char, char, char)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, signed char, signed char, signed char)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, signed short, signed short, signed short)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, signed int, signed int, signed int)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, signed long, signed long, signed long)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, signed long long, signed long long, signed long long)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_LCM(lcm_gcd_exact, unsigned long long, unsigned long long, unsigned long long)

PPL_SPECIALIZE_SGN(sgn_generic, char)
PPL_SPECIALIZE_SGN(sgn_generic, signed char)
PPL_SPECIALIZE_SGN(sgn_generic, signed short)
PPL_SPECIALIZE_SGN(sgn_generic, signed int)
PPL_SPECIALIZE_SGN(sgn_generic, signed long)
PPL_SPECIALIZE_SGN(sgn_generic, signed long long)
PPL_SPECIALIZE_SGN(sgn_generic, unsigned char)
PPL_SPECIALIZE_SGN(sgn_generic, unsigned short)
PPL_SPECIALIZE_SGN(sgn_generic, unsigned int)
PPL_SPECIALIZE_SGN(sgn_generic, unsigned long)
PPL_SPECIALIZE_SGN(sgn_generic, unsigned long long)

PPL_SPECIALIZE_CMP(cmp_generic, char, char)
PPL_SPECIALIZE_CMP(cmp_generic, signed char, signed char)
PPL_SPECIALIZE_CMP(cmp_generic, signed short, signed short)
PPL_SPECIALIZE_CMP(cmp_generic, signed int, signed int)
PPL_SPECIALIZE_CMP(cmp_generic, signed long, signed long)
PPL_SPECIALIZE_CMP(cmp_generic, signed long long, signed long long)
PPL_SPECIALIZE_CMP(cmp_generic, unsigned char, unsigned char)
PPL_SPECIALIZE_CMP(cmp_generic, unsigned short, unsigned short)
PPL_SPECIALIZE_CMP(cmp_generic, unsigned int, unsigned int)
PPL_SPECIALIZE_CMP(cmp_generic, unsigned long, unsigned long)
PPL_SPECIALIZE_CMP(cmp_generic, unsigned long long, unsigned long long)

PPL_SPECIALIZE_ADD_MUL(add_mul_int, char, char, char)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, signed char, signed char, signed char)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, signed short, signed short, signed short)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, signed int, signed int, signed int)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, signed long, signed long, signed long)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, signed long long, signed long long, signed long long)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_ADD_MUL(add_mul_int, unsigned long long, unsigned long long, unsigned long long)

PPL_SPECIALIZE_SUB_MUL(sub_mul_int, char, char, char)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, signed char, signed char, signed char)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, signed short, signed short, signed short)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, signed int, signed int, signed int)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, signed long, signed long, signed long)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, signed long long, signed long long, signed long long)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, unsigned char, unsigned char, unsigned char)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, unsigned short, unsigned short, unsigned short)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, unsigned int, unsigned int, unsigned int)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, unsigned long, unsigned long, unsigned long)
PPL_SPECIALIZE_SUB_MUL(sub_mul_int, unsigned long long, unsigned long long, unsigned long long)

PPL_SPECIALIZE_INPUT(input_generic, char)
PPL_SPECIALIZE_INPUT(input_generic, signed char)
PPL_SPECIALIZE_INPUT(input_generic, signed short)
PPL_SPECIALIZE_INPUT(input_generic, signed int)
PPL_SPECIALIZE_INPUT(input_generic, signed long)
PPL_SPECIALIZE_INPUT(input_generic, signed long long)
PPL_SPECIALIZE_INPUT(input_generic, unsigned char)
PPL_SPECIALIZE_INPUT(input_generic, unsigned short)
PPL_SPECIALIZE_INPUT(input_generic, unsigned int)
PPL_SPECIALIZE_INPUT(input_generic, unsigned long)
PPL_SPECIALIZE_INPUT(input_generic, unsigned long long)

PPL_SPECIALIZE_OUTPUT(output_char, char)
PPL_SPECIALIZE_OUTPUT(output_char, signed char)
PPL_SPECIALIZE_OUTPUT(output_int, signed short)
PPL_SPECIALIZE_OUTPUT(output_int, signed int)
PPL_SPECIALIZE_OUTPUT(output_int, signed long)
PPL_SPECIALIZE_OUTPUT(output_int, signed long long)
PPL_SPECIALIZE_OUTPUT(output_char, unsigned char)
PPL_SPECIALIZE_OUTPUT(output_int, unsigned short)
PPL_SPECIALIZE_OUTPUT(output_int, unsigned int)
PPL_SPECIALIZE_OUTPUT(output_int, unsigned long)
PPL_SPECIALIZE_OUTPUT(output_int, unsigned long long)

} // namespace Checked

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_checked_int_inlines_hh)
