dnl  -*- C -*-
m4_divert(-1)

This m4 file contains the program header code for generating the
files ppl_c_domains.h.

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

dnl No code is needed for these procedure schemas in the C interface.
dnl
m4_define(`ppl_@CLASS@_swap_code', `')
m4_define(`ppl_@CLASS@_ascii_dump_code', `')

dnl There is no code at present for these procedures in the C interface.
dnl Remove the macro if its definition is added.
dnl

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
PPL_PROTO((ppl_const_@CLASS@_t pset));

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
PPL_PROTO((ppl_const_@CLASS@_t pset_before, ppl_const_@CLASS@_t pset_after));

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
PPL_PROTO((ppl_const_@CLASS@_t pset,
           ppl_Generator_t point));

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
PPL_PROTO((ppl_const_@CLASS@_t pset_before,
           ppl_const_@CLASS@_t pset_after,
           ppl_Generator_t point));

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
PPL_PROTO((ppl_const_@CLASS@_t pset,
           ppl_Polyhedron_t ph));

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
PPL_PROTO((ppl_const_@CLASS@_t pset_before,
           ppl_const_@CLASS@_t pset_after,
           ppl_Polyhedron_t ph));

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@
PPL_PROTO((ppl_const_@CLASS@_t pset,
           ppl_Polyhedron_t ph_decreasing,
           ppl_Polyhedron_t ph_bounded));

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2
PPL_PROTO((ppl_const_@CLASS@_t pset_before,
           ppl_const_@CLASS@_t pset_after,
           ppl_Polyhedron_t ph_decreasing,
           ppl_Polyhedron_t ph_bounded));

')

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_linear_@PARTITION@
PPL_PROTO((ppl_const_@CLASS@_t x,
           ppl_const_@CLASS@_t y,
           ppl_@CLASS@_t* p_inters,
           ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest));

')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_approximate_@PARTITION@
PPL_PROTO((ppl_const_@CLASS@_t x,
           ppl_const_@CLASS@_t y,
           ppl_@CLASS@_t* p_inters,
           ppl_Pointset_Powerset_Grid_t* p_rest,
           int* p_finite));

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension
PPL_PROTO((ppl_@CLASS@_t* pph, ppl_dimension_type d, int empty));

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@
PPL_PROTO((ppl_@CLASS@_t* pph, ppl_const_@A_FRIEND@_t ph));

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity
PPL_PROTO((ppl_@CLASS@_t* pph, ppl_const_@A_FRIEND@_t ph, int complexity));

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_new_@TOPOLOGY@@CLASS@_from_@!BUILD_REPRESENT@_System
PPL_PROTO((ppl_@CLASS@_t* pph, ppl_const_@!BUILD_REPRESENT@_System_t cs));

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_recycle_@BUILD_REPRESENT@s_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_new_@TOPOLOGY@@CLASS@_recycle_@!BUILD_REPRESENT@_System
PPL_PROTO((ppl_@CLASS@_t* pph, ppl_@!BUILD_REPRESENT@_System_t cs));

')

m4_define(`ppl_delete_@CLASS@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_delete_@CLASS@
PPL_PROTO((ppl_const_@CLASS@_t ph));

')

m4_define(`ppl_assign_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_assign_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@
PPL_PROTO((ppl_@CLASS@_t dst, ppl_const_@CLASS@_t src));

')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@DIMENSION@
PPL_PROTO((ppl_const_@CLASS@_t ph, ppl_dimension_type* m));

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_get_@CLASS_REPRESENT@s
PPL_PROTO((ppl_const_@CLASS@_t ph,
           ppl_const_@!CLASS_REPRESENT@_System_t* pcs));

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s
PPL_PROTO((ppl_const_@CLASS@_t ph,
           ppl_const_@!CLASS_REPRESENT@_System_t* pcs));

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_relation_with_@!RELATION_REPRESENT@
PPL_PROTO((ppl_const_@CLASS@_t ph,
           ppl_const_@!RELATION_REPRESENT@_t c));

')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@HAS_PROPERTY@
PPL_PROTO((ppl_const_@CLASS@_t ph));

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_bounds_from_@ABOVEBELOW@
PPL_PROTO((ppl_const_@CLASS@_t ph,
           ppl_const_Linear_Expression_t le));

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@MAXMIN@
PPL_PROTO((ppl_const_@CLASS@_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum));

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@MAXMIN@_with_point
PPL_PROTO((ppl_const_@CLASS@_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* poptimum,
           ppl_Generator_t point));

')

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_get_@UPPERLOWER@_bound
PPL_PROTO((ppl_@CLASS@_t ps,
           ppl_dimension_type var,
           ppl_Coefficient_t ext_n,
           ppl_Coefficient_t ext_d,
           int* pclosed));

')

m4_define(`ppl_@CLASS@_frequency_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_frequency
PPL_PROTO((ppl_const_@CLASS@_t ph,
           ppl_const_Linear_Expression_t le,
           ppl_Coefficient_t ext_fn,
           ppl_Coefficient_t ext_fd,
           ppl_Coefficient_t ext_vn,
           ppl_Coefficient_t ext_vd));

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@COMPARISON@_@CLASS@
PPL_PROTO((ppl_const_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_equals_@CLASS@
PPL_PROTO((ppl_const_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_OK_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_OK
PPL_PROTO((ppl_const_@CLASS@_t ph));

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@SIMPLIFY@
PPL_PROTO((ppl_@CLASS@_t ph));

')

m4_define(`ppl_@CLASS@_constrains_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_constrains
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type var));

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_unconstrain_space_dimension
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type var));

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_unconstrain_space_dimensions
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type ds[],
           size_t n));

