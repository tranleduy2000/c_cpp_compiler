/* C++ compiler related stuff.
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

#ifndef PPL_compiler_hh
#define PPL_compiler_hh 1

#include <cstddef>
#include <climits>
#include <cassert>

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  No-op macro that allows to avoid unused variable warnings from
  the compiler.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define PPL_USED(v) (void)(v)

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  No-op function that force the compiler to store the argument and
  to reread it from memory if needed (thus preventing CSE).
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
inline void
PPL_CC_FLUSH(const T& x) {
#if defined(__GNUC__) || defined(__INTEL_COMPILER)
  __asm__ __volatile__ ("" : "+m" (const_cast<T&>(x)));
#else
  // FIXME: is it possible to achieve the same effect in a portable way?
  PPL_USED(x);
#endif
}

#ifndef PPL_SUPPRESS_UNINIT_WARNINGS
#define PPL_SUPPRESS_UNINIT_WARNINGS 1
#endif

#ifndef PPL_SUPPRESS_UNINITIALIZED_WARNINGS
#define PPL_SUPPRESS_UNINITIALIZED_WARNINGS 1
#endif

#if PPL_SUPPRESS_UNINITIALIZED_WARNINGS
template <typename T>
struct Suppress_Uninitialized_Warnings_Type {
  typedef T synonym;
};

#define PPL_UNINITIALIZED(type, name)                                   \
  PPL_U(type) PPL_U(name)                                               \
  = Suppress_Uninitialized_Warnings_Type<PPL_U(type)>::synonym ()
#else
#define PPL_UNINITIALIZED(type, name)           \
  PPL_U(type) name
#endif

#define sizeof_to_bits(size)                    \
  ((size) * static_cast<std::size_t>(CHAR_BIT))

#if !defined(__GNUC__)

inline unsigned int
clz32(uint32_t w) {
  unsigned int r = 31;
  if ((w & 0xffff0000U) != 0) {
    w >>= 16;
    r -= 16;
  }
  if ((w & 0xff00U) != 0) {
    w >>= 8;
    r -= 8;
  }
  if ((w & 0xf0U) != 0) {
    w >>= 4;
    r -= 4;
  }
  if ((w & 0xcU) != 0) {
    w >>= 2;
    r -= 2;
  }
  if ((w & 0x2U) != 0)
    r -= 1;
  return r;
}

inline unsigned int
clz64(uint64_t w) {
  if ((w & 0xffffffff00000000ULL) == 0)
    return clz32(static_cast<uint32_t>(w)) + 32;
  else
    return clz32(static_cast<uint32_t>(w >> 32));
}

inline unsigned int
ctz32(uint32_t w) {
  static const unsigned int mod37_table[] = {
    32, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13,
    4, 7, 17, 0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9,
    5, 20, 8, 19, 18
  };
  return mod37_table[(w & -w) % 37];
}

inline unsigned int
ctz64(uint64_t w) {
  if ((w & 0x00000000ffffffffULL) == 0)
    return ctz32(static_cast<uint32_t>(w >> 32)) + 32;
  else
    return ctz32(static_cast<uint32_t>(w));
}

#endif

inline unsigned int
clz(unsigned int u) {
  assert(u != 0);
#if defined(__GNUC__)
  return static_cast<unsigned int>(__builtin_clz(u));
#elif PPL_SIZEOF_INT == 4
  return clz32(u);
#elif PPL_SIZEOF_INT == 8
  return clz64(u);
#else
  #error "Unsupported unsigned int size"
#endif
}

inline unsigned int
clz(unsigned long ul) {
  assert(ul != 0);
#if defined(__GNUC__)
  return static_cast<unsigned int>(__builtin_clzl(ul));
#elif PPL_SIZEOF_LONG == 4
  return clz32(ul);
#elif PPL_SIZEOF_LONG == 8
  return clz64(ul);
#else
  #error "Unsupported unsigned long size"
#endif
}

inline unsigned int
clz(unsigned long long ull) {
  assert(ull != 0);
#if defined(__GNUC__)
  return static_cast<unsigned int>(__builtin_clzll(ull));
#elif PPL_SIZEOF_LONG_LONG == 4
  return clz32(ull);
#elif PPL_SIZEOF_LONG_LONG == 8
  return clz64(ull);
#else
  #error "Unsupported unsigned long long size"
#endif
}


inline unsigned int
ctz(unsigned int u) {
  assert(u != 0);
#if defined(__GNUC__)
  return static_cast<unsigned int>(__builtin_ctz(u));
#elif PPL_SIZEOF_INT == 4
  return ctz32(u);
#elif PPL_SIZEOF_INT == 8
  return ctz64(u);
#else
  #error "Unsupported unsigned int size"
#endif
}

inline unsigned int
ctz(unsigned long ul) {
  assert(ul != 0);
#if defined(__GNUC__)
  return static_cast<unsigned int>(__builtin_ctzl(ul));
#elif PPL_SIZEOF_LONG == 4
  return ctz32(ul);
#elif PPL_SIZEOF_LONG == 8
  return ctz64(ul);
#else
  #error "Unsupported unsigned long size"
#endif
}

inline unsigned int
ctz(unsigned long long ull) {
  assert(ull != 0);
#if defined(__GNUC__)
  return static_cast<unsigned int>(__builtin_ctzll(ull));
#elif PPL_SIZEOF_LONG_LONG == 4
  return ctz32(ull);
#elif PPL_SIZEOF_LONG_LONG == 8
  return ctz64(ull);
#else
  #error "Unsupported unsigned long long size"
#endif
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_compiler_hh)
