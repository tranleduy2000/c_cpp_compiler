/* DB_Matrix class implementation: inline functions.
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

#ifndef PPL_DB_Matrix_inlines_hh
#define PPL_DB_Matrix_inlines_hh 1

#include "globals.defs.hh"
#include "Checked_Number.defs.hh"
#include "distances.defs.hh"
#include "assert.hh"
#include <iostream>

namespace Parma_Polyhedra_Library {

template <typename T>
inline void
DB_Matrix<T>::m_swap(DB_Matrix& y) {
  using std::swap;
  swap(rows, y.rows);
  swap(row_size, y.row_size);
  swap(row_capacity, y.row_capacity);
}

template <typename T>
inline dimension_type
DB_Matrix<T>::max_num_rows() {
  return std::vector<DB_Row<T> >().max_size();
}

template <typename T>
inline dimension_type
DB_Matrix<T>::max_num_columns() {
  return DB_Row<T>::max_size();
}

template <typename T>
inline memory_size_type
DB_Matrix<T>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename T>
inline
DB_Matrix<T>::const_iterator::const_iterator()
  : i(Iter()) {
}

template <typename T>
inline
DB_Matrix<T>::const_iterator::const_iterator(const Iter& b)
  : i(b) {
}

template <typename T>
inline
DB_Matrix<T>::const_iterator::const_iterator(const const_iterator& y)
  : i(y.i) {
}

template <typename T>
inline typename DB_Matrix<T>::const_iterator&
DB_Matrix<T>::const_iterator::operator=(const const_iterator& y) {
  i = y.i;
  return *this;
}

template <typename T>
inline typename DB_Matrix<T>::const_iterator::reference
DB_Matrix<T>::const_iterator::operator*() const {
  return *i;
}

template <typename T>
inline typename DB_Matrix<T>::const_iterator::pointer
DB_Matrix<T>::const_iterator::operator->() const {
  return &*i;
}

template <typename T>
inline typename DB_Matrix<T>::const_iterator&
DB_Matrix<T>::const_iterator::operator++() {
  ++i;
  return *this;
}

template <typename T>
inline typename DB_Matrix<T>::const_iterator
DB_Matrix<T>::const_iterator::operator++(int) {
  return const_iterator(i++);
}

template <typename T>
inline bool
DB_Matrix<T>::const_iterator::operator==(const const_iterator& y) const {
  return i == y.i;
}

template <typename T>
inline bool
DB_Matrix<T>::const_iterator::operator!=(const const_iterator& y) const {
  return !operator==(y);
}

template <typename T>
inline typename DB_Matrix<T>::const_iterator
DB_Matrix<T>::begin() const {
  return const_iterator(rows.begin());
}

template <typename T>
inline typename DB_Matrix<T>::const_iterator
DB_Matrix<T>::end() const {
  return const_iterator(rows.end());
}

template <typename T>
inline
DB_Matrix<T>::DB_Matrix()
  : rows(),
    row_size(0),
    row_capacity(0) {
}

template <typename T>
inline
DB_Matrix<T>::~DB_Matrix() {
}

template <typename T>
inline DB_Row<T>&
DB_Matrix<T>::operator[](const dimension_type k) {
  PPL_ASSERT(k < rows.size());
  return rows[k];
}

template <typename T>
inline const DB_Row<T>&
DB_Matrix<T>::operator[](const dimension_type k) const {
  PPL_ASSERT(k < rows.size());
  return rows[k];
}

template <typename T>
inline dimension_type
DB_Matrix<T>::num_rows() const {
  return rows.size();
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
inline bool
operator!=(const DB_Matrix<T>& x, const DB_Matrix<T>& y) {
  return !(x == y);
}

template <typename T>
inline
DB_Matrix<T>::DB_Matrix(const DB_Matrix& y)
  : rows(y.rows),
    row_size(y.row_size),
    row_capacity(compute_capacity(y.row_size, max_num_columns())) {
}

template <typename T>
inline DB_Matrix<T>&
DB_Matrix<T>::operator=(const DB_Matrix& y) {
  // Without the following guard against auto-assignments we would
  // recompute the row capacity based on row size, possibly without
  // actually increasing the capacity of the rows.  This would lead to
  // an inconsistent state.
  if (this != &y) {
    // The following assignment may do nothing on auto-assignments...
    rows = y.rows;
    row_size = y.row_size;
    // ... hence the following assignment must not be done on
    // auto-assignments.
    row_capacity = compute_capacity(y.row_size, max_num_columns());
  }
  return *this;
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Specialization, typename Temp, typename To, typename T>
inline bool
l_m_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
		    const DB_Matrix<T>& x,
		    const DB_Matrix<T>& y,
		    const Rounding_Dir dir,
		    Temp& tmp0,
		    Temp& tmp1,
		    Temp& tmp2) {
  const dimension_type x_num_rows = x.num_rows();
  if (x_num_rows != y.num_rows())
    return false;
  assign_r(tmp0, 0, ROUND_NOT_NEEDED);
  for (dimension_type i = x_num_rows; i-- > 0; ) {
    const DB_Row<T>& x_i = x[i];
    const DB_Row<T>& y_i = y[i];
    for (dimension_type j = x_num_rows; j-- > 0; ) {
      const T& x_i_j = x_i[j];
      const T& y_i_j = y_i[j];
      if (is_plus_infinity(x_i_j)) {
	if (is_plus_infinity(y_i_j))
	  continue;
	else {
	pinf:
	  assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
	  return true;
	}
      }
      else if (is_plus_infinity(y_i_j))
	goto pinf;

      const Temp* tmp1p;
      const Temp* tmp2p;
      if (x_i_j > y_i_j) {
	maybe_assign(tmp1p, tmp1, x_i_j, dir);
	maybe_assign(tmp2p, tmp2, y_i_j, inverse(dir));
      }
      else {
	maybe_assign(tmp1p, tmp1, y_i_j, dir);
	maybe_assign(tmp2p, tmp2, x_i_j, inverse(dir));
      }
      sub_assign_r(tmp1, *tmp1p, *tmp2p, dir);
      PPL_ASSERT(sgn(tmp1) >= 0);
      Specialization::combine(tmp0, tmp1, dir);
    }
  }
  Specialization::finalize(tmp0, dir);
  assign_r(r, tmp0, dir);
  return true;
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
inline bool
rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			    const DB_Matrix<T>& x,
			    const DB_Matrix<T>& y,
			    const Rounding_Dir dir,
			    Temp& tmp0,
			    Temp& tmp1,
			    Temp& tmp2) {
  return
    l_m_distance_assign<Rectilinear_Distance_Specialization<Temp> >(r, x, y,
								    dir,
								    tmp0,
								    tmp1,
								    tmp2);
}


#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
inline bool
euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			  const DB_Matrix<T>& x,
			  const DB_Matrix<T>& y,
			  const Rounding_Dir dir,
			  Temp& tmp0,
			  Temp& tmp1,
			  Temp& tmp2) {
  return
    l_m_distance_assign<Euclidean_Distance_Specialization<Temp> >(r, x, y,
								  dir,
								  tmp0,
								  tmp1,
								  tmp2);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
inline bool
l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			   const DB_Matrix<T>& x,
			   const DB_Matrix<T>& y,
			   const Rounding_Dir dir,
			   Temp& tmp0,
			   Temp& tmp1,
			   Temp& tmp2) {
  return
    l_m_distance_assign<L_Infinity_Distance_Specialization<Temp> >(r, x, y,
								   dir,
								   tmp0,
								   tmp1,
								   tmp2);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
inline void
swap(DB_Matrix<T>& x, DB_Matrix<T>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_DB_Matrix_inlines_hh)
