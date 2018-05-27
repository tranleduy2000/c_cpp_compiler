/* Threshold_Watcher and associated classes' declaration and inline functions.
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

#ifndef PPL_Threshold_Watcher_defs_hh
#define PPL_Threshold_Watcher_defs_hh 1

#include "Threshold_Watcher.types.hh"
#include "Handler.types.hh"
#include "Pending_List.defs.hh"
#include <cassert>

/*! \brief
  A class of watchdogs controlling the exceeding of a threshold.

  \tparam Traits
  A class to set data types and functions for the threshold handling.
  See \c Parma_Polyhedra_Library::Weightwatch_Traits for an example.
*/
template <typename Traits>
class Parma_Polyhedra_Library::Threshold_Watcher {
public:
  template <typename Flag_Base, typename Flag>
  Threshold_Watcher(const typename Traits::Delta& delta,
                    const Flag_Base* volatile& holder,
                    Flag& flag);

  Threshold_Watcher(const typename Traits::Delta& delta,
                    void (*function)());

  ~Threshold_Watcher();

private:
  typedef Implementation::Watchdog::Pending_List<Traits> TW_Pending_List;
  typedef Implementation::Watchdog::Handler TW_Handler;

  bool expired;
  const TW_Handler& handler;
  typename TW_Pending_List::Iterator pending_position;

  // Just to prevent their use.
  Threshold_Watcher(const Threshold_Watcher&);
  Threshold_Watcher& operator=(const Threshold_Watcher&);

  struct Initialize {
    //! The ordered queue of pending thresholds.
    TW_Pending_List pending;
  };
  static Initialize init;

  // Handle the addition of a new threshold.
  static typename TW_Pending_List::Iterator
  add_threshold(typename Traits::Threshold threshold,
		const TW_Handler& handler,
		bool& expired_flag);

  // Handle the removal of a threshold.
  static typename TW_Pending_List::Iterator
  remove_threshold(typename TW_Pending_List::Iterator position);

  //! Check threshold reaching.
  static void check();

}; // class Parma_Polyhedra_Library::Threshold_Watcher


// Templatic initialization of static data member.
template <typename Traits>
typename
Parma_Polyhedra_Library::Threshold_Watcher<Traits>::Initialize
Parma_Polyhedra_Library::Threshold_Watcher<Traits>::init;

#include "Threshold_Watcher.inlines.hh"
#include "Threshold_Watcher.templates.hh"

#endif // !defined(PPL_Threshold_Watcher_defs_hh)

