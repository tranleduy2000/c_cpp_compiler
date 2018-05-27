/* Matrix class implementation: non-inline template functions.
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

#ifndef PPL_Matrix_templates_hh
#define PPL_Matrix_templates_hh 1

namespace Parma_Polyhedra_Library {

template <typename Row>
Matrix<Row>::Matrix(dimension_type n)
  : rows(n), num_columns_(n) {
  for (dimension_type i = 0; i < rows.size(); ++i)
    rows[i].resize(num_columns_);
  PPL_ASSERT(OK());
}

template <typename Row>
Matrix<Row>::Matrix(dimension_type num_rows, dimension_type num_columns)
  : rows(num_rows), num_columns_(num_columns) {
  for (dimension_type i = 0; i < rows.size(); ++i)
    rows[i].resize(num_columns_);
  PPL_ASSERT(OK());
}

template <typename Row>
void
Matrix<Row>::resize(dimension_type num_rows, dimension_type num_columns) {
  const dimension_type old_num_rows = rows.size();
  rows.resize(num_rows);
  if (old_num_rows < num_rows) {
    for (dimension_type i = old_num_rows; i < num_rows; ++i)
      rows[i].resize(num_columns);
    if (num_columns_ != num_columns) {
      num_columns_ = num_columns;
      for (dimension_type i = 0; i < old_num_rows; ++i)
        rows[i].resize(num_columns);
    }
  }
  else
    if (num_columns_ != num_columns) {
      num_columns_ = num_columns;
      for (dimension_type i = 0; i < num_rows; ++i)
        rows[i].resize(num_columns);
    }
  PPL_ASSERT(OK());
}

template <typename Row>
void
Matrix<Row>::permute_columns(const std::vector<dimension_type>& cycles) {
  PPL_DIRTY_TEMP_COEFFICIENT(tmp);
  const dimension_type n = cycles.size();
  PPL_ASSERT(cycles[n - 1] == 0);
  for (dimension_type k = num_rows(); k-- > 0; ) {
    Row& rows_k = (*this)[k];
    for (dimension_type i = 0, j = 0; i < n; i = ++j) {
      // Make `j' be the index of the next cycle terminator.
      while (cycles[j] != 0)
        ++j;
      // Cycles of length less than 2 are not allowed.
      PPL_ASSERT(j - i >= 2);
      if (j - i == 2)
        // For cycles of length 2 no temporary is needed, just a swap.
        rows_k.swap_coefficients(cycles[i], cycles[i + 1]);
      else {
        // Longer cycles need a temporary.
        tmp = rows_k.get(cycles[j - 1]);
        for (dimension_type l = (j - 1); l > i; --l)
          rows_k.swap_coefficients(cycles[l-1], cycles[l]);
        if (tmp == 0)
          rows_k.reset(cycles[i]);
        else {
          using std::swap;
          swap(tmp, rows_k[cycles[i]]);
        }
      }
    }
  }
}

template <typename Row>
void
Matrix<Row>::swap_columns(dimension_type i, dimension_type j) {
  for (dimension_type k = num_rows(); k-- > 0; )
    (*this)[k].swap_coefficients(i, j);
}

template <typename Row>
void
Matrix<Row>::add_zero_columns(dimension_type n, dimension_type i) {
  for (dimension_type j = rows.size(); j-- > 0; )
    rows[j].add_zeroes_and_shift(n, i);
  num_columns_ += n;
  PPL_ASSERT(OK());
}

template <typename Row>
void
Matrix<Row>::remove_column(dimension_type i) {
  for (dimension_type j = rows.size(); j-- > 0; )
    rows[j].delete_element_and_shift(i);
  --num_columns_;
  PPL_ASSERT(OK());
}

template <typename Row>
void
Matrix<Row>::ascii_dump(std::ostream& s) const {
  s << num_rows() << " x ";
  s << num_columns() << "\n";
  for (const_iterator i = begin(), i_end = end(); i !=i_end; ++i)
    i->ascii_dump(s);
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS_ASCII_ONLY(Row, Matrix<Row>)

template <typename Row>
bool
Matrix<Row>::ascii_load(std::istream& s) {
  std::string str;
  dimension_type new_num_rows;
  dimension_type new_num_cols;
  if (!(s >> new_num_rows))
    return false;
  if (!(s >> str) || str != "x")
    return false;
  if (!(s >> new_num_cols))
    return false;

  for (iterator i = rows.begin(), i_end = rows.end(); i != i_end; ++i)
    i->clear();

  resize(new_num_rows, new_num_cols);

  for (dimension_type row = 0; row < new_num_rows; ++row)
    if (!rows[row].ascii_load(s))
      return false;

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

template <typename Row>
memory_size_type
Matrix<Row>::external_memory_in_bytes() const {
  return rows.external_memory_in_bytes();
}

template <typename Row>
bool
Matrix<Row>::OK() const {
  for (const_iterator i = begin(), i_end = end(); i != i_end; ++i)
    if (i->size() != num_columns_)
      return false;
  return true;
}

/*! \relates Parma_Polyhedra_Library::Matrix */
template <typename Row>
bool
operator==(const Matrix<Row>& x, const Matrix<Row>& y) {
  if (x.num_rows() != y.num_rows())
    return false;
  if (x.num_columns() != y.num_columns())
    return false;
  for (dimension_type i = x.num_rows(); i-- > 0; )
    if (x[i] != y[i])
      return false;
  return true;
}

/*! \relates Parma_Polyhedra_Library::Matrix */
template <typename Row>
bool
operator!=(const Matrix<Row>& x, const Matrix<Row>& y) {
  return !(x == y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Matrix_templates_hh)
