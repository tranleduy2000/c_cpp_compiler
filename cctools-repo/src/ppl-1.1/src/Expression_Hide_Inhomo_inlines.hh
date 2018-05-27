/* Expression_Hide_Inhomo class implementation: inline functions.
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

#ifndef PPL_Expression_Hide_Inhomo_inlines_hh
#define PPL_Expression_Hide_Inhomo_inlines_hh 1

#include "Variables_Set_defs.hh"

namespace Parma_Polyhedra_Library {

template <typename T>
Expression_Hide_Inhomo<T>::Expression_Hide_Inhomo(const raw_type& expr)
  : base_type(expr) {
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Hide_Inhomo<T>::inhomogeneous_term() const {
  // Pretend it is zero.
  return Coefficient_zero();
}

template <typename T>
inline bool
Expression_Hide_Inhomo<T>::is_zero() const {
  // Don't check the inhomogeneous_term (i.e., pretend it is zero).
  return this->inner().all_homogeneous_terms_are_zero();
}

template <typename T>
template <typename Expression>
inline bool
Expression_Hide_Inhomo<T>
::is_equal_to(const Expression& y) const {
  const dimension_type x_dim = this->space_dimension();
  const dimension_type y_dim = y.space_dimension();
  if (x_dim != y_dim)
    return false;
  if (y.inhomogeneous_term() != 0)
    return false;
  // Note that the inhomogeneous term is not compared.
  return this->inner().is_equal_to(y, 1, x_dim + 1);
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Hide_Inhomo<T>::get(dimension_type i) const {
  if (i == 0)
    return Coefficient_zero();
  else
    return this->inner().get(i);
}

template <typename T>
inline Coefficient_traits::const_reference
Expression_Hide_Inhomo<T>::get(Variable v) const {
  return this->inner().get(v);
}

template <typename T>
inline bool
Expression_Hide_Inhomo<T>
::all_zeroes(const Variables_Set& vars) const {
  return this->inner().all_zeroes(vars);
}

template <typename T>
inline bool
Expression_Hide_Inhomo<T>::all_zeroes(dimension_type start,
                                      dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return this->inner().all_zeroes(start, end);
}

template <typename T>
inline dimension_type
Expression_Hide_Inhomo<T>::num_zeroes(dimension_type start,
                                      dimension_type end) const {
  if (start == end)
    return 0;
  dimension_type nz = 0;
  if (start == 0) {
    ++start;
    ++nz;
  }
  nz += this->inner().num_zeroes(start, end);
  return nz;
}

template <typename T>
inline Coefficient
Expression_Hide_Inhomo<T>::gcd(dimension_type start,
                               dimension_type end) const {
  if (start == end)
    return Coefficient_zero();
  if (start == 0)
    ++start;
  return this->inner().gcd(start, end);
}

template <typename T>
inline dimension_type
Expression_Hide_Inhomo<T>::last_nonzero() const {
  return this->inner().last_nonzero();
}

template <typename T>
inline dimension_type
Expression_Hide_Inhomo<T>::last_nonzero(dimension_type first,
                                        dimension_type last) const {
  if (first == last)
    return last;
  if (first == 0)
    ++first;
  return this->inner().last_nonzero(first, last);
}

template <typename T>
inline dimension_type
Expression_Hide_Inhomo<T>::first_nonzero(dimension_type first,
                                         dimension_type last) const {
  if (first == last)
    return last;
  if (first == 0)
    ++first;
  return this->inner().first_nonzero(first, last);
}

template <typename T>
inline bool
Expression_Hide_Inhomo<T>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return this->inner().all_zeroes_except(vars, start, end);
}

template <typename T>
inline void
Expression_Hide_Inhomo<T>
::has_a_free_dimension_helper(std::set<dimension_type>& y) const {
  bool had_0 = (y.count(0) == 1);
  this->inner().has_a_free_dimension_helper(y);
  if (had_0)
    y.insert(0);
}

template <typename T>
template <typename Expression>
inline bool
Expression_Hide_Inhomo<T>
::is_equal_to(const Expression& y,
              dimension_type start, dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return this->inner().is_equal_to(y, start, end);
}

template <typename T>
template <typename Expression>
inline bool
Expression_Hide_Inhomo<T>
::is_equal_to(const Expression& y,
              Coefficient_traits::const_reference c1,
              Coefficient_traits::const_reference c2,
              dimension_type start, dimension_type end) const {
  if (start == end)
    return true;
  if (start == 0)
    ++start;
  return this->inner().is_equal_to(y, c1, c2, start, end);
}

template <typename T>
inline void
Expression_Hide_Inhomo<T>::get_row(Dense_Row& row) const {
  this->inner().get_row(row);
  row.reset(0);
}

template <typename T>
inline void
Expression_Hide_Inhomo<T>::get_row(Sparse_Row& row) const {
  this->inner().get_row(row);
  row.reset(0);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Expression_Hide_Inhomo_inlines_hh)
