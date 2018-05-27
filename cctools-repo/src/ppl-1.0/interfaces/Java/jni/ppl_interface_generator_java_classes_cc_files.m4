m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file parma_polyhedra_library_classes.cc
dnl using the code in ppl_interface_generator_java_classes_cc_code.m4.

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
m4_include(`ppl_interface_generator_java_classes_cc_code.m4')
m4_include(`ppl_interface_generator_java_procedure_generators.m4')

dnl The macro m4_java_prefix_one_annotation is defined in
dnl ppl_interfaces_generatos_common.m4 and will expand to the
dnl extra pattern annotations for "1" and "1!";
dnl that will insert a "1" after the "_" in the replacements.
m4_java_prefix_one_annotation

dnl -----------------------------------------------------------------
dnl Macros needed for the class-dependent code.
dnl -----------------------------------------------------------------
dnl Ensure any schematic procedure macro that is not defined
dnl in the code file outputs a warning message.
m4_define(`m4_default_code', `m4_dumpdef($1`'_code)')
m4_divert
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_java_Termination.cc << ___END_OF_FILE___
/* Java Termination interface code.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

`#'include "ppl_java_common.defs.hh"
`#'include "parma_polyhedra_library_Termination.h"
`#'include "parma_polyhedra_library_C_Polyhedron.h"
`#'include "parma_polyhedra_library_NNC_Polyhedron.h"
using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::Interfaces::Java;

dnl m4_pre_extra_class_code(Class, CPP_Class, Class_Kind)
dnl Prefix extra code for each class.
m4_define(`m4_pre_extra_class_code', `dnl
m4_define(`m4_current_interface', `m4_interface_class$1')`'dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ ppl_java_`'m4_current_interface`'.cc << ___END_OF_FILE___
/* Java m4_current_interface interface code.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

`#'include "ppl_java_common.defs.hh"
`#'include "parma_polyhedra_library_`'m4_current_interface`'.h"
m4_ifelse(m4_current_interface, Polyhedron,
`#'include "parma_polyhedra_library_C_Polyhedron.h"
`#'include "parma_polyhedra_library_NNC_Polyhedron.h"
)`'dnl
using namespace Parma_Polyhedra_Library;
using namespace Parma_Polyhedra_Library::Interfaces::Java;

')

dnl -----------------------------------------------------------------
dnl Main call to macro m4_all_code to generate code
dnl -----------------------------------------------------------------
m4_divert`'dnl
dnl Generate the non-fixed part of the file.
m4_all_code`'dnl
___END_OF_FILE___
dnl
dnl End of file generation.
