dnl  -*- java -*-
m4_divert(-1)

This m4 file contains the code for generating ppl_java_generated_tests.java

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
site: http://bugseng.com/products/ppl/ .

FIXME: Find a way to avoid having these dummy macros.
No code is needed for these procedure schemas in the Java interface
as the tokens argument for widening and extrapolation is optional.

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code', `')
m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code', `')
m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code', `')
m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code', `')
FIXME: This is the pattern used for finalize() which is protected.
m4_define(`ppl_delete_@CLASS@_code', `')

Define here as empty any known schematic method macros for which
the definition is not yet implemented.
m4_define(`ppl_delete_@CLASS@_iterator_code', `')


m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`dnl
{
    PPL_Test.print_if_noisy("Testing @TOPOLOGY@@CLASS@_from_space_dimension: ");
    @TOPOLOGY@@CLASS@ new_0_universe
        = new @TOPOLOGY@@CLASS@(0, Degenerate_Element.UNIVERSE);
    @TOPOLOGY@@CLASS@ new_6_universe
        = new @TOPOLOGY@@CLASS@(6, Degenerate_Element.UNIVERSE);
    @TOPOLOGY@@CLASS@ new_0_empty
        = new @TOPOLOGY@@CLASS@(0, Degenerate_Element.EMPTY);
    @TOPOLOGY@@CLASS@ new_6_empty
        = new @TOPOLOGY@@CLASS@(6, Degenerate_Element.EMPTY);
    report_success_or_failure(new_0_universe.OK() && new_6_universe.OK()
       && new_0_empty.OK() && new_6_empty.OK());
    new_0_universe.free();
    new_6_universe.free();
    new_0_empty.free();
    new_6_empty.free();
}

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`dnl
{
    PPL_Test.print_if_noisy("Testing @TOPOLOGY@@CLASS@ from @FRIEND@: ");
    @FRIEND@ friend_gd = new @FRIEND@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ new_gd = new @TOPOLOGY@@CLASS@(friend_gd);
    report_success_or_failure(new_gd.OK());
    friend_gd.free();
    new_gd.free();
}

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
    `dnl
{
    if (("@FRIEND@" != "@TOPOLOGY@@CLASS@")) {
        PPL_Test.print_if_noisy("Testing @TOPOLOGY@@CLASS@ from @FRIEND@ with complexity: ");
        @FRIEND@ friend_gd
            = new @FRIEND@(@CONSTRAINER@s1);
        @TOPOLOGY@@CLASS@ new_gd_pc
            = new @TOPOLOGY@@CLASS@(friend_gd, Complexity_Class.POLYNOMIAL_COMPLEXITY);
        @TOPOLOGY@@CLASS@ new_gd_sc
            = new @TOPOLOGY@@CLASS@(friend_gd, Complexity_Class.SIMPLEX_COMPLEXITY);
        @TOPOLOGY@@CLASS@ new_gd_ac
            = new @TOPOLOGY@@CLASS@(friend_gd, Complexity_Class.ANY_COMPLEXITY);
        report_success_or_failure(new_gd_ac.OK());
        friend_gd.free();
        new_gd_pc.free();
        new_gd_sc.free();
        new_gd_ac.free();
    }
}

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @TOPOLOGY@@CLASS@ from @BUILD_REPRESENT@s: ");
    @TOPOLOGY@@CLASS@ new_gd1 = new @TOPOLOGY@@CLASS@(@BUILD_REPRESENT@s1);
    report_success_or_failure(new_gd1.OK());
    new_gd1.free();
}

')

m4_define(`ppl_@CLASS@_swap_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing swap: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s2);
    gd1.swap(gd2);
    report_success_or_failure(gd1.OK() && gd2.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing bounds_from_@ABOVEBELOW@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean bounds_from_@ABOVEBELOW@
        = gd.bounds_from_@ABOVEBELOW@(le_A);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_hashcode_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing hashcode: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    PPL_Test.print_if_noisy("The hashcode is: " + gd.hashCode());
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @HAS_PROPERTY@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    if (gd.@HAS_PROPERTY@())
        PPL_Test.println_if_noisy("@HAS_PROPERTY@ is true for gd.");
    else
        PPL_Test.println_if_noisy("@HAS_PROPERTY@ is false for gd.");
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @DIMENSION@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    PPL_Test.print_if_noisy("@DIMENSION@ of gd = ");
    PPL_Test.println_if_noisy(gd.@DIMENSION@());
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@BINOP@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @BINOP@: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s2);
    gd1.@BINOP@(gd2);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing simplify_using_context_assign: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean gd_simplify_using_context_assign
        = gd.simplify_using_context_assign(gd);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing get_@CLASS_REPRESENT@s: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @!CLASS_REPRESENT@_System gd_@CLASS_REPRESENT@ = gd.@CLASS_REPRESENT@s();
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing get_minimized_@CLASS_REPRESENT@s: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @!CLASS_REPRESENT@_System gr = gd.minimized_@CLASS_REPRESENT@s();
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @COMPARISON@: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s2);
    boolean gd1_@COMPARISON@ = gd2.@COMPARISON@(gd1);
    report_success_or_failure(gd1.OK() && gd2.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @EXTRAPOLATION@_narrowing_assign: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s2);
    gd1.@EXTRAPOLATION@_narrowing_assign(gd2);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing relation_with_@RELATION_REPRESENT@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    Poly_@!A_RELATION_REPRESENT@_Relation
        poly_relation = gd.relation_with(@RELATION_REPRESENT@1);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing add_@!CLASS_REPRESENT@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.add_@CLASS_REPRESENT@(@CLASS_REPRESENT@1);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing refine_with_@REFINE_REPRESENT@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.refine_with_@REFINE_REPRESENT@(@REFINE_REPRESENT@1);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing add_@CLASS_REPRESENT@s: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.add_@CLASS_REPRESENT@s(@CLASS_REPRESENT@s1);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing refine_with_@REFINE_REPRESENT@s: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.refine_with_@REFINE_REPRESENT@s(@REFINE_REPRESENT@s1);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @UB_EXACT@: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean is_exact = gd1.@UB_EXACT@(gd2);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @AFFIMAGE@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.@AFFIMAGE@(var_C, le_A, coeff_5);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing generalized_@AFFIMAGE@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.generalized_@AFFIMAGE@(var_C, Relation_Symbol.EQUAL, le_A, coeff_5);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing generalized_@AFFIMAGE@_lhs_rhs: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.generalized_@AFFIMAGE@(le_A, Relation_Symbol.EQUAL, le_A);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing generalized_@AFFIMAGE@_with_congruence: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.generalized_@AFFIMAGE@_with_congruence(var_C, Relation_Symbol.EQUAL,
                                              le_A, coeff_5, coeff_5);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing generalized_@AFFIMAGE@_lhs_rhs_with_congruence: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.generalized_@AFFIMAGE@_lhs_rhs_with_congruence(le_A,
                                                      Relation_Symbol.EQUAL,
                                                      le_A, coeff_5);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing equals: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean equals = gd.equals(gd);
    if (!gd.equals(new Object()))
        PPL_Test.println_if_noisy("A generic object is not equal to gd");
    report_success_or_failure(equals && gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_OK_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing OK: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean ok = gd.OK();
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing bounded_@AFFIMAGE@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.bounded_@AFFIMAGE@(var_C, le_A, le_A, coeff_5);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @SIMPLIFY@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.@SIMPLIFY@();
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing unconstrain_space_dimension: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.unconstrain_space_dimension(var_C);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing unconstrain_space_dimensions: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.unconstrain_space_dimensions(var_set_A);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_constrains_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing constrains: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean constrains = gd.constrains(var_C);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing has_@UPPERLOWER@_bound: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean has_@UPPERLOWER@
        = gd.has_@UPPERLOWER@_bound(var_C, coeff_0, coeff_5, bool_by_ref1);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @MAXMIN@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean @MAXMIN@
        = gd.@MAXMIN@(le_A, coeff_0, coeff_5, bool_by_ref1);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @MAXMIN@_with_point: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean @MAXMIN@_with_point
        = gd.@MAXMIN@(le_A, coeff_0, coeff_5, bool_by_ref2, generator1);
    report_success_or_failure(gd.OK());
    gd.free();
}

