/* Bit_Row class implementation (non-inline functions).
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

#include "ppl-config.h"
#include "Bit_Row.defs.hh"
#include "assert.hh"
#include "C_Integer.hh"

namespace PPL = Parma_Polyhedra_Library;

unsigned long
PPL::Bit_Row::first() const {
  const mp_size_t vec_size = vec->_mp_size;
  PPL_ASSERT(vec_size >= 0);
  mp_srcptr p = vec->_mp_d;
  for (mp_size_t li = 0; li < vec_size; ++li, ++p) {
    const mp_limb_t limb = *p;
    if (limb != 0)
      return static_cast<unsigned long>(li) * PPL_BITS_PER_GMP_LIMB
        + Implementation::first_one(limb);
  }
  return C_Integer<unsigned long>::max;
}

unsigned long
PPL::Bit_Row::next(unsigned long position) const {
  ++position;

  // The alternative implementation using the mpz_scan1() function
  // of GMP was measured to be slower that ours.  Here it is, in
  // case mpz_scan1() is improved.
  //
  // <CODE>
  //   unsigned long r = mpz_scan1(vec, position);
  //   return (r == C_Integer<unsigned long>::max) ? -1 : r;
  // </CODE>

  const unsigned long uli = position / PPL_BITS_PER_GMP_LIMB;
  mp_size_t li = static_cast<mp_size_t>(uli);
  const mp_size_t vec_size = vec->_mp_size;
  PPL_ASSERT(vec_size >= 0);
  if (li >= vec_size)
    return C_Integer<unsigned long>::max;

  // Get the first limb.
  mp_srcptr p = vec->_mp_d + li;

  // Mask off any bits before `position' in the first limb.
  mp_limb_t limb
    = *p & ((~static_cast<mp_limb_t>(0)) << (position % PPL_BITS_PER_GMP_LIMB));

  while (true) {
    if (limb != 0)
      return static_cast<unsigned long>(li) * PPL_BITS_PER_GMP_LIMB
        + Implementation::first_one(limb);
    ++li;
    if (li == vec_size)
      break;
    ++p;
    limb = *p;
  }
  return C_Integer<unsigned long>::max;
}

unsigned long
PPL::Bit_Row::last() const {
  mp_size_t li = vec->_mp_size;
  PPL_ASSERT(li >= 0);
  if (li == 0)
    return C_Integer<unsigned long>::max;
  --li;
  const mp_srcptr p = vec->_mp_d + li;
  const mp_limb_t limb = *p;
  PPL_ASSERT(limb != 0);
  return static_cast<unsigned long>(li) * PPL_BITS_PER_GMP_LIMB
    + Implementation::last_one(limb);
}

unsigned long
PPL::Bit_Row::prev(unsigned long position) const {
  if (position == 0)
    return C_Integer<unsigned long>::max;

  --position;

  const mp_size_t vec_size = vec->_mp_size;
  PPL_ASSERT(vec_size > 0);
  const unsigned long uli = position / PPL_BITS_PER_GMP_LIMB;
  mp_size_t li = static_cast<mp_size_t>(uli);

  mp_limb_t limb;
  mp_srcptr p = vec->_mp_d;

  // Get the first limb.
  if (li >= vec_size) {
    li = vec_size - 1;
    p += li;
    limb = *p;
  }
  else {
    const mp_limb_t mask
      = (~static_cast<mp_limb_t>(0))
      >> (PPL_BITS_PER_GMP_LIMB - 1U - position % PPL_BITS_PER_GMP_LIMB);
    p += li;
    limb = *p & mask;
  }

  while (true) {
    if (limb != 0)
      return static_cast<unsigned long>(li) * PPL_BITS_PER_GMP_LIMB 
        + Implementation::last_one(limb);
    if (li == 0)
      break;
    --li;
    --p;
    limb = *p;
  }
  return C_Integer<unsigned long>::max;
}

bool
PPL::Bit_Row::operator[](const unsigned long k) const {
  const mp_size_t vec_size = vec->_mp_size;
  PPL_ASSERT(vec_size >= 0);

  unsigned long i = k / static_cast<unsigned long>(GMP_NUMB_BITS);
  if (i >= static_cast<unsigned long>(vec_size))
    return false;

  mp_limb_t limb = *(vec->_mp_d + i);
  return ((limb >> (k % static_cast<unsigned long>(GMP_NUMB_BITS))) & 1U) != 0;
}

void
PPL::Bit_Row::set_until(unsigned long k) {
  // FIXME, TODO: this is an inefficient implementation.
  while (k-- > 0)
    mpz_setbit(vec, k);
}

/*! \relates Parma_Polyhedra_Library::Bit_Row */
int
PPL::compare(const Bit_Row& x, const Bit_Row& y) {
  const mp_size_t x_size = x.vec->_mp_size;
  PPL_ASSERT(x_size >= 0);
  const mp_size_t y_size = y.vec->_mp_size;
  PPL_ASSERT(y_size >= 0);
  mp_size_t size = ((x_size > y_size) ? y_size : x_size);
  mp_srcptr xp = x.vec->_mp_d;
  mp_srcptr yp = y.vec->_mp_d;
  while (size > 0) {
    const mp_limb_t xl = *xp;
    const mp_limb_t yl = *yp;
    if (xl != yl) {
      // Get the ones where they are different.
      const mp_limb_t diff = xl ^ yl;
      // First bit that is different.
      const mp_limb_t mask = diff & ~(diff-1);
      return ((xl & mask) != 0) ? 1 : -1;
    }
    ++xp;
    ++yp;
    --size;
  }
  return (x_size == y_size) ? 0 : ((x_size > y_size) ? 1 : -1);
}

