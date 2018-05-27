/* Linear_Expression_Impl class implementation.
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

#include "ppl-config.h"
#include "Linear_Expression_Impl_defs.hh"
#include "Dense_Row_defs.hh"
#include "Sparse_Row_defs.hh"
#include "assert.hh"
#include <iostream>

namespace Parma_Polyhedra_Library {

template <>
bool
Linear_Expression_Impl<Dense_Row>::OK() const {
  return (row.size() != 0);
}

template <>
bool
Linear_Expression_Impl<Sparse_Row>::OK() const {
  if (row.size() == 0)
    return false;
  for (Sparse_Row::const_iterator i = row.begin(),
         i_end = row.end(); i != i_end; ++i) {
    if (*i == 0) {
      std::cerr << "Linear_Expression_Impl<Sparse_Row>::OK() failed."
                << " row was:\n";
      row.ascii_dump(std::cerr);
      // Found a stored zero.
      return false;
    }
  }
  return true;
}

template <>
void
Linear_Expression_Impl<Dense_Row>
::remove_space_dimensions(const Variables_Set& vars) {
  PPL_ASSERT(vars.space_dimension() <= space_dimension());
  if (vars.empty())
    return;

  // For each variable to be removed, replace the corresponding coefficient
  // by shifting left the coefficient to the right that will be kept.
  Variables_Set::const_iterator vsi = vars.begin();
  Variables_Set::const_iterator vsi_end = vars.end();
  dimension_type dst_col = *vsi+1;
  dimension_type src_col = dst_col + 1;
  for (++vsi; vsi != vsi_end; ++vsi) {
    const dimension_type vsi_col = *vsi+1;
    // Move all columns in between to the left.
    while (src_col < vsi_col)
      row.swap_coefficients(dst_col++, src_col++);
    ++src_col;
  }
  // Move any remaining columns.
  const dimension_type sz = row.size();
  while (src_col < sz)
    row.swap_coefficients(dst_col++, src_col++);

  // The number of remaining coefficients is `dst_col'.
  row.resize(dst_col);
  PPL_ASSERT(OK());
}

template <>
void
Linear_Expression_Impl<Sparse_Row>
::remove_space_dimensions(const Variables_Set& vars) {
  PPL_ASSERT(vars.space_dimension() <= space_dimension());
  if (vars.empty())
    return;

  // For each variable to be removed, replace the corresponding coefficient
  // by shifting left the coefficient to the right that will be kept.
  Variables_Set::const_iterator vsi = vars.begin();
  Variables_Set::const_iterator vsi_end = vars.end();
  Sparse_Row::iterator src = row.lower_bound(*vsi + 1);
  const Sparse_Row::iterator& row_end = row.end();
  dimension_type num_removed = 0;
  while (vsi != vsi_end) {
    // Delete the element.
    if (src != row_end && src.index() == *vsi + 1)
      src = row.reset(src);
    ++num_removed;
    ++vsi;
    if (vsi != vsi_end) {
      // Shift left the coefficients in [src.index(), *vsi + 1) by num_removed
      // positions.
      while (src != row_end && src.index() < *vsi + 1) {
        row.fast_swap(src.index() - num_removed, src);
        ++src;
      }
    }
    else {
      // Shift left the coefficients in [src.index(), row.size()) by
      // num_removed positions.
      while (src != row_end) {
        row.fast_swap(src.index() - num_removed, src);
        ++src;
      }
    }
  }

  PPL_ASSERT(num_removed == vars.size());

  row.resize(row.size() - num_removed);
  PPL_ASSERT(OK());
}

template <>
bool
Linear_Expression_Impl<Dense_Row>::is_zero() const {
  for (dimension_type i = row.size(); i-- > 0; )
    if (row[i] != 0)
      return false;
  return true;
}

template <>
bool
Linear_Expression_Impl<Dense_Row>::all_homogeneous_terms_are_zero() const {
  for (dimension_type i = 1; i < row.size(); ++i)
    if (row[i] != 0)
      return false;
  return true;
}

template <>
bool
Linear_Expression_Impl<Dense_Row>::all_zeroes(dimension_type start,
                                              dimension_type end) const {
  for (dimension_type i = start; i < end; ++i)
    if (row[i] != 0)
      return false;
  return true;
}

template <>
dimension_type
Linear_Expression_Impl<Dense_Row>::num_zeroes(dimension_type start,
                                              dimension_type end) const {
  PPL_ASSERT(start <= end);
  dimension_type result = 0;
  for (dimension_type i = start; i < end; ++i)
    if (row[i] == 0)
      ++result;
  return result;
}

template <>
Coefficient
Linear_Expression_Impl<Dense_Row>::gcd(dimension_type start,
                                       dimension_type end) const {
  dimension_type i;

  for (i = start; i < end; ++i)
    if (row[i] != 0)
      break;

  if (i == end)
    return 0;

  PPL_ASSERT(row[i] != 0);

  Coefficient result = row[i];
  ++i;

  if (result < 0)
    neg_assign(result);

  for ( ; i < end; ++i) {
    if (row[i] == 0)
      continue;
    gcd_assign(result, row[i], result);
    if (result == 1)
      return result;
  }

  return result;
}

template <>
Coefficient
Linear_Expression_Impl<Sparse_Row>::gcd(dimension_type start,
                                        dimension_type end) const {
  Sparse_Row::const_iterator i = row.lower_bound(start);
  Sparse_Row::const_iterator i_end = row.lower_bound(end);

  if (i == i_end)
    return 0;

  PPL_ASSERT(*i != 0);

  Coefficient result = *i;
  ++i;

  if (result < 0)
    neg_assign(result);

  for ( ; i != i_end; ++i) {
    gcd_assign(result, *i, result);
    if (result == 1)
      return result;
  }

  return result;
}

template <>
bool
Linear_Expression_Impl<Dense_Row>
::all_zeroes(const Variables_Set& vars) const {
  Variables_Set::const_iterator j = vars.begin();
  Variables_Set::const_iterator j_end = vars.end();

  for ( ; j != j_end; ++j)
    if (row[*j + 1] != 0)
      return false;

  return true;
}

template <>
bool
Linear_Expression_Impl<Sparse_Row>
::all_zeroes(const Variables_Set& vars) const {
  Sparse_Row::const_iterator i = row.begin();
  Sparse_Row::const_iterator i_end = row.end();
  Variables_Set::const_iterator j = vars.begin();
  Variables_Set::const_iterator j_end = vars.end();

  for ( ; j != j_end; ++j) {
    i = row.lower_bound(i, *j + 1);
    if (i == i_end)
      break;
    if (i.index() == *j + 1)
      return false;
  }

  return true;
}

template <>
bool
Linear_Expression_Impl<Dense_Row>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const {
  if (start == 0) {
    if (row[0] != 0)
      return false;
    ++start;
  }
  for (dimension_type i = start; i < end; ++i)
    if (row[i] != 0 && vars.count(i - 1) == 0)
      return false;
  return true;
}

template <>
bool
Linear_Expression_Impl<Sparse_Row>
::all_zeroes_except(const Variables_Set& vars,
                    dimension_type start, dimension_type end) const {
  PPL_ASSERT(start <= end);
  if (start == end)
    return true;
  if (start == 0) {
    if (row.find(0) != row.end())
      return false;

    start = 1;
  }

  PPL_ASSERT(start != 0);
  PPL_ASSERT(start <= end);
  for (Sparse_Row::const_iterator i = row.lower_bound(start),
         i_end = row.lower_bound(end); i != i_end; ++i)
    if (vars.count(i.index() - 1) == 0)
      return false;

  return true;
}

template <>
dimension_type
Linear_Expression_Impl<Dense_Row>::last_nonzero() const {
  for (dimension_type i = row.size(); i-- > 0; )
    if (row[i] != 0)
      return i;
  return 0;
}

template <>
dimension_type
Linear_Expression_Impl<Dense_Row>
::first_nonzero(dimension_type first, dimension_type last) const {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= row.size());
  for (dimension_type i = first; i < last; ++i)
    if (row[i] != 0)
      return i;

  return last;
}

template <>
dimension_type
Linear_Expression_Impl<Dense_Row>
::last_nonzero(dimension_type first, dimension_type last) const {
  PPL_ASSERT(first <= last);
  PPL_ASSERT(last <= row.size());
  for (dimension_type i = last; i-- > first; )
    if (row[i] != 0)
      return i;

  return last;
}

template <>
void
Linear_Expression_Impl<Dense_Row>
::has_a_free_dimension_helper(std::set<dimension_type>& x) const {
  typedef std::set<dimension_type> set_t;
  set_t result;
  for (set_t::const_iterator i = x.begin(), i_end = x.end(); i != i_end; ++i)
    if (row[*i] == 0)
      result.insert(*i);
  using std::swap;
  swap(x, result);
}

template <>
void
Linear_Expression_Impl<Sparse_Row>
::has_a_free_dimension_helper(std::set<dimension_type>& x) const {
  typedef std::set<dimension_type> set_t;
  set_t result;
  Sparse_Row::const_iterator itr = row.end();
  Sparse_Row::const_iterator itr_end = row.end();
  set_t::const_iterator i = x.begin();
  set_t::const_iterator i_end = x.end();
  for ( ; i != i_end; ++i) {
    itr = row.lower_bound(itr, *i);
    if (itr == itr_end)
      break;
    if (itr.index() != *i)
      result.insert(*i);
  }
  for ( ; i != i_end; ++i)
    result.insert(*i);
  using std::swap;
  swap(x, result);
}

template <>
template <>
bool
Linear_Expression_Impl<Dense_Row>
::have_a_common_variable(const Linear_Expression_Impl<Dense_Row>& y,
                         Variable first, Variable last) const {
  const dimension_type start = first.space_dimension();
  const dimension_type end = last.space_dimension();
  PPL_ASSERT(start <= end);
  PPL_ASSERT(end <= row.size());
  PPL_ASSERT(end <= y.row.size());
  for (dimension_type i = start; i < end; ++i)
    if (row[i] != 0 && y.row[i] != 0)
      return true;
  return false;
}

template <>
template <>
bool
Linear_Expression_Impl<Sparse_Row>
::have_a_common_variable(const Linear_Expression_Impl<Dense_Row>& y,
                         Variable first, Variable last) const {
  const dimension_type start = first.space_dimension();
  const dimension_type end = last.space_dimension();
  PPL_ASSERT(start <= end);
  PPL_ASSERT(end <= row.size());
  PPL_ASSERT(end <= y.row.size());
  for (Sparse_Row::const_iterator i = row.lower_bound(start),
        i_end = row.lower_bound(end); i != i_end; ++i)
    if (y.row[i.index()] != 0)
      return true;
  return false;
}

template <>
template <>
bool
Linear_Expression_Impl<Dense_Row>
::have_a_common_variable(const Linear_Expression_Impl<Sparse_Row>& y,
                         Variable first, Variable last) const {
  return y.have_a_common_variable(*this, first, last);
}

template <>
template <>
bool
Linear_Expression_Impl<Sparse_Row>
::have_a_common_variable(const Linear_Expression_Impl<Sparse_Row>& y,
                         Variable first, Variable last) const {
  const dimension_type start = first.space_dimension();
  const dimension_type end = last.space_dimension();
  PPL_ASSERT(start <= end);
  PPL_ASSERT(end <= row.size());
  PPL_ASSERT(end <= y.row.size());
  Sparse_Row::const_iterator i = row.lower_bound(start);
  Sparse_Row::const_iterator i_end = row.lower_bound(end);
  Sparse_Row::const_iterator j = y.row.lower_bound(start);
  Sparse_Row::const_iterator j_end = y.row.lower_bound(end);
  while (i != i_end && j != j_end) {
    if (i.index() == j.index())
      return true;
    if (i.index() < j.index())
      ++i;
    else
      ++j;
  }
  return false;
}

template <>
void
Linear_Expression_Impl<Dense_Row>::const_iterator
::skip_zeroes_forward() {
  while (itr != row->end() && *itr == 0)
    ++itr;
}

template <>
void
Linear_Expression_Impl<Dense_Row>::const_iterator
::skip_zeroes_backward() {
  PPL_ASSERT(itr.index() > 0);
  while (*itr == 0) {
    PPL_ASSERT(itr.index() > 1);
    --itr;
  }
}

} // namespace Parma_Polyhedra_Library

