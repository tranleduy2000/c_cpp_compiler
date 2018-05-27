/* OR_Matrix class declaration.
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

#ifndef PPL_OR_Matrix_defs_hh
#define PPL_OR_Matrix_defs_hh 1

#include "globals.defs.hh"
#include "OR_Matrix.types.hh"
#include "DB_Row.defs.hh"
#include "Checked_Number.defs.hh"
#include <cstddef>
#include <iosfwd>

#ifndef PPL_OR_MATRIX_EXTRA_DEBUG
#ifdef PPL_ABI_BREAKING_EXTRA_DEBUG
#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  When PPL_OR_MATRIX_EXTRA_DEBUG evaluates to <CODE>true</CODE>, each
  instance of the class OR_Matrix::Pseudo_Row carries its own size;
  this enables extra consistency checks to be performed.
  \ingroup PPL_CXX_interface
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define PPL_OR_MATRIX_EXTRA_DEBUG 1
#else // !defined(PPL_ABI_BREAKING_EXTRA_DEBUG)
#define PPL_OR_MATRIX_EXTRA_DEBUG 0
#endif // !defined(PPL_ABI_BREAKING_EXTRA_DEBUG)
#endif // !defined(PPL_OR_MATRIX_EXTRA_DEBUG)

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are identical.
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
bool operator==(const OR_Matrix<T>& x, const OR_Matrix<T>& y);

namespace IO_Operators {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Output operator.
/*! \relates Parma_Polyhedra_Library::OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
std::ostream&
operator<<(std::ostream& s, const OR_Matrix<T>& m);

} // namespace IO_Operators

} // namespace Parma_Polyhedra_Library


#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A matrix representing octagonal constraints.
/*!
  An OR_Matrix object is a DB_Row object that allows
  the representation of a \em pseudo-triangular matrix,
  like the following:

<PRE>
         _ _
   0    |_|_|
   1    |_|_|_ _
   2    |_|_|_|_|
   3    |_|_|_|_|_ _
   4    |_|_|_|_|_|_|
   5    |_|_|_|_|_|_|
         . . .
         _ _ _ _ _ _       _
 2n-2   |_|_|_|_|_|_| ... |_|
 2n-1   |_|_|_|_|_|_| ... |_|
         0 1 2 3 4 5  ... 2n-1

</PRE>

  It is characterized by parameter n that defines the structure,
  and such that there are 2*n rows (and 2*n columns).
  It provides row_iterators for the access to the rows
  and element_iterators for the access to the elements.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)

template <typename T>
class Parma_Polyhedra_Library::OR_Matrix {
private:
  /*! \brief
    An object that behaves like a matrix's row with respect to
    the subscript operators.
  */
  template <typename U>
  class Pseudo_Row {
  public:
    /*! \brief
      Copy constructor allowing the construction of a const pseudo-row
      from a non-const pseudo-row.
      Ordinary copy constructor.
    */
    template <typename V>
    Pseudo_Row(const Pseudo_Row<V>& y);

    //! Destructor.
    ~Pseudo_Row();

    //! Subscript operator.
    U& operator[](dimension_type k) const;

    //! Default constructor: creates an invalid object that has to be assigned.
    Pseudo_Row();

    //! Assignment operator.
    Pseudo_Row& operator=(const Pseudo_Row& y);

#if !defined(__GNUC__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)
  private:
#else
  // Work around a bug of GCC 4.0.x (and, likely, previous versions).
  public:
#endif

#if PPL_OR_MATRIX_EXTRA_DEBUG

    //! Private constructor for a Pseudo_Row with size \p s beginning at \p y.
    Pseudo_Row(U& y, dimension_type s);

#else // !PPL_OR_MATRIX_EXTRA_DEBUG

    //! Private constructor for a Pseudo_Row beginning at \p y.
    explicit Pseudo_Row(U& y);

#endif // !PPL_OR_MATRIX_EXTRA_DEBUG

    //! Holds a reference to the beginning of this row.
    U* first;

#if !defined(__GNUC__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ > 0)
#else
  // Work around a bug of GCC 4.0.x (and, likely, previous versions).
  private:
#endif

#if PPL_OR_MATRIX_EXTRA_DEBUG

    //! The size of the row.
    dimension_type size_;

    //! Returns the size of the row.
    dimension_type size() const;

#endif // PPL_OR_MATRIX_EXTRA_DEBUG

    // FIXME: the EDG-based compilers (such as Comeau and Intel)
    // are here in wild disagreement with GCC: what is a legal friend
    // declaration for one, is illegal for the others.
#ifdef __EDG__
    template <typename V> template<typename W>
    friend class OR_Matrix<V>::Pseudo_Row;
    template <typename V> template<typename W>
    friend class OR_Matrix<V>::any_row_iterator;
#else
    template <typename V> friend class Pseudo_Row;
    template <typename V> friend class any_row_iterator;
#endif

    friend class OR_Matrix;
  }; // class Pseudo_Row

