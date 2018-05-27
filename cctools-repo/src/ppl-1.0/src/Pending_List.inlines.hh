/* Pending_List class implementation: inline functions.
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

#ifndef PPL_Pending_List_inlines_hh
#define PPL_Pending_List_inlines_hh 1

#include <cassert>

namespace Parma_Polyhedra_Library {

namespace Implementation {

namespace Watchdog {

template <typename Traits>
inline
Pending_List<Traits>::Pending_List()
  : active_list(),
    free_list() {
  assert(OK());
}

template <typename Traits>
inline
Pending_List<Traits>::~Pending_List() {
}

template <typename Traits>
inline typename Pending_List<Traits>::Iterator
Pending_List<Traits>::begin() {
  return active_list.begin();
}

template <typename Traits>
inline typename Pending_List<Traits>::Iterator
Pending_List<Traits>::end() {
  return active_list.end();
}

template <typename Traits>
inline bool
Pending_List<Traits>::empty() const {
  return active_list.empty();
}

template <typename Traits>
inline typename Pending_List<Traits>::Iterator
Pending_List<Traits>::erase(Iterator position) {
  assert(!empty());
  Iterator next = active_list.erase(position);
  free_list.push_back(*position);
  assert(OK());
  return next;
}

} // namespace Watchdog

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Pending_List_inlines_hh)
