/* Implementation of marked pointers for use in some PPL non-templatic
   language interfaces.
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

#ifndef PPL_marked_pointers_hh
#define PPL_marked_pointers_hh 1

namespace Parma_Polyhedra_Library {

#define PPL_MARKED_POINTERS_MASK ((uintptr_t) 1)

/*! \brief
  Returns the marked pointer corresponding to \p p.

  The marking consists in setting the least significant bit to 1.
*/
template <typename T>
inline T*
mark(T* p) {
  uintptr_t uint_p = reinterpret_cast<uintptr_t>(p);
  return reinterpret_cast<T*>(uint_p | (PPL_MARKED_POINTERS_MASK));
}

//! Returns the unmarked pointer corresponding to \p p.
template <typename T>
inline T*
unmark(T* p) {
  uintptr_t uint_p = reinterpret_cast<uintptr_t>(p);
  return reinterpret_cast<T*>(uint_p & ~(PPL_MARKED_POINTERS_MASK));
}

//! Returns <CODE>true</CODE> if and only if \p p is marked.
template <typename T>
inline bool
marked(T* p) {
  uintptr_t uint_p = reinterpret_cast<uintptr_t>(p);
  return (uint_p & (PPL_MARKED_POINTERS_MASK)) != 0;
}

#undef PPL_MARKED_POINTERS_MASK

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_marked_pointers_hh)
