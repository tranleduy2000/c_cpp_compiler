dnl  -*- Tuareg -*-
sm4_divert(-1)

This m4 file contains the program code for generating ppl_ocaml.ml

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

dnl No code is needed for these procedure schemas in the OCaml interface.
dnl
m4_define(`ppl_delete_@CLASS@_code', `')
m4_define(`ppl_delete_@CLASS@_iterator_code', `')

dnl There is no code at present for these procedures in the OCaml interface.
dnl Remove the macro if its definition is added.

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`dnl
external ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension:
  int -> degenerate_element -> @!CLASS@
  = "ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension"

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
`dnl
external ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s:
  @BUILD_REPRESENT@_system -> @!CLASS@
  = "ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s"

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
`dnl
external ppl_@CLASS@_relation_with_@RELATION_REPRESENT@:
  @!CLASS@ -> linear_@RELATION_REPRESENT@
  -> poly_@A_RELATION_REPRESENT@_relation list
  = "ppl_@CLASS@_relation_with_@RELATION_REPRESENT@"

')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
`dnl
external ppl_@CLASS@_@DIMENSION@:
	    @!CLASS@ -> int = "ppl_@CLASS@_@DIMENSION@"
')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
`dnl
external ppl_@CLASS@_@HAS_PROPERTY@:
  @!CLASS@ -> bool = "ppl_@CLASS@_@HAS_PROPERTY@"


')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
`dnl
external ppl_@CLASS@_@SIMPLIFY@:
  @!CLASS@ -> unit = "ppl_@CLASS@_@SIMPLIFY@"


')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
`dnl
external ppl_@CLASS@_bounds_from_@ABOVEBELOW@:
  @!CLASS@ -> linear_expression -> bool
  = "ppl_@CLASS@_bounds_from_@ABOVEBELOW@"

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
`dnl
external ppl_@CLASS@_add_@CLASS_REPRESENT@:
  @!CLASS@ -> linear_@CLASS_REPRESENT@ -> unit
  = "ppl_@CLASS@_add_@CLASS_REPRESENT@"

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
`dnl
external ppl_@CLASS@_add_@CLASS_REPRESENT@s:
  @!CLASS@ -> @CLASS_REPRESENT@_system -> unit
  = "ppl_@CLASS@_add_@CLASS_REPRESENT@s"

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
`dnl
external ppl_@CLASS@_refine_with_@REFINE_REPRESENT@:
  @!CLASS@ -> linear_@REFINE_REPRESENT@ -> unit
  = "ppl_@CLASS@_refine_with_@REFINE_REPRESENT@"

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
`dnl
external ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s:
  @!CLASS@ -> @REFINE_REPRESENT@_system -> unit
  = "ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s"

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
`dnl
external ppl_@CLASS@_@COMPARISON@_@CLASS@:
  @!CLASS@ -> @!CLASS@ -> bool
  = "ppl_@CLASS@_@COMPARISON@_@CLASS@"

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
`dnl
external ppl_@CLASS@_equals_@CLASS@:
  @!CLASS@ -> @!CLASS@ -> bool
  = "ppl_@CLASS@_equals_@CLASS@"

')


m4_define(`ppl_@CLASS@_@BINOP@_code',
`dnl
external ppl_@CLASS@_@BINOP@:
  @!CLASS@ -> @!CLASS@ -> unit
  = "ppl_@CLASS@_@BINOP@"

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
`dnl
external ppl_@CLASS@_simplify_using_context_assign:
  @!CLASS@ -> @!CLASS@ -> bool
  = "ppl_@CLASS@_simplify_using_context_assign"

')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
`dnl
external ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@:
  @!CLASS@ -> int -> unit
  = "ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@"

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
`dnl
external ppl_@CLASS@_remove_space_dimensions:
  @!CLASS@ -> int list -> unit
  = "ppl_@CLASS@_remove_space_dimensions"

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
`dnl
external ppl_@CLASS@_remove_higher_space_dimensions:
  @!CLASS@ -> int -> unit
  = "ppl_@CLASS@_remove_higher_space_dimensions"

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
`dnl
external ppl_@CLASS@_fold_space_dimensions:
  @!CLASS@ -> int list -> int -> unit
  = "ppl_@CLASS@_fold_space_dimensions"

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
`dnl
external ppl_@CLASS@_map_space_dimensions:
  @!CLASS@ -> (int*int) list -> unit
  = "ppl_@CLASS@_map_space_dimensions"

')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
`dnl
external ppl_@CLASS@_expand_space_dimension:
  @!CLASS@ -> int -> int -> unit
  = "ppl_@CLASS@_expand_space_dimension"

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
`dnl
external ppl_@CLASS@_drop_some_non_integer_points:
  @!CLASS@ -> complexity_class -> unit
  = "ppl_@CLASS@_drop_some_non_integer_points"

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
`dnl
external ppl_@CLASS@_drop_some_non_integer_points_2:
  @!CLASS@ -> int list -> complexity_class -> unit
  = "ppl_@CLASS@_drop_some_non_integer_points_2"

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
`dnl
external ppl_@CLASS@_get_@CLASS_REPRESENT@s:
  @!CLASS@ -> @CLASS_REPRESENT@_system
  = "ppl_@CLASS@_get_@CLASS_REPRESENT@s"

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
`dnl
external ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s:
  @!CLASS@ -> @CLASS_REPRESENT@_system
  = "ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s"

')

m4_define(`ppl_@CLASS@_constrains_code',
`dnl
external ppl_@CLASS@_constrains:
  @!CLASS@ -> int -> bool
  = "ppl_@CLASS@_constrains"

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
`dnl
external ppl_@CLASS@_unconstrain_space_dimension:
  @!CLASS@ -> int -> unit
  = "ppl_@CLASS@_unconstrain_space_dimension"

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
`dnl
external ppl_@CLASS@_unconstrain_space_dimensions:
  @!CLASS@ -> int list -> unit
  = "ppl_@CLASS@_unconstrain_space_dimensions"

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
`dnl
external ppl_@CLASS@_bounded_@AFFIMAGE@:
  @!CLASS@ -> int -> linear_expression
  -> linear_expression -> Z.t -> unit
  = "ppl_@CLASS@_bounded_@AFFIMAGE@"

')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
`dnl
external ppl_@CLASS@_@AFFIMAGE@:
  @!CLASS@ -> int -> linear_expression -> Z.t -> unit
  = "ppl_@CLASS@_@AFFIMAGE@"

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
`dnl
external ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs:
  @!CLASS@ -> linear_expression
  -> relation_symbol -> linear_expression -> unit
  = "ppl_@CLASS@_generalized_@AFFIMAGE@1"

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
`dnl
external ppl_@CLASS@_generalized_@AFFIMAGE@:
  @!CLASS@ -> int -> relation_symbol
  -> linear_expression -> Z.t -> unit
  = "ppl_@CLASS@_generalized_@AFFIMAGE@2"

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
`dnl
external ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence:
  @!CLASS@ -> int -> relation_symbol
  -> linear_expression -> Z.t -> Z.t -> unit
  = "ppl_@CLASS@_generalized_@AFFIMAGE@1_with_congruence_bytecode" "ppl_@CLASS@_generalized_@AFFIMAGE@1_with_congruence"

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
`dnl
external ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence:
  @!CLASS@ -> linear_expression -> relation_symbol
  -> linear_expression -> Z.t -> unit
  = "ppl_@CLASS@_generalized_@AFFIMAGE@1_lhs_rhs_with_congruence"

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
`dnl
external ppl_@CLASS@_@WIDEN@_widening_assign:
  @!CLASS@ -> @!CLASS@ -> unit
  = "ppl_@CLASS@_@WIDEN@_widening_assign"

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code',
`dnl
external ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens:
  @!CLASS@ -> @!CLASS@ -> int -> int
  = "ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens"

')

m4_define(`ppl_@CLASS@_widening_assign_code',
`dnl
external ppl_@CLASS@_widening_assign:
  @!CLASS@ -> @!CLASS@ -> unit
  = "ppl_@CLASS@_widening_assign"

')

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code',
`dnl
external ppl_@CLASS@_widening_assign_with_tokens:
  @!CLASS@ -> @!CLASS@ -> int -> int
  = "ppl_@CLASS@_widening_assign_with_tokens"

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
`dnl
external ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign:
  @!CLASS@ -> @!CLASS@ -> @CONSTRAINER@_system -> unit
  = "ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign"

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code',
`dnl
external ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens:
  @!CLASS@ -> @!CLASS@ -> @CONSTRAINER@_system -> int -> int
  = "ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens"

')


m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
`dnl
external ppl_@CLASS@_has_@UPPERLOWER@_bound:
  @!CLASS@ -> int
  -> bool * Z.t * Z.t * bool
  = "ppl_@CLASS@_has_@UPPERLOWER@_bound"

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
`dnl
external ppl_@CLASS@_@MAXMIN@:
  @!CLASS@ -> linear_expression
  -> bool * Z.t * Z.t * bool
  = "ppl_@CLASS@_@MAXMIN@"

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
`dnl
external ppl_@CLASS@_@MAXMIN@_with_point:
  @!CLASS@ -> linear_expression
  -> bool * Z.t * Z.t * bool * linear_generator
  = "ppl_@CLASS@_@MAXMIN@_with_point"

')

m4_define(`ppl_@CLASS@_frequency_code',
`dnl
external ppl_@CLASS@_frequency:
  @!CLASS@ -> linear_expression
  -> bool * Z.t * Z.t * Z.t * Z.t
  = "ppl_@CLASS@_frequency"

')

m4_define(`ppl_@CLASS@_OK_code',
`dnl
external ppl_@CLASS@_OK:
  @!CLASS@ -> bool = "ppl_@CLASS@_OK"

')


m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
 `
external ppl_@CLASS@_@MEMBYTES@:
  @!CLASS@  -> int = "ppl_@CLASS@_@MEMBYTES@"

')

m4_define(`ppl_@CLASS@_swap_code',
`dnl
external ppl_@CLASS@_swap:
  @!CLASS@ -> @!CLASS@ -> unit
  = "ppl_@CLASS@_swap"

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`dnl
external ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@:
  @!A_FRIEND@ -> @!CLASS@
  = "ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@"

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`dnl
external ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity:
  @!A_FRIEND@ -> complexity_class -> @!CLASS@
  = "ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@"

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
 `dnl
 external ppl_@CLASS@_@UB_EXACT@:
   @!CLASS@ -> @!CLASS@ -> bool
   = "ppl_@CLASS@_@UB_EXACT@"

 ')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code',
 `dnl
 external ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens:
   @!CLASS@ -> @!CLASS@ -> int -> int
   = "ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens"

 ')


m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
 `dnl
 external ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign:
   @!CLASS@ -> @!CLASS@ -> unit
   = "ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign"

 ')

 m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
 `dnl
 external ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign:
   @!CLASS@  -> @!CLASS@ -> unit
   = "ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign"

 ')

 m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
 `
external ppl_@CLASS@_@BEGINEND@_iterator:
  @!CLASS@  -> @!CLASS@_iterator
  = "ppl_@CLASS@_@BEGINEND@_iterator"

')

m4_define(`ppl_@CLASS@_get_disjunct_code',
 `
external ppl_@CLASS@_get_disjunct:
   @!CLASS@_iterator  -> @!DISJUNCT@
   = "ppl_@CLASS@_get_disjunct"

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
 `
external ppl_@CLASS@_add_disjunct:
   @!CLASS@  -> @!DISJUNCT@ -> unit
   = "ppl_@CLASS@_add_disjunct"

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
 `
 external ppl_@CLASS@_drop_disjunct:
   @!CLASS@  ->  @!CLASS@_iterator -> unit
   = "ppl_@CLASS@_drop_disjunct"

')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
 `
 external ppl_@CLASS@_drop_disjuncts:
   @!CLASS@  ->  @!CLASS@_iterator -> @!CLASS@_iterator -> unit
   = "ppl_@CLASS@_drop_disjuncts"

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`
type @!CLASS@_iterator

external ppl_new_@CLASS@_iterator_from_iterator:
  @!CLASS@_iterator -> @!CLASS@_iterator
  = "ppl_new_@CLASS@_iterator_from_iterator"

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
 `
 external ppl_@CLASS@_@INCDEC@_iterator:
   @!CLASS@_iterator -> unit
   = "ppl_@CLASS@_@INCDEC@_iterator"

')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
 `
external ppl_@CLASS@_iterator_equals_iterator:
  @!CLASS@_iterator ->  @!CLASS@_iterator -> bool
  = "ppl_@CLASS@_iterator_equals_iterator"

')

m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
`dnl
external ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign:
  @!CLASS@ -> @!CLASS@ -> unit
  = "ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign"

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
`dnl
external ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign:
  @!CLASS@ -> @!CLASS@ -> int -> unit
  = "ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign"

')

m4_define(`ppl_@CLASS@_ascii_dump_code',
`dnl
external ppl_@CLASS@_ascii_dump:
  @!CLASS@ -> string = "ppl_@CLASS@_ascii_dump"

')

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
`dnl
external ppl_@CLASS@_linear_@PARTITION@:
 @!CLASS@ -> @!CLASS@ -> @!CLASS@ * pointset_powerset_nnc_polyhedron
 = "ppl_@CLASS@_linear_partition"

')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code',
`dnl
external ppl_@CLASS@_approximate_@PARTITION@:
 @!CLASS@ -> @!CLASS@ ->
   @!CLASS@ * pointset_powerset_grid * bool
 = "ppl_@CLASS@_approximate_partition"

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
`dnl
external ppl_@CLASS@_@MAXMIN@:
  @!CLASS@ -> linear_expression
  -> bool * Z.t * Z.t * bool
  = "ppl_@CLASS@_@MAXMIN@"

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
external ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@:
  @!CLASS@ -> bool = "ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@"

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
external ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2:
  @!CLASS@ -> @!CLASS@ -> bool
  = "ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2"

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
external ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@:
  @!CLASS@ -> bool * linear_generator
  = "ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@"

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
external ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2:
  @!CLASS@ -> @!CLASS@ -> bool * linear_generator
  = "ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2"

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
external ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@:
  @!CLASS@ -> polyhedron
  = "ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@"

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
external ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2:
  @!CLASS@ -> @!CLASS@ -> polyhedron
  = "ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2"

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_code',
`dnl
external ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@:
  @!CLASS@ -> polyhedron * polyhedron
  = "ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@"

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2_code',
`dnl
external ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2:
  @!CLASS@ -> @!CLASS@ -> polyhedron * polyhedron
  = "ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2"

')

m4_define(`ppl_@CLASS@_wrap_assign_code',
`dnl
external ppl_@CLASS@_wrap_assign: @!CLASS@
                                  -> int list
                                  -> bounded_integer_type_width
                                  -> bounded_integer_type_representation
                                  -> bounded_integer_type_overflow
                                  -> constraint_system
                                  -> int -> int -> unit
                        = "ppl_@CLASS@_wrap_assign_bytecode"
                          "ppl_@CLASS@_wrap_assign_native"

')
