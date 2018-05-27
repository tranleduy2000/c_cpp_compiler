/* Expression_Hide_Last class implementation: inline functions.
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

#ifndef PPL_Expression_Hide_Last_inlines_hh
#define PPL_Expression_Hide_Last_inlines_hh 1

#include "Variables_Set.defs.hh"

namespace Parma_Polyhedra_Library {

template <typename Expression>
inline
Expression_Hide_Last<Expression>
::Expression_Hide_Last(Expression& expr, bool hide_last)
  : expr(expr), hide_last(hide_last) {
}

template <typename Expression>
inline void
Expression_Hide_Last<Expression>::set_hide_last(bool hide_last) {
  this->hide_last = hide_last;
}

template <typename Expression>
inline Representation
Expression_Hide_Last<Expression>::representation() const {
  return expr.representation();
}

template <typename Expression>
inline typename Expression_Hide_Last<Expression>::const_iterator
Expression_Hide_Last<Expression>::begin() const {
  return expr.begin();
}

template <typename Expression>
inline typename Expression_Hide_Last<Expression>::const_iterator
Expression_Hide_Last<Expression>::end() const {
  if (hide_last) {
    PPL_ASSERT(expr.space_dimension() != 0);
    return expr.lower_bound(Variable(expr.space_dimension() - 1));
  } else {
    return expr.end();
  }
}

template <typename Expression>
inline typename Expression_Hide_Last<Expression>::const_iterator
Expression_Hide_Last<Expression>::lower_bound(Variable v) const {
  PPL_ASSERT(v.space_dimension() <= space_dimension() + 1);
  return expr.lower_bound(v);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Last<Expression>::space_dimension() const {
  if (hide_last) {
    PPL_ASSERT(expr.space_dimension() != 0);
    return expr.space_dimension() - 1;
  } else
    return expr.space_dimension();
}

template <typename Expression>
inline Coefficient_traits::const_reference
Expression_Hide_Last<Expression>::coefficient(Variable v) const {
  PPL_ASSERT(v.space_dimension() <= space_dimension());
  return expr.coefficient(v);
}

template <typename Expression>
inline Coefficient_traits::const_reference
Expression_Hide_Last<Expression>::inhomogeneous_term() const {
  return expr.inhomogeneous_term();
}

template <typename Expression>
inline bool
Expression_Hide_Last<Expression>::is_zero() const {
  return expr.all_zeroes(0, space_dimension() + 1);
}

template <typename Expression>
inline bool
Expression_Hide_Last<Expression>::all_homogeneous_terms_are_zero() const {
  return expr.all_zeroes(1, space_dimension() + 1);
}

template <typename Expression>
template <typename Expression2>
inline bool
Expression_Hide_Last<Expression>
::is_equal_to(const Expression2& x) const {
  const dimension_type dim1 = space_dimension();
  const dimension_type dim2 = x.space_dimension();
  if (dim1 != dim2)
    return false;
  return is_equal_to(x, 0, dim1 + 1);
}

template <typename Expression>
inline bool
Expression_Hide_Last<Expression>::all_zeroes(const Variables_Set& vars) const {
  PPL_ASSERT(vars.space_dimension() <= space_dimension());
  return expr.all_zeroes(vars);
}

template <typename Expression>
inline Coefficient_traits::const_reference
Expression_Hide_Last<Expression>::get(dimension_type i) const {
  PPL_ASSERT(i <= space_dimension());
  return expr.get(i);
}

template <typename Expression>
inline Coefficient_traits::const_reference
Expression_Hide_Last<Expression>::get(Variable v) const {
  PPL_ASSERT(v.space_dimension() <= space_dimension());
  return expr.get(v);
}

template <typename Expression>
inline bool
Expression_Hide_Last<Expression>::all_zeroes(dimension_type start,
                                      dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  return expr.all_zeroes(start, end);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Last<Expression>::num_zeroes(dimension_type start,
                                      dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  return expr.num_zeroes(start, end);
}

template <typename Expression>
inline Coefficient
Expression_Hide_Last<Expression>::gcd(dimension_type start,
                               dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  return expr.gcd(start, end);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Last<Expression>::last_nonzero() const {
  return expr.last_nonzero(0, space_dimension() + 1);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Last<Expression>::last_nonzero(dimension_type first,
                                        dimension_type last) const {
  PPL_ASSERT(last <= space_dimension() + 1);
  return expr.last_nonzero(first, last);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Last<Expression>::first_nonzero(dimension_type first,
                                         dimension_type last) const {
  PPL_ASSERT(last <= space_dimension() + 1);
  return expr.first_nonzero(first, last);
}

template <typename Expression>
inline bool
Expression_Hide_Last<Expression>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  return expr.all_zeroes_except(vars, start, end);
}

template <typename Expression>
inline void
Expression_Hide_Last<Expression>
::has_a_free_dimension_helper(std::set<dimension_type>& x) const {
  if (x.empty())
    return;
  const dimension_type max_dim = *(x.end());
  PPL_ASSERT(max_dim <= space_dimension());
  expr.has_a_free_dimension_helper(x);
}

template <typename Expression>
template <typename Expression2>
inline bool
Expression_Hide_Last<Expression>
::is_equal_to(const Expression2& x,
              dimension_type start, dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  PPL_ASSERT(end <= x.space_dimension() + 1);
  return expr.is_equal_to(x, start, end);
}

template <>
template <typename Expression2>
inline bool
Expression_Hide_Last<Linear_Expression>
::is_equal_to(const Expression2& x,
              dimension_type start, dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  PPL_ASSERT(end <= x.space_dimension() + 1);
  return x.is_equal_to(expr, start, end);
}

template <typename Expression>
template <typename Expression2>
inline bool
Expression_Hide_Last<Expression>
::is_equal_to(const Expression2& x,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  PPL_ASSERT(end <= x.space_dimension() + 1);
  return expr.is_equal_to(x, c1, c2, start, end);
}

template <>
template <typename Expression2>
inline bool
Expression_Hide_Last<Linear_Expression>
::is_equal_to(const Expression2& x,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  PPL_ASSERT(end <= space_dimension() + 1);
  PPL_ASSERT(end <= x.space_dimension() + 1);
  return x.is_equal_to(expr, c2, c1, start, end);
}

template <typename Expression>
inline void
Expression_Hide_Last<Expression>::get_row(Dense_Row& row) const {
  expr.get_row(row);
  if (hide_last) {
    PPL_ASSERT(row.size() != 0);
    row.resize(row.size() - 1);
  }
}

template <typename Expression>
inline void
Expression_Hide_Last<Expression>::get_row(Sparse_Row& row) const {
  expr.get_row(row);
  if (hide_last) {
    PPL_ASSERT(row.size() != 0);
    row.resize(row.size() - 1);
  }
}

template <typename Expression>
template <typename Expression2>
inline bool
Expression_Hide_Last<Expression>
::have_a_common_variable(const Expression2& x,
                         Variable first, Variable last) const {
  PPL_ASSERT(last.space_dimension() <= space_dimension() + 1);
  PPL_ASSERT(last.space_dimension() <= x.space_dimension() + 1);
  return expr.have_a_common_variable(x, first, last);
}

template <>
template <typename Expression2>
inline bool
Expression_Hide_Last<Linear_Expression>
::have_a_common_variable(const Expression2& x,
                         Variable first, Variable last) const {
  PPL_ASSERT(last.space_dimension() <= space_dimension() + 1);
  PPL_ASSERT(last.space_dimension() <= x.space_dimension() + 1);
  return x.have_a_common_variable(expr, first, last);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Expression_Hide_Last_inlines_hh)
