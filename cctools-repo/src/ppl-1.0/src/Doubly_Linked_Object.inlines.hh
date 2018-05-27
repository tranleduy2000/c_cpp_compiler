/* Doubly_Linked_Object class implementation: inline functions.
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

#ifndef PPL_Doubly_Linked_Object_inlines_hh
#define PPL_Doubly_Linked_Object_inlines_hh 1

namespace Parma_Polyhedra_Library {

namespace Implementation {

namespace Watchdog {

inline
Doubly_Linked_Object::Doubly_Linked_Object() {
}

inline
Doubly_Linked_Object::Doubly_Linked_Object(Doubly_Linked_Object* f,
					   Doubly_Linked_Object* b)
  : next(f),
    prev(b) {
}

inline void
Doubly_Linked_Object::insert_before(Doubly_Linked_Object& y) {
  y.next = this;
  y.prev = prev;
  prev->next = &y;
  prev = &y;
}

inline void
Doubly_Linked_Object::insert_after(Doubly_Linked_Object& y) {
  y.next = next;
  y.prev = this;
  next->prev = &y;
  next = &y;
}

inline Doubly_Linked_Object*
Doubly_Linked_Object::erase() {
  next->prev = prev;
  prev->next = next;
  return next;
}

inline
Doubly_Linked_Object::~Doubly_Linked_Object() {
  erase();
}

} // namespace Watchdog

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Doubly_Linked_Object_inlines_hh)
