dnl  -*- java -*-
m4_divert(-1)

This m4 file contains the code for generating files <CLASS_NAME>.java

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

Define here as empty any known schematic method macros for which
the definition is not yet implemented.

m4_define(`m4_class_build_cpp_object1_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    private native void build_cpp_object(@!BUILD_REPRESENT@_System cs);

')

m4_define(`m4_class_build_cpp_object2_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    private native void build_cpp_object(long num_dimensions,
                                         Degenerate_Element kind);

')

m4_define(`m4_class_build_cpp_object3_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    private native void build_cpp_object(@FRIEND@ y);

___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    private native void build_cpp_object(@FRIEND@ y,
                                         Complexity_Class complexity);

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    public @TOPOLOGY@@CLASS@(long num_dimensions,
			Degenerate_Element kind) {
	build_cpp_object(num_dimensions, kind);
    }

    private @TOPOLOGY@@CLASS@() {
    }

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    public @TOPOLOGY@@CLASS@(@FRIEND@ y) {
        build_cpp_object(y);
    }

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    public @TOPOLOGY@@CLASS@(@FRIEND@ y, Complexity_Class complexity) {
        build_cpp_object(y, complexity);
    }

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    public @TOPOLOGY@@CLASS@(@!BUILD_REPRESENT@_System cs) {
	build_cpp_object(cs);
    }

')

m4_define(`ppl_delete_@CLASS@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    protected native void finalize();

')

m4_define(`ppl_free_@CLASS@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    public native void free();

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___
    public native boolean @UB_EXACT@(@TOPOLOGY@@CLASS@ y);

')

m4_define(`ppl_@CLASS@_swap_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void swap(@CLASS@ y);

')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native long @DIMENSION@();

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native @!CLASS_REPRESENT@_System @CLASS_REPRESENT@s();

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native @!CLASS_REPRESENT@_System minimized_@CLASS_REPRESENT@s();

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native Poly_@!A_RELATION_REPRESENT@_Relation relation_with(@!RELATION_REPRESENT@ c);

')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean @HAS_PROPERTY@();

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void @SIMPLIFY@();

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void unconstrain_space_dimension(Variable var);

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void
      unconstrain_space_dimensions(Variables_Set vars);

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean bounds_from_@ABOVEBELOW@(Linear_Expression expr);

')

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean has_@UPPERLOWER@_bound(Variable var,
				   Coefficient bound_n, Coefficient bound_d,
				   By_Reference<Boolean> closed);

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean @MAXMIN@(Linear_Expression expr,
				   Coefficient sup_n, Coefficient sup_d,
				   By_Reference<Boolean> maximum);

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean @MAXMIN@(Linear_Expression expr,
				   Coefficient sup_n, Coefficient sup_d,
				   By_Reference<Boolean> maximum,
				   Generator point);

')

m4_define(`ppl_@CLASS@_frequency_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean frequency(Linear_Expression expr,
				   Coefficient freq_n, Coefficient freq_d,
				   Coefficient val_n, Coefficient val_d);

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean @COMPARISON@(@CLASS@ y);

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean equals(@CLASS@ p);

    public boolean equals(Object y) {
   if (this == y)
     return true;
   if (y == null || y.getClass() != this.getClass())
     return false;
   return this.equals((@CLASS@) y);
  }
')

m4_define(`ppl_@CLASS@_hashcode_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native int hashCode();

')


m4_define(`ppl_@CLASS@_OK_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean OK();

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void add_@CLASS_REPRESENT@(@!CLASS_REPRESENT@ c);

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void refine_with_@REFINE_REPRESENT@(@!REFINE_REPRESENT@ c);

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void add_@CLASS_REPRESENT@s(@!CLASS_REPRESENT@_System c);

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void refine_with_@REFINE_REPRESENT@s(@!REFINE_REPRESENT@_System c);

')

m4_define(`ppl_@CLASS@_@BINOP@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void @BINOP@(@CLASS@ p);

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native boolean simplify_using_context_assign(@CLASS@ p);

')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void @AFFIMAGE@(Variable var, Linear_Expression expr,
				    Coefficient denominator);

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void bounded_@AFFIMAGE@(Variable var,
					    Linear_Expression lb_expr,
					    Linear_Expression ub_expr,
					    Coefficient denominator);

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void generalized_@AFFIMAGE@(Variable var,
				Relation_Symbol relsym,
				Linear_Expression expr,
				Coefficient denominator);

')


m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void generalized_@AFFIMAGE@(Linear_Expression lhs,
				Relation_Symbol relsym,
				Linear_Expression rhs);

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void
    @EXTRAPOLATION@_extrapolation_assign(@TOPOLOGY@@CLASS@ arg,
                                By_Reference<Integer> tp);

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void
    @EXTRAPOLATION@_narrowing_assign(@TOPOLOGY@@CLASS@ arg);

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void generalized_@AFFIMAGE@_with_congruence(Variable var,
				Relation_Symbol relsym,
				Linear_Expression expr,
				Coefficient denominator,
				Coefficient modulus);

')


m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void generalized_@AFFIMAGE@_lhs_rhs_with_congruence(Linear_Expression lhs,
				Relation_Symbol relsym,
				Linear_Expression rhs,
				Coefficient modulus);

')

m4_define(`ppl_@CLASS@_wrap_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void wrap_assign(Variables_Set vars,
				   Bounded_Integer_Type_Width w,
				   Bounded_Integer_Type_Representation r,
                                   Bounded_Integer_Type_Overflow o,
                                   Constraint_System cs,
                                   long complexity_threshold,
                                   boolean wrap_individually);

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
public native void @WIDEN@_widening_assign(@CLASS@ y,
                                By_Reference<Integer> tp);

')

m4_define(`ppl_@CLASS@_widening_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
public native void widening_assign(@CLASS@ y,
                                By_Reference<Integer> tp);

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native
	void @LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(@CLASS@ y,
						 @!CONSTRAINER@_System cs,
						 By_Reference<Integer> tp);

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native
	void BGP99_@DISJUNCT_WIDEN@_extrapolation_assign(@CLASS@ y,
						 Integer d);

')

m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void
    BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign(@CLASS@ y);

')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void add_space_dimensions_@EMBEDPROJECT@(long m);

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void remove_space_dimensions(Variables_Set vars);

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void remove_higher_space_dimensions(long new_dimension);

')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void expand_space_dimension(Variable var, long m);

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void fold_space_dimensions(Variables_Set vars,
					     Variable dest);

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void drop_some_non_integer_points
      (Integer complexity_threshold);

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void drop_some_non_integer_points_2
      (Variables_Set vars, Integer complexity_threshold);

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native void map_space_dimensions(Partial_Function pfunc);

')

m4_define(`ppl_@CLASS@_string_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native String toString();

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@_Iterator.java << ___END_OF_FILE___
    public @CLASS@_Iterator(@CLASS@_Iterator y) {
        build_cpp_object(y);
    }

    private native void
        build_cpp_object(@CLASS@_Iterator y);

')

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native @CLASS@_Iterator @BEGINEND@_iterator();

')

m4_define(`ppl_delete_@CLASS@_iterator_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@_Iterator.java << ___END_OF_FILE___
    public native void free();

    protected native void finalize();

')

m4_define(`ppl_@CLASS@_get_disjunct_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@_Iterator.java << ___END_OF_FILE___
  public native @DISJUNCT_TOPOLOGY@@DISJUNCT@ get_disjunct();

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
  public native void drop_disjunct(@CLASS@_Iterator itr);

')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
`dnl
  public native void drop_disjuncts(@CLASS@_Iterator itr1,
                                    @CLASS@_Iterator itr2);

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
  public native void add_disjunct(@DISJUNCT_TOPOLOGY@@DISJUNCT@ j_disj);

')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@_Iterator.java << ___END_OF_FILE___
  public native boolean equals(@CLASS@_Iterator itr);

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@_Iterator.java << ___END_OF_FILE___
  public native void @A_INCDEC@();

')

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
  public native long @MEMBYTES@();

')

m4_define(`ppl_@CLASS@_constrains_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
  public native boolean constrains(Variable var);

')

m4_define(`ppl_@CLASS@_ascii_dump_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___
    public native String ascii_dump();

')

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @TOPOLOGY@@CLASS@.java << ___END_OF_FILE___

  public static native
    Pair <@TOPOLOGY@@CLASS@|COMMA| Pointset_Powerset_NNC_Polyhedron >
      linear_@PARTITION@(@TOPOLOGY@@CLASS@ p, @TOPOLOGY@@CLASS@ q);
')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ @CLASS@.java << ___END_OF_FILE___

  public static native Pair <@CLASS@|COMMA| Pointset_Powerset_Grid >
      approximate_@PARTITION@(@CLASS@ p,
                            @CLASS@ q,
                            By_Reference<Boolean> finite);
')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
    public static native boolean
      termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@(@TOPOLOGY@@CLASS@ p);
')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
    public static native boolean
      termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
        (@TOPOLOGY@@CLASS@ p_before, @TOPOLOGY@@CLASS@ p_after);
')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
    public static native boolean
      one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
        (@TOPOLOGY@@CLASS@ p, Generator g);
')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
    public static native boolean
      one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
        (@TOPOLOGY@@CLASS@ p_before, @TOPOLOGY@@CLASS@ p_after, Generator g);
')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
    public static native void
      all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
        (@TOPOLOGY@@CLASS@ p, Polyhedron ph);
')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
    public static native void
      all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
        (@TOPOLOGY@@CLASS@ p_before, @TOPOLOGY@@CLASS@ p_after, Polyhedron ph);
')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
    public static native void
      all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@
        (@TOPOLOGY@@CLASS@ p, Polyhedron ph_decreasing, Polyhedron ph_bounded);
')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2_code',
`dnl
___END_OF_FILE___
___BEGIN_OF_FILE___ Termination.java << ___END_OF_FILE___
    public static native void
      all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2
        (@TOPOLOGY@@CLASS@ p_before, @TOPOLOGY@@CLASS@ p_after,
         Polyhedron ph_decreasing, Polyhedron ph_bounded);
')
