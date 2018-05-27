m4_divert(-1)
m4_define(`dnl', `m4_dnl')

dnl This m4 file is used to generate ppl_prolog_domains.hh
dnl and files ppl_prolog_DOMAIN.hh, for each interface domain DOMAIN
dnl in ppl_interface instantiations.m4 using the code in
dnl ppl_interface_generator_prolog_hh_code.m4.

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
m4_include(ppl_interface_generator_prolog_hh_code.m4)
m4_include(ppl_interface_generator_prolog_procedure_generators.m4)

m4_divert`'dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_prolog_domains.hh << ___END_OF_FILE___
/* Prolog interface code: inclusion of all domain declarations.
m4_include(`ppl_interface_generator_copyright')dnl
*/
m4_divert(-1)

dnl Ensure any schematic procedure macro that is not defined
dnl in the code file outputs a warning message.
m4_define(`m4_procedure_schema_debug', `m4_dumpdef($1`'_code)')
dnl m4_pre_extra_class_code(Class_Counter)
dnl Prefix extra code for each class.
m4_define(`m4_pre_extra_class_code', `dnl
m4_define(`m4_current_interface', m4_interface_class`'$1)
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_prolog_domains.hh << ___END_OF_FILE___
`#'include "ppl_prolog_`'m4_current_interface.hh"
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_prolog_`'m4_current_interface.hh << ___END_OF_FILE___
/* Prolog m4_current_interface interface code: declarations.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

m4_undefine(`m4_current_interface')`'dnl
')

m4_divert`'dnl

`#'include "ppl_prolog_common.defs.hh"`'dnl
dnl
dnl Generate the non-fixed part of the file.
m4_all_code`'dnl
___END_OF_FILE___
dnl
dnl End of file generation.

