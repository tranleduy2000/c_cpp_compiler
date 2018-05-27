/* Prolog main program for the toy PPL/CIAO-Prolog predicate checker.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

This file is part of the Parma Polyhedra Library (PPL).

The PPL is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The PPL is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software Foundation,
Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.

For the most up-to-date information see the Parma Polyhedra Library
site: http://bugseng.com/products/ppl/ . */

:- module(_, [main/0], []).
:- use_module(library(debugger)).
:- use_module(library(dynamic)).
:- use_module(library(lists)).
:- use_module(library(prolog_sys)).
:- use_module(library(read)).
:- use_module(library(write)).
:- use_module(ppl_ciao, [
	ppl_version_major/1,
	ppl_version_minor/1,
	ppl_version_revision/1,
	ppl_version_beta/1,
	ppl_version/1,
	ppl_banner/1,
	ppl_max_space_dimension/1,
	ppl_Coefficient_is_bounded/0,
	ppl_Coefficient_max/1,
	ppl_Coefficient_min/1,
	ppl_initialize/0,
	ppl_finalize/0,
	ppl_set_timeout_exception_atom/1,
	ppl_timeout_exception_atom/1,
	ppl_set_timeout/1,
	ppl_reset_timeout/0,
        ppl_set_rounding_for_PPL/0,
        ppl_restore_pre_PPL_rounding/0,
	ppl_new_C_Polyhedron_from_space_dimension/3,
	ppl_new_NNC_Polyhedron_from_space_dimension/3,
	ppl_new_C_Polyhedron_from_C_Polyhedron/2,
        ppl_new_C_Polyhedron_from_C_Polyhedron_with_complexity/3,
        ppl_new_C_Polyhedron_from_NNC_Polyhedron/2,
        ppl_new_C_Polyhedron_from_NNC_Polyhedron_with_complexity/3,
	ppl_new_NNC_Polyhedron_from_C_Polyhedron/2,
        ppl_new_NNC_Polyhedron_from_C_Polyhedron_with_complexity/3,
	ppl_new_NNC_Polyhedron_from_NNC_Polyhedron/2,
        ppl_new_NNC_Polyhedron_from_NNC_Polyhedron_with_complexity/3,
	ppl_new_C_Polyhedron_from_constraints/2,
	ppl_new_NNC_Polyhedron_from_constraints/2,
	ppl_new_C_Polyhedron_from_generators/2,
	ppl_new_NNC_Polyhedron_from_generators/2,
	ppl_Polyhedron_swap/2,
	ppl_delete_Polyhedron/1,
	ppl_Polyhedron_space_dimension/2,
	ppl_Polyhedron_affine_dimension/2,
	ppl_Polyhedron_get_constraints/2,
	ppl_Polyhedron_get_minimized_constraints/2,
	ppl_Polyhedron_get_generators/2,
	ppl_Polyhedron_get_minimized_generators/2,
	ppl_Polyhedron_relation_with_constraint/3,
	ppl_Polyhedron_relation_with_generator/3,
	ppl_Polyhedron_is_empty/1,
	ppl_Polyhedron_is_universe/1,
	ppl_Polyhedron_is_bounded/1,
	ppl_Polyhedron_contains_integer_point/1,
	ppl_Polyhedron_bounds_from_above/2,
	ppl_Polyhedron_bounds_from_below/2,
	ppl_Polyhedron_maximize/5,
	ppl_Polyhedron_maximize_with_point/6,
	ppl_Polyhedron_minimize/5,
	ppl_Polyhedron_minimize_with_point/6,
        ppl_Polyhedron_is_topologically_closed/1,
	ppl_Polyhedron_contains_Polyhedron/2,
	ppl_Polyhedron_strictly_contains_Polyhedron/2,
	ppl_Polyhedron_is_disjoint_from_Polyhedron/2,
        ppl_Polyhedron_equals_Polyhedron/2,
	ppl_Polyhedron_OK/1,
	ppl_Polyhedron_add_constraint/2,
	ppl_Polyhedron_add_generator/2,
	ppl_Polyhedron_add_constraints/2,
	ppl_Polyhedron_add_generators/2,
	ppl_Polyhedron_intersection_assign/2,
	ppl_Polyhedron_poly_hull_assign/2,
	ppl_Polyhedron_poly_difference_assign/2,
	ppl_Polyhedron_affine_image/4,
	ppl_Polyhedron_affine_preimage/4,
	ppl_Polyhedron_bounded_affine_image/5,
	ppl_Polyhedron_bounded_affine_preimage/5,
	ppl_Polyhedron_generalized_affine_image/5,
	ppl_Polyhedron_generalized_affine_preimage/5,
	ppl_Polyhedron_generalized_affine_image_lhs_rhs/4,
	ppl_Polyhedron_generalized_affine_preimage_lhs_rhs/4,
	ppl_Polyhedron_time_elapse_assign/2,
        ppl_Polyhedron_topological_closure_assign/1,
        ppl_Polyhedron_constrains/2,
        ppl_Polyhedron_unconstrain_space_dimension/2,
        ppl_Polyhedron_unconstrain_space_dimensions/2,
	ppl_Polyhedron_BHRZ03_widening_assign_with_tokens/4,
	ppl_Polyhedron_BHRZ03_widening_assign/2,
	ppl_Polyhedron_limited_BHRZ03_extrapolation_assign_with_tokens/5,
	ppl_Polyhedron_limited_BHRZ03_extrapolation_assign/3,
	ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign_with_tokens/5,
	ppl_Polyhedron_bounded_BHRZ03_extrapolation_assign/3,
	ppl_Polyhedron_H79_widening_assign_with_tokens/4,
	ppl_Polyhedron_H79_widening_assign/2,
	ppl_Polyhedron_limited_H79_extrapolation_assign_with_tokens/5,
	ppl_Polyhedron_limited_H79_extrapolation_assign/3,
	ppl_Polyhedron_bounded_H79_extrapolation_assign_with_tokens/5,
	ppl_Polyhedron_bounded_H79_extrapolation_assign/3,
	ppl_Polyhedron_add_space_dimensions_and_project/2,
	ppl_Polyhedron_add_space_dimensions_and_embed/2,
	ppl_Polyhedron_concatenate_assign/2,
	ppl_Polyhedron_remove_space_dimensions/2,
	ppl_Polyhedron_remove_higher_space_dimensions/2,
	ppl_Polyhedron_expand_space_dimension/3,
	ppl_Polyhedron_fold_space_dimensions/3,
	ppl_Polyhedron_map_space_dimensions/2,
        ppl_new_MIP_Problem_from_space_dimension/2,
        ppl_new_MIP_Problem/5,
        ppl_new_MIP_Problem_from_MIP_Problem/2,
        ppl_MIP_Problem_swap/2,
        ppl_delete_MIP_Problem/1,
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
        ppl_MIP_Problem_is_satisfiable/1,
        ppl_MIP_Problem_solve/2,
        ppl_MIP_Problem_feasible_point/2,
        ppl_MIP_Problem_optimizing_point/2,
        ppl_MIP_Problem_optimal_value/3,
        ppl_MIP_Problem_evaluate_objective_function/4,
        ppl_MIP_Problem_OK/1,
        ppl_MIP_Problem_get_control_parameter/3,
        ppl_MIP_Problem_set_control_parameter/2
]).

:- set_prolog_flag(multi_arity_warnings, off).

:- include('pl_check.pl').

prolog_system('Ciao').

main:-
   (check_all ->
        write('OK')
   ;
        write('FAILURE')
   ),
   nl.
