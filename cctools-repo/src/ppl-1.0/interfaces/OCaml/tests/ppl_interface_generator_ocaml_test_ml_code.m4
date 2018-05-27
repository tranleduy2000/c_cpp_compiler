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
m4_divert(-1)

dnl Code for generating the domain elements to be used in the tests.
dnl Note that if a test may change these at all, then a copy should be used.
m4_define(`m4_add_init_class_code', `dnl
let @!TOPOLOGY@@!CLASS@01
  = ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension 3 Universe;;
ppl_@CLASS@_refine_with_@CONSTRAINER@s
  @!TOPOLOGY@@!CLASS@01 @CONSTRAINER@s01;;
let @!TOPOLOGY@@!CLASS@02
  = ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension 3 Universe;;
ppl_@CLASS@_refine_with_@CONSTRAINER@s
  @!TOPOLOGY@@!CLASS@02 @CONSTRAINER@s02;;
let @!TOPOLOGY@@!CLASS@03
  = ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension 3 Universe;;
ppl_@CLASS@_refine_with_@CONSTRAINER@s
  @!TOPOLOGY@@!CLASS@03 @CONSTRAINER@s03;;
print_string_if_noisy "\n";;
')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`
print_string_if_noisy "testing ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension" ;;
print_string_if_noisy "\n";;
print_string_if_noisy ("space_dimension: ");;
for i = 6 downto 0 do
  (let @!TOPOLOGY@@!CLASS@
    = ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension i Empty
    in let dimension
       =  ppl_@CLASS@_space_dimension(@!TOPOLOGY@@!CLASS@)
       in (print_int_if_noisy dimension;
     print_string_if_noisy "@COMMA@ "))
done;;
print_string_if_noisy "\n";;

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`
print_string_if_noisy "testing ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@: " ;;
let ps
  = ppl_new_@FRIEND@_from_space_dimension 3 Universe;;
let ps1
  = ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@(ps);;
let out = if (ppl_@A_FRIEND@_OK ps & ppl_@CLASS@_OK ps1)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`
print_string_if_noisy
  "testing ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity: " ;;
print_string_if_noisy "\n";;
let ps
  = ppl_new_@FRIEND@_from_space_dimension 3 Universe;;
let ps_pc
  = ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity ps Polynomial_Complexity ;;
let out = if (ppl_@A_FRIEND@_OK ps & ppl_@CLASS@_OK ps_pc)
  then "with Polynomial_Complexity - success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
let ps_sc
  = ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity ps Simplex_Complexity ;;
let out = if (ppl_@A_FRIEND@_OK ps & ppl_@CLASS@_OK ps_sc)
  then "with Simplex_Complexity - success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
let ps_ac
  = ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity ps Any_Complexity ;;
let out = if (ppl_@A_FRIEND@_OK ps & ppl_@CLASS@_OK ps_ac)
  then "with Any_Complexity - success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
`
print_string_if_noisy "testing ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s: " ;;
let @!TOPOLOGY@@!CLASS@
  = ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s(@BUILD_REPRESENT@s1);;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@UB_EXACT@: " ;;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy02
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let _result
  =  ppl_@CLASS@_@UB_EXACT@ copy01 copy02;;
let out
  = if (ppl_@CLASS@_OK copy01 & ppl_@CLASS@_OK copy02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_swap_code',
`
print_string_if_noisy "testing ppl_@CLASS@_swap: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy02
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_swap copy01 copy02;;
let out
  = if (u = () & ppl_@CLASS@_OK copy01 & ppl_@CLASS@_OK copy02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@DIMENSION@: " ;;
let dim
  =  ppl_@CLASS@_@DIMENSION@ @!TOPOLOGY@@!CLASS@01;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_string_if_noisy "@DIMENSION@: ";;
print_int_if_noisy dim ;;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_relation_with_@RELATION_REPRESENT@: " ;;
let _result
  = ppl_@CLASS@_relation_with_@RELATION_REPRESENT@
    @!TOPOLOGY@@!CLASS@01 @RELATION_REPRESENT@1;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
`
print_string_if_noisy "testing ppl_@CLASS@_get_@CLASS_REPRESENT@s: ";;
let @CLASS_REPRESENT@s = ppl_@CLASS@_get_@CLASS_REPRESENT@s
  @!TOPOLOGY@@!CLASS@01;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
List.iter print_@CLASS_REPRESENT@ @CLASS_REPRESENT@s;;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@HAS_PROPERTY@: ";;
let result = ppl_@CLASS@_@HAS_PROPERTY@ @!TOPOLOGY@@!CLASS@01;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_string_if_noisy "@HAS_PROPERTY@: ";;
print_bool_if_noisy result;;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@SIMPLIFY@: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_@SIMPLIFY@ copy01;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_bounds_from_@ABOVEBELOW@: " ;;
let _result
  =  ppl_@CLASS@_bounds_from_above @!TOPOLOGY@@!CLASS@01 e2;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@MAXMIN@: ";;
let (is_bounded, num, den, is_supremum)
  = ppl_@CLASS@_@MAXMIN@ @!TOPOLOGY@@!CLASS@01 e3;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_int_if_noisy(Z.to_int num);;
print_string_if_noisy "/";;
print_int_if_noisy(Z.to_int den);;
print_string_if_noisy (", bound: ");;
print_string_if_noisy (string_of_bool is_bounded);;
print_string_if_noisy (", sup: ");;
print_string_if_noisy (string_of_bool is_supremum);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@MAXMIN@_with_point: ";;
let (is_bounded, num, den, is_supremum, pt)
  = ppl_@CLASS@_@MAXMIN@_with_point @!TOPOLOGY@@!CLASS@01 e3;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_int_if_noisy(Z.to_int num);;
print_string_if_noisy "/";;
print_int_if_noisy(Z.to_int den);;
print_string_if_noisy (", bound: ");;
print_string_if_noisy (string_of_bool is_bounded);;
print_string_if_noisy (", sup: ");;
print_string_if_noisy (string_of_bool is_supremum);;
print_string_if_noisy (", ");;
print_generator(pt);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@COMPARISON@_@CLASS@: " ;;
let b = ppl_@CLASS@_@COMPARISON@_@CLASS@
  @!TOPOLOGY@@!CLASS@01 @!TOPOLOGY@@!CLASS@02;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01
               & ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_equals_@CLASS@: " ;;
let b = ppl_@CLASS@_equals_@CLASS@
  @!TOPOLOGY@@!CLASS@01 @!TOPOLOGY@@!CLASS@02;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01
               & ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_OK_code',
`
print_string_if_noisy "testing ppl_@CLASS@_OK: ";;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01
               & ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_add_@CLASS_REPRESENT@: " ;;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_add_@CLASS_REPRESENT@ copy01 @CLASS_REPRESENT@1;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
`
print_string_if_noisy "testing ppl_@CLASS@_add_@CLASS_REPRESENT@s: " ;;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_add_@CLASS_REPRESENT@s copy01 @CLASS_REPRESENT@s1;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
`
print_string_if_noisy
  "testing ppl_@CLASS@_refine_with_@REFINE_REPRESENT@: " ;;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_refine_with_@REFINE_REPRESENT@
  copy01 @REFINE_REPRESENT@01;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
`
print_string_if_noisy
  "testing ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s: " ;;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s
  copy01 @REFINE_REPRESENT@s01;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@BINOP@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@BINOP@: " ;;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy02
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_@BINOP@ copy01 copy02;;
let out
  = if (u = () & ppl_@CLASS@_OK copy01 & ppl_@CLASS@_OK copy02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
`
print_string_if_noisy "testing ppl_@CLASS@_simplify_using_context_assign: " ;;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy02
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let b = ppl_@CLASS@_simplify_using_context_assign copy01 copy02;;
let out
  = if (ppl_@CLASS@_OK copy01 & ppl_@CLASS@_OK copy02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@AFFIMAGE@: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_@AFFIMAGE@ copy01 1 ((Z.of_int 2) */ v2) (Z.from_int 2);;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_bounded_@AFFIMAGE@: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_bounded_@AFFIMAGE@ copy01 1
  ((Z.of_int 2) */ v2) v2 (Z.from_int 10);;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_generalized_@AFFIMAGE@: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_generalized_@AFFIMAGE@
  copy01 1 Equal_RS v1 (Z.from_int 10);;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
`
print_string_if_noisy "testing ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs
  copy01 ((Z.of_int 1) */ v0) Equal_RS (linear_expression_of_int 7);;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
`
print_string_if_noisy "testing ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence
  copy01 1 Equal_RS v1 (Z.from_int 1) (Z.from_int 10);;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
`
print_string_if_noisy "testing ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence
  copy01 ((Z.of_int 1) */ v0) Equal_RS (linear_expression_of_int 7) (Z.from_int 1);;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@ copy01 2;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
`
print_string_if_noisy "testing ppl_@CLASS@_remove_space_dimensions: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let dimensions_to_remove = [2;0];;
let u = ppl_@CLASS@_remove_space_dimensions copy01 dimensions_to_remove;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
`
print_string_if_noisy "testing ppl_@CLASS@_remove_higher_space_dimensions: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_remove_higher_space_dimensions copy01 1;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
`
print_string_if_noisy "testing ppl_@CLASS@_expand_space_dimension: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_expand_space_dimension copy01 1 1;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
`
print_string_if_noisy "testing ppl_@CLASS@_fold_space_dimensions: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let dimensions_to_fold = [1];;
let u = ppl_@CLASS@_fold_space_dimensions copy01 dimensions_to_fold 0;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
`
print_string_if_noisy "testing ppl_@CLASS@_map_space_dimensions: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let dimensions_to_map = [(0,1);(1,2);(2,0);];;
let u = ppl_@CLASS@_map_space_dimensions copy01 dimensions_to_map;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_constrains_code',
`
print_string_if_noisy "testing ppl_@CLASS@_constrains: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
ppl_@CLASS@_constrains copy01 1;;
let dimensions_to_map = [(0,1);(1,2);(2,0);];;
ppl_@CLASS@_map_space_dimensions copy01 dimensions_to_map;;
let out = if (ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
`
print_string_if_noisy "testing ppl_@CLASS@_unconstrain_space_dimension: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_unconstrain_space_dimension copy01 1;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
`
print_string_if_noisy "testing ppl_@CLASS@_unconstrain_space_dimensions: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let dimensions_to_unconstrain = [1];;
let u = ppl_@CLASS@_unconstrain_space_dimensions
  copy01 dimensions_to_unconstrain;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@MEMBYTES@: " ;;
let mb
  =  ppl_@CLASS@_@MEMBYTES@ @!TOPOLOGY@@!CLASS@01;;
let out = if (ppl_@CLASS@_OK @!TOPOLOGY@@!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_string_if_noisy "@MEMBYTES@: ";;
print_int_if_noisy mb;;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@WIDEN@_widening_assign: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let u = ppl_@CLASS@_@WIDEN@_widening_assign copy01 copy03 ;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let tokens
  = ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens copy01 copy03 5;;
let out = if (ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_string_if_noisy "tokens after widening = ";;
print_int_if_noisy tokens;;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_widening_assign_code',
`
print_string_if_noisy "testing ppl_@CLASS@_widening_assign: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let u = ppl_@CLASS@_widening_assign copy01 copy03 ;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code',
`
print_string_if_noisy "testing ppl_@CLASS@_widening_assign_with_tokens_code: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let tokens
  = ppl_@CLASS@_widening_assign_with_tokens copy01 copy03 5;;
let out = if (ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_string_if_noisy "tokens after widening = ";;
print_int_if_noisy tokens;;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
`
print_string_if_noisy
  "testing ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let u = ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign
  copy01 copy03 @CONSTRAINER@s04;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code',
`
print_string_if_noisy
  "testing ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let tokens
  = ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens
  copy01 copy03 @CONSTRAINER@s04 10;;
let out = if (ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_string_if_noisy "tokens after widening = ";;
print_int_if_noisy tokens;;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let u = ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign copy01 copy03 ;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let tokens
  = ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens
    copy01 copy03 5;;
let out = if (ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
print_string_if_noisy "tokens after extrapolation = ";;
print_int_if_noisy tokens;;
print_string_if_noisy "\n";;
')


m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@03);;
let u = ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign copy03 copy01 ;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')


m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
`
print_string_if_noisy "testing ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign: ";;
let copy01
  = ppl_new_@CLASS@_from_@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy03
  = ppl_new_@CLASS@_from_@CLASS@(@!CLASS@03);;
let u = ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign copy01 copy03 ;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
`
print_string_if_noisy "testing ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign: ";;
let copy01
  = ppl_new_@CLASS@_from_@CLASS@(@!CLASS@01);;
let copy03
  = ppl_new_@CLASS@_from_@CLASS@(@!CLASS@03);;
let u = ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign copy01 copy03 2;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
`
print_string_if_noisy "testing ppl_@BEGINEND@_iterator: " ;;
let it = ppl_@CLASS@_@BEGINEND@_iterator @!CLASS@01;;
let out = if (ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
`
print_string_if_noisy "testing ppl_@CLASS@_iterator_equals_iterator: " ;;
let it = ppl_@CLASS@_begin_iterator @!CLASS@01;;
let it_begin = ppl_@CLASS@_begin_iterator @!CLASS@01;;
let b = ppl_@CLASS@_iterator_equals_iterator it it_begin;;
let out = if (ppl_@CLASS@_OK @!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`
print_string_if_noisy "testing ppl_new_@CLASS@_iterator_from_iterator: " ;;
let it_begin = ppl_@CLASS@_begin_iterator @!CLASS@01;;
let it_copy = ppl_new_@CLASS@_iterator_from_iterator it_begin;;
let b = ppl_@CLASS@_iterator_equals_iterator it_copy it_begin;;
let out = if (ppl_@CLASS@_OK @!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
`
print_string_if_noisy "testing ppl_@CLASS@_@INCDEC@_iterator: " ;;
let itb = ppl_@CLASS@_begin_iterator @!CLASS@01;;
let it_begin = ppl_@CLASS@_begin_iterator @!CLASS@01;;
let ite = ppl_@CLASS@_end_iterator @!CLASS@01;;
let it_end = ppl_@CLASS@_end_iterator @!CLASS@01;;
let size = ppl_@CLASS@_size @!CLASS@01 in
if (size > 0)
  then (
    ppl_@CLASS@_increment_iterator itb;
    ppl_@CLASS@_decrement_iterator itb;
    ppl_@CLASS@_decrement_iterator ite;
    ppl_@CLASS@_increment_iterator ite;
  );;
let out = if ((ppl_@CLASS@_OK @!CLASS@01)
               & (ppl_@CLASS@_iterator_equals_iterator itb it_begin)
               & (ppl_@CLASS@_iterator_equals_iterator ite it_end))
   then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_get_disjunct_code',
`
print_string_if_noisy "testing ppl_@CLASS@_get_disjunct: " ;;
let it = ppl_@CLASS@_begin_iterator @!CLASS@01;;
let d = ppl_@CLASS@_get_disjunct it;;
let out = if (ppl_@CLASS@_OK @!CLASS@01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
`
print_string_if_noisy "testing ppl_@CLASS@_drop_disjunct: " ;;
let copy01
  = ppl_new_@CLASS@_from_@CLASS@(@!CLASS@01);;
let it = ppl_@CLASS@_begin_iterator copy01;;
let u = ppl_@CLASS@_drop_disjunct copy01 it;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
`
print_string_if_noisy "testing ppl_@CLASS@_drop_disjuncts: " ;;
let copy01
  = ppl_new_@CLASS@_from_@CLASS@(@!CLASS@01);;
let it1 = ppl_@CLASS@_begin_iterator copy01;;
let it2 = ppl_@CLASS@_begin_iterator copy01;;
let u = ppl_@CLASS@_drop_disjuncts copy01 it1 it2;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_ascii_dump_code',
`
print_string_if_noisy "testing ppl_@CLASS@_ascii_dump: " ;;
print_string_if_noisy (ppl_@CLASS@_ascii_dump @!TOPOLOGY@@!CLASS@01);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_linear_@PARTITION@: " ;;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy02
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let (intsn, ptt)
  =  ppl_@CLASS@_linear_partition copy01 copy02;;
let out
  = if (ppl_@CLASS@_OK copy01 & ppl_@CLASS@_OK copy02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code',
`
print_string_if_noisy "testing ppl_@CLASS@_linear_@PARTITION@: " ;;
let copy01
  = ppl_new_@CLASS@_from_@CLASS@(@!CLASS@01);;
let copy02
  = ppl_new_@CLASS@_from_@CLASS@(@!CLASS@01);;
let (intsn, ptt, is_finite)
  =  ppl_@CLASS@_approximate_partition copy01 copy02;;
let out
  = if (ppl_@CLASS@_OK copy01 & ppl_@CLASS@_OK copy02)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
`
print_string_if_noisy "testing ppl_@CLASS@_drop_some_non_integer_points: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let u = ppl_@CLASS@_drop_some_non_integer_points copy01 Any_Complexity ;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
`
print_string_if_noisy "testing ppl_@CLASS@_drop_some_non_integer_points: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let dimensions_integer = [1];;
let u = ppl_@CLASS@_drop_some_non_integer_points_2 copy01 dimensions_integer Any_Complexity ;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_@CLASS@_wrap_assign_code',
`
print_string_if_noisy "testing ppl_@CLASS@_wrap_assign: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let dimensions_integer = [1];;
let u = ppl_@CLASS@_wrap_assign copy01 [] Bits_8 Unsigned Overflow_Wraps
                                [] 0 1;;
let out = if (u = () & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`
print_string_if_noisy
  "testing ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let univ
  = ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension 2 Universe;;
let u = ppl_@CLASS@_concatenate_assign copy01 @!TOPOLOGY@@!CLASS@01;;
let result1 = ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@ univ;;
let result2
  = ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@ copy01;;
let out = if (not result1 & not result2 & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`
print_string_if_noisy
  "testing ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2: ";;
let copy01
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@01);;
let copy02
  = ppl_new_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@(@!TOPOLOGY@@!CLASS@02);;
let u = ppl_@CLASS@_concatenate_assign copy02 @!TOPOLOGY@@!CLASS@02;;
let result1
  = ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2 copy01 copy02;;
let univ01
  = ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension 2 Universe;;
let univ02
  = ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension 4 Universe;;
let result2
  = ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2 univ01 univ02;;
let out = if (not result1 & not result2 & ppl_@CLASS@_OK copy01)
  then "success" else "failed"
    in (print_string_if_noisy out);;
print_string_if_noisy "\n";;
')
