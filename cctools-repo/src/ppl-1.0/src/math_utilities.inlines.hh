/* Implementation of some math utility functions: inline functions.
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

#ifndef PPL_math_utilities_inlines_hh
#define PPL_math_utilities_inlines_hh 1

#include "Coefficient.defs.hh"
#include <limits>
#include "assert.hh"

namespace Parma_Polyhedra_Library {

inline void
normalize2(Coefficient_traits::const_reference x,
	   Coefficient_traits::const_reference y,
	   Coefficient& n_x, Coefficient& n_y) {
  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  gcd_assign(gcd, x, y);
  exact_div_assign(n_x, x, gcd);
  exact_div_assign(n_y, y, gcd);
}

template <typename T>
inline T
low_bits_mask(const unsigned n) {
  PPL_ASSERT(n < unsigned(std::numeric_limits<T>::digits));
  return ~((~static_cast<T>(0)) << n);
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, void>::type
numer_denom(const T& from,
	    Coefficient& numer, Coefficient& denom) {
  PPL_ASSERT(!is_not_a_number(from)
	 && !is_minus_infinity(from)
	 && !is_plus_infinity(from));
  PPL_DIRTY_TEMP(mpq_class, q);
  assign_r(q, from, ROUND_NOT_NEEDED);
  numer = q.get_num();
  denom = q.get_den();
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, void>::type
div_round_up(T& to,
	     Coefficient_traits::const_reference x,
	     Coefficient_traits::const_reference y) {
  PPL_DIRTY_TEMP(mpq_class, q_x);
  PPL_DIRTY_TEMP(mpq_class, q_y);
  // Note: this code assumes that a Coefficient is always convertible
  // to an mpq_class without loss of precision.
  assign_r(q_x, x, ROUND_NOT_NEEDED);
  assign_r(q_y, y, ROUND_NOT_NEEDED);
  div_assign_r(q_x, q_x, q_y, ROUND_NOT_NEEDED);
  assign_r(to, q_x, ROUND_UP);
}

template <typename N>
inline void
min_assign(N& x, const N& y) {
  if (x > y)
    x = y;
}

template <typename N>
inline void
max_assign(N& x, const N& y) {
  if (x < y)
    x = y;
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
is_even(const T& x) {
  T mod;
  return umod_2exp_assign_r(mod, x, 1, ROUND_DIRECT | ROUND_STRICT_RELATION) == V_EQ
    && mod == 0;
}

template <typename T>
inline typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
is_additive_inverse(const T& x, const T& y) {
  T negated_x;
  return neg_assign_r(negated_x, x, ROUND_DIRECT | ROUND_STRICT_RELATION) == V_EQ
    && negated_x == y;
}

inline bool
is_canonical(const mpq_class& x) {
  if (x.get_den() <= 0)
    return false;
  PPL_DIRTY_TEMP(mpq_class, temp);
  temp = x;
  temp.canonicalize();
  return temp.get_num() == x.get_num();
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_math_utilities_inlines_hh)
