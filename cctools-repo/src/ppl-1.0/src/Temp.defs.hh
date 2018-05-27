/* Temp_* classes declarations.
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

#ifndef PPL_Temp_defs_hh
#define PPL_Temp_defs_hh 1

#include "meta_programming.hh"
#include "Slow_Copy.hh"

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A pool of temporary items of type \p T.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Temp_Item {
public:
  //! Obtains a reference to a temporary item.
  static Temp_Item& obtain();

  //! Releases the temporary item \p p.
  static void release(Temp_Item& p);

  //! Returns a reference to the encapsulated item.
  T& item();

private:
  //! The encapsulated item.
  T item_;

  //! Pointer to the next item in the free list.
  Temp_Item* next;

  //! Head of the free list.
  static Temp_Item* free_list_head;

  //! Default constructor.
  Temp_Item();

  //! Copy constructor: private and intentionally not implemented.
  Temp_Item(const Temp_Item&);

  //! Assignment operator: private and intentionally not implemented.
  Temp_Item& operator=(const Temp_Item&);
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! An holder for a reference to a temporary object.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Temp_Reference_Holder {
public:
  //! Constructs an holder holding a dirty temp.
  Temp_Reference_Holder();

  //! Destructor.
  ~Temp_Reference_Holder();

  //! Returns a reference to the held item.
  T& item();

private:
  //! Copy constructor: private and intentionally not implemented.
  Temp_Reference_Holder(const Temp_Reference_Holder&);

  //! Assignment operator: private and intentionally not implemented.
  Temp_Reference_Holder& operator=(const Temp_Reference_Holder&);

  //! The held item, encapsulated.
  Temp_Item<T>& held;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! An (fake) holder for the value of a temporary object.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Temp_Value_Holder {
public:
  //! Constructs a fake holder.
  Temp_Value_Holder();

  //! Returns the value of the held item.
  T& item();

private:
  //! Copy constructor: private and intentionally not implemented.
  Temp_Value_Holder(const Temp_Value_Holder&);

  //! Assignment operator: private and intentionally not implemented.
  Temp_Value_Holder& operator=(const Temp_Value_Holder&);

  //! The held item.
  T item_;
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! A structure for the efficient handling of temporaries.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T, typename Enable = void>
class Dirty_Temp;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Specialization for the handling of temporaries with a free list.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Dirty_Temp<T, typename Enable_If<Slow_Copy<T>::value>::type>
  : public Temp_Reference_Holder<T> {
};

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Specialization for the handling of temporaries with local variables.
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
class Dirty_Temp<T, typename Enable_If<!Slow_Copy<T>::value>::type>
  : public Temp_Value_Holder<T> {
};

} // namespace Parma_Polyhedra_Library

#include "Temp.inlines.hh"
#include "Temp.templates.hh"

#endif // !defined(PPL_Temp_defs_hh)
