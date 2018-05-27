dnl Used by ppl_interfaces_generator_ocaml_hh_files.m4
dnl for generating the access code for the OCaml interface.
dnl Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
dnl Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)
dnl
dnl This file is part of the Parma Polyhedra Library (PPL).
dnl
dnl The PPL is free software; you can redistribute it and/or modify it
dnl under the terms of the GNU General Public License as published by the
dnl Free Software Foundation; either version 3 of the License, or (at your
dnl option) any later version.
dnl
dnl The PPL is distributed in the hope that it will be useful, but WITHOUT
dnl ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
dnl FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
dnl for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software Foundation,
dnl Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
dnl
dnl For the most up-to-date information see the Parma Polyhedra Library
dnl site: http://bugseng.com/products/ppl/ .

m4_define(`m4_access_class_code',
`dnl
namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace OCaml {

//! Give access to the embedded @CLASS@* in \p v.
inline @CPP_CLASS@*
p_@CLASS@_val(value v) {
  return unmark(*reinterpret_cast<@CPP_CLASS@**>(Data_custom_val(v)));
}

//! Give access to the embedded @CLASS@* in \p v.
inline @CPP_CLASS@*&
actual_p_@CLASS@_val(value v) {
  return *reinterpret_cast<@CPP_CLASS@**>(Data_custom_val(v));
}

inline void
custom_@CLASS@_finalize(value v) {
   if (!marked(actual_p_@CLASS@_val(v)))
      delete actual_p_@CLASS@_val(v);
}

inline value
unregistered_value_p_@CLASS@(const @CPP_CLASS@& ph) {
  extern struct custom_operations @CLASS@_custom_operations;
  value v = caml_alloc_custom(&@CLASS@_custom_operations,
			      sizeof(@CPP_CLASS@*), 0, 1);
  actual_p_@CLASS@_val(v) = const_cast<@CPP_CLASS@*>(&ph);
  return v;
}

} // namespace OCaml

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

')
