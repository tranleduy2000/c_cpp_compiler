/* Matrix class implementation: inline functions.
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

#ifndef PPL_Matrix_inlines_hh
#define PPL_Matrix_inlines_hh 1

namespace Parma_Polyhedra_Library {

template <typename Row>
inline dimension_type
Matrix<Row>::max_num_rows() {
  return std::vector<Row>().max_size();
}

template <typename Row>
inline dimension_type
Matrix<Row>::max_num_columns() {
  return Row::max_size();
}

template <typename Row>
inline void
Matrix<Row>::m_swap(Matrix& x) {
  using std::swap;
  swap(rows, x.rows);
  swap(num_columns_, x.num_columns_);
}

template <typename Row>
inline dimension_type
Matrix<Row>::num_rows() const {
  return rows.size();
}

template <typename Row>
inline dimension_type
Matrix<Row>::num_columns() const {
  return num_columns_;
}

template <typename Row>
inline dimension_type
Matrix<Row>::capacity() const {
  return rows.capacity();
}

template <typename Row>
inline bool
Matrix<Row>::has_no_rows() const {
  return num_rows() == 0;
}

template <typename Row>
inline void
Matrix<Row>::resize(dimension_type n) {
  resize(n, n);
}

template <typename Row>
inline void
Matrix<Row>::reserve_rows(dimension_type requested_capacity) {

  rows.reserve(requested_capacity);
}

template <typename Row>
inline void
Matrix<Row>::add_zero_rows_and_columns(dimension_type n, dimension_type m) {
  resize(num_rows() + n, num_columns() + m);
}

template <typename Row>
inline void
Matrix<Row>::add_zero_rows(dimension_type n) {
  resize(num_rows() + n, num_columns());
}

template <typename Row>
inline void
Matrix<Row>::add_row(const Row& x) {
  // TODO: Optimize this.
  Row row(x);
  add_zero_rows(1);
  // Now x may have been invalidated, if it was a row of this matrix.
  swap(rows.back(), row);
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Matrix<Row>::add_recycled_row(Row& x) {
  add_zero_rows(1);
  swap(rows.back(), x);
  PPL_ASSERT(OK());
}

template <typename Row>
inline void
Matrix<Row>::remove_trailing_rows(dimension_type n) {
  resize(num_rows() - n, num_columns());
}

template <typename Row>
inline void
Matrix<Row>::remove_rows(iterator first, iterator last) {
  rows.erase(first, last);
}

template <typename Row>
inline void
Matrix<Row>::add_zero_columns(dimension_type n) {
  resize(num_rows(), num_columns() + n);
}

template <typename Row>
inline void
Matrix<Row>::remove_trailing_columns(dimension_type n) {
  PPL_ASSERT(n <= num_columns());
  resize(num_rows(), num_columns() - n);
}

template <typename Row>
inline void
Matrix<Row>::clear() {
  resize(0, 0);
}

template <typename Row>
inline typename Matrix<Row>::iterator
Matrix<Row>::begin() {
  return rows.begin();
}

template <typename Row>
inline typename Matrix<Row>::iterator
Matrix<Row>::end() {
  return rows.end();
}

template <typename Row>
inline typename Matrix<Row>::const_iterator
Matrix<Row>::begin() const {
  return rows.begin();
}

template <typename Row>
inline typename Matrix<Row>::const_iterator
Matrix<Row>::end() const {
  return rows.end();
}

template <typename Row>
inline Row&
Matrix<Row>::operator[](dimension_type i) {
  PPL_ASSERT(i < rows.size());
  return rows[i];
}

template <typename Row>
inline const Row&
Matrix<Row>::operator[](dimension_type i) const {
  PPL_ASSERT(i < rows.size());
  return rows[i];
}

template <typename Row>
inline memory_size_type
Matrix<Row>::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

template <typename Row>
inline void
swap(Matrix<Row>& x, Matrix<Row>& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Matrix_inlines_hh)
