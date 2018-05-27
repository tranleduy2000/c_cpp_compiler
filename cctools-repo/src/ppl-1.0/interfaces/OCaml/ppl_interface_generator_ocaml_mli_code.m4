dnl  -*- Tuareg -*-
m4_divert(-1)

This m4 file contains the program code for generating ppl_ocaml.mli

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

Define here as empty any known schematic method macros for which
the definition is not yet implemented.

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`dnl
val ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension:
  int -> degenerate_element -> @!CLASS@

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
`dnl
val ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s:
  @BUILD_REPRESENT@_system -> @!CLASS@

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
`dnl
val ppl_@CLASS@_relation_with_@RELATION_REPRESENT@:
  @!CLASS@ -> linear_@RELATION_REPRESENT@
  -> poly_@A_RELATION_REPRESENT@_relation list

')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
`dnl
val ppl_@CLASS@_@DIMENSION@:
  @!CLASS@ -> int
')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
`dnl
val ppl_@CLASS@_@HAS_PROPERTY@:
  @!CLASS@ -> bool

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
`dnl
val ppl_@CLASS@_@SIMPLIFY@:
  @!CLASS@ -> unit

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
`dnl
val ppl_@CLASS@_bounds_from_@ABOVEBELOW@:
  @!CLASS@ -> linear_expression -> bool

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
`dnl
val ppl_@CLASS@_add_@CLASS_REPRESENT@:
  @!CLASS@ -> linear_@CLASS_REPRESENT@ -> unit

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
`dnl
val ppl_@CLASS@_add_@CLASS_REPRESENT@s:
  @!CLASS@ -> @CLASS_REPRESENT@_system -> unit

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
`dnl
val ppl_@CLASS@_refine_with_@REFINE_REPRESENT@:
  @!CLASS@ -> linear_@REFINE_REPRESENT@ -> unit

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
`dnl
val ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s:
  @!CLASS@ -> @REFINE_REPRESENT@_system -> unit

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
`dnl
val ppl_@CLASS@_@COMPARISON@_@CLASS@:
  @!CLASS@ -> @!CLASS@ -> bool

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
`dnl
val ppl_@CLASS@_equals_@CLASS@:
  @!CLASS@ -> @!CLASS@ -> bool

')


m4_define(`ppl_@CLASS@_@BINOP@_code',
`dnl
val ppl_@CLASS@_@BINOP@:
   @!CLASS@ -> @!CLASS@ -> unit

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
`dnl
val ppl_@CLASS@_simplify_using_context_assign:
  @!CLASS@ -> @!CLASS@ -> bool

')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
`dnl
val ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@:
  @!CLASS@ -> int -> unit

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
`dnl
val ppl_@CLASS@_remove_space_dimensions:
  @!CLASS@ -> int list -> unit

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
`dnl
val ppl_@CLASS@_remove_higher_space_dimensions:
  @!CLASS@ -> int -> unit

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
`dnl
val ppl_@CLASS@_fold_space_dimensions:
  @!CLASS@ -> int list -> int -> unit

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
`dnl
val ppl_@CLASS@_map_space_dimensions:
  @!CLASS@ -> (int*int) list -> unit
')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
`dnl
val ppl_@CLASS@_expand_space_dimension:
  @!CLASS@ -> int -> int -> unit
')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
`dnl
val ppl_@CLASS@_drop_some_non_integer_points:
  @!CLASS@ -> complexity_class -> unit

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
`dnl
val ppl_@CLASS@_drop_some_non_integer_points_2:
  @!CLASS@ -> int list -> complexity_class -> unit

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
`dnl
val ppl_@CLASS@_get_@CLASS_REPRESENT@s:
  @!CLASS@ -> @CLASS_REPRESENT@_system

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
`dnl
val ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s:
  @!CLASS@ -> @CLASS_REPRESENT@_system

')

