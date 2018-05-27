/* Sparse_Row class declaration.
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

#ifndef PPL_Sparse_Row_defs_hh
#define PPL_Sparse_Row_defs_hh 1

#include "Sparse_Row_types.hh"

#include "CO_Tree_defs.hh"
#include "Coefficient_defs.hh"
#include "Dense_Row_types.hh"

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A finite sparse sequence of coefficients.
/*! \ingroup PPL_CXX_interface
  This class is implemented using a CO_Tree. See the documentation of CO_Tree
  for details on the implementation and the performance.

  This class is a drop-in replacement of Dense_Row, meaning that code
  using Dense_Row can be ported to Sparse_Row changing only the type.
  The resulting code will work, but probably needs more CPU and memory (it
  does not exploit the sparse representation yet).

  To take advantage of the sparse representation, the client code must then be
  modified to use methods which can have a faster implementation on sparse
  data structures.

  The main changes are the replacement of calls to operator[] with calls to
  find(), lower_bound() or insert(), using hint iterators when possible.
  Sequential scanning of rows should probably be implemented using iterators
  rather than indexes, to improve performance.
  reset() should be called to zero elements.

  \see Sparse_Matrix
  \see CO_Tree
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
class Parma_Polyhedra_Library::Sparse_Row {

public:

  //! An %iterator on the row elements
  /*!
    This %iterator skips non-stored zeroes.
    \see CO_Tree::iterator
  */
  typedef CO_Tree::iterator iterator;

  //! A const %iterator on the row elements
  /*!
    This %iterator skips non-stored zeroes.
    \see CO_Tree::const_iterator
  */
  typedef CO_Tree::const_iterator const_iterator;

  //! Constructs a row with the specified size.
  /*!
    \param n
    The size for the new row.

    The row will contain only non-stored zeroes.

    This constructor takes \f$O(1)\f$ time.
  */
  explicit Sparse_Row(dimension_type n = 0);

  //! Constructs a row with the specified size.
  /*!
    \param n
    The size for the new row.

    \param capacity
    It is ignored. This parameter is needed for compatibility with Dense_Row.

    The row will contain only non-stored zeroes.

    This constructor takes \f$O(1)\f$ time.
  */
  Sparse_Row(dimension_type n, dimension_type capacity);

  //! Copy constructor with specified capacity.
  /*!
    It is assumed that \p capacity is greater than or equal to
    the size of \p y.
  */
  Sparse_Row(const Sparse_Row& y, dimension_type capacity);

  //! Copy constructor with specified size and capacity.
  /*!
    It is assumed that \p sz is less than or equal to \p capacity.
  */
  Sparse_Row(const Sparse_Row& y, dimension_type sz, dimension_type capacity);

  //! Constructor from a Dense_Row.
  /*!
    \param row
    The row that will be copied into *this.

    This constructor takes \f$O(n)\f$ time. Note that constructing of a row of
    zeroes and then inserting n elements costs \f$O(n*\log^2 n)\f$ time.
  */
  explicit Sparse_Row(const Dense_Row& row);

  //! Copy constructor from a Dense_Row with specified size and capacity.
  /*!
    It is assumed that \p sz is less than or equal to \p capacity.
  */
  Sparse_Row(const Dense_Row& y, dimension_type sz, dimension_type capacity);

  Sparse_Row& operator=(const Dense_Row& row);

  //! Swaps *this and x.
  /*!
    \param x
    The row that will be swapped with *this.

    This method takes \f$O(1)\f$ time.
  */
  void m_swap(Sparse_Row& x);

  //! Returns the size of the row.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  dimension_type size() const;

  //! Returns the number of elements explicitly stored in the row.
  /*!
    This is equivalent to std::distance(begin(), end()), but it's much faster.

    This method takes \f$O(1)\f$ time.
  */
  dimension_type num_stored_elements() const;

  //! Resizes the row to the specified size.
  /*!
    \param n
    The new size for the row.

    This method takes \f$O(k*\log^2 n)\f$ amortized time when shrinking the
    row and removing the trailing k elements.
    It takes \f$O(1)\f$ time when enlarging the row.
  */
  void resize(dimension_type n);

  //! Resizes the row to size \p n.
  /*!
    \param n
    The new size for the row.

    This method, with this signature, is needed for compatibility with
    Dense_Row.

    This method takes \f$O(1)\f$ time.
  */
  void expand_within_capacity(dimension_type n);

  //! Resizes the row to size \p n.
  /*!
    \param n
    The new size for the row.

    This method, with this signature, is needed for compatibility with
    Dense_Row.

    This method takes \f$O(k*\log^2 n)\f$ amortized time where k is the number
    of removed elements.
  */
  void shrink(dimension_type n);

  /*!
    \brief Deletes the i-th element from the row, shifting the next elements
           to the left.

    \param i
    The index of the element that will be deleted.

    The size of the row is decreased by 1.

    This operation invalidates existing iterators.

    This method takes \f$O(k+\log^2 n)\f$ amortized time, where k is the
    number of elements with index greater than i.
  */
  void delete_element_and_shift(dimension_type i);

  //! Adds \p n zeroes before index \p i.
  /*!
    \param n
    The number of non-stored zeroes that will be added to the row.

    \param i
    The index of the element before which the zeroes will be added.

    Existing elements with index greater than or equal to \p i are shifted to
    the right by \p n positions. The size is increased by \p n.

    Existing iterators are not invalidated, but are shifted to the right
    by \p n if they pointed at or after index \p i (i.e., they point to
    the same, possibly shifted, values as before).

    This method takes \f$O(k + \log m)\f$ expected time, where \f$k\f$ is
    the number of elements with index greater than or equal to \p i and
    \f$m\f$ the number of stored elements.
  */
  void add_zeroes_and_shift(dimension_type n, dimension_type i);

  //! Returns an %iterator that points at the first stored element.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  iterator begin();

  //! Returns an %iterator that points after the last stored element.
  /*!
    This method always returns a reference to the same internal %iterator,
    that is kept valid.
    Client code can keep a const reference to that %iterator instead of
    keep updating a local %iterator.

    This method takes \f$O(1)\f$ time.
  */
  const iterator& end();

  //! Equivalent to <CODE>cbegin()</CODE>.
  const_iterator begin() const;

  //! Equivalent to <CODE>cend()</CODE>.
  const const_iterator& end() const;

  //! Returns an %iterator that points at the first element.
  /*!
    This method takes \f$O(1)\f$ time.
  */
  const_iterator cbegin() const;

  //! Returns an %iterator that points after the last element.
  /*!
    This method always returns a reference to the same internal %iterator,
    that is updated at each operation that modifies the structure.
    Client code can keep a const reference to that %iterator instead of
    keep updating a local %iterator.

    This method takes \f$O(1)\f$ time.
  */
  const const_iterator& cend() const;

  //! Returns the size() of the largest possible Sparse_Row.
  static dimension_type max_size();

  //! Resets all the elements of this row.
  /*!
    This method takes \f$O(n)\f$ time.
  */
  void clear();

  //! Gets a reference to the i-th element.
  /*!
    \param i
    The index of the desired element.

    For read-only access it's better to use get(), that avoids allocating
    space for zeroes.

    If possible, use the insert(), find() or lower_bound() methods with
    a hint instead of this, to improve performance.

    This operation invalidates existing iterators.

    This method takes \f$O(\log n)\f$ amortized time when there is already an
    element with index \p i, and \f$O(\log^2 n)\f$ otherwise.
  */
  Coefficient& operator[](dimension_type i);

  //! Equivalent to <CODE>get(i)</CODE>, provided for convenience.
  /*!
    This method takes \f$O(\log n)\f$ time.
  */
  Coefficient_traits::const_reference operator[](dimension_type i) const;

  //! Gets the i-th element in the sequence.
  /*!
    \param i
    The index of the desired element.

    If possible, use the insert(), find() or lower_bound() methods with
    a hint instead of this, to improve performance.

    This method takes \f$O(\log n)\f$ time.
  */
  Coefficient_traits::const_reference get(dimension_type i) const;

  //! Looks for an element with index i.
  /*!
    \param i
    The index of the desired element.

    If possible, use the find() method that takes a hint %iterator, to improve
    performance.

    This method takes \f$O(\log n)\f$ time.
  */
  iterator find(dimension_type i);

  //! Looks for an element with index i.
  /*!
    \param i
    The index of the desired element.

    \param itr
    It is used as a hint. This method will be faster if the searched element
    is near to \p itr.

    The value of \p itr does not affect the result of this method, as long it
    is a valid %iterator for this row. \p itr may even be end().

    This method takes \f$O(\log n)\f$ time.
    If the distance between \p itr and the searched position is \f$O(1)\f$,
    this method takes \f$O(1)\f$ time.
  */
  iterator find(iterator itr, dimension_type i);

  //! Looks for an element with index i.
  /*!
    \param i
    The index of the desired element.

    If possible, use the find() method that takes a hint %iterator, to improve
    performance.

    This method takes \f$O(\log n)\f$ time.
  */
  const_iterator find(dimension_type i) const;

  //! Looks for an element with index i.
  /*!
    \param i
    The index of the desired element.

    \param itr
    It is used as a hint. This method will be faster if the searched element
    is near to \p itr.

    The value of \p itr does not affect the result of this method, as long it
    is a valid %iterator for this row. \p itr may even be end().

    This method takes \f$O(\log n)\f$ time.
    If the distance between \p itr and the searched position is \f$O(1)\f$,
    this method takes \f$O(1)\f$ time.
  */
  const_iterator find(const_iterator itr, dimension_type i) const;

  //! Lower bound of index i.
  /*!
    \param i
    The index of the desired element.

    \returns an %iterator to the first element with index greater than or
             equal to i.
             If there are no such elements, returns end().

    If possible, use the find() method that takes a hint %iterator, to improve
    performance.

    This method takes \f$O(\log n)\f$ time.
  */
  iterator lower_bound(dimension_type i);

  //! Lower bound of index i.
  /*!
    \param i
    The index of the desired element.

    \param itr
    It is used as a hint. This method will be faster if the searched element
    is near to \p itr.

    \returns an %iterator to the first element with index greater than or
             equal to i.
             If there are no such elements, returns end().

    The value of \p itr does not affect the result of this method, as long it
    is a valid %iterator for this row. \p itr may even be end().

    This method takes \f$O(\log n)\f$ time.
    If the distance between \p itr and the searched position is \f$O(1)\f$,
    this method takes \f$O(1)\f$ time.
  */
  iterator lower_bound(iterator itr, dimension_type i);

  //! Lower bound of index i.
  /*!

    \param i
    The index of the desired element.

    \returns an %iterator to the first element with index greater than or
             equal to i.
             If there are no such elements, returns end().

    If possible, use the find() method that takes a hint %iterator, to improve
    performance.

    This method takes \f$O(\log n)\f$ time.
  */
  const_iterator lower_bound(dimension_type i) const;

  //! Lower bound of index i.
  /*!
    \param i
    The index of the desired element.

    \param itr
    It is used as a hint. This method will be faster if the searched element
    is near to \p itr.

    \returns an %iterator to the first element with index greater than or
             equal to i.
             If there are no such elements, returns end().

    The value of \p itr does not affect the result of this method, as long it
    is a valid %iterator for this row. \p itr may even be end().

    This method takes \f$O(\log n)\f$ time.
    If the distance between \p itr and the searched position is \f$O(1)\f$,
    this method takes \f$O(1)\f$ time.
  */
  const_iterator lower_bound(const_iterator itr, dimension_type i) const;

  //! Equivalent to <CODE>(*this)[i] = x; find(i)</CODE>, but faster.
  /*!
    \param i
    The index of the desired element.

    \param x
    The value that will be associated to the element.

    If possible, use versions of this method that take a hint, to improve
    performance.

    This operation invalidates existing iterators.

    This method takes \f$O(\log^2 n)\f$ amortized time.
  */
  iterator insert(dimension_type i, Coefficient_traits::const_reference x);

  //! Equivalent to <CODE>(*this)[i] = x; find(i)</CODE>, but faster.
  /*!
    \param i
    The index of the desired element.

    \param x
    The value that will be associated to the element.

    \param itr
    It is used as a hint. This method will be faster if the searched element
    is near to \p itr, even faster than <CODE>(*this)[i] = x</CODE>.

    The value of \p itr does not affect the result of this method, as long it
    is a valid %iterator for this row. \p itr may even be end().

    This operation invalidates existing iterators.

    This method takes \f$O(\log^2 n)\f$ amortized time. If the distance
    between \p itr and the searched position is \f$O(1)\f$ and the row already
    contains an element with this index, this method takes \f$O(1)\f$ time.
  */
  iterator insert(iterator itr, dimension_type i,
                  Coefficient_traits::const_reference x);

  //! Equivalent to <CODE>(*this)[i]; find(i)</CODE>, but faster.
  /*!
    \param i
    The index of the desired element.

    If possible, use versions of this method that take a hint, to improve
    performance.

    This operation invalidates existing iterators.

    This method takes \f$O(\log^2 n)\f$ amortized time.
  */
  iterator insert(dimension_type i);

  //! Equivalent to <CODE>(*this)[i]; find(i)</CODE>, but faster.
  /*!
    \param i
    The index of the desired element.

    \param itr
    It is used as a hint. This method will be faster if the searched element
    is near to \p itr, even faster than <CODE>(*this)[i]</CODE>.

    The value of \p itr does not affect the result of this method, as long it
    is a valid %iterator for this row. \p itr may even be end().

    This operation invalidates existing iterators.

    This method takes \f$O(\log^2 n)\f$ amortized time. If the distance
    between \p itr and the searched position is \f$O(1)\f$ and the row already
    contains an element with this index, this method takes \f$O(1)\f$ time.
  */
  iterator insert(iterator itr, dimension_type i);

  //! Swaps the i-th element with the j-th element.
  /*!
    \param i
    The index of an element.

    \param j
    The index of another element.

    This operation invalidates existing iterators.

    This method takes \f$O(\log^2 n)\f$ amortized time.
  */
  void swap_coefficients(dimension_type i, dimension_type j);

  //! Equivalent to swap(i,itr.index()), but it assumes that
  //! lower_bound(i)==itr.
  /*!
    Iterators that pointed to the itr.index()-th element remain valid
    but now point to the i-th element. Other iterators are unaffected.

    This method takes \f$O(1)\f$ time.
  */
  void fast_swap(dimension_type i, iterator itr);

  //! Swaps the element pointed to by i with the element pointed to by j.
  /*!
    \param i
    An %iterator pointing to an element.

    \param j
    An %iterator pointing to another element.

    This method takes \f$O(1)\f$ time.
  */
  void swap_coefficients(iterator i, iterator j);

  //! Resets to zero the value pointed to by i.
  /*!
    \param i
    An %iterator pointing to the element that will be reset (not stored
    anymore).

    By calling this method instead of getting a reference to the value and
    setting it to zero, the element will no longer be stored.

    This operation invalidates existing iterators.

    This method takes \f$O(\log^2 n)\f$ amortized time.
  */
  iterator reset(iterator i);

  //! Resets to zero the values in the range [first,last).
  /*!
    \param first
    An %iterator pointing to the first element to reset.

    \param last
    An %iterator pointing after the last element to reset.

    By calling this method instead of getting a reference to the values and
    setting them to zero, the elements will no longer be stored.

    This operation invalidates existing iterators.

    This method takes \f$O(k*\log^2 n)\f$ amortized time, where k is the
    number of elements in [first,last).
  */
  iterator reset(iterator first, iterator last);

  //! Resets to zero the i-th element.
  /*!
    \param i
    The index of the element to reset.

    By calling this method instead of getting a reference to the value and
    setting it to zero, the element will no longer be stored.

    This operation invalidates existing iterators.

    This method takes \f$O(\log^2 n)\f$ amortized time.
  */
  void reset(dimension_type i);

  //! Resets to zero the elements with index greater than or equal to i.
  /*!
    \param i
    The index of the first element to reset.

    By calling this method instead of getting a reference to the values and
    setting them to zero, the elements will no longer be stored.

    This operation invalidates existing iterators.

    This method takes \f$O(k*\log^2 n)\f$ amortized time, where k is the
    number of elements with index greater than or equal to i.
  */
  void reset_after(dimension_type i);

  //! Normalizes the modulo of coefficients so that they are mutually prime.
  /*!
    Computes the Greatest Common Divisor (GCD) among the elements of the row
    and normalizes them by the GCD itself.

    This method takes \f$O(n)\f$ time.
  */
  void normalize();

  //! Calls g(x[i],y[i]), for each i.
  /*!
    \param y
    The row that will be combined with *this.

    \param f
    A functor that should take a Coefficient&.
    f(c1) must be equivalent to g(c1, 0).

    \param g
    A functor that should take a Coefficient& and a
    Coefficient_traits::const_reference.
    g(c1, c2) must do nothing when c1 is zero.

    This method takes \f$O(n*\log^2 n)\f$ time.

    \note
    The functors will only be called when necessary, assuming the requested
    properties hold.

    \see combine_needs_second
    \see combine
  */
  template <typename Func1, typename Func2>
  void combine_needs_first(const Sparse_Row& y,
                           const Func1& f, const Func2& g);

  //! Calls g(x[i],y[i]), for each i.
  /*!
    \param y
    The row that will be combined with *this.

    \param g
    A functor that should take a Coefficient& and a
    Coefficient_traits::const_reference.
    g(c1, 0) must do nothing, for every c1.

    \param h
    A functor that should take a Coefficient& and a
    Coefficient_traits::const_reference.
    h(c1, c2) must be equivalent to g(c1, c2) when c1 is zero.

    This method takes \f$O(n*\log^2 n)\f$ time.

    \note
    The functors will only be called when necessary, assuming the requested
    properties hold.

    \see combine_needs_first
    \see combine
  */
  template <typename Func1, typename Func2>
  void combine_needs_second(const Sparse_Row& y,
                            const Func1& g, const Func2& h);

  //! Calls g(x[i],y[i]), for each i.
  /*!
    \param y
    The row that will be combined with *this.

    \param f
    A functor that should take a Coefficient&.
    f(c1) must be equivalent to g(c1, 0).

    \param g
    A functor that should take a Coefficient& and a
    Coefficient_traits::const_reference.
    g(c1, c2) must do nothing when both c1 and c2 are zero.

    \param h
    A functor that should take a Coefficient& and a
    Coefficient_traits::const_reference.
    h(c1, c2) must be equivalent to g(c1, c2) when c1 is zero.

    This method takes \f$O(n*\log^2 n)\f$ time.

    \note
    The functors will only be called when necessary, assuming the requested
    properties hold.

    \see combine_needs_first
    \see combine_needs_second
  */
  template <typename Func1, typename Func2, typename Func3>
  void combine(const Sparse_Row& y,
               const Func1& f, const Func2& g, const Func3& h);

  //! Executes <CODE>(*this)[i] = (*this)[i]*coeff1 + y[i]*coeff2</CODE>, for
  //! each i.
  /*!
    \param y
    The row that will be combined with *this.

    \param coeff1
    The coefficient used for elements of *this.
    This must not be 0.

    \param coeff2
    The coefficient used for elements of y.
    This must not be 0.

    This method takes \f$O(n*\log^2 n)\f$ time.

    \note
    The functors will only be called when necessary.
    This method can be implemented in user code, too. It is provided for
    convenience only.

    \see combine_needs_first
    \see combine_needs_second
    \see combine
  */
  void linear_combine(const Sparse_Row& y,
                      Coefficient_traits::const_reference coeff1,
                      Coefficient_traits::const_reference coeff2);

  //! Equivalent to <CODE>(*this)[i] = (*this)[i] * c1 + y[i] * c2</CODE>,
  //! for each i in [start, end).
  /*!
    This method, unlike the other linear_combine() method, detects when
    coeff1==1 and/or coeff2==1 or coeff2==-1 in order to save some work.
  */
  void linear_combine(const Sparse_Row& y,
                      Coefficient_traits::const_reference c1,
                      Coefficient_traits::const_reference c2,
                      dimension_type start, dimension_type end);

  PPL_OUTPUT_DECLARATIONS

  //! Loads the row from an ASCII representation generated using ascii_dump().
  /*!
    \param s
    The stream from which the ASCII representation will be loaded.
  */
  bool ascii_load(std::istream& s);

  //! Returns the size in bytes of the memory managed by \p *this.
  /*!
    This method takes \f$O(n)\f$ time.
  */
  memory_size_type external_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  /*!
    This method is provided for compatibility with Dense_Row.

    This method takes \f$O(n)\f$ time.

    \param capacity
    This parameter is ignored.
  */
  memory_size_type external_memory_in_bytes(dimension_type capacity) const;

  //! Returns the size in bytes of the memory managed by \p *this.
  /*!
    This method takes \f$O(n)\f$ time.
  */
  memory_size_type total_memory_in_bytes() const;

  //! Returns the size in bytes of the memory managed by \p *this.
  /*!
    This method is provided for compatibility with Dense_Row.

    This method takes \f$O(n)\f$ time.

    \param capacity
    This parameter is ignored.
  */
  memory_size_type total_memory_in_bytes(dimension_type capacity) const;

  //! Checks the invariant.
  bool OK() const;

  //! Checks the invariant.
  /*!
    This method is provided for compatibility with Dense_Row.

    \param capacity
    This parameter is ignored.
  */
  bool OK(dimension_type capacity) const;

