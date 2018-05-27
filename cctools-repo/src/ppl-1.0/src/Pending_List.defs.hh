/* Pending_List class declaration.
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

#ifndef PPL_Pending_List_defs_hh
#define PPL_Pending_List_defs_hh 1

#include "Pending_List.types.hh"
#include "Pending_Element.defs.hh"
#include "EList.defs.hh"
#include "Handler.types.hh"

//! An ordered list for recording pending watchdog events.
template <typename Traits>
class Parma_Polyhedra_Library::Implementation::Watchdog::Pending_List {
public:
  //! A non-const iterator to traverse the list.
  typedef typename EList<Pending_Element<typename Traits::Threshold> >::Iterator Iterator;

  //! A const iterator to traverse the list.
  typedef typename EList<Pending_Element<typename Traits::Threshold> >::Const_Iterator Const_Iterator;

  //! Constructs an empty list.
  Pending_List();

  //! Destructor.
  ~Pending_List();

  //! Inserts a new Pending_Element object with the given attributes.
  Iterator insert(const typename Traits::Threshold& deadline,
		  const Handler& handler,
		  bool& expired_flag);

  /*! \brief
    Removes the element pointed to by \p position, returning
    an iterator pointing to the next element, if any, or end(), otherwise.
  */
  Iterator erase(Iterator position);

  //! Returns <CODE>true</CODE> if and only if the list is empty.
  bool empty() const;

  //! Returns an iterator pointing to the beginning of the list.
  Iterator begin();

  //! Returns an iterator pointing one past the last element in the list.
  Iterator end();

  //! Checks if all the invariants are satisfied.
  bool OK() const;

private:
  EList<Pending_Element<typename Traits::Threshold> > active_list;
  EList<Pending_Element<typename Traits::Threshold> > free_list;
};

#include "Pending_List.inlines.hh"
#include "Pending_List.templates.hh"

#endif // !defined(PPL_Pending_List_defs_hh)
