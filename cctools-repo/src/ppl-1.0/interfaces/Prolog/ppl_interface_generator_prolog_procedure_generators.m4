m4_divert(-1)

dnl This m4 file defines the list of predicates
dnl for the Prolog interfaces; this includes:
dnl - the list in the imported file and any Prolog specific predicates;
dnl   ppl_interface_generator_common_procedure_generators.m4;
dnl - the list of library predicates needed for the system dependent files.

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
m4_include(ppl_interface_generator_common.m4)
m4_include(ppl_interface_generator_common_dat.m4)
m4_include(ppl_interface_generator_common_procedure_generators.m4)

dnl Library predicate list.
m4_define(`m4_library_predicate_list', `dnl
ppl_version_major/1,
ppl_version_minor/1,
ppl_version_revision/1,
ppl_version_beta/1,
ppl_version/1,
ppl_banner/1,
ppl_max_space_dimension/1,
ppl_Coefficient_bits/1,
ppl_Coefficient_is_bounded/0,
ppl_Coefficient_max/1,
ppl_Coefficient_min/1,
ppl_initialize/0 *nofail,
ppl_finalize/0 *nofail,
ppl_set_rounding_for_PPL/0 *nofail,
ppl_restore_pre_PPL_rounding/0 *nofail,
ppl_irrational_precision/1,
ppl_set_irrational_precision/1,
ppl_set_timeout_exception_atom/1 *nofail,
ppl_timeout_exception_atom/1,
ppl_set_timeout/1 *nofail,
ppl_reset_timeout/0 *nofail,
ppl_set_deterministic_timeout/2 *nofail,
ppl_reset_deterministic_timeout/0 *nofail,
ppl_new_MIP_Problem_from_space_dimension/2,
ppl_new_MIP_Problem/5,
ppl_new_MIP_Problem_from_MIP_Problem/2,
ppl_MIP_Problem_swap/2 *nofail,
ppl_delete_MIP_Problem/1 *nofail,
ppl_MIP_Problem_space_dimension/2,
ppl_MIP_Problem_integer_space_dimensions/2,
ppl_MIP_Problem_constraints/2,
ppl_MIP_Problem_objective_function/2,
ppl_MIP_Problem_optimization_mode/2,
ppl_MIP_Problem_clear/1,
ppl_MIP_Problem_add_space_dimensions_and_embed/2,
ppl_MIP_Problem_add_to_integer_space_dimensions/2,
ppl_MIP_Problem_add_constraint/2,
ppl_MIP_Problem_add_constraints/2,
ppl_MIP_Problem_set_objective_function/2,
ppl_MIP_Problem_set_optimization_mode/2,
ppl_MIP_Problem_set_control_parameter/2 *nofail,
ppl_MIP_Problem_get_control_parameter/3,
ppl_MIP_Problem_is_satisfiable/1,
ppl_MIP_Problem_solve/2,
ppl_MIP_Problem_feasible_point/2,
ppl_MIP_Problem_optimizing_point/2,
ppl_MIP_Problem_optimal_value/3,
ppl_MIP_Problem_evaluate_objective_function/4,
ppl_MIP_Problem_OK/1
ppl_MIP_Problem_ascii_dump/1,
ppl_new_PIP_Problem_from_space_dimension/2,
ppl_new_PIP_Problem/4,
ppl_new_PIP_Problem_from_PIP_Problem/2,
ppl_PIP_Problem_swap/2 *nofail,
ppl_delete_PIP_Problem/1 *nofail,
ppl_PIP_Problem_space_dimension/2,
ppl_PIP_Problem_parameter_space_dimensions/2,
ppl_PIP_Problem_constraints/2,
ppl_PIP_Problem_clear/1,
ppl_PIP_Problem_add_space_dimensions_and_embed/3,
ppl_PIP_Problem_add_to_parameter_space_dimensions/2,
ppl_PIP_Problem_add_constraint/2,
ppl_PIP_Problem_add_constraints/2,
ppl_PIP_Problem_set_control_parameter/2 *nofail,
ppl_PIP_Problem_get_control_parameter/3,
ppl_PIP_Problem_has_big_parameter_dimension/2,
ppl_PIP_Problem_set_big_parameter_dimension/2,
ppl_PIP_Problem_is_satisfiable/1,
ppl_PIP_Problem_solve/2,
ppl_PIP_Problem_solution/2,
ppl_PIP_Problem_optimizing_solution/2,
ppl_PIP_Problem_OK/1,
ppl_PIP_Problem_ascii_dump/1,
ppl_PIP_Tree_Node_constraints/2,
ppl_PIP_Tree_Node_is_solution/1,
ppl_PIP_Tree_Node_is_decision/1,
ppl_PIP_Tree_Node_is_bottom/1,
ppl_PIP_Tree_Node_artificials/2,
ppl_PIP_Tree_Node_OK/1,
ppl_PIP_Tree_Node_parametric_values/3,
ppl_PIP_Tree_Node_true_child/2,
ppl_PIP_Tree_Node_false_child/2`'dnl
')

m4_define(`m4_procedure_list',
  `m4_echo_unquoted(`m4_common_procedure_list',
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@/1 +simple,
ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@/2 +simple,
ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@/2 +simple,
ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@/3 +simple,
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2/2 +simple,
ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2/3 +simple,
ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2/3 +simple,
ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2/4 +simple,
)
')
