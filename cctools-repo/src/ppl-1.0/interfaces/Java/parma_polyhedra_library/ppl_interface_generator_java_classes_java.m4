m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the files <CLASS_NAME>.java
dnl using the code in ppl_interface_generator_java_classes_java_code.m4.

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
m4_include(`ppl_interface_generator_java_classes_java_code.m4')
m4_include(`ppl_interface_generator_java_procedure_generators.m4')

dnl Ensure any schematic procedure macro that is not defined
dnl in the code file outputs a warning message.
m4_define(`m4_default_code', `m4_dumpdef($1`'_code)')

m4_divert
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
/* PPL Java interface: Termination definition.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

package parma_polyhedra_library;
public class Termination {`'dnl

m4_divert(-1)

dnl m4_pre_extra_class_code(Class, CPP_Class, Class_Kind)
dnl Prefix extra code for each class.
m4_define(`m4_pre_extra_class_code', `dnl
m4_define(`m4_this_class', `m4_interface_class$1')`'dnl
m4_define(`m4_this_cplusplus_class', `Parma_Polyhedra_Library::`'m4_cplusplus_class$1')`'dnl
m4_define(`m4_this_class_kind', `m4_class_kind$1')`'dnl
m4_divert
___END_OF_FILE___
___BEGIN_OF_FILE___ m4_this_class`'.java << ___END_OF_FILE___
/* PPL Java interface: m4_this_class definition.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

package parma_polyhedra_library;

import java.io.Writer;
import java.io.IOException;

/*! \brief
  Java class interfacing C++ m4_this_cplusplus_class
  \ingroup PPL_java_interface
*/
public class m4_this_class extends PPL_Object {`'dnl
m4_ifelse(m4_this_class, Polyhedron,
  `
___END_OF_FILE___
___BEGIN_OF_FILE___ C_Polyhedron.java << ___END_OF_FILE___
/* PPL Java interface: C_Polyhedron definition.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

package parma_polyhedra_library;

/*! \brief
  Java class interfacing C++ Parma_Polyhedra_Library::C_Polyhedron
  \ingroup PPL_java_interface
*/
public class C_Polyhedron extends Polyhedron {
___END_OF_FILE___
___BEGIN_OF_FILE___ NNC_Polyhedron.java << ___END_OF_FILE___
/* PPL Java interface: NNC_Polyhedron definition.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

package parma_polyhedra_library;

/*! \brief
  Java class interfacing C++ Parma_Polyhedra_Library::NNC_Polyhedron
  \ingroup PPL_java_interface
*/
public class NNC_Polyhedron extends Polyhedron {
',
m4_this_class_kind, Pointset_Powerset,
`
___END_OF_FILE___
___BEGIN_OF_FILE___ m4_this_class`'_Iterator.java << ___END_OF_FILE___
/* PPL Java interface: m4_this_class`'_Iterator definition.
m4_include(`ppl_interface_generator_copyright')`'dnl
*/

package parma_polyhedra_library;

/*! \brief
  Java class interfacing C++ m4_this_cplusplus_class`':: iterator
  \ingroup PPL_java_interface
*/
public class m4_this_class`'_Iterator extends PPL_Object {

  private m4_this_class`'_Iterator() {};

  private native void build_ppl_object(m4_this_class obj);
')`'dnl
m4_undefine(`m4_this_class')`'dnl
m4_undefine(`m4_this_cplusplus_class')`'dnl
m4_undefine(`m4_this_class_kind')
')

m4_divert(-1)

dnl m4_post_extra_class_code(Class, CPP_Class, Class_Kind)
dnl Postfix extra code for each class.
m4_define(`m4_post_extra_class_code', `dnl
m4_replace_all_patterns($1, m4_class_build_cpp_object1_code)`'dnl
m4_replace_all_patterns($1, m4_class_build_cpp_object2_code)`'dnl
m4_replace_all_patterns($1, m4_class_build_cpp_object3_code)`'dnl
m4_define(`m4_this_class', `m4_interface_class$1')`'dnl
m4_define(`m4_this_class_kind', `m4_class_kind$1')
___END_OF_FILE___
___BEGIN_OF_FILE___ m4_this_class`'.java << ___END_OF_FILE___
}`'dnl
m4_ifelse(m4_this_class, Polyhedron,
  `
___END_OF_FILE___
___BEGIN_OF_FILE___ C_Polyhedron.java << ___END_OF_FILE___
}

___END_OF_FILE___
___BEGIN_OF_FILE___ NNC_Polyhedron.java << ___END_OF_FILE___
}
',
m4_this_class_kind, Pointset_Powerset,
  `
___END_OF_FILE___
___BEGIN_OF_FILE___ m4_this_class`'_Iterator.java << ___END_OF_FILE___
}`'dnl
')`'dnl
m4_undefine(`m4_this_class')`'dnl
m4_undefine(`m4_this_class_kind')
')

dnl -----------------------------------------------------------------
dnl Main calls to macro m4_all_code to generate code
dnl -----------------------------------------------------------------
m4_divert`'dnl
m4_all_code`'dnl
___END_OF_FILE___
m4_divert(-1)
m4_divert
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
}
___END_OF_FILE___
m4_divert(-1)
dnl End of file generation.
