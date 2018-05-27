/* Inline functions implementing distances.
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

#ifndef PPL_distances_inlines_hh
#define PPL_distances_inlines_hh 1

#include "Result.defs.hh"

namespace Parma_Polyhedra_Library {

// A struct to work around the lack of partial specialization
// of function templates in C++.
template <typename To, typename From>
struct maybe_assign_struct {
  static inline Result
  function(const To*& top, To& tmp, const From& from, Rounding_Dir dir) {
    // When `To' and `From' are different types, we make the conversion
    // and use `tmp'.
    top = &tmp;
    return assign_r(tmp, from, dir);
  }
};

template <typename Type>
struct maybe_assign_struct<Type, Type> {
  static inline Result
  function(const Type*& top, Type&, const Type& from, Rounding_Dir) {
    // When the types are the same, conversion is unnecessary.
    top = &from;
    return V_EQ;
  }
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Assigns to \p top a pointer to a location that holds the
  conversion, according to \p dir, of \p from to type \p To.  When
  necessary, and only when necessary, the variable \p tmp is used to
  hold the result of conversion.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename To, typename From>
inline Result
maybe_assign(const To*& top, To& tmp, const From& from, Rounding_Dir dir) {
  return maybe_assign_struct<To, From>::function(top, tmp, from, dir);
}

template <typename Temp>
inline void
Rectilinear_Distance_Specialization<Temp>::combine(Temp& running,
						   const Temp& current,
						   Rounding_Dir dir) {
  add_assign_r(running, running, current, dir);
}

template <typename Temp>
inline void
Rectilinear_Distance_Specialization<Temp>::finalize(Temp&, Rounding_Dir) {
}

template <typename Temp>
inline void
Euclidean_Distance_Specialization<Temp>::combine(Temp& running,
						 Temp& current,
						 Rounding_Dir dir) {
  mul_assign_r(current, current, current, dir);
  add_assign_r(running, running, current, dir);
}

template <typename Temp>
inline void
Euclidean_Distance_Specialization<Temp>::finalize(Temp& running,
						  Rounding_Dir dir) {
  sqrt_assign_r(running, running, dir);
}

template <typename Temp>
inline void
L_Infinity_Distance_Specialization<Temp>::combine(Temp& running,
						  const Temp& current,
						  Rounding_Dir) {
  if (current > running)
    running = current;
}

template <typename Temp>
inline void
L_Infinity_Distance_Specialization<Temp>::finalize(Temp&, Rounding_Dir) {
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_distances_inlines_hh)
