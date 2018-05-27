/* C integers info.
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

#ifndef PPL_C_Integer_hh
#define PPL_C_Integer_hh 1

#include "meta_programming.hh"
#include <climits>

// C99 defines LLONG_MIN, LLONG_MAX and ULLONG_MAX, but this part of
// C99 is not yet included into the C++ standard.
// GCC defines LONG_LONG_MIN, LONG_LONG_MAX and ULONG_LONG_MAX.
// Some compilers (such as Comeau C++ up to and including version 4.3.3)
// define nothing.  In this last case we make a reasonable guess.
#ifndef LLONG_MIN
#if defined(LONG_LONG_MIN)
#define LLONG_MIN LONG_LONG_MIN
#elif PPL_SIZEOF_LONG_LONG == 8
#define LLONG_MIN 0x8000000000000000LL
#endif
#endif

#ifndef LLONG_MAX
#if defined(LONG_LONG_MAX)
#define LLONG_MAX LONG_LONG_MAX
#elif PPL_SIZEOF_LONG_LONG == 8
#define LLONG_MAX 0x7fffffffffffffffLL
#endif
#endif

#ifndef ULLONG_MAX
#if defined(ULONG_LONG_MAX)
#define ULLONG_MAX ULONG_LONG_MAX
#elif PPL_SIZEOF_LONG_LONG == 8
#define ULLONG_MAX 0xffffffffffffffffULL
#endif
#endif

namespace Parma_Polyhedra_Library {

template <typename T>
struct C_Integer : public False { };

template <>
struct C_Integer<char> : public True {
  enum const_bool_value {
#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
    is_signed = true
#else
    is_signed = false
#endif
  };
  typedef void smaller_type;
  typedef void smaller_signed_type;
  typedef void smaller_unsigned_type;
#if PPL_CXX_PLAIN_CHAR_IS_SIGNED
  typedef unsigned char other_type;
#else
  typedef signed char other_type;
#endif
  static const char min = static_cast<char>(CHAR_MIN);
  static const char max = static_cast<char>(CHAR_MAX);
};

template <>
struct C_Integer<signed char> : public True {
  enum const_bool_value { is_signed = true };
  typedef void smaller_type;
  typedef void smaller_signed_type;
  typedef void smaller_unsigned_type;
  typedef unsigned char other_type;
  static const signed char min = SCHAR_MIN;
  static const signed char max = SCHAR_MAX;
};

template <>
struct C_Integer<signed short> : public True {
  enum const_bool_value { is_signed = true };
  typedef signed char smaller_type;
  typedef signed char smaller_signed_type;
  typedef unsigned char smaller_unsigned_type;
  typedef unsigned short other_type;
  static const signed short min = SHRT_MIN;
  static const signed short max = SHRT_MAX;
};

template <>
struct C_Integer<signed int> : public True {
  enum const_bool_value { is_signed = true };
  typedef signed short smaller_type;
  typedef signed short smaller_signed_type;
  typedef unsigned short smaller_unsigned_type;
  typedef unsigned int other_type;
  static const signed int min = INT_MIN;
  static const signed int max = INT_MAX;
};

template <>
struct C_Integer<signed long> : public True {
  enum const_bool_value { is_signed = true };
  typedef signed int smaller_type;
  typedef signed int smaller_signed_type;
  typedef unsigned int smaller_unsigned_type;
  typedef unsigned long other_type;
  static const signed long min = LONG_MIN;
  static const signed long max = LONG_MAX;
};

template <>
struct C_Integer<signed long long> : public True {
  enum const_bool_value { is_signed = true };
  typedef signed long smaller_type;
  typedef signed long smaller_signed_type;
  typedef unsigned long smaller_unsigned_type;
  typedef unsigned long long other_type;
  static const signed long long min = LLONG_MIN;
  static const signed long long max = LLONG_MAX;
};

template <>
struct C_Integer<unsigned char> : public True {
  enum const_bool_value { is_signed = false };
  typedef void smaller_type;
  typedef void smaller_signed_type;
  typedef void smaller_unsigned_type;
  typedef signed char other_type;
  static const unsigned char min = 0U;
  static const unsigned char max = static_cast<unsigned char>(~0U);
};

template <>
struct C_Integer<unsigned short> : public True {
  enum const_bool_value { is_signed = false };
  typedef unsigned char smaller_type;
  typedef signed char smaller_signed_type;
  typedef unsigned char smaller_unsigned_type;
  typedef signed short other_type;
  static const unsigned short min = 0U;
  static const unsigned short max = static_cast<unsigned short>(~0U);
};

template <>
struct C_Integer<unsigned int> : public True {
  enum const_bool_value { is_signed = false };
  typedef unsigned short smaller_type;
  typedef signed short smaller_signed_type;
  typedef unsigned short smaller_unsigned_type;
  typedef signed int other_type;
  static const unsigned int min = 0U;
  static const unsigned int max = ~0U;
};

template <>
struct C_Integer<unsigned long> : public True {
  enum const_bool_value { is_signed = false };
  typedef unsigned int smaller_type;
  typedef signed int smaller_signed_type;
  typedef unsigned int smaller_unsigned_type;
  typedef signed long other_type;
  static const unsigned long min = 0UL;
  static const unsigned long max = ~0UL;
};

template <>
struct C_Integer<unsigned long long> : public True {
  enum const_bool_value { is_signed = false };
  typedef unsigned long smaller_type;
  typedef signed long smaller_signed_type;
  typedef unsigned long smaller_unsigned_type;
  typedef signed long long other_type;
  static const unsigned long long min = 0ULL;
  static const unsigned long long max = ~0ULL;
};

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_C_Integer_hh)