')

m4_define(`ppl_@CLASS@_@BINOP@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@BINOP@
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@UB_EXACT@
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_simplify_using_context_assign
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_add_@CLASS_REPRESENT@
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_const_@!CLASS_REPRESENT@_t c));

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_refine_with_@REFINE_REPRESENT@
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_const_@!REFINE_REPRESENT@_t c));

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_add_@CLASS_REPRESENT@s
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_const_@!CLASS_REPRESENT@_System_t cs));

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_const_@!REFINE_REPRESENT@_System_t cs));

')

m4_define(`ppl_@CLASS@_add_recycled_@CLASS_REPRESENT@s_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_add_recycled_@CLASS_REPRESENT@s
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_@!CLASS_REPRESENT@_System_t cs));

')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@AFFIMAGE@
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_bounded_@AFFIMAGE@
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type var,
           ppl_const_Linear_Expression_t lb,
           ppl_const_Linear_Expression_t ub,
           ppl_const_Coefficient_t d));

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_generalized_@AFFIMAGE@
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d));

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs));

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type var,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t le,
           ppl_const_Coefficient_t d,
           ppl_const_Coefficient_t m));

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_const_Linear_Expression_t lhs,
           enum ppl_enum_Constraint_Type relsym,
           ppl_const_Linear_Expression_t rhs,
           ppl_const_Coefficient_t m));

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y,
           unsigned* tp));

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@WIDEN@_widening_assign
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_widening_assign_with_tokens
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y,
           unsigned* tp));

')

m4_define(`ppl_@CLASS@_widening_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_widening_assign
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y,
           ppl_const_@!CONSTRAINER@_System_t cs,
           unsigned* tp));

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y,
           ppl_const_@!CONSTRAINER@_System_t cs));

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y,
           unsigned* tp));

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y,
           int disjuncts));

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign
PPL_PROTO((ppl_@CLASS@_t x,
           ppl_const_@CLASS@_t y));

')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type d));

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_remove_space_dimensions
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type ds[],
           size_t n));

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_remove_higher_space_dimensions
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type d));

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_map_space_dimensions
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type maps[],
           size_t n));

')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_expand_space_dimension
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type d,
           ppl_dimension_type m));

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_fold_space_dimensions
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type ds[],
           size_t n,
           ppl_dimension_type d));

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_drop_some_non_integer_points
PPL_PROTO((ppl_@CLASS@_t ph,
           int complexity));

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
`dnl
/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_drop_some_non_integer_points_2
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type ds[],
           size_t n,
           int complexity));

')

