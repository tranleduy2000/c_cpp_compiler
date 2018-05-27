/* Expression_Hide_Last class implementation: inline functions.
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

#ifndef PPL_Expression_Hide_Last_inlines_hh
#define PPL_Expression_Hide_Last_inlines_hh 1

#include "Variables_Set_defs.hh"

namespace Parma_Polyhedra_Library {

template <typename T>
inline
Expression_Hide_Last<T>::Expression_Hide_Last(const raw_type& expr,
                                              const bool hide_last)
  : base_type(expr), hide_last_(hide_last) {
}

template <typename T>
inline dimension_type
Expression_Hide_Last<T>::space_dimension() const {
  dimension_type dim = this->inner().space_dimension();
  if (hide_last_) {
    PPL_ASSERT(dim > 0);
    --dim;
  }
  return dim;
}

template <typename T>
inline typename Expression_Hide_Last<T>::const_iterator
Expression_Hide_Last<T>::end() const {
  if (hide_last_) {
    return this->inner().lower_bound(Variable(space_dimension()));
  }
  else {
    return this->inner().end();
  }
}

template <typename T>
inline typename Expression_Hide_Last<T>::const_iterator
Expression_Hide_Last<T>::lower_bound(Variable v) const {
  PPL_ASSERT(v.space_dimension() <= space_dimension() + 1);
  return this->inner().lower_bound(v);
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Hide_Last<T>::coefficient(Variable v) const {
  PPL_ASSERT(v.space_dimension() <= space_dimension());
  return this->inner().coefficient(v);
}

template <typename T>
inline bool
Expression_Hide_Last<T>::is_zero() const {
  return this->inner().all_zeroes(0, space_dimension() + 1);
}

template <typename T>
inline bool
Expression_Hide_Last<T>::all_homogeneous_terms_are_zero() const {
  return this->inner().all_zeroes(1, space_dimension() + 1);
}

template <typename T>
template <typename Expression>
inline bool
Expression_Hide_Last<T>
::is_equal_to(const Expression& y) const {
  const dimension_type x_dim = space_dimension();
  const dimension_type y_dim = y.space_dimension();
  if (x_dim != y_dim)
    return false;
  return is_equal_to(y, 0, x_dim + 1);
}

template <typename T>
inline bool
Expression_Hide_Last<T>::all_zeroes(const Variables_Set& vars) const {
  PPL_ASSERT(vars.space_dimension() <= space_dimension());
  return this->inner().all_zeroes(vars);
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Hide_Last<T>::get(dimension_type i) const {
  PPL_ASSERT(i <= space_dimension());
  return this->inner().get(i);
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Hide_Last<T>::get(Variable v) const {
  PPL_ASSERT(v.space_dimension() <= space_dimension());
  return this->inner().get(v);
}

template <typename T>
inline bool
Expression_Hide_Last<T>::all_zeroes(dimension_type start,
                                    dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  return this->inner().all_zeroes(start, end);
}

template <typename T>
inline dimension_type
Expression_Hide_Last<T>::num_zeroes(dimension_type start,
                                    dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  return this->inner().num_zeroes(start, end);
}

template <typename T>
inline Coefficient
Expression_Hide_Last<T>::gcd(dimension_type start,
                             dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  return this->inner().gcd(start, end);
}

template <typename T>
inline dimension_type
Expression_Hide_Last<T>::last_nonzero() const {
  return this->inner().last_nonzero(0, space_dimension() + 1);
}

template <typename T>
inline dimension_type
Expression_Hide_Last<T>::last_nonzero(dimension_type first,
                                      dimension_type last) const {
  PPL_ASSERT(last <= space_dimension() + 1);
  return this->inner().last_nonzero(first, last);
}

template <typename T>
inline dimension_type
Expression_Hide_Last<T>::first_nonzero(dimension_type first,
                                       dimension_type last) const {
  PPL_ASSERT(last <= space_dimension() + 1);
  return this->inner().first_nonzero(first, last);
}

template <typename T>
inline bool
Expression_Hide_Last<T>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  return this->inner().all_zeroes_except(vars, start, end);
}

template <typename T>
inline void
Expression_Hide_Last<T>
::has_a_free_dimension_helper(std::set<dimension_type>& x) const {
  if (x.empty())
    return;
  PPL_ASSERT(*(--x.end()) <= space_dimension());
  this->inner().has_a_free_dimension_helper(x);
}

template <typename T>
template <typename Expression>
inline bool
Expression_Hide_Last<T>
::is_equal_to(const Expression& y,
              dimension_type start, dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  PPL_ASSERT(end <= y.space_dimension() + 1);
  return this->inner().is_equal_to(y, start, end);
}

template <typename T>
template <typename Expression>
inline bool
Expression_Hide_Last<T>
::is_equal_to(const Expression& y,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  PPL_ASSERT(end <= y.space_dimension() + 1);
  return this->inner().is_equal_to(y, c1, c2, start, end);
}

template <typename T>
inline void
Expression_Hide_Last<T>::get_row(Dense_Row& row) const {
  this->inner().get_row(row);
  if (hide_last_) {
    PPL_ASSERT(row.size() != 0);
    row.resize(row.size() - 1);
  }
}

template <typename T>
inline void
Expression_Hide_Last<T>::get_row(Sparse_Row& row) const {
  this->inner().get_row(row);
  if (hide_last_) {
    PPL_ASSERT(row.size() != 0);
    row.resize(row.size() - 1);
  }
}

template <typename T>
template <typename Expression>
inline bool
Expression_Hide_Last<T>
::have_a_common_variable(const Expression& y,
                         Variable first, Variable last) const {
  PPL_ASSERT(last.space_dimension() <= space_dimension() + 1);
  PPL_ASSERT(last.space_dimension() <= y.space_dimension() + 1);
  return this->inner().have_a_common_variable(y, first, last);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Expression_Hide_Last_inlines_hh)
