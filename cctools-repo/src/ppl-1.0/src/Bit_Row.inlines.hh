/* Bit_Row class implementation: inline functions.
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

#ifndef PPL_Bit_Row_inlines_hh
#define PPL_Bit_Row_inlines_hh 1

#include "compiler.hh"
#include "globals.defs.hh"
#include "assert.hh"

// For the declaration of ffs(3).
#if defined(PPL_HAVE_STRINGS_H)
# include <strings.h>
#elif defined(PPL_HAVE_STRING_H)
# include <string.h>
#endif

#define PPL_BITS_PER_GMP_LIMB sizeof_to_bits(PPL_SIZEOF_MP_LIMB_T)

namespace Parma_Polyhedra_Library {

inline
Bit_Row::Bit_Row() {
  mpz_init(vec);
}

inline
Bit_Row::Bit_Row(const Bit_Row& y) {
  mpz_init_set(vec, y.vec);
}

inline
Bit_Row::Bit_Row(const Bit_Row& y, const Bit_Row& z) {
  const mp_size_t y_size = y.vec->_mp_size;
  PPL_ASSERT(y_size >= 0);
  const mp_size_t z_size = z.vec->_mp_size;
  PPL_ASSERT(z_size >= 0);
  if (y_size < z_size) {
    PPL_ASSERT(static_cast<unsigned long>(z_size)
               <= C_Integer<unsigned long>::max / PPL_BITS_PER_GMP_LIMB);
    mpz_init2(vec, static_cast<unsigned long>(z_size) * PPL_BITS_PER_GMP_LIMB);
    union_helper(y, z);
  }
  else {
    PPL_ASSERT(static_cast<unsigned long>(y_size)
               <= C_Integer<unsigned long>::max / PPL_BITS_PER_GMP_LIMB);
    mpz_init2(vec, static_cast<unsigned long>(y_size) * PPL_BITS_PER_GMP_LIMB);
    union_helper(z, y);
  }
}

inline
Bit_Row::~Bit_Row() {
  mpz_clear(vec);
}

inline Bit_Row&
Bit_Row::operator=(const Bit_Row& y) {
  mpz_set(vec, y.vec);
  return *this;
}

inline void
Bit_Row::set(const unsigned long k) {
  mpz_setbit(vec, k);
}

inline void
Bit_Row::clear(const unsigned long k) {
  mpz_clrbit(vec, k);
}

inline void
Bit_Row::clear_from(const unsigned long k) {
  mpz_tdiv_r_2exp(vec, vec, k);
}

inline unsigned long
Bit_Row::count_ones() const {
  mp_size_t x_size = vec->_mp_size;
  PPL_ASSERT(x_size >= 0);
  return (x_size == 0) ? 0 : mpn_popcount(vec->_mp_d, x_size);
}

inline bool
Bit_Row::empty() const {
  return mpz_sgn(vec) == 0;
}

inline void
Bit_Row::m_swap(Bit_Row& y) {
  mpz_swap(vec, y.vec);
}

inline void
Bit_Row::clear() {
  mpz_set_ui(vec, 0UL);
}

inline memory_size_type
Bit_Row::external_memory_in_bytes() const {
  return static_cast<memory_size_type>(vec[0]._mp_alloc) * PPL_SIZEOF_MP_LIMB_T;
}

inline memory_size_type
Bit_Row::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline void
Bit_Row::union_assign(const Bit_Row& x, const Bit_Row& y) {
  const mp_size_t x_size = x.vec->_mp_size;
  PPL_ASSERT(x_size >= 0);
  const mp_size_t y_size = y.vec->_mp_size;
  PPL_ASSERT(y_size >= 0);
  if (x_size < y_size) {
    PPL_ASSERT(static_cast<unsigned long>(y_size)
               <= C_Integer<unsigned long>::max / PPL_BITS_PER_GMP_LIMB);
    mpz_realloc2(vec, static_cast<unsigned long>(y_size) * PPL_BITS_PER_GMP_LIMB);
    union_helper(x, y);
  }
  else {
    PPL_ASSERT(static_cast<unsigned long>(x_size)
               <= C_Integer<unsigned long>::max / PPL_BITS_PER_GMP_LIMB);
    mpz_realloc2(vec, static_cast<unsigned long>(x_size) * PPL_BITS_PER_GMP_LIMB);
    union_helper(y, x);
  }
}

inline void
Bit_Row::intersection_assign(const Bit_Row& x, const Bit_Row& y) {
  mpz_and(vec, x.vec, y.vec);
}

inline void
Bit_Row::difference_assign(const Bit_Row& x, const Bit_Row& y) {
  PPL_DIRTY_TEMP(mpz_class, complement_y);
  mpz_com(complement_y.get_mpz_t(), y.vec);
  mpz_and(vec, x.vec, complement_y.get_mpz_t());
}

namespace Implementation {

/*! \brief
  Assuming \p u is nonzero, returns the index of the first set bit in \p u.
*/
inline unsigned int
first_one(unsigned int u) {
  return ctz(u);
}

/*! \brief
  Assuming \p ul is nonzero, returns the index of the first set bit in
  \p ul.
*/
inline unsigned int
first_one(unsigned long ul) {
  return ctz(ul);
}

/*! \brief
  Assuming \p ull is nonzero, returns the index of the first set bit in
  \p ull.
*/
inline unsigned int
first_one(unsigned long long ull) {
  return ctz(ull);
}

/*! \brief
  Assuming \p u is nonzero, returns the index of the last set bit in \p u.
*/
inline unsigned int
last_one(unsigned int u) {
  return static_cast<unsigned int>(sizeof_to_bits(sizeof(u)))
    - 1U - clz(u);
}

/*! \brief
  Assuming \p ul is nonzero, returns the index of the last set bit in
  \p ul.
*/
inline unsigned int
last_one(unsigned long ul) {
  return static_cast<unsigned int>(sizeof_to_bits(sizeof(ul)))
    - 1U - clz(ul);
}

/*! \brief
  Assuming \p ull is nonzero, returns the index of the last set bit in
  \p ull.
*/
inline unsigned int
last_one(unsigned long long ull) {
  return static_cast<unsigned int>(sizeof_to_bits(sizeof(ull)))
    - 1U - clz(ull);
}

} // namespace Implementation

/*! \relates Bit_Row */
inline void
swap(Bit_Row& x, Bit_Row& y) {
  x.m_swap(y);
}

/*! \relates Bit_Row */
inline void
iter_swap(std::vector<Bit_Row>::iterator x,
          std::vector<Bit_Row>::iterator y) {
  swap(*x, *y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Bit_Row_inlines_hh)
