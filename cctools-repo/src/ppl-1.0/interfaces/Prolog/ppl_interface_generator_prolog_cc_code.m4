dnl  -*- C++ -*-
m4_divert(-1)

This m4 file contains the program code for generating the
files ppl_prolog_DOMAIN.cc for each interface domain DOMAIN
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

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension_code',
`extern "C" Prolog_foreign_return_type
ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension(Prolog_term_ref t_nd,
                                               Prolog_term_ref t_uoe,
                                               Prolog_term_ref t_ph) {
  static const char* where = "ppl_new_@TOPOLOGY@@CLASS@_from_space_dimension/3";
  try {
    @TOPOLOGY@@CPP_CLASS@* ph;
    Prolog_atom uoe = term_to_universe_or_empty(t_uoe, where);

    if (uoe == a_empty)
      ph = new @TOPOLOGY@@CPP_CLASS@(term_to_unsigned<dimension_type>(t_nd,
                                                                      where),
                                     EMPTY);
    else
      ph = new @TOPOLOGY@@CPP_CLASS@(term_to_unsigned<dimension_type>(t_nd,
                                                                      where),
                                     UNIVERSE);

      Prolog_term_ref tmp = Prolog_new_term_ref();
      Prolog_put_address(tmp, ph);
      if (Prolog_unify(t_ph, tmp)) {
        PPL_REGISTER(ph);
        return PROLOG_SUCCESS;
      }
      else
        delete ph;
  }
  CATCH_ALL;
}

')

  m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_code',
`extern "C" Prolog_foreign_return_type
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@(
                     Prolog_term_ref t_ph_source, Prolog_term_ref t_ph)
{
  static const char* where =
                   "ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@/2";
  try {
    @TOPOLOGY@@CPP_CLASS@* ph;
    const @B_FRIEND@* ph_source
        = static_cast<const @B_FRIEND@*>
        (term_to_handle<@B_FRIEND@ >(t_ph_source, where));
    PPL_CHECK(ph_source);
        ph = new @TOPOLOGY@@CPP_CLASS@(*ph_source);
    Prolog_term_ref tmp = Prolog_new_term_ref();
    Prolog_put_address(tmp, ph);
    if (Prolog_unify(t_ph, tmp)) {
      PPL_REGISTER(ph);
      return PROLOG_SUCCESS;
    }
    else
      delete ph;
  }
  CATCH_ALL;
}

')

  m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity_code',
