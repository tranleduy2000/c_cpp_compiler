/* Linear_Expression_Impl class implementation: inline functions.
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

#ifndef PPL_Linear_Expression_Impl_inlines_hh
#define PPL_Linear_Expression_Impl_inlines_hh 1

#include "math_utilities_defs.hh"
#include <stdexcept>

namespace Parma_Polyhedra_Library {

template <typename Row>
inline dimension_type
Linear_Expression_Impl<Row>::max_space_dimension() {
  return Row::max_size() - 1;
}

template <typename Row>
inline
Linear_Expression_Impl<Row>::Linear_Expression_Impl()
  : row(1) {
  PPL_ASSERT(OK());
}

template <typename Row>
inline
Linear_Expression_Impl<Row>
::Linear_Expression_Impl(dimension_type space_dim, bool)
  : row(space_dim + 1) {
  PPL_ASSERT(OK());
}

template <typename Row>
inline
Linear_Expression_Impl<Row>::~Linear_Expression_Impl() {
}

template <typename Row>
inline
Linear_Expression_Impl<Row>
::Linear_Expression_Impl(Coefficient_traits::const_reference n)
  : row(1) {
  if (n != 0)
    row.insert(0, n);
  PPL_ASSERT(OK());
}

template <typename Row>
inline dimension_type
Linear_Expression_Impl<Row>::space_dimension() const {
  return row.size() - 1;
}

template <typename Row>
inline void
Linear_Expression_Impl<Row>::set_space_dimension(dimension_type n) {
  row.resize(n + 1);
  PPL_ASSERT(OK());
}

template <typename Row>
inline Coefficient_traits::const_reference
Linear_Expression_Impl<Row>::coefficient(Variable v) const {
  if (v.space_dimension() > space_dimension())
    return Coefficient_zero();
  return row.get(v.id() + 1);
}

template <typename Row>
inline void
Linear_Expression_Impl<Row>
::set_coefficient(Variable v, Coefficient_traits::const_reference n) {
  PPL_ASSERT(v.space_dimension() <= space_dimension());
  const dimension_type i = v.space_dimension();
  if (n == 0)
    row.reset(i);
  else
    row.insert(i, n);
  PPL_ASSERT(OK());
}

template <typename Row>
inline Coefficient_traits::const_reference
Linear_Expression_Impl<Row>::inhomogeneous_term() const {
  return row.get(0);
}

template <typename Row>
inline void
Linear_Expression_Impl<Row>
::set_inhomogeneous_term(Coefficient_traits::const_reference n) {
  if (n == 0)
    row.reset(0);
  else
    row.insert(0, n);
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_Expression_Impl<Row>::swap_space_dimensions(Variable v1, Variable v2) {
  row.swap_coefficients(v1.space_dimension(), v2.space_dimension());
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Linear_Expression_Impl<Row>::shift_space_dimensions(Variable v,
                                                    dimension_type n) {
  row.add_zeroes_and_shift(n, v.space_dimension());
  PPL_ASSERT(OK());
}

template <typename Row>
inline memory_size_type
Linear_Expression_Impl<Row>::external_memory_in_bytes() const {
  return row.external_memory_in_bytes();
}

template <typename Row>
inline memory_size_type
Linear_Expression_Impl<Row>::total_memory_in_bytes() const {
  return external_memory_in_bytes() + sizeof(*this);
}

template <typename Row>
inline Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::operator+=(Coefficient_traits::const_reference n) {
  typename Row::iterator itr = row.insert(0);
  (*itr) += n;
  if (*itr == 0)
    row.reset(itr);
  PPL_ASSERT(OK());
  return *this;
}

template <typename Row>
inline Linear_Expression_Impl<Row>&
Linear_Expression_Impl<Row>::operator-=(Coefficient_traits::const_reference n) {
  typename Row::iterator itr = row.insert(0);
  (*itr) -= n;
  if (*itr == 0)
    row.reset(itr);
  PPL_ASSERT(OK());
  return *this;
}

template <typename Row>
inline void
Linear_Expression_Impl<Row>::normalize() {
  row.normalize();
  PPL_ASSERT(OK());
}

template <>
inline bool
Linear_Expression_Impl<Sparse_Row>::is_zero() const {
  return row.num_stored_elements() == 0;
}

template <>
inline bool
Linear_Expression_Impl<Sparse_Row>::all_homogeneous_terms_are_zero() const {
  return row.lower_bound(1) == row.end();
}

template <>
inline bool
Linear_Expression_Impl<Sparse_Row>::all_zeroes(dimension_type start,
                                               dimension_type end) const {
  return row.lower_bound(start) == row.lower_bound(end);
}

template <>
inline dimension_type
Linear_Expression_Impl<Sparse_Row>::num_zeroes(dimension_type start,
                                               dimension_type end) const {
  PPL_ASSERT(start <= end);
  return (end - start)
    - std::distance(row.lower_bound(start), row.lower_bound(end));
}

template <>
inline dimension_type
Linear_Expression_Impl<Sparse_Row>::last_nonzero() const {
  if (row.num_stored_elements() == 0)
    return 0;
  Sparse_Row::const_iterator i = row.end();
  --i;
  return i.index();
}

template <>
inline dimension_type
Linear_Expression_Impl<Sparse_Row>
::first_nonzero(dimension_type first, dimension_type last) const {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= row.size());
  Sparse_Row::const_iterator i = row.lower_bound(first);

  if (i != row.end() && i.index() < last)
    return i.index();
  else
    return last;
}

template <>
inline dimension_type
Linear_Expression_Impl<Sparse_Row>
::last_nonzero(dimension_type first, dimension_type last) const {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= row.size());
  Sparse_Row::const_iterator itr1 = row.lower_bound(first);
  Sparse_Row::const_iterator itr2 = row.lower_bound(last);

  if (itr1 == itr2)
    return last;

  --itr2;
  return itr2.index();
}

template <>
inline Representation
Linear_Expression_Impl<Dense_Row>::representation() const {
  return DENSE;
}

template <>
inline Representation
Linear_Expression_Impl<Sparse_Row>::representation() const {
  return SPARSE;
}

template <>
inline void
Linear_Expression_Impl<Sparse_Row>::const_iterator
::skip_zeroes_forward() {
  // Nothing to do.
}

template <>
inline void
Linear_Expression_Impl<Sparse_Row>::const_iterator
::skip_zeroes_backward() {
  // Nothing to do.
}

namespace IO_Operators {

template <typename Row>
inline std::ostream&
operator<<(std::ostream& s, const Linear_Expression_Impl<Row>& e) {
  e.print(s);
  return s;
}

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Linear_Expression_Impl_inlines_hh)
