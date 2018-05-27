/* EList_Iterator class implementation: inline functions.
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

#ifndef PPL_EList_Iterator_inlines_hh
#define PPL_EList_Iterator_inlines_hh 1

#include "Doubly_Linked_Object.defs.hh"

namespace Parma_Polyhedra_Library {

namespace Implementation {

namespace Watchdog {

template <typename T>
inline
EList_Iterator<T>::EList_Iterator() {
}

template <typename T>
inline
EList_Iterator<T>::EList_Iterator(Doubly_Linked_Object* p)
  : ptr(p) {
}

template <typename T>
inline EList_Iterator<T>&
EList_Iterator<T>::operator=(Doubly_Linked_Object* p) {
  ptr = p;
  return *this;
}

template <typename T>
inline T*
EList_Iterator<T>::operator->() {
  return static_cast<T*>(ptr);
}

template <typename T>
inline T&
EList_Iterator<T>::operator*() {
  return *operator->();
}

template <typename T>
inline EList_Iterator<T>&
EList_Iterator<T>::operator++() {
  ptr = ptr->next;
  return *this;
}

template <typename T>
inline EList_Iterator<T>
EList_Iterator<T>::operator++(int) {
  EList_Iterator tmp = *this;
  ++*this;
  return tmp;
}

template <typename T>
inline EList_Iterator<T>&
EList_Iterator<T>::operator--() {
  ptr = ptr->prev;
  return *this;
}

template <typename T>
inline EList_Iterator<T>
EList_Iterator<T>::operator--(int) {
  EList_Iterator tmp = *this;
  --*this;
  return tmp;
}

template <typename T>
inline bool
operator==(const EList_Iterator<T>& x, const EList_Iterator<T>& y) {
  return x.ptr == y.ptr;
}

template <typename T>
inline bool
operator!=(const EList_Iterator<T>& x, const EList_Iterator<T>& y) {
  return x.ptr != y.ptr;
}

} // namespace Watchdog

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_EList_Iterator_inlines_hh)
