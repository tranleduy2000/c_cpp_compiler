/* Threshold_Watcher and associated classes'.
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

#ifndef PPL_Threshold_Watcher_templates_hh
#define PPL_Threshold_Watcher_templates_hh 1

namespace Parma_Polyhedra_Library {

template <typename Traits>
typename Threshold_Watcher<Traits>::TW_Pending_List::Iterator
Threshold_Watcher<Traits>::add_threshold(typename Traits::Threshold threshold,
                                         const TW_Handler& handler,
                                         bool& expired_flag) {
  Traits::check_function = Threshold_Watcher::check;
  return init.pending.insert(threshold, handler, expired_flag);
}

template <typename Traits>
typename Threshold_Watcher<Traits>::TW_Pending_List::Iterator
Threshold_Watcher<Traits>
::remove_threshold(typename TW_Pending_List::Iterator position) {
  typename TW_Pending_List::Iterator i = init.pending.erase(position);
  if (init.pending.empty())
    Traits::check_function = 0;
  return i;
}

template <typename Traits>
Threshold_Watcher<Traits>::~Threshold_Watcher() {
  if (!expired)
    remove_threshold(pending_position);
  delete &handler;
}

template <typename Traits>
void
Threshold_Watcher<Traits>::check() {
  typename TW_Pending_List::Iterator i = init.pending.begin();
  assert(i != init.pending.end());
  const typename Traits::Threshold& current = Traits::get();
  while (!Traits::less_than(current, i->deadline())) {
    i->handler().act();
    i->expired_flag() = true;
    i = remove_threshold(i);
    if (i == init.pending.end())
      break;
  }
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Threshold_Watcher_templates_hh)
