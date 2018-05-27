/* Declarations of some math utility functions.
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

#ifndef PPL_math_utilities_defs_hh
#define PPL_math_utilities_defs_hh 1

#include "Coefficient.types.hh"
#include "Checked_Number.defs.hh"
#include <gmpxx.h>

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Extract the numerator and denominator components of \p from.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
typename Enable_If<Is_Native_Or_Checked<T>::value, void>::type
numer_denom(const T& from,
	    Coefficient& numer, Coefficient& denom);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Divides \p x by \p y into \p to, rounding the result towards plus infinity.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
typename Enable_If<Is_Native_Or_Checked<T>::value, void>::type
div_round_up(T& to,
	     Coefficient_traits::const_reference x,
	     Coefficient_traits::const_reference y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Assigns to \p x the minimum between \p x and \p y.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename N>
void
min_assign(N& x, const N& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Assigns to \p x the maximum between \p x and \p y.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename N>
void
max_assign(N& x, const N& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x is an even number.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
is_even(const T& x);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \f$x = -y\f$.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
typename Enable_If<Is_Native_Or_Checked<T>::value, bool>::type
is_additive_inverse(const T& x, const T& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  If \f$g\f$ is the GCD of \p x and \p y, the values of \p x and \p y
  divided by \f$g\f$ are assigned to \p n_x and \p n_y, respectively.

  \note
  \p x and \p n_x may be the same object and likewise for
  \p y and \p n_y.  Any other aliasing results in undefined behavior.
*/
#endif
void
normalize2(Coefficient_traits::const_reference x,
	   Coefficient_traits::const_reference y,
	   Coefficient& n_x, Coefficient& n_y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x is in canonical form.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool
is_canonical(const mpq_class& x);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns a mask for the lowest \p n bits,
#endif
template <typename T>
T
low_bits_mask(unsigned n);

} // namespace Parma_Polyhedra_Library

#include "math_utilities.inlines.hh"

#endif // !defined(PPL_math_utilities_defs_hh)
