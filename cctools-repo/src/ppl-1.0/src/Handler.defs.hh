/* Handler and derived classes' declaration.
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

#ifndef PPL_Handler_defs_hh
#define PPL_Handler_defs_hh 1

#include "Handler.types.hh"

//! Abstract base class for handlers of the watchdog events.
class Parma_Polyhedra_Library::Implementation::Watchdog::Handler {
public:
  //! Does the job.
  virtual void act() const = 0;

  //! Virtual destructor.
  virtual ~Handler();
};

//! A kind of Handler that installs a flag onto a flag-holder.
/*!
  The template class Handler_Flag<Flag_Base, Flag> is an handler whose
  job is to install a flag onto an <EM>holder</EM> for the flag.
  The flag is of type \p Flag and the holder is a (volatile) pointer
  to \p Flag_Base.  Installing the flag onto the holder means making
  the holder point to the flag, so that it must be possible to assign
  a value of type <CODE>Flag*</CODE> to an entity of type
  <CODE>Flag_Base*</CODE>.
  The class \p Flag must provide the method

  \code
    int priority() const
  \endcode
  returning an integer priority associated to the flag.

  The handler will install its flag onto the holder only if the holder
  is empty, namely, it is the null pointer, or if the holder holds a
  flag of strictly lower priority.
 */
template <typename Flag_Base, typename Flag>
class Parma_Polyhedra_Library::Implementation::Watchdog::Handler_Flag
  : public Handler {
public:
  //! Constructor with a given function.
  Handler_Flag(const Flag_Base* volatile& holder, Flag& flag);

  /*! \brief
    Does its job: installs the flag onto the holder, if a flag with
    an higher priority has not already been installed.
  */
  virtual void act() const;

private:
  // declare holder as reference to volatile pointer to const Flag_Base
  const Flag_Base* volatile& h;
  Flag& f;
};

//! A kind of Handler calling a given function.
class Parma_Polyhedra_Library::Implementation::Watchdog::Handler_Function
  : public Handler {
public:
  //! Constructor with a given function.
  Handler_Function(void (*function)());

  //! Does its job: calls the embedded function.
  virtual void act() const;

private:
  //! Pointer to the embedded function.
  void (*f)();
};

#include "Handler.inlines.hh"

#endif // !defined(PPL_Handler_defs_hh)