');

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing add_space_dimensions_@EMBEDPROJECT@: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.add_space_dimensions_@EMBEDPROJECT@(2);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing remove_higher_space_dimensions: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.remove_higher_space_dimensions(2);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing remove_space_dimensions: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.remove_space_dimensions(var_set_A);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing expand_space_dimension: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.expand_space_dimension(var_C, 1);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing fold_space_dimensions: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.fold_space_dimensions(var_set_A, var_C);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing map_space_dimensions: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd.map_space_dimensions(partial_function);
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @WIDEN@_widening_assign: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd1.@WIDEN@_widening_assign(gd2);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing : ppl_@CLASS@_@WIDEN@_widening_assign");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd1.@WIDEN@_widening_assign(gd2, int_by_ref1);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_widening_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing widening_assign: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd1.widening_assign(gd2, int_by_ref1);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @EXTRAPOLATION@_extrapolation_assign: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd1.@EXTRAPOLATION@_extrapolation_assign(gd2, int_by_ref1);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @EXTRAPOLATION@_narrowing_assign: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd1.@EXTRAPOLATION@_narrowing_assign(gd2);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd1.@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(gd2, @CONSTRAINER@s1,
                                                          zero_by_ref1);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing BGP99_@DISJUNCT_WIDEN@_extrapolation_assign: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd1.BGP99_@DISJUNCT_WIDEN@_extrapolation_assign(gd2, 2);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    gd1.BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign(gd2);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_string_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing toString(): ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    PPL_Test.println_if_noisy(gd.toString());
    report_success_or_failure(gd.OK());
    gd.free();
}
');

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @MEMBYTES@(): ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    PPL_Test.println_if_noisy("@MEMBYTES@ of gd: ");
    PPL_Test.println_if_noisy(gd.@MEMBYTES@());
    report_success_or_failure(gd.OK());
    gd.free();
}

