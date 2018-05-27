dnl  -*- C++ -*-
m4_divert(-1)

dnl This m4 file contains the program code for generating the
dnl files ppl_prolog_DOMAIN.hh for each interface domain DOMAIN
dnl in ppl_interface instantiations.m4.

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

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
`dnl
extern "C" Prolog_foreign_return_type
ppl_@CLASS@_linear_@PARTITION@(Prolog_term_ref t_ph,
			     Prolog_term_ref t_qh,
			     Prolog_term_ref t_inters,
			     Prolog_term_ref t_pset);

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`extern "C" Prolog_foreign_return_type
ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension(Prolog_term_ref t_nd,
                                               Prolog_term_ref t_uoe,
                                               Prolog_term_ref t_ph);

')

  m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`extern "C" Prolog_foreign_return_type
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@(
                     Prolog_term_ref t_ph_source, Prolog_term_ref t_ph);

')

  m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`extern "C" Prolog_foreign_return_type
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity(
                     Prolog_term_ref t_ph_source,
                     Prolog_term_ref t_ph,
                     Prolog_term_ref t_cc);

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s(Prolog_term_ref t_clist,
						    Prolog_term_ref t_ph);
')

  m4_define(`ppl_@CLASS@_swap_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_swap(Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')

m4_define(`ppl_delete_@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_delete_@CLASS@(Prolog_term_ref t_ph);
')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@DIMENSION@(Prolog_term_ref t_ph, Prolog_term_ref t_sd);
')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_get_@CLASS_REPRESENT@s(Prolog_term_ref t_ph,
				   Prolog_term_ref t_glist);

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s(Prolog_term_ref t_ph,
					     Prolog_term_ref t_glist);

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`extern "C" Prolog_foreign_return_type
ppl_new_@CLASS@_iterator_from_iterator(Prolog_term_ref t_source,
				       Prolog_term_ref t_it);

')

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@BEGINEND@_iterator(Prolog_term_ref t_pps,
				  Prolog_term_ref t_it);

')

m4_define(`ppl_delete_@CLASS@_iterator_code',
  `extern "C" Prolog_foreign_return_type
  ppl_delete_@CLASS@_iterator(Prolog_term_ref t_it);
')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@INCDEC@_iterator(Prolog_term_ref t_it);

')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_iterator_equals_iterator(Prolog_term_ref t_it1,
				       Prolog_term_ref t_it2);

')

m4_define(`ppl_@CLASS@_get_disjunct_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_get_disjunct(Prolog_term_ref t_it,
			   Prolog_term_ref t_disj);

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_drop_disjunct(Prolog_term_ref t_pps,
			    Prolog_term_ref t_it);

')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_drop_disjuncts(Prolog_term_ref t_pps,
			     Prolog_term_ref t_it1,
			     Prolog_term_ref t_it2);

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_add_disjunct(Prolog_term_ref t_ph, Prolog_term_ref t_d);
')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code',
  `dnl
  extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_approximate_@PARTITION@(Prolog_term_ref t_ph,
				    Prolog_term_ref t_qh,
				    Prolog_term_ref t_finite,
				    Prolog_term_ref t_inters,
				    Prolog_term_ref t_pset);

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_relation_with_@RELATION_REPRESENT@(Prolog_term_ref t_ph,
						 Prolog_term_ref t_c,
						 Prolog_term_ref t_r);

')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@HAS_PROPERTY@(Prolog_term_ref t_ph);

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@SIMPLIFY@(Prolog_term_ref t_ph);

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_unconstrain_space_dimension(Prolog_term_ref t_ph,
                           Prolog_term_ref t_v);

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_unconstrain_space_dimensions(Prolog_term_ref t_ph,
                           Prolog_term_ref t_vlist);

')

m4_define(`ppl_@CLASS@_constrains_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_constrains(Prolog_term_ref t_ph,
                          Prolog_term_ref t_v);

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_bounds_from_@ABOVEBELOW@(Prolog_term_ref t_ph,
				       Prolog_term_ref t_expr);

