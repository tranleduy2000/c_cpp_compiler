/* Pointset_Powerset class implementation: non-inline template functions.
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

#ifndef PPL_Pointset_Powerset_templates_hh
#define PPL_Pointset_Powerset_templates_hh 1

#include "Constraint_defs.hh"
#include "Constraint_System_defs.hh"
#include "Constraint_System_inlines.hh"
#include "C_Polyhedron_defs.hh"
#include "NNC_Polyhedron_defs.hh"
#include "Variables_Set_defs.hh"
#include <algorithm>
#include <deque>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace Parma_Polyhedra_Library {

template <typename PSET>
void
Pointset_Powerset<PSET>::add_disjunct(const PSET& ph) {
  Pointset_Powerset& x = *this;
  if (x.space_dimension() != ph.space_dimension()) {
    std::ostringstream s;
    s << "PPL::Pointset_Powerset<PSET>::add_disjunct(ph):\n"
      << "this->space_dimension() == " << x.space_dimension() << ", "
      << "ph.space_dimension() == " << ph.space_dimension() << ".";
    throw std::invalid_argument(s.str());
  }
  x.sequence.push_back(Determinate<PSET>(ph));
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <>
template <typename QH>
Pointset_Powerset<NNC_Polyhedron>
::Pointset_Powerset(const Pointset_Powerset<QH>& y,
                    Complexity_Class complexity)
  : Base(), space_dim(y.space_dimension()) {
  Pointset_Powerset& x = *this;
  for (typename Pointset_Powerset<QH>::const_iterator i = y.begin(),
         y_end = y.end(); i != y_end; ++i)
    x.sequence.push_back(Determinate<NNC_Polyhedron>
                         (NNC_Polyhedron(i->pointset(), complexity)));

  // FIXME: If the domain elements can be represented _exactly_ as NNC
  // polyhedra, then having x.reduced = y.reduced is correct. This is
  // the case if the domains are both linear and convex which holds
  // for all the currently supported instantiations except for
  // Grids; for this reason the Grid specialization has a
  // separate implementation.  For any non-linear or non-convex
  // domains (e.g., a domain of Intervals with restrictions or a
  // domain of circles) that may be supported in the future, the
  // assignment x.reduced = y.reduced will be a bug.
  x.reduced = y.reduced;

  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
template <typename QH>
Pointset_Powerset<PSET>
::Pointset_Powerset(const Pointset_Powerset<QH>& y,
                    Complexity_Class complexity)
  : Base(), space_dim(y.space_dimension()) {
  Pointset_Powerset& x = *this;
  for (typename Pointset_Powerset<QH>::const_iterator i = y.begin(),
         y_end = y.end(); i != y_end; ++i)
    x.sequence.push_back(Determinate<PSET>(PSET(i->pointset(), complexity)));
  // Note: this might be non-reduced even when `y' is known to be
  // omega-reduced, because the constructor of PSET may have made
  // different QH elements to become comparable.
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::concatenate_assign(const Pointset_Powerset& y) {
  Pointset_Powerset& x = *this;
  // Ensure omega-reduction here, since what follows has quadratic complexity.
  x.omega_reduce();
  y.omega_reduce();
  Pointset_Powerset<PSET> new_x(x.space_dim + y.space_dim, EMPTY);
  for (const_iterator xi = x.begin(), x_end = x.end(),
         y_begin = y.begin(), y_end = y.end(); xi != x_end; ) {
    for (const_iterator yi = y_begin; yi != y_end; ++yi) {
      Det_PSET zi = *xi;
      zi.concatenate_assign(*yi);
      PPL_ASSERT_HEAVY(!zi.is_bottom());
      new_x.sequence.push_back(zi);
    }
    ++xi;
    if ((abandon_expensive_computations != 0)
        && (xi != x_end) && (y_begin != y_end)) {
      // Hurry up!
      PSET x_ph = xi->pointset();
      for (++xi; xi != x_end; ++xi)
        x_ph.upper_bound_assign(xi->pointset());
      const_iterator yi = y_begin;
      PSET y_ph = yi->pointset();
      for (++yi; yi != y_end; ++yi)
        y_ph.upper_bound_assign(yi->pointset());
      x_ph.concatenate_assign(y_ph);
      swap(x, new_x);
      x.add_disjunct(x_ph);
      PPL_ASSERT_HEAVY(x.OK());
      return;
    }
  }
  swap(x, new_x);
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::add_constraint(const Constraint& c) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().add_constraint(c);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::refine_with_constraint(const Constraint& c) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().refine_with_constraint(c);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::add_constraints(const Constraint_System& cs) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().add_constraints(cs);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::refine_with_constraints(const Constraint_System& cs) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().refine_with_constraints(cs);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::add_congruence(const Congruence& cg) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().add_congruence(cg);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::refine_with_congruence(const Congruence& cg) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().refine_with_congruence(cg);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::add_congruences(const Congruence_System& cgs) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().add_congruences(cgs);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::refine_with_congruences(const Congruence_System& cgs) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().refine_with_congruences(cgs);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::unconstrain(const Variable var) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().unconstrain(var);
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::unconstrain(const Variables_Set& vars) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().unconstrain(vars);
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::add_space_dimensions_and_embed(dimension_type m) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().add_space_dimensions_and_embed(m);
  x.space_dim += m;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::add_space_dimensions_and_project(dimension_type m) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().add_space_dimensions_and_project(m);
  x.space_dim += m;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::remove_space_dimensions(const Variables_Set& vars) {
  Pointset_Powerset& x = *this;
  Variables_Set::size_type num_removed = vars.size();
  if (num_removed > 0) {
    for (Sequence_iterator si = x.sequence.begin(),
           s_end = x.sequence.end(); si != s_end; ++si) {
      si->pointset().remove_space_dimensions(vars);
      x.reduced = false;
    }
    x.space_dim -= num_removed;
    PPL_ASSERT_HEAVY(x.OK());
  }
}

template <typename PSET>
void
Pointset_Powerset<PSET>
::remove_higher_space_dimensions(dimension_type new_dimension) {
  Pointset_Powerset& x = *this;
  if (new_dimension < x.space_dim) {
    for (Sequence_iterator si = x.sequence.begin(),
           s_end = x.sequence.end(); si != s_end; ++si) {
      si->pointset().remove_higher_space_dimensions(new_dimension);
      x.reduced = false;
    }
    x.space_dim = new_dimension;
    PPL_ASSERT_HEAVY(x.OK());
  }
}

template <typename PSET>
template <typename Partial_Function>
void
Pointset_Powerset<PSET>::map_space_dimensions(const Partial_Function& pfunc) {
  Pointset_Powerset& x = *this;
  if (x.is_bottom()) {
    dimension_type n = 0;
    for (dimension_type i = x.space_dim; i-- > 0; ) {
      dimension_type new_i;
      if (pfunc.maps(i, new_i))
        ++n;
    }
    x.space_dim = n;
  }
  else {
    Sequence_iterator s_begin = x.sequence.begin();
    for (Sequence_iterator si = s_begin,
           s_end = x.sequence.end(); si != s_end; ++si)
      si->pointset().map_space_dimensions(pfunc);
    x.space_dim = s_begin->pointset().space_dimension();
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::expand_space_dimension(Variable var,
                                                dimension_type m) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().expand_space_dimension(var, m);
  x.space_dim += m;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::fold_space_dimensions(const Variables_Set& vars,
                                               Variable dest) {
  Pointset_Powerset& x = *this;
  Variables_Set::size_type num_folded = vars.size();
  if (num_folded > 0) {
    for (Sequence_iterator si = x.sequence.begin(),
           s_end = x.sequence.end(); si != s_end; ++si)
      si->pointset().fold_space_dimensions(vars, dest);
  }
  x.space_dim -= num_folded;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::affine_image(Variable var,
                                      const Linear_Expression& expr,
                                      Coefficient_traits::const_reference
                                      denominator) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().affine_image(var, expr, denominator);
    // Note that the underlying domain can apply conservative approximation:
    // that is why it would not be correct to make the loss of reduction
    // conditional on `var' and `expr'.
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::affine_preimage(Variable var,
                                         const Linear_Expression& expr,
                                         Coefficient_traits::const_reference
                                         denominator) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().affine_preimage(var, expr, denominator);
    // Note that the underlying domain can apply conservative approximation:
    // that is why it would not be correct to make the loss of reduction
    // conditional on `var' and `expr'.
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}


template <typename PSET>
void
Pointset_Powerset<PSET>
::generalized_affine_image(const Linear_Expression& lhs,
                           const Relation_Symbol relsym,
                           const Linear_Expression& rhs) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().generalized_affine_image(lhs, relsym, rhs);
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>
::generalized_affine_preimage(const Linear_Expression& lhs,
                              const Relation_Symbol relsym,
                              const Linear_Expression& rhs) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().generalized_affine_preimage(lhs, relsym, rhs);
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>
::generalized_affine_image(Variable var,
                           const Relation_Symbol relsym,
                           const Linear_Expression& expr,
                           Coefficient_traits::const_reference denominator) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().generalized_affine_image(var, relsym, expr, denominator);
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>
::generalized_affine_preimage(Variable var,
                              const Relation_Symbol relsym,
                              const Linear_Expression& expr,
                              Coefficient_traits::const_reference
                              denominator) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().generalized_affine_preimage(var, relsym, expr, denominator);
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}


template <typename PSET>
void
Pointset_Powerset<PSET>
::bounded_affine_image(Variable var,
                       const Linear_Expression& lb_expr,
                       const Linear_Expression& ub_expr,
                       Coefficient_traits::const_reference denominator) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().bounded_affine_image(var, lb_expr, ub_expr, denominator);
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>
::bounded_affine_preimage(Variable var,
                          const Linear_Expression& lb_expr,
                          const Linear_Expression& ub_expr,
                          Coefficient_traits::const_reference denominator) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    si->pointset().bounded_affine_preimage(var, lb_expr, ub_expr,
                                          denominator);
    x.reduced = false;
  }
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
dimension_type
Pointset_Powerset<PSET>::affine_dimension() const {
  // The affine dimension of the powerset is the affine dimension of
  // the smallest vector space in which it can be embedded.
  const Pointset_Powerset& x = *this;
  C_Polyhedron x_ph(space_dim, EMPTY);

  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    PSET pi(si->pointset());
    if (!pi.is_empty()) {
      C_Polyhedron phi(space_dim);
      const Constraint_System& cs = pi.minimized_constraints();
      for (Constraint_System::const_iterator i = cs.begin(),
             cs_end = cs.end(); i != cs_end; ++i) {
        const Constraint& c = *i;
        if (c.is_equality())
          phi.add_constraint(c);
      }
      x_ph.poly_hull_assign(phi);
    }
  }

  return x_ph.affine_dimension();
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::is_universe() const {
  const Pointset_Powerset& x = *this;
  // Exploit omega-reduction, if already computed.
  if (x.is_omega_reduced())
    return x.size() == 1 && x.begin()->pointset().is_universe();

  // A powerset is universe iff one of its disjuncts is.
  for (const_iterator x_i = x.begin(), x_end = x.end(); x_i != x_end; ++x_i)
    if (x_i->pointset().is_universe()) {
      // Speculative omega-reduction, if it is worth.
      if (x.size() > 1) {
        Pointset_Powerset<PSET> universe(x.space_dimension(), UNIVERSE);
        Pointset_Powerset& xx = const_cast<Pointset_Powerset&>(x);
        swap(xx, universe);
      }
      return true;
    }
  return false;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::is_empty() const {
  const Pointset_Powerset& x = *this;
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    if (!si->pointset().is_empty())
      return false;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::is_discrete() const {
  const Pointset_Powerset& x = *this;
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    if (!si->pointset().is_discrete())
      return false;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::is_topologically_closed() const {
  const Pointset_Powerset& x = *this;
  // The powerset must be omega-reduced before checking
  // topological closure.
  x.omega_reduce();
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    if (!si->pointset().is_topologically_closed())
      return false;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::is_bounded() const {
  const Pointset_Powerset& x = *this;
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    if (!si->pointset().is_bounded())
      return false;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::constrains(Variable var) const {
  const Pointset_Powerset& x = *this;
  // `var' should be one of the dimensions of the powerset.
  const dimension_type var_space_dim = var.space_dimension();
  if (x.space_dimension() < var_space_dim) {
    std::ostringstream s;
    s << "PPL::Pointset_Powerset<PSET>::constrains(v):\n"
      << "this->space_dimension() == " << x.space_dimension() << ", "
      << "v.space_dimension() == " << var_space_dim << ".";
    throw std::invalid_argument(s.str());
  }
  // omega_reduction needed, since a redundant disjunct may constrain var.
  x.omega_reduce();
  // An empty powerset constrains all variables.
  if (x.is_empty())
    return true;
  for (const_iterator x_i = x.begin(), x_end = x.end(); x_i != x_end; ++x_i)
    if (x_i->pointset().constrains(var))
      return true;
  return false;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::is_disjoint_from(const Pointset_Powerset& y) const {
  const Pointset_Powerset& x = *this;
  for (Sequence_const_iterator si = x.sequence.begin(),
         x_s_end = x.sequence.end(); si != x_s_end; ++si) {
    const PSET& pi = si->pointset();
    for (Sequence_const_iterator sj = y.sequence.begin(),
           y_s_end = y.sequence.end(); sj != y_s_end; ++sj) {
      const PSET& pj = sj->pointset();
      if (!pi.is_disjoint_from(pj))
        return false;
    }
  }
  return true;
}

template <typename PSET>
void
Pointset_Powerset<PSET>
::drop_some_non_integer_points(const Variables_Set& vars,
                               Complexity_Class complexity) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().drop_some_non_integer_points(vars, complexity);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>
::drop_some_non_integer_points(Complexity_Class complexity) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().drop_some_non_integer_points(complexity);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::topological_closure_assign() {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().topological_closure_assign();
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
bool
Pointset_Powerset<PSET>
::intersection_preserving_enlarge_element(PSET& dest) const {
  // FIXME: this is just an executable specification.
  const Pointset_Powerset& context = *this;
  PPL_ASSERT(context.space_dimension() == dest.space_dimension());
  bool nonempty_intersection = false;
  // TODO: maybe use a *sorted* constraint system?
  PSET enlarged(context.space_dimension(), UNIVERSE);
  for (Sequence_const_iterator si = context.sequence.begin(),
         s_end = context.sequence.end(); si != s_end; ++si) {
    PSET context_i(si->pointset());
    context_i.intersection_assign(enlarged);
    PSET enlarged_i(dest);
    if (enlarged_i.simplify_using_context_assign(context_i))
      nonempty_intersection = true;
    // TODO: merge the sorted constraints of `enlarged' and `enlarged_i'?
    enlarged.intersection_assign(enlarged_i);
  }
  swap(dest, enlarged);
  return nonempty_intersection;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>
::simplify_using_context_assign(const Pointset_Powerset& y) {
  Pointset_Powerset& x = *this;

  // Omega reduction is required.
  // TODO: check whether it would be more efficient to Omega-reduce x
  // during the simplification process: when examining *si, we check
  // if it has been made redundant by any of the elements preceding it
  // (which have been already simplified).
  x.omega_reduce();
  if (x.is_empty())
    return false;
  y.omega_reduce();
  if (y.is_empty()) {
    x = y;
    return false;
  }

  if (y.size() == 1) {
    // More efficient, special handling of the singleton context case.
    const PSET& y_i = y.sequence.begin()->pointset();
    for (Sequence_iterator si = x.sequence.begin(),
           s_end = x.sequence.end(); si != s_end; ) {
      PSET& x_i = si->pointset();
      if (x_i.simplify_using_context_assign(y_i))
        ++si;
      else
        // Intersection is empty: drop the disjunct.
        si = x.sequence.erase(si);
    }
  }
  else {
    // The context is not a singleton.
    for (Sequence_iterator si = x.sequence.begin(),
           s_end = x.sequence.end(); si != s_end; ) {
      if (y.intersection_preserving_enlarge_element(si->pointset()))
        ++si;
      else
        // Intersection with `*si' is empty: drop the disjunct.
        si = x.sequence.erase(si);
    }
  }
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
  return !x.sequence.empty();
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::contains(const Pointset_Powerset& y) const {
  const Pointset_Powerset& x = *this;
  for (Sequence_const_iterator si = y.sequence.begin(),
         y_s_end = y.sequence.end(); si != y_s_end; ++si) {
    const PSET& pi = si->pointset();
    bool pi_is_contained = false;
    for (Sequence_const_iterator sj = x.sequence.begin(),
           x_s_end = x.sequence.end();
         (sj != x_s_end && !pi_is_contained);
         ++sj) {
      const PSET& pj = sj->pointset();
      if (pj.contains(pi))
        pi_is_contained = true;
    }
    if (!pi_is_contained)
      return false;
  }
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::strictly_contains(const Pointset_Powerset& y) const {
  /* omega reduction ensures that a disjunct of y cannot be strictly
     contained in one disjunct and also contained but not strictly
     contained in another disjunct of *this */
  const Pointset_Powerset& x = *this;
  x.omega_reduce();
  for (Sequence_const_iterator si = y.sequence.begin(),
         y_s_end = y.sequence.end(); si != y_s_end; ++si) {
    const PSET& pi = si->pointset();
    bool pi_is_strictly_contained = false;
    for (Sequence_const_iterator sj = x.sequence.begin(),
           x_s_end = x.sequence.end();
         (sj != x_s_end && !pi_is_strictly_contained); ++sj) {
      const PSET& pj = sj->pointset();
      if (pj.strictly_contains(pi))
        pi_is_strictly_contained = true;
    }
    if (!pi_is_strictly_contained)
      return false;
  }
  return true;
}

