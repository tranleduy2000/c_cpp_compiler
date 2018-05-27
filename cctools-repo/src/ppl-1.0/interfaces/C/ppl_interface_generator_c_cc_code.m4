dnl  -*- C++ -*-
m4_divert(-1)

This m4 file contains the program implementation code for generating the
files ppl_c_DOMAIN.cc for each interface domain DOMAIN
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

dnl No code is needed for these procedure schemas in the C interface.

m4_define(`ppl_@CLASS@_swap_code', `')
m4_define(`ppl_@CLASS@_ascii_dump_code', `')

dnl There is no code at present for these procedures in the C interface.
dnl Remove the macro if its definition is added.

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`int
ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension
(ppl_@CLASS@_t* pph,
 ppl_dimension_type d,
 int empty) try {
  Degenerate_Element e = (empty != 0) ? EMPTY : UNIVERSE;
  *pph = to_nonconst(new @TOPOLOGY@@CPP_CLASS@(d, e));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`int
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@
(ppl_@CLASS@_t* pph,
 ppl_const_@A_FRIEND@_t ph) try {
  const @B_FRIEND@& phh
    = *static_cast<const @B_FRIEND@*>(to_const(ph));
  *pph = to_nonconst(new @TOPOLOGY@@CPP_CLASS@(phh));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`int
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity
(ppl_@CLASS@_t* pph,
 ppl_const_@A_FRIEND@_t ph,
 int complexity) try {
  const @B_FRIEND@& phh
    = *static_cast<const @B_FRIEND@*>(to_const(ph));
  switch (complexity) {
  case 0:
    *pph = to_nonconst(new @TOPOLOGY@@CPP_CLASS@(phh, POLYNOMIAL_COMPLEXITY));
    break;
  case 1:
    *pph = to_nonconst(new @TOPOLOGY@@CPP_CLASS@(phh, SIMPLEX_COMPLEXITY));
    break;
  case 2:
    *pph = to_nonconst(new @TOPOLOGY@@CPP_CLASS@(phh, ANY_COMPLEXITY));
    break;
  }
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
`int
ppl_new_@TOPOLOGY@@CLASS@_from_@!BUILD_REPRESENT@_System
(ppl_@CLASS@_t* pph, ppl_const_@!BUILD_REPRESENT@_System_t cs) try {
  const @!BUILD_REPRESENT@_System& ccs = *to_const(cs);
  *pph = to_nonconst(new @TOPOLOGY@@CPP_CLASS@(ccs));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_recycle_@BUILD_REPRESENT@s_code',
`int
ppl_new_@TOPOLOGY@@CLASS@_recycle_@!BUILD_REPRESENT@_System
(ppl_@CLASS@_t* pph, ppl_@!BUILD_REPRESENT@_System_t cs) try {
  @!BUILD_REPRESENT@_System& ccs = *to_nonconst(cs);
  *pph = to_nonconst(new @TOPOLOGY@@CPP_CLASS@(ccs@RECYCLE@));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_delete_@CLASS@_code',
`int
ppl_delete_@CLASS@(ppl_const_@CLASS@_t ph) try {
  delete to_const(ph);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_assign_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@_code',
`int
ppl_assign_@TOPOLOGY@@CLASS@_from_@TOPOLOGY@@CLASS@
(ppl_@CLASS@_t dst,
 ppl_const_@CLASS@_t src) try {
  const @TOPOLOGY@@CPP_CLASS@& ssrc
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(src));
  @TOPOLOGY@@CPP_CLASS@& ddst
    = *static_cast<@TOPOLOGY@@CPP_CLASS@*>(to_nonconst(dst));
  ddst = ssrc;
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
`int
ppl_@CLASS@_@DIMENSION@
(ppl_const_@CLASS@_t ph,
 ppl_dimension_type* m) try {
  *m = to_const(ph)->@DIMENSION@();
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
`int
ppl_@CLASS@_get_@CLASS_REPRESENT@s
(ppl_const_@CLASS@_t ph,
 ppl_const_@!CLASS_REPRESENT@_System_t* pcs) try {
  const @CPP_CLASS@& pph = *to_const(ph);
  const @!CLASS_REPRESENT@_System& cs = pph.@CLASS_REPRESENT@s();
  *pcs = to_const(&cs);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
`int
ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s
(ppl_const_@CLASS@_t ph,
 ppl_const_@!CLASS_REPRESENT@_System_t* pcs) try {
  const @CPP_CLASS@& pph = *to_const(ph);
  const @!CLASS_REPRESENT@_System& cs = pph.minimized_@CLASS_REPRESENT@s();
  *pcs = to_const(&cs);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
`int
ppl_@CLASS@_relation_with_@!RELATION_REPRESENT@
(ppl_const_@CLASS@_t ph,
 ppl_const_@!RELATION_REPRESENT@_t c) try {
  const @CPP_CLASS@& pph = *to_const(ph);
  const @!RELATION_REPRESENT@& cc = *to_const(c);
  return static_cast<int>(pph.relation_with(cc).get_flags());
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
`int
ppl_@CLASS@_@HAS_PROPERTY@(ppl_const_@CLASS@_t ph) try {
  const @CPP_CLASS@& pph = *to_const(ph);
  return pph.@HAS_PROPERTY@() ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
`int
ppl_@CLASS@_bounds_from_@ABOVEBELOW@
(ppl_const_@CLASS@_t ph,
 ppl_const_Linear_Expression_t le) try {
  const @CPP_CLASS@& pph = *to_const(ph);
  const Linear_Expression& lle = *to_const(le);
  return pph.bounds_from_@ABOVEBELOW@(lle) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
`int
ppl_@CLASS@_@MAXMIN@
(ppl_const_@CLASS@_t ph,
 ppl_const_Linear_Expression_t le,
 ppl_Coefficient_t sup_n,
 ppl_Coefficient_t sup_d,
 int* poptimum) try {
  const @CPP_CLASS@& pph = *to_const(ph);
  const Linear_Expression& lle = *to_const(le);
  Coefficient& ssup_n = *to_nonconst(sup_n);
  Coefficient& ssup_d = *to_nonconst(sup_d);
  bool optimum;
  bool ok = pph.@MAXMIN@(lle, ssup_n, ssup_d, optimum);
  if (ok)
    *poptimum = optimum ? 1 : 0;
  return ok ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
`int
ppl_@CLASS@_@MAXMIN@_with_point
(ppl_const_@CLASS@_t ph,
 ppl_const_Linear_Expression_t le,
 ppl_Coefficient_t sup_n,
 ppl_Coefficient_t sup_d,
 int* poptimum,
 ppl_Generator_t point) try {
  const @CPP_CLASS@& pph = *to_const(ph);
  const Linear_Expression& lle = *to_const(le);
  Coefficient& ssup_n = *to_nonconst(sup_n);
  Coefficient& ssup_d = *to_nonconst(sup_d);
  Generator& ppoint = *to_nonconst(point);
  bool optimum;
  bool ok = pph.@MAXMIN@(lle, ssup_n, ssup_d, optimum, ppoint);
  if (ok)
    *poptimum = optimum ? 1 : 0;
  return ok ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
`int
ppl_@CLASS@_has_@UPPERLOWER@_bound
(ppl_@CLASS@_t ps,
 ppl_dimension_type var,
 ppl_Coefficient_t ext_n,
 ppl_Coefficient_t ext_d,
 int* pclosed) try {
  const @CPP_CLASS@& pps = *to_const(ps);
  Coefficient& eext_n = *to_nonconst(ext_n);
  Coefficient& eext_d = *to_nonconst(ext_d);
  bool closed;
  bool bounded
    = pps.has_@UPPERLOWER@_bound(Variable(var), eext_n, eext_d, closed);
  if (bounded)
    *pclosed = closed ? 1 : 0;
  return bounded ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_frequency_code',
`int
ppl_@CLASS@_frequency
(ppl_const_@CLASS@_t ph,
 ppl_const_Linear_Expression_t le,
 ppl_Coefficient_t freq_n,
 ppl_Coefficient_t freq_d,
 ppl_Coefficient_t val_n,
 ppl_Coefficient_t val_d) try {
  const @CPP_CLASS@& pph = *to_const(ph);
  const Linear_Expression& lle = *to_const(le);
  Coefficient& pfreq_n = *to_nonconst(freq_n);
  Coefficient& pfreq_d = *to_nonconst(freq_d);
  Coefficient& pval_n = *to_nonconst(val_n);
  Coefficient& pval_d = *to_nonconst(val_d);
  return pph.frequency(lle, pfreq_n, pfreq_d, pval_n, pval_d) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
`int
ppl_@CLASS@_@COMPARISON@_@CLASS@
(ppl_const_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  const @CPP_CLASS@& xx = *to_const(x);
  const @CPP_CLASS@& yy = *to_const(y);
  return xx.@COMPARISON@(yy) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
`int
ppl_@CLASS@_equals_@CLASS@
(ppl_const_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  const @CPP_CLASS@& xx = *to_const(x);
  const @CPP_CLASS@& yy = *to_const(y);
  return (xx == yy) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_OK_code',
`int
ppl_@CLASS@_OK(ppl_const_@CLASS@_t ph) try {
  return to_const(ph)->OK() ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
`int
ppl_@CLASS@_@SIMPLIFY@(ppl_@CLASS@_t ph) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  pph.@SIMPLIFY@();
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
`int
ppl_@CLASS@_unconstrain_space_dimension
(ppl_@CLASS@_t ph,
 ppl_dimension_type var
) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  pph.unconstrain(Variable(var));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
`int
ppl_@CLASS@_unconstrain_space_dimensions
(ppl_@CLASS@_t ph,
 ppl_dimension_type ds[],
 size_t n) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  Variables_Set vars;
  for (ppl_dimension_type i = n; i-- > 0; )
    vars.insert(ds[i]);
  pph.unconstrain(vars);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_constrains_code',
`int
ppl_@CLASS@_constrains
(ppl_@CLASS@_t ph,
 ppl_dimension_type var
) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  return pph.constrains(Variable(var)) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@BINOP@_code',
`int
ppl_@CLASS@_@BINOP@
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  xx.@BINOP@(yy);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
`int
ppl_@CLASS@_@UB_EXACT@
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
`m4_ifelse(m4_current_interface, `Polyhedron',
  `m4_ub_exact_for_polyhedron_domains',
          `m4_ub_exact_for_non_polyhedron_domains')'
}
CATCH_ALL

  ')

m4_define(`m4_ub_exact_for_polyhedron_domains',
` if (Interfaces::is_necessarily_closed_for_interfaces(*to_const(x))) {
    C_Polyhedron& xx = static_cast<C_Polyhedron&>(*to_nonconst(x));
    const C_Polyhedron& yy = static_cast<const C_Polyhedron&>(*to_const(y));
    return xx.upper_bound_assign_if_exact(yy) ? 1 : 0;
  }
  else {
    NNC_Polyhedron& xx = static_cast<NNC_Polyhedron&>(*to_nonconst(x));
    const NNC_Polyhedron& yy = static_cast<const NNC_Polyhedron&>(*to_const(y));
    return xx.upper_bound_assign_if_exact(yy) ? 1 : 0;
  }
')

m4_define(`m4_ub_exact_for_non_polyhedron_domains',
`  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  return xx.@UB_EXACT@(yy) ? 1 : 0;
')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
`int
ppl_@CLASS@_simplify_using_context_assign
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  return xx.simplify_using_context_assign(yy) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
`int
ppl_@CLASS@_add_@CLASS_REPRESENT@
(ppl_@CLASS@_t ph,
 ppl_const_@!CLASS_REPRESENT@_t c) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const @!CLASS_REPRESENT@& cc = *to_const(c);
  pph.add_@CLASS_REPRESENT@(cc);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
`int
ppl_@CLASS@_refine_with_@REFINE_REPRESENT@
(ppl_@CLASS@_t ph,
 ppl_const_@!REFINE_REPRESENT@_t c) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const @!REFINE_REPRESENT@& cc = *to_const(c);
  pph.refine_with_@REFINE_REPRESENT@(cc);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
`int
ppl_@CLASS@_add_@CLASS_REPRESENT@s
(ppl_@CLASS@_t ph,
 ppl_const_@!CLASS_REPRESENT@_System_t cs) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const @!CLASS_REPRESENT@_System& ccs = *to_const(cs);
  pph.add_@CLASS_REPRESENT@s(ccs);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
`int
ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s
(ppl_@CLASS@_t ph,
 ppl_const_@!REFINE_REPRESENT@_System_t cs) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const @!REFINE_REPRESENT@_System& ccs = *to_const(cs);
  pph.refine_with_@REFINE_REPRESENT@s(ccs);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_add_recycled_@CLASS_REPRESENT@s_code',
`int
ppl_@CLASS@_add_recycled_@CLASS_REPRESENT@s
(ppl_@CLASS@_t ph,
 ppl_@!CLASS_REPRESENT@_System_t cs) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  @!CLASS_REPRESENT@_System& ccs = *to_nonconst(cs);
  pph.add_recycled_@CLASS_REPRESENT@s(ccs);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
`int
ppl_@CLASS@_@AFFIMAGE@
(ppl_@CLASS@_t ph,
 ppl_dimension_type var,
 ppl_const_Linear_Expression_t le,
 ppl_const_Coefficient_t d) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const Linear_Expression& lle = *to_const(le);
  const Coefficient& dd = *to_const(d);
  pph.@AFFIMAGE@(Variable(var), lle, dd);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
`int
ppl_@CLASS@_bounded_@AFFIMAGE@
(ppl_@CLASS@_t ph,
 ppl_dimension_type var,
 ppl_const_Linear_Expression_t lb,
 ppl_const_Linear_Expression_t ub,
 ppl_const_Coefficient_t d) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const Linear_Expression& llb = *to_const(lb);
  const Linear_Expression& uub = *to_const(ub);
  const Coefficient& dd = *to_const(d);
  pph.bounded_@AFFIMAGE@(Variable(var), llb, uub, dd);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
`int
ppl_@CLASS@_generalized_@AFFIMAGE@
(ppl_@CLASS@_t ph,
 ppl_dimension_type var,
 enum ppl_enum_Constraint_Type relsym,
 ppl_const_Linear_Expression_t le,
 ppl_const_Coefficient_t d) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const Linear_Expression& lle = *to_const(le);
  const Coefficient& dd = *to_const(d);
  pph.generalized_@AFFIMAGE@
    (Variable(var), relation_symbol(relsym), lle, dd);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
`int
ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs
(ppl_@CLASS@_t ph,
 ppl_const_Linear_Expression_t lhs,
 enum ppl_enum_Constraint_Type relsym,
 ppl_const_Linear_Expression_t rhs) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const Linear_Expression& llhs = *to_const(lhs);
  const Linear_Expression& rrhs = *to_const(rhs);
  pph.generalized_@AFFIMAGE@(llhs, relation_symbol(relsym), rrhs);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
`int
ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence
(ppl_@CLASS@_t ph,
 ppl_dimension_type var,
 enum ppl_enum_Constraint_Type relsym,
 ppl_const_Linear_Expression_t le,
 ppl_const_Coefficient_t d,
 ppl_const_Coefficient_t m) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const Linear_Expression& lle = *to_const(le);
  const Coefficient& dd = *to_const(d);
  const Coefficient& mm = *to_const(m);
  pph.generalized_@AFFIMAGE@
    (Variable(var), relation_symbol(relsym), lle, dd, mm);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
`int
ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence
(ppl_@CLASS@_t ph,
 ppl_const_Linear_Expression_t lhs,
 enum ppl_enum_Constraint_Type relsym,
 ppl_const_Linear_Expression_t rhs,
 ppl_const_Coefficient_t m) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  const Linear_Expression& llhs = *to_const(lhs);
  const Linear_Expression& rrhs = *to_const(rhs);
  const Coefficient& mm = *to_const(m);
  pph.generalized_@AFFIMAGE@(llhs, relation_symbol(relsym), rrhs, mm);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code',
`int
ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y,
 unsigned* tp) try {
  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  xx.@WIDEN@_widening_assign(yy, tp);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
`int
ppl_@CLASS@_@WIDEN@_widening_assign
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  return ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens(x, y, 0);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code',
`int
ppl_@CLASS@_widening_assign_with_tokens
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y,
 unsigned* tp) try {
  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  xx.widening_assign(yy, tp);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_widening_assign_code',
`int
ppl_@CLASS@_widening_assign
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  return ppl_@CLASS@_widening_assign_with_tokens(x, y, 0);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code',
`int
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y,
 ppl_const_@!CONSTRAINER@_System_t cs,
 unsigned* tp) try {
  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  const @!CONSTRAINER@_System& ccs = *to_const(cs);
  xx.@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(yy, ccs, tp);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
`int
ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y,
 ppl_const_@!CONSTRAINER@_System_t cs) try {
  return
    ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens
      (x, y, cs, 0);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code',
`int
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y,
 unsigned* tp) try {
  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  xx.@EXTRAPOLATION@_extrapolation_assign(yy, tp);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
`int
ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  return
    ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens
      (x, y, 0);
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
`int
ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  xx.@EXTRAPOLATION@_narrowing_assign(yy);
  return 0;
}
CATCH_ALL

')

  m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
`dnl
int
ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y) try {
  @CPP_CLASS@& xx = *to_nonconst(x);
  const @CPP_CLASS@& yy = *to_const(y);
  xx.BHZ03_widening_assign<@A_DISJUNCT_WIDEN@_Certificate>(yy,
       widen_fun_ref(
         &@DISJUNCT_TOPOLOGY@@A_DISJUNCT@::@DISJUNCT_WIDEN@_widening_assign));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
`int
ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign
(ppl_@CLASS@_t x,
 ppl_const_@CLASS@_t y,
 unsigned disjuncts) try {
   @CPP_CLASS@& xx = *to_nonconst(x);
   const @CPP_CLASS@& yy = *to_const(y);
   xx.BGP99_extrapolation_assign(yy,
       widen_fun_ref(&@DISJUNCT_TOPOLOGY@@A_DISJUNCT@::
           @DISJUNCT_WIDEN@_widening_assign),
       disjuncts);
   return 0;
}
CATCH_ALL


')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
`int
ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@
(ppl_@CLASS@_t ph,
 ppl_dimension_type d) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  pph.add_space_dimensions_@EMBEDPROJECT@(d);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
`int
ppl_@CLASS@_remove_space_dimensions
(ppl_@CLASS@_t ph,
 ppl_dimension_type ds[],
 size_t n) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  Variables_Set vars;
  for (ppl_dimension_type i = n; i-- > 0; )
    vars.insert(ds[i]);
  pph.remove_space_dimensions(vars);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
`int
ppl_@CLASS@_remove_higher_space_dimensions
(ppl_@CLASS@_t ph,
 ppl_dimension_type d) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  pph.remove_higher_space_dimensions(d);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
`int
ppl_@CLASS@_map_space_dimensions
(ppl_@CLASS@_t ph,
 ppl_dimension_type maps[],
 size_t n) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  Array_Partial_Function_Wrapper function(maps, n);
  pph.map_space_dimensions(function);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
`int
ppl_@CLASS@_expand_space_dimension
(ppl_@CLASS@_t ph,
 ppl_dimension_type d,
 ppl_dimension_type m) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  pph.expand_space_dimension(Variable(d), m);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
`int
ppl_@CLASS@_fold_space_dimensions
(ppl_@CLASS@_t ph,
 ppl_dimension_type ds[],
 size_t n,
 ppl_dimension_type d) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  Variables_Set vars;
  for (ppl_dimension_type i = n; i-- > 0; )
    vars.insert(ds[i]);
  pph.fold_space_dimensions(vars, Variable(d));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
`int
ppl_@CLASS@_drop_some_non_integer_points
(ppl_@CLASS@_t ph,
 int complexity) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  switch (complexity) {
  case 0:
    pph.drop_some_non_integer_points(POLYNOMIAL_COMPLEXITY);
    break;
  case 1:
    pph.drop_some_non_integer_points(SIMPLEX_COMPLEXITY);
    break;
  case 2:
    pph.drop_some_non_integer_points(ANY_COMPLEXITY);
    break;
  }
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
`int
ppl_@CLASS@_drop_some_non_integer_points_2
(ppl_@CLASS@_t ph,
 ppl_dimension_type ds[],
 size_t n,
 int complexity) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  Variables_Set vars;
  for (ppl_dimension_type i = n; i-- > 0; )
    vars.insert(ds[i]);
  switch (complexity) {
  case 0:
    pph.drop_some_non_integer_points(vars, POLYNOMIAL_COMPLEXITY);
    break;
  case 1:
    pph.drop_some_non_integer_points(vars, SIMPLEX_COMPLEXITY);
    break;
  case 2:
    pph.drop_some_non_integer_points(vars, ANY_COMPLEXITY);
    break;
  }
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
`int
ppl_@CLASS@_@MEMBYTES@
(ppl_const_@CLASS@_t ps,
 size_t* sz) try {
  *sz = to_const(ps)->@MEMBYTES@();
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_new_@CLASS@_iterator_code',
`dnl

typedef @CPP_CLASS@::iterator
        @CLASS@_iterator;
typedef @CPP_CLASS@::const_iterator
        @CLASS@_const_iterator;

DECLARE_CONVERSIONS(@CLASS@_iterator,
                    @CLASS@_iterator)
DECLARE_CONVERSIONS(@CLASS@_const_iterator,
                    @CLASS@_const_iterator)

int
ppl_new_@CLASS@_iterator
(ppl_@CLASS@_iterator_t* pit) try {
  *pit = to_nonconst(new @CLASS@_iterator());
  return 0;
}
CATCH_ALL

int
ppl_new_@CLASS@_const_iterator
(ppl_@CLASS@_const_iterator_t* pit) try {
  *pit = to_nonconst(new @CLASS@_const_iterator());
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`dnl

int
ppl_new_@CLASS@_iterator_from_iterator
(ppl_@CLASS@_iterator_t* px, ppl_const_@CLASS@_iterator_t y) try {
  const @CLASS@_iterator& yy
    = *static_cast<const @CLASS@_iterator*>(to_const(y));
  *px = to_nonconst(new @CLASS@_iterator(yy));
  return 0;
}
CATCH_ALL

int
ppl_new_@CLASS@_const_iterator_from_const_iterator
(ppl_@CLASS@_const_iterator_t* px,
 ppl_const_@CLASS@_const_iterator_t y) try {
  const @CLASS@_const_iterator& yy
    = *static_cast<const @CLASS@_const_iterator*>(to_const(y));
  *px = to_nonconst(new @CLASS@_const_iterator(yy));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
`dnl
int
ppl_@CLASS@_iterator_equal_test
(ppl_const_@CLASS@_iterator_t x,
 ppl_const_@CLASS@_iterator_t y) try {
  return (*to_const(x) == *to_const(y)) ? 1 : 0;
}
CATCH_ALL

int
ppl_@CLASS@_const_iterator_equal_test
(ppl_const_@CLASS@_const_iterator_t x,
 ppl_const_@CLASS@_const_iterator_t y) try {
  return (*to_const(x) == *to_const(y)) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
`dnl
int
ppl_@CLASS@_iterator_@BEGINEND@
(ppl_@CLASS@_t ps,
 ppl_@CLASS@_iterator_t psit) try {
  @CPP_CLASS@::iterator& ppsit = *to_nonconst(psit);
  ppsit = to_nonconst(ps)->@BEGINEND@();
  return 0;
}
CATCH_ALL

int
ppl_@CLASS@_const_iterator_@BEGINEND@
(ppl_const_@CLASS@_t ps,
 ppl_@CLASS@_const_iterator_t psit) try {
  @CPP_CLASS@::const_iterator& ppsit = *to_nonconst(psit);
  ppsit = to_const(ps)->@BEGINEND@();
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_delete_@CLASS@_iterator_code',
`dnl
int
ppl_delete_@CLASS@_iterator
(ppl_const_@CLASS@_iterator_t it)
  try {
  delete to_const(it);
  return 0;
}
CATCH_ALL

int
ppl_delete_@CLASS@_const_iterator
(ppl_const_@CLASS@_const_iterator_t it)
  try {
  delete to_const(it);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
`dnl
int
ppl_@CLASS@_iterator_@INCDEC@
(ppl_@CLASS@_iterator_t it)
  try {
    @CPP_CLASS@::iterator& iit = *to_nonconst(it);
    @B_INCDEC@iit;
    return 0;
}
CATCH_ALL

int
ppl_@CLASS@_const_iterator_@INCDEC@
(ppl_@CLASS@_const_iterator_t it)
  try {
    @CPP_CLASS@::const_iterator& iit = *to_nonconst(it);
    @B_INCDEC@iit;
    return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
`dnl
int
ppl_@CLASS@_drop_disjunct
(ppl_@CLASS@_t ps,
 ppl_const_@CLASS@_iterator_t cit,
 ppl_@CLASS@_iterator_t it) try {
  @CPP_CLASS@& pps = *to_nonconst(ps);
  const @CPP_CLASS@::iterator& ccit = *to_const(cit);
  @CPP_CLASS@::iterator& iit = *to_nonconst(it);
  iit = pps.drop_disjunct(ccit);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
`dnl
int
ppl_@CLASS@_drop_disjuncts
(ppl_@CLASS@_t ps,
 ppl_const_@CLASS@_iterator_t first,
 ppl_const_@CLASS@_iterator_t last) try {
  @CPP_CLASS@& pps = *to_nonconst(ps);
  const @CPP_CLASS@::iterator& ffirst = *to_const(first);
  const @CPP_CLASS@::iterator& llast = *to_const(last);
  pps.drop_disjuncts(ffirst, llast);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
`dnl
int
ppl_@CLASS@_add_disjunct
(ppl_@CLASS@_t ps,
 ppl_const_@DISJUNCT@_t d) try {
  @CPP_CLASS@& pps = *to_nonconst(ps);
  const @DISJUNCT_TOPOLOGY@@A_DISJUNCT@& dd
     = *static_cast<const @DISJUNCT_TOPOLOGY@@A_DISJUNCT@*>(to_const(d));
  pps.add_disjunct(dd);
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_get_disjunct_code',
`dnl
int
ppl_@CLASS@_iterator_dereference
(ppl_const_@CLASS@_iterator_t psit,
 ppl_const_@DISJUNCT@_t* cd) try {
   const @CPP_CLASS@::iterator& cpsit = *to_const(psit);
   const @DISJUNCT_TOPOLOGY@@A_DISJUNCT@& d = cpsit->pointset();
   *cd = to_const(&d);
   return 0;
}
CATCH_ALL

int
ppl_@CLASS@_const_iterator_dereference
(ppl_const_@CLASS@_const_iterator_t psit,
 ppl_const_@DISJUNCT@_t* cd) try {
   const @CPP_CLASS@::const_iterator& cpsit = *to_const(psit);
   const @DISJUNCT_TOPOLOGY@@A_DISJUNCT@& d = cpsit->pointset();
   *cd = to_const(&d);
   return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
`dnl
int
ppl_@CLASS@_linear_@PARTITION@
(ppl_const_@CLASS@_t x,
 ppl_const_@CLASS@_t y,
 ppl_@CLASS@_t* p_inters,
 ppl_Pointset_Powerset_NNC_Polyhedron_t* p_rest) try {
`m4_ifelse(m4_current_interface, `Polyhedron',
  `m4_linear_partition_for_polyhedron_domains',
          `m4_linear_partition_for_non_polyhedron_domains')'
}
CATCH_ALL

')

m4_define(`m4_linear_partition_for_polyhedron_domains',
`dnl
 if (Interfaces::is_necessarily_closed_for_interfaces(*to_const(x))) {
    const C_@CPP_CLASS@& xx
      = static_cast<const C_@CPP_CLASS@&>(*to_const(x));
    const C_@CPP_CLASS@& yy
      = static_cast<const C_@CPP_CLASS@&>(*to_const(y));
    std::pair<C_@CPP_CLASS@|COMMA| Pointset_Powerset<NNC_Polyhedron> >
      r = linear_partition(xx, yy);
    *p_inters = to_nonconst(&r.first);
    *p_rest = to_nonconst(&r.second);
 }
 else {
    const C_@CPP_CLASS@& xx
      = static_cast<const C_@CPP_CLASS@&>(*to_const(x));
    const C_@CPP_CLASS@& yy
      = static_cast<const C_@CPP_CLASS@&>(*to_const(y));
    std::pair<C_@CPP_CLASS@|COMMA| Pointset_Powerset<NNC_Polyhedron> >
      r = linear_partition(xx, yy);
    *p_inters = to_nonconst(&r.first);
    *p_rest = to_nonconst(&r.second);
}
  return 0;

')

m4_define(`m4_linear_partition_for_non_polyhedron_domains',
`dnl
    const @CPP_CLASS@& xx
      = static_cast<const @CPP_CLASS@&>(*to_const(x));
    const @CPP_CLASS@& yy
      = static_cast<const @CPP_CLASS@&>(*to_const(y));
    std::pair<@CPP_CLASS@|COMMA| Pointset_Powerset<NNC_Polyhedron> >
      r = linear_partition(xx, yy);
    *p_inters = to_nonconst(&r.first);
    *p_rest = to_nonconst(&r.second);
  return 0;

')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code',
`dnl
int
ppl_@CLASS@_approximate_@PARTITION@
(ppl_const_@CLASS@_t x,
 ppl_const_@CLASS@_t y,
 ppl_@CLASS@_t* p_inters,
 ppl_Pointset_Powerset_Grid_t* p_rest,
 int* p_finite) try {
    const @CPP_CLASS@& xx
      = static_cast<const @CPP_CLASS@&>(*to_const(x));
    const @CPP_CLASS@& yy
      = static_cast<const @CPP_CLASS@&>(*to_const(y));
    bool finite;
    std::pair<@CPP_CLASS@|COMMA| Pointset_Powerset<Grid> >
      r = approximate_partition(xx, yy, finite);
    *p_inters = to_nonconst(&r.first);
    *p_rest = to_nonconst(&r.second);
    *p_finite = finite ? 1 : 0;
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`dnl
int
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
(ppl_const_@CLASS@_t pset) try {
  const @TOPOLOGY@@CPP_CLASS@& ppset
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset));
  return termination_test_@TERMINATION_ID@(ppset) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`dnl
int
ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
(ppl_const_@CLASS@_t pset_before, ppl_const_@CLASS@_t pset_after) try {
  const @TOPOLOGY@@CPP_CLASS@& ppset_before
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset_before));
  const @TOPOLOGY@@CPP_CLASS@& ppset_after
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset_after));
  return termination_test_@TERMINATION_ID@_2(ppset_before, ppset_after) ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`int
ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
(ppl_const_@CLASS@_t pset,
 ppl_Generator_t point) try {
  const @TOPOLOGY@@CPP_CLASS@& ppset
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset));
  Generator& ppoint = *to_nonconst(point);
  bool ok = one_affine_ranking_function_@TERMINATION_ID@(ppset, ppoint);
  return ok ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`int
ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
(ppl_const_@CLASS@_t pset_before,
 ppl_const_@CLASS@_t pset_after,
 ppl_Generator_t point) try {
  const @TOPOLOGY@@CPP_CLASS@& ppset_before
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset_before));
  const @TOPOLOGY@@CPP_CLASS@& ppset_after
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset_after));
  Generator& ppoint = *to_nonconst(point);
  bool ok = one_affine_ranking_function_@TERMINATION_ID@_2(ppset_before,
                                                           ppset_after,
                                                           ppoint);
  return ok ? 1 : 0;
}
CATCH_ALL

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
`int
ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
(ppl_const_@CLASS@_t pset,
 ppl_Polyhedron_t ph) try {
  const @TOPOLOGY@@CPP_CLASS@& ppset
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset));
  all_affine_ranking_functions_@TERMINATION_ID@
    (ppset, *static_cast<@A_TERMINATION_ID@Polyhedron*> (to_nonconst(ph)));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
`int
ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
(ppl_const_@CLASS@_t pset_before,
 ppl_const_@CLASS@_t pset_after,
 ppl_Polyhedron_t ph) try {
  const @TOPOLOGY@@CPP_CLASS@& ppset_before
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset_before));
  const @TOPOLOGY@@CPP_CLASS@& ppset_after
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset_after));
  all_affine_ranking_functions_@TERMINATION_ID@_2
    (ppset_before, ppset_after,
     *static_cast<@A_TERMINATION_ID@Polyhedron*>(to_nonconst(ph)));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_code',
`int
ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@
(ppl_const_@CLASS@_t pset,
 ppl_Polyhedron_t ph_decreasing,
 ppl_Polyhedron_t ph_bounded) try {
  const @TOPOLOGY@@CPP_CLASS@& ppset
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset));
  all_affine_quasi_ranking_functions_MS(ppset,
    *static_cast<C_Polyhedron*>(to_nonconst(ph_decreasing)),
    *static_cast<C_Polyhedron*>(to_nonconst(ph_bounded)));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2_code',
`int
ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2
(ppl_const_@CLASS@_t pset_before,
 ppl_const_@CLASS@_t pset_after,
 ppl_Polyhedron_t ph_decreasing,
 ppl_Polyhedron_t ph_bounded) try {
  const @TOPOLOGY@@CPP_CLASS@& ppset_before
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset_before));
  const @TOPOLOGY@@CPP_CLASS@& ppset_after
    = *static_cast<const @TOPOLOGY@@CPP_CLASS@*>(to_const(pset_after));
  all_affine_quasi_ranking_functions_MS_2(ppset_before, ppset_after,
    *static_cast<C_Polyhedron*>(to_nonconst(ph_decreasing)),
    *static_cast<C_Polyhedron*>(to_nonconst(ph_bounded)));
  return 0;
}
CATCH_ALL

')

m4_define(`ppl_@CLASS@_wrap_assign_code',
`int
ppl_@CLASS@_wrap_assign
(ppl_@CLASS@_t ph,
 ppl_dimension_type ds[],
 size_t n,
 enum ppl_enum_Bounded_Integer_Type_Width w,
 enum ppl_enum_Bounded_Integer_Type_Representation r,
 enum ppl_enum_Bounded_Integer_Type_Overflow o,
 const ppl_const_Constraint_System_t* pcs,
 unsigned complexity_threshold,
 int wrap_individually) try {
  @CPP_CLASS@& pph = *to_nonconst(ph);
  Variables_Set vars;
  for (ppl_dimension_type i = n; i-- > 0; )
    vars.insert(ds[i]);
  const Constraint_System* ccs = to_const(*pcs);
  bool b = (wrap_individually != 0);
  pph.wrap_assign(vars,
                  bounded_integer_type_width(w),
                  bounded_integer_type_representation(r),
                  bounded_integer_type_overflow(o),
                  ccs, complexity_threshold, b);
  return 0;
}
CATCH_ALL

')
