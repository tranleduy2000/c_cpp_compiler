/* Partial_Function class implementation: inline functions.
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

#ifndef PPL_Partial_Function_inlines_hh
#define PPL_Partial_Function_inlines_hh 1

#include <stdexcept>
#include "assert.hh"

namespace Parma_Polyhedra_Library {

inline
Partial_Function::Partial_Function()
  : max(0) {
}

inline bool
Partial_Function::has_empty_codomain() const {
  PPL_ASSERT(vec.empty() == codomain.empty());
  return vec.empty();
}

inline dimension_type
Partial_Function::max_in_codomain() const {
  if (has_empty_codomain())
    throw std::runtime_error("Partial_Function::max_in_codomain() called"
			     " when has_empty_codomain()");
  PPL_ASSERT(codomain.begin() != codomain.end()
             && max == *codomain.rbegin());
  return max;
}

inline void
Partial_Function::insert(dimension_type i, dimension_type j) {
#ifndef NDEBUG
  // The partial function has to be an injective map.
  std::pair<std::set<dimension_type>::iterator, bool> s = codomain.insert(j);
  PPL_ASSERT(s.second);
#endif // #ifndef NDEBUG

  // Expand `vec' if needed.
  const dimension_type sz = vec.size();
  if (i >= sz)
    vec.insert(vec.end(), i - sz + 1, not_a_dimension());

  // We cannot remap the same index to another one.
  PPL_ASSERT(i < vec.size() && vec[i] == not_a_dimension());
  vec[i] = j;

  // Maybe update `max'.
  if (j > max)
    max = j;
  PPL_ASSERT(codomain.begin() != codomain.end()
             && max == *codomain.rbegin());
}

inline bool
Partial_Function::maps(dimension_type i, dimension_type& j) const {
  if (i >= vec.size())
    return false;
  dimension_type vec_i = vec[i];
  if (vec_i == not_a_dimension())
    return false;
  j = vec_i;
  return true;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Partial_Function_inlines_hh)