template <typename PSET>
Poly_Con_Relation
Pointset_Powerset<PSET>::relation_with(const Congruence& cg) const {
  const Pointset_Powerset& x = *this;

  /* *this is included in cg if every disjunct is included in cg */
  bool is_included = true;
  /* *this is disjoint with cg if every disjunct is disjoint with cg */
  bool is_disjoint = true;
  /* *this strictly_intersects with cg if some disjunct strictly
     intersects with cg */
  bool is_strictly_intersecting = false;
  /* *this saturates cg if some disjunct saturates cg and
     every disjunct is either disjoint from cg or saturates cg */
  bool saturates_once = false;
  bool may_saturate = true;
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    Poly_Con_Relation relation_i = si->pointset().relation_with(cg);
    if (!relation_i.implies(Poly_Con_Relation::is_included()))
      is_included = false;
    if (!relation_i.implies(Poly_Con_Relation::is_disjoint()))
      is_disjoint = false;
    if (relation_i.implies(Poly_Con_Relation::strictly_intersects()))
      is_strictly_intersecting = true;
    if (relation_i.implies(Poly_Con_Relation::saturates()))
      saturates_once = true;
    else if (!relation_i.implies(Poly_Con_Relation::is_disjoint()))
      may_saturate = false;
  }

  Poly_Con_Relation result = Poly_Con_Relation::nothing();
  if (is_included)
    result = result && Poly_Con_Relation::is_included();
  if (is_disjoint)
    result = result && Poly_Con_Relation::is_disjoint();
  if (is_strictly_intersecting)
    result = result && Poly_Con_Relation::strictly_intersects();
  if (saturates_once && may_saturate)
    result = result && Poly_Con_Relation::saturates();

  return result;
}

