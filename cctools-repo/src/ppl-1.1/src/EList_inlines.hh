/* EList class implementation: inline functions.
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

#ifndef PPL_EList_inlines_hh
#define PPL_EList_inlines_hh 1

#include <cassert>

namespace Parma_Polyhedra_Library {

namespace Implementation {

template <typename T>
inline
EList<T>::EList()
  : Doubly_Linked_Object(this, this) {
}

template <typename T>
inline void
EList<T>::push_front(T& obj) {
  next->insert_before(obj);
}

template <typename T>
inline void
EList<T>::push_back(T& obj) {
  prev->insert_after(obj);
}

template <typename T>
inline typename EList<T>::iterator
EList<T>::insert(iterator position, T& obj) {
  position->insert_before(obj);
  return iterator(&obj);
}

template <typename T>
inline typename EList<T>::iterator
EList<T>::begin() {
  return iterator(next);
}

template <typename T>
inline typename EList<T>::iterator
EList<T>::end() {
  return iterator(this);
}

template <typename T>
inline typename EList<T>::const_iterator
EList<T>::begin() const {
  return const_iterator(next);
}

template <typename T>
inline typename EList<T>::const_iterator
EList<T>::end() const {
  return const_iterator(const_cast<EList<T>*>(this));
}

template <typename T>
inline bool
EList<T>::empty() const {
  return begin() == end();
}

template <typename T>
inline typename EList<T>::iterator
EList<T>::erase(iterator position) {
  assert(!empty());
  return iterator(position->erase());
}

template <typename T>
inline
EList<T>::~EList() {
  // Erase and deallocate all the elements.
  for (iterator i = begin(), lend = end(), next; i != lend; i = next) {
    next = erase(i);
    delete &*i;
  }
}

template <typename T>
inline bool
EList<T>::OK() const {
  for (const_iterator i = begin(), lend = end(); i != lend; ++i)
    if (!i->OK())
      return false;

  return true;
}

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_EList_inlines_hh)
