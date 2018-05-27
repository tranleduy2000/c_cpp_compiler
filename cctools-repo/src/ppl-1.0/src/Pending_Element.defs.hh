/* Pending_Element class declaration.
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

#ifndef PPL_Pending_Element_defs_hh
#define PPL_Pending_Element_defs_hh 1

#include "Pending_Element.types.hh"
#include "Doubly_Linked_Object.defs.hh"
#include "Handler.types.hh"

//! A class for pending watchdog events with embedded links.
/*!
  Each pending watchdog event is characterized by a deadline (a positive
  time interval), an associated handler that will be invoked upon event
  expiration, and a Boolean flag that indicates whether the event has already
  expired or not.
*/
template <typename Threshold>
class Parma_Polyhedra_Library::Implementation::Watchdog::Pending_Element
  : public Doubly_Linked_Object {
public:
  //! Constructs an element with the given attributes.
  Pending_Element(const Threshold& deadline,
		  const Handler& handler,
		  bool& expired_flag);

  //! Modifies \p *this so that it has the given attributes.
  void assign(const Threshold& deadline,
	      const Handler& handler,
	      bool& expired_flag);

  //! Returns the deadline of the event.
  const Threshold& deadline() const;

  //! Returns the handler associated to the event.
  const Handler& handler() const;

  //! Returns a reference to the "event-expired" flag.
  bool& expired_flag() const;

  //! Checks if all the invariants are satisfied.
  bool OK() const;

private:
  //! The deadline of the event.
  Threshold d;

  //! A pointer to the handler associated to the event.
  const Handler* p_h;

  //! A pointer to a flag saying whether the event has already expired or not.
  bool* p_f;
};

#include "Pending_Element.inlines.hh"

#endif // !defined(PPL_Pending_Element_defs_hh)
