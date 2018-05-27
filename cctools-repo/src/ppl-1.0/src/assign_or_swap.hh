/* The assign_or_swap() utility functions.
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

#ifndef PPL_assign_or_swap_hh
#define PPL_assign_or_swap_hh 1

#include "meta_programming.hh"
#include "Has_Assign_Or_Swap.hh"
#include "Slow_Copy.hh"

namespace Parma_Polyhedra_Library {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface
  If there is an assign_or_swap() method, use it.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
inline typename Enable_If<Has_Assign_Or_Swap<T>::value, void>::type
assign_or_swap(T& to, T& from) {
  to.assign_or_swap(from);
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface
  If there is no assign_or_swap() method but copies are not slow, copy.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
inline typename Enable_If<!Has_Assign_Or_Swap<T>::value
                          && !Slow_Copy<T>::value, void>::type
assign_or_swap(T& to, T& from) {
  to = from;
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_CXX_interface
  If there is no assign_or_swap() and copies are slow, delegate to swap().
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename T>
inline typename Enable_If<!Has_Assign_Or_Swap<T>::value
                          && Slow_Copy<T>::value, void>::type
assign_or_swap(T& to, T& from) {
  using std::swap;
  swap(to, from);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_assign_or_swap_hh)
