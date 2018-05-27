m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file ppl_ocaml.ml
dnl using the code in ppl_interface_generator_ocaml_ml_code.m4.

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

dnl Include files defining macros that generate the non-fixed part.
m4_include(`ppl_interface_generator_ocaml_ml_code.m4')
m4_include(`ppl_interface_generator_ocaml_procedure_generators.m4')

m4_divert`'dnl
(* OCaml interface code.
m4_include(`ppl_interface_generator_copyright')`'dnl
*)

open Gmp
include Ppl_ocaml_globals

m4_divert(-1)
m4_pushdef(`m4_one_class_code', `type m4_downcase(m4_interface_class$1)
')

dnl -----------------------------------------------------------------
dnl Generate type declarations for all the classes.
dnl -----------------------------------------------------------------
m4_divert`'dnl
m4_all_code
m4_divert(-1)
m4_popdef(`m4_one_class_code')

dnl -----------------------------------------------------------------
dnl Generate the main class-dependent code.
dnl -----------------------------------------------------------------
dnl Ensure any schematic procedure macro that is not defined
dnl in the code file outputs a warning message.
m4_define(`m4_default_code', `m4_dumpdef($1`'_code)')
m4_divert`'dnl
m4_all_code
dnl
dnl End of file generation.
