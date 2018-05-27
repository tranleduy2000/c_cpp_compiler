/* EList_Iterator class declaration.
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

#ifndef PPL_EList_Iterator_defs_hh
#define PPL_EList_Iterator_defs_hh 1

#include "EList_Iterator.types.hh"
#include "Doubly_Linked_Object.types.hh"

namespace Parma_Polyhedra_Library {

namespace Implementation {

namespace Watchdog {

//! Returns <CODE>true</CODE> if and only if \p x and \p y are equal.
template <typename T>
bool operator==(const EList_Iterator<T>& x, const EList_Iterator<T>& y);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
template <typename T>
bool operator!=(const EList_Iterator<T>& x, const EList_Iterator<T>& y);

} // namespace Watchdog

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

//! A class providing iterators for embedded lists.
template <typename T>
class Parma_Polyhedra_Library::Implementation::Watchdog::EList_Iterator {
public:
  //! Constructs an iterator pointing to nothing.
  EList_Iterator();

  //! Constructs an iterator pointing to \p p.
  explicit EList_Iterator(Doubly_Linked_Object* p);

  //! Changes \p *this so that it points to \p p.
  EList_Iterator& operator=(Doubly_Linked_Object* p);

  //! Indirect member selector.
  T* operator->();

  //! Dereference operator.
  T& operator*();

  //! Preincrement operator.
  EList_Iterator& operator++();

  //! Postincrement operator.
  EList_Iterator operator++(int);

  //! Predecrement operator.
  EList_Iterator& operator--();

  //! Postdecrement operator.
  EList_Iterator operator--(int);

private:
  //! Embedded pointer.
  Doubly_Linked_Object* ptr;

  friend bool operator==<T>(const EList_Iterator& x, const EList_Iterator& y);

  friend bool operator!=<T>(const EList_Iterator& x, const EList_Iterator& y);
};

#include "EList_Iterator.inlines.hh"

#endif // !defined(PPL_EList_Iterator_defs_hh)