m4_define(`ppl_@CLASS@_constrains_code',
`dnl
val ppl_@CLASS@_constrains:
  @!CLASS@ -> int -> bool

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
`dnl
val ppl_@CLASS@_unconstrain_space_dimension:
  @!CLASS@ -> int -> unit

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
`dnl
val ppl_@CLASS@_unconstrain_space_dimensions:
  @!CLASS@ -> int list -> unit

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
`dnl
val ppl_@CLASS@_bounded_@AFFIMAGE@:
  @!CLASS@ -> int
  -> linear_expression -> linear_expression
  -> Z.t -> unit
')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
`dnl
val ppl_@CLASS@_@AFFIMAGE@:
  @!CLASS@ -> int -> linear_expression -> Z.t -> unit

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
`dnl
val ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs:
  @!CLASS@ -> linear_expression
  -> relation_symbol -> linear_expression
  -> unit

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
`dnl
val ppl_@CLASS@_generalized_@AFFIMAGE@:
  @!CLASS@ -> int -> relation_symbol -> linear_expression
  -> Z.t -> unit

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
`dnl
val ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence:
  @!CLASS@ -> int -> relation_symbol -> linear_expression
  -> Z.t -> Z.t -> unit

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
`dnl
val ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence:
  @!CLASS@ -> linear_expression
  -> relation_symbol -> linear_expression -> Z.t
  -> unit

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
`dnl
val ppl_@CLASS@_@WIDEN@_widening_assign:
  @!CLASS@ -> @!CLASS@ -> unit

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code',
`dnl
val ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens:
  @!CLASS@ -> @!CLASS@ -> int -> int

')

m4_define(`ppl_@CLASS@_widening_assign_code',
`dnl
val ppl_@CLASS@_widening_assign:
  @!CLASS@ -> @!CLASS@ -> unit

')

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code',
`dnl
val ppl_@CLASS@_widening_assign_with_tokens:
  @!CLASS@ -> @!CLASS@ -> int -> int

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code',
`dnl
val ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens:
  @!CLASS@ -> @!CLASS@
  -> @CONSTRAINER@_system -> int -> int

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
`dnl
val ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign:
  @!CLASS@ -> @!CLASS@
  -> @CONSTRAINER@_system -> unit

')

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
`dnl
val ppl_@CLASS@_has_@UPPERLOWER@_bound:
  @!CLASS@ -> int
  -> bool * Z.t * Z.t * bool

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
`dnl
val ppl_@CLASS@_@MAXMIN@:
  @!CLASS@ -> linear_expression
  -> bool * Z.t * Z.t * bool

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
`dnl
val ppl_@CLASS@_@MAXMIN@_with_point:
  @!CLASS@ -> linear_expression
  -> bool * Z.t * Z.t * bool * linear_generator

')

m4_define(`ppl_@CLASS@_frequency_code',
`dnl
val ppl_@CLASS@_frequency:
  @!CLASS@ -> linear_expression
  -> bool * Z.t * Z.t * Z.t * Z.t

')

m4_define(`ppl_@CLASS@_OK_code',
`dnl
val ppl_@CLASS@_OK:
  @!CLASS@ -> bool
')

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
`dnl
val ppl_@CLASS@_@MEMBYTES@:
  @!CLASS@  -> int

')


m4_define(`ppl_@CLASS@_swap_code',
`dnl
val ppl_@CLASS@_swap:
  @!CLASS@ -> @!CLASS@ -> unit

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`dnl
val ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@:
  @!A_FRIEND@ -> @!CLASS@

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`dnl
val ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity:
  @!A_FRIEND@ -> complexity_class -> @!CLASS@

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
`dnl
val ppl_@CLASS@_@UB_EXACT@:
  @!CLASS@ -> @!CLASS@ -> bool

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
`dnl
val ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign:
  @!CLASS@  -> @!CLASS@ -> unit

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code',
`dnl
val ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens:
  @!CLASS@  -> @!CLASS@ -> int -> int

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
`dnl
val ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign:
  @!CLASS@  -> @!CLASS@ -> unit

')

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
`dnl
val ppl_@CLASS@_@BEGINEND@_iterator:
  @!CLASS@  -> @!CLASS@_iterator

')

m4_define(`ppl_@CLASS@_get_disjunct_code',
`dnl
val ppl_@CLASS@_get_disjunct:
  @!CLASS@_iterator  ->  @!DISJUNCT@

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
`dnl
val ppl_@CLASS@_add_disjunct:
  @!CLASS@  ->  @!DISJUNCT@ -> unit

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
`dnl
val ppl_@CLASS@_drop_disjunct:
  @!CLASS@  ->  @!CLASS@_iterator -> unit

')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
`dnl
val ppl_@CLASS@_drop_disjuncts:
  @!CLASS@  ->  @!CLASS@_iterator -> @!CLASS@_iterator -> unit

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`dnl
type @!CLASS@_iterator

val ppl_new_@CLASS@_iterator_from_iterator:
  @!CLASS@_iterator ->  @!CLASS@_iterator

')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
`dnl

val ppl_@CLASS@_iterator_equals_iterator:
  @!CLASS@_iterator ->  @!CLASS@_iterator -> bool

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
`dnl
val ppl_@CLASS@_@INCDEC@_iterator:
  @!CLASS@_iterator -> unit

')

m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
`dnl
val ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign:
  @!CLASS@ -> @!CLASS@ -> unit

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
`dnl
val ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign:
  @!CLASS@ -> @!CLASS@ -> int -> unit

')

m4_define(`ppl_@CLASS@_ascii_dump_code',
`dnl

val ppl_@CLASS@_ascii_dump:
  @!CLASS@ -> string

')

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
`dnl
val ppl_@CLASS@_linear_@PARTITION@:
  @!CLASS@ -> @!CLASS@ -> @!CLASS@ * pointset_powerset_nnc_polyhedron

')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code',
`dnl
val ppl_@CLASS@_approximate_@PARTITION@:
  @!CLASS@ -> @!CLASS@ ->
    @!CLASS@ * pointset_powerset_grid * bool

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
val ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@:
  @!CLASS@ -> bool

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
val ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2:
  @!CLASS@ -> @!CLASS@ -> bool

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
val ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@:
  @!CLASS@ -> bool * linear_generator

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
val ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2:
  @!CLASS@ -> @!CLASS@ -> bool * linear_generator

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
val ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@:
  @!CLASS@ -> polyhedron

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
val ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2:
  @!CLASS@ -> @!CLASS@ -> polyhedron

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_code',
`dnl
val ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@:
  @!CLASS@ -> polyhedron * polyhedron

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2_code',
`dnl
val ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2:
  @!CLASS@ -> @!CLASS@ -> polyhedron * polyhedron

')

m4_define(`ppl_@CLASS@_wrap_assign_code',
`dnl
val ppl_@CLASS@_wrap_assign: @!CLASS@
                             -> int list
                             -> bounded_integer_type_width
                             -> bounded_integer_type_representation
                             -> bounded_integer_type_overflow
                             -> constraint_system
                             -> int -> int -> unit

')