/*! \relates Parma_Polyhedra_Library::Bit_Row */
bool
PPL::subset_or_equal(const Bit_Row& x, const Bit_Row& y) {
  mp_size_t x_size = x.vec->_mp_size;
  PPL_ASSERT(x_size >= 0);
  mp_size_t y_size = y.vec->_mp_size;
  PPL_ASSERT(y_size >= 0);
  if (x_size > y_size)
    return false;
  mp_srcptr xp = x.vec->_mp_d;
  mp_srcptr yp = y.vec->_mp_d;
  while (x_size > 0) {
    if ((*xp & ~*yp) != 0)
      return false;
    ++xp;
    ++yp;
    --x_size;
  }
  return true;
}

/*! \relates Parma_Polyhedra_Library::Bit_Row */
bool
PPL::subset_or_equal(const Bit_Row& x, const Bit_Row& y,
		     bool& strict_subset) {
  mp_size_t x_size = x.vec->_mp_size;
  PPL_ASSERT(x_size >= 0);
  mp_size_t y_size = y.vec->_mp_size;
  PPL_ASSERT(y_size >= 0);
  if (x_size > y_size)
    return false;
  mp_srcptr xp = x.vec->_mp_d;
  mp_srcptr yp = y.vec->_mp_d;
  strict_subset = (x_size < y_size);
  mp_limb_t xl;
  mp_limb_t yl;
  if (strict_subset) {
    while (x_size > 0) {
      xl = *xp;
      yl = *yp;
      if ((xl & ~yl) != 0)
	return false;
    strict_subset_next:
      ++xp;
      ++yp;
      --x_size;
    }
  }
  else {
    while (x_size > 0) {
      xl = *xp;
      yl = *yp;
      if (xl != yl) {
	if ((xl & ~yl) != 0)
	  return false;
	strict_subset = true;
	goto strict_subset_next;
      }
      ++xp;
      ++yp;
      --x_size;
    }
  }
  return true;
}

/*! \relates Parma_Polyhedra_Library::Bit_Row */
bool
PPL::strict_subset(const Bit_Row& x, const Bit_Row& y) {
  mp_size_t x_size = x.vec->_mp_size;
  PPL_ASSERT(x_size >= 0);
  mp_size_t y_size = y.vec->_mp_size;
  PPL_ASSERT(y_size >= 0);
  if (x_size > y_size)
    return false;
  bool different = (x_size < y_size);
  mp_srcptr xp = x.vec->_mp_d;
  mp_srcptr yp = y.vec->_mp_d;
  while (x_size > 0) {
    const mp_limb_t xl = *xp;
    const mp_limb_t yl = *yp;
    if ((xl & ~yl) != 0)
      return false;
    if (!different && xl != yl)
      different = true;
    ++xp;
    ++yp;
    --x_size;
  }
  return different;
}

/*! \relates Bit_Row */
bool
PPL::operator==(const Bit_Row& x, const Bit_Row& y) {
  const mp_size_t x_vec_size = x.vec->_mp_size;
  PPL_ASSERT(x_vec_size >= 0);
  const mp_size_t y_vec_size = y.vec->_mp_size;
  PPL_ASSERT(y_vec_size >= 0);

  if (x_vec_size != y_vec_size)
    return false;

  return mpn_cmp(x.vec->_mp_d, y.vec->_mp_d, x_vec_size) == 0;
}

/*! \relates Bit_Row */
bool
PPL::operator!=(const Bit_Row& x, const Bit_Row& y) {
  const mp_size_t x_vec_size = x.vec->_mp_size;
  PPL_ASSERT(x_vec_size >= 0);
  const mp_size_t y_vec_size = y.vec->_mp_size;
  PPL_ASSERT(y_vec_size >= 0);

  if (x_vec_size != y_vec_size)
    return true;

  return mpn_cmp(x.vec->_mp_d, y.vec->_mp_d, x_vec_size) != 0;
}

bool
PPL::Bit_Row::OK() const {
  const mp_size_t vec_size = vec->_mp_size;
  const mp_size_t vec_alloc = vec->_mp_alloc;
  return vec_size >= 0
    && vec_alloc >= vec_size
    && (vec_size == 0 || mpz_getlimbn(vec, vec_size-1) != 0);
}

void
PPL::Bit_Row::union_helper(const Bit_Row& y, const Bit_Row& z) {
  mp_size_t y_size = y.vec->_mp_size;
  mp_size_t z_size = z.vec->_mp_size;
  PPL_ASSERT(y_size <= z_size);
  PPL_ASSERT(vec->_mp_alloc >= z_size);
  vec->_mp_size = z.vec->_mp_size;
  mp_srcptr yp = y.vec->_mp_d;
  mp_srcptr zp = z.vec->_mp_d;
  mp_ptr p = vec->_mp_d;
  z_size -= y_size;
  while (y_size > 0) {
    *p = *yp | * zp;
    ++yp;
    ++zp;
    ++p;
    --y_size;
  }
  while (z_size > 0) {
    *p = *zp;
    ++zp;
    ++p;
    --z_size;
  }
}
