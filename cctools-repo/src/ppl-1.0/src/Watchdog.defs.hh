/* Watchdog and associated classes' declaration and inline functions.
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

#ifndef PPL_Watchdog_defs_hh
#define PPL_Watchdog_defs_hh 1

#include "Watchdog.types.hh"
#include "Time.defs.hh"
#include "Handler.types.hh"
#include "Pending_List.defs.hh"
#include <cassert>
#include <functional>

#ifdef PPL_HAVE_SYS_TIME_H
# include <sys/time.h>
#endif

namespace Parma_Polyhedra_Library {

// Set linkage now to declare it friend later.
extern "C" void PPL_handle_timeout(int signum);

struct Watchdog_Traits {
  typedef Implementation::Watchdog::Time Threshold;
  static bool less_than(const Threshold& a, const Threshold& b) {
    return a < b;
  }
};

//! A watchdog timer.
class Watchdog {
public:
  template <typename Flag_Base, typename Flag>
  Watchdog(long csecs, const Flag_Base* volatile& holder, Flag& flag);

  /*! \brief
    Constructor: if not reset, the watchdog will trigger after \p csecs
    centiseconds, invoking handler \p function.
  */
  Watchdog(long csecs, void (*function)());

  //! Destructor.
  ~Watchdog();

#if PPL_HAVE_DECL_SETITIMER && PPL_HAVE_DECL_SIGACTION

  //! Static class initialization.
  static void initialize();
  //! Static class finalization.
  static void finalize();

private:
  //! Whether or not this watchdog has expired.
  bool expired;

  typedef Implementation::Watchdog::Pending_List<Watchdog_Traits>
  WD_Pending_List;

  typedef Implementation::Watchdog::Handler
  WD_Handler;

  const WD_Handler& handler;
  WD_Pending_List::Iterator pending_position;

  // Private and not implemented: copy construction is not allowed.
  Watchdog(const Watchdog&);
  // Private and not implemented: copy assignment is not allowed.
  Watchdog& operator=(const Watchdog&);

  // Pass this to getitimer().
  static itimerval current_timer_status;

  //! Reads the timer value into \p time.
  static void get_timer(Implementation::Watchdog::Time& time);

  // Pass this to setitimer().
  static itimerval signal_once;

  // Last time value we set the timer to.
  static Implementation::Watchdog::Time last_time_requested;

  //! Sets the timer value to \p time.
  static void set_timer(const Implementation::Watchdog::Time& time);

  //! Stops the timer.
  static void stop_timer();

  //! Quick reschedule to avoid race conditions.
  static void reschedule();

  // Used by the above.
  static Implementation::Watchdog::Time reschedule_time;

  // Records the time elapsed since last fresh start.
  static Implementation::Watchdog::Time time_so_far;

  //! The ordered queue of pending watchdog events.
  static WD_Pending_List pending;

  //! The actual signal handler.
  static void handle_timeout(int);

  //! Handles the addition of a new watchdog event.
  static WD_Pending_List::Iterator
  new_watchdog_event(long csecs,
                     const WD_Handler& handler,
                     bool& expired_flag);

  //! Handles the removal of the watchdog event referred by \p position.
  void remove_watchdog_event(WD_Pending_List::Iterator position);

  //! Whether the alarm clock is running.
  static volatile bool alarm_clock_running;

  //! Whether we are changing data that is also changed by the signal handler.
  static volatile bool in_critical_section;

  friend void PPL_handle_timeout(int signum);

#endif // PPL_HAVE_DECL_SETITIMER && PPL_HAVE_DECL_SIGACTION
};

} // namespace Parma_Polyhedra_Library

#include "Watchdog.inlines.hh"

#endif // !defined(PPL_Watchdog_defs_hh)

