dnl  -*- C++ -*-
m4_divert(-1)

This m4 file contains the program implementation code for generating the
files ppl_ocaml_DOMAIN.cc for each interface domain DOMAIN
in ppl_interface instantiations.m4.

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

m4_define(`m4_custom_operations_class_code',
`dnl

namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace OCaml {

struct custom_operations m4_current_interface`'_custom_operations = {
  "com.bugseng.ppl" "." PPL_VERSION "." "m4_current_interface"|COMMA|
  custom_`'m4_current_interface`'_finalize|COMMA|
  custom_compare_default|COMMA|
  custom_hash_default|COMMA|
  custom_serialize_default|COMMA|
  custom_deserialize_default
};

} // namespace OCaml

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`dnl
extern "C"
CAMLprim value
ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension(value d, value caml_de) try {
  CAMLparam2(d, caml_de);
  dimension_type dd = value_to_ppl_dimension(d);
  Degenerate_Element ppl_de = build_ppl_Degenerate_Element(caml_de);
  @TOPOLOGY@@CPP_CLASS@& ppl_value = *new @TOPOLOGY@@CPP_CLASS@(dd, ppl_de);
  CAMLreturn(unregistered_value_p_@CLASS@(ppl_value));
}
CATCH_ALL

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
`dnl
extern "C"
CAMLprim value
ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s(value cl) try {
  CAMLparam1(cl);
  @!BUILD_REPRESENT@_System cs = build_ppl_@!BUILD_REPRESENT@_System(cl);
  @TOPOLOGY@@CPP_CLASS@& ppl_value = *new @TOPOLOGY@@CPP_CLASS@(cs);
  CAMLreturn(unregistered_value_p_@CLASS@(ppl_value));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_relation_with_@RELATION_REPRESENT@(value ph, value c) try {
  CAMLparam2(ph, c);
  const @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  @!RELATION_REPRESENT@ ppl_c = build_ppl_@!RELATION_REPRESENT@(c);
  Poly_@!A_RELATION_REPRESENT@_Relation r = pph.relation_with(ppl_c);
  CAMLreturn(build_ocaml_poly_@A_RELATION_REPRESENT@_relation(r));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@DIMENSION@(value ph) try {
  CAMLparam1(ph);
  const @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  dimension_type d = pph.@DIMENSION@();
  CAMLreturn(ppl_dimension_to_value(d));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@HAS_PROPERTY@(value ph) try {
  CAMLparam1(ph);
  const @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  CAMLreturn(Val_bool(pph.@HAS_PROPERTY@()));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@SIMPLIFY@(value ph) try {
  CAMLparam1(ph);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.@SIMPLIFY@();
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_bounds_from_@ABOVEBELOW@(value ph, value le) try {
  CAMLparam2(ph, le);
  const @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  Linear_Expression ple = build_ppl_Linear_Expression(le);
  CAMLreturn(Val_bool(pph.bounds_from_@ABOVEBELOW@(ple)));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_add_@CLASS_REPRESENT@(value ph, value c) try {
  CAMLparam2(ph, c);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  @!CLASS_REPRESENT@ pc = build_ppl_@!CLASS_REPRESENT@(c);
  pph.add_@CLASS_REPRESENT@(pc);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_add_@CLASS_REPRESENT@s(value ph, value cs) try {
  CAMLparam2(ph, cs);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  @!CLASS_REPRESENT@_System pcs = build_ppl_@!CLASS_REPRESENT@_System(cs);
  pph.add_@CLASS_REPRESENT@s(pcs);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_refine_with_@REFINE_REPRESENT@(value ph, value c) try {
  CAMLparam2(ph, c);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  @!REFINE_REPRESENT@ pc = build_ppl_@!REFINE_REPRESENT@(c);
  pph.refine_with_@REFINE_REPRESENT@(pc);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s(value ph, value cs) try {
  CAMLparam2(ph, cs);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  @!REFINE_REPRESENT@_System pcs = build_ppl_@!REFINE_REPRESENT@_System(cs);
  pph.refine_with_@REFINE_REPRESENT@s(pcs);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@COMPARISON@_@CLASS@(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  const @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  const @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  CAMLreturn(Val_bool(pph1.@COMPARISON@(pph2)));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_equals_@CLASS@(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  const @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  const @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  CAMLreturn(Val_bool(pph1 == pph2));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@BINOP@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@BINOP@(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  pph1.@BINOP@(pph2);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_simplify_using_context_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  CAMLreturn(Val_bool(pph1.simplify_using_context_assign(pph2)));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@
(value ph, value d) try {
  CAMLparam2(ph, d);
  dimension_type dd = value_to_ppl_dimension(d);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.add_space_dimensions_and_embed(dd);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_remove_space_dimensions(value ph, value caml_vset) try {
  CAMLparam2(ph, caml_vset);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.remove_space_dimensions(build_ppl_Variables_Set(caml_vset));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_remove_higher_space_dimensions(value ph, value d) try {
  CAMLparam2(ph, d);
  dimension_type dd = value_to_ppl_dimension(d);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.remove_higher_space_dimensions(dd);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_fold_space_dimensions
(value ph, value caml_vset, value caml_dim) try {
  CAMLparam1(ph);
  dimension_type ppl_dim = Int_val(caml_dim);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  Variables_Set ppl_vset;
  if (Int_val(caml_vset) == 0)
    CAMLreturn(Val_unit);
  while (true) {
    ppl_vset.insert(Int_val(Field(caml_vset, 0)));
    if (Int_val(Field(caml_vset, 1)) == 0)
      break;
    caml_vset = Field(caml_vset, 1);
  }
  pph.fold_space_dimensions(ppl_vset, Variable(ppl_dim));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_map_space_dimensions(value ph, value caml_mapped_dims) try {
  CAMLparam2(ph, caml_mapped_dims);
  CAMLlocal1(head);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  Partial_Function pfunc;
  while (caml_mapped_dims != Val_emptylist) {
    head = Field(caml_mapped_dims, 0);
    dimension_type domain_dim = value_to_ppl_dimension(Field(head, 0));
    dimension_type codomain_dim = value_to_ppl_dimension(Field(head, 1));
    pfunc.insert(domain_dim, codomain_dim);
    caml_mapped_dims = Field(caml_mapped_dims, 1);
  }
  pph.map_space_dimensions(pfunc);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')


m4_define(`ppl_@CLASS@_expand_space_dimension_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_expand_space_dimension
(value ph, value var_index, value m) try {
  CAMLparam3(ph, var_index, m);
  Variable var = build_ppl_Variable(var_index);
  dimension_type c_m = value_to_ppl_dimension(m);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.expand_space_dimension(var, c_m);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_drop_some_non_integer_points
(value ph, value caml_cc) try {
  CAMLparam1(ph);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  Complexity_Class ppl_cc = build_ppl_Complexity_Class(caml_cc);
  pph.drop_some_non_integer_points(ppl_cc);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_drop_some_non_integer_points_2
(value ph, value caml_vset, value caml_cc) try {
  CAMLparam1(ph);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  Variables_Set ppl_vset;
  if (Int_val(caml_vset) == 0)
    CAMLreturn(Val_unit);
  while (true) {
    ppl_vset.insert(Int_val(Field(caml_vset, 0)));
    if (Int_val(Field(caml_vset, 1)) == 0)
      break;
    caml_vset = Field(caml_vset, 1);
  }
  Complexity_Class ppl_cc = build_ppl_Complexity_Class(caml_cc);
  pph.drop_some_non_integer_points(ppl_vset, ppl_cc);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_get_@CLASS_REPRESENT@s(value ph) try {
  CAMLparam1(ph);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  CAMLreturn(build_ocaml_@CLASS_REPRESENT@_system(pph.@CLASS_REPRESENT@s()));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s(value ph) try {
  CAMLparam1(ph);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  CAMLreturn(build_ocaml_@CLASS_REPRESENT@_system(pph.minimized_@CLASS_REPRESENT@s()));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_constrains_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_constrains(value ph, value var) try {
  CAMLparam2(ph, var);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  CAMLreturn(Val_bool(pph.constrains(build_ppl_Variable(var))));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_unconstrain_space_dimension(value ph, value var) try {
  CAMLparam2(ph, var);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.unconstrain(build_ppl_Variable(var));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_unconstrain_space_dimensions(value ph, value caml_vset) try {
  CAMLparam2(ph, caml_vset);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.unconstrain(build_ppl_Variables_Set(caml_vset));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_bounded_@AFFIMAGE@(value ph, value var, value lb_expr,
				    value ub_expr, value coeff) try {
  CAMLparam5(ph, var, lb_expr, ub_expr, coeff);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.bounded_@AFFIMAGE@(build_ppl_Variable(var),
			   build_ppl_Linear_Expression(lb_expr),
 			   build_ppl_Linear_Expression(ub_expr),
 			   build_ppl_Coefficient(coeff));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@AFFIMAGE@
(value ph, value var, value expr, value coeff) try {
  CAMLparam4(ph, var, expr, coeff);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  build_ppl_Linear_Expression(expr);
  pph.@AFFIMAGE@(build_ppl_Variable(var),
		   build_ppl_Linear_Expression(expr),
		   build_ppl_Coefficient(coeff));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_generalized_@AFFIMAGE@1
(value ph, value le1, value rel_sym, value le2) try {
  CAMLparam4(ph, le1, rel_sym, le2);
  build_ppl_relsym(rel_sym);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.generalized_@AFFIMAGE@(build_ppl_Linear_Expression(le1),
			       build_ppl_relsym(rel_sym),
			       build_ppl_Linear_Expression(le2));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_generalized_@AFFIMAGE@2
(value ph, value int_val, value rel_sym, value le, value caml_coeff) try {
  CAMLparam5(ph, int_val, rel_sym, le, caml_coeff);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.generalized_@AFFIMAGE@(build_ppl_Variable(int_val),
			       build_ppl_relsym(rel_sym),
			       build_ppl_Linear_Expression(le),
			       build_ppl_Coefficient(caml_coeff));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_generalized_@AFFIMAGE@1_with_congruence
(value ph, value int_val, value rel_sym, value le,
 value caml_coeff, value caml_modulus) try {
  CAMLparam5(ph, int_val, rel_sym, le, caml_coeff);
  CAMLxparam1(caml_modulus);
  build_ppl_relsym(rel_sym);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.generalized_@AFFIMAGE@(build_ppl_Variable(int_val),
			       build_ppl_relsym(rel_sym),
			       build_ppl_Linear_Expression(le),
			       build_ppl_Coefficient(caml_coeff),
			       build_ppl_Coefficient(caml_modulus));
  CAMLreturn(Val_unit);
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_@CLASS@_generalized_@AFFIMAGE@1_with_congruence_bytecode
(value* argv, int /*argn*/) try {
  return ppl_@CLASS@_generalized_@AFFIMAGE@1_with_congruence
    (argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_generalized_@AFFIMAGE@1_lhs_rhs_with_congruence
(value ph, value le1, value rel_sym, value le2, value caml_modulus) try {
  CAMLparam5(ph, le1, rel_sym, le2, caml_modulus);
  build_ppl_relsym(rel_sym);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  pph.generalized_@AFFIMAGE@(build_ppl_Linear_Expression(le1),
			     build_ppl_relsym(rel_sym),
			     build_ppl_Linear_Expression(le2),
			     build_ppl_Coefficient(caml_modulus));
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@WIDEN@_widening_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  pph1.@WIDEN@_widening_assign(pph2);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')


m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens
(value ph1, value ph2, value tokens) try {
  CAMLparam3(ph1, ph2, tokens);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  unsigned u_tokens = value_to_unsigned<unsigned>(tokens);
  pph1.@WIDEN@_widening_assign(pph2, &u_tokens);
  CAMLreturn(Val_long(u_tokens));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_widening_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_widening_assign(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  pph1.widening_assign(pph2);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_widening_assign_with_tokens
(value ph1, value ph2, value tokens) try {
  CAMLparam3(ph1, ph2, tokens);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  unsigned u_tokens = value_to_unsigned<unsigned>(tokens);
  pph1.widening_assign(pph2, &u_tokens);
  CAMLreturn(Val_long(u_tokens));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens
(value ph1, value ph2, value caml_cs, value tokens) try {
  CAMLparam4(ph1, ph2, caml_cs, tokens);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  @!CONSTRAINER@_System ppl_cs = build_ppl_@!CONSTRAINER@_System(caml_cs);
  unsigned u_tokens = value_to_unsigned<unsigned>(tokens);
  pph1.@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(pph2, ppl_cs,
							 &u_tokens);
  CAMLreturn(Val_long(u_tokens));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(value ph1,
						   value ph2,
						   value caml_cs) try {
  CAMLparam3(ph1, ph2, caml_cs);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  @!CONSTRAINER@_System ppl_cs = build_ppl_@!CONSTRAINER@_System(caml_cs);
  pph1.@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(pph2, ppl_cs);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_has_@UPPERLOWER@_bound(value ph, value var) try {
  CAMLparam2(ph, var);
  CAMLlocal1(caml_return_value);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  Variable vv = build_ppl_Variable(var);
  PPL_DIRTY_TEMP_COEFFICIENT(num);
  PPL_DIRTY_TEMP_COEFFICIENT(den);
  bool is_closed = false;
  bool ppl_return_value = pph.has_@UPPERLOWER@_bound(vv, num, den, is_closed);
  caml_return_value = caml_alloc(4, 0);
  Store_field(caml_return_value, 0, Val_bool(ppl_return_value));
  Store_field(caml_return_value, 1, build_ocaml_coefficient(num));
  Store_field(caml_return_value, 2, build_ocaml_coefficient(den));
  Store_field(caml_return_value, 3, Val_bool(is_closed));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@MAXMIN@(value ph, value caml_le) try {
  CAMLparam2(ph, caml_le);
  CAMLlocal1(caml_return_value);
  PPL_DIRTY_TEMP_COEFFICIENT(num);
  PPL_DIRTY_TEMP_COEFFICIENT(den);
  bool is_supremum = false;
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  bool ppl_return_value = pph.@MAXMIN@(build_ppl_Linear_Expression(caml_le),
				      num, den, is_supremum);
  caml_return_value = caml_alloc(4, 0);
  Store_field(caml_return_value, 0, Val_bool(ppl_return_value));
  Store_field(caml_return_value, 1, build_ocaml_coefficient(num));
  Store_field(caml_return_value, 2, build_ocaml_coefficient(den));
  Store_field(caml_return_value, 3, Val_bool(is_supremum));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@MAXMIN@_with_point(value ph, value caml_le) try {
  CAMLparam2(ph, caml_le);
  CAMLlocal1(caml_return_value);
  PPL_DIRTY_TEMP_COEFFICIENT(num);
  PPL_DIRTY_TEMP_COEFFICIENT(den);
  bool is_supremum = false;
  Generator g = point();
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  bool ppl_return_value = pph.@MAXMIN@(build_ppl_Linear_Expression(caml_le),
				      num, den, is_supremum, g);
  caml_return_value = caml_alloc(5, 0);
  Store_field(caml_return_value, 0, Val_bool(ppl_return_value));
  Store_field(caml_return_value, 1, build_ocaml_coefficient(num));
  Store_field(caml_return_value, 2, build_ocaml_coefficient(den));
  Store_field(caml_return_value, 3, Val_bool(is_supremum));
  Store_field(caml_return_value, 4, build_ocaml_generator(g));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_frequency_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_frequency(value ph, value caml_le) try {
  CAMLparam2(ph, caml_le);
  CAMLlocal1(caml_return_value);
  PPL_DIRTY_TEMP_COEFFICIENT(f_num);
  PPL_DIRTY_TEMP_COEFFICIENT(f_den);
  PPL_DIRTY_TEMP_COEFFICIENT(v_num);
  PPL_DIRTY_TEMP_COEFFICIENT(v_den);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  bool ppl_return_value = pph.frequency(build_ppl_Linear_Expression(caml_le),
				        f_num, f_den, v_num, v_den);
  caml_return_value = caml_alloc(5, 0);
  Store_field(caml_return_value, 0, Val_bool(ppl_return_value));
  Store_field(caml_return_value, 1, build_ocaml_coefficient(f_num));
  Store_field(caml_return_value, 2, build_ocaml_coefficient(f_den));
  Store_field(caml_return_value, 3, build_ocaml_coefficient(v_num));
  Store_field(caml_return_value, 4, build_ocaml_coefficient(v_den));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_OK_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_OK(value ph) try {
  CAMLparam1(ph);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  CAMLreturn(Val_bool(pph.OK()));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@MEMBYTES@(value ph) try {
  CAMLparam1(ph);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  CAMLreturn(Val_long(pph.@MEMBYTES@()));
}
CATCH_ALL

 ')

m4_define(`ppl_@CLASS@_swap_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_swap(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  swap(pph1, pph2);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`dnl
extern "C"
CAMLprim value
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@(value ph) try {
  CAMLparam1(ph);
  @B_FRIEND@& pph = *(reinterpret_cast<@B_FRIEND@*>( p_@A_FRIEND@_val(ph)));
  @CPP_CLASS@& ppl_value
    = *(reinterpret_cast<@CPP_CLASS@*>(new @TOPOLOGY@@CPP_CLASS@(pph)));
  CAMLreturn(unregistered_value_p_@CLASS@(ppl_value));
}
CATCH_ALL

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`dnl
extern "C"
CAMLprim value
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity(value ph, value caml_cc) try {
  CAMLparam1(ph);
  @B_FRIEND@& pph = *(reinterpret_cast<@B_FRIEND@*>( p_@A_FRIEND@_val(ph)));
  Complexity_Class ppl_cc = build_ppl_Complexity_Class(caml_cc);
  @CPP_CLASS@& ppl_value
    = *(reinterpret_cast<@CPP_CLASS@*>(new @TOPOLOGY@@CPP_CLASS@(pph, ppl_cc)));
  CAMLreturn(unregistered_value_p_@CLASS@(ppl_value));
}
CATCH_ALL

')


m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@UB_EXACT@(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
`m4_ifelse(m4_current_interface, `Polyhedron',
          `m4_ub_exact_for_polyhedron_domains',
          `m4_ub_exact_for_non_polyhedron_domains')'dnl
}
CATCH_ALL

')

m4_define(`m4_ub_exact_for_polyhedron_domains',
`dnl
  if (Interfaces::is_necessarily_closed_for_interfaces
        (*p_Polyhedron_val(ph1))) {
    C_Polyhedron& xx
      = static_cast<C_Polyhedron&>(*p_Polyhedron_val(ph1));
    const C_Polyhedron& yy
      = static_cast<const C_Polyhedron&>(*p_Polyhedron_val(ph2));
    CAMLreturn(Val_bool(xx.upper_bound_assign_if_exact(yy)));
  }
  else {
    NNC_Polyhedron& xx
      = static_cast<NNC_Polyhedron&>(*p_Polyhedron_val(ph1));
    const NNC_Polyhedron& yy
      = static_cast<const NNC_Polyhedron&>(*p_Polyhedron_val(ph2));
    CAMLreturn(Val_bool(xx.upper_bound_assign_if_exact(yy)));
  }
')

m4_define(`m4_ub_exact_for_non_polyhedron_domains',
`dnl
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  const @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);

  CAMLreturn(Val_bool(pph1.@UB_EXACT@(pph2)));
')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign
(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  pph1.@EXTRAPOLATION@_extrapolation_assign(pph2);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')


m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens
(value ph1, value ph2, value tokens) try {
  CAMLparam3(ph1, ph2, tokens);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  unsigned u_tokens = value_to_unsigned<unsigned>(tokens);
  pph1.@EXTRAPOLATION@_extrapolation_assign(pph2, &u_tokens);
  CAMLreturn(Val_long(u_tokens));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign(value ph1,
                                                       value ph2) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  const @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  pph1.@EXTRAPOLATION@_narrowing_assign(pph2);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@BEGINEND@_iterator(value t_pps) try {
  CAMLparam1(t_pps);
  @CPP_CLASS@& pps = *p_@CLASS@_val(t_pps);
  @CPP_CLASS@::iterator& ppl_value
    = *new @CPP_CLASS@::iterator(pps.@BEGINEND@());
  CAMLreturn(unregistered_value_p_@CLASS@_iterator(ppl_value));
}
CATCH_ALL

')


m4_define(`ppl_@CLASS@_get_disjunct_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_get_disjunct(value caml_it) try {
  CAMLparam1(caml_it);
  CAMLlocal1(caml_return_value);
  @CPP_CLASS@::iterator& cpp_it  = *p_@CLASS@_iterator_val(caml_it);
  const @DISJUNCT_TOPOLOGY@@A_DISJUNCT@& disjunct = cpp_it->pointset();
  caml_return_value = unregistered_value_p_@DISJUNCT@(disjunct);
  actual_p_@DISJUNCT@_val(caml_return_value)
    = const_cast<@DISJUNCT_TOPOLOGY@@A_DISJUNCT@*>(mark(&disjunct));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_add_disjunct(value t_pps, value caml_item_to_add) try {
  CAMLparam2(t_pps, caml_item_to_add);
  @CPP_CLASS@& pps = *p_@CLASS@_val(t_pps);
  @DISJUNCT_TOPOLOGY@@A_DISJUNCT@& item
    = *(reinterpret_cast<@DISJUNCT_TOPOLOGY@@A_DISJUNCT@*>
          (p_@DISJUNCT@_val(caml_item_to_add)));
  pps.add_disjunct(item);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_drop_disjunct(value t_pps, value caml_item_to_drop) try {
  CAMLparam2(t_pps, caml_item_to_drop);
  @CPP_CLASS@& pps = *p_@CLASS@_val(t_pps);
  @CPP_CLASS@::iterator& itr = *p_@CLASS@_iterator_val(caml_item_to_drop);
  itr = pps.drop_disjunct(itr);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_drop_disjuncts
(value t_pps, value caml_item1_to_drop, value caml_item2_to_drop) try {
  CAMLparam3(t_pps, caml_item1_to_drop, caml_item2_to_drop);
  @CPP_CLASS@& pps = *p_@CLASS@_val(t_pps);
  @CPP_CLASS@::iterator& itr1 = *p_@CLASS@_iterator_val(caml_item1_to_drop);
  @CPP_CLASS@::iterator& itr2 = *p_@CLASS@_iterator_val(caml_item2_to_drop);
  pps.drop_disjuncts(itr1, itr2);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`dnl
//! Give access to the embedded @CLASS@* in \p v.
inline @CPP_CLASS@::iterator*&
p_@CLASS@_iterator_val(value v) {
  return *reinterpret_cast<@CPP_CLASS@::iterator**>(Data_custom_val(v));
}

void
custom_@CLASS@_iterator_finalize(value v) {
  delete p_@CLASS@_iterator_val(v);
}

struct custom_operations @CLASS@_iterator_custom_operations = {
  "com.bugseng.ppl" "." PPL_VERSION "." "@CLASS@_iterator"|COMMA|
  custom_@CLASS@_iterator_finalize|COMMA|
  custom_compare_default|COMMA|
  custom_hash_default|COMMA|
  custom_serialize_default|COMMA|
  custom_deserialize_default
};

inline value
unregistered_value_p_@CLASS@_iterator(const @CPP_CLASS@::iterator& ph) {
  value v = caml_alloc_custom(&@CLASS@_iterator_custom_operations,
                              sizeof(@CPP_CLASS@::iterator*), 0, 1);
  p_@CLASS@_iterator_val(v) = const_cast<@CPP_CLASS@::iterator*>(&ph);
  return v;
}

extern "C"
CAMLprim value
ppl_new_@CLASS@_iterator_from_iterator(value y) try {
  CAMLparam1(y);
  @CPP_CLASS@::iterator& yy
    = *(reinterpret_cast<@CPP_CLASS@::iterator*>( p_@CLASS@_iterator_val(y)));
  @CPP_CLASS@::iterator& ppl_value
    = *(reinterpret_cast<@CPP_CLASS@::iterator*>(new @CPP_CLASS@::iterator(yy)));
  CAMLreturn(unregistered_value_p_@CLASS@_iterator(ppl_value));
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_@INCDEC@_iterator(value caml_itr) try {
  CAMLparam1(caml_itr);
  @CPP_CLASS@::iterator& itr = *p_@CLASS@_iterator_val(caml_itr);
  m4_@INCDEC@_cpp_name()itr;
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`m4_increment_cpp_name', `++')
m4_define(`m4_decrement_cpp_name', `--')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_iterator_equals_iterator(value caml_itr1, value caml_itr2) try {
  CAMLparam2(caml_itr1, caml_itr2);
  @CPP_CLASS@::iterator& itr1 = *p_@CLASS@_iterator_val(caml_itr1);
  @CPP_CLASS@::iterator& itr2 = *p_@CLASS@_iterator_val(caml_itr2);
  if (itr1 == itr2)
    CAMLreturn(Val_bool(true));
  else
    CAMLreturn(Val_bool(false));
}
CATCH_ALL

')


m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign
(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  pph1.BHZ03_widening_assign<@A_DISJUNCT_WIDEN@_Certificate>
    (pph2,
     widen_fun_ref(&@DISJUNCT_TOPOLOGY@@A_DISJUNCT@::@DISJUNCT_WIDEN@_widening_assign));;
  CAMLreturn(Val_unit);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign
(value ph1, value ph2, value max_disj) try {
  CAMLparam2(ph1, ph2);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  @CPP_CLASS@& pph2 = *p_@CLASS@_val(ph2);
  unsigned cpp_max_disj
    = value_to_unsigned<unsigned>(max_disj);
  pph1.BGP99_extrapolation_assign
    (pph2,
     widen_fun_ref(&@DISJUNCT_TOPOLOGY@@A_DISJUNCT@::@DISJUNCT_WIDEN@_widening_assign),
     cpp_max_disj);
  CAMLreturn(Val_unit);
}
CATCH_ALL

')


m4_define(`ppl_@CLASS@_ascii_dump_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_ascii_dump(value ph1) try {
  CAMLparam1(ph1);
  @CPP_CLASS@& pph1 = *p_@CLASS@_val(ph1);
  std::ostringstream s;
  pph1.ascii_dump(s);
  CAMLreturn(caml_copy_string(s.str().c_str()));
}
CATCH_ALL

')


m4_define(`ppl_@CLASS@_linear_@PARTITION@_code', `
extern "C"
CAMLprim value
ppl_@CLASS@_linear_@PARTITION@(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  CAMLlocal1(caml_return_value);
  @CPP_CLASS@* rfh;
  Pointset_Powerset<NNC_Polyhedron>* rsh;
`m4_ifelse(m4_current_interface, `Polyhedron',
           `m4_linear_partition_for_polyhedron_domains',
           `m4_linear_partition_for_non_polyhedron_domains')'dnl
  caml_return_value = caml_alloc(2, 0);
  Store_field(caml_return_value, 0,
              unregistered_value_p_@CLASS@(*rfh));
  Store_field(caml_return_value, 1,
              unregistered_value_p_Pointset_Powerset_NNC_Polyhedron(*rsh));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`m4_linear_partition_for_polyhedron_domains',
`dnl
  if (Interfaces::is_necessarily_closed_for_interfaces
       (*p_Polyhedron_val(ph1))) {
    C_Polyhedron& pph1
      = reinterpret_cast<C_Polyhedron&>(*p_Polyhedron_val(ph1));
    C_Polyhedron& pph2
      = reinterpret_cast<C_Polyhedron&>(*p_Polyhedron_val(ph2));
    std::pair<C_Polyhedron|COMMA| Pointset_Powerset<NNC_Polyhedron> >
      r = linear_partition(pph1, pph2);
    rfh = new C_Polyhedron(0, EMPTY);
    rsh = new Pointset_Powerset<NNC_Polyhedron>(0, EMPTY);
    swap(*rfh, r.first);
    swap(*rsh, r.second);
  }
  else {
    NNC_Polyhedron& pph1
      = reinterpret_cast<NNC_Polyhedron&>(*p_Polyhedron_val(ph1));
    NNC_Polyhedron& pph2
      = reinterpret_cast<NNC_Polyhedron&>(*p_Polyhedron_val(ph2));
    std::pair<NNC_Polyhedron|COMMA| Pointset_Powerset<NNC_Polyhedron> >
      r = linear_partition(pph1, pph2);
    rfh = new NNC_Polyhedron(0, EMPTY);
    rsh = new Pointset_Powerset<NNC_Polyhedron>(0, EMPTY);
    swap(*rfh, r.first);
    swap(*rsh, r.second);
  }
')

m4_define(`m4_linear_partition_for_non_polyhedron_domains',
`dnl
  @CPP_CLASS@& pph1
    = reinterpret_cast<@CPP_CLASS@&>(*p_@CLASS@_val(ph1));
  @CPP_CLASS@& pph2
    = reinterpret_cast<@CPP_CLASS@&>(*p_@CLASS@_val(ph2));
  std::pair<@CPP_CLASS@|COMMA| Pointset_Powerset<NNC_Polyhedron> >
    r = linear_partition(pph1, pph2);
  rfh = new @CPP_CLASS@(0, EMPTY);
  rsh = new Pointset_Powerset<NNC_Polyhedron>(0, EMPTY);
  swap(*rfh, r.first);
  swap(*rsh, r.second);
')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code', `
extern "C"
CAMLprim value
ppl_@CLASS@_approximate_@PARTITION@(value ph1, value ph2) try {
  CAMLparam2(ph1, ph2);
  CAMLlocal1(caml_return_value);
  @CPP_CLASS@& pph1
    = reinterpret_cast<@CPP_CLASS@&>(*p_@CPP_CLASS@_val(ph1));
  @CPP_CLASS@& pph2
    = reinterpret_cast<@CPP_CLASS@&>(*p_@CPP_CLASS@_val(ph2));
  bool is_finite = false;
  std::pair<@CPP_CLASS@|COMMA| Pointset_Powerset<Grid> >
    r = approximate_partition(pph1, pph2, is_finite);
  @CPP_CLASS@* rfh = new @CPP_CLASS@(0, EMPTY);
  Pointset_Powerset<Grid>* rsh = new Pointset_Powerset<Grid>(0, EMPTY);
  swap(*rfh, r.first);
  swap(*rsh, r.second);
  caml_return_value = caml_alloc(3, 0);
  Store_field(caml_return_value, 0,
              unregistered_value_p_@CLASS@(*rfh));
  Store_field(caml_return_value, 1,
              unregistered_value_p_Pointset_Powerset_Grid(*rsh));
  Store_field(caml_return_value, 2, Val_bool(is_finite));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
extern "C"
CAMLprim value
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@(value pset) try {
  CAMLparam1(pset);
  const @TOPOLOGY@@CPP_CLASS@& ppset
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset)));
  CAMLreturn(Val_bool(termination_test_@TERMINATION_ID@(ppset)));
}
CATCH_ALL

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
extern "C"
CAMLprim value
  ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2(value pset_before,
                                                            value pset_after)
  try {
    CAMLparam2(pset_before, pset_after);
  const @TOPOLOGY@@CPP_CLASS@& ppset_before
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset_before)));
  const @TOPOLOGY@@CPP_CLASS@& ppset_after
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset_after)));
  CAMLreturn(Val_bool(termination_test_@TERMINATION_ID@_2(ppset_before,
                                                          ppset_after)));
}
CATCH_ALL

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
extern "C"
CAMLprim value
  ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
    (value pset)
  try {
    CAMLparam1(pset);
    CAMLlocal1(caml_return_value);
  const @TOPOLOGY@@CPP_CLASS@& ppset
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset)));
  Generator g(point());
  bool ppl_return_value = one_affine_ranking_function_@TERMINATION_ID@
    (ppset, g);
  caml_return_value = caml_alloc(2, 0);
  Store_field(caml_return_value, 0, Val_bool(ppl_return_value));
  Store_field(caml_return_value, 1, build_ocaml_generator(g));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
extern "C"
CAMLprim value
  ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
  (value pset_before, value pset_after)
  try {
    CAMLparam2(pset_before, pset_after);
    CAMLlocal1(caml_return_value);
  const @TOPOLOGY@@CPP_CLASS@& ppset_before
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset_before)));
  const @TOPOLOGY@@CPP_CLASS@& ppset_after
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset_after)));
  Generator g(point());
  bool ppl_return_value = one_affine_ranking_function_@TERMINATION_ID@_2
    (ppset_before, ppset_after, g);
  caml_return_value = caml_alloc(2, 0);
  Store_field(caml_return_value, 0, Val_bool(ppl_return_value));
  Store_field(caml_return_value, 1, build_ocaml_generator(g));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
extern "C"
CAMLprim value
  ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
  (value pset)
  try {
    CAMLparam1(pset);
    CAMLlocal1(caml_return_value);
  const @TOPOLOGY@@CPP_CLASS@& ppset
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset)));
  @A_TERMINATION_ID@Polyhedron* ph = new @A_TERMINATION_ID@Polyhedron();
  all_affine_ranking_functions_@TERMINATION_ID@(ppset, *ph);
  CAMLreturn(unregistered_value_p_Polyhedron(*ph));
}
CATCH_ALL

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
extern "C"
CAMLprim value
  ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
  (value pset_before, value pset_after)
  try {
    CAMLparam2(pset_before, pset_after);
    CAMLlocal1(caml_return_value);
  const @TOPOLOGY@@CPP_CLASS@& ppset_before
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset_before)));
  const @TOPOLOGY@@CPP_CLASS@& ppset_after
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset_after)));
  @A_TERMINATION_ID@Polyhedron* ph = new @A_TERMINATION_ID@Polyhedron();
  all_affine_ranking_functions_@TERMINATION_ID@_2(ppset_before, ppset_after, *ph);
  CAMLreturn(unregistered_value_p_Polyhedron(*ph));
}
CATCH_ALL

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_code',
`dnl
extern "C"
CAMLprim value
  ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@
  (value pset)
  try {
    CAMLparam1(pset);
    CAMLlocal1(caml_return_value);
  const @TOPOLOGY@@CPP_CLASS@& ppset
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset)));
  C_Polyhedron* ph_decreasing = new C_Polyhedron();
  C_Polyhedron* ph_bounded = new C_Polyhedron();
  all_affine_quasi_ranking_functions_MS(ppset, *ph_decreasing, *ph_bounded);
  caml_return_value = caml_alloc(2, 0);
  Store_field(caml_return_value, 0,
              unregistered_value_p_Polyhedron(*ph_decreasing));
  Store_field(caml_return_value, 1,
              unregistered_value_p_Polyhedron(*ph_bounded));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2_code',
`dnl
extern "C"
CAMLprim value
  ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2
  (value pset_before, value pset_after)
  try {
    CAMLparam2(pset_before, pset_after);
    CAMLlocal1(caml_return_value);
  const @TOPOLOGY@@CPP_CLASS@& ppset_before
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset_before)));
  const @TOPOLOGY@@CPP_CLASS@& ppset_after
     = *(reinterpret_cast<@TOPOLOGY@@CPP_CLASS@*>(p_@CLASS@_val(pset_after)));
  C_Polyhedron* ph_decreasing = new C_Polyhedron();
  C_Polyhedron* ph_bounded = new C_Polyhedron();
  all_affine_quasi_ranking_functions_MS_2(ppset_before, ppset_after,
                                          *ph_decreasing, *ph_bounded);
  caml_return_value = caml_alloc(2, 0);
  Store_field(caml_return_value, 0,
              unregistered_value_p_Polyhedron(*ph_decreasing));
  Store_field(caml_return_value, 1,
              unregistered_value_p_Polyhedron(*ph_bounded));
  CAMLreturn(caml_return_value);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_wrap_assign_code',
`dnl
extern "C"
CAMLprim value
ppl_@CLASS@_wrap_assign_native
  (value ph, value caml_vset, value width, value rep, value oflow,
   value cs, value complexity, value wrap_ind) try {
  CAMLparam5(ph, caml_vset, width, rep, oflow);
  CAMLxparam3(cs, complexity, wrap_ind);
  @CPP_CLASS@& pph = *p_@CLASS@_val(ph);
  Variables_Set ppl_vset;
  if (Int_val(caml_vset) == 0)
    CAMLreturn(Val_unit);
  while (true) {
    ppl_vset.insert(Int_val(Field(caml_vset, 0)));
    if (Int_val(Field(caml_vset, 1)) == 0)
      break;
    caml_vset = Field(caml_vset, 1);
  }
  build_ppl_bounded_integer_type_representation(rep);
  build_ppl_bounded_integer_type_overflow(oflow);
  Constraint_System ppl_cs = build_ppl_Constraint_System(cs);
  unsigned ppl_complexity = value_to_unsigned<unsigned>(complexity);
  bool ppl_wrap_ind = Bool_val(wrap_ind);
  pph.wrap_assign(ppl_vset,
                  build_ppl_bounded_integer_type_width(width),
                  build_ppl_bounded_integer_type_representation(rep),
                  build_ppl_bounded_integer_type_overflow(oflow),
                  &ppl_cs, ppl_complexity, ppl_wrap_ind);
  CAMLreturn(Val_unit);
}
CATCH_ALL

extern "C"
CAMLprim value
ppl_@CLASS@_wrap_assign_bytecode(value * argv, int)
{
  return ppl_@CLASS@_wrap_assign_native(argv[0], argv[1], argv[2], argv[3],
                                        argv[4], argv[5], argv[6], argv[7]);
}

')

