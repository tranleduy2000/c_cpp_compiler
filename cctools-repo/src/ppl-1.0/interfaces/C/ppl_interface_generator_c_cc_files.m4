m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file ppl_c_cc
dnl using the code in ppl_interface_generator_c_cc_code.m4.

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
m4_include(`ppl_interface_generator_c_cc_code.m4')
m4_include(`ppl_interface_generator_c_procedure_generators.m4')
m4_define(`m4_pattern_extensions', `, A_, B_')

dnl Ensure any schematic procedure macro that is not defined
dnl in the code file outputs a warning message.
m4_define(`m4_default_code', `m4_dumpdef($1`'_code)')
dnl -----------------------------------------------------------------
dnl Macros needed for the class-dependent code.
dnl -----------------------------------------------------------------
dnl Prefix extra code for each class.
m4_define(`m4_pre_extra_class_code', `dnl
m4_define(`m4_current_interface', m4_interface_class`'$1)`'dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_c_`'m4_current_interface`'.cc << ___END_OF_FILE___
/* C m4_current_interface interface code: definitions.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

`#'include "ppl_c_implementation_common.defs.hh"
`#'include "interfaced_boxes.hh"
`#'include "ppl_c_implementation_domains.hh"

using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::Interfaces::C;

')

dnl Postfix extra code for each class.
m4_define(`m4_post_extra_class_code', `dnl
DEFINE_OUTPUT_FUNCTIONS(m4_interface_class`'$1)
')

dnl -----------------------------------------------------------------
dnl Output the main class-dependent code.
dnl -----------------------------------------------------------------
m4_divert`'dnl
m4_all_code`'dnl
___END_OF_FILE___
dnl
dnl End of file generation.