');

m4_define(`ppl_@CLASS@_ascii_dump_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing ascii_dump(): ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    PPL_Test.println_if_noisy(gd.ascii_dump());
    report_success_or_failure(gd.OK());
    gd.free();
}

');

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing linear_@PARTITION@: ");
    @TOPOLOGY@@CLASS@ gd1
        = new @TOPOLOGY@@CLASS@(constraints1);
    @TOPOLOGY@@CLASS@ gd2
        = new @TOPOLOGY@@CLASS@(constraints1);
    Pair p = @TOPOLOGY@@CLASS@.linear_partition(gd1, gd2);
    report_success_or_failure(gd1.OK());
    PPL_Test.print_if_noisy("Printing Pair from linear_partition: ");
    PPL_Test.print_if_noisy((p.getFirst()).toString());
    PPL_Test.print_if_noisy(", ");
    PPL_Test.println_if_noisy((p.getSecond()).toString());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_approximate_partition_code',
    `dnl
@SUPERPOWER_EXISTS@`'dnl
{
    PPL_Test.print_if_noisy("Testing @CLASS@_approximate_partition: ");
    @CLASS@ gd1
        = new @CLASS@(constraints1);
    @CLASS@ gd2
        = new @CLASS@(constraints1);
    Pair p
        = @CLASS@.approximate_partition(gd1, gd2, bool_by_ref1);
    report_success_or_failure(gd1.OK());
    PPL_Test.print_if_noisy("Printing Pair from approximate_partition: ");
    PPL_Test.print_if_noisy((p.getFirst()).toString());
    PPL_Test.print_if_noisy(", ");
    PPL_Test.print_if_noisy((p.getSecond()).toString());
    PPL_Test.print_if_noisy(", ");
    PPL_Test.println_if_noisy(bool_by_ref1);
    PPL_Test.println_if_noisy();
    gd1.free();
    gd2.free();
}

');

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @BEGINEND@_iterator: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@_Iterator it_gd = gd.@BEGINEND@_iterator();
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing @INCDEC@_iterator: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    if ("@INCDEC@" == "increment") {
      @TOPOLOGY@@CLASS@_Iterator it_gd = gd.begin_iterator();
      it_gd.@A_INCDEC@();
    }
    else {
      @TOPOLOGY@@CLASS@_Iterator it_gd = gd.end_iterator();
      it_gd.@A_INCDEC@();
    }
    report_success_or_failure(gd.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_get_disjunct_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing get_disjunct: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@_Iterator it_gd = gd.begin_iterator();
    @TOPOLOGY@@DISJUNCT@ gd_disjunct = it_gd.get_disjunct();
    report_success_or_failure(gd.OK() && gd_disjunct.OK());
    gd.free();
}

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing new_iterator_from_iterator: ");
    @CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @CLASS@_Iterator it_gd = gd.begin_iterator();
    @CLASS@_Iterator it_gd_copy = new @CLASS@_Iterator(it_gd);
    @TOPOLOGY@@DISJUNCT@ gd_disjunct = it_gd_copy.get_disjunct();
    report_success_or_failure(gd.OK() && gd_disjunct.OK());
    gd.free();
}

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing drop_disjunct: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@_Iterator it_gd = gd.begin_iterator();
    gd.drop_disjunct(it_gd);
    report_success_or_failure(gd.OK());
    gd.free();
}

{
    PPL_Test.print_if_noisy("Testing drop_disjuncts: ");
    @CLASS@ gd1 = new @CLASS@(@CONSTRAINER@s1);
    @CLASS@ gd2 = new @CLASS@(@CONSTRAINER@s2);
    gd1.upper_bound_assign(gd2);
    @CLASS@_Iterator it_gd1 = gd1.begin_iterator();
    @CLASS@_Iterator it_gd1_end = gd1.end_iterator();
    gd1.drop_disjuncts(it_gd1, it_gd1_end);
    report_success_or_failure(gd1.OK());
    gd1.free();
    gd2.free();
}

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing add_disjunct: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @DISJUNCT_TOPOLOGY@@DISJUNCT@ gd_disjunct
        = new @DISJUNCT_TOPOLOGY@@DISJUNCT@(@CONSTRAINER@s1);
    gd.add_disjunct(gd_disjunct);
    report_success_or_failure(gd.OK());
    gd.free();
    gd_disjunct.free();
}

');

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing iterator_equals_iterator: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@_Iterator it_gd1 = gd.begin_iterator();
    @TOPOLOGY@@CLASS@_Iterator it_gd2 = gd.begin_iterator();
    boolean equals = it_gd1.equals(it_gd2);
    report_success_or_failure(gd.OK() && equals);
    gd.free();
}

')

m4_define(`ppl_free_@CLASS@_code',
    `dnl
{
    PPL_Test.print_if_noisy("Testing free: ");
    @TOPOLOGY@@CLASS@ gd = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(gd);
    gd1.free();
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(gd);
    gd2 = null;
    report_success_or_failure(true);
}

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_codexxx',
    `dnl
{
    PPL_Test.print_if_noisy("Testing termination_@TERMINATION_ID@_@TOPOLOGY@@CLASS@: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    boolean term = Termination.termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@(gd1);
    report_success_or_failure(gd1.OK() && term);
    gd1.free();
}

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_codexxx',
    `dnl
{
    PPL_Test.print_if_noisy("Testing termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@: ");
    @TOPOLOGY@@CLASS@ gd1 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s1);
    @TOPOLOGY@@CLASS@ gd2 = new @TOPOLOGY@@CLASS@(@CONSTRAINER@s2);
    boolean term = Termination.termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2(gd1, gd2);
    report_success_or_failure(gd1.OK() && gd2.OK() && term);
    gd1.free();
    gd2.free();
}

')

m4_divert`'dnl