m4_define(`ppl_@CLASS@_wrap_assign_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_wrap_assign
PPL_PROTO((ppl_@CLASS@_t ph,
           ppl_dimension_type ds[],
           size_t n,
           enum ppl_enum_Bounded_Integer_Type_Width w,
           enum ppl_enum_Bounded_Integer_Type_Representation r,
           enum ppl_enum_Bounded_Integer_Type_Overflow o,
           const ppl_const_Constraint_System_t* pcs,
           unsigned complexity_threshold,
           int wrap_individually));
')

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_@MEMBYTES@
PPL_PROTO((ppl_const_@CLASS@_t ps,
           size_t* sz));

')

m4_define(`ppl_new_@CLASS@_iterator_code',
`dnl
/*! \interface ppl_@CLASS@_iterator_tag
  \brief Types and functions for iterating on the disjuncts of a
  ppl_@CLASS@_tag.
*/
/*! \interface ppl_@CLASS@_const_iterator_tag
  \brief Types and functions for iterating on the disjuncts of a
  const ppl_@CLASS@_tag.
*/

PPL_TYPE_DECLARATION(@CLASS@_iterator)
PPL_TYPE_DECLARATION(@CLASS@_const_iterator)

/*! \relates ppl_@CLASS@_iterator_tag */
int
ppl_new_@CLASS@_iterator
PPL_PROTO((ppl_@CLASS@_iterator_t* pit));

/*! \relates ppl_@CLASS@_const_iterator_tag */
int
ppl_new_@CLASS@_const_iterator
PPL_PROTO((ppl_@CLASS@_const_iterator_t* pit));

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`/*! \relates ppl_@CLASS@_iterator_tag */
int
ppl_new_@CLASS@_iterator_from_iterator
PPL_PROTO((ppl_@CLASS@_iterator_t* px,
           ppl_const_@CLASS@_iterator_t y));

/*! \relates ppl_@CLASS@_const_iterator_tag */
int
ppl_new_@CLASS@_const_iterator_from_const_iterator
PPL_PROTO((ppl_@CLASS@_const_iterator_t* px,
           ppl_const_@CLASS@_const_iterator_t y));

')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
`/*! \relates ppl_@CLASS@_iterator_tag */
int
ppl_@CLASS@_iterator_equal_test
PPL_PROTO((ppl_const_@CLASS@_iterator_t x,
           ppl_const_@CLASS@_iterator_t y));

/*! \relates ppl_@CLASS@_const_iterator_tag */
int
ppl_@CLASS@_const_iterator_equal_test
PPL_PROTO((ppl_const_@CLASS@_const_iterator_t x,
           ppl_const_@CLASS@_const_iterator_t y));

')

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
`/*! \relates ppl_@CLASS@_iterator_tag */
int
ppl_@CLASS@_iterator_@BEGINEND@
PPL_PROTO((ppl_@CLASS@_t ps,
           ppl_@CLASS@_iterator_t psit));

/*! \relates ppl_@CLASS@_const_iterator_tag */
int
ppl_@CLASS@_const_iterator_@BEGINEND@
PPL_PROTO((ppl_const_@CLASS@_t ps,
           ppl_@CLASS@_const_iterator_t psit));

')

m4_define(`ppl_delete_@CLASS@_iterator_code',
`/*! \relates ppl_@CLASS@_iterator_tag */
int
ppl_delete_@CLASS@_iterator
PPL_PROTO((ppl_const_@CLASS@_iterator_t psit));

/*! \relates ppl_@CLASS@_const_iterator_tag */
int
ppl_delete_@CLASS@_const_iterator
PPL_PROTO((ppl_const_@CLASS@_const_iterator_t psit));

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
`/*! \relates ppl_@CLASS@_iterator_tag */
int
ppl_@CLASS@_iterator_@INCDEC@
PPL_PROTO((ppl_@CLASS@_iterator_t psit));

/*! \relates ppl_@CLASS@_const_iterator_tag */
int
ppl_@CLASS@_const_iterator_@INCDEC@
PPL_PROTO((ppl_@CLASS@_const_iterator_t psit));

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_drop_disjunct
PPL_PROTO((ppl_@CLASS@_t ps,
 ppl_const_@CLASS@_iterator_t cit,
 ppl_@CLASS@_iterator_t it));

')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_drop_disjuncts
PPL_PROTO((ppl_@CLASS@_t ps,
 ppl_const_@CLASS@_iterator_t first,
 ppl_const_@CLASS@_iterator_t last));

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
`/*! \relates ppl_@CLASS@_tag */
int
ppl_@CLASS@_add_disjunct
PPL_PROTO((ppl_@CLASS@_t ps,
           ppl_const_@DISJUNCT@_t d));

')

m4_define(`ppl_@CLASS@_get_disjunct_code',
`/*! \relates ppl_@CLASS@_iterator_tag */
int
ppl_@CLASS@_iterator_dereference
PPL_PROTO((ppl_const_@CLASS@_iterator_t ps,
           ppl_const_@DISJUNCT@_t* d));

/*! \relates ppl_@CLASS@_const_iterator_tag */
int
ppl_@CLASS@_const_iterator_dereference
PPL_PROTO((ppl_const_@CLASS@_const_iterator_t ps,
           ppl_const_@DISJUNCT@_t* d));

')