')

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_has_@UPPERLOWER@_bound(Prolog_term_ref t_ph, Prolog_term_ref t_v,
		       Prolog_term_ref t_n, Prolog_term_ref t_d,
		       Prolog_term_ref t_closed);

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@MAXMIN@(Prolog_term_ref t_ph, Prolog_term_ref t_le_expr,
		       Prolog_term_ref t_n,  Prolog_term_ref t_d,
		       Prolog_term_ref t_maxmin);

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@MAXMIN@_with_point(Prolog_term_ref t_ph,
				  Prolog_term_ref t_le_expr,
				  Prolog_term_ref t_n,
                                  Prolog_term_ref t_d,
				  Prolog_term_ref t_maxmin,
                                  Prolog_term_ref t_g);

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@COMPARISON@_@CLASS@(Prolog_term_ref t_lhs,
				   Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_equals_@CLASS@(Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_OK_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_OK(Prolog_term_ref t_ph);

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_add_@CLASS_REPRESENT@(Prolog_term_ref t_ph, Prolog_term_ref t_c);

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_refine_with_@REFINE_REPRESENT@(Prolog_term_ref t_ph, Prolog_term_ref t_c);

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_add_@CLASS_REPRESENT@s(Prolog_term_ref t_ph,
				   Prolog_term_ref t_clist);

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s(Prolog_term_ref t_ph,
				   Prolog_term_ref t_clist);

')

m4_define(`ppl_@CLASS@_@BINOP@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@BINOP@
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_simplify_using_context_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs, Prolog_term_ref t_b);

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@UB_EXACT@
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@AFFIMAGE@
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_v, Prolog_term_ref t_le, Prolog_term_ref t_d);

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_v, Prolog_term_ref t_r, Prolog_term_ref t_le,
   Prolog_term_ref t_d, Prolog_term_ref t_m);

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_generalized_@AFFIMAGE@
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_v, Prolog_term_ref t_r, Prolog_term_ref t_le,
   Prolog_term_ref t_d);

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_lhs, Prolog_term_ref t_r, Prolog_term_ref t_rhs,
   Prolog_term_ref t_m);

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_lhs, Prolog_term_ref t_r, Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_bounded_@AFFIMAGE@
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_v, Prolog_term_ref t_lb_le, Prolog_term_ref t_ub_le,
   Prolog_term_ref t_d);

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
   Prolog_term_ref t_ti, Prolog_term_ref t_to);

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@WIDEN@_widening_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_widening_assign_with_tokens
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
   Prolog_term_ref t_ti, Prolog_term_ref t_to);

')

m4_define(`ppl_@CLASS@_widening_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_widening_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
   Prolog_term_ref t_ti, Prolog_term_ref t_to);

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')


dnl FIXME: This code is redundant as there is no options for tokens
dnl        in the current C++ code.
m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_with_tokens(
										      Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
										      Prolog_term_ref t_ti, Prolog_term_ref t_to);

')

m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
  `extern "C" Prolog_foreign_return_type

  ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign(
									  Prolog_term_ref t_lhs, Prolog_term_ref t_rhs);

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
  `extern "C" Prolog_foreign_return_type

  ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign(
							  Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
							  Prolog_term_ref t_d);

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_EXTRAPOLATION@_extrapolation_assign_code',
  `extern "C" Prolog_foreign_return_type

  ppl_@CLASS@_BGP99_@DISJUNCT_EXTRAPOLATION@_extrapolation_assign(
								  Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
								  Prolog_term_ref t_d);

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs, Prolog_term_ref t_clist,
   Prolog_term_ref t_ti, Prolog_term_ref t_to);

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs, Prolog_term_ref t_clist);

')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@
  (Prolog_term_ref t_ph, Prolog_term_ref t_nnd);

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_remove_space_dimensions
  (Prolog_term_ref t_ph, Prolog_term_ref t_vlist);

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_remove_higher_space_dimensions
  (Prolog_term_ref t_ph, Prolog_term_ref t_nd);

')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_expand_space_dimension
  (Prolog_term_ref t_ph, Prolog_term_ref t_v, Prolog_term_ref t_nd);

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_fold_space_dimensions
  (Prolog_term_ref t_ph, Prolog_term_ref t_vlist, Prolog_term_ref t_v);

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_map_space_dimensions
  (Prolog_term_ref t_ph, Prolog_term_ref t_pfunc);

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_drop_some_non_integer_points
  (Prolog_term_ref t_ph, Prolog_term_ref t_cc);

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_drop_some_non_integer_points_2
  (Prolog_term_ref t_ph, Prolog_term_ref t_vlist, Prolog_term_ref t_cc);

')

m4_define(`ppl_@CLASS@_ascii_dump_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_ascii_dump
  (Prolog_term_ref t_ph);

')

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@MEMBYTES@(Prolog_term_ref t_pps,
			 Prolog_term_ref t_m);

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@(Prolog_term_ref t_pset);

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
     (Prolog_term_ref t_pset_before,
      Prolog_term_ref t_pset_after);

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
     (Prolog_term_ref t_pset,
      Prolog_term_ref t_g);

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
     (Prolog_term_ref t_pset_before,
      Prolog_term_ref t_pset_after,
      Prolog_term_ref t_g);

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
     (Prolog_term_ref t_pset,
      Prolog_term_ref t_ph);

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
     (Prolog_term_ref t_pset_before,
      Prolog_term_ref t_pset_after,
      Prolog_term_ref t_ph);

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@
     (Prolog_term_ref t_pset,
      Prolog_term_ref t_ph_decreasing,
      Prolog_term_ref t_ph_bounded);

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2
     (Prolog_term_ref t_pset_before,
      Prolog_term_ref t_pset_after,
      Prolog_term_ref t_ph_decreasing,
      Prolog_term_ref t_ph_bounded);

')

m4_define(`ppl_@CLASS@_wrap_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_wrap_assign
     (Prolog_term_ref t_ph,
      Prolog_term_ref t_vars,
      Prolog_term_ref t_w,
      Prolog_term_ref t_r,
      Prolog_term_ref t_o,
      Prolog_term_ref t_cs,
      Prolog_term_ref t_complexity,
      Prolog_term_ref t_ind);

')

m4_define(`ppl_@CLASS@_frequency_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_frequency(Prolog_term_ref t_ph, Prolog_term_ref t_le_expr,
		       Prolog_term_ref t_freqn,  Prolog_term_ref t_freqd,
		       Prolog_term_ref t_valn,  Prolog_term_ref t_vald);

')