public:
  //! A (non const) reference to a matrix's row.
  typedef Pseudo_Row<T> row_reference_type;

  //! A const reference to a matrix's row.
  typedef Pseudo_Row<const T> const_row_reference_type;

private:
  /*! \brief
    A template class to derive both OR_Matrix::iterator
    and OR_Matrix::const_iterator.
  */
  template <typename U>
  class any_row_iterator {
  public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef Pseudo_Row<U> value_type;
    typedef long difference_type;
    typedef const Pseudo_Row<U>* pointer;
    typedef const Pseudo_Row<U>& reference;

    //! Constructor to build past-the-end objects.
    any_row_iterator(dimension_type n_rows);

    /*! \brief
      Builds an iterator pointing at the beginning of an OR_Matrix whose
      first element is \p base;
    */
    explicit any_row_iterator(U& base);

    /*! \brief
      Copy constructor allowing the construction of a const_iterator
      from a non-const iterator.
    */
    template <typename V>
    any_row_iterator(const any_row_iterator<V>& y);

    /*! \brief
      Assignment operator allowing the assignment of a non-const iterator
      to a const_iterator.
    */
    template <typename V>
    any_row_iterator& operator=(const any_row_iterator<V>& y);

    //! Dereference operator.
    reference operator*() const;

    //! Indirect member selector.
    pointer operator->() const;

    //! Prefix increment operator.
    any_row_iterator& operator++();

    //! Postfix increment operator.
    any_row_iterator operator++(int);

    //! Prefix decrement operator.
    any_row_iterator& operator--();

    //! Postfix decrement operator.
    any_row_iterator operator--(int);

    //! Subscript operator.
    reference operator[](difference_type m) const;

    //! Assignment-increment operator.
    any_row_iterator& operator+=(difference_type m);

    //! Assignment-increment operator for \p m of unsigned type.
    template <typename Unsigned>
    typename Enable_If<(static_cast<Unsigned>(-1) > 0), any_row_iterator&>::type
    operator+=(Unsigned m);

    //! Assignment-decrement operator.
    any_row_iterator& operator-=(difference_type m);

    //! Returns the difference between \p *this and \p y.
    difference_type operator-(const any_row_iterator& y) const;

    //! Returns the sum of \p *this and \p m.
    any_row_iterator operator+(difference_type m) const;

    //! Returns the sum of \p *this and \p m, for \p m of unsigned type.
    template <typename Unsigned>
    typename Enable_If<(static_cast<Unsigned>(-1) > 0), any_row_iterator>::type
    operator+(Unsigned m) const;

    //! Returns the difference of \p *this and \p m.
    any_row_iterator operator-(difference_type m) const;

    //! Returns <CODE>true</CODE> if and only if \p *this is equal to \p y.
    bool operator==(const any_row_iterator& y) const;

    /*! \brief
      Returns <CODE>true</CODE> if and only if \p *this
      is different from \p y.
    */
    bool operator!=(const any_row_iterator& y) const;

    //! Returns <CODE>true</CODE> if and only if \p *this is less than \p y.
    bool operator<(const any_row_iterator& y) const;

    /*! \brief
      Returns <CODE>true</CODE> if and only if \p *this is less than
      or equal to \p y.
    */
    bool operator<=(const any_row_iterator& y) const;

    //! Returns <CODE>true</CODE> if and only if \p *this is greater than \p y.
    bool operator>(const any_row_iterator& y) const;

    /*! \brief
      Returns <CODE>true</CODE> if and only if \p *this is greater than
      or equal to \p y.
    */
    bool operator>=(const any_row_iterator& y) const;

    dimension_type row_size() const;

    dimension_type index() const;

  private:
    //! Represents the beginning of a row.
    Pseudo_Row<U> value;

    //! External index.
    dimension_type e;

    //! Internal index: <CODE>i = (e+1)*(e+1)/2</CODE>.
    dimension_type i;

    // FIXME: the EDG-based compilers (such as Comeau and Intel)
    // are here in wild disagreement with GCC: what is a legal friend
    // declaration for one, is illegal for the others.
#ifdef __EDG__
    template <typename V> template<typename W>
    friend class OR_Matrix<V>::any_row_iterator;
#else
    template <typename V> friend class any_row_iterator;
#endif
  }; // class any_row_iterator

