/* Variable_Floating_Point_Expression class implementation: inline functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Variable_Floating_Point_Expression_inlines_hh
#define PPL_Variable_Floating_Point_Expression_inlines_hh 1

#include "globals_defs.hh"

namespace Parma_Polyhedra_Library {

template <typename FP_Interval_Type, typename FP_Format>
inline
Variable_Floating_Point_Expression<FP_Interval_Type, FP_Format>::
Variable_Floating_Point_Expression(const dimension_type v_index)
  : variable_index(v_index) {}

template <typename FP_Interval_Type, typename FP_Format>
inline
Variable_Floating_Point_Expression<FP_Interval_Type, FP_Format>::
~Variable_Floating_Point_Expression() {}

template <typename FP_Interval_Type, typename FP_Format>
inline void
Variable_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::m_swap(Variable_Floating_Point_Expression& y) {
  using std::swap;
  swap(variable_index, y.variable_index);
}

template <typename FP_Interval_Type, typename FP_Format>
inline bool
Variable_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::linearize(const FP_Interval_Abstract_Store&,
            const FP_Linear_Form_Abstract_Store& lf_store,
            FP_Linear_Form& result) const {
  typename FP_Linear_Form_Abstract_Store::const_iterator
           variable_value = lf_store.find(variable_index);

  if (variable_value == lf_store.end()) {
    result = FP_Linear_Form(Variable(variable_index));
    return true;
  }

  result = FP_Linear_Form(variable_value->second);
  return !this->overflows(result);
}

template <typename FP_Interval_Type, typename FP_Format>
inline void
Variable_Floating_Point_Expression<FP_Interval_Type, FP_Format>
::linear_form_assign(const FP_Linear_Form& lf,
                           FP_Linear_Form_Abstract_Store& lf_store) const {
  for (typename FP_Linear_Form_Abstract_Store::iterator
         i = lf_store.begin(); i != lf_store.end(); ) {
    if ((i->second).coefficient(Variable(variable_index)) != 0)
      i = lf_store.erase(i);
    else
      ++i;
  }
  lf_store[variable_index] = lf;
  return;
}

/*! \relates Variable_Floating_Point_Expression */
template <typename FP_Interval_Type, typename FP_Format>
inline void
swap(Variable_Floating_Point_Expression<FP_Interval_Type, FP_Format>& x,
     Variable_Floating_Point_Expression<FP_Interval_Type, FP_Format>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Variable_Floating_Point_Expression_inlines_hh)
