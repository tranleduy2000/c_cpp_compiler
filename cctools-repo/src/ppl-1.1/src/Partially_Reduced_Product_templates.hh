/* Partially_Reduced_Product class implementation:
   non-inline template functions.
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2013 BUGSENG srl (http://bugseng.com)

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
site: http://bugseng.com/products/ppl/ . */

#ifndef PPL_Partially_Reduced_Product_templates_hh
#define PPL_Partially_Reduced_Product_templates_hh 1

#include "Grid_Generator_defs.hh"
#include "Grid_Generator_System_defs.hh"
#include "Grid_Generator_System_inlines.hh"
#include <algorithm>
#include <deque>

namespace Parma_Polyhedra_Library {

template <typename D1, typename D2, typename R>
void
Partially_Reduced_Product<D1, D2, R>
::throw_space_dimension_overflow(const char* method,
                                 const char* reason) {
  std::ostringstream s;
  s << "PPL::Partially_Reduced_Product::" << method << ":" << std::endl
    << reason << ".";
  throw std::length_error(s.str());
}

template <typename D1, typename D2, typename R>
Constraint_System
Partially_Reduced_Product<D1, D2, R>::constraints() const {
  reduce();
  Constraint_System cs = d2.constraints();
  const Constraint_System& cs1 = d1.constraints();
  for (Constraint_System::const_iterator i = cs1.begin(),
         cs_end = cs1.end(); i != cs_end; ++i)
    cs.insert(*i);
  return cs;
}

template <typename D1, typename D2, typename R>
Constraint_System
Partially_Reduced_Product<D1, D2, R>::minimized_constraints() const {
  reduce();
  Constraint_System cs = d2.constraints();
  const Constraint_System& cs1 = d1.constraints();
  for (Constraint_System::const_iterator i = cs1.begin(),
         cs_end = cs1.end(); i != cs_end; ++i)
    cs.insert(*i);
  if (cs.has_strict_inequalities()) {
    NNC_Polyhedron ph(cs);
    return ph.minimized_constraints();
  }
  else {
    C_Polyhedron ph(cs);
    return ph.minimized_constraints();
  }
}

template <typename D1, typename D2, typename R>
Congruence_System
Partially_Reduced_Product<D1, D2, R>::congruences() const {
  reduce();
  Congruence_System cgs = d2.congruences();
  const Congruence_System& cgs1 = d1.congruences();
  for (Congruence_System::const_iterator i = cgs1.begin(),
         cgs_end = cgs1.end(); i != cgs_end; ++i)
    cgs.insert(*i);
  return cgs;
}

template <typename D1, typename D2, typename R>
Congruence_System
Partially_Reduced_Product<D1, D2, R>::minimized_congruences() const {
  reduce();
  Congruence_System cgs = d2.congruences();
  const Congruence_System& cgs1 = d1.congruences();
  for (Congruence_System::const_iterator i = cgs1.begin(),
         cgs_end = cgs1.end(); i != cgs_end; ++i)
    cgs.insert(*i);
  Grid gr(cgs);
  return gr.minimized_congruences();
}

template <typename D1, typename D2, typename R>
void
Partially_Reduced_Product<D1, D2, R>
::add_recycled_constraints(Constraint_System& cs) {
  if (d1.can_recycle_constraint_systems()) {
    d2.refine_with_constraints(cs);
    d1.add_recycled_constraints(cs);
  }
  else
    if (d2.can_recycle_constraint_systems()) {
      d1.refine_with_constraints(cs);
      d2.add_recycled_constraints(cs);
    }
    else {
      d1.add_constraints(cs);
      d2.add_constraints(cs);
    }
  clear_reduced_flag();
}

template <typename D1, typename D2, typename R>
void
Partially_Reduced_Product<D1, D2, R>
::add_recycled_congruences(Congruence_System& cgs) {
  if (d1.can_recycle_congruence_systems()) {
    d2.refine_with_congruences(cgs);
    d1.add_recycled_congruences(cgs);
  }
  else
    if (d2.can_recycle_congruence_systems()) {
      d1.refine_with_congruences(cgs);
      d2.add_recycled_congruences(cgs);
    }
    else {
      d1.add_congruences(cgs);
      d2.add_congruences(cgs);
    }
  clear_reduced_flag();
}

template <typename D1, typename D2, typename R>
Poly_Gen_Relation
Partially_Reduced_Product<D1, D2, R>
::relation_with(const Generator& g) const {
  reduce();
  if (Poly_Gen_Relation::nothing() == d1.relation_with(g)
      || Poly_Gen_Relation::nothing() == d2.relation_with(g))
    return Poly_Gen_Relation::nothing();
  else
    return Poly_Gen_Relation::subsumes();
}

template <typename D1, typename D2, typename R>
Poly_Con_Relation
Partially_Reduced_Product<D1, D2, R>
::relation_with(const Constraint& c) const {
  reduce();
  Poly_Con_Relation relation1 = d1.relation_with(c);
  Poly_Con_Relation relation2 = d2.relation_with(c);

  Poly_Con_Relation result = Poly_Con_Relation::nothing();

  if (relation1.implies(Poly_Con_Relation::is_included()))
    result = result && Poly_Con_Relation::is_included();
  else if (relation2.implies(Poly_Con_Relation::is_included()))
    result = result && Poly_Con_Relation::is_included();
  if (relation1.implies(Poly_Con_Relation::saturates()))
    result = result && Poly_Con_Relation::saturates();
  else if (relation2.implies(Poly_Con_Relation::saturates()))
    result = result && Poly_Con_Relation::saturates();
  if (relation1.implies(Poly_Con_Relation::is_disjoint()))
    result = result && Poly_Con_Relation::is_disjoint();
  else if (relation2.implies(Poly_Con_Relation::is_disjoint()))
    result = result && Poly_Con_Relation::is_disjoint();

  return result;
}

template <typename D1, typename D2, typename R>
Poly_Con_Relation
Partially_Reduced_Product<D1, D2, R>
::relation_with(const Congruence& cg) const {
  reduce();
  Poly_Con_Relation relation1 = d1.relation_with(cg);
  Poly_Con_Relation relation2 = d2.relation_with(cg);

  Poly_Con_Relation result = Poly_Con_Relation::nothing();

  if (relation1.implies(Poly_Con_Relation::is_included()))
    result = result && Poly_Con_Relation::is_included();
  else if (relation2.implies(Poly_Con_Relation::is_included()))
    result = result && Poly_Con_Relation::is_included();
  if (relation1.implies(Poly_Con_Relation::saturates()))
    result = result && Poly_Con_Relation::saturates();
  else if (relation2.implies(Poly_Con_Relation::saturates()))
    result = result && Poly_Con_Relation::saturates();
  if (relation1.implies(Poly_Con_Relation::is_disjoint()))
    result = result && Poly_Con_Relation::is_disjoint();
  else if (relation2.implies(Poly_Con_Relation::is_disjoint()))
    result = result && Poly_Con_Relation::is_disjoint();

  return result;
}

template <typename D1, typename D2, typename R>
bool
Partially_Reduced_Product<D1, D2, R>
::maximize(const Linear_Expression& expr,
           Coefficient& sup_n,
           Coefficient& sup_d,
           bool& maximum) const {
  reduce();

  if (is_empty())
    return false;

  PPL_DIRTY_TEMP_COEFFICIENT(sup1_n);
  PPL_DIRTY_TEMP_COEFFICIENT(sup1_d);
  PPL_DIRTY_TEMP_COEFFICIENT(sup2_n);
  PPL_DIRTY_TEMP_COEFFICIENT(sup2_d);
  bool maximum1;
  bool maximum2;
  bool r1 = d1.maximize(expr, sup1_n, sup1_d, maximum1);
  bool r2 = d2.maximize(expr, sup2_n, sup2_d, maximum2);
  // If neither is bounded from above, return false.
  if (!r1 && !r2)
    return false;
  // If only d2 is bounded from above, then use the values for d2.
  if (!r1) {
    sup_n = sup2_n;
    sup_d = sup2_d;
    maximum = maximum2;
    return true;
  }
  // If only d1 is bounded from above, then use the values for d1.
  if (!r2) {
    sup_n = sup1_n;
    sup_d = sup1_d;
    maximum = maximum1;
    return true;
  }
  // If both d1 and d2 are bounded from above, then use the minimum values.
  if (sup2_d * sup1_n >= sup1_d * sup2_n) {
    sup_n = sup1_n;
    sup_d = sup1_d;
    maximum = maximum1;
  }
  else {
    sup_n = sup2_n;
    sup_d = sup2_d;
    maximum = maximum2;
  }
  return true;
}

template <typename D1, typename D2, typename R>
bool
Partially_Reduced_Product<D1, D2, R>
::minimize(const Linear_Expression& expr,
           Coefficient& inf_n,
           Coefficient& inf_d,
           bool& minimum) const {
  reduce();

  if (is_empty())
    return false;
  PPL_ASSERT(reduced);

  PPL_DIRTY_TEMP_COEFFICIENT(inf1_n);
  PPL_DIRTY_TEMP_COEFFICIENT(inf1_d);
  PPL_DIRTY_TEMP_COEFFICIENT(inf2_n);
  PPL_DIRTY_TEMP_COEFFICIENT(inf2_d);
  bool minimum1;
  bool minimum2;
  bool r1 = d1.minimize(expr, inf1_n, inf1_d, minimum1);
  bool r2 = d2.minimize(expr, inf2_n, inf2_d, minimum2);
  // If neither is bounded from below, return false.
  if (!r1 && !r2)
    return false;
  // If only d2 is bounded from below, then use the values for d2.
  if (!r1) {
    inf_n = inf2_n;
    inf_d = inf2_d;
    minimum = minimum2;
    return true;
  }
  // If only d1 is bounded from below, then use the values for d1.
  if (!r2) {
    inf_n = inf1_n;
    inf_d = inf1_d;
    minimum = minimum1;
    return true;
  }
  // If both d1 and d2 are bounded from below, then use the minimum values.
  if (inf2_d * inf1_n <= inf1_d * inf2_n) {
    inf_n = inf1_n;
    inf_d = inf1_d;
    minimum = minimum1;
  }
  else {
    inf_n = inf2_n;
    inf_d = inf2_d;
    minimum = minimum2;
  }
  return true;
}

template <typename D1, typename D2, typename R>
bool
Partially_Reduced_Product<D1, D2, R>
::maximize(const Linear_Expression& expr,
           Coefficient& sup_n,
           Coefficient& sup_d,
           bool& maximum,
           Generator& g) const {
  reduce();

  if (is_empty())
    return false;
  PPL_ASSERT(reduced);

  PPL_DIRTY_TEMP_COEFFICIENT(sup1_n);
  PPL_DIRTY_TEMP_COEFFICIENT(sup1_d);
  PPL_DIRTY_TEMP_COEFFICIENT(sup2_n);
  PPL_DIRTY_TEMP_COEFFICIENT(sup2_d);
  bool maximum1;
  bool maximum2;
  Generator g1(point());
  Generator g2(point());
  bool r1 = d1.maximize(expr, sup1_n, sup1_d, maximum1, g1);
  bool r2 = d2.maximize(expr, sup2_n, sup2_d, maximum2, g2);
  // If neither is bounded from above, return false.
  if (!r1 && !r2)
    return false;
  // If only d2 is bounded from above, then use the values for d2.
  if (!r1) {
    sup_n = sup2_n;
    sup_d = sup2_d;
    maximum = maximum2;
    g = g2;
    return true;
  }
  // If only d1 is bounded from above, then use the values for d1.
  if (!r2) {
    sup_n = sup1_n;
    sup_d = sup1_d;
    maximum = maximum1;
    g = g1;
    return true;
  }
  // If both d1 and d2 are bounded from above, then use the minimum values.
  if (sup2_d * sup1_n >= sup1_d * sup2_n) {
    sup_n = sup1_n;
    sup_d = sup1_d;
    maximum = maximum1;
    g = g1;
  }
  else {
    sup_n = sup2_n;
    sup_d = sup2_d;
    maximum = maximum2;
    g = g2;
  }
  return true;
}

template <typename D1, typename D2, typename R>
bool
Partially_Reduced_Product<D1, D2, R>
::minimize(const Linear_Expression& expr,
           Coefficient& inf_n,
           Coefficient& inf_d,
           bool& minimum,
           Generator& g) const {
  reduce();

  if (is_empty())
    return false;
  PPL_ASSERT(reduced);

  PPL_DIRTY_TEMP_COEFFICIENT(inf1_n);
  PPL_DIRTY_TEMP_COEFFICIENT(inf1_d);
  PPL_DIRTY_TEMP_COEFFICIENT(inf2_n);
  PPL_DIRTY_TEMP_COEFFICIENT(inf2_d);
  bool minimum1;
  bool minimum2;
  Generator g1(point());
  Generator g2(point());
  bool r1 = d1.minimize(expr, inf1_n, inf1_d, minimum1, g1);
  bool r2 = d2.minimize(expr, inf2_n, inf2_d, minimum2, g2);
  // If neither is bounded from below, return false.
  if (!r1 && !r2)
    return false;
  // If only d2 is bounded from below, then use the values for d2.
  if (!r1) {
    inf_n = inf2_n;
    inf_d = inf2_d;
    minimum = minimum2;
    g = g2;
    return true;
  }
  // If only d1 is bounded from below, then use the values for d1.
  if (!r2) {
    inf_n = inf1_n;
    inf_d = inf1_d;
    minimum = minimum1;
    g = g1;
    return true;
  }
  // If both d1 and d2 are bounded from below, then use the minimum values.
  if (inf2_d * inf1_n <= inf1_d * inf2_n) {
    inf_n = inf1_n;
    inf_d = inf1_d;
    minimum = minimum1;
    g = g1;
  }
  else {
    inf_n = inf2_n;
    inf_d = inf2_d;
    minimum = minimum2;
    g = g2;
  }
  return true;
}

template <typename D1, typename D2, typename R>
inline bool
Partially_Reduced_Product<D1, D2, R>::OK() const {
  if (reduced) {
    Partially_Reduced_Product<D1, D2, R> dp1 = *this;
    Partially_Reduced_Product<D1, D2, R> dp2 = *this;
    /* Force dp1 reduction */
    dp1.clear_reduced_flag();
    dp1.reduce();
    if (dp1 != dp2)
      return false;
  }
  return d1.OK() && d2.OK();
}

template <typename D1, typename D2, typename R>
bool
Partially_Reduced_Product<D1, D2, R>::ascii_load(std::istream& s) {
  const char yes = '+';
  const char no = '-';
  std::string str;
  if (!(s >> str) || str != "Partially_Reduced_Product")
    return false;
  if (!(s >> str)
      || (str[0] != yes && str[0] != no)
      || str.substr(1) != "reduced")
    return false;
  reduced = (str[0] == yes);
  if (!(s >> str) || str != "Domain")
    return false;
  if (!(s >> str) || str != "1:")
    return false;
  if (!d1.ascii_load(s))
    return false;
  if (!(s >> str) || str != "Domain")
    return false;
  if (!(s >> str) || str != "2:")
    return false;
  return d2.ascii_load(s);
}

template <typename D1, typename D2>
void Smash_Reduction<D1, D2>::product_reduce(D1& d1, D2& d2) {
  using std::swap;
  if (d2.is_empty()) {
    if (!d1.is_empty()) {
      D1 new_d1(d1.space_dimension(), EMPTY);
      swap(d1, new_d1);
    }
  }
  else if (d1.is_empty()) {
    D2 new_d2(d2.space_dimension(), EMPTY);
    swap(d2, new_d2);
  }
}

template <typename D1, typename D2>
void Constraints_Reduction<D1, D2>::product_reduce(D1& d1, D2& d2) {
  if (d1.is_empty() || d2.is_empty()) {
    // If one of the components is empty, do the smash reduction and return.
    Parma_Polyhedra_Library::Smash_Reduction<D1, D2> sr;
    sr.product_reduce(d1, d2);
    return;
  }
  else {
    using std::swap;
    dimension_type space_dim = d1.space_dimension();
    d1.refine_with_constraints(d2.minimized_constraints());
    if (d1.is_empty()) {
      D2 new_d2(space_dim, EMPTY);
      swap(d2, new_d2);
      return;
    }
    d2.refine_with_constraints(d1.minimized_constraints());
    if (d2.is_empty()) {
      D1 new_d1(space_dim, EMPTY);
      swap(d1, new_d1);
    }
  }
}

/* Auxiliary procedure for the Congruences_Reduction() method.
   If more than one hyperplane defined by congruence cg intersect
   d2, then d1 and d2 are unchanged; if exactly one intersects d2, then
   the corresponding equality is added to d1 and d2;
   otherwise d1 and d2 are set empty. */
template <typename D1, typename D2>
bool shrink_to_congruence_no_check(D1& d1, D2& d2, const Congruence& cg) {
  // It is assumed that cg is a proper congruence.
  PPL_ASSERT(cg.modulus() != 0);
  // It is assumed that cg is satisfied by all points in d1.
  PPL_ASSERT(d1.relation_with(cg) == Poly_Con_Relation::is_included());

  Linear_Expression e(cg.expression());

  // Find the maximum and minimum bounds for the domain element d with the
  // linear expression e.
  PPL_DIRTY_TEMP_COEFFICIENT(max_numer);
  PPL_DIRTY_TEMP_COEFFICIENT(max_denom);
  bool max_included;
  PPL_DIRTY_TEMP_COEFFICIENT(min_numer);
  PPL_DIRTY_TEMP_COEFFICIENT(min_denom);
  if (d2.maximize(e, max_numer, max_denom, max_included)) {
    bool min_included;
    if (d2.minimize(e, min_numer, min_denom, min_included)) {
      // Adjust values to allow for the denominators max_denom and min_denom.
      max_numer *= min_denom;
      min_numer *= max_denom;
      PPL_DIRTY_TEMP_COEFFICIENT(denom);
      PPL_DIRTY_TEMP_COEFFICIENT(mod);
      denom = max_denom * min_denom;
      mod = cg.modulus() * denom;
      // If the difference between the maximum and minimum bounds is more than
      // twice the modulus, then there will be two neighboring hyperplanes
      // defined by cg that are intersected by the domain element d;
      // there is no possible reduction in this case.
      PPL_DIRTY_TEMP_COEFFICIENT(mod2);
      mod2 = 2 * mod;
      if (max_numer - min_numer < mod2
          || (max_numer - min_numer == mod2 && (!max_included || !min_included)))
        {
          PPL_DIRTY_TEMP_COEFFICIENT(shrink_amount);
          PPL_DIRTY_TEMP_COEFFICIENT(max_decreased);
          PPL_DIRTY_TEMP_COEFFICIENT(min_increased);
          // Find the amount by which the maximum value may be decreased.
          shrink_amount = max_numer % mod;
          if (!max_included && shrink_amount == 0)
            shrink_amount = mod;
          if (shrink_amount < 0)
            shrink_amount += mod;
          max_decreased = max_numer - shrink_amount;
          // Find the amount by which the minimum value may be increased.
          shrink_amount = min_numer % mod;
          if (!min_included && shrink_amount == 0)
            shrink_amount = - mod;
          if (shrink_amount > 0)
            shrink_amount -= mod;
          min_increased = min_numer - shrink_amount;
          if (max_decreased == min_increased) {
            // The domain element d2 intersects exactly one hyperplane
            // defined by cg, so add the equality to d1 and d2.
            Constraint new_c(denom * e == min_increased);
            d1.refine_with_constraint(new_c);
            d2.refine_with_constraint(new_c);
            return true;
          }
          else {
            if (max_decreased < min_increased) {
              using std::swap;
              // In this case, d intersects no hyperplanes defined by cg,
              // so set d to empty and return false.
              D1 new_d1(d1.space_dimension(), EMPTY);
              swap(d1, new_d1);
              D2 new_d2(d2.space_dimension(), EMPTY);
              swap(d2, new_d2);
              return false;
            }
          }
        }
    }
  }
  return true;
}

template <typename D1, typename D2>
void
Congruences_Reduction<D1, D2>::product_reduce(D1& d1, D2& d2) {
  if (d1.is_empty() || d2.is_empty()) {
    // If one of the components is empty, do the smash reduction and return.
    Parma_Polyhedra_Library::Smash_Reduction<D1, D2> sr;
    sr.product_reduce(d1, d2);
    return;
  }
  // Use the congruences representing d1 to shrink both components.
  const Congruence_System cgs1 = d1.minimized_congruences();
  for (Congruence_System::const_iterator i = cgs1.begin(),
         cgs_end = cgs1.end(); i != cgs_end; ++i) {
    const Congruence& cg1 = *i;
    if (cg1.is_equality())
      d2.refine_with_congruence(cg1);
    else
      if (!Parma_Polyhedra_Library::
          shrink_to_congruence_no_check(d1, d2, cg1))
        // The product is empty.
        return;
  }
  // Use the congruences representing d2 to shrink both components.
  const Congruence_System cgs2 = d2.minimized_congruences();
  for (Congruence_System::const_iterator i = cgs2.begin(),
         cgs_end = cgs2.end(); i != cgs_end; ++i) {
    const Congruence& cg2 = *i;
    if (cg2.is_equality())
      d1.refine_with_congruence(cg2);
    else
      if (!Parma_Polyhedra_Library::
          shrink_to_congruence_no_check(d2, d1, cg2))
        // The product is empty.
        return;
  }
}

template <typename D1, typename D2>
void
Shape_Preserving_Reduction<D1, D2>::product_reduce(D1& d1, D2& d2) {
  // First do the congruences reduction.
  Parma_Polyhedra_Library::Congruences_Reduction<D1, D2> cgr;
  cgr.product_reduce(d1, d2);
  if (d1.is_empty())
    return;

  PPL_DIRTY_TEMP_COEFFICIENT(freq_n);
  PPL_DIRTY_TEMP_COEFFICIENT(freq_d);
  PPL_DIRTY_TEMP_COEFFICIENT(val_n);
  PPL_DIRTY_TEMP_COEFFICIENT(val_d);

  // Use the constraints representing d2.
  Constraint_System cs = d2.minimized_constraints();
  Constraint_System refining_cs;
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i) {
    const Constraint& c = *i;
    if (c.is_equality())
      continue;
    // Check the frequency and value of the linear expression for
    // the constraint `c'.
    Linear_Expression le(c.expression());
    if (!d1.frequency(le, freq_n, freq_d, val_n, val_d))
      // Nothing to do.
      continue;
    if (val_n == 0)
      // Nothing to do.
      continue;
    // Adjust the value of the inhomogeneous term to satisfy
    // the implied congruence.
    if (val_n < 0) {
      val_n = val_n*freq_d + val_d*freq_n;
      val_d *= freq_d;
    }
    le *= val_d;
    le -= val_n;
    refining_cs.insert(le >= 0);
  }
  d2.refine_with_constraints(refining_cs);

  // Use the constraints representing d1.
  cs = d1.minimized_constraints();
  refining_cs.clear();
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i) {
    const Constraint& c = *i;
    if (c.is_equality())
      // Equalities already shared.
      continue;
    // Check the frequency and value of the linear expression for
    // the constraint `c'.
    Linear_Expression le(c.expression());
    if (!d2.frequency(le, freq_n, freq_d, val_n, val_d))
      // Nothing to do.
      continue;
    if (val_n == 0)
      // Nothing to do.
      continue;
    // Adjust the value of the inhomogeneous term to satisfy
    // the implied congruence.
    if (val_n < 0) {
      val_n = val_n*freq_d + val_d*freq_n;
      val_d *= freq_d;
    }
    le *= val_d;
    le -= val_n;
    refining_cs.insert(le >= 0);
  }
  d1.refine_with_constraints(refining_cs);

  // The reduction may have introduced additional equalities
  // so these must be shared with the other component.
  Parma_Polyhedra_Library::Constraints_Reduction<D1, D2> cr;
  cr.product_reduce(d1, d2);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Partially_Reduced_Product_templates_hh)
