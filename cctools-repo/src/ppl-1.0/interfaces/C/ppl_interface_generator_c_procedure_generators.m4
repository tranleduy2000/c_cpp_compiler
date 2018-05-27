m4_divert(-1)

dnl This m4 file defines the list of procedures
dnl for the C interface; this includes:
dnl - the list in the imported file and any C specific procedures.

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

dnl Include the language independent macros.
m4_include(`ppl_interface_generator_common_procedure_generators.m4')
m4_include(`ppl_interface_generator_common.m4')
m4_include(`ppl_interface_generator_common_dat.m4')

dnl m4_procedure_list
dnl This is the main procedure for generating the C code.
dnl
dnl Note that the code for the schema "<name>_code" must be defined
dnl in the ppl_interface_generator_*_code.m4 file.
dnl The <name> must be exactly as written here.

m4_define(`m4_procedure_list',
  `m4_echo_unquoted(ppl_new_@CLASS@_iterator +pointset_powerset,
`m4_common_procedure_list',
ppl_new_@TOPOLOGY@@CLASS@_recycle_@BUILD_REPRESENT@s +simple,
ppl_assign_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@ +simple,
ppl_@CLASS@_add_recycled_@CLASS_REPRESENT@s +simple,
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@ +simple,
ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@ +simple,
ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@ +simple,
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2 +simple,
ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2 +simple,
ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2 +simple,
)
')
