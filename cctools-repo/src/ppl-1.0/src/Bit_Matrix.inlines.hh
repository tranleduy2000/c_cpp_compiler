/* Bit_Matrix class implementation: inline functions.
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

#ifndef PPL_Bit_Matrix_inlines_hh
#define PPL_Bit_Matrix_inlines_hh 1

#include <algorithm>
#include "assert.hh"

namespace Parma_Polyhedra_Library {

inline
Bit_Matrix::Bit_Matrix()
  : rows(),
    row_size(0) {
}

inline dimension_type
Bit_Matrix::max_num_rows() {
  return std::vector<Bit_Row>().max_size();
}

inline
Bit_Matrix::Bit_Matrix(const dimension_type n_rows,
		       const dimension_type n_columns)
  : rows(n_rows),
    row_size(n_columns) {
}

inline
Bit_Matrix::Bit_Matrix(const Bit_Matrix& y)
  : rows(y.rows),
    row_size(y.row_size) {
}

inline
Bit_Matrix::~Bit_Matrix() {
}

inline void
Bit_Matrix::remove_trailing_rows(const dimension_type n) {
  // The number of rows to be erased cannot be greater
  // than the actual number of the rows of the matrix.
  PPL_ASSERT(n <= rows.size());
  if (n != 0)
    rows.resize(rows.size() - n);
  PPL_ASSERT(OK());
}

inline void
Bit_Matrix::remove_trailing_columns(const dimension_type n) {
  // The number of columns to be erased cannot be greater
  // than the actual number of the columns of the matrix.
  PPL_ASSERT(n <= row_size);
  row_size -= n;
  PPL_ASSERT(OK());
}

inline void
Bit_Matrix::m_swap(Bit_Matrix& y) {
  using std::swap;
  swap(row_size, y.row_size);
  swap(rows, y.rows);
}

inline Bit_Row&
Bit_Matrix::operator[](const dimension_type k) {
  PPL_ASSERT(k < rows.size());
  return rows[k];
}

inline const Bit_Row&
Bit_Matrix::operator[](const dimension_type k) const {
  PPL_ASSERT(k < rows.size());
  return rows[k];
}

inline dimension_type
Bit_Matrix::num_columns() const {
  return row_size;
}

inline dimension_type
Bit_Matrix::num_rows() const {
  return rows.size();
}

inline void
Bit_Matrix::clear() {
  // Clear `rows' and minimize its capacity.
  std::vector<Bit_Row> tmp;
  using std::swap;
  swap(tmp, rows);
  row_size = 0;
}

inline memory_size_type
Bit_Matrix::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline bool
Bit_Matrix::Bit_Row_Less_Than::
operator()(const Bit_Row& x, const Bit_Row& y) const {
  return compare(x, y) < 0;
}

inline bool
Bit_Matrix::sorted_contains(const Bit_Row& row) const {
  PPL_ASSERT(check_sorted());
  return std::binary_search(rows.begin(), rows.end(), row,
			    Bit_Row_Less_Than());
}

/*! \relates Bit_Matrix */
inline bool
operator!=(const Bit_Matrix& x, const Bit_Matrix& y) {
  return !(x == y);
}

/*! \relates Bit_Matrix */
inline void
swap(Bit_Matrix& x, Bit_Matrix& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Bit_Matrix_inlines_hh)
