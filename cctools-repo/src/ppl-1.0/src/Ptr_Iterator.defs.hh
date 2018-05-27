/* Ptr_Iterator class declaration.
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

#ifndef PPL_Ptr_Iterator_defs_hh
#define PPL_Ptr_Iterator_defs_hh 1

#include "Ptr_Iterator.types.hh"
#include <iterator>

namespace Parma_Polyhedra_Library {

namespace Implementation {

template<typename P, typename Q>
bool operator==(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y);

template<typename P, typename Q>
bool operator!=(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y);

template<typename P, typename Q>
bool operator<(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y);

template<typename P, typename Q>
bool operator<=(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y);

template<typename P, typename Q>
bool operator>(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y);

template<typename P, typename Q>
bool operator>=(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y);

template<typename P, typename Q>
typename Ptr_Iterator<P>::difference_type
operator-(const Ptr_Iterator<P>& x, const Ptr_Iterator<Q>& y);

template<typename P>
Ptr_Iterator<P> operator+(typename Ptr_Iterator<P>::difference_type m,
			  const Ptr_Iterator<P>& y);

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A class to define STL const and non-const iterators from pointer types.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename P>
class Parma_Polyhedra_Library::Implementation::Ptr_Iterator
  : public std::iterator<typename std::iterator_traits<P>::iterator_category,
			 typename std::iterator_traits<P>::value_type,
			 typename std::iterator_traits<P>::difference_type,
			 typename std::iterator_traits<P>::pointer,
			 typename std::iterator_traits<P>::reference> {
public:
  typedef typename std::iterator_traits<P>::difference_type difference_type;
  typedef typename std::iterator_traits<P>::reference reference;
  typedef typename std::iterator_traits<P>::pointer pointer;

  //! Default constructor: no guarantees.
  Ptr_Iterator();

  //! Construct an iterator pointing at \p q.
  explicit Ptr_Iterator(const P& q);

  /*! \brief
    Copy constructor allowing the construction of a const_iterator
    from a non-const iterator.
  */
  template<typename Q>
  Ptr_Iterator(const Ptr_Iterator<Q>& q);

  //! Dereference operator.
  reference operator*() const;

  //! Indirect member selector.
  pointer operator->() const;

  //! Subscript operator.
  reference operator[](const difference_type m) const;

  //! Prefix increment operator.
  Ptr_Iterator& operator++();

  //! Postfix increment operator.
  Ptr_Iterator operator++(int);

  //! Prefix decrement operator
  Ptr_Iterator& operator--();

  //! Postfix decrement operator.
  Ptr_Iterator operator--(int);

  //! Assignment-increment operator.
  Ptr_Iterator& operator+=(const difference_type m);

  //! Assignment-decrement operator.
  Ptr_Iterator& operator-=(const difference_type m);

  //! Returns the difference between \p *this and \p y.
  difference_type operator-(const Ptr_Iterator& y) const;

  //! Returns the sum of \p *this and \p m.
  Ptr_Iterator operator+(const difference_type m) const;

  //! Returns the difference of \p *this and \p m.
  Ptr_Iterator operator-(const difference_type m) const;

private:
  //! The base pointer implementing the iterator.
  P p;

  //! Returns the hidden pointer.
  const P& base() const;

  template <typename Q, typename R>
  friend bool Parma_Polyhedra_Library::Implementation::
  operator==(const Ptr_Iterator<Q>& x, const Ptr_Iterator<R>& y);

  template <typename Q, typename R>
  friend bool Parma_Polyhedra_Library::Implementation::
  operator!=(const Ptr_Iterator<Q>& x, const Ptr_Iterator<R>& y);

  template<typename Q, typename R>
  friend bool Parma_Polyhedra_Library::Implementation::
  operator<(const Ptr_Iterator<Q>& x, const Ptr_Iterator<R>& y);

  template<typename Q, typename R>
  friend bool Parma_Polyhedra_Library::Implementation::
  operator<=(const Ptr_Iterator<Q>& x, const Ptr_Iterator<R>& y);

  template<typename Q, typename R>
  friend bool Parma_Polyhedra_Library::Implementation::
  operator>(const Ptr_Iterator<Q>& x, const Ptr_Iterator<R>& y);

  template<typename Q, typename R>
  friend bool Parma_Polyhedra_Library::Implementation::
  operator>=(const Ptr_Iterator<Q>& x, const Ptr_Iterator<R>& y);

  template<typename Q, typename R>
  friend typename Ptr_Iterator<Q>::difference_type
  Parma_Polyhedra_Library::Implementation::
  operator-(const Ptr_Iterator<Q>& x, const Ptr_Iterator<R>& y);

  friend Ptr_Iterator<P>
  Parma_Polyhedra_Library::Implementation::
  operator+<>(typename Ptr_Iterator<P>::difference_type m,
	      const Ptr_Iterator<P>& y);
};

#include "Ptr_Iterator.inlines.hh"

#endif // !defined(PPL_Ptr_Iterator_defs_hh)
