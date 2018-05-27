/* Pending_Element class implementation: inline functions.
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

#ifndef PPL_Pending_Element_inlines_hh
#define PPL_Pending_Element_inlines_hh 1

#include <cassert>

namespace Parma_Polyhedra_Library {

namespace Implementation {

namespace Watchdog {

template <typename Threshold>
inline bool
Pending_Element<Threshold>::OK() const {
  return true;
}

template <typename Threshold>
inline
Pending_Element<Threshold>::Pending_Element(const Threshold& deadline,
					    const Handler& handler,
					    bool& expired_flag)
  : d(deadline), p_h(&handler), p_f(&expired_flag) {
  assert(OK());
}

template <typename Threshold>
inline void
Pending_Element<Threshold>::assign(const Threshold& deadline,
				   const Handler& handler,
				   bool& expired_flag) {
  d = deadline;
  p_h = &handler;
  p_f = &expired_flag;
  assert(OK());
}

template <typename Threshold>
inline const Threshold&
Pending_Element<Threshold>::deadline() const {
  return d;
}

template <typename Threshold>
inline const Handler&
Pending_Element<Threshold>::handler() const {
  return *p_h;
}

template <typename Threshold>
inline bool&
Pending_Element<Threshold>::expired_flag() const {
  return *p_f;
}

} // namespace Watchdog

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Pending_Element_inlines_hh)