`extern "C" Prolog_foreign_return_type
ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity(
                     Prolog_term_ref t_ph_source,
                     Prolog_term_ref t_ph,
                     Prolog_term_ref t_cc)
{
  static const char* where =
                   "ppl_new_@TOPOLOGY@@CLASS@_from_@FRIEND@_with_complexity/3";
  try {
    @TOPOLOGY@@CPP_CLASS@* ph;
    const @B_FRIEND@* ph_source
        = static_cast<const @B_FRIEND@*>
        (term_to_handle<@B_FRIEND@ >(t_ph_source, where));

    Prolog_atom p_cc = term_to_complexity_class(t_cc, where);
    Complexity_Class cc;
    if (p_cc == a_polynomial)
      cc = POLYNOMIAL_COMPLEXITY;
    else if (p_cc == a_simplex)
      cc = SIMPLEX_COMPLEXITY;
    else
      cc = ANY_COMPLEXITY;

    PPL_CHECK(ph_source);
    ph = new @TOPOLOGY@@CPP_CLASS@(*ph_source, cc);

    Prolog_term_ref tmp = Prolog_new_term_ref();
    Prolog_put_address(tmp, ph);
    if (Prolog_unify(t_ph, tmp)) {
      PPL_REGISTER(ph);
      return PROLOG_SUCCESS;
    }
    else
      delete ph;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s(Prolog_term_ref t_clist,
                                                    Prolog_term_ref t_ph)
{
  static const char* where =
    "ppl_new_@TOPOLOGY@@CLASS@_from_@BUILD_REPRESENT@s/2";
  try {
    @!BUILD_REPRESENT@_System cs;
    Prolog_term_ref c = Prolog_new_term_ref();

    while (Prolog_is_cons(t_clist)) {
      Prolog_get_cons(t_clist, c, t_clist);
      cs.insert(build_@BUILD_REPRESENT@(c, where));
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_clist, where);

    @TOPOLOGY@@CPP_CLASS@* ph;
    ph = new @TOPOLOGY@@CPP_CLASS@(cs@RECYCLE@);
    Prolog_term_ref tmp = Prolog_new_term_ref();
    Prolog_put_address(tmp, ph);
    if (Prolog_unify(t_ph, tmp)) {
      PPL_REGISTER(ph);
      return PROLOG_SUCCESS;
    }
    else
      delete ph;
  }
  CATCH_ALL;
}

')

  m4_define(`ppl_@CLASS@_swap_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_swap(Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_swap/2";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    swap(*lhs, *rhs);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_delete_@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_delete_@CLASS@(Prolog_term_ref t_ph) {
  static const char* where = "ppl_delete_@CLASS@/1";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_UNREGISTER(ph);
    delete ph;
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@DIMENSION@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@DIMENSION@(Prolog_term_ref t_ph, Prolog_term_ref t_sd) {
  static const char* where = "ppl_@CLASS@_@DIMENSION@/2";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    if (unify_ulong(t_sd, ph->@DIMENSION@()))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_get_@CLASS_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_get_@CLASS_REPRESENT@s(Prolog_term_ref t_ph,
                                   Prolog_term_ref t_glist) {
  static const char* where = "ppl_@CLASS@_get_@CLASS_REPRESENT@s/2";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);

    Prolog_term_ref tail = Prolog_new_term_ref();
    Prolog_put_atom(tail, a_nil);
    const @!CLASS_REPRESENT@_System& gs = ph->@CLASS_REPRESENT@s();
    for (@!CLASS_REPRESENT@_System::const_iterator i = gs.begin(),
           gs_end = gs.end(); i != gs_end; ++i)
      Prolog_construct_cons(tail, @CLASS_REPRESENT@_term(*i), tail);

    if (Prolog_unify(t_glist, tail))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s(Prolog_term_ref t_ph,
                                             Prolog_term_ref t_glist) {
  static const char* where = "ppl_@CLASS@_get_minimized_@CLASS_REPRESENT@s/2";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);

    Prolog_term_ref tail = Prolog_new_term_ref();
    Prolog_put_atom(tail, a_nil);
    const @!CLASS_REPRESENT@_System& gs = ph->minimized_@CLASS_REPRESENT@s();
    for (@!CLASS_REPRESENT@_System::const_iterator i = gs.begin(),
           gs_end = gs.end(); i != gs_end; ++i)
      Prolog_construct_cons(tail, @CLASS_REPRESENT@_term(*i), tail);

    if (Prolog_unify(t_glist, tail))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@BEGINEND@_iterator_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@BEGINEND@_iterator(Prolog_term_ref t_pps,
                                  Prolog_term_ref t_it) {
  static const char* where = "ppl_@CLASS@_@BEGINEND@_iterator/2";
  try {
    @CPP_CLASS@* pps = term_to_handle<@CPP_CLASS@ >(t_pps, where);
    PPL_CHECK(pps);

    @CPP_CLASS@::iterator* i = new @CPP_CLASS@::iterator(pps->@BEGINEND@());
    Prolog_term_ref t_i = Prolog_new_term_ref();
    Prolog_put_address(t_i, i);

    if (Prolog_unify(t_it, t_i)) {
      PPL_REGISTER(i);
      return PROLOG_SUCCESS;
    }
    else
      delete i;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_new_@CLASS@_iterator_from_iterator_code',
`extern "C" Prolog_foreign_return_type
ppl_new_@CLASS@_iterator_from_iterator(Prolog_term_ref t_source,
                                       Prolog_term_ref t_it) {
  static const char* where = "ppl_new_@CLASS@_iterator_from_iterator/2";
  try {
    const @CPP_CLASS@::iterator* source
      = term_to_handle<const @CPP_CLASS@::iterator>(t_source, where);
    PPL_CHECK(source);

    @CPP_CLASS@::iterator* it = new @CPP_CLASS@::iterator(*source);
    Prolog_term_ref t_i = Prolog_new_term_ref();
    Prolog_put_address(t_i, it);

    if (Prolog_unify(t_it, t_i)) {
      PPL_REGISTER(it);
      return PROLOG_SUCCESS;
    }
    else
      delete it;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_delete_@CLASS@_iterator_code',
  `extern "C" Prolog_foreign_return_type
  ppl_delete_@CLASS@_iterator(Prolog_term_ref t_it) {
  static const char* where = "ppl_delete_@CLASS@_iterator/1";
  try {
    const @CPP_CLASS@::iterator* it
      = term_to_handle<@CPP_CLASS@::iterator >(t_it, where);
    PPL_UNREGISTER(it);
    delete it;
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@INCDEC@_iterator_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@INCDEC@_iterator(Prolog_term_ref t_it) {
  static const char* where = "ppl_@CLASS@_@INCDEC@_iterator/2";
  try {
    @CPP_CLASS@::iterator* it
      = term_to_handle<@CPP_CLASS@::iterator >(t_it, where);
    PPL_CHECK(it);
    @B_INCDEC@(*it);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_iterator_equals_iterator_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_iterator_equals_iterator(Prolog_term_ref t_it1,
                                       Prolog_term_ref t_it2) {
  static const char* where = "ppl_@CLASS@_iterator_equals_iterator/2";
  try {
    @CPP_CLASS@::iterator* it1
      = term_to_handle<@CPP_CLASS@::iterator >(t_it1, where);
    PPL_CHECK(it1);
    @CPP_CLASS@::iterator* it2
      = term_to_handle<@CPP_CLASS@::iterator >(t_it2, where);
    PPL_CHECK(it2);
    if (*it1 == *it2)
      return PROLOG_SUCCESS;
    else
      return PROLOG_FAILURE;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_get_disjunct_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_get_disjunct(Prolog_term_ref t_it,
                           Prolog_term_ref t_disj) {
  static const char* where = "ppl_@CLASS@_get_disjunct/2";
  try {
    const @CPP_CLASS@::iterator* it
      = term_to_handle<@CPP_CLASS@::iterator >(t_it, where);
    PPL_CHECK(it);

    @DISJUNCT_TOPOLOGY@@A_DISJUNCT@* disj
      = const_cast<@DISJUNCT_TOPOLOGY@@A_DISJUNCT@*>(&((*it)->pointset()));
    Prolog_term_ref t_d = Prolog_new_term_ref();
    Prolog_put_address(t_d, disj);

    if (Prolog_unify(t_disj, t_d)) {
      PPL_WEAK_REGISTER(disj);
      return PROLOG_SUCCESS;
    }
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_drop_disjunct_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_drop_disjunct(Prolog_term_ref t_pps,
                            Prolog_term_ref t_it) {
  static const char* where = "ppl_@CLASS@_drop_disjuncts/2";
  try {
    @CPP_CLASS@* pps = term_to_handle<@CPP_CLASS@ >(t_pps, where);
    PPL_CHECK(pps);

    @CPP_CLASS@::iterator* it
      = term_to_handle<@CPP_CLASS@::iterator >(t_it, where);
    PPL_CHECK(it);

    @CPP_CLASS@::iterator& i = *it;
    i = pps->drop_disjunct(i);

    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_drop_disjuncts_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_drop_disjuncts(Prolog_term_ref t_pps,
                             Prolog_term_ref t_it1,
                             Prolog_term_ref t_it2) {
  static const char* where = "ppl_@CLASS@_drop_disjuncts/3";
  try {
    @CPP_CLASS@* pps = term_to_handle<@CPP_CLASS@ >(t_pps, where);
    PPL_CHECK(pps);

    @CPP_CLASS@::iterator* it1
      = term_to_handle<@CPP_CLASS@::iterator >(t_it1, where);
    PPL_CHECK(it1);
    @CPP_CLASS@::iterator* it2
      = term_to_handle<@CPP_CLASS@::iterator >(t_it2, where);
    PPL_CHECK(it2);

    @CPP_CLASS@::iterator& i1 = *it1;
    @CPP_CLASS@::iterator& i2 = *it2;
    pps->drop_disjuncts(i1, i2);

    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_add_disjunct_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_add_disjunct(Prolog_term_ref t_ph, Prolog_term_ref t_d) {
  static const char* where = "ppl_@CLASS@_add_disjunct/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    @DISJUNCT_TOPOLOGY@@A_DISJUNCT@* d =
      static_cast<@DISJUNCT_TOPOLOGY@@A_DISJUNCT@*>
      (term_to_handle<@DISJUNCT_TOPOLOGY@@A_DISJUNCT@ >(t_d, where));
    PPL_CHECK(d);
    ph->add_disjunct(*d);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_linear_@PARTITION@_code',
`dnl
extern "C" Prolog_foreign_return_type
ppl_@CLASS@_linear_@PARTITION@(Prolog_term_ref t_ph,
                             Prolog_term_ref t_qh,
                             Prolog_term_ref t_inters,
                             Prolog_term_ref t_pset) {
  try {
    static const char* where = "ppl_@CLASS@_linear_partition/4";
    @CPP_CLASS@* rfh;
    Pointset_Powerset<NNC_Polyhedron>* rsh;
`m4_ifelse(m4_current_interface, `Polyhedron',
  `m4_linear_partition_for_polyhedron_domains',
           `m4_linear_partition_for_non_polyhedron_domains')'
    Prolog_term_ref t_r_first = Prolog_new_term_ref();
    Prolog_term_ref t_r_second = Prolog_new_term_ref();
    Prolog_put_address(t_r_first, rfh);
    Prolog_put_address(t_r_second, rsh);
    if (Prolog_unify(t_inters, t_r_first)
        && Prolog_unify(t_pset, t_r_second)) {
      PPL_REGISTER(rfh);
      PPL_REGISTER(rsh);
      return PROLOG_SUCCESS;
    }
    else {
      delete rfh;
      delete rsh;
    }
  }
  CATCH_ALL;
}

')

m4_define(`m4_linear_partition_for_polyhedron_domains',
`    const Polyhedron* xph = term_to_handle<Polyhedron>(t_ph, where);
    if (Interfaces::is_necessarily_closed_for_interfaces(*xph)) {
      const C_Polyhedron* ph = term_to_handle<C_Polyhedron>(t_ph, where);
      const C_Polyhedron* qh = term_to_handle<C_Polyhedron>(t_qh, where);
      PPL_CHECK(ph);
      PPL_CHECK(qh);
      std::pair<C_Polyhedron|COMMA| Pointset_Powerset<NNC_Polyhedron> >
        r = linear_partition(*ph, *qh);
      rfh = new C_Polyhedron(0, EMPTY);
      rsh = new Pointset_Powerset<NNC_Polyhedron>(0, EMPTY);
      swap(*rfh, r.first);
      swap(*rsh, r.second);
    }
    else {
      const NNC_Polyhedron* ph = term_to_handle<NNC_Polyhedron>(t_ph, where);
      const NNC_Polyhedron* qh = term_to_handle<NNC_Polyhedron>(t_qh, where);
      PPL_CHECK(ph);
      PPL_CHECK(qh);
      std::pair<NNC_Polyhedron|COMMA| Pointset_Powerset<NNC_Polyhedron> >
        r = linear_partition(*ph, *qh);
      rfh = new NNC_Polyhedron(0, EMPTY);
      rsh = new Pointset_Powerset<NNC_Polyhedron>(0, EMPTY);
      swap(*rfh, r.first);
      swap(*rsh, r.second);
    }
')

m4_define(`m4_linear_partition_for_non_polyhedron_domains',
`  const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
  const @CPP_CLASS@* qh = term_to_handle<@CPP_CLASS@ >(t_qh, where);
  PPL_CHECK(ph);
  PPL_CHECK(qh);
  std::pair<@CPP_CLASS@|COMMA| Pointset_Powerset<NNC_Polyhedron> >
    r = linear_partition(*ph, *qh);
  rfh = new @CPP_CLASS@(0, EMPTY);
  rsh = new Pointset_Powerset<NNC_Polyhedron>(0, EMPTY);
  swap(*rfh, r.first);
  swap(*rsh, r.second);
')

m4_define(`ppl_@CLASS@_approximate_@PARTITION@_code',
  `dnl
  extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_approximate_@PARTITION@(Prolog_term_ref t_ph,
                                    Prolog_term_ref t_qh,
                                    Prolog_term_ref t_finite,
                                    Prolog_term_ref t_inters,
                                    Prolog_term_ref t_pset) {
  static const char* where = "ppl_@CLASS@_approximate_partition/5";
  try {
    const @CLASS@* ph =
      term_to_handle<@CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    const @CLASS@* qh =
      term_to_handle<@CLASS@ >(t_qh, where);
    PPL_CHECK(qh);
    bool finite;

    std::pair<@CLASS@|COMMA| Pointset_Powerset<Grid> > r =
      approximate_partition(*ph, *qh, finite);

    @CLASS@* rfh = new @CLASS@(EMPTY);
    swap(*rfh, r.first);

    Pointset_Powerset<Grid>* rsh =
      new Pointset_Powerset<Grid>(EMPTY);
    swap(*rsh, r.second);

    Prolog_term_ref t_b = Prolog_new_term_ref();
    Prolog_term_ref t_r_first = Prolog_new_term_ref();
    Prolog_term_ref t_r_second = Prolog_new_term_ref();
    Prolog_atom a = (finite ? a_true : a_false);
    Prolog_put_atom(t_b, a);

    Prolog_put_address(t_r_first, rfh);
    Prolog_put_address(t_r_second, rsh);

    if (Prolog_unify(t_inters, t_r_first)
        && Prolog_unify(t_pset, t_r_second
                        && Prolog_unify(t_finite, t_b)))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_relation_with_@RELATION_REPRESENT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_relation_with_@RELATION_REPRESENT@(Prolog_term_ref t_ph,
                                                 Prolog_term_ref t_c,
                                                 Prolog_term_ref t_r) {
  static const char* where =
    "ppl_@CLASS@_relation_with_@RELATION_REPRESENT@/3";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
relation_with_@RELATION_REPRESENT@_code
      if (Prolog_unify(t_r, tail))
        return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`relation_with_constraint_code', `
  Poly_Con_Relation r = ph->relation_with(build_constraint(t_c, where));

  Prolog_term_ref tail = Prolog_new_term_ref();
  Prolog_put_atom(tail, a_nil);
  while (r != Poly_Con_Relation::nothing()) {
    if (r.implies(Poly_Con_Relation::is_disjoint())) {
      Prolog_term_ref t_dis = Prolog_new_term_ref();
      Prolog_put_atom(t_dis, a_is_disjoint);
      Prolog_construct_cons(tail, t_dis, tail);
      r = r - Poly_Con_Relation::is_disjoint();
    }
    else if (r.implies(Poly_Con_Relation::strictly_intersects())) {
      Prolog_term_ref t_sin = Prolog_new_term_ref();
      Prolog_put_atom(t_sin, a_strictly_intersects);
      Prolog_construct_cons(tail, t_sin, tail);
      r = r - Poly_Con_Relation::strictly_intersects();
    }
    else if (r.implies(Poly_Con_Relation::is_included())) {
      Prolog_term_ref t_inc = Prolog_new_term_ref();
      Prolog_put_atom(t_inc, a_is_included);
      Prolog_construct_cons(tail, t_inc, tail);
      r = r - Poly_Con_Relation::is_included();
    }
    else if (r.implies(Poly_Con_Relation::saturates())) {
      Prolog_term_ref t_sat = Prolog_new_term_ref();
      Prolog_put_atom(t_sat, a_saturates);
      Prolog_construct_cons(tail, t_sat, tail);
      r = r - Poly_Con_Relation::saturates();
    }
   }
')

m4_define(`relation_with_congruence_code', `
  Poly_Con_Relation r = ph->relation_with(build_congruence(t_c, where));

  Prolog_term_ref tail = Prolog_new_term_ref();
  Prolog_put_atom(tail, a_nil);
  while (r != Poly_Con_Relation::nothing()) {
    if (r.implies(Poly_Con_Relation::is_disjoint())) {
      Prolog_term_ref t_dis = Prolog_new_term_ref();
      Prolog_put_atom(t_dis, a_is_disjoint);
      Prolog_construct_cons(tail, t_dis, tail);
      r = r - Poly_Con_Relation::is_disjoint();
    }
    else if (r.implies(Poly_Con_Relation::strictly_intersects())) {
      Prolog_term_ref t_sin = Prolog_new_term_ref();
      Prolog_put_atom(t_sin, a_strictly_intersects);
      Prolog_construct_cons(tail, t_sin, tail);
      r = r - Poly_Con_Relation::strictly_intersects();
    }
    else if (r.implies(Poly_Con_Relation::is_included())) {
      Prolog_term_ref t_inc = Prolog_new_term_ref();
      Prolog_put_atom(t_inc, a_is_included);
      Prolog_construct_cons(tail, t_inc, tail);
      r = r - Poly_Con_Relation::is_included();
    }
    else if (r.implies(Poly_Con_Relation::saturates())) {
      Prolog_term_ref t_sat = Prolog_new_term_ref();
      Prolog_put_atom(t_sat, a_saturates);
      Prolog_construct_cons(tail, t_sat, tail);
      r = r - Poly_Con_Relation::saturates();
    }
    else
      break;
   }
')

m4_define(`relation_with_generator_code', `
  Poly_Gen_Relation r = ph->relation_with(build_generator(t_c, where));

  Prolog_term_ref tail = Prolog_new_term_ref();
Prolog_put_atom(tail, a_nil);
while (r != Poly_Gen_Relation::nothing()) {
  if (r.implies(Poly_Gen_Relation::subsumes())) {
    Prolog_term_ref t_sub = Prolog_new_term_ref();
    Prolog_put_atom(t_sub, a_subsumes);
    Prolog_construct_cons(tail, t_sub, tail);
    r = r - Poly_Gen_Relation::subsumes();
  }
 }
')

m4_define(`relation_with_grid_generator_code', `
  Poly_Gen_Relation r = ph->relation_with(build_grid_generator(t_c, where));

  Prolog_term_ref tail = Prolog_new_term_ref();
  Prolog_put_atom(tail, a_nil);
  while (r != Poly_Gen_Relation::nothing()) {
    if (r.implies(Poly_Gen_Relation::subsumes())) {
      Prolog_term_ref t_sub = Prolog_new_term_ref();
      Prolog_put_atom(t_sub, a_subsumes);
      Prolog_construct_cons(tail, t_sub, tail);
      r = r - Poly_Gen_Relation::subsumes();
    }
   }
')

m4_define(`ppl_@CLASS@_@HAS_PROPERTY@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@HAS_PROPERTY@(Prolog_term_ref t_ph) {
  static const char* where = "ppl_@CLASS@_@HAS_PROPERTY@/1";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    if (ph->@HAS_PROPERTY@())
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@SIMPLIFY@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@SIMPLIFY@(Prolog_term_ref t_ph) {
  static const char* where = "ppl_@CLASS@_@SIMPLIFY@/1";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->@SIMPLIFY@();
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimension_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_unconstrain_space_dimension(Prolog_term_ref t_ph,
                           Prolog_term_ref t_v) {
  static const char* where = "ppl_@CLASS@__unconstrain/1";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->unconstrain(term_to_Variable(t_v, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_unconstrain_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_unconstrain_space_dimensions(Prolog_term_ref t_ph,
                           Prolog_term_ref t_vlist) {
  static const char* where = "ppl_@CLASS@__unconstrain/1";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    Variables_Set unconstrain_variables;
    Prolog_term_ref v = Prolog_new_term_ref();
    while (Prolog_is_cons(t_vlist)) {
      Prolog_get_cons(t_vlist, v, t_vlist);
      unconstrain_variables.insert(term_to_Variable(v, where).id());
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_vlist, where);
    ph->unconstrain(unconstrain_variables);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_constrains_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_constrains(Prolog_term_ref t_ph,
                          Prolog_term_ref t_v) {
  static const char* where = "ppl_@CLASS@__constrains/1";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    if (ph->constrains(term_to_Variable(t_v, where)))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_bounds_from_@ABOVEBELOW@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_bounds_from_@ABOVEBELOW@(Prolog_term_ref t_ph,
                                       Prolog_term_ref t_expr) {
  static const char* where = "ppl_@CLASS@_bounds_from_@ABOVEBELOW@/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    Linear_Expression l = build_linear_expression(t_expr, where);
    if (ph->bounds_from_@ABOVEBELOW@(l))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_has_@UPPERLOWER@_bound_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_has_@UPPERLOWER@_bound(Prolog_term_ref t_ph,
                       Prolog_term_ref t_v,
                       Prolog_term_ref t_n, Prolog_term_ref t_d,
                       Prolog_term_ref t_closed) {
  static const char* where = "ppl_@CLASS@_has_@UPPERLOWER@_bound/5";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    const Variable v = term_to_Variable(t_v, where);
    PPL_DIRTY_TEMP_COEFFICIENT(n);
    PPL_DIRTY_TEMP_COEFFICIENT(d);
    bool closed;
    if (ph->has_@UPPERLOWER@_bound(v, n, d, closed)) {
      Prolog_term_ref t = Prolog_new_term_ref();
      Prolog_atom a = (closed ? a_true : a_false);
      Prolog_put_atom(t, a);
      if (Prolog_unify_Coefficient(t_n, n)
          && Prolog_unify_Coefficient(t_d, d)
          && Prolog_unify(t_closed, t))
        return PROLOG_SUCCESS;
    }
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@MAXMIN@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@MAXMIN@(Prolog_term_ref t_ph, Prolog_term_ref t_le_expr,
                       Prolog_term_ref t_n,  Prolog_term_ref t_d,
                       Prolog_term_ref t_maxmin) {
  static const char* where = "ppl_@CLASS@_@MAXMIN@/5";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    const Linear_Expression le = build_linear_expression(t_le_expr, where);
    PPL_DIRTY_TEMP_COEFFICIENT(n);
    PPL_DIRTY_TEMP_COEFFICIENT(d);
    bool maxmin;
    if (ph->@MAXMIN@(le, n, d, maxmin)) {
      Prolog_term_ref t = Prolog_new_term_ref();
      Prolog_atom a = (maxmin ? a_true : a_false);
      Prolog_put_atom(t, a);
      if (Prolog_unify_Coefficient(t_n, n)
          && Prolog_unify_Coefficient(t_d, d)
          && Prolog_unify(t_maxmin, t))
        return PROLOG_SUCCESS;
    }
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@MAXMIN@_with_point_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@MAXMIN@_with_point(Prolog_term_ref t_ph,
                                  Prolog_term_ref t_le_expr,
                                  Prolog_term_ref t_n, Prolog_term_ref t_d,
                                  Prolog_term_ref t_maxmin, Prolog_term_ref t_g) {
  static const char* where = "ppl_@CLASS@_@MAXMIN@_with_point/6";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    const Linear_Expression le = build_linear_expression(t_le_expr, where);
    PPL_DIRTY_TEMP_COEFFICIENT(n);
    PPL_DIRTY_TEMP_COEFFICIENT(d);
    bool maxmin;
    Generator g(point());
    if (ph->@MAXMIN@(le, n, d, maxmin, g)) {
      Prolog_term_ref t = Prolog_new_term_ref();
      Prolog_atom a = (maxmin ? a_true : a_false);
      Prolog_put_atom(t, a);
      if (Prolog_unify_Coefficient(t_n, n)
          && Prolog_unify_Coefficient(t_d, d)
          && Prolog_unify(t_maxmin, t)
          && Prolog_unify(t_g, generator_term(g)))
        return PROLOG_SUCCESS;
    }
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@COMPARISON@_@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@COMPARISON@_@CLASS@(Prolog_term_ref t_lhs,
                                   Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_@COMPARISON@_@CLASS@/2";
  try {
    const @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    if (lhs->@COMPARISON@(*rhs))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_equals_@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_equals_@CLASS@(Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_equals_@CLASS@/2";
  try {
    const @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    if (*lhs == *rhs)
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_OK_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_OK(Prolog_term_ref t_ph) {
  static const char* where = "ppl_@CLASS@_OK/1";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    if (ph->OK())
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_add_@CLASS_REPRESENT@(Prolog_term_ref t_ph, Prolog_term_ref t_c) {
  static const char* where = "ppl_@CLASS@_add_@CLASS_REPRESENT@/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->add_@CLASS_REPRESENT@(build_@CLASS_REPRESENT@(t_c, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_refine_with_@REFINE_REPRESENT@(Prolog_term_ref t_ph,
                                             Prolog_term_ref t_c) {
  static const char* where = "ppl_@CLASS@_refine_with_@REFINE_REPRESENT@/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->refine_with_@REFINE_REPRESENT@(build_@REFINE_REPRESENT@(t_c, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_add_@CLASS_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_add_@CLASS_REPRESENT@s(Prolog_term_ref t_ph,
                                   Prolog_term_ref t_clist) {
  static const char* where = "ppl_@CLASS@_add_@CLASS_REPRESENT@s/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    @!CLASS_REPRESENT@_System cs;
    Prolog_term_ref c = Prolog_new_term_ref();

    while (Prolog_is_cons(t_clist)) {
      Prolog_get_cons(t_clist, c, t_clist);
      cs.insert(build_@CLASS_REPRESENT@(c, where));
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_clist, where);

    ph->add_@CLASS_REPRESENT@s(cs);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s(Prolog_term_ref t_ph,
                                   Prolog_term_ref t_clist) {
  static const char* where = "ppl_@CLASS@_refine_with_@REFINE_REPRESENT@s/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    @!REFINE_REPRESENT@_System cs;
    Prolog_term_ref c = Prolog_new_term_ref();

    while (Prolog_is_cons(t_clist)) {
      Prolog_get_cons(t_clist, c, t_clist);
      cs.insert(build_@REFINE_REPRESENT@(c, where));
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_clist, where);

    ph->refine_with_@REFINE_REPRESENT@s(cs);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`bop_assign_code',
`namespace Parma_Polyhedra_Library {

namespace Interfaces {

namespace Prolog {

Prolog_foreign_return_type
bop_assign(Prolog_term_ref t_lhs,
           Prolog_term_ref t_rhs,
           void (@CPP_CLASS@::* bop_assign)(const @CPP_CLASS@&),
           const char* where) {
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    (lhs->*bop_assign)(*rhs);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

} // namespace Prolog

} // namespace Interfaces

} // namespace Parma_Polyhedra_Library

using namespace Parma_Polyhedra_Library::Interfaces::Prolog;

')

m4_define(`ppl_@CLASS@_@BINOP@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@BINOP@
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_@BINOP@";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    lhs->@BINOP@(*rhs);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_simplify_using_context_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_simplify_using_context_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs, Prolog_term_ref t_b) {
  static const char* where = "ppl_@CLASS@_simplify_using_context_assign";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    Prolog_term_ref t_is_intersect = Prolog_new_term_ref();
    Prolog_atom is_intersect
      = (lhs->simplify_using_context_assign(*rhs) ? a_true : a_false);
    Prolog_put_atom(t_is_intersect, is_intersect);
    if (Prolog_unify(t_b, t_is_intersect))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@UB_EXACT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@UB_EXACT@
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_@UB_EXACT@";
  try {
`m4_ifelse(m4_current_interface, `Polyhedron',
  `m4_ub_exact_for_polyhedron_domains',
           `m4_ub_exact_for_non_polyhedron_domains')'
  }
  CATCH_ALL;
}

')

m4_define(`m4_ub_exact_for_polyhedron_domains',
`   const Polyhedron* xlhs = term_to_handle<Polyhedron >(t_lhs, where);
 if (Interfaces::is_necessarily_closed_for_interfaces(*xlhs)) {
     C_Polyhedron* lhs = term_to_handle<C_Polyhedron >(t_lhs, where);
     const C_Polyhedron* rhs = term_to_handle<C_Polyhedron >(t_rhs, where);
     PPL_CHECK(lhs);
     PPL_CHECK(rhs);
     return lhs->@UB_EXACT@(*rhs) ? PROLOG_SUCCESS : PROLOG_FAILURE;
   }
   else {
     NNC_Polyhedron* lhs = term_to_handle<NNC_Polyhedron >(t_lhs, where);
     const NNC_Polyhedron* rhs = term_to_handle<NNC_Polyhedron >(t_rhs, where);
     PPL_CHECK(lhs);
     PPL_CHECK(rhs);
     return lhs->@UB_EXACT@(*rhs) ? PROLOG_SUCCESS : PROLOG_FAILURE;
   }
')

m4_define(`m4_ub_exact_for_non_polyhedron_domains',
`   @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    return lhs->@UB_EXACT@(*rhs) ? PROLOG_SUCCESS : PROLOG_FAILURE;
')

m4_define(`ppl_@CLASS@_@AFFIMAGE@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@AFFIMAGE@
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_v, Prolog_term_ref t_le, Prolog_term_ref t_d) {
  const char* where = "ppl_@CLASS@_@AFFIMAGE@/4";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->@AFFIMAGE@(term_to_Variable(t_v, where),
                   build_linear_expression(t_le, where),
                   term_to_Coefficient(t_d, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_generalized_@AFFIMAGE@_with_congruence
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_v, Prolog_term_ref t_r, Prolog_term_ref t_le,
   Prolog_term_ref t_d, Prolog_term_ref t_m) {
  static const char* where = "ppl_@CLASS@_generalized_@AFFIMAGE@/6";
  try {
    @CLASS@* ph = term_to_handle<@CLASS@>(t_ph, where);
    PPL_CHECK(ph);
    ph->generalized_@AFFIMAGE@(term_to_Variable(t_v, where),
                               term_to_relation_symbol(t_r, where),
                               build_linear_expression(t_le, where),
                               term_to_Coefficient(t_d, where),
                               term_to_Coefficient(t_m, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_generalized_@AFFIMAGE@
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_v, Prolog_term_ref t_r, Prolog_term_ref t_le,
   Prolog_term_ref t_d) {
  static const char* where = "ppl_@CLASS@_generalized_@AFFIMAGE@/5";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->generalized_@AFFIMAGE@(term_to_Variable(t_v, where),
                               term_to_relation_symbol(t_r, where),
                               build_linear_expression(t_le, where),
                               term_to_Coefficient(t_d, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_with_congruence
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_lhs, Prolog_term_ref t_r, Prolog_term_ref t_rhs,
   Prolog_term_ref t_m) {
  static const char* where = "ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs/5";
  try {
    @CLASS@* ph = term_to_handle<@CLASS@>(t_ph, where);
    PPL_CHECK(ph);
    ph->generalized_@AFFIMAGE@(build_linear_expression(t_lhs, where),
                               term_to_relation_symbol(t_r, where),
                               build_linear_expression(t_rhs, where),
                               term_to_Coefficient(t_m, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_lhs, Prolog_term_ref t_r, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_generalized_@AFFIMAGE@_lhs_rhs/4";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    Relation_Symbol r = term_to_relation_symbol(t_r, where);
    ph->generalized_@AFFIMAGE@(build_linear_expression(t_lhs, where),
                               r,
                               build_linear_expression(t_rhs, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_bounded_@AFFIMAGE@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_bounded_@AFFIMAGE@
  (Prolog_term_ref t_ph,
   Prolog_term_ref t_v, Prolog_term_ref t_lb_le, Prolog_term_ref t_ub_le,
   Prolog_term_ref t_d) {
  static const char* where = "ppl_@CLASS@_bounded_@AFFIMAGE@/5";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->bounded_@AFFIMAGE@(term_to_Variable(t_v, where),
                           build_linear_expression(t_lb_le, where),
                           build_linear_expression(t_ub_le, where),
                           term_to_Coefficient(t_d, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
  (Prolog_term_ref t_pset) {
  static const char* where = "ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@/1";
  try {
    @TOPOLOGY@@CPP_CLASS@* pset = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset, where);
    PPL_CHECK(pset);
    if (Parma_Polyhedra_Library::termination_test_@TERMINATION_ID@(*pset))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
  (Prolog_term_ref t_pset_before, Prolog_term_ref t_pset_after) {
  static const char* where
      = "ppl_termination_test_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2/2";
  try {
    @TOPOLOGY@@CPP_CLASS@* pset_before
       = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset_before, where);
    @TOPOLOGY@@CPP_CLASS@* pset_after
       = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset_after, where);
    PPL_CHECK(pset_before);
    PPL_CHECK(pset_after);
    if (Parma_Polyhedra_Library
        ::termination_test_@TERMINATION_ID@_2(*pset_before, *pset_after))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
  (Prolog_term_ref t_pset,
   Prolog_term_ref t_g) {
  static const char* where
     = "ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@/2";
  try {
    @TOPOLOGY@@CPP_CLASS@* pset = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset, where);
    Generator gg(point());
    PPL_CHECK(pset);
    if (Parma_Polyhedra_Library
        ::one_affine_ranking_function_@TERMINATION_ID@(*pset, gg)
        && Prolog_unify(t_g, generator_term(gg)))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
  (Prolog_term_ref t_pset_before,
   Prolog_term_ref t_pset_after,
   Prolog_term_ref t_g) {
  static const char* where
     = "ppl_one_affine_ranking_function_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2/3";
  try {
    @TOPOLOGY@@CPP_CLASS@* pset_before
       = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset_before, where);
    @TOPOLOGY@@CPP_CLASS@* pset_after
       = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset_after, where);
    Generator gg(point());
    PPL_CHECK(pset_before);
    PPL_CHECK(pset_after);
    if (Parma_Polyhedra_Library
        ::one_affine_ranking_function_@TERMINATION_ID@_2(*pset_before,
                                                         *pset_after,
                                                         gg)
        && Prolog_unify(t_g, generator_term(gg)))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@
  (Prolog_term_ref t_pset,
   Prolog_term_ref t_ph) {
  static const char* where =
      "ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@/2";
  try {
    @TOPOLOGY@@CPP_CLASS@* pset = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset, where);
    PPL_CHECK(pset);
    @A_TERMINATION_ID@Polyhedron* ph = new @A_TERMINATION_ID@Polyhedron();
    Parma_Polyhedra_Library
      ::all_affine_ranking_functions_@TERMINATION_ID@(*pset, *ph);
    Prolog_term_ref tmp = Prolog_new_term_ref();
    Prolog_put_address(tmp, ph);
    if (Prolog_unify(t_ph, tmp)) {
      PPL_REGISTER(ph);
      return PROLOG_SUCCESS;
    }
    else
      delete ph;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2
  (Prolog_term_ref t_pset_before,
   Prolog_term_ref t_pset_after,
   Prolog_term_ref t_ph) {
  static const char* where =
      "ppl_all_affine_ranking_functions_@TERMINATION_ID@_@TOPOLOGY@@CLASS@_2/3";
  try {
    @TOPOLOGY@@CPP_CLASS@* pset_before
       = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset_before, where);
    @TOPOLOGY@@CPP_CLASS@* pset_after
       = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset_after, where);
    PPL_CHECK(pset_before);
    PPL_CHECK(pset_after);
    @A_TERMINATION_ID@Polyhedron* ph = new @A_TERMINATION_ID@Polyhedron();
    Parma_Polyhedra_Library
      ::all_affine_ranking_functions_@TERMINATION_ID@_2(*pset_before,
                                                        *pset_after,
                                                        *ph);
    Prolog_term_ref tmp = Prolog_new_term_ref();
    Prolog_put_address(tmp, ph);
    if (Prolog_unify(t_ph, tmp)) {
      PPL_REGISTER(ph);
      return PROLOG_SUCCESS;
    }
    else
      delete ph;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@
  (Prolog_term_ref t_pset,
   Prolog_term_ref t_ph_decreasing,
   Prolog_term_ref t_ph_bounded) {
  static const char* where =
      "ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@/3";
  try {
    @TOPOLOGY@@CPP_CLASS@* pset = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset, where);
    PPL_CHECK(pset);
    C_Polyhedron* ph_decreasing = new C_Polyhedron();
    C_Polyhedron* ph_bounded = new C_Polyhedron();
    Parma_Polyhedra_Library
      ::all_affine_quasi_ranking_functions_MS(*pset,
                                              *ph_decreasing, *ph_bounded);
    Prolog_term_ref tmp_decreasing = Prolog_new_term_ref();
    Prolog_put_address(tmp_decreasing, ph_decreasing);
    Prolog_term_ref tmp_bounded = Prolog_new_term_ref();
    Prolog_put_address(tmp_bounded, ph_bounded);
    if (Prolog_unify(t_ph_decreasing, tmp_decreasing)
        && Prolog_unify(t_ph_bounded, tmp_bounded)) {
      PPL_REGISTER(ph_decreasing);
      PPL_REGISTER(ph_bounded);
      return PROLOG_SUCCESS;
    }
    else {
      delete ph_decreasing;
      delete ph_bounded;
    }
  }
  CATCH_ALL;
}

')

m4_define(`ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2
  (Prolog_term_ref t_pset_before,
   Prolog_term_ref t_pset_after,
   Prolog_term_ref t_ph_decreasing,
   Prolog_term_ref t_ph_bounded) {
  static const char* where =
      "ppl_all_affine_quasi_ranking_functions_MS_@TOPOLOGY@@CLASS@_2/4";
  try {
    @TOPOLOGY@@CPP_CLASS@* pset_before
       = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset_before, where);
    @TOPOLOGY@@CPP_CLASS@* pset_after
       = term_to_handle<@TOPOLOGY@@CPP_CLASS@ >(t_pset_after, where);
    PPL_CHECK(pset_before);
    PPL_CHECK(pset_after);
    C_Polyhedron* ph_decreasing = new C_Polyhedron();
    C_Polyhedron* ph_bounded = new C_Polyhedron();
    Parma_Polyhedra_Library
      ::all_affine_quasi_ranking_functions_MS_2(*pset_before, *pset_after,
                                                *ph_decreasing, *ph_bounded);
    Prolog_term_ref tmp_decreasing = Prolog_new_term_ref();
    Prolog_put_address(tmp_decreasing, ph_decreasing);
    Prolog_term_ref tmp_bounded = Prolog_new_term_ref();
    Prolog_put_address(tmp_bounded, ph_bounded);
    if (Prolog_unify(t_ph_decreasing, tmp_decreasing)
        && Prolog_unify(t_ph_bounded, tmp_bounded)) {
      PPL_REGISTER(ph_decreasing);
      PPL_REGISTER(ph_bounded);
      return PROLOG_SUCCESS;
    }
    else {
      delete ph_decreasing;
      delete ph_bounded;
    }
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
   Prolog_term_ref t_ti, Prolog_term_ref t_to) {
  static const char* where = "ppl_@CLASS@_@WIDEN@_widening_assign_with_tokens/4";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    unsigned t = term_to_unsigned<unsigned>(t_ti, where);
    lhs->@WIDEN@_widening_assign(*rhs, &t);
    if (unify_long(t_to, t))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@WIDEN@_widening_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@WIDEN@_widening_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_@WIDEN@_widening_assign/2";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    lhs->@WIDEN@_widening_assign(*rhs, 0);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_widening_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_widening_assign_with_tokens
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
   Prolog_term_ref t_ti, Prolog_term_ref t_to) {
  static const char* where = "ppl_@CLASS@_widening_assign_with_tokens/4";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    unsigned t = term_to_unsigned<unsigned>(t_ti, where);
    lhs->widening_assign(*rhs, &t);
    if (unify_long(t_to, t))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_widening_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_widening_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_widening_assign/2";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    lhs->widening_assign(*rhs, 0);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
   Prolog_term_ref t_ti, Prolog_term_ref t_to) {
  static const char* where = "ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_with_tokens/4";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);

    unsigned t = term_to_unsigned<unsigned>(t_ti, where);
    lhs->@EXTRAPOLATION@_extrapolation_assign(*rhs, &t);
    if (unify_long(t_to, t))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_@EXTRAPOLATION@_extrapolation_assign/2";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);

    lhs->@EXTRAPOLATION@_extrapolation_assign(*rhs, 0);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_@EXTRAPOLATION@_narrowing_assign/2";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    lhs->@EXTRAPOLATION@_narrowing_assign(*rhs);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')


dnl FIXME: This code is redundant as there is no options for tokens
dnl        in the current C++ code.
m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_with_tokens(
                                                                                      Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
                                                                                      Prolog_term_ref t_ti, Prolog_term_ref t_to) {
  static const char* where = "ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_with_tokens/4";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    PPL_CHECK(lhs);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(rhs);

    unsigned t = term_to_unsigned<unsigned>(t_ti, where);
    lhs->BHZ03_widening_assign<@A_DISJUNCT_WIDEN@_Certificate>
      (*rhs,
       widen_fun_ref(&@DISJUNCT_TOPOLOGY@@A_DISJUNCT@::@DISJUNCT_WIDEN@_widening_assign),
       &t);
    if (unify_long(t_to, t))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign_code',
  `extern "C" Prolog_foreign_return_type

  ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign(
                                                                          Prolog_term_ref t_lhs, Prolog_term_ref t_rhs) {
  static const char* where = "ppl_@CLASS@_BHZ03_@A_DISJUNCT_WIDEN@_@DISJUNCT_WIDEN@_widening_assign/2";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    PPL_CHECK(lhs);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(rhs);

    lhs->BHZ03_widening_assign<@A_DISJUNCT_WIDEN@_Certificate>
      (*rhs,
       widen_fun_ref(&@DISJUNCT_TOPOLOGY@@A_DISJUNCT@::@DISJUNCT_WIDEN@_widening_assign));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign_code',
  `extern "C" Prolog_foreign_return_type

  ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign(
                                                          Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
                                                          Prolog_term_ref t_d) {
  static const char* where = "ppl_@CLASS@_BGP99_@DISJUNCT_WIDEN@_extrapolation_assign/3";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    PPL_CHECK(lhs);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(rhs);

    lhs->BGP99_extrapolation_assign
      (*rhs,
       widen_fun_ref(&@DISJUNCT_TOPOLOGY@@A_DISJUNCT@::@DISJUNCT_WIDEN@_widening_assign),
       term_to_unsigned<unsigned>(t_d, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_BGP99_@DISJUNCT_EXTRAPOLATION@_extrapolation_assign_code',
  `extern "C" Prolog_foreign_return_type

  ppl_@CLASS@_BGP99_@DISJUNCT_EXTRAPOLATION@_extrapolation_assign(
                                                                  Prolog_term_ref t_lhs, Prolog_term_ref t_rhs,
                                                                  Prolog_term_ref t_d) {
  static const char* where = "ppl_@CLASS@_BGP99_@DISJUNCT_EXTRAPOLATION@_extrapolation_assign/3";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    PPL_CHECK(lhs);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(rhs);

    lhs->BGP99_extrapolation_assign
      (*rhs,
       widen_fun_ref(&@DISJUNCT_TOPOLOGY@@A_DISJUNCT@::@DISJUNCT_EXTRAPOLATION@_extrapolation_assign),
       term_to_unsigned<unsigned>(t_d, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs, Prolog_term_ref t_clist,
   Prolog_term_ref t_ti, Prolog_term_ref t_to) {
  static const char* where = "ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_with_tokens/5";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    @!CONSTRAINER@_System cs;
    Prolog_term_ref c = Prolog_new_term_ref();

    while (Prolog_is_cons(t_clist)) {
      Prolog_get_cons(t_clist, c, t_clist);
      cs.insert(build_@CONSTRAINER@(c, where));
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_clist, where);

    unsigned t = term_to_unsigned<unsigned>(t_ti, where);
    lhs->@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(*rhs, cs, &t);
    if (unify_long(t_to, t))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign
  (Prolog_term_ref t_lhs, Prolog_term_ref t_rhs, Prolog_term_ref t_clist) {
  static const char* where = "ppl_@CLASS@_@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign/3";
  try {
    @CPP_CLASS@* lhs = term_to_handle<@CPP_CLASS@ >(t_lhs, where);
    const @CPP_CLASS@* rhs = term_to_handle<@CPP_CLASS@ >(t_rhs, where);
    PPL_CHECK(lhs);
    PPL_CHECK(rhs);
    @!CONSTRAINER@_System cs;
    Prolog_term_ref c = Prolog_new_term_ref();

    while (Prolog_is_cons(t_clist)) {
      Prolog_get_cons(t_clist, c, t_clist);
      cs.insert(build_@CONSTRAINER@(c, where));
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_clist, where);

    lhs->@LIMITEDBOUNDED@_@WIDENEXPN@_extrapolation_assign(*rhs, cs, 0);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@
  (Prolog_term_ref t_ph, Prolog_term_ref t_nnd) {
  static const char* where = "ppl_@CLASS@_add_space_dimensions_@EMBEDPROJECT@/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    dimension_type d = term_to_unsigned<dimension_type>(t_nnd, where);
    ph->add_space_dimensions_@EMBEDPROJECT@(d);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_remove_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_remove_space_dimensions
  (Prolog_term_ref t_ph, Prolog_term_ref t_vlist) {
  static const char* where = "ppl_@CLASS@_remove_space_dimensions/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    Variables_Set dead_variables;
    Prolog_term_ref v = Prolog_new_term_ref();
    while (Prolog_is_cons(t_vlist)) {
      Prolog_get_cons(t_vlist, v, t_vlist);
      dead_variables.insert(term_to_Variable(v, where).id());
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_vlist, where);

    ph->remove_space_dimensions(dead_variables);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_remove_higher_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_remove_higher_space_dimensions
  (Prolog_term_ref t_ph, Prolog_term_ref t_nd) {
  static const char* where = "ppl_@CLASS@_remove_higher_space_dimensions/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->remove_higher_space_dimensions(term_to_unsigned<dimension_type>(t_nd,
                                                                        where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_expand_space_dimension_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_expand_space_dimension
  (Prolog_term_ref t_ph, Prolog_term_ref t_v, Prolog_term_ref t_nd) {
  static const char* where = "ppl_@CLASS@_expand_space_dimension/3";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->expand_space_dimension(term_to_Variable(t_v, where),
                               term_to_unsigned<dimension_type>(t_nd, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_fold_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_fold_space_dimensions
  (Prolog_term_ref t_ph, Prolog_term_ref t_vlist, Prolog_term_ref t_v) {
  static const char* where = "ppl_@CLASS@_fold_space_dimensions/3";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    Variables_Set fold_variables;
    Prolog_term_ref v = Prolog_new_term_ref();
    while (Prolog_is_cons(t_vlist)) {
      Prolog_get_cons(t_vlist, v, t_vlist);
      fold_variables.insert(term_to_Variable(v, where).id());
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_vlist, where);

    ph->fold_space_dimensions(fold_variables, term_to_Variable(t_v, where));
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_map_space_dimensions_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_map_space_dimensions
  (Prolog_term_ref t_ph, Prolog_term_ref t_pfunc) {
  static const char* where = "ppl_@CLASS@_map_space_dimensions/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    dimension_type space_dim = ph->space_dimension();
    PPL_CHECK(ph);
    Partial_Function pfunc;
    Prolog_term_ref t_pair = Prolog_new_term_ref();
    while (Prolog_is_cons(t_pfunc)) {
      Prolog_get_cons(t_pfunc, t_pair, t_pfunc);
      Prolog_atom functor;
      int arity;
      Prolog_get_compound_name_arity(t_pair, &functor, &arity);
      if (arity != 2 || functor != a_minus)
        return PROLOG_FAILURE;
      Prolog_term_ref t_i = Prolog_new_term_ref();
      Prolog_term_ref t_j = Prolog_new_term_ref();
      Prolog_get_arg(1, t_pair, t_i);
      Prolog_get_arg(2, t_pair, t_j);
      dimension_type i = term_to_Variable(t_i, where).id();
      dimension_type j = term_to_Variable(t_j, where).id();
      if (i >= space_dim)
        return PROLOG_FAILURE;
      pfunc.insert(i, j);
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_pfunc, where);

    ph->map_space_dimensions(pfunc);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_drop_some_non_integer_points
  (Prolog_term_ref t_ph, Prolog_term_ref t_cc) {
  static const char* where = "ppl_@CLASS@_drop_some_non_integer_points/2";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    Prolog_atom p_cc = term_to_complexity_class(t_cc, where);
    Complexity_Class cc;
    if (p_cc == a_polynomial)
      cc = POLYNOMIAL_COMPLEXITY;
    else if (p_cc == a_simplex)
      cc = SIMPLEX_COMPLEXITY;
    else
      cc = ANY_COMPLEXITY;

    ph->drop_some_non_integer_points(cc);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_drop_some_non_integer_points_2_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_drop_some_non_integer_points_2
  (Prolog_term_ref t_ph, Prolog_term_ref t_vlist, Prolog_term_ref t_cc) {
  static const char* where = "ppl_@CLASS@_drop_some_non_integer_points_2/3";
  try {
    @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    Variables_Set variables;
    Prolog_term_ref v = Prolog_new_term_ref();
    while (Prolog_is_cons(t_vlist)) {
      Prolog_get_cons(t_vlist, v, t_vlist);
      variables.insert(term_to_Variable(v, where).id());
    }

    // Check the list is properly terminated.
    check_nil_terminating(t_vlist, where);

    Prolog_atom p_cc = term_to_complexity_class(t_cc, where);
    Complexity_Class cc;
    if (p_cc == a_polynomial)
      cc = POLYNOMIAL_COMPLEXITY;
    else if (p_cc == a_simplex)
      cc = SIMPLEX_COMPLEXITY;
    else
      cc = ANY_COMPLEXITY;

    ph->drop_some_non_integer_points(variables, cc);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_ascii_dump_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_ascii_dump
  (Prolog_term_ref t_ph) {
  static const char* where = "ppl_@CLASS@_ascii_dump/1";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    ph->ascii_dump(std::cout);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

')

m4_define(`ppl_@CLASS@_@MEMBYTES@_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_@MEMBYTES@(Prolog_term_ref t_pps,
                         Prolog_term_ref t_m) {
  static const char* where = "ppl_@CLASS@_@MEMBYTES@/2";
  try {
    @CPP_CLASS@* pps = term_to_handle<@CPP_CLASS@ >(t_pps, where);
    PPL_CHECK(pps);

    if (unify_ulong(t_m, pps->@MEMBYTES@()))
      return PROLOG_SUCCESS;
  }
  CATCH_ALL;
}

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
      Prolog_term_ref t_ind) {
  static const char* where = "ppl_@CLASS@_wrap_assign/8";
  try {
    @CPP_CLASS@* pph = term_to_handle<@CPP_CLASS@ >(t_ph, where);

    Variables_Set vars;
    Prolog_term_ref v = Prolog_new_term_ref();
    while (Prolog_is_cons(t_vars)) {
      Prolog_get_cons(t_vars, v, t_vars);
      vars.insert(term_to_Variable(v, where).id());
    }
    // Check the list is properly terminated.
    check_nil_terminating(t_vars, where);

    Prolog_atom p_w = term_to_bounded_integer_type_width(t_w, where);
    Bounded_Integer_Type_Width w;
    if (p_w == a_bits_8)
      w = BITS_8;
    else if (p_w == a_bits_16)
      w = BITS_16;
    else if (p_w == a_bits_32)
      w = BITS_32;
    else if (p_w == a_bits_64)
      w = BITS_64;
    else
      w = BITS_128;

    Prolog_atom p_r = term_to_bounded_integer_type_representation(t_r, where);
    Bounded_Integer_Type_Representation r;
    if (p_r == a_unsigned)
      r = UNSIGNED;
    else
      r = SIGNED_2_COMPLEMENT;
    Prolog_atom p_o = term_to_bounded_integer_type_overflow(t_o, where);
    Bounded_Integer_Type_Overflow o;
    if (p_o == a_overflow_wraps)
      o = OVERFLOW_WRAPS;
    else if (p_o == a_overflow_undefined)
      o = OVERFLOW_UNDEFINED;
    else
      o = OVERFLOW_IMPOSSIBLE;

    Constraint_System cs;
    Prolog_term_ref c = Prolog_new_term_ref();
    while (Prolog_is_cons(t_cs)) {
      Prolog_get_cons(t_cs, c, t_cs);
      cs.insert(build_constraint(c, where));
    }
    // Check the list is properly terminated.
    check_nil_terminating(t_cs, where);

    unsigned complexity = term_to_unsigned<unsigned>(t_complexity, where);

    Prolog_atom p_ind = term_to_boolean(t_ind, where);
    bool ind = (p_ind == a_true) ? true : false;

    pph->wrap_assign(vars, w, r, o, &cs, complexity, ind);
    return PROLOG_SUCCESS;
  }
  CATCH_ALL;
 }

')

m4_define(`ppl_@CLASS@_frequency_code',
  `extern "C" Prolog_foreign_return_type
  ppl_@CLASS@_frequency(Prolog_term_ref t_ph,
                        Prolog_term_ref t_le_expr,
                        Prolog_term_ref t_freqn, Prolog_term_ref t_freqd,
                        Prolog_term_ref t_valn, Prolog_term_ref t_vald) {
  static const char* where = "ppl_@CLASS@_frequency/6";
  try {
    const @CPP_CLASS@* ph = term_to_handle<@CPP_CLASS@ >(t_ph, where);
    PPL_CHECK(ph);
    const Linear_Expression le = build_linear_expression(t_le_expr, where);
    PPL_DIRTY_TEMP_COEFFICIENT(freqn);
    PPL_DIRTY_TEMP_COEFFICIENT(freqd);
    PPL_DIRTY_TEMP_COEFFICIENT(valn);
    PPL_DIRTY_TEMP_COEFFICIENT(vald);
    if (ph->frequency(le, freqn, freqd, valn, vald)) {
      if (Prolog_unify_Coefficient(t_freqn, freqn)
          && Prolog_unify_Coefficient(t_freqd, freqd)
          && Prolog_unify_Coefficient(t_valn, valn)
          && Prolog_unify_Coefficient(t_vald, vald))
        return PROLOG_SUCCESS;
    }
  }
  CATCH_ALL;
}

')
