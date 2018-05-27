/* EList class declaration.
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

#ifndef PPL_EList_defs_hh
#define PPL_EList_defs_hh 1

#include "EList.types.hh"
#include "Doubly_Linked_Object.defs.hh"
#include "EList_Iterator.defs.hh"

/*! \brief
  A simple kind of embedded list (i.e., a doubly linked objects
  where the links are embedded in the objects themselves).
*/
template <typename T>
class Parma_Polyhedra_Library::Implementation::Watchdog::EList
  : private Doubly_Linked_Object {
public:
  //! A const iterator to traverse the list.
  typedef EList_Iterator<const T> Const_Iterator;

  //! A non-const iterator to traverse the list.
  typedef EList_Iterator<T> Iterator;

  //! Constructs an empty list.
  EList();

  //! Destructs the list and all the elements in it.
  ~EList();

  //! Pushes \p obj to the front of the list.
  void push_front(T& obj);

  //! Pushes \p obj to the back of the list.
  void push_back(T& obj);

  /*! \brief
    Inserts \p obj just before \p position and returns an iterator
    that points to the inserted object.
  */
  Iterator insert(Iterator position, T& obj);

  /*! \brief
    Removes the element pointed to by \p position, returning
    an iterator pointing to the next element, if any, or end(), otherwise.
  */
  Iterator erase(Iterator position);

  //! Returns <CODE>true</CODE> if and only if the list is empty.
  bool empty() const;

  //! Returns an iterator pointing to the beginning of the list.
  Iterator begin();

  //! Returns an iterator pointing one past the last element in the list.
  Iterator end();

  //! Returns a const iterator pointing to the beginning of the list.
  Const_Iterator begin() const;

  //! Returns a const iterator pointing one past the last element in the list.
  Const_Iterator end() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;
};

#include "EList.inlines.hh"

#endif // !defined(PPL_EList_defs_hh)
