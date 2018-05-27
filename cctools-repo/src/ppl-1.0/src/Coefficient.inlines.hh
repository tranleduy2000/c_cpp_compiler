/* Coefficient class implementation: inline functions.
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

#ifndef PPL_Coefficient_inlines_hh
#define PPL_Coefficient_inlines_hh 1

namespace Parma_Polyhedra_Library {

#ifdef PPL_CHECKED_INTEGERS
inline void
Bounded_Integer_Coefficient_Policy::handle_result(Result r) {
  // Note that the input functions can return VC_NAN.
  if (result_overflow(r) || result_class(r) == VC_NAN)
    throw_result_exception(r);
}
#endif // PPL_CHECKED_INTEGERS


#if defined(PPL_CHECKED_INTEGERS) || defined(PPL_NATIVE_INTEGERS)
inline Coefficient_traits::const_reference
Coefficient_zero() {
  // FIXME: is there a way to avoid this static variable?
  static Coefficient zero(0);
  return zero;
}

inline Coefficient_traits::const_reference
Coefficient_one() {
  // FIXME: is there a way to avoid this static variable?
  static Coefficient one(1);
  return one;
}
#endif // defined(PPL_CHECKED_INTEGERS) || defined(PPL_NATIVE_INTEGERS)

#ifdef PPL_GMP_INTEGERS
inline Coefficient_traits::const_reference
Coefficient_zero() {
  extern const Coefficient* Coefficient_zero_p;
  return *Coefficient_zero_p;
}

inline Coefficient_traits::const_reference
Coefficient_one() {
  extern const Coefficient* Coefficient_one_p;
  PPL_ASSERT(*Coefficient_one_p != 0);
  return *Coefficient_one_p;
}
#endif // PPL_GMP_INTEGERS

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Coefficient_inlines_hh)
