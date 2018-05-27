/* Expression_Adapter class implementation: inline functions.
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

#ifndef PPL_Expression_Adapter_inlines_hh
#define PPL_Expression_Adapter_inlines_hh 1

#include "Variables_Set_defs.hh"

namespace Parma_Polyhedra_Library {

template <typename T>
inline
Expression_Adapter<T>::Expression_Adapter(const raw_type& expr)
  : raw_(expr) {
}

template <typename T>
inline typename Expression_Adapter<T>::inner_type
Expression_Adapter<T>::inner() const {
  return inner_type(raw_);
}

template <typename T>
inline Representation
Expression_Adapter<T>::representation() const {
  return inner().representation();
}

template <typename T>
inline typename Expression_Adapter<T>::const_iterator
Expression_Adapter<T>::begin() const {
  return inner().begin();
}

template <typename T>
inline typename Expression_Adapter<T>::const_iterator
Expression_Adapter<T>::end() const {
  return inner().end();
}

template <typename T>
inline typename Expression_Adapter<T>::const_iterator
Expression_Adapter<T>::lower_bound(Variable v) const {
  return inner().lower_bound(v);
}

template <typename T>
inline dimension_type
Expression_Adapter<T>::space_dimension() const {
  return inner().space_dimension();
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Adapter<T>::coefficient(Variable v) const {
  return inner().coefficient(v);
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Adapter<T>::inhomogeneous_term() const {
  return inner().inhomogeneous_term();
}

template <typename T>
inline bool
Expression_Adapter<T>::is_zero() const {
  return inner().is_zero();
}

template <typename T>
inline bool
Expression_Adapter<T>::all_homogeneous_terms_are_zero() const {
  return inner().all_homogeneous_terms_are_zero();
}

template <typename T>
template <typename Expression>
inline bool
Expression_Adapter<T>::is_equal_to(const Expression& y) const {
  return inner().is_equal_to(y);
}

template <typename T>
inline bool
Expression_Adapter<T>
::all_zeroes(const Variables_Set& vars) const {
  return inner().all_zeroes(vars);
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Adapter<T>::get(dimension_type i) const {
  return inner().get(i);
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Adapter<T>::get(Variable v) const {
  return inner().get(v);
}

template <typename T>
inline bool
Expression_Adapter<T>::all_zeroes(dimension_type start,
                                  dimension_type end) const {
  return inner().all_zeroes(start, end);
}

template <typename T>
inline dimension_type
Expression_Adapter<T>::num_zeroes(dimension_type start,
                                  dimension_type end) const {
  return inner().num_zeroes(start, end);
}

template <typename T>
inline Coefficient
Expression_Adapter<T>::gcd(dimension_type start,
                           dimension_type end) const {
  return inner().gcd(start, end);
}

template <typename T>
inline dimension_type
Expression_Adapter<T>::last_nonzero() const {
  return inner().last_nonzero();
}

template <typename T>
inline dimension_type
Expression_Adapter<T>::last_nonzero(dimension_type first,
                                    dimension_type last) const {
  return inner().last_nonzero(first, last);
}

template <typename T>
inline dimension_type
Expression_Adapter<T>::first_nonzero(dimension_type first,
                                     dimension_type last) const {
  return inner().first_nonzero(first, last);
}

template <typename T>
inline bool
Expression_Adapter<T>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const {
  return inner().all_zeroes_except(vars, start, end);
}

template <typename T>
inline void
Expression_Adapter<T>
::has_a_free_dimension_helper(std::set<dimension_type>& x) const {
  inner().has_a_free_dimension_helper(x);
}

template <typename T>
template <typename Expression>
inline bool
Expression_Adapter<T>
::is_equal_to(const Expression& y,
              dimension_type start, dimension_type end) const {
  return inner().is_equal_to(y, start, end);
}

template <typename T>
template <typename Expression>
inline bool
Expression_Adapter<T>
::is_equal_to(const Expression& y,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  return inner().is_equal_to(y, c1, c2, start, end);
}

template <typename T>
inline void
Expression_Adapter<T>::get_row(Dense_Row& row) const {
  inner().get_row(row);
}

template <typename T>
inline void
Expression_Adapter<T>::get_row(Sparse_Row& row) const {
  inner().get_row(row);
}

template <typename T>
template <typename Expression>
inline bool
Expression_Adapter<T>
::have_a_common_variable(const Expression& y,
                         Variable first, Variable last) const {
  return inner().have_a_common_variable(y, first, last);
}

template <typename T>
inline
Expression_Adapter_Transparent<T>
::Expression_Adapter_Transparent(const raw_type& expr)
  : base_type(expr) {
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Expression_Adapter_inlines_hh)
