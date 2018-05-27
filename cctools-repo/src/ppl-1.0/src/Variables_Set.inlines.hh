/* Variables_Set class implementation: inline functions.
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

#ifndef PPL_Variables_Set_inlines_hh
#define PPL_Variables_Set_inlines_hh 1

#include "globals.defs.hh"
#include <stdexcept>

namespace Parma_Polyhedra_Library {

inline
Variables_Set::Variables_Set()
  : Base() {
}

inline void
Variables_Set::insert(const Variable v) {
  insert(v.id());
}

inline
Variables_Set::Variables_Set(const Variable v)
  : Base() {
  insert(v);
}

inline dimension_type
Variables_Set::max_space_dimension() {
  return Variable::max_space_dimension();
}

inline dimension_type
Variables_Set::space_dimension() const {
  reverse_iterator i = rbegin();
  return (i == rend()) ? 0 : (*i + 1);
}

inline memory_size_type
Variables_Set::external_memory_in_bytes() const {
  // We assume sets are implemented by means of red-black trees that
  // require to store the color (we assume an enum) and three pointers
  // to the parent, left and right child, respectively.
  enum color { red, black };
  return size() * (sizeof(color) + 3*sizeof(void*) + sizeof(dimension_type));
}

inline memory_size_type
Variables_Set::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Variables_Set_inlines_hh)
