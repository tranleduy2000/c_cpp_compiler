m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)dnl

dnl This m4 file is used to generate ppl_ocaml_domains.hh
dnl and files ppl_ocaml_DOMAIN.hh, for each interface domain DOMAIN
dnl in ppl_interface instantiations.m4 using the code in
dnl ppl_interface_generator_ocaml_hh_code.m4.

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
m4_include(`ppl_interface_generator_ocaml_hh_code.m4')
m4_include(`ppl_interface_generator_ocaml_procedure_generators.m4')

m4_pushdef(`m4_one_class_code', `dnl
m4_define(`m4_current_interface', m4_interface_class`'$1)`'dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_ocaml_domains.hh << ___END_OF_FILE___
`#'include "ppl_ocaml_`'m4_current_interface.hh"
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_ocaml_`'m4_current_interface.hh << ___END_OF_FILE___
/* OCaml m4_current_interface interface code: declarations.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

m4_replace_all_patterns($1, m4_access_class_code)`'dnl
m4_undefine(`m4_current_interface')`'dnl
')

dnl -----------------------------------------------------------------
dnl Generate type declarations for all the classes.
dnl -----------------------------------------------------------------

m4_divert`'dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_ocaml_domains.hh << ___END_OF_FILE___
/* OCaml interface code: inclusion of all the domain-dependent declarations.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

`#'include "ppl_ocaml_common.defs.hh"
m4_all_code
___END_OF_FILE___
dnl
dnl End of file generation.