template <typename PSET>
Poly_Con_Relation
Pointset_Powerset<PSET>::relation_with(const Constraint& c) const {
  const Pointset_Powerset& x = *this;

  /* *this is included in c if every disjunct is included in c */
  bool is_included = true;
  /* *this is disjoint with c if every disjunct is disjoint with c */
  bool is_disjoint = true;
  /* *this strictly_intersects with c if some disjunct strictly
     intersects with c */
  bool is_strictly_intersecting = false;
  /* *this saturates c if some disjunct saturates c and
     every disjunct is either disjoint from c or saturates c */
  bool saturates_once = false;
  bool may_saturate = true;
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    Poly_Con_Relation relation_i = si->pointset().relation_with(c);
    if (!relation_i.implies(Poly_Con_Relation::is_included()))
      is_included = false;
    if (!relation_i.implies(Poly_Con_Relation::is_disjoint()))
      is_disjoint = false;
    if (relation_i.implies(Poly_Con_Relation::strictly_intersects()))
      is_strictly_intersecting = true;
    if (relation_i.implies(Poly_Con_Relation::saturates()))
      saturates_once = true;
    else if (!relation_i.implies(Poly_Con_Relation::is_disjoint()))
      may_saturate = false;
  }

  Poly_Con_Relation result = Poly_Con_Relation::nothing();
  if (is_included)
    result = result && Poly_Con_Relation::is_included();
  if (is_disjoint)
    result = result && Poly_Con_Relation::is_disjoint();
  if (is_strictly_intersecting)
    result = result && Poly_Con_Relation::strictly_intersects();
  if (saturates_once && may_saturate)
    result = result && Poly_Con_Relation::saturates();

  return result;
}

