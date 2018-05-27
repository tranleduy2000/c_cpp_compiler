/* iterator_to_const and const_iterator_to_const class declarations.
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

#ifndef PPL_iterator_to_const_hh
#define PPL_iterator_to_const_hh 1

#include "iterator_to_const.types.hh"
#include "Powerset.types.hh"
//#include "Ask_Tell.types.hh"
#include <iterator>

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! An iterator on a sequence of read-only objects.
/*! \ingroup PPL_CXX_interface
  This template class implements a bidirectional <EM>read-only</EM>
  iterator on the sequence of objects <CODE>Container</CODE>.
  By using this iterator class it is not possible to modify the objects
  contained in <CODE>Container</CODE>; rather, object modification has
  to be implemented by object replacement, i.e., by using the methods
  provided by <CODE>Container</CODE> to remove/insert objects.
  Such a policy (a modifiable container of read-only objects) allows
  for a reliable enforcement of invariants (such as sortedness of the
  objects in the sequence).

  \note
  For any developers' need, suitable friend declarations allow for
  accessing the low-level iterators on the sequence of objects.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Container>
class Parma_Polyhedra_Library::iterator_to_const {
private:
  //! The type of the underlying mutable iterator.
  typedef typename Container::iterator Base;

  //! A shortcut for naming the const_iterator traits.
  typedef typename
  std::iterator_traits<typename Container::const_iterator> Traits;

  //! A (mutable) iterator on the sequence of elements.
  Base base;

  //! Constructs from the lower-level iterator.
  iterator_to_const(const Base& b);

  friend class const_iterator_to_const<Container>;
  template <typename T> friend class Powerset;

public:
  // Same traits of the const_iterator, therefore
  // forbidding the direct modification of sequence elements.
  typedef typename Traits::iterator_category iterator_category;
  typedef typename Traits::value_type value_type;
  typedef typename Traits::difference_type difference_type;
  typedef typename Traits::pointer pointer;
  typedef typename Traits::reference reference;

  //! Default constructor.
  iterator_to_const();

  //! Copy constructor.
  iterator_to_const(const iterator_to_const& y);

  //! Dereference operator.
  reference operator*() const;

  //! Indirect access operator.
  pointer operator->() const;

  //! Prefix increment operator.
  iterator_to_const& operator++();

  //! Postfix increment operator.
  iterator_to_const operator++(int);

  //! Prefix decrement operator.
  iterator_to_const& operator--();

  //! Postfix decrement operator.
  iterator_to_const operator--(int);

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    \p *this and \p y are identical.
  */
  bool operator==(const iterator_to_const& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    \p *this and \p y are different.
  */
  bool operator!=(const iterator_to_const& y) const;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A %const_iterator on a sequence of read-only objects.
/*! \ingroup PPL_CXX_interface
  This class, besides implementing a read-only bidirectional iterator
  on a read-only sequence of objects, ensures interoperability
  with template class iterator_to_const.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Container>
class Parma_Polyhedra_Library::const_iterator_to_const {
private:
  //! The type of the underlying %const_iterator.
  typedef typename Container::const_iterator Base;

  //! A shortcut for naming traits.
  typedef typename std::iterator_traits<Base> Traits;

  //! A %const_iterator on the sequence of elements.
  Base base;

  //! Constructs from the lower-level const_iterator.
  const_iterator_to_const(const Base& b);

  friend class iterator_to_const<Container>;
  template <typename T> friend class Powerset;

public:
  // Same traits of the underlying const_iterator.
  typedef typename Traits::iterator_category iterator_category;
  typedef typename Traits::value_type value_type;
  typedef typename Traits::difference_type difference_type;
  typedef typename Traits::pointer pointer;
  typedef typename Traits::reference reference;

  //! Default constructor.
  const_iterator_to_const();

  //! Copy constructor.
  const_iterator_to_const(const const_iterator_to_const& y);

  //! Constructs from the corresponding non-const iterator.
  const_iterator_to_const(const iterator_to_const<Container>& y);

  //! Dereference operator.
  reference operator*() const;

  //! Indirect member selector.
  pointer operator->() const;

  //! Prefix increment operator.
  const_iterator_to_const& operator++();

  //! Postfix increment operator.
  const_iterator_to_const operator++(int);

  //! Prefix decrement operator.
  const_iterator_to_const& operator--();

  //! Postfix decrement operator.
  const_iterator_to_const operator--(int);

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    \p *this and \p y are identical.
  */
  bool operator==(const const_iterator_to_const& y) const;

  /*! \brief
    Returns <CODE>true</CODE> if and only if
    \p *this and \p y are different.
  */
  bool operator!=(const const_iterator_to_const& y) const;
};

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Mixed comparison operator: returns <CODE>true</CODE> if and only
  if (the const version of) \p x is identical to \p y.

  \relates const_iterator_to_const
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Container>
bool
operator==(const iterator_to_const<Container>& x,
	   const const_iterator_to_const<Container>& y);

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \brief
  Mixed comparison operator: returns <CODE>true</CODE> if and only
  if (the const version of) \p x is different from \p y.

  \relates const_iterator_to_const
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Container>
bool
operator!=(const iterator_to_const<Container>& x,
	   const const_iterator_to_const<Container>& y);

} // namespace Parma_Polyhedra_Library

#include "iterator_to_const.inlines.hh"

#endif // !defined(PPL_iterator_to_const_hh)
