/* DB_Matrix class declaration.
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

#ifndef PPL_DB_Matrix_defs_hh
#define PPL_DB_Matrix_defs_hh 1

#include "DB_Matrix.types.hh"
#include "globals.defs.hh"
#include "DB_Row.defs.hh"
#include "Checked_Number.types.hh"
#include "Rounding_Dir.defs.hh"
#include <vector>
#include <cstddef>
#include <iosfwd>

namespace Parma_Polyhedra_Library {

namespace IO_Operators {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Output operator.
/*! \relates Parma_Polyhedra_Library::DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
std::ostream&
operator<<(std::ostream& s, const DB_Matrix<T>& c);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library


#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! The base class for the square matrices.
/*! \ingroup PPL_CXX_interface
  The template class DB_Matrix<T> allows for the representation of
  a square matrix of T objects.
  Each DB_Matrix<T> object can be viewed as a multiset of DB_Row<T>.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Parma_Polyhedra_Library::DB_Matrix {
public:
  //! Returns the maximum number of rows a DB_Matrix can handle.
  static dimension_type max_num_rows();

  //! Returns the maximum number of columns a DB_Matrix can handle.
  static dimension_type max_num_columns();

  //! Builds an empty matrix.
  /*!
    DB_Rows' size and capacity are initialized to \f$0\f$.
  */
  DB_Matrix();

  //! Builds a square matrix having the specified dimension.
  explicit DB_Matrix(dimension_type n_rows);

  //! Copy constructor.
  DB_Matrix(const DB_Matrix& y);

  //! Constructs a conservative approximation of \p y.
  template <typename U>
  explicit DB_Matrix(const DB_Matrix<U>& y);

  //! Destructor.
  ~DB_Matrix();

  //! Assignment operator.
  DB_Matrix& operator=(const DB_Matrix& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
  //! A read-only iterator over the rows of the matrix.
  /*! \ingroup PPL_CXX_interface */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
  class const_iterator {
  private:
    typedef typename std::vector<DB_Row<T> >::const_iterator Iter;
    //! The const iterator on the rows' vector \p rows.
    Iter i;

  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    typedef typename std::iterator_traits<Iter>::difference_type
    difference_type;
    typedef typename std::iterator_traits<Iter>::pointer pointer;
    typedef typename std::iterator_traits<Iter>::reference reference;

    //! Default constructor.
    const_iterator();

    /*! \brief
      Builds a const iterator on the matrix starting from
      an iterator \p b on the elements of the vector \p rows.
    */
    explicit const_iterator(const Iter& b);

    //! Ordinary copy constructor.
    const_iterator(const const_iterator& y);

    //! Assignment operator.
    const_iterator& operator=(const const_iterator& y);

    //! Dereference operator.
    reference operator*() const;

    //! Indirect member selector.
    pointer operator->() const;

    //! Prefix increment operator.
    const_iterator& operator++();

    //! Postfix increment operator.
    const_iterator operator++(int);

    /*! \brief
      Returns <CODE>true</CODE> if and only if
      \p *this and \p y are identical.
    */
    bool operator==(const const_iterator& y) const;

    /*! \brief
      Returns <CODE>true</CODE> if and only if
      \p *this and \p y are different.
    */
    bool operator!=(const const_iterator& y) const;
  };

  /*! \brief
    Returns the const_iterator pointing to the first row,
    if \p *this is not empty;
    otherwise, returns the past-the-end const_iterator.
  */
  const_iterator begin() const;

  //! Returns the past-the-end const_iterator.
  const_iterator end() const;

private:
  template <typename U> friend class DB_Matrix;

  //! The rows of the matrix.
  std::vector<DB_Row<T> > rows;

  //! Size of the initialized part of each row.
  dimension_type row_size;

  /*! \brief
    Capacity allocated for each row, i.e., number of
    <CODE>long</CODE> objects that each row can contain.
  */
  dimension_type row_capacity;

public:
  //! Swaps \p *this with \p y.
  void m_swap(DB_Matrix& y);

  //! Makes the matrix grow by adding more rows and more columns.
  /*!
    \param new_n_rows
    The number of rows and columns of the resized matrix.

    A new matrix, with the specified dimension, is created.
    The contents of the old matrix are copied in the upper, left-hand
    corner of the new matrix, which is then assigned to \p *this.
  */
  void grow(dimension_type new_n_rows);

  //! Resizes the matrix without worrying about the old contents.
  /*!
    \param new_n_rows
    The number of rows and columns of the resized matrix.

    A new matrix, with the specified dimension, is created without copying
    the content of the old matrix and assigned to \p *this.
  */
  void resize_no_copy(dimension_type new_n_rows);

  //! Returns the number of rows in the matrix.
  dimension_type num_rows() const;

  //! \name Subscript operators.
  //@{
  //! Returns a reference to the \p k-th row of the matrix.
  DB_Row<T>& operator[](dimension_type k);

  //! Returns a constant reference to the \p k-th row of the matrix.
  const DB_Row<T>& operator[](dimension_type k) const;
  //@}

  PPL_OUTPUT_DECLARATIONS

  /*! \brief
    Loads from \p s an ASCII representation (as produced by
    ascii_dump(std::ostream&) const) and sets \p *this accordingly.
    Returns <CODE>true</CODE> if successful, <CODE>false</CODE> otherwise.
  */
  bool ascii_load(std::istream& s);

  //! Returns the total size in bytes of the memory occupied by \p *this.
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  memory_size_type external_memory_in_bytes() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;
};

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
void swap(DB_Matrix<T>& x, DB_Matrix<T>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are identical.
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
bool operator==(const DB_Matrix<T>& x, const DB_Matrix<T>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
/*! \relates DB_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
bool operator!=(const DB_Matrix<T>& x, const DB_Matrix<T>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates DB_Matrix
  If the rectilinear distance between \p x and \p y is defined,
  stores an approximation of it into to \p r
  and returns <CODE>true</CODE>;  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
bool rectilinear_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
				 const DB_Matrix<T>& x,
				 const DB_Matrix<T>& y,
				 Rounding_Dir dir,
				 Temp& tmp0,
				 Temp& tmp1,
				 Temp& tmp2);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Computes the euclidean distance between \p x and \p y.
/*! \relates DB_Matrix
  If the Euclidean distance between \p x and \p y is defined,
  stores an approximation of it into to \p r
  and returns <CODE>true</CODE>;  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
bool euclidean_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
			       const DB_Matrix<T>& x,
			       const DB_Matrix<T>& y,
			       Rounding_Dir dir,
			       Temp& tmp0,
			       Temp& tmp1,
			       Temp& tmp2);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates DB_Matrix
  If the \f$L_\infty\f$ distance between \p x and \p y is defined,
  stores an approximation of it into to \p r
  and returns <CODE>true</CODE>;  returns <CODE>false</CODE> otherwise.

  The direction of the approximation is specified by \p dir.

  All computations are performed using the temporary variables
  \p tmp0, \p tmp1 and \p tmp2.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Temp, typename To, typename T>
bool l_infinity_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                                const DB_Matrix<T>& x,
                                const DB_Matrix<T>& y,
                                Rounding_Dir dir,
                                Temp& tmp0,
                                Temp& tmp1,
                                Temp& tmp2);

} // namespace Parma_Polyhedra_Library

#include "DB_Matrix.inlines.hh"
#include "DB_Matrix.templates.hh"

#endif // !defined(PPL_DB_Matrix_defs_hh)
