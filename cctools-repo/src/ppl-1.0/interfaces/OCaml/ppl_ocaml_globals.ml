(* OCaml interface: domain-independent functions.
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
site: http://bugseng.com/products/ppl/ . *)

open Gmp

exception PPL_arithmetic_overflow of string
let _ = Callback.register_exception "PPL_arithmetic_overflow"
          (PPL_arithmetic_overflow "any string")

exception PPL_timeout_exception
let _ = Callback.register_exception "PPL_timeout_exception"
          (PPL_timeout_exception)

exception PPL_internal_error of string
let _ = Callback.register_exception "PPL_internal_error"
          (PPL_internal_error "any string")

exception PPL_unknown_standard_exception of string
let _ = Callback.register_exception "PPL_unknown_standard_exception"
          (PPL_unknown_standard_exception "any string")

exception PPL_unexpected_error of string
let _ = Callback.register_exception "PPL_unexpected_error"
          (PPL_unexpected_error "any string")

type degenerate_element =
    Universe
  | Empty

type linear_expression =
    Variable of int
  | Coefficient of Z.t
  | Unary_Plus of linear_expression
  | Unary_Minus of linear_expression
  | Plus of linear_expression * linear_expression
  | Minus of linear_expression * linear_expression
  | Times of Z.t * linear_expression

type linear_constraint =
    Less_Than of linear_expression * linear_expression
  | Less_Or_Equal of linear_expression * linear_expression
  | Equal of linear_expression * linear_expression
  | Greater_Than of linear_expression * linear_expression
  | Greater_Or_Equal of linear_expression * linear_expression

type linear_generator =
    Line of linear_expression
  | Ray of linear_expression
  | Point of linear_expression * Z.t
  | Closure_Point of linear_expression * Z.t

type linear_grid_generator =
    Grid_Line of linear_expression
  | Grid_Parameter of linear_expression * Z.t
  | Grid_Point of linear_expression * Z.t

type poly_gen_relation =
    Subsumes

type poly_con_relation =
    Is_Disjoint
  | Strictly_Intersects
  | Is_Included
  | Saturates

type relation_with_congruence =
    Is_Disjoint
  | Strictly_Intersects
  | Is_Included

type linear_congruence = linear_expression * linear_expression * Z.t

type constraint_system = linear_constraint list

type generator_system = linear_generator list

type grid_generator_system = linear_grid_generator list

type congruence_system = linear_congruence list

(* Note: the "_RS" suffix is to avoid name clashes with the declaration
   of linear_constraint. *)
type relation_symbol = Less_Than_RS | Less_Or_Equal_RS | Equal_RS
                       | Greater_Than_RS | Greater_Or_Equal_RS

type bounded_integer_type_overflow = Overflow_Wraps
                                     | Overflow_Undefined
                                     | Overflow_Impossible

type bounded_integer_type_representation = Unsigned
                                           | Signed_2_Complement

type bounded_integer_type_width = Bits_8
                                  | Bits_16
                                  | Bits_32
                                  | Bits_64
                                  | Bits_128

type complexity_class = Polynomial_Complexity
                        | Simplex_Complexity
                        | Any_Complexity

type optimization_mode = Minimization | Maximization

type mip_problem_status = Unfeasible_Mip_Problem | Unbounded_Mip_Problem
                        | Optimized_Mip_Problem

type control_parameter_name = Pricing

type control_parameter_value = Pricing_Steepest_Edge_Float
                             | Pricing_Steepest_Edge_Exact
                             | Pricing_Textbook

type mip_problem

external ppl_version_major:
unit -> int = "ppl_version_major"

external ppl_version_minor:
unit -> int = "ppl_version_minor"

external ppl_version_revision:
unit -> int = "ppl_version_revision"

external ppl_version_beta:
unit -> int = "ppl_version_beta"

external ppl_version:
unit -> string = "ppl_version"

external ppl_banner:
unit -> string = "ppl_banner"

external ppl_io_wrap_string:
string -> int -> int -> int -> string = "ppl_io_wrap_string"

external ppl_max_space_dimension:
unit -> int = "ppl_max_space_dimension"

external ppl_Coefficient_bits:
unit -> int = "ppl_Coefficient_bits"

external ppl_Coefficient_is_bounded:
unit -> bool = "ppl_Coefficient_is_bounded"

external ppl_Coefficient_max:
unit -> Z.t = "ppl_Coefficient_max"

external ppl_Coefficient_min:
unit -> Z.t = "ppl_Coefficient_min"

external ppl_Linear_Expression_is_zero:
linear_expression -> bool = "ppl_Linear_Expression_is_zero"

external ppl_Linear_Expression_all_homogeneous_terms_are_zero:
linear_expression -> bool
      = "ppl_Linear_Expression_all_homogeneous_terms_are_zero"

external ppl_set_rounding_for_PPL:
unit -> unit = "ppl_set_rounding_for_PPL"

external ppl_restore_pre_PPL_rounding:
unit -> unit = "ppl_restore_pre_PPL_rounding"

external ppl_irrational_precision:
unit -> int = "ppl_irrational_precision"

external ppl_set_irrational_precision:
int -> unit = "ppl_set_irrational_precision"

external ppl_set_timeout:
int -> unit = "ppl_set_timeout"

external ppl_reset_timeout:
unit -> unit = "ppl_reset_timeout"

external ppl_set_deterministic_timeout:
int -> int -> unit = "ppl_set_deterministic_timeout"

external ppl_reset_deterministic_timeout:
unit -> unit = "ppl_reset_deterministic_timeout"

external ppl_new_MIP_Problem_from_space_dimension:
  int -> mip_problem = "ppl_new_MIP_Problem_from_space_dimension"

external ppl_new_MIP_Problem:
  int -> constraint_system -> linear_expression
    -> optimization_mode -> mip_problem
      = "ppl_new_MIP_Problem"

external ppl_MIP_Problem_space_dimension:
  mip_problem -> int = "ppl_MIP_Problem_space_dimension"

external ppl_MIP_Problem_integer_space_dimensions:
  mip_problem -> int list = "ppl_MIP_Problem_integer_space_dimensions"

external ppl_MIP_Problem_constraints:
  mip_problem -> constraint_system = "ppl_MIP_Problem_constraints"

external ppl_MIP_Problem_add_space_dimensions_and_embed:
  mip_problem -> int -> unit
      = "ppl_MIP_Problem_add_space_dimensions_and_embed"

external ppl_MIP_Problem_add_to_integer_space_dimensions:
  mip_problem -> int list -> unit
      = "ppl_MIP_Problem_add_to_integer_space_dimensions"

external ppl_MIP_Problem_add_constraint:
  mip_problem -> linear_constraint -> unit
      = "ppl_MIP_Problem_add_constraint"

external ppl_MIP_Problem_add_constraints:
  mip_problem -> constraint_system -> unit
      = "ppl_MIP_Problem_add_constraints"

external ppl_MIP_Problem_set_objective_function:
  mip_problem -> linear_expression -> unit
      = "ppl_MIP_Problem_set_objective_function"

external ppl_MIP_Problem_is_satisfiable:
  mip_problem -> bool
      = "ppl_MIP_Problem_is_satisfiable"

external ppl_MIP_Problem_solve:
  mip_problem -> mip_problem_status
      = "ppl_MIP_Problem_solve"

external ppl_MIP_Problem_optimization_mode:
  mip_problem -> optimization_mode
      = "ppl_MIP_Problem_optimization_mode"

external ppl_MIP_Problem_feasible_point:
  mip_problem -> linear_generator
      = "ppl_MIP_Problem_feasible_point"

external ppl_MIP_Problem_optimizing_point:
  mip_problem -> linear_generator
      = "ppl_MIP_Problem_optimizing_point"

external ppl_MIP_Problem_objective_function:
  mip_problem -> linear_expression
      = "ppl_MIP_Problem_objective_function"

external ppl_MIP_Problem_optimal_value:
  mip_problem -> Z.t * Z.t
      = "ppl_MIP_Problem_optimal_value"

external ppl_MIP_Problem_evaluate_objective_function:
  mip_problem -> linear_generator  -> Z.t * Z.t
      = "ppl_MIP_Problem_evaluate_objective_function"

external ppl_MIP_Problem_OK:
  mip_problem -> bool
      = "ppl_MIP_Problem_OK"

external ppl_MIP_Problem_clear:
  mip_problem -> unit
      = "ppl_MIP_Problem_clear"

external ppl_MIP_Problem_set_optimization_mode:
  mip_problem -> optimization_mode -> unit
      = "ppl_MIP_Problem_set_optimization_mode"

external ppl_MIP_Problem_set_control_parameter:
  mip_problem -> control_parameter_value -> unit
      = "ppl_MIP_Problem_set_control_parameter"

external ppl_MIP_Problem_get_control_parameter:
  mip_problem -> control_parameter_name -> control_parameter_value
      = "ppl_MIP_Problem_get_control_parameter"

external ppl_MIP_Problem_swap:
  mip_problem -> mip_problem -> unit
      = "ppl_MIP_Problem_swap"

external ppl_MIP_Problem_ascii_dump:
  mip_problem -> string
      = "ppl_MIP_Problem_ascii_dump"


type pip_problem_status = Unfeasible_Pip_Problem
                        | Optimized_Pip_Problem

type pip_problem_control_parameter_name = Cutting_Strategy | Pivot_Row_Strategy

type pip_problem_control_parameter_value = Cutting_Strategy_First
                                         | Cutting_Strategy_Deepest
                                         | Cutting_Strategy_All
                                         | Pivot_Row_Strategy_First
                                         | Pivot_Row_Strategy_Max_Column

type artificial_parameter = linear_expression * Z.t

type pip_tree_node

type pip_problem

external ppl_new_PIP_Problem_from_space_dimension:
  int -> pip_problem = "ppl_new_PIP_Problem_from_space_dimension"

external ppl_new_PIP_Problem:
  int -> constraint_system -> int list -> pip_problem
    = "ppl_new_PIP_Problem"

external ppl_PIP_Problem_space_dimension:
  pip_problem -> int = "ppl_PIP_Problem_space_dimension"

external ppl_PIP_Problem_parameter_space_dimensions:
  pip_problem -> int list = "ppl_PIP_Problem_parameter_space_dimensions"

external ppl_PIP_Problem_constraints:
  pip_problem -> constraint_system = "ppl_PIP_Problem_constraints"

external ppl_PIP_Problem_add_space_dimensions_and_embed:
  pip_problem -> int -> int -> unit
      = "ppl_PIP_Problem_add_space_dimensions_and_embed"

external ppl_PIP_Problem_add_to_parameter_space_dimensions:
  pip_problem -> int list -> unit
      = "ppl_PIP_Problem_add_to_parameter_space_dimensions"

external ppl_PIP_Problem_add_constraint:
  pip_problem -> linear_constraint -> unit
      = "ppl_PIP_Problem_add_constraint"

external ppl_PIP_Problem_add_constraints:
  pip_problem -> constraint_system -> unit
      = "ppl_PIP_Problem_add_constraints"

external ppl_PIP_Problem_is_satisfiable:
  pip_problem -> bool
      = "ppl_PIP_Problem_is_satisfiable"

external ppl_PIP_Problem_solve:
  pip_problem -> pip_problem_status
      = "ppl_PIP_Problem_solve"

external ppl_PIP_Problem_solution:
  pip_problem -> pip_tree_node
      = "ppl_PIP_Problem_solution"

external ppl_PIP_Problem_optimizing_solution:
  pip_problem -> pip_tree_node
      = "ppl_PIP_Problem_optimizing_solution"

external ppl_PIP_Problem_get_big_parameter_dimension:
  pip_problem -> int = "ppl_PIP_Problem_get_big_parameter_dimension"

external ppl_PIP_Problem_has_big_parameter_dimension:
  pip_problem -> bool = "ppl_PIP_Problem_has_big_parameter_dimension"

external ppl_PIP_Problem_set_big_parameter_dimension:
  pip_problem -> int -> unit = "ppl_PIP_Problem_set_big_parameter_dimension"

external ppl_PIP_Problem_OK:
  pip_problem -> bool
      = "ppl_PIP_Problem_OK"

external ppl_PIP_Problem_clear:
  pip_problem -> unit
      = "ppl_PIP_Problem_clear"

external ppl_PIP_Problem_set_control_parameter:
  pip_problem -> pip_problem_control_parameter_value -> unit
      = "ppl_PIP_Problem_set_control_parameter"

external ppl_PIP_Problem_get_control_parameter:
  pip_problem -> pip_problem_control_parameter_name
              -> pip_problem_control_parameter_value
      = "ppl_PIP_Problem_get_control_parameter"

external ppl_PIP_Problem_swap:
  pip_problem -> pip_problem -> unit
      = "ppl_PIP_Problem_swap"

external ppl_PIP_Problem_ascii_dump:
  pip_problem -> string
      = "ppl_PIP_Problem_ascii_dump"

external ppl_PIP_Tree_Node_constraints:
  pip_tree_node -> constraint_system
      = "ppl_PIP_Tree_Node_constraints"

external ppl_PIP_Tree_Node_artificials:
  pip_tree_node -> artificial_parameter list
      = "ppl_PIP_Tree_Node_artificials"

external ppl_PIP_Tree_Node_OK:
  pip_tree_node -> bool
      = "ppl_PIP_Tree_Node_OK"

external ppl_PIP_Tree_Node_ascii_dump:
  pip_tree_node -> string
      = "ppl_PIP_Tree_Node_ascii_dump"

external ppl_PIP_Tree_Node_is_bottom:
  pip_tree_node -> bool
      = "ppl_PIP_Tree_Node_is_bottom"

external ppl_PIP_Tree_Node_is_solution:
  pip_tree_node -> bool
      = "ppl_PIP_Tree_Node_is_solution"

external ppl_PIP_Tree_Node_parametric_values:
  pip_tree_node -> int -> linear_expression
      = "ppl_PIP_Tree_Node_parametric_values"

external ppl_PIP_Tree_Node_is_decision:
  pip_tree_node -> bool
      = "ppl_PIP_Tree_Node_is_decision"

external ppl_PIP_Tree_Node_true_child:
  pip_tree_node -> pip_tree_node
      = "ppl_PIP_Tree_Node_true_child"

external ppl_PIP_Tree_Node_false_child:
  pip_tree_node -> pip_tree_node
      = "ppl_PIP_Tree_Node_false_child"
