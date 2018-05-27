m4_divert(-1)

dnl This m4 file defines the list of the procedures generated
dnl that are independent of the interface being generated.

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

dnl m4_procedure_list
dnl This class using patterns wherever possible.
dnl Which classes the schema applies to is determined by +/-group_name.
dnl The group_names represent sets of PPL classes +group1 and -group2.
dnl These are defined in ../ppl_interface_generator_common_dat.m4.
dnl The actual classes the schema applies to is the set difference
dnl +group1 \ -group2 where a missing +group1 or -group2 is
dnl assumed to be the empty set.
dnl For the pointset-powerset classes, the schema they apply to may also
dnl depend on the disjunct. In this case, a schema with
dnl "+pointset_powerset \group" will apply to any powerset
dnl whose disjunct is not in "group".
dnl Where "@CLASS@" is replaced by the class name, then that class only
dnl is applicable for that schema.
dnl
dnl Note that the code for the schema "<name>_code" must be defined
dnl in the ppl_interface_generator_*_code.m4 file.
dnl The <name> must be exactly as written here.

m4_define(`m4_common_procedure_list', `dnl
ppl_delete_@CLASS@/1 *nofail +all,
ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension/3 +all,
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@/2 +all,
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity/3 +all,
ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s/2 +all,
ppl_@CLASS@_swap/2 *nofail +all,
ppl_@CLASS@_@DIMENSION@/2 +all,
ppl_@CLASS@_relation_with_@RELATION_REPRESENT@/3 +all,
dnl
dnl NOTE: The next two schemas are only for simple domains since
dnl       we can only "get" a meaningful system (such as a set of
dnl       constraints) that represents a domain from a simple domain.
dnl
ppl_@CLASS@_get_@CLASS_REPRESENT@s/2 +simple,
ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s/2 +simple,
ppl_@CLASS@_@HAS_PROPERTY@/1 +all,
ppl_@CLASS@_@SIMPLIFY@/1 *nofail +all,
ppl_@CLASS@_bounds_from_@ABOVEBELOW@/2 +all,
ppl_@CLASS@_has_@UPPERLOWER@_bound/5 +box,
ppl_@CLASS@_@MAXMIN@/5 +all,
ppl_@CLASS@_@MAXMIN@_with_point/6 +all,
ppl_@CLASS@_frequency/6 +simple,
ppl_@CLASS@_@COMPARISON@_@CLASS@/2 +all,
ppl_@CLASS@_equals_@CLASS@/2 +all,
ppl_@CLASS@_OK/1 +all,
ppl_@CLASS@_add_@CLASS_REPRESENT@/2 *nofail +all,
ppl_@CLASS@_add_@CLASS_REPRESENT@s/2 *nofail +all,
ppl_@CLASS@_refine_with_@REFINE_REPRESENT@/2 *nofail +all,
ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s/2 *nofail +all,
ppl_@CLASS@_@BINOP@/2 *nofail +all,
ppl_@CLASS@_@UB_EXACT@/2 +all,
ppl_@CLASS@_simplify_using_context_assign/3 +simple_pps,
ppl_@CLASS@_constrains/2 +all,
ppl_@CLASS@_unconstrain_space_dimension/2 +all,
ppl_@CLASS@_unconstrain_space_dimensions/2 +all,
ppl_@CLASS@_@AFFIMAGE@/4 *nofail +all,
ppl_@CLASS@_bounded_@AFFIMAGE@/5 *nofail +all,
ppl_@CLASS@_generalized_@AFFIMAGE@/5 +all,
ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs/4 +all,
ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence/6 +grid,
ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence/5 +grid,
ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@/2 *nofail +all,
ppl_@CLASS@_remove_space_dimensions/2 +all,
ppl_@CLASS@_remove_higher_space_dimensions/2 *nofail +all,
ppl_@CLASS@_expand_space_dimension/3 *nofail +all,
ppl_@CLASS@_fold_space_dimensions/3  +all,
ppl_@CLASS@_map_space_dimensions/2 +all,
ppl_@CLASS@_drop_some_non_integer_points/2 +all,
ppl_@CLASS@_drop_some_non_integer_points_2/3 +all,
ppl_@CLASS@_ascii_dump/1 +all,
ppl_@CLASS@_@MEMBYTES@/2 +all,
dnl
dnl NOTE: The next few schemas provide special widenings and
dnl       extrapolations that depend on the domains.
dnl
ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens/4 +simple,
ppl_@CLASS@_@WIDEN@_widening_assign/2 *nofail +simple,
dnl
dnl FIXME: We do not have a default widening for the
dnl        pointset_powerset domain.
dnl
ppl_@CLASS@_widening_assign_with_tokens/4 +simple +product,
ppl_@CLASS@_widening_assign/2 *nofail +simple +product,
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens/5 +simple,
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign/3 *nofail +simple,
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens/4 +wr_shape,
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign/2 *nofail +wr_shape,
ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign/2 +shape_group,
dnl
dnl NOTE: The next few schemas provide procedures specifically for
dnl       the pointset_powerset domains.
dnl
ppl_new_@CLASS@_iterator_from_iterator/2 +pointset_powerset,
ppl_@CLASS@_@BEGINEND@_iterator/2 +pointset_powerset,
ppl_@CLASS@_iterator_equals_iterator/2 +pointset_powerset,
ppl_@CLASS@_@INCDEC@_iterator/1 +pointset_powerset,
ppl_@CLASS@_get_disjunct/2 +pointset_powerset,
ppl_delete_@CLASS@_iterator/1 +pointset_powerset,
ppl_@CLASS@_add_disjunct/2 *nofail +pointset_powerset,
ppl_@CLASS@_drop_disjunct/2 +pointset_powerset,
ppl_@CLASS@_drop_disjuncts/3 +pointset_powerset,
ppl_@CLASS@_linear_@PARTITION@/4 +simple -grid,
ppl_@CLASS@_approximate_@PARTITION@/5  +grid,
ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign/2 +pointset_powerset \box,
ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign/3 +pointset_powerset \box,
dnl
dnl CHECKME: This is not implemented.
dnl
ppl_@CLASS@_BGP99_@DISJUNCT_EXTRAPOLATION@_extrapolation_assign/3 -pointset_powerset,
ppl_@CLASS@_wrap_assign/8 +simple,

')
