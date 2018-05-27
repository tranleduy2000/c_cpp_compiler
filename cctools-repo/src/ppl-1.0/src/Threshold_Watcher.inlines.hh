/* Threshold_Watcher and associated classes' implementation: inline functions.
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

#ifndef PPL_Threshold_Watcher_inlines_hh
#define PPL_Threshold_Watcher_inlines_hh 1

#include <stdexcept>

#include "Handler.defs.hh"

namespace Parma_Polyhedra_Library {

template <typename Traits>
template <typename Flag_Base, typename Flag>
Threshold_Watcher<Traits>
::Threshold_Watcher(const typename Traits::Delta& delta,
                    const Flag_Base* volatile& holder,
                    Flag& flag)
  : expired(false),
    handler(*new
            Implementation::Watchdog::Handler_Flag<Flag_Base, Flag>(holder,
                                                                    flag)) {
  typename Traits::Threshold threshold;
  Traits::from_delta(threshold, delta);
  if (!Traits::less_than(Traits::get(), threshold))
    throw std::invalid_argument("Threshold_Watcher constructor called with a"
				" threshold already reached");
  pending_position = add_threshold(threshold, handler, expired);
}

template <typename Traits>
inline
Threshold_Watcher<Traits>::Threshold_Watcher(const typename Traits::Delta& delta, void (*function)())
  : expired(false),
    handler(*new Implementation::Watchdog::Handler_Function(function)) {
  typename Traits::Threshold threshold;
  Traits::from_delta(threshold, delta);
  if (!Traits::less_than(Traits::get(), threshold))
    throw std::invalid_argument("Threshold_Watcher constructor called with a"
				" threshold already reached");
  pending_position = add_threshold(threshold, handler, expired);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Threshold_Watcher_inlines_hh)