public:
  //! A (non const) row iterator.
  typedef any_row_iterator<T> row_iterator;

  //! A const row iterator.
  typedef any_row_iterator<const T> const_row_iterator;

  //! A (non const) element iterator.
  typedef typename DB_Row<T>::iterator element_iterator;

  //! A const element iterator.
  typedef typename DB_Row<T>::const_iterator const_element_iterator;

public:
  //! Returns the maximum number of rows of a OR_Matrix.
  static dimension_type max_num_rows();

  //! Builds a matrix with specified dimensions.
  /*!
    \param num_dimensions
    The space dimension of the matrix that will be created.

    This constructor creates a matrix with \p 2*num_dimensions rows.
    Each element is initialized to plus infinity.
  */
  OR_Matrix(dimension_type num_dimensions);

  //! Copy constructor.
  OR_Matrix(const OR_Matrix& y);

  //! Constructs a conservative approximation of \p y.
  template <typename U>
  explicit OR_Matrix(const OR_Matrix<U>& y);

  //! Destructor.
  ~OR_Matrix();

  //! Assignment operator.
  OR_Matrix& operator=(const OR_Matrix& y);

private:
  template <typename U> friend class OR_Matrix;

  //! Contains the rows of the matrix.
  /*!
    A DB_Row which contains the rows of the OR_Matrix
    inserting each successive row to the end of the vec.
    To contain all the elements of OR_Matrix the size of the DB_Row
    is 2*n*(n+1), where the n is the characteristic parameter of
    OR_Matrix.
  */
  DB_Row<T> vec;

  //! Contains the dimension of the space of the matrix.
  dimension_type space_dim;

  //! Contains the capacity of \p vec.
  dimension_type vec_capacity;

  //! Private and not implemented: default construction is not allowed.
  OR_Matrix();

  /*! \brief
    Returns the index into <CODE>vec</CODE> of the first element
    of the row of index \p k.
  */
  static dimension_type row_first_element_index(dimension_type k);

