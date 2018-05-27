/* Matrix class declaration.
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

#ifndef PPL_Matrix_defs_hh
#define PPL_Matrix_defs_hh 1

#include "Matrix_types.hh"
#include "globals_defs.hh"
#include "Coefficient_defs.hh"
#include "Swapping_Vector_defs.hh"
#include <ostream>

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A sparse matrix of Coefficient.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
class Parma_Polyhedra_Library::Matrix {

public:
  typedef typename Swapping_Vector<Row>::iterator iterator;
  typedef typename Swapping_Vector<Row>::const_iterator const_iterator;

  //! Returns the maximum number of rows of a Sparse_Matrix.
  static dimension_type max_num_rows();

  //! Returns the maximum number of columns of a Sparse_Matrix.
  static dimension_type max_num_columns();

  /*!
    \brief Constructs a square matrix with the given size, filled with
           unstored zeroes.

    \param n
    The size of the new square matrix.

    This method takes \f$O(n)\f$ time.
  */
  explicit Matrix(dimension_type n = 0);

  /*!
    \brief Constructs a matrix with the given dimensions, filled with unstored
           zeroes.

    \param num_rows
    The number of rows in the new matrix.

    \param num_columns
    The number of columns in the new matrix.

    This method takes \f$O(n)\f$ time, where n is \p num_rows.
  */
  Matrix(dimension_type num_rows, dimension_type num_columns);

  //! Swaps (*this) with x.
  /*!

    \param x
    The matrix that will be swapped with *this.

    This method takes \f$O(1)\f$ time.
  */
  void m_swap(Matrix& x);

  //! Returns the number of rows in the matrix.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  dimension_type num_rows() const;

  //! Returns the number of columns in the matrix.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  dimension_type num_columns() const;

  // TODO: Check if this can be removed.
  //! Returns the capacity of the row vector.
  dimension_type capacity() const;

  //! Returns <CODE>true</CODE> if and only if \p *this has no rows.
  /*!
    \note
    The unusual naming for this method is \em intentional:
    we do not want it to be named \c empty because this would cause
    an error prone name clash with the corresponding methods in derived
    classes Constraint_System and Congruence_System (which have a
    different semantics).
  */
  bool has_no_rows() const;

  //! Equivalent to resize(n, n).
  void resize(dimension_type n);

  // TODO: Check if this can become private.
  //! Reserves space for at least \p n rows.
  void reserve_rows(dimension_type n);

  //! Resizes this matrix to the specified dimensions.
  /*!

    \param num_rows
    The desired numer of rows.

    \param num_columns
    The desired numer of columns.

    New rows and columns will contain non-stored zeroes.

    This operation invalidates existing iterators.

    Adding n rows takes \f$O(n)\f$ amortized time.

    Adding n columns takes \f$O(r)\f$ time, where r is \p num_rows.

    Removing n rows takes \f$O(n+k)\f$ amortized time, where k is the total
    number of elements stored in the removed rows.

    Removing n columns takes \f$O(\sum_{j=1}^{r} (k_j*\log^2 n_j))\f$ time,
    where r is the number of rows, \f$k_j\f$ is the number of elements stored
    in the columns of the j-th row that must be removed and \f$n_j\f$ is the
    total number of elements stored in the j-th row.
    A weaker (but simpler) bound is \f$O(r+k*\log^2 c)\f$, where r is the
    number of rows, k is the number of elements that have to be removed and c
    is the number of columns.
  */
  void resize(dimension_type num_rows, dimension_type num_columns);

  //! Adds \p n rows and \p m columns of zeroes to the matrix.
  /*!
    \param n
    The number of rows to be added: must be strictly positive.

    \param m
    The number of columns to be added: must be strictly positive.

    Turns the \f$r \times c\f$ matrix \f$M\f$ into
    the \f$(r+n) \times (c+m)\f$ matrix
    \f$\bigl(\genfrac{}{}{0pt}{}{M}{0} \genfrac{}{}{0pt}{}{0}{0}\bigr)\f$.
    The matrix is expanded avoiding reallocation whenever possible.

    This method takes \f$O(r)\f$ time, where r is the number of the matrix's
    rows after the operation.
  */
  void add_zero_rows_and_columns(dimension_type n, dimension_type m);

  //! Adds to the matrix \p n rows of zeroes.
  /*!
    \param n
    The number of rows to be added: must be strictly positive.

    Turns the \f$r \times c\f$ matrix \f$M\f$ into
    the \f$(r+n) \times c\f$ matrix \f$\genfrac{(}{)}{0pt}{}{M}{0}\f$.
    The matrix is expanded avoiding reallocation whenever possible.

    This method takes \f$O(k)\f$ amortized time, where k is the number of the
    new rows.
  */
  void add_zero_rows(dimension_type n);

  //! Adds a copy of the row \p x at the end of the matrix.
  /*!

    \param x
    The row that will be appended to the matrix.

    This operation invalidates existing iterators.

    This method takes \f$O(n)\f$ amortized time, where n is the numer of
    elements stored in \p x.
  */
  void add_row(const Row& x);

  //! Adds the row \p y to the matrix.
  /*!
    \param y
    The row to be added: it must have the same size and capacity as
    \p *this. It is not declared <CODE>const</CODE> because its
    data-structures will recycled to build the new matrix row.

    Turns the \f$r \times c\f$ matrix \f$M\f$ into
    the \f$(r+1) \times c\f$ matrix
    \f$\genfrac{(}{)}{0pt}{}{M}{y}\f$.
    The matrix is expanded avoiding reallocation whenever possible.
  */
  void add_recycled_row(Row& y);

  /*! \brief
    Removes from the matrix the last \p n rows.

    \param n
    The number of row that will be removed.

    It is equivalent to num_rows() - n, num_columns()).

    This method takes \f$O(n+k)\f$ amortized time, where k is the total number
    of elements stored in the removed rows and n is the number of removed
    rows.
  */
  void remove_trailing_rows(dimension_type n);

  void remove_rows(iterator first, iterator last);

  //! Permutes the columns of the matrix.
  /*!
    This method may be slow for some Row types, and should be avoided if
    possible.

    \param cycles
    A vector representing the non-trivial cycles of the permutation
    according to which the columns must be rearranged.

    The \p cycles vector contains, one after the other, the
    non-trivial cycles (i.e., the cycles of length greater than one)
    of a permutation of \e non-zero column indexes.  Each cycle is
    terminated by zero.  For example, assuming the matrix has 7
    columns, the permutation \f$ \{ 1 \mapsto 3, 2 \mapsto 4,
    3 \mapsto 6, 4 \mapsto 2, 5 \mapsto 5, 6 \mapsto 1 \}\f$ can be
    represented by the non-trivial cycles \f$(1 3 6)(2 4)\f$ that, in
    turn can be represented by a vector of 6 elements containing 1, 3,
    6, 0, 2, 4, 0.

    This method takes \f$O(k*\sum_{j=1}^{r} \log^2 n_j)\f$ expected time,
    where k is the size of the \p cycles vector, r the number of rows and
    \f$n_j\f$ the number of elements stored in row j.
    A weaker (but simpler) bound is \f$O(k*r*\log^2 c)\f$, where k is the size
    of the \p cycles vector, r is the number of rows and c is the number of
    columns.

    \note
    The first column of the matrix, having index zero, is never involved
    in a permutation.
  */
  void permute_columns(const std::vector<dimension_type>& cycles);

  //! Swaps the columns having indexes \p i and \p j.
  void swap_columns(dimension_type i,  dimension_type j);

  //! Adds \p n columns of zeroes to the matrix.
  /*!
    \param n
    The number of columns to be added: must be strictly positive.

    Turns the \f$r \times c\f$ matrix \f$M\f$ into
    the \f$r \times (c+n)\f$ matrix \f$(M \, 0)\f$.

    This method takes \f$O(r)\f$ amortized time, where r is the numer of the
    matrix's rows.
  */
  void add_zero_columns(dimension_type n);

  //! Adds \p n columns of non-stored zeroes to the matrix before column i.
  /*!

    \param n
    The numer of columns that will be added.

    \param i
    The index of the column before which the new columns will be added.

    This operation invalidates existing iterators.

    This method takes \f$O(\sum_{j=1}^{r} (k_j+\log n_j))\f$ time, where r is
    the number of rows, \f$k_j\f$ is the number of elements stored in the
    columns of the j-th row that must be shifted and \f$n_j\f$ is the number
    of elements stored in the j-th row.
    A weaker (but simpler) bound is \f$O(k+r*\log c)\f$ time, where k is the
    number of elements that must be shifted, r is the number of the rows and c
    is the number of the columns.
  */
  void add_zero_columns(dimension_type n, dimension_type i);

  //! Removes the i-th from the matrix, shifting other columns to the left.
  /*!

    \param i
    The index of the column that will be removed.

    This operation invalidates existing iterators on rows' elements.

    This method takes \f$O(k + \sum_{j=1}^{r} (\log^2 n_j))\f$ amortized time,
    where k is the number of elements stored with column index greater than i,
    r the number of rows in this matrix and \f$n_j\f$ the number of elements
    stored in row j.
    A weaker (but simpler) bound is \f$O(r*(c-i+\log^2 c))\f$, where r is the
    number of rows, c is the number of columns and i is the parameter passed
    to this method.
  */
  void remove_column(dimension_type i);

  //! Shrinks the matrix by removing its \p n trailing columns.
  /*!

    \param n
    The number of trailing columns that will be removed.

    This operation invalidates existing iterators.

    This method takes \f$O(\sum_{j=1}^r (k_j*\log n_j))\f$ amortized time,
    where r is the number of rows, \f$k_j\f$ is the number of elements that
    have to be removed from row j and \f$n_j\f$ is the total number of
    elements stored in row j.
    A weaker (but simpler) bound is \f$O(r*n*\log c)\f$, where r is the number
    of rows, c the number of columns and n the parameter passed to this
    method.
  */
  void remove_trailing_columns(dimension_type n);

  //! Equivalent to resize(0,0).
  void clear();

  //! Returns an %iterator pointing to the first row.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  iterator begin();

  //! Returns an %iterator pointing after the last row.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  iterator end();

  //! Returns an %iterator pointing to the first row.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  const_iterator begin() const;

  //! Returns an %iterator pointing after the last row.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  const_iterator end() const;

  //! Returns a reference to the i-th row.
  /*!
    \param i
    The index of the desired row.

    This method takes \f$O(1)\f$ time.
  */
  Row& operator[](dimension_type i);

  //! Returns a const reference to the i-th row.
  /*!
    \param i
    The index of the desired row.

    This method takes \f$O(1)\f$ time.
  */
  const Row& operator[](dimension_type i) const;

  //! Loads the row from an ASCII representation generated using ascii_dump().
  /*!
    \param s
    The stream from which read the ASCII representation.

    This method takes \f$O(n*\log n)\f$ time.
  */
  bool ascii_load(std::istream& s);

  PPL_OUTPUT_DECLARATIONS

  //! Returns the total size in bytes of the memory occupied by \p *this.
  /*!
    This method is \f$O(r+k)\f$, where r is the number of rows and k is the
    number of elements stored in the matrix.
  */
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  /*!
    This method is \f$O(r+k)\f$, where r is the number of rows and k is the
    number of elements stored in the matrix.
  */
  memory_size_type external_memory_in_bytes() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

private:
  //! The vector that stores the matrix's elements.
  Swapping_Vector<Row> rows;

  //! The number of columns in this matrix.
  dimension_type num_columns_;
};

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
void swap(Matrix<Row>& x, Matrix<Row>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are identical.
/*! \relates Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
bool operator==(const Matrix<Row>& x, const Matrix<Row>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
/*! \relates Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Row>
bool operator!=(const Matrix<Row>& x, const Matrix<Row>& y);

} // namespace Parma_Polyhedra_Library


#include "Matrix_inlines.hh"
#include "Matrix_templates.hh"

#endif // !defined(PPL_Matrix_defs_hh)
