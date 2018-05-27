/* Variable class implementation: inline functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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

#ifndef PPL_Variable_inlines_hh
#define PPL_Variable_inlines_hh 1

#include "globals_defs.hh"
#include <stdexcept>

namespace Parma_Polyhedra_Library {

inline dimension_type
Variable::max_space_dimension() {
  return not_a_dimension() - 1;
}

inline
Variable::Variable(dimension_type i)
  : varid((i < max_space_dimension())
          ? i
          : (throw std::length_error("PPL::Variable::Variable(i):\n"
                                     "i exceeds the maximum allowed "
                                     "variable identifier."), i)) {
}

inline dimension_type
Variable::id() const {
  return varid;
}

inline dimension_type
Variable::space_dimension() const {
  return varid + 1;
}

inline memory_size_type
Variable::external_memory_in_bytes() const {
  return 0;
}

inline memory_size_type
Variable::total_memory_in_bytes() const {
  return sizeof(*this) + external_memory_in_bytes();
}

inline void
Variable::set_output_function(output_function_type* p) {
  current_output_function = p;
}

inline Variable::output_function_type*
Variable::get_output_function() {
  return current_output_function;
}

/*! \relates Variable */
inline bool
less(const Variable v, const Variable w) {
  return v.id() < w.id();
}

inline bool
Variable::Compare::operator()(const Variable x, const Variable y) const {
  return less(x, y);
}

inline void
Variable::m_swap(Variable& v) {
  using std::swap;
  swap(varid, v.varid);
}

inline void
swap(Variable& x, Variable& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Variable_inlines_hh)