public:
  //! Returns the size of the row of index \p k.
  static dimension_type row_size(dimension_type k);

  //! Swaps \p *this with \p y.
  void m_swap(OR_Matrix& y);

  //! Makes the matrix grow by adding more space dimensions.
  /*!
    \param new_dim
    The new dimension of the resized matrix.

    Adds new rows of right dimension to the end if
    there is enough capacity; otherwise, creates a new matrix,
    with the specified dimension, copying the old elements
    in the upper part of the new matrix, which is
    then assigned to \p *this.
    Each new element is initialized to plus infinity.
  */
  void grow(dimension_type new_dim);

  //! Makes the matrix shrink by removing the last space dimensions.
  /*!
    \param new_dim
    The new dimension of the resized matrix.

    Erases from matrix to the end the rows with index
    greater than 2*new_dim-1.
  */
  void shrink(dimension_type new_dim);

  //! Resizes the matrix without worrying about the old contents.
  /*!
    \param new_dim
    The new dimension of the resized matrix.

    If the new dimension is greater than the old one, it adds new rows
    of right dimension to the end if there is enough capacity; otherwise,
    it creates a new matrix, with the specified dimension, which is
    then assigned to \p *this.
    If the new dimension is less than the old one, it erase from the matrix
    the rows having index greater than 2*new_dim-1
  */
  void resize_no_copy(dimension_type new_dim);

  //! Returns the space-dimension of the matrix.
  dimension_type space_dimension() const;

  //! Returns the number of rows in the matrix.
  dimension_type num_rows() const;

  //! \name Subscript operators.
  //@{
  //! Returns a reference to the \p k-th row of the matrix.
  row_reference_type operator[](dimension_type k);

  //! Returns a constant reference to the \p k-th row of the matrix.
  const_row_reference_type operator[](dimension_type k) const;
  //@}


  /*! \brief
    Returns an iterator pointing to the first row,
    if \p *this is not empty;
    otherwise, returns the past-the-end const_iterator.
  */
  row_iterator row_begin();

  //! Returns the past-the-end const_iterator.
  row_iterator row_end();

  /*! \brief
    Returns a const row iterator pointing to the first row,
    if \p *this is not empty;
    otherwise, returns the past-the-end const_iterator.
  */
  const_row_iterator row_begin() const;

  //! Returns the past-the-end const row iterator.
  const_row_iterator row_end() const;

  /*! \brief
    Returns an iterator pointing to the first element,
    if \p *this is not empty;
    otherwise, returns the past-the-end const_iterator.
  */
  element_iterator element_begin();

  //! Returns the past-the-end const_iterator.
  element_iterator element_end();

  /*! \brief
    Returns a const element iterator pointing to the first element,
    if \p *this is not empty;
    otherwise, returns the past-the-end const_iterator.
  */
  const_element_iterator element_begin() const;

  //! Returns the past-the-end const element iterator.
  const_element_iterator element_end() const;

  //! Clears the matrix deallocating all its rows.
  void clear();

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

  friend bool operator==<T>(const OR_Matrix<T>& x, const OR_Matrix<T>& y);

  //! Checks if all the invariants are satisfied.
  bool OK() const;
};

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
void swap(OR_Matrix<T>& x, OR_Matrix<T>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
/*! \relates OR_Matrix */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
bool operator!=(const OR_Matrix<T>& x, const OR_Matrix<T>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Computes the rectilinear (or Manhattan) distance between \p x and \p y.
/*! \relates OR_Matrix
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
				 const OR_Matrix<T>& x,
				 const OR_Matrix<T>& y,
				 Rounding_Dir dir,
				 Temp& tmp0,
				 Temp& tmp1,
				 Temp& tmp2);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Computes the euclidean distance between \p x and \p y.
/*! \relates OR_Matrix
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
			       const OR_Matrix<T>& x,
			       const OR_Matrix<T>& y,
			       Rounding_Dir dir,
			       Temp& tmp0,
			       Temp& tmp1,
			       Temp& tmp2);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Computes the \f$L_\infty\f$ distance between \p x and \p y.
/*! \relates OR_Matrix
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
				 const OR_Matrix<T>& x,
				 const OR_Matrix<T>& y,
				 Rounding_Dir dir,
				 Temp& tmp0,
				 Temp& tmp1,
				 Temp& tmp2);

} // namespace Parma_Polyhedra_Library

#include "OR_Matrix.inlines.hh"
#include "OR_Matrix.templates.hh"

#endif // !defined(PPL_OR_Matrix_defs_hh)
