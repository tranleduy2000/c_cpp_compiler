/* Watchdog and associated classes' implementation: inline functions.
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

#ifndef PPL_Watchdog_inlines_hh
#define PPL_Watchdog_inlines_hh 1

#include "Handler_defs.hh"
#include <stdexcept>

namespace Parma_Polyhedra_Library {

#if PPL_HAVE_DECL_SETITIMER && PPL_HAVE_DECL_SIGACTION

template <typename Flag_Base, typename Flag>
Watchdog::Watchdog(long csecs,
                   const Flag_Base* volatile& holder,
                   Flag& flag)
  : expired(false),
    handler(*new
            Implementation::Watchdog::Handler_Flag<Flag_Base, Flag>(holder,
                                                                    flag)) {
  if (csecs == 0)
    throw std::invalid_argument("Watchdog constructor called with a"
                                " non-positive number of centiseconds");
  in_critical_section = true;
  pending_position = new_watchdog_event(csecs, handler, expired);
  in_critical_section = false;
}

inline
Watchdog::Watchdog(long csecs, void (* const function)())
  : expired(false),
    handler(*new Implementation::Watchdog::Handler_Function(function)) {
  if (csecs == 0)
    throw std::invalid_argument("Watchdog constructor called with a"
                                " non-positive number of centiseconds");
  in_critical_section = true;
  pending_position = new_watchdog_event(csecs, handler, expired);
  in_critical_section = false;
}

inline
Watchdog::~Watchdog() {
  if (!expired) {
    in_critical_section = true;
    remove_watchdog_event(pending_position);
    in_critical_section = false;
  }
  delete &handler;
}

inline void
Watchdog::reschedule() {
  set_timer(reschedule_time);
}

#else // !PPL_HAVE_DECL_SETITIMER !! !PPL_HAVE_DECL_SIGACTION

template <typename Flag_Base, typename Flag>
Watchdog::Watchdog(long /* csecs */,
                   const Flag_Base* volatile& /* holder */,
                   Flag& /* flag */) {
  throw std::logic_error("PPL::Watchdog::Watchdog objects not supported:"
                         " system does not provide setitimer()");
}

inline
Watchdog::Watchdog(long /* csecs */, void (* /* function */)()) {
  throw std::logic_error("PPL::Watchdog::Watchdog objects not supported:"
                         " system does not provide setitimer()");
}

inline
Watchdog::~Watchdog() {
}

#endif // !PPL_HAVE_DECL_SETITIMER !! !PPL_HAVE_DECL_SIGACTION

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Watchdog_inlines_hh)
