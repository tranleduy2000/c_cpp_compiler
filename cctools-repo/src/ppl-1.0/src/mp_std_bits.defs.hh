/* Declarations of specializations of std:: objects for
   multi-precision types.
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

#ifndef PPL_mp_std_bits_defs_hh
#define PPL_mp_std_bits_defs_hh 1

#include <gmpxx.h>
#include <limits>

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void swap(mpz_class& x, mpz_class& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void swap(mpq_class& x, mpq_class& y);

namespace std {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Specialization of std::numeric_limits.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <>
class numeric_limits<mpz_class> {
private:
  typedef mpz_class Type;

public:
  static const bool is_specialized = true;
  static const int digits = 0;
  static const int digits10 = 0;
  static const bool is_signed = true;
  static const bool is_integer = true;
  static const bool is_exact = true;
  static const int radix = 2;
  static const int min_exponent = 0;
  static const int min_exponent10 = 0;
  static const int max_exponent = 0;
  static const int max_exponent10 = 0;
  static const bool has_infinity = false;
  static const bool has_quiet_NaN =  false;
  static const bool has_signaling_NaN = false;
  static const float_denorm_style has_denorm = denorm_absent;
  static const bool has_denorm_loss = false;
  static const bool is_iec559 = false;
  static const bool is_bounded = false;
  static const bool is_modulo = false;
  static const bool traps = false;
  static const bool tininess_before = false;
  static const float_round_style round_style = round_toward_zero;

  static Type min() {
    return static_cast<Type>(0);
  }

  static Type max() {
    return static_cast<Type>(0);
  }

  static Type epsilon() {
    return static_cast<Type>(0);
  }

  static Type round_error() {
    return static_cast<Type>(0);
  }

  static Type infinity() {
    return static_cast<Type>(0);
  }

  static Type quiet_NaN() {
    return static_cast<Type>(0);
  }

  static Type denorm_min() {
    return static_cast<Type>(1);
  }
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Specialization of std::numeric_limits.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <>
class numeric_limits<mpq_class> {
private:
  typedef mpq_class Type;

public:
  static const bool is_specialized = true;
  static const int digits = 0;
  static const int digits10 = 0;
  static const bool is_signed = true;
  static const bool is_integer = false;
  static const bool is_exact = true;
  static const int radix = 2;
  static const int min_exponent = 0;
  static const int min_exponent10 = 0;
  static const int max_exponent = 0;
  static const int max_exponent10 = 0;
  static const bool has_infinity = false;
  static const bool has_quiet_NaN =  false;
  static const bool has_signaling_NaN = false;
  static const float_denorm_style has_denorm = denorm_absent;
  static const bool has_denorm_loss = false;
  static const bool is_iec559 = false;
  static const bool is_bounded = false;
  static const bool is_modulo = false;
  static const bool traps = false;
  static const bool tininess_before = false;
  static const float_round_style round_style = round_toward_zero;

  static Type min() {
    return static_cast<Type>(0);
  }

  static Type max() {
    return static_cast<Type>(0);
  }

  static Type epsilon() {
    return static_cast<Type>(0);
  }

  static Type round_error() {
    return static_cast<Type>(0);
  }

  static Type infinity() {
    return static_cast<Type>(0);
  }

  static Type quiet_NaN() {
    return static_cast<Type>(0);
  }

  static Type denorm_min() {
    return static_cast<Type>(0);
  }
};

} // namespace std

#include "mp_std_bits.inlines.hh"

#endif // !defined(PPL_mp_std_bits_defs_hh)
