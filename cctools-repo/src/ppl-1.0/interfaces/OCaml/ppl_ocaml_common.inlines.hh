/* Domain-independent part of the OCaml interface: inline functions.
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

#ifndef PPL_ppl_ocaml_common_inlines_hh
#define PPL_ppl_ocaml_common_inlines_hh 1

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace OCaml {

template <typename U_Int>
U_Int
value_to_unsigned(value v) {
  // FIXME: check the following at compile time.
  assert(std::numeric_limits<U_Int>::is_integer
         && !std::numeric_limits<U_Int>::is_signed);
  if (!Is_long(v)) {
    const char* what = "PPL OCaml interface invalid argument error:\n"
      "argument is not an integer (expecting a non-negative integer).";
    throw std::invalid_argument(what);
  }
  intnat vv = Long_val(v);
  if (vv < 0) {
    const char* what = "PPL OCaml interface invalid argument error:\n"
      "argument is negative (expecting a non-negative integer).";
    throw std::invalid_argument(what);
  }
  const uintnat u_max = std::numeric_limits<U_Int>::max();
  if (static_cast<uintnat>(vv) > u_max) {
    const char* what = "PPL OCaml interface invalid argument:\n"
      "argument value is too big (expecting a smaller non-negative integer).";
    throw std::invalid_argument(what);
  }
  return static_cast<U_Int>(vv);
}

inline dimension_type
value_to_ppl_dimension(value v) {
  return value_to_unsigned<dimension_type>(v);
}

inline value
ppl_dimension_to_value(dimension_type dim) {
  // FIXME: what if this assertion fails?
  assert(static_cast<uintnat>(dim) <= static_cast<uintnat>(Max_long));
  return Val_long(dim);
}

inline Variable
build_ppl_Variable(value caml_var) {
  return Variable(value_to_ppl_dimension(caml_var));
}

} // namespace OCaml

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_ppl_ocaml_common_inlines_hh)
