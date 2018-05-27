/* Time class declaration.
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

#ifndef PPL_Time_defs_hh
#define PPL_Time_defs_hh 1

#include "Time.types.hh"

namespace Parma_Polyhedra_Library {

namespace Implementation {

namespace Watchdog {

//! Returns <CODE>true</CODE> if and only if \p x and \p y are equal.
bool operator==(const Time& x, const Time& y);

//! Returns <CODE>true</CODE> if and only if \p x and \p y are different.
bool operator!=(const Time& x, const Time& y);

//! Returns <CODE>true</CODE> if and only if \p x is shorter than \p y.
bool operator<(const Time& x, const Time& y);

/*! \brief
  Returns <CODE>true</CODE> if and only if \p x is shorter than
  or equal to \p y.
*/
bool operator<=(const Time& x, const Time& y);

//! Returns <CODE>true</CODE> if and only if \p x is longer than \p y.
bool operator>(const Time& x, const Time& y);

/*! \brief
  Returns <CODE>true</CODE> if and only if \p x is longer than
  or equal to \p y.
*/
bool operator>=(const Time& x, const Time& y);

//! Returns the sum of \p x and \p y.
Time operator+(const Time& x, const Time& y);

/*! \brief
  Returns the difference of \p x and \p y or the null interval,
  if \p x is shorter than \p y.
*/
Time operator-(const Time& x, const Time& y);

} // namespace Watchdog

} // namespace Implementation

} // namespace Parma_Polyhedra_Library

//! A class for representing and manipulating positive time intervals.
class Parma_Polyhedra_Library::Implementation::Watchdog::Time {
public:
  //! Zero seconds.
  Time();

  //! Constructor taking a number of centiseconds.
  explicit Time(long centisecs);

  //! Constructor with seconds and microseconds.
  Time(long s, long m);

  /*! \brief
    Returns the number of whole seconds contained in the represented
    time interval.
  */
  long seconds() const;

  /*! \brief
    Returns the number of microseconds that, when added to the number
    of seconds returned by seconds(), give the represent time interval.
  */
  long microseconds() const;

  //! Adds \p y to \p *this.
  Time& operator+=(const Time& y);

  /*! \brief
    Subtracts \p y from \p *this; if \p *this is shorter than \p y,
    \p *this is set to the null interval.
  */
  Time& operator-=(const Time& y);

  //! Checks if all the invariants are satisfied.
  bool OK() const;

private:
  //! Number of microseconds in a second.
  static const long USECS_PER_SEC = 1000000L;

  //! Number of centiseconds in a second.
  static const long CSECS_PER_SEC = 100L;

  //! Number of seconds.
  long secs;

  //! Number of microseconds.
  long microsecs;
};

#include "Time.inlines.hh"

#endif // !defined(PPL_Time_defs_hh)
