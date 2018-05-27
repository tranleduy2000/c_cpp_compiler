/* Expression_Hide_Inhomo class implementation: inline functions.
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

#ifndef PPL_Expression_Hide_Inhomo_inlines_hh
#define PPL_Expression_Hide_Inhomo_inlines_hh 1

#include "Variables_Set.defs.hh"
#include "Linear_Expression.defs.hh"

namespace Parma_Polyhedra_Library {

template <typename Expression>
inline
Expression_Hide_Inhomo<Expression>
::Expression_Hide_Inhomo(Expression& expr)
  : expr(expr) {
}

template <typename Expression>
inline Representation
Expression_Hide_Inhomo<Expression>::representation() const {
  return expr.representation();
}

template <typename Expression>
inline typename Expression_Hide_Inhomo<Expression>::const_iterator
Expression_Hide_Inhomo<Expression>::begin() const {
  return expr.begin();
}

template <typename Expression>
inline typename Expression_Hide_Inhomo<Expression>::const_iterator
Expression_Hide_Inhomo<Expression>::end() const {
  return expr.end();
}

template <typename Expression>
inline typename Expression_Hide_Inhomo<Expression>::const_iterator
Expression_Hide_Inhomo<Expression>::lower_bound(Variable v) const {
  return expr.lower_bound(v);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Inhomo<Expression>::space_dimension() const {
  return expr.space_dimension();
}

template <typename Expression>
inline Coefficient_traits::const_reference
Expression_Hide_Inhomo<Expression>::coefficient(Variable v) const {
  return expr.coefficient(v);
}

template <typename Expression>
inline Coefficient_traits::const_reference
Expression_Hide_Inhomo<Expression>::inhomogeneous_term() const {
  return Coefficient_zero();
}

template <typename Expression>
inline bool
Expression_Hide_Inhomo<Expression>::is_zero() const {
  // Don't check the inhomogeneous_term (i.e. pretend it's 0).
  return expr.all_homogeneous_terms_are_zero();
}

template <typename Expression>
inline bool
Expression_Hide_Inhomo<Expression>::all_homogeneous_terms_are_zero() const {
  return expr.all_homogeneous_terms_are_zero();
}

template <typename Expression>
template <typename Expression2>
inline bool
Expression_Hide_Inhomo<Expression>
::is_equal_to(const Expression2& x) const {
  const dimension_type dim1 = space_dimension();
  const dimension_type dim2 = x.space_dimension();
  if (dim1 != dim2)
    return false;
  if (x.inhomogeneous_term() != 0)
    return false;
  // Note that the inhomogeneous term is not compared.
  return expr.is_equal_to(x, 1, dim1 + 1);
}

template <>
template <typename Expression2>
inline bool
Expression_Hide_Inhomo<Linear_Expression>::is_equal_to(const Expression2& x) const {
  const dimension_type dim1 = space_dimension();
  const dimension_type dim2 = x.space_dimension();
  if (dim1 != dim2)
    return false;
  if (x.inhomogeneous_term() != 0)
    return false;
  // Note that the inhomogeneous term is not compared.
  return x.is_equal_to(expr, 1, dim1 + 1);
}

template <typename Expression>
inline bool
Expression_Hide_Inhomo<Expression>::all_zeroes(const Variables_Set& vars) const {
  return expr.all_zeroes(vars);
}

template <typename Expression>
inline Coefficient_traits::const_reference
Expression_Hide_Inhomo<Expression>::get(dimension_type i) const {
  if (i == 0)
    return Coefficient_zero();
  else
    return expr.get(i);
}

template <typename Expression>
inline Coefficient_traits::const_reference
Expression_Hide_Inhomo<Expression>::get(Variable v) const {
  return expr.get(v);
}

template <typename Expression>
inline bool
Expression_Hide_Inhomo<Expression>::all_zeroes(dimension_type start,
                                               dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return expr.all_zeroes(start, end);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Inhomo<Expression>::num_zeroes(dimension_type start,
                                               dimension_type end) const {
  if (start == end)
    return 0;
  if (start == 0)
    return 1 + expr.num_zeroes(start + 1, end);
  else
    return expr.num_zeroes(start, end);
}

template <typename Expression>
inline Coefficient
Expression_Hide_Inhomo<Expression>::gcd(dimension_type start,
                                        dimension_type end) const {
  if (start == end)
    return Coefficient_zero();
  if (start == 0)
    ++start;
  return expr.gcd(start, end);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Inhomo<Expression>::last_nonzero() const {
  return expr.last_nonzero();
}

template <typename Expression>
inline dimension_type
Expression_Hide_Inhomo<Expression>::last_nonzero(dimension_type first,
                                                 dimension_type last) const {
  return expr.last_nonzero(first, last);
}

template <typename Expression>
inline dimension_type
Expression_Hide_Inhomo<Expression>::first_nonzero(dimension_type first,
                                                  dimension_type last) const {
  if (first == last)
    return last;
  if (first == 0)
    ++first;
  return expr.first_nonzero(first, last);
}

template <typename Expression>
inline bool
Expression_Hide_Inhomo<Expression>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return expr.all_zeroes_except(vars, start, end);
}

template <typename Expression>
inline void
Expression_Hide_Inhomo<Expression>
::has_a_free_dimension_helper(std::set<dimension_type>& x) const {
  bool had_0 = (x.count(0) == 1);
  expr.has_a_free_dimension_helper(x);
  if (had_0)
    x.insert(0);
}

template <typename Expression>
template <typename Expression2>
inline bool
Expression_Hide_Inhomo<Expression>
::is_equal_to(const Expression2& x,
              dimension_type start, dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return expr.is_equal_to(x, start, end);
}

template <>
template <typename Expression2>
inline bool
Expression_Hide_Inhomo<Linear_Expression>
::is_equal_to(const Expression2& x,
              dimension_type start, dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return x.is_equal_to(expr, start, end);
}

template <typename Expression>
template <typename Expression2>
inline bool
Expression_Hide_Inhomo<Expression>
::is_equal_to(const Expression2& x,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return expr.is_equal_to(x, c1, c2, start, end);
}

template <>
template <typename Expression2>
inline bool
Expression_Hide_Inhomo<Linear_Expression>
::is_equal_to(const Expression2& x,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return x.is_equal_to(expr, c2, c1, start, end);
}

template <typename Expression>
inline void
Expression_Hide_Inhomo<Expression>::get_row(Dense_Row& row) const {
  expr.get_row(row);
  row.reset(0);
}

template <typename Expression>
inline void
Expression_Hide_Inhomo<Expression>::get_row(Sparse_Row& row) const {
  expr.get_row(row);
  row.reset(0);
}

template <typename Expression>
template <typename Expression2>
inline bool
Expression_Hide_Inhomo<Expression>
::have_a_common_variable(const Expression2& x,
                         Variable first, Variable last) const {
  return expr.have_a_common_variable(x, first, last);
}

template <>
template <typename Expression2>
inline bool
Expression_Hide_Inhomo<Linear_Expression>
::have_a_common_variable(const Expression2& x,
                         Variable first, Variable last) const {
  return x.have_a_common_variable(expr, first, last);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Expression_Hide_Inhomo_inlines_hh)
