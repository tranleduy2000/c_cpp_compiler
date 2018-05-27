m4_define(`dnl', `m4_dnl')`'dnl
m4_divert(-1)

dnl This m4 file generates the file Prolog_configured_domains.dox

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

dnl FIXME: This should be in the main m4 generator file.

dnl -----------------------------------------------------------------
dnl Include the needed m4 files.
dnl -----------------------------------------------------------------
m4_include(`ppl_interface_generator_prolog_procedure_generators.m4')
m4_include(`ppl_interface_generator_prolog_dox_code.m4')

dnl =================================================================
dnl Description of file and copyright notice
dnl =================================================================
m4_divert`'dnl
/* Configuration Dependent Documentation for Prolog.
m4_include(`ppl_interface_generator_copyright')*/

/*! \page configured_domains_predicates Predicates for Configured Domains

The structure of this section is as follows:
m4_pushdef(`m4_one_class_code',`dnl
  - \ref m4_interface_class$1`'_predicates "Predicates for the m4_interface_class$1 Domain"
')`'dnl
m4_all_code`'dnl
m4_popdef(`m4_one_class_code')`'dnl
m4_divert(-1)

dnl =================================================================
dnl DOMAIN DEPENDENT DOCUMENTATION.
dnl =================================================================

dnl -----------------------------------------------------------------
dnl Initialize the class definitions
dnl as we do not use m4_all_code to generate the documentation,
dnl this must be done here.
dnl -----------------------------------------------------------------
m4_initialize_all

dnl -----------------------------------------------------------------
dnl For the documentation, the predicates for a single class are
dnl divinided into categories and then into subcategories.
dnl Each category and subcategory can have its own introduction.
dnl -----------------------------------------------------------------

dnl -----------------------------------------------------------------
dnl Define documentation for one subcategory.
dnl This requires that m4_ext is defined.
dnl -----------------------------------------------------------------
m4_define(`m4_one_subcategory', `dnl
m4_define(`m4_procedure_schema_extension', m4_ext`'_doc)`'dnl
m4_echo_unquoted(m4_subcategory_`'m4_ext)
m4_ifdef(m4_interface_class`'$1,
`m4_patsubst(m4_one_class_code($1), |COMMA|, `,')`'dnl
')`'dnl
')

dnl -----------------------------------------------------------------
dnl Define the documentation for all categories for simple domains;
dnl -----------------------------------------------------------------
m4_define(`m4_all_simple_categories', `dnl
m4_category_constr_destr`'dnl
m4_define(`m4_ext', build)`'dnl
m4_one_subcategory($1)`'dnl
m4_define(`m4_ext', convert)`'dnl
m4_one_subcategory($1)`'dnl
m4_define(`m4_ext', destruct)`'dnl
m4_one_subcategory($1)`'dnl
m4_finalize_category_constr_destr`'dnl
m4_category_const`'dnl
m4_define(`m4_ext', testpoly)`'dnl
m4_one_subcategory($1)`'dnl
m4_define(`m4_ext', constpoly)`'dnl
m4_one_subcategory($1)`'dnl
m4_define(`m4_ext', dump)`'dnl
m4_one_subcategory($1)`'dnl
m4_finalize_category_const`'dnl
m4_category_nonconst`'dnl
m4_define(`m4_ext', addto)`'dnl
m4_one_subcategory($1)`'dnl
m4_define(`m4_ext', trans)`'dnl
m4_one_subcategory($1)`'dnl
m4_define(`m4_ext', binop)`'dnl
m4_one_subcategory($1)`'dnl
m4_define(`m4_ext', widen)`'dnl
m4_one_subcategory($1)`'dnl
m4_finalize_category_nonconst`'dnl
m4_category_varspace`'dnl
m4_define(`m4_ext', concat)`'dnl
m4_one_subcategory($1)`'dnl
m4_define(`m4_ext', spacemod)`'dnl
m4_one_subcategory($1)`'dnl
m4_finalize_category_varspace`'dnl
')

dnl -----------------------------------------------------------------
dnl Define the documentation for an extra category for powerset domains;
dnl -----------------------------------------------------------------
m4_define(`m4_iter_pps_categories', `dnl
m4_define(`m4_ext', pps_iter)`'dnl
m4_one_subcategory($1)`'dnl
')

dnl -----------------------------------------------------------------
dnl Documentation for one simple class.
dnl -----------------------------------------------------------------
m4_define(`m4_one_simple_class_code', `dnl
\anchor m4_this_interface_class`'_predicates
<H1>Predicates for the m4_this_topology m4_this_interface_class Domain</H1>
  Here we provide a short description for each of the predicates
  available for the domain of m4_this_topology m4_this_ppl_objects.
m4_ifdef(`m4_prolog_ci_documentation',
  `Note that predicates for other domains will follow a similar pattern.')

m4_all_simple_categories($1)`'dnl
')

dnl -----------------------------------------------------------------
dnl FIXME: Some experimental macros for text processing to try
dnl and make the documentation a bit more grammatically correct!
dnl -----------------------------------------------------------------

dnl m4_underscore_to_space(String)
dnl
dnl Changes an underscore to space.
dnl Example: m4_underscore_to_space(`xyz_abc') ==> xyz abc
m4_define(`m4_underscore_to_space', `m4_translit(`$1', `_', ` ')')

dnl Changes an underscore to space and upper to lower case
dnl It also tries to change a powerset name into English!
m4_define(`m4_PPL_domain2text', `dnl
  m4_patsubst(m4_patsubst(m4_downcase(m4_underscore_to_space($1)),
                           ` c polyhedron', ` of C polyhedra'),
              ` nnc polyhedron', ` of NNC polyhedra')')

dnl m4_plural(String)
dnl
dnl Very simple macro for making domains (lowercase) plural.
m4_define(`m4_plural', `dnl
m4_patsubst(m4_patsubst(m4_patsubst(m4_patsubst(m4_patsubst(m4_patsubst($1,
  polyhedron, polyhedra),
  grid, grids),
  shape, shapes),
  box, boxes),
  powerset, powersets),
  product, products)`'dnl
')

dnl =================================================================
dnl Now the generation of the documentation.
dnl =================================================================

dnl -----------------------------------------------------------------
dnl CONFIGURATION INDEPENDENT
dnl -----------------------------------------------------------------

dnl -----------------------------------------------------------------
dnl Additional ad hoc predicates for the configure independent manual.
dnl
dnl Note that currently this only describes predicates for the
dnl pointset powerset domains but allows for other ad hoc predicates
dnl for other domains to be added later.
dnl -----------------------------------------------------------------
m4_define(`m4_ci_pps_class_code', `dnl
\anchor other_domains
<H1>Ad hoc Predicates for Other Domains</H1>
dnl The pointset powerset domains.
m4_category_pps
dnl The predicates for iterators and disjuncts for the powerset domain.
m4_iter_pps_categories($1)`'dnl
dnl Other adhoc predicates for the powerset domain.
m4_define(`m4_ext', pps_other)`'dnl
m4_one_subcategory($1)`'dnl
m4_pushdef(`m4_subcategory_widen', `')`'dnl
m4_define(`m4_ext', widen)`'dnl
m4_one_subcategory($1)`'dnl
m4_popdef(`m4_subcategory_widen')`'dnl
m4_finalize_category_pps`'dnl
')

m4_divert`'dnl
m4_ifdef(`m4_prolog_ci_documentation', `dnl
m4_define(`m4_this_interface_class', Polyhedron)`'dnl
m4_pushdef(`m4_a_class_replacements', `polyhedron')`'dnl
m4_pushdef(`m4_Polyhedron_topology_replacements', `C_')`'dnl
m4_pushdef(`m4_Polyhedron_a_topology_replacements', `C ')`'dnl
m4_pushdef(`m4_this_ppl_one_object', `polyhedron')`'dnl
m4_pushdef(`m4_this_ppl_objects', `polyhedra')`'dnl
m4_pushdef(`m4_this_ppl_group', `polyhedra')`'dnl
m4_pushdef(`m4_this_topology', `C')`'dnl
m4_pushdef(`m4_Polyhedron_friend_replacements',
  `C_Polyhedron, NNC_Polyhedron')`'dnl
m4_pushdef(`m4_Polyhedron_a_friend_replacements',
   `C polyhedron, NNC polyhedron')`'dnl
m4_patsubst(`m4_one_simple_class_code(1)', |COMMA|, `,')`'dnl
m4_popdef(`m4_Polyhedron_topology_replacements')`'dnl
m4_popdef(`m4_Polyhedron_a_topology_replacements')`'dnl
m4_popdef(`m4_this_ppl_objects')`'dnl
m4_popdef(`m4_this_topology')`'dnl
m4_pushdef(`m4_Pointset_Powerset_comparison_replacements',
         `geometrically_covers, geometrically_equals')`'dnl
m4_pushdef(`m4_Pointset_Powerset_a_comparison_replacements',
         `geometrically covers, geometrically equals')`'dnl
m4_patsubst(`m4_ci_pps_class_code(2)', |COMMA|, `,')`'dnl
')`'dnl
m4_divert(-1)

dnl -----------------------------------------------------------------
dnl CONFIGURATION DEPENDENT
dnl -----------------------------------------------------------------

dnl m4_all_classes_code(Class_Counter)
dnl
dnl Class_Counter   - The index for the current class.
dnl This iterates through the classes to define the documentation.
dnl Note that at the end of this stage, commas are still to be substituted.
m4_define(`m4_all_classes_doc', `dnl
dnl
dnl Check for class counter in range. If so, generate the documentation
dnl for the current class. If not finish.
m4_ifdef(m4_interface_class`'$1, `dnl
dnl
dnl First some temporary definitions.
m4_define(`m4_this_interface_class', m4_interface_class`'$1)`'dnl
m4_pushdef(`m4_a_class_replacement', `dnl
m4_PPL_domain2text(m4_this_interface_class)')
m4_pushdef(`m4_this_topology', `')`'dnl
m4_pushdef(`m4_this_ppl_group', m4_class_group`'$1)`'dnl
m4_pushdef(`m4_this_ppl_kind', m4_class_kind`'$1)`'dnl
m4_pushdef(`m4_this_ppl_disjunct_kind', m4_class_body_kind`'$1)`'dnl
m4_define(`m4_disjunct_topology', `dnl
m4_upcase(m4_get_class_topology(m4_cplusplus_class_body$1))')`'dnl
m4_pushdef(`m4_this_ppl_one_object', `dnl
m4_PPL_domain2text(m4_this_ppl_kind)')
m4_pushdef(`m4_this_ppl_objects', `dnl
m4_ifelse(m4_this_ppl_group, box,
   m4_plural(m4_PPL_domain2text(m4_this_interface_class)),
          m4_this_ppl_group, pointset_powerset,
   pointset powersets of m4_plural(m4_PPL_domain2text(m4_disjunct_topology m4_this_ppl_disjunct_kind)),
  m4_plural(m4_PPL_domain2text(m4_this_ppl_kind)))')`'dnl
dnl
dnl Generate documentation for current class.
m4_one_simple_class_code($1)`'dnl
m4_ifelse(m4_this_ppl_group,
  pointset_powerset, `m4_iter_pps_categories($1)')`'dnl
dnl
dnl Remove temporary definition.
m4_popdef(`m4_this_ppl_objects')`'dnl
m4_popdef(`m4_this_topology')`'dnl
m4_popdef(`m4_this_ppl_group')`'dnl
m4_popdef(`m4_this_ppl_kind')`'dnl
m4_popdef(`m4_this_ppl_disjunct_kind')`'dnl
m4_popdef(`m4_disjunct_topology')`'dnl
dnl
dnl Increment class counter and repeat.
m4_all_classes_doc(m4_incr($1))')`'dnl
')

dnl -----------------------------------------------------------------
dnl Generate all documentation for all the classes
dnl -----------------------------------------------------------------
m4_divert`'dnl
m4_ifdef(`m4_prolog_ci_documentation', `',
  `m4_patsubst(`m4_all_classes_doc(1)', |COMMA|, `,')`'dnl
')`'dnl
m4_divert(-1)

m4_divert`'dnl
*/ /* \page configured_domains_predicates */
dnl
dnl End of file generation.