template <typename PSET>
Poly_Gen_Relation
Pointset_Powerset<PSET>::relation_with(const Generator& g) const {
  const Pointset_Powerset& x = *this;

  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    Poly_Gen_Relation relation_i = si->pointset().relation_with(g);
    if (relation_i.implies(Poly_Gen_Relation::subsumes()))
      return Poly_Gen_Relation::subsumes();
  }

  return Poly_Gen_Relation::nothing();
}

template <typename PSET>
bool
Pointset_Powerset<PSET>
::bounds_from_above(const Linear_Expression& expr) const {
  const Pointset_Powerset& x = *this;
  x.omega_reduce();
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    if (!si->pointset().bounds_from_above(expr))
      return false;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>
::bounds_from_below(const Linear_Expression& expr) const {
  const Pointset_Powerset& x = *this;
  x.omega_reduce();
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    if (!si->pointset().bounds_from_below(expr))
      return false;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::maximize(const Linear_Expression& expr,
                                  Coefficient& sup_n,
                                  Coefficient& sup_d,
                                  bool& maximum) const {
  const Pointset_Powerset& x = *this;
  x.omega_reduce();
  if (x.is_empty())
    return false;

  bool first = true;

  PPL_DIRTY_TEMP_COEFFICIENT(best_sup_n);
  PPL_DIRTY_TEMP_COEFFICIENT(best_sup_d);
  best_sup_n = 0;
  best_sup_d = 1;
  bool best_max = false;

  PPL_DIRTY_TEMP_COEFFICIENT(iter_sup_n);
  PPL_DIRTY_TEMP_COEFFICIENT(iter_sup_d);
  iter_sup_n = 0;
  iter_sup_d = 1;
  bool iter_max = false;

  PPL_DIRTY_TEMP_COEFFICIENT(tmp);

  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    if (!si->pointset().maximize(expr, iter_sup_n, iter_sup_d, iter_max))
      return false;
    else
      if (first) {
        first = false;
        best_sup_n = iter_sup_n;
        best_sup_d = iter_sup_d;
        best_max = iter_max;
      }
      else {
        tmp = (best_sup_n * iter_sup_d) - (iter_sup_n * best_sup_d);
        if (tmp < 0) {
          best_sup_n = iter_sup_n;
          best_sup_d = iter_sup_d;
          best_max = iter_max;
        }
        else if (tmp == 0)
          best_max = (best_max || iter_max);
      }
  }
  sup_n = best_sup_n;
  sup_d = best_sup_d;
  maximum = best_max;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::maximize(const Linear_Expression& expr,
                                  Coefficient& sup_n,
                                  Coefficient& sup_d,
                                  bool& maximum,
                                  Generator& g) const {
  const Pointset_Powerset& x = *this;
  x.omega_reduce();
  if (x.is_empty())
    return false;

  bool first = true;

  PPL_DIRTY_TEMP_COEFFICIENT(best_sup_n);
  PPL_DIRTY_TEMP_COEFFICIENT(best_sup_d);
  best_sup_n = 0;
  best_sup_d = 1;
  bool best_max = false;
  Generator best_g = point();

  PPL_DIRTY_TEMP_COEFFICIENT(iter_sup_n);
  PPL_DIRTY_TEMP_COEFFICIENT(iter_sup_d);
  iter_sup_n = 0;
  iter_sup_d = 1;
  bool iter_max = false;
  Generator iter_g = point();

  PPL_DIRTY_TEMP_COEFFICIENT(tmp);

  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    if (!si->pointset().maximize(expr,
                                 iter_sup_n, iter_sup_d, iter_max, iter_g))
      return false;
    else
      if (first) {
        first = false;
        best_sup_n = iter_sup_n;
        best_sup_d = iter_sup_d;
        best_max = iter_max;
        best_g = iter_g;
      }
      else {
        tmp = (best_sup_n * iter_sup_d) - (iter_sup_n * best_sup_d);
        if (tmp < 0) {
          best_sup_n = iter_sup_n;
          best_sup_d = iter_sup_d;
          best_max = iter_max;
          best_g = iter_g;
        }
        else if (tmp == 0) {
          best_max = (best_max || iter_max);
          best_g = iter_g;
        }
      }
  }
  sup_n = best_sup_n;
  sup_d = best_sup_d;
  maximum = best_max;
  g = best_g;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::minimize(const Linear_Expression& expr,
                                  Coefficient& inf_n,
                                  Coefficient& inf_d,
                                  bool& minimum) const {
  const Pointset_Powerset& x = *this;
  x.omega_reduce();
  if (x.is_empty())
    return false;

  bool first = true;

  PPL_DIRTY_TEMP_COEFFICIENT(best_inf_n);
  PPL_DIRTY_TEMP_COEFFICIENT(best_inf_d);
  best_inf_n = 0;
  best_inf_d = 1;
  bool best_min = false;

  PPL_DIRTY_TEMP_COEFFICIENT(iter_inf_n);
  PPL_DIRTY_TEMP_COEFFICIENT(iter_inf_d);
  iter_inf_n = 0;
  iter_inf_d = 1;
  bool iter_min = false;

  PPL_DIRTY_TEMP_COEFFICIENT(tmp);

  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    if (!si->pointset().minimize(expr, iter_inf_n, iter_inf_d, iter_min))
      return false;
    else
      if (first) {
        first = false;
        best_inf_n = iter_inf_n;
        best_inf_d = iter_inf_d;
        best_min = iter_min;
      }
      else {
        tmp = (best_inf_n * iter_inf_d) - (iter_inf_n * best_inf_d);
        if (tmp > 0) {
          best_inf_n = iter_inf_n;
          best_inf_d = iter_inf_d;
          best_min = iter_min;
        }
        else if (tmp == 0)
          best_min = (best_min || iter_min);
      }
  }
  inf_n = best_inf_n;
  inf_d = best_inf_d;
  minimum = best_min;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::minimize(const Linear_Expression& expr,
                                  Coefficient& inf_n,
                                  Coefficient& inf_d,
                                  bool& minimum,
                                  Generator& g) const {
  const Pointset_Powerset& x = *this;
  x.omega_reduce();
  if (x.is_empty())
    return false;

  bool first = true;

  PPL_DIRTY_TEMP_COEFFICIENT(best_inf_n);
  PPL_DIRTY_TEMP_COEFFICIENT(best_inf_d);
  best_inf_n = 0;
  best_inf_d = 1;
  bool best_min = false;
  Generator best_g = point();

  PPL_DIRTY_TEMP_COEFFICIENT(iter_inf_n);
  PPL_DIRTY_TEMP_COEFFICIENT(iter_inf_d);
  iter_inf_n = 0;
  iter_inf_d = 1;
  bool iter_min = false;
  Generator iter_g = point();

  PPL_DIRTY_TEMP_COEFFICIENT(tmp);

  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si) {
    if (!si->pointset().minimize(expr,
                                 iter_inf_n, iter_inf_d, iter_min, iter_g))
      return false;
    else
      if (first) {
        first = false;
        best_inf_n = iter_inf_n;
        best_inf_d = iter_inf_d;
        best_min = iter_min;
        best_g = iter_g;
      }
      else {
        tmp = (best_inf_n * iter_inf_d) - (iter_inf_n * best_inf_d);
        if (tmp > 0) {
          best_inf_n = iter_inf_n;
          best_inf_d = iter_inf_d;
          best_min = iter_min;
          best_g = iter_g;
        }
        else if (tmp == 0) {
          best_min = (best_min || iter_min);
          best_g = iter_g;
        }
      }
  }
  inf_n = best_inf_n;
  inf_d = best_inf_d;
  minimum = best_min;
  g = best_g;
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::contains_integer_point() const {
  const Pointset_Powerset& x = *this;
  for (Sequence_const_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    if (si->pointset().contains_integer_point())
      return true;
  return false;
}

template <typename PSET>
void
Pointset_Powerset<PSET>::wrap_assign(const Variables_Set& vars,
                                     Bounded_Integer_Type_Width w,
                                     Bounded_Integer_Type_Representation r,
                                     Bounded_Integer_Type_Overflow o,
                                     const Constraint_System* cs_p,
                                     unsigned complexity_threshold,
                                     bool wrap_individually) {
  Pointset_Powerset& x = *this;
  for (Sequence_iterator si = x.sequence.begin(),
         s_end = x.sequence.end(); si != s_end; ++si)
    si->pointset().wrap_assign(vars, w, r, o, cs_p,
                               complexity_threshold, wrap_individually);
  x.reduced = false;
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
void
Pointset_Powerset<PSET>::pairwise_reduce() {
  Pointset_Powerset& x = *this;
  // It is wise to omega-reduce before pairwise-reducing.
  x.omega_reduce();

  size_type n = x.size();
  size_type deleted;
  do {
    Pointset_Powerset new_x(x.space_dim, EMPTY);
    std::deque<bool> marked(n, false);
    deleted = 0;
    Sequence_iterator s_begin = x.sequence.begin();
    Sequence_iterator s_end = x.sequence.end();
    unsigned si_index = 0;
    for (Sequence_iterator si = s_begin; si != s_end; ++si, ++si_index) {
      if (marked[si_index])
        continue;
      PSET& pi = si->pointset();
      Sequence_const_iterator sj = si;
      unsigned sj_index = si_index;
      for (++sj, ++sj_index; sj != s_end; ++sj, ++sj_index) {
        if (marked[sj_index])
          continue;
        const PSET& pj = sj->pointset();
        if (pi.upper_bound_assign_if_exact(pj)) {
          marked[si_index] = true;
          marked[sj_index] = true;
          new_x.add_non_bottom_disjunct_preserve_reduction(pi);
          ++deleted;
          goto next;
        }
      }
    next:
      ;
    }
    iterator new_x_begin = new_x.begin();
    iterator new_x_end = new_x.end();
    unsigned xi_index = 0;
    for (const_iterator xi = x.begin(),
           x_end = x.end(); xi != x_end; ++xi, ++xi_index)
      if (!marked[xi_index])
        new_x_begin
          = new_x.add_non_bottom_disjunct_preserve_reduction(*xi,
                                                             new_x_begin,
                                                             new_x_end);
    using std::swap;
    swap(x.sequence, new_x.sequence);
    n -= deleted;
  } while (deleted > 0);
  PPL_ASSERT_HEAVY(x.OK());
}

template <typename PSET>
template <typename Widening>
void
Pointset_Powerset<PSET>::
BGP99_heuristics_assign(const Pointset_Powerset& y, Widening widen_fun) {
  // `x' is the current iteration value.
  Pointset_Powerset& x = *this;

#ifndef NDEBUG
  {
    // We assume that `y' entails `x'.
    const Pointset_Powerset<PSET> x_copy = x;
    const Pointset_Powerset<PSET> y_copy = y;
    PPL_ASSERT_HEAVY(y_copy.definitely_entails(x_copy));
  }
#endif

  size_type n = x.size();
  Pointset_Powerset new_x(x.space_dim, EMPTY);
  std::deque<bool> marked(n, false);
  const_iterator x_begin = x.begin();
  const_iterator x_end = x.end();
  unsigned i_index = 0;
  for (const_iterator i = x_begin,
         y_begin = y.begin(), y_end = y.end(); i != x_end; ++i, ++i_index)
    for (const_iterator j = y_begin; j != y_end; ++j) {
      const PSET& pi = i->pointset();
      const PSET& pj = j->pointset();
      if (pi.contains(pj)) {
        PSET pi_copy = pi;
        widen_fun(pi_copy, pj);
        new_x.add_non_bottom_disjunct_preserve_reduction(pi_copy);
        marked[i_index] = true;
      }
    }
  iterator new_x_begin = new_x.begin();
  iterator new_x_end = new_x.end();
  i_index = 0;
  for (const_iterator i = x_begin; i != x_end; ++i, ++i_index)
    if (!marked[i_index])
      new_x_begin
        = new_x.add_non_bottom_disjunct_preserve_reduction(*i,
                                                           new_x_begin,
                                                           new_x_end);
  using std::swap;
  swap(x.sequence, new_x.sequence);
  PPL_ASSERT_HEAVY(x.OK());
  PPL_ASSERT(x.is_omega_reduced());
}

template <typename PSET>
template <typename Widening>
void
Pointset_Powerset<PSET>::
BGP99_extrapolation_assign(const Pointset_Powerset& y,
                           Widening widen_fun,
                           unsigned max_disjuncts) {
  // `x' is the current iteration value.
  Pointset_Powerset& x = *this;

#ifndef NDEBUG
  {
    // We assume that `y' entails `x'.
    const Pointset_Powerset<PSET> x_copy = x;
    const Pointset_Powerset<PSET> y_copy = y;
    PPL_ASSERT_HEAVY(y_copy.definitely_entails(x_copy));
  }
#endif

  x.pairwise_reduce();
  if (max_disjuncts != 0)
    x.collapse(max_disjuncts);
  x.BGP99_heuristics_assign(y, widen_fun);
}

template <typename PSET>
template <typename Cert>
void
Pointset_Powerset<PSET>::
collect_certificates(std::map<Cert, size_type,
                     typename Cert::Compare>& cert_ms) const {
  const Pointset_Powerset& x = *this;
  PPL_ASSERT(x.is_omega_reduced());
  PPL_ASSERT(cert_ms.size() == 0);
  for (const_iterator i = x.begin(), end = x.end(); i != end; ++i) {
    Cert ph_cert(i->pointset());
    ++cert_ms[ph_cert];
  }
}

template <typename PSET>
template <typename Cert>
bool
Pointset_Powerset<PSET>::
is_cert_multiset_stabilizing(const std::map<Cert, size_type,
                             typename Cert::Compare>& y_cert_ms) const {
  typedef std::map<Cert, size_type, typename Cert::Compare> Cert_Multiset;
  Cert_Multiset x_cert_ms;
  collect_certificates(x_cert_ms);
  typename Cert_Multiset::const_iterator xi = x_cert_ms.begin();
  typename Cert_Multiset::const_iterator x_cert_ms_end = x_cert_ms.end();
  typename Cert_Multiset::const_iterator yi = y_cert_ms.begin();
  typename Cert_Multiset::const_iterator y_cert_ms_end = y_cert_ms.end();
  while (xi != x_cert_ms_end && yi != y_cert_ms_end) {
    const Cert& xi_cert = xi->first;
    const Cert& yi_cert = yi->first;
    switch (xi_cert.compare(yi_cert)) {
    case 0:
      // xi_cert == yi_cert: check the number of multiset occurrences.
      {
        const size_type& xi_count = xi->second;
        const size_type& yi_count = yi->second;
        if (xi_count == yi_count) {
          // Same number of occurrences: compare the next pair.
          ++xi;
          ++yi;
        }
        else
          // Different number of occurrences: can decide ordering.
          return xi_count < yi_count;
        break;
      }
    case 1:
      // xi_cert > yi_cert: it is not stabilizing.
      return false;

    case -1:
      // xi_cert < yi_cert: it is stabilizing.
      return true;
    }
  }
  // Here xi == x_cert_ms_end or yi == y_cert_ms_end.
  // Stabilization is achieved if `y_cert_ms' still has other elements.
  return yi != y_cert_ms_end;
}

template <typename PSET>
template <typename Cert, typename Widening>
void
Pointset_Powerset<PSET>::BHZ03_widening_assign(const Pointset_Powerset& y,
                                               Widening widen_fun) {
  // `x' is the current iteration value.
  Pointset_Powerset& x = *this;

#ifndef NDEBUG
  {
    // We assume that `y' entails `x'.
    const Pointset_Powerset<PSET> x_copy = x;
    const Pointset_Powerset<PSET> y_copy = y;
    PPL_ASSERT_HEAVY(y_copy.definitely_entails(x_copy));
  }
#endif

  // First widening technique: do nothing.

  // If `y' is the empty collection, do nothing.
  PPL_ASSERT(x.size() > 0);
  if (y.size() == 0)
    return;

  // Compute the poly-hull of `x'.
  PSET x_hull(x.space_dim, EMPTY);
  for (const_iterator i = x.begin(), x_end = x.end(); i != x_end; ++i)
    x_hull.upper_bound_assign(i->pointset());

  // Compute the poly-hull of `y'.
  PSET y_hull(y.space_dim, EMPTY);
  for (const_iterator i = y.begin(), y_end = y.end(); i != y_end; ++i)
    y_hull.upper_bound_assign(i->pointset());
  // Compute the certificate for `y_hull'.
  const Cert y_hull_cert(y_hull);

  // If the hull is stabilizing, do nothing.
  int hull_stabilization = y_hull_cert.compare(x_hull);
  if (hull_stabilization == 1)
    return;

  // Multiset ordering is only useful when `y' is not a singleton.
  const bool y_is_not_a_singleton = y.size() > 1;

  // The multiset certificate for `y':
  // we want to be lazy about its computation.
  typedef std::map<Cert, size_type, typename Cert::Compare> Cert_Multiset;
  Cert_Multiset y_cert_ms;
  bool y_cert_ms_computed = false;

  if (hull_stabilization == 0 && y_is_not_a_singleton) {
    // Collect the multiset certificate for `y'.
    y.collect_certificates(y_cert_ms);
    y_cert_ms_computed = true;
    // If multiset ordering is stabilizing, do nothing.
    if (x.is_cert_multiset_stabilizing(y_cert_ms))
      return;
  }

  // Second widening technique: try the BGP99 powerset heuristics.
  Pointset_Powerset<PSET> bgp99_heuristics = x;
  bgp99_heuristics.BGP99_heuristics_assign(y, widen_fun);

  // Compute the poly-hull of `bgp99_heuristics'.
  PSET bgp99_heuristics_hull(x.space_dim, EMPTY);
  for (const_iterator i = bgp99_heuristics.begin(),
         b_h_end = bgp99_heuristics.end(); i != b_h_end; ++i)
    bgp99_heuristics_hull.upper_bound_assign(i->pointset());

  // Check for stabilization and, if successful,
  // commit to the result of the extrapolation.
  hull_stabilization = y_hull_cert.compare(bgp99_heuristics_hull);
  if (hull_stabilization == 1) {
    // The poly-hull is stabilizing.
    swap(x, bgp99_heuristics);
    return;
  }
  else if (hull_stabilization == 0 && y_is_not_a_singleton) {
    // If not already done, compute multiset certificate for `y'.
    if (!y_cert_ms_computed) {
      y.collect_certificates(y_cert_ms);
      y_cert_ms_computed = true;
    }
    if (bgp99_heuristics.is_cert_multiset_stabilizing(y_cert_ms)) {
      swap(x, bgp99_heuristics);
      return;
    }
    // Third widening technique: pairwise-reduction on `bgp99_heuristics'.
    // Note that pairwise-reduction does not affect the computation
    // of the poly-hulls, so that we only have to check the multiset
    // certificate relation.
    Pointset_Powerset<PSET> reduced_bgp99_heuristics(bgp99_heuristics);
    reduced_bgp99_heuristics.pairwise_reduce();
    if (reduced_bgp99_heuristics.is_cert_multiset_stabilizing(y_cert_ms)) {
      swap(x, reduced_bgp99_heuristics);
      return;
    }
  }

  // Fourth widening technique: this is applicable only when
  // `y_hull' is a proper subset of `bgp99_heuristics_hull'.
  if (bgp99_heuristics_hull.strictly_contains(y_hull)) {
    // Compute (y_hull \widen bgp99_heuristics_hull).
    PSET ph = bgp99_heuristics_hull;
    widen_fun(ph, y_hull);
    // Compute the difference between `ph' and `bgp99_heuristics_hull'.
    ph.difference_assign(bgp99_heuristics_hull);
    x.add_disjunct(ph);
    return;
  }

  // Fall back to the computation of the poly-hull.
  Pointset_Powerset<PSET> x_hull_singleton(x.space_dim, EMPTY);
  x_hull_singleton.add_disjunct(x_hull);
  swap(x, x_hull_singleton);
}

template <typename PSET>
void
Pointset_Powerset<PSET>::ascii_dump(std::ostream& s) const {
  const Pointset_Powerset& x = *this;
  s << "size " << x.size()
    << "\nspace_dim " << x.space_dim
    << "\n";
  for (const_iterator xi = x.begin(), x_end = x.end(); xi != x_end; ++xi)
    xi->pointset().ascii_dump(s);
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS(PSET, Pointset_Powerset<PSET>)

template <typename PSET>
bool
Pointset_Powerset<PSET>::ascii_load(std::istream& s) {
  Pointset_Powerset& x = *this;
  std::string str;

  if (!(s >> str) || str != "size")
    return false;

  size_type sz;

  if (!(s >> sz))
    return false;

  if (!(s >> str) || str != "space_dim")
    return false;

  if (!(s >> x.space_dim))
    return false;

  Pointset_Powerset new_x(x.space_dim, EMPTY);
  while (sz-- > 0) {
    PSET ph;
    if (!ph.ascii_load(s))
      return false;
    new_x.add_disjunct(ph);
  }
  swap(x, new_x);

  // Check invariants.
  PPL_ASSERT_HEAVY(x.OK());
  return true;
}

template <typename PSET>
bool
Pointset_Powerset<PSET>::OK() const {
  const Pointset_Powerset& x = *this;
  for (const_iterator xi = x.begin(), x_end = x.end(); xi != x_end; ++xi) {
    const PSET& pi = xi->pointset();
    if (pi.space_dimension() != x.space_dim) {
#ifndef NDEBUG
      std::cerr << "Space dimension mismatch: is " << pi.space_dimension()
                << " in an element of the sequence,\nshould be "
                << x.space_dim << "."
                << std::endl;
#endif
      return false;
    }
  }
  return x.Base::OK();
}

namespace Implementation {

namespace Pointset_Powersets {

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
//! Partitions polyhedron \p pset according to constraint \p c.
/*! \relates Parma_Polyhedra_Library::Pointset_Powerset
  On exit, the intersection of \p pset and constraint \p c is stored
  in \p pset, whereas the intersection of \p pset with the negation of \p c
  is added as a new disjunct of the powerset \p r.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename PSET>
void
linear_partition_aux(const Constraint& c,
                     PSET& pset,
                     Pointset_Powerset<NNC_Polyhedron>& r) {
  const Linear_Expression le(c.expression());
  const Constraint& neg_c = c.is_strict_inequality() ? (le <= 0) : (le < 0);
  NNC_Polyhedron nnc_ph_pset(pset);
  nnc_ph_pset.add_constraint(neg_c);
  if (!nnc_ph_pset.is_empty())
    r.add_disjunct(nnc_ph_pset);
  pset.add_constraint(c);
}

} // namespace Pointset_Powersets

} // namespace Implementation


/*! \relates Pointset_Powerset */
template <typename PSET>
std::pair<PSET, Pointset_Powerset<NNC_Polyhedron> >
linear_partition(const PSET& p, const PSET& q) {
  using Implementation::Pointset_Powersets::linear_partition_aux;

  Pointset_Powerset<NNC_Polyhedron> r(p.space_dimension(), EMPTY);
  PSET pset = q;
  const Constraint_System& p_constraints = p.constraints();
  for (Constraint_System::const_iterator i = p_constraints.begin(),
         p_constraints_end = p_constraints.end();
       i != p_constraints_end;
       ++i) {
    const Constraint& c = *i;
    if (c.is_equality()) {
      const Linear_Expression le(c.expression());
      linear_partition_aux(le <= 0, pset, r);
      linear_partition_aux(le >= 0, pset, r);
    }
    else
      linear_partition_aux(c, pset, r);
  }
  return std::make_pair(pset, r);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Pointset_Powerset_templates_hh)