private:
  //! The tree used to store the elements.
  CO_Tree tree;

  //! The size of the row.
  /*!
    The elements contained in this row have indexes that are less than size_.
  */
  dimension_type size_;
};


namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Swaps \p x with \p y.
/*! \relates Sparse_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void swap(Parma_Polyhedra_Library::Sparse_Row& x,
          Parma_Polyhedra_Library::Sparse_Row& y);

void swap(Parma_Polyhedra_Library::Sparse_Row& x,
          Parma_Polyhedra_Library::Dense_Row& y);

void swap(Parma_Polyhedra_Library::Dense_Row& x,
          Parma_Polyhedra_Library::Sparse_Row& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are equal.
/*! \relates Sparse_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool operator==(const Sparse_Row& x, const Sparse_Row& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
/*! \relates Sparse_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool operator!=(const Sparse_Row& x, const Sparse_Row& y);

bool operator==(const Dense_Row& x, const Sparse_Row& y);
bool operator!=(const Dense_Row& x, const Sparse_Row& y);

bool operator==(const Sparse_Row& x, const Dense_Row& y);
bool operator!=(const Sparse_Row& x, const Dense_Row& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Equivalent to <CODE>x[i] = x[i] * c1 + y[i] * c2</CODE>,
//! for each i in [start, end).
/*! \relates Sparse_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void linear_combine(Sparse_Row& x, const Dense_Row& y,
                    Coefficient_traits::const_reference coeff1,
                    Coefficient_traits::const_reference coeff2);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Equivalent to <CODE>x[i] = x[i] * c1 + y[i] * c2</CODE>,
//! for each i in [start, end).
/*! \relates Sparse_Row
  This function detects when coeff1==1 and/or coeff2==1 or coeff2==-1 in
  order to save some work.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void linear_combine(Sparse_Row& x, const Dense_Row& y,
                    Coefficient_traits::const_reference c1,
                    Coefficient_traits::const_reference c2,
                    dimension_type start, dimension_type end);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Equivalent to <CODE>x[i] = x[i] * c1 + y[i] * c2</CODE>,
//! for each i in [start, end).
/*! \relates Sparse_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void linear_combine(Dense_Row& x, const Sparse_Row& y,
                    Coefficient_traits::const_reference coeff1,
                    Coefficient_traits::const_reference coeff2);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Equivalent to <CODE>x[i] = x[i] * c1 + y[i] * c2</CODE>,
//! for each i in [start, end).
/*! \relates Sparse_Row
  This function detects when coeff1==1 and/or coeff2==1 or coeff2==-1 in
  order to save some work.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void linear_combine(Dense_Row& x, const Sparse_Row& y,
                    Coefficient_traits::const_reference c1,
                    Coefficient_traits::const_reference c2,
                    dimension_type start, dimension_type end);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Equivalent to <CODE>x[i] = x[i] * c1 + y[i] * c2</CODE>,
//! for each i in [start, end).
/*! \relates Sparse_Row */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void linear_combine(Sparse_Row& x, const Sparse_Row& y,
                    Coefficient_traits::const_reference coeff1,
                    Coefficient_traits::const_reference coeff2);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Equivalent to <CODE>x[i] = x[i] * c1 + y[i] * c2</CODE>,
//! for each i in [start, end).
/*! \relates Sparse_Row
  This function detects when coeff1==1 and/or coeff2==1 or coeff2==-1 in
  order to save some work.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void linear_combine(Sparse_Row& x, const Sparse_Row& y,
                    Coefficient_traits::const_reference c1,
                    Coefficient_traits::const_reference c2,
                    dimension_type start, dimension_type end);

} // namespace Parma_Polyhedra_Library

#include "Sparse_Row_inlines.hh"
#include "Sparse_Row_templates.hh"

#endif // !defined(PPL_Sparse_Row_defs_hh)
