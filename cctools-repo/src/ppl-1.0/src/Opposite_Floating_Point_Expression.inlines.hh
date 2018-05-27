/* Opposite_Floating_Point_Expression class implementation: inline functions.
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

#ifndef PPL_Opposite_Floating_Point_Expression_inlines_hh
#define PPL_Opposite_Floating_Point_Expression_inlines_hh 1

#include "globals.defs.hh"

namespace Parma_Polyhedra_Library {

template <typename FP_Interval_Type, typename FP_Format>
inline
Opposite_Floating_Point_Expression<FP_Interval_Type, FP_Format>::
Opposite_Floating_Point_Expression(
         Floating_Point_Expression<FP_Interval_Type, FP_Format>* const op)
  : operand(op)
{
  assert(op != 0);
}

template <typename FP_Interval_Type, typename FP_Format>
inline
Opposite_Floating_Point_Expression<FP_Interval_Type, FP_Format>::
~Opposite_Floating_Point_Expression() {
  delete operand;
}

template <typename FP_Interval_Type, typename FP_Format>
inline void
Opposite_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::m_swap(Opposite_Floating_Point_Expression& y) {
  using std::swap;
  swap(operand, y.operand);
}

template <typename FP_Interval_Type, typename FP_Format>
inline bool
Opposite_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::linearize(const FP_Interval_Abstract_Store& int_store,
            const FP_Linear_Form_Abstract_Store& lf_store,
            FP_Linear_Form& result) const {
  if (!operand->linearize(int_store, lf_store, result))
    return false;

  result.negate();
  return true;
}

/*! \relates Opposite_Floating_Point_Expression */
template <typename FP_Interval_Type, typename FP_Format>
inline void
swap(Opposite_Floating_Point_Expression<FP_Interval_Type, FP_Format>& x,
     Opposite_Floating_Point_Expression<FP_Interval_Type, FP_Format>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Opposite_Floating_Point_Expression_inlines_hh)
