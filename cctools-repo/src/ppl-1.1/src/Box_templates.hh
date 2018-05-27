/* Box class implementation: non-inline template functions.
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

#ifndef PPL_Box_templates_hh
#define PPL_Box_templates_hh 1

#include "Variables_Set_defs.hh"
#include "Constraint_System_defs.hh"
#include "Constraint_System_inlines.hh"
#include "Generator_System_defs.hh"
#include "Generator_System_inlines.hh"
#include "Poly_Con_Relation_defs.hh"
#include "Poly_Gen_Relation_defs.hh"
#include "Polyhedron_defs.hh"
#include "Grid_defs.hh"
#include "Interval_defs.hh"
#include "Linear_Form_defs.hh"
#include "BD_Shape_defs.hh"
#include "Octagonal_Shape_defs.hh"
#include "MIP_Problem_defs.hh"
#include "Rational_Interval.hh"
#include <vector>
#include <map>
#include <iostream>

namespace Parma_Polyhedra_Library {

template <typename ITV>
inline
Box<ITV>::Box(dimension_type num_dimensions, Degenerate_Element kind)
  : seq(check_space_dimension_overflow(num_dimensions,
                                       max_space_dimension(),
                                       "PPL::Box::",
                                       "Box(n, k)",
                                       "n exceeds the maximum "
                                       "allowed space dimension")),
    status() {
  // In a box that is marked empty the intervals are completely
  // meaningless: we exploit this by avoiding their initialization.
  if (kind == UNIVERSE) {
    for (dimension_type i = num_dimensions; i-- > 0; )
      seq[i].assign(UNIVERSE);
    set_empty_up_to_date();
  }
  else
    set_empty();
  PPL_ASSERT(OK());
}

template <typename ITV>
inline
Box<ITV>::Box(const Constraint_System& cs)
  : seq(check_space_dimension_overflow(cs.space_dimension(),
                                       max_space_dimension(),
                                       "PPL::Box::",
                                       "Box(cs)",
                                       "cs exceeds the maximum "
                                       "allowed space dimension")),
    status() {
  // FIXME: check whether we can avoid the double initialization.
  for (dimension_type i = cs.space_dimension(); i-- > 0; )
    seq[i].assign(UNIVERSE);
  add_constraints_no_check(cs);
}

template <typename ITV>
inline
Box<ITV>::Box(const Congruence_System& cgs)
  : seq(check_space_dimension_overflow(cgs.space_dimension(),
                                       max_space_dimension(),
                                       "PPL::Box::",
                                       "Box(cgs)",
                                       "cgs exceeds the maximum "
                                       "allowed space dimension")),
    status() {
  // FIXME: check whether we can avoid the double initialization.
  for (dimension_type i = cgs.space_dimension(); i-- > 0; )
    seq[i].assign(UNIVERSE);
  add_congruences_no_check(cgs);
}

template <typename ITV>
template <typename Other_ITV>
inline
Box<ITV>::Box(const Box<Other_ITV>& y, Complexity_Class)
  : seq(y.space_dimension()),
    // FIXME: why the following does not work?
    // status(y.status) {
    status() {
  // FIXME: remove when the above is fixed.
  if (y.marked_empty())
    set_empty();

  if (!y.marked_empty())
    for (dimension_type k = y.space_dimension(); k-- > 0; )
      seq[k].assign(y.seq[k]);
  PPL_ASSERT(OK());
}

template <typename ITV>
Box<ITV>::Box(const Generator_System& gs)
  : seq(check_space_dimension_overflow(gs.space_dimension(),
                                       max_space_dimension(),
                                       "PPL::Box::",
                                       "Box(gs)",
                                       "gs exceeds the maximum "
                                       "allowed space dimension")),
    status() {
  const Generator_System::const_iterator gs_begin = gs.begin();
  const Generator_System::const_iterator gs_end = gs.end();
  if (gs_begin == gs_end) {
    // An empty generator system defines the empty box.
    set_empty();
    return;
  }

  // The empty flag will be meaningful, whatever happens from now on.
  set_empty_up_to_date();

  const dimension_type space_dim = space_dimension();
  PPL_DIRTY_TEMP(mpq_class, q);
  bool point_seen = false;
  // Going through all the points.
  for (Generator_System::const_iterator
         gs_i = gs_begin; gs_i != gs_end; ++gs_i) {
    const Generator& g = *gs_i;
    if (g.is_point()) {
      const Coefficient& d = g.divisor();
      if (point_seen) {
        // This is not the first point: `seq' already contains valid values.
        // TODO: If the variables in the expression that have coefficient 0
        // have no effect on seq[i], this loop can be optimized using
        // Generator::expr_type::const_iterator.
        for (dimension_type i = space_dim; i-- > 0; ) {
          assign_r(q.get_num(), g.coefficient(Variable(i)), ROUND_NOT_NEEDED);
          assign_r(q.get_den(), d, ROUND_NOT_NEEDED);
          q.canonicalize();
          PPL_DIRTY_TEMP(ITV, iq);
          iq.build(i_constraint(EQUAL, q));
          seq[i].join_assign(iq);
        }
      }
      else {
        // This is the first point seen: initialize `seq'.
        point_seen = true;
        // TODO: If the variables in the expression that have coefficient 0
        // have no effect on seq[i], this loop can be optimized using
        // Generator::expr_type::const_iterator.
        for (dimension_type i = space_dim; i-- > 0; ) {
          assign_r(q.get_num(), g.coefficient(Variable(i)), ROUND_NOT_NEEDED);
          assign_r(q.get_den(), d, ROUND_NOT_NEEDED);
          q.canonicalize();
          seq[i].build(i_constraint(EQUAL, q));
        }
      }
    }
  }

  if (!point_seen)
    // The generator system is not empty, but contains no points.
    throw std::invalid_argument("PPL::Box<ITV>::Box(gs):\n"
                                "the non-empty generator system gs "
                                "contains no points.");

  // Going through all the lines, rays and closure points.
  for (Generator_System::const_iterator gs_i = gs_begin;
       gs_i != gs_end; ++gs_i) {
    const Generator& g = *gs_i;
    switch (g.type()) {
    case Generator::LINE:
      for (Generator::expr_type::const_iterator i = g.expression().begin(),
              i_end = g.expression().end();
              i != i_end; ++i)
          seq[i.variable().id()].assign(UNIVERSE);
      break;
    case Generator::RAY:
      for (Generator::expr_type::const_iterator i = g.expression().begin(),
              i_end = g.expression().end();
              i != i_end; ++i)
        switch (sgn(*i)) {
        case 1:
          seq[i.variable().id()].upper_extend();
          break;
        case -1:
          seq[i.variable().id()].lower_extend();
          break;
        default:
          PPL_UNREACHABLE;
          break;
        }
      break;
    case Generator::CLOSURE_POINT:
      {
        const Coefficient& d = g.divisor();
        // TODO: If the variables in the expression that have coefficient 0
        // have no effect on seq[i], this loop can be optimized using
        // Generator::expr_type::const_iterator.
        for (dimension_type i = space_dim; i-- > 0; ) {
          assign_r(q.get_num(), g.coefficient(Variable(i)), ROUND_NOT_NEEDED);
          assign_r(q.get_den(), d, ROUND_NOT_NEEDED);
          q.canonicalize();
          ITV& seq_i = seq[i];
          seq_i.lower_extend(i_constraint(GREATER_THAN, q));
          seq_i.upper_extend(i_constraint(LESS_THAN, q));
        }
      }
      break;
    default:
      // Points already dealt with.
      break;
    }
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
template <typename T>
Box<ITV>::Box(const BD_Shape<T>& bds, Complexity_Class)
  : seq(check_space_dimension_overflow(bds.space_dimension(),
                                       max_space_dimension(),
                                       "PPL::Box::",
                                       "Box(bds)",
                                       "bds exceeds the maximum "
                                       "allowed space dimension")),
    status() {
  // Expose all the interval constraints.
  bds.shortest_path_closure_assign();
  if (bds.marked_empty()) {
    set_empty();
    PPL_ASSERT(OK());
    return;
  }

  // The empty flag will be meaningful, whatever happens from now on.
  set_empty_up_to_date();

  const dimension_type space_dim = space_dimension();
  if (space_dim == 0) {
    PPL_ASSERT(OK());
    return;
  }

  typedef typename BD_Shape<T>::coefficient_type Coeff;
  PPL_DIRTY_TEMP(Coeff, tmp);
  const DB_Row<Coeff>& dbm_0 = bds.dbm[0];
  for (dimension_type i = space_dim; i-- > 0; ) {
    I_Constraint<Coeff> lower;
    I_Constraint<Coeff> upper;
    ITV& seq_i = seq[i];

    // Set the upper bound.
    const Coeff& u = dbm_0[i+1];
    if (!is_plus_infinity(u))
      upper.set(LESS_OR_EQUAL, u, true);

    // Set the lower bound.
    const Coeff& negated_l = bds.dbm[i+1][0];
    if (!is_plus_infinity(negated_l)) {
      neg_assign_r(tmp, negated_l, ROUND_DOWN);
      lower.set(GREATER_OR_EQUAL, tmp);
    }

    seq_i.build(lower, upper);
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
template <typename T>
Box<ITV>::Box(const Octagonal_Shape<T>& oct, Complexity_Class)
  : seq(check_space_dimension_overflow(oct.space_dimension(),
                                       max_space_dimension(),
                                       "PPL::Box::",
                                       "Box(oct)",
                                       "oct exceeds the maximum "
                                       "allowed space dimension")),
    status() {
  // Expose all the interval constraints.
  oct.strong_closure_assign();
  if (oct.marked_empty()) {
    set_empty();
    return;
  }

  // The empty flag will be meaningful, whatever happens from now on.
  set_empty_up_to_date();

  const dimension_type space_dim = space_dimension();
  if (space_dim == 0)
    return;

  PPL_DIRTY_TEMP(mpq_class, lower_bound);
  PPL_DIRTY_TEMP(mpq_class, upper_bound);
  for (dimension_type i = space_dim; i-- > 0; ) {
    typedef typename Octagonal_Shape<T>::coefficient_type Coeff;
    I_Constraint<mpq_class> lower;
    I_Constraint<mpq_class> upper;
    ITV& seq_i = seq[i];
    const dimension_type ii = 2*i;
    const dimension_type cii = ii + 1;

    // Set the upper bound.
    const Coeff& twice_ub = oct.matrix[cii][ii];
    if (!is_plus_infinity(twice_ub)) {
      assign_r(upper_bound, twice_ub, ROUND_NOT_NEEDED);
      div_2exp_assign_r(upper_bound, upper_bound, 1, ROUND_NOT_NEEDED);
      upper.set(LESS_OR_EQUAL, upper_bound);
    }

    // Set the lower bound.
    const Coeff& twice_lb = oct.matrix[ii][cii];
    if (!is_plus_infinity(twice_lb)) {
      assign_r(lower_bound, twice_lb, ROUND_NOT_NEEDED);
      neg_assign_r(lower_bound, lower_bound, ROUND_NOT_NEEDED);
      div_2exp_assign_r(lower_bound, lower_bound, 1, ROUND_NOT_NEEDED);
      lower.set(GREATER_OR_EQUAL, lower_bound);
    }
    seq_i.build(lower, upper);
  }
}

template <typename ITV>
Box<ITV>::Box(const Polyhedron& ph, Complexity_Class complexity)
  : seq(check_space_dimension_overflow(ph.space_dimension(),
                                       max_space_dimension(),
                                       "PPL::Box::",
                                       "Box(ph)",
                                       "ph exceeds the maximum "
                                       "allowed space dimension")),
    status() {
  // The empty flag will be meaningful, whatever happens from now on.
  set_empty_up_to_date();

  // We do not need to bother about `complexity' if:
  // a) the polyhedron is already marked empty; or ...
  if (ph.marked_empty()) {
    set_empty();
    return;
  }

  // b) the polyhedron is zero-dimensional; or ...
  const dimension_type space_dim = ph.space_dimension();
  if (space_dim == 0)
    return;

  // c) the polyhedron is already described by a generator system.
  if (ph.generators_are_up_to_date() && !ph.has_pending_constraints()) {
    Box tmp(ph.generators());
    m_swap(tmp);
    return;
  }

  // Here generators are not up-to-date or there are pending constraints.
  PPL_ASSERT(ph.constraints_are_up_to_date());

  if (complexity == POLYNOMIAL_COMPLEXITY) {
    // FIXME: is there a way to avoid this initialization?
    for (dimension_type i = space_dim; i-- > 0; )
      seq[i].assign(UNIVERSE);
    // Get a simplified version of the constraints.
    const Constraint_System cs = ph.simplified_constraints();
    // Propagate easy-to-find bounds from the constraints,
    // allowing for a limited number of iterations.
    // FIXME: 20 is just a wild guess.
    const dimension_type max_iterations = 20;
    propagate_constraints_no_check(cs, max_iterations);
  }
  else if (complexity == SIMPLEX_COMPLEXITY) {
    MIP_Problem lp(space_dim);
    const Constraint_System& ph_cs = ph.constraints();
    if (!ph_cs.has_strict_inequalities())
      lp.add_constraints(ph_cs);
    else
      // Adding to `lp' a topologically closed version of `ph_cs'.
      for (Constraint_System::const_iterator i = ph_cs.begin(),
             ph_cs_end = ph_cs.end(); i != ph_cs_end; ++i) {
        const Constraint& c = *i;
        if (c.is_strict_inequality()) {
          const Linear_Expression expr(c.expression());
          lp.add_constraint(expr >= 0);
        }
        else
          lp.add_constraint(c);
      }
    // Check for unsatisfiability.
    if (!lp.is_satisfiable()) {
      set_empty();
      return;
    }
    // Get all the bounds for the space dimensions.
    Generator g(point());
    PPL_DIRTY_TEMP(mpq_class, lower_bound);
    PPL_DIRTY_TEMP(mpq_class, upper_bound);
    PPL_DIRTY_TEMP(Coefficient, bound_numer);
    PPL_DIRTY_TEMP(Coefficient, bound_denom);
    for (dimension_type i = space_dim; i-- > 0; ) {
      I_Constraint<mpq_class> lower;
      I_Constraint<mpq_class> upper;
      ITV& seq_i = seq[i];
      lp.set_objective_function(Variable(i));
      // Evaluate upper bound.
      lp.set_optimization_mode(MAXIMIZATION);
      if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
        g = lp.optimizing_point();
        lp.evaluate_objective_function(g, bound_numer, bound_denom);
        assign_r(upper_bound.get_num(), bound_numer, ROUND_NOT_NEEDED);
        assign_r(upper_bound.get_den(), bound_denom, ROUND_NOT_NEEDED);
        PPL_ASSERT(is_canonical(upper_bound));
        upper.set(LESS_OR_EQUAL, upper_bound);
      }
      // Evaluate optimal lower bound.
      lp.set_optimization_mode(MINIMIZATION);
      if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
        g = lp.optimizing_point();
        lp.evaluate_objective_function(g, bound_numer, bound_denom);
        assign_r(lower_bound.get_num(), bound_numer, ROUND_NOT_NEEDED);
        assign_r(lower_bound.get_den(), bound_denom, ROUND_NOT_NEEDED);
        PPL_ASSERT(is_canonical(lower_bound));
        lower.set(GREATER_OR_EQUAL, lower_bound);
      }
      seq_i.build(lower, upper);
    }
  }
  else {
    PPL_ASSERT(complexity == ANY_COMPLEXITY);
    if (ph.is_empty())
      set_empty();
    else {
      Box tmp(ph.generators());
      m_swap(tmp);
    }
  }
}

template <typename ITV>
Box<ITV>::Box(const Grid& gr, Complexity_Class)
  : seq(check_space_dimension_overflow(gr.space_dimension(),
                                       max_space_dimension(),
                                       "PPL::Box::",
                                       "Box(gr)",
                                       "gr exceeds the maximum "
                                       "allowed space dimension")),
    status() {

  if (gr.marked_empty()) {
    set_empty();
    return;
  }

  // The empty flag will be meaningful, whatever happens from now on.
  set_empty_up_to_date();

  const dimension_type space_dim = gr.space_dimension();

  if (space_dim == 0)
    return;

  if (!gr.generators_are_up_to_date() && !gr.update_generators()) {
    // Updating found the grid empty.
    set_empty();
    return;
  }

  PPL_ASSERT(!gr.gen_sys.empty());

  // For each dimension that is bounded by the grid, set both bounds
  // of the interval to the value of the associated coefficient in a
  // generator point.
  PPL_DIRTY_TEMP(mpq_class, bound);
  PPL_DIRTY_TEMP(Coefficient, bound_numer);
  PPL_DIRTY_TEMP(Coefficient, bound_denom);
  for (dimension_type i = space_dim; i-- > 0; ) {
    ITV& seq_i = seq[i];
    Variable var(i);
    bool max;
    if (gr.maximize(var, bound_numer, bound_denom, max)) {
      assign_r(bound.get_num(), bound_numer, ROUND_NOT_NEEDED);
      assign_r(bound.get_den(), bound_denom, ROUND_NOT_NEEDED);
      bound.canonicalize();
      seq_i.build(i_constraint(EQUAL, bound));
    }
    else
      seq_i.assign(UNIVERSE);
  }
}

template <typename ITV>
template <typename D1, typename D2, typename R>
Box<ITV>::Box(const Partially_Reduced_Product<D1, D2, R>& dp,
              Complexity_Class complexity)
  : seq(), status() {
  check_space_dimension_overflow(dp.space_dimension(),
                                 max_space_dimension(),
                                 "PPL::Box::",
                                 "Box(dp)",
                                 "dp exceeds the maximum "
                                 "allowed space dimension");
  Box tmp1(dp.domain1(), complexity);
  Box tmp2(dp.domain2(), complexity);
  tmp1.intersection_assign(tmp2);
  m_swap(tmp1);
}

template <typename ITV>
inline void
Box<ITV>::add_space_dimensions_and_embed(const dimension_type m) {
  // Adding no dimensions is a no-op.
  if (m == 0)
    return;
  check_space_dimension_overflow(m, max_space_dimension() - space_dimension(),
                                 "PPL::Box::",
                                 "add_space_dimensions_and_embed(m)",
                                 "adding m new space dimensions exceeds "
                                 "the maximum allowed space dimension");
  // To embed an n-dimension space box in a (n+m)-dimension space,
  // we just add `m' new universe elements to the sequence.
  seq.insert(seq.end(), m, ITV(UNIVERSE));
  PPL_ASSERT(OK());
}

template <typename ITV>
inline void
Box<ITV>::add_space_dimensions_and_project(const dimension_type m) {
  // Adding no dimensions is a no-op.
  if (m == 0)
    return;
  check_space_dimension_overflow(m, max_space_dimension() - space_dimension(),
                                 "PPL::Box::",
                                 "add_space_dimensions_and_project(m)",
                                 "adding m new space dimensions exceeds "
                                 "the maximum allowed space dimension");
  // Add `m' new zero elements to the sequence.
  seq.insert(seq.end(), m, ITV(0));
  PPL_ASSERT(OK());
}

template <typename ITV>
bool
operator==(const Box<ITV>& x, const Box<ITV>& y) {
  const dimension_type x_space_dim = x.space_dimension();
  if (x_space_dim != y.space_dimension())
    return false;

  if (x.is_empty())
    return y.is_empty();

  if (y.is_empty())
    return x.is_empty();

  for (dimension_type k = x_space_dim; k-- > 0; )
    if (x.seq[k] != y.seq[k])
      return false;
  return true;
}

template <typename ITV>
bool
Box<ITV>::bounds(const Linear_Expression& expr, const bool from_above) const {
  // `expr' should be dimension-compatible with `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  const dimension_type space_dim = space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible((from_above
                                  ? "bounds_from_above(e)"
                                  : "bounds_from_below(e)"), "e", expr);
  // A zero-dimensional or empty Box bounds everything.
  if (space_dim == 0 || is_empty())
    return true;

  const int from_above_sign = from_above ? 1 : -1;
  // TODO: This loop can be optimized more, if needed, exploiting the
  // (possible) sparseness of expr.
  for (Linear_Expression::const_iterator i = expr.begin(),
          i_end = expr.end(); i != i_end; ++i) {
    const Variable v = i.variable();
    switch (sgn(*i) * from_above_sign) {
    case 1:
      if (seq[v.id()].upper_is_boundary_infinity())
        return false;
      break;
    case 0:
      PPL_UNREACHABLE;
      break;
    case -1:
      if (seq[v.id()].lower_is_boundary_infinity())
        return false;
      break;
    }
  }
  return true;
}

template <typename ITV>
Poly_Con_Relation
interval_relation(const ITV& i,
                  const Constraint::Type constraint_type,
                  Coefficient_traits::const_reference numer,
                  Coefficient_traits::const_reference denom) {

  if (i.is_universe())
    return Poly_Con_Relation::strictly_intersects();

  PPL_DIRTY_TEMP(mpq_class, bound);
  assign_r(bound.get_num(), numer, ROUND_NOT_NEEDED);
  assign_r(bound.get_den(), denom, ROUND_NOT_NEEDED);
  bound.canonicalize();
  neg_assign_r(bound, bound, ROUND_NOT_NEEDED);
  const bool is_lower_bound = (denom > 0);

  PPL_DIRTY_TEMP(mpq_class, bound_diff);
  if (constraint_type == Constraint::EQUALITY) {
    if (i.lower_is_boundary_infinity()) {
      PPL_ASSERT(!i.upper_is_boundary_infinity());
      assign_r(bound_diff, i.upper(), ROUND_NOT_NEEDED);
      sub_assign_r(bound_diff, bound_diff, bound, ROUND_NOT_NEEDED);
      switch (sgn(bound_diff)) {
      case 1:
        return Poly_Con_Relation::strictly_intersects();
      case 0:
        return i.upper_is_open()
          ? Poly_Con_Relation::is_disjoint()
          : Poly_Con_Relation::strictly_intersects();
      case -1:
        return Poly_Con_Relation::is_disjoint();
      }
    }
    else {
      assign_r(bound_diff, i.lower(), ROUND_NOT_NEEDED);
      sub_assign_r(bound_diff, bound_diff, bound, ROUND_NOT_NEEDED);
      switch (sgn(bound_diff)) {
      case 1:
        return Poly_Con_Relation::is_disjoint();
      case 0:
        if (i.lower_is_open())
          return Poly_Con_Relation::is_disjoint();
        if (i.is_singleton())
          return Poly_Con_Relation::is_included()
            && Poly_Con_Relation::saturates();
        return Poly_Con_Relation::strictly_intersects();
      case -1:
        if (i.upper_is_boundary_infinity())
          return Poly_Con_Relation::strictly_intersects();
        else {
          assign_r(bound_diff, i.upper(), ROUND_NOT_NEEDED);
          sub_assign_r(bound_diff, bound_diff, bound, ROUND_NOT_NEEDED);
          switch (sgn(bound_diff)) {
          case 1:
            return Poly_Con_Relation::strictly_intersects();
          case 0:
            if (i.upper_is_open())
              return Poly_Con_Relation::is_disjoint();
            else
              return Poly_Con_Relation::strictly_intersects();
          case -1:
            return Poly_Con_Relation::is_disjoint();
          }
        }
      }
    }
  }

  PPL_ASSERT(constraint_type != Constraint::EQUALITY);
  if (is_lower_bound) {
    if (i.lower_is_boundary_infinity()) {
      PPL_ASSERT(!i.upper_is_boundary_infinity());
      assign_r(bound_diff, i.upper(), ROUND_NOT_NEEDED);
      sub_assign_r(bound_diff, bound_diff, bound, ROUND_NOT_NEEDED);
      switch (sgn(bound_diff)) {
      case 1:
        return Poly_Con_Relation::strictly_intersects();
      case 0:
        if (constraint_type == Constraint::STRICT_INEQUALITY
            || i.upper_is_open())
          return Poly_Con_Relation::is_disjoint();
        else
          return Poly_Con_Relation::strictly_intersects();
      case -1:
        return Poly_Con_Relation::is_disjoint();
      }
    }
    else {
      assign_r(bound_diff, i.lower(), ROUND_NOT_NEEDED);
      sub_assign_r(bound_diff, bound_diff, bound, ROUND_NOT_NEEDED);
      switch (sgn(bound_diff)) {
      case 1:
        return Poly_Con_Relation::is_included();
      case 0:
        if (constraint_type == Constraint::NONSTRICT_INEQUALITY
            || i.lower_is_open()) {
          Poly_Con_Relation result = Poly_Con_Relation::is_included();
          if (i.is_singleton())
            result = result && Poly_Con_Relation::saturates();
          return result;
        }
        else {
          PPL_ASSERT(constraint_type == Constraint::STRICT_INEQUALITY
                 && !i.lower_is_open());
          if (i.is_singleton())
            return Poly_Con_Relation::is_disjoint()
              && Poly_Con_Relation::saturates();
          else
            return Poly_Con_Relation::strictly_intersects();
        }
      case -1:
        if (i.upper_is_boundary_infinity())
          return Poly_Con_Relation::strictly_intersects();
        else {
          assign_r(bound_diff, i.upper(), ROUND_NOT_NEEDED);
          sub_assign_r(bound_diff, bound_diff, bound, ROUND_NOT_NEEDED);
          switch (sgn(bound_diff)) {
          case 1:
            return Poly_Con_Relation::strictly_intersects();
          case 0:
            if (constraint_type == Constraint::STRICT_INEQUALITY
                || i.upper_is_open())
              return Poly_Con_Relation::is_disjoint();
            else
              return Poly_Con_Relation::strictly_intersects();
          case -1:
            return Poly_Con_Relation::is_disjoint();
          }
        }
      }
    }
  }
  else {
    // `c' is an upper bound.
    if (i.upper_is_boundary_infinity())
      return Poly_Con_Relation::strictly_intersects();
    else {
      assign_r(bound_diff, i.upper(), ROUND_NOT_NEEDED);
      sub_assign_r(bound_diff, bound_diff, bound, ROUND_NOT_NEEDED);
      switch (sgn(bound_diff)) {
      case -1:
        return Poly_Con_Relation::is_included();
      case 0:
        if (constraint_type == Constraint::NONSTRICT_INEQUALITY
            || i.upper_is_open()) {
          Poly_Con_Relation result = Poly_Con_Relation::is_included();
          if (i.is_singleton())
            result = result && Poly_Con_Relation::saturates();
          return result;
        }
        else {
          PPL_ASSERT(constraint_type == Constraint::STRICT_INEQUALITY
                 && !i.upper_is_open());
          if (i.is_singleton())
            return Poly_Con_Relation::is_disjoint()
              && Poly_Con_Relation::saturates();
          else
            return Poly_Con_Relation::strictly_intersects();
        }
      case 1:
        if (i.lower_is_boundary_infinity())
          return Poly_Con_Relation::strictly_intersects();
        else {
          assign_r(bound_diff, i.lower(), ROUND_NOT_NEEDED);
          sub_assign_r(bound_diff, bound_diff, bound, ROUND_NOT_NEEDED);
          switch (sgn(bound_diff)) {
          case -1:
            return Poly_Con_Relation::strictly_intersects();
          case 0:
            if (constraint_type == Constraint::STRICT_INEQUALITY
                || i.lower_is_open())
              return Poly_Con_Relation::is_disjoint();
            else
              return Poly_Con_Relation::strictly_intersects();
          case 1:
            return Poly_Con_Relation::is_disjoint();
          }
        }
      }
    }
  }

  // Quiet a compiler warning: this program point is unreachable.
  PPL_UNREACHABLE;
  return Poly_Con_Relation::nothing();
}

template <typename ITV>
Poly_Con_Relation
Box<ITV>::relation_with(const Congruence& cg) const {
  const dimension_type cg_space_dim = cg.space_dimension();
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (cg_space_dim > space_dim)
    throw_dimension_incompatible("relation_with(cg)", cg);

  if (is_empty())
    return Poly_Con_Relation::saturates()
      && Poly_Con_Relation::is_included()
      && Poly_Con_Relation::is_disjoint();

  if (space_dim == 0) {
    if (cg.is_inconsistent())
      return Poly_Con_Relation::is_disjoint();
    else
      return Poly_Con_Relation::saturates()
        && Poly_Con_Relation::is_included();
  }

  if (cg.is_equality()) {
    const Constraint c(cg);
    return relation_with(c);
  }

  PPL_DIRTY_TEMP(Rational_Interval, r);
  PPL_DIRTY_TEMP(Rational_Interval, t);
  PPL_DIRTY_TEMP(mpq_class, m);
  r = 0;
  for (Congruence::expr_type::const_iterator i = cg.expression().begin(),
      i_end = cg.expression().end(); i != i_end; ++i) {
    const Coefficient& cg_i = *i;
    const Variable v = i.variable();
    assign_r(m, cg_i, ROUND_NOT_NEEDED);
    // FIXME: an add_mul_assign() method would come handy here.
    t.build(seq[v.id()].lower_constraint(), seq[v.id()].upper_constraint());
    t *= m;
    r += t;
  }

  if (r.lower_is_boundary_infinity() || r.upper_is_boundary_infinity())
    return Poly_Con_Relation::strictly_intersects();


  // Find the value that satisfies the congruence and is
  // nearest to the lower bound such that the point lies on or above it.

  PPL_DIRTY_TEMP_COEFFICIENT(lower);
  PPL_DIRTY_TEMP_COEFFICIENT(mod);
  PPL_DIRTY_TEMP_COEFFICIENT(v);
  mod = cg.modulus();
  v = cg.inhomogeneous_term() % mod;
  assign_r(lower, r.lower(), ROUND_DOWN);
  v -= ((lower / mod) * mod);
  if (v + lower > 0)
    v -= mod;
  return interval_relation(r, Constraint::EQUALITY, v);
}

template <typename ITV>
Poly_Con_Relation
Box<ITV>::relation_with(const Constraint& c) const {
  const dimension_type c_space_dim = c.space_dimension();
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (c_space_dim > space_dim)
    throw_dimension_incompatible("relation_with(c)", c);

  if (is_empty())
    return Poly_Con_Relation::saturates()
      && Poly_Con_Relation::is_included()
      && Poly_Con_Relation::is_disjoint();

  if (space_dim == 0) {
    if ((c.is_equality() && c.inhomogeneous_term() != 0)
        || (c.is_inequality() && c.inhomogeneous_term() < 0))
      return Poly_Con_Relation::is_disjoint();
    else if (c.is_strict_inequality() && c.inhomogeneous_term() == 0)
      // The constraint 0 > 0 implicitly defines the hyperplane 0 = 0;
      // thus, the zero-dimensional point also saturates it.
      return Poly_Con_Relation::saturates()
        && Poly_Con_Relation::is_disjoint();
    else if (c.is_equality() || c.inhomogeneous_term() == 0)
      return Poly_Con_Relation::saturates()
        && Poly_Con_Relation::is_included();
    else
      // The zero-dimensional point saturates
      // neither the positivity constraint 1 >= 0,
      // nor the strict positivity constraint 1 > 0.
      return Poly_Con_Relation::is_included();
  }

  dimension_type c_num_vars = 0;
  dimension_type c_only_var = 0;

  if (Box_Helpers::extract_interval_constraint(c, c_num_vars, c_only_var))
    if (c_num_vars == 0)
      // c is a trivial constraint.
      switch (sgn(c.inhomogeneous_term())) {
      case -1:
        return Poly_Con_Relation::is_disjoint();
      case 0:
        if (c.is_strict_inequality())
          return Poly_Con_Relation::saturates()
            && Poly_Con_Relation::is_disjoint();
        else
          return Poly_Con_Relation::saturates()
            && Poly_Con_Relation::is_included();
      case 1:
        return Poly_Con_Relation::is_included();
      }
    else {
      // c is an interval constraint.
      return interval_relation(seq[c_only_var],
                               c.type(),
                               c.inhomogeneous_term(),
                               c.coefficient(Variable(c_only_var)));
    }
  else {
    // Deal with a non-trivial and non-interval constraint.
    PPL_DIRTY_TEMP(Rational_Interval, r);
    PPL_DIRTY_TEMP(Rational_Interval, t);
    PPL_DIRTY_TEMP(mpq_class, m);
    r = 0;
    const Constraint::expr_type& e = c.expression();
    for (Constraint::expr_type::const_iterator i = e.begin(), i_end = e.end();
          i != i_end; ++i) {
      assign_r(m, *i, ROUND_NOT_NEEDED);
      const Variable v = i.variable();
      // FIXME: an add_mul_assign() method would come handy here.
      t.build(seq[v.id()].lower_constraint(), seq[v.id()].upper_constraint());
      t *= m;
      r += t;
    }
    return interval_relation(r,
                             c.type(),
                             c.inhomogeneous_term());
  }

  // Quiet a compiler warning: this program point is unreachable.
  PPL_UNREACHABLE;
  return Poly_Con_Relation::nothing();
}

template <typename ITV>
Poly_Gen_Relation
Box<ITV>::relation_with(const Generator& g) const {
  const dimension_type space_dim = space_dimension();
  const dimension_type g_space_dim = g.space_dimension();

  // Dimension-compatibility check.
  if (space_dim < g_space_dim)
    throw_dimension_incompatible("relation_with(g)", g);

  // The empty box cannot subsume a generator.
  if (is_empty())
    return Poly_Gen_Relation::nothing();

  // A universe box in a zero-dimensional space subsumes
  // all the generators of a zero-dimensional space.
  if (space_dim == 0)
    return Poly_Gen_Relation::subsumes();

  if (g.is_line_or_ray()) {
    if (g.is_line()) {
      const Generator::expr_type& e = g.expression();
      for (Generator::expr_type::const_iterator i = e.begin(), i_end = e.end();
           i != i_end; ++i)
        if (!seq[i.variable().id()].is_universe())
          return Poly_Gen_Relation::nothing();
      return Poly_Gen_Relation::subsumes();
    }
    else {
      PPL_ASSERT(g.is_ray());
      const Generator::expr_type& e = g.expression();
      for (Generator::expr_type::const_iterator i = e.begin(), i_end = e.end();
           i != i_end; ++i) {
        const Variable v = i.variable();
        switch (sgn(*i)) {
        case 1:
          if (!seq[v.id()].upper_is_boundary_infinity())
            return Poly_Gen_Relation::nothing();
          break;
        case 0:
          PPL_UNREACHABLE;
          break;
        case -1:
          if (!seq[v.id()].lower_is_boundary_infinity())
            return Poly_Gen_Relation::nothing();
          break;
        }
      }
      return Poly_Gen_Relation::subsumes();
    }
  }

  // Here `g' is a point or closure point.
  const Coefficient& g_divisor = g.divisor();
  PPL_DIRTY_TEMP(mpq_class, g_coord);
  PPL_DIRTY_TEMP(mpq_class, bound);
  // TODO: If the variables in the expression that have coefficient 0
  // have no effect on seq[i], this loop can be optimized using
  // Generator::expr_type::const_iterator.
  for (dimension_type i = g_space_dim; i-- > 0; ) {
    const ITV& seq_i = seq[i];
    if (seq_i.is_universe())
      continue;
    assign_r(g_coord.get_num(), g.coefficient(Variable(i)), ROUND_NOT_NEEDED);
    assign_r(g_coord.get_den(), g_divisor, ROUND_NOT_NEEDED);
    g_coord.canonicalize();
    // Check lower bound.
    if (!seq_i.lower_is_boundary_infinity()) {
      assign_r(bound, seq_i.lower(), ROUND_NOT_NEEDED);
      if (g_coord <= bound) {
        if (seq_i.lower_is_open()) {
          if (g.is_point() || g_coord != bound)
            return Poly_Gen_Relation::nothing();
        }
        else if (g_coord != bound)
          return Poly_Gen_Relation::nothing();
      }
    }
    // Check upper bound.
    if (!seq_i.upper_is_boundary_infinity()) {
      assign_r(bound, seq_i.upper(), ROUND_NOT_NEEDED);
      if (g_coord >= bound) {
        if (seq_i.upper_is_open()) {
          if (g.is_point() || g_coord != bound)
            return Poly_Gen_Relation::nothing();
        }
        else if (g_coord != bound)
          return Poly_Gen_Relation::nothing();
      }
    }
  }
  return Poly_Gen_Relation::subsumes();
}


template <typename ITV>
bool
Box<ITV>::max_min(const Linear_Expression& expr,
                  const bool maximize,
                  Coefficient& ext_n, Coefficient& ext_d,
                  bool& included) const {
  // `expr' should be dimension-compatible with `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible((maximize
                                  ? "maximize(e, ...)"
                                  : "minimize(e, ...)"), "e", expr);
  // Deal with zero-dim Box first.
  if (space_dim == 0) {
    if (marked_empty())
      return false;
    else {
      ext_n = expr.inhomogeneous_term();
      ext_d = 1;
      included = true;
      return true;
    }
  }

  // For an empty Box we simply return false.
  if (is_empty())
    return false;

  PPL_DIRTY_TEMP(mpq_class, result);
  assign_r(result, expr.inhomogeneous_term(), ROUND_NOT_NEEDED);
  bool is_included = true;
  const int maximize_sign = maximize ? 1 : -1;
  PPL_DIRTY_TEMP(mpq_class, bound_i);
  PPL_DIRTY_TEMP(mpq_class, expr_i);
  for (Linear_Expression::const_iterator i = expr.begin(),
          i_end = expr.end(); i != i_end; ++i) {
    const ITV& seq_i = seq[i.variable().id()];
    assign_r(expr_i, *i, ROUND_NOT_NEEDED);
    switch (sgn(expr_i) * maximize_sign) {
    case 1:
      if (seq_i.upper_is_boundary_infinity())
        return false;
      assign_r(bound_i, seq_i.upper(), ROUND_NOT_NEEDED);
      add_mul_assign_r(result, bound_i, expr_i, ROUND_NOT_NEEDED);
      if (seq_i.upper_is_open())
        is_included = false;
      break;
    case 0:
      PPL_UNREACHABLE;
      break;
    case -1:
      if (seq_i.lower_is_boundary_infinity())
        return false;
      assign_r(bound_i, seq_i.lower(), ROUND_NOT_NEEDED);
      add_mul_assign_r(result, bound_i, expr_i, ROUND_NOT_NEEDED);
      if (seq_i.lower_is_open())
        is_included = false;
      break;
    }
  }
  // Extract output info.
  PPL_ASSERT(is_canonical(result));
  ext_n = result.get_num();
  ext_d = result.get_den();
  included = is_included;
  return true;
}

template <typename ITV>
bool
Box<ITV>::max_min(const Linear_Expression& expr,
                  const bool maximize,
                  Coefficient& ext_n, Coefficient& ext_d,
                  bool& included,
                  Generator& g) const {
  if (!max_min(expr, maximize, ext_n, ext_d, included))
    return false;

  // Compute generator `g'.
  Linear_Expression g_expr;
  PPL_DIRTY_TEMP(Coefficient, g_divisor);
  g_divisor = 1;
  const int maximize_sign = maximize ? 1 : -1;
  PPL_DIRTY_TEMP(mpq_class, g_coord);
  PPL_DIRTY_TEMP(Coefficient, numer);
  PPL_DIRTY_TEMP(Coefficient, denom);
  PPL_DIRTY_TEMP(Coefficient, lcm);
  PPL_DIRTY_TEMP(Coefficient, factor);
  // TODO: Check if the following loop can be optimized to exploit the
  // (possible) sparseness of expr.
  for (dimension_type i = space_dimension(); i-- > 0; ) {
    const ITV& seq_i = seq[i];
    switch (sgn(expr.coefficient(Variable(i))) * maximize_sign) {
    case 1:
      assign_r(g_coord, seq_i.upper(), ROUND_NOT_NEEDED);
      break;
    case 0:
      // If 0 belongs to the interval, choose it
      // (and directly proceed to the next iteration).
      // FIXME: name qualification issue.
      if (seq_i.contains(0))
        continue;
      if (!seq_i.lower_is_boundary_infinity())
        if (seq_i.lower_is_open())
          if (!seq_i.upper_is_boundary_infinity())
            if (seq_i.upper_is_open()) {
              // Bounded and open interval: compute middle point.
              assign_r(g_coord, seq_i.lower(), ROUND_NOT_NEEDED);
              PPL_DIRTY_TEMP(mpq_class, q_seq_i_upper);
              assign_r(q_seq_i_upper, seq_i.upper(), ROUND_NOT_NEEDED);
              g_coord += q_seq_i_upper;
              g_coord /= 2;
            }
            else
              // The upper bound is in the interval.
              assign_r(g_coord, seq_i.upper(), ROUND_NOT_NEEDED);
          else {
            // Lower is open, upper is unbounded.
            assign_r(g_coord, seq_i.lower(), ROUND_NOT_NEEDED);
            ++g_coord;
          }
        else
          // The lower bound is in the interval.
          assign_r(g_coord, seq_i.lower(), ROUND_NOT_NEEDED);
      else {
        // Lower is unbounded, hence upper is bounded
        // (since we know that 0 does not belong to the interval).
        PPL_ASSERT(!seq_i.upper_is_boundary_infinity());
        assign_r(g_coord, seq_i.upper(), ROUND_NOT_NEEDED);
        if (seq_i.upper_is_open())
          --g_coord;
      }
      break;
    case -1:
      assign_r(g_coord, seq_i.lower(), ROUND_NOT_NEEDED);
      break;
    }
    // Add g_coord * Variable(i) to the generator.
    assign_r(denom, g_coord.get_den(), ROUND_NOT_NEEDED);
    lcm_assign(lcm, g_divisor, denom);
    exact_div_assign(factor, lcm, g_divisor);
    g_expr *= factor;
    exact_div_assign(factor, lcm, denom);
    assign_r(numer, g_coord.get_num(), ROUND_NOT_NEEDED);
    numer *= factor;
    g_expr += numer * Variable(i);
    g_divisor = lcm;
  }
  g = Generator::point(g_expr, g_divisor);
  return true;
}

template <typename ITV>
bool
Box<ITV>::contains(const Box& y) const {
  const Box& x = *this;
  // Dimension-compatibility check.
  if (x.space_dimension() != y.space_dimension())
    x.throw_dimension_incompatible("contains(y)", y);

  // If `y' is empty, then `x' contains `y'.
  if (y.is_empty())
    return true;

  // If `x' is empty, then `x' cannot contain `y'.
  if (x.is_empty())
    return false;

  for (dimension_type k = x.seq.size(); k-- > 0; )
    // FIXME: fix this name qualification issue.
    if (!x.seq[k].contains(y.seq[k]))
      return false;
  return true;
}

template <typename ITV>
bool
Box<ITV>::is_disjoint_from(const Box& y) const {
  const Box& x = *this;
  // Dimension-compatibility check.
  if (x.space_dimension() != y.space_dimension())
    x.throw_dimension_incompatible("is_disjoint_from(y)", y);

  // If any of `x' or `y' is marked empty, then they are disjoint.
  // Note: no need to use `is_empty', as the following loop is anyway correct.
  if (x.marked_empty() || y.marked_empty())
    return true;

  for (dimension_type k = x.seq.size(); k-- > 0; )
    // FIXME: fix this name qualification issue.
    if (x.seq[k].is_disjoint_from(y.seq[k]))
      return true;
  return false;
}

template <typename ITV>
inline bool
Box<ITV>::upper_bound_assign_if_exact(const Box& y) {
  Box& x = *this;

  // Dimension-compatibility check.
  if (x.space_dimension() != y.space_dimension())
    x.throw_dimension_incompatible("upper_bound_assign_if_exact(y)", y);

  // The lub of a box with an empty box is equal to the first box.
  if (y.is_empty())
    return true;
  if (x.is_empty()) {
    x = y;
    return true;
  }

  bool x_j_does_not_contain_y_j = false;
  bool y_j_does_not_contain_x_j = false;

  for (dimension_type i = x.seq.size(); i-- > 0; ) {
    const ITV& x_seq_i = x.seq[i];
    const ITV& y_seq_i = y.seq[i];

    if (!x_seq_i.can_be_exactly_joined_to(y_seq_i))
      return false;

    // Note: the use of `y_i_does_not_contain_x_i' is needed
    // because we want to temporarily preserve the old value
    // of `y_j_does_not_contain_x_j'.
    bool y_i_does_not_contain_x_i = !y_seq_i.contains(x_seq_i);
    if (y_i_does_not_contain_x_i && x_j_does_not_contain_y_j)
      return false;
    if (!x_seq_i.contains(y_seq_i)) {
      if (y_j_does_not_contain_x_j)
        return false;
      else
        x_j_does_not_contain_y_j = true;
    }
    if (y_i_does_not_contain_x_i)
      y_j_does_not_contain_x_j = true;
  }

  // The upper bound is exact: compute it into *this.
  for (dimension_type k = x.seq.size(); k-- > 0; )
    x.seq[k].join_assign(y.seq[k]);
  return true;
}

template <typename ITV>
bool
Box<ITV>::OK() const {
  if (status.test_empty_up_to_date() && !status.test_empty()) {
    Box tmp = *this;
    tmp.reset_empty_up_to_date();
    if (tmp.check_empty()) {
#ifndef NDEBUG
      std::cerr << "The box is empty, but it is marked as non-empty."
                << std::endl;
#endif // NDEBUG
      return false;
    }
  }

  // A box that is not marked empty must have meaningful intervals.
  if (!marked_empty()) {
    for (dimension_type k = seq.size(); k-- > 0; )
      if (!seq[k].OK())
        return false;
  }

  return true;
}

template <typename ITV>
dimension_type
Box<ITV>::affine_dimension() const {
  dimension_type d = space_dimension();
  // A zero-space-dim box always has affine dimension zero.
  if (d == 0)
    return 0;

  // An empty box has affine dimension zero.
  if (is_empty())
    return 0;

  for (dimension_type k = d; k-- > 0; )
    if (seq[k].is_singleton())
      --d;

  return d;
}

template <typename ITV>
bool
Box<ITV>::check_empty() const {
  PPL_ASSERT(!marked_empty());
  Box<ITV>& x = const_cast<Box<ITV>&>(*this);
  for (dimension_type k = seq.size(); k-- > 0; )
    if (seq[k].is_empty()) {
      x.set_empty();
      return true;
    }
  x.set_nonempty();
  return false;
}

template <typename ITV>
bool
Box<ITV>::is_universe() const {
  if (marked_empty())
    return false;
  for (dimension_type k = seq.size(); k-- > 0; )
    if (!seq[k].is_universe())
      return false;
  return true;
}

template <typename ITV>
bool
Box<ITV>::is_topologically_closed() const {
  if (ITV::is_always_topologically_closed() || is_empty())
    return true;

  for (dimension_type k = seq.size(); k-- > 0; )
    if (!seq[k].is_topologically_closed())
      return false;
  return true;
}

template <typename ITV>
bool
Box<ITV>::is_discrete() const {
  if (is_empty())
    return true;
  for (dimension_type k = seq.size(); k-- > 0; )
    if (!seq[k].is_singleton())
      return false;
  return true;
}

template <typename ITV>
bool
Box<ITV>::is_bounded() const {
  if (is_empty())
    return true;
  for (dimension_type k = seq.size(); k-- > 0; )
    if (!seq[k].is_bounded())
      return false;
  return true;
}

template <typename ITV>
bool
Box<ITV>::contains_integer_point() const {
  if (marked_empty())
    return false;
  for (dimension_type k = seq.size(); k-- > 0; )
    if (!seq[k].contains_integer_point())
      return false;
  return true;
}

template <typename ITV>
bool
Box<ITV>::frequency(const Linear_Expression& expr,
                  Coefficient& freq_n, Coefficient& freq_d,
                  Coefficient& val_n, Coefficient& val_d) const {
  dimension_type space_dim = space_dimension();
  // The dimension of `expr' must be at most the dimension of *this.
  if (space_dim < expr.space_dimension())
    throw_dimension_incompatible("frequency(e, ...)", "e", expr);

  // Check if `expr' has a constant value.
  // If it is constant, set the frequency `freq_n' to 0
  // and return true. Otherwise the values for \p expr
  // are not discrete so return false.

  // Space dimension is 0: if empty, then return false;
  // otherwise the frequency is 0 and the value is the inhomogeneous term.
  if (space_dim == 0) {
    if (is_empty())
      return false;
    freq_n = 0;
    freq_d = 1;
    val_n = expr.inhomogeneous_term();
    val_d = 1;
    return true;
  }

  // For an empty Box, we simply return false.
  if (is_empty())
    return false;

  // The Box has at least 1 dimension and is not empty.
  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);
  PPL_DIRTY_TEMP(mpq_class, tmp);
  Coefficient c = expr.inhomogeneous_term();

  PPL_DIRTY_TEMP_COEFFICIENT(val_denom);
  val_denom = 1;

  for (Linear_Expression::const_iterator i = expr.begin(), i_end = expr.end();
       i != i_end; ++i) {
    const ITV& seq_i = seq[i.variable().id()];
    // Check if `v' is constant in the BD shape.
    if (seq_i.is_singleton()) {
      // If `v' is constant, replace it in `le' by the value.
      assign_r(tmp, seq_i.lower(), ROUND_NOT_NEEDED);
      numer = tmp.get_num();
      denom = tmp.get_den();
      c *= denom;
      c += numer * val_denom * (*i);
      val_denom *= denom;
      continue;
    }
    // The expression `expr' is not constant.
    return false;
  }

  // The expression `expr' is constant.
  freq_n = 0;
  freq_d = 1;

  // Reduce `val_n' and `val_d'.
  normalize2(c, val_denom, val_n, val_d);
  return true;
}

template <typename ITV>
bool
Box<ITV>::constrains(Variable var) const {
  // `var' should be one of the dimensions of the polyhedron.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dimension() < var_space_dim)
    throw_dimension_incompatible("constrains(v)", "v", var);

  if (marked_empty() || !seq[var_space_dim-1].is_universe())
    return true;
  // Now force an emptiness check.
  return is_empty();
}

template <typename ITV>
void
Box<ITV>::unconstrain(const Variables_Set& vars) {
  // The cylindrification with respect to no dimensions is a no-op.
  // This case also captures the only legal cylindrification
  // of a box in a 0-dim space.
  if (vars.empty())
    return;

  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dimension() < min_space_dim)
    throw_dimension_incompatible("unconstrain(vs)", min_space_dim);

  // If the box is already empty, there is nothing left to do.
  if (marked_empty())
    return;

  // Here the box might still be empty (but we haven't detected it yet):
  // check emptiness of the interval for each of the variables in
  // `vars' before cylindrification.
  for (Variables_Set::const_iterator vsi = vars.begin(),
         vsi_end = vars.end(); vsi != vsi_end; ++vsi) {
    ITV& seq_vsi = seq[*vsi];
    if (!seq_vsi.is_empty())
      seq_vsi.assign(UNIVERSE);
    else {
      set_empty();
      break;
    }
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::topological_closure_assign() {
  if (ITV::is_always_topologically_closed() || is_empty())
    return;

  for (dimension_type k = seq.size(); k-- > 0; )
    seq[k].topological_closure_assign();
}

template <typename ITV>
void
Box<ITV>::wrap_assign(const Variables_Set& vars,
                      Bounded_Integer_Type_Width w,
                      Bounded_Integer_Type_Representation r,
                      Bounded_Integer_Type_Overflow o,
                      const Constraint_System* cs_p,
                      unsigned complexity_threshold,
                      bool wrap_individually) {
#if 0 // Generic implementation commented out.
  Implementation::wrap_assign(*this,
                              vars, w, r, o, cs_p,
                              complexity_threshold, wrap_individually,
                              "Box");
#else // Specialized implementation.
  PPL_USED(wrap_individually);
  PPL_USED(complexity_threshold);
  Box& x = *this;

  // Dimension-compatibility check for `*cs_p', if any.
  const dimension_type vars_space_dim = vars.space_dimension();
  if (cs_p != 0 && cs_p->space_dimension() > vars_space_dim) {
    std::ostringstream s;
    s << "PPL::Box<ITV>::wrap_assign(vars, w, r, o, cs_p, ...):"
      << std::endl
      << "vars.space_dimension() == " << vars_space_dim
      << ", cs_p->space_dimension() == " << cs_p->space_dimension() << ".";
    throw std::invalid_argument(s.str());
  }

  // Wrapping no variable only requires refining with *cs_p, if any.
  if (vars.empty()) {
    if (cs_p != 0)
      refine_with_constraints(*cs_p);
    return;
  }

  // Dimension-compatibility check for `vars'.
  const dimension_type space_dim = x.space_dimension();
  if (space_dim < vars_space_dim) {
    std::ostringstream s;
    s << "PPL::Box<ITV>::wrap_assign(vars, ...):"
      << std::endl
      << "this->space_dimension() == " << space_dim
      << ", required space dimension == " << vars_space_dim << ".";
    throw std::invalid_argument(s.str());
  }

  // Wrapping an empty polyhedron is a no-op.
  if (x.is_empty())
    return;

  // FIXME: temporarily (ab-) using Coefficient.
  // Set `min_value' and `max_value' to the minimum and maximum values
  // a variable of width `w' and signedness `s' can take.
  PPL_DIRTY_TEMP_COEFFICIENT(min_value);
  PPL_DIRTY_TEMP_COEFFICIENT(max_value);
  if (r == UNSIGNED) {
    min_value = 0;
    mul_2exp_assign(max_value, Coefficient_one(), w);
    --max_value;
  }
  else {
    PPL_ASSERT(r == SIGNED_2_COMPLEMENT);
    mul_2exp_assign(max_value, Coefficient_one(), w-1);
    neg_assign(min_value, max_value);
    --max_value;
  }

  // FIXME: Build the (integer) quadrant interval.
  PPL_DIRTY_TEMP(ITV, integer_quadrant_itv);
  PPL_DIRTY_TEMP(ITV, rational_quadrant_itv);
  {
    I_Constraint<Coefficient> lower = i_constraint(GREATER_OR_EQUAL, min_value);
    I_Constraint<Coefficient> upper = i_constraint(LESS_OR_EQUAL, max_value);
    integer_quadrant_itv.build(lower, upper);
    // The rational quadrant is only needed if overflow is undefined.
    if (o == OVERFLOW_UNDEFINED) {
      ++max_value;
      upper = i_constraint(LESS_THAN, max_value);
      rational_quadrant_itv.build(lower, upper);
    }
  }

  const Variables_Set::const_iterator vs_end = vars.end();

  if (cs_p == 0) {
    // No constraint refinement is needed here.
    switch (o) {
    case OVERFLOW_WRAPS:
      for (Variables_Set::const_iterator i = vars.begin(); i != vs_end; ++i)
        x.seq[*i].wrap_assign(w, r, integer_quadrant_itv);
      reset_empty_up_to_date();
      break;
    case OVERFLOW_UNDEFINED:
      for (Variables_Set::const_iterator i = vars.begin(); i != vs_end; ++i) {
        ITV& x_seq_v = x.seq[*i];
        if (!rational_quadrant_itv.contains(x_seq_v)) {
          x_seq_v.assign(integer_quadrant_itv);
        }
      }
      break;
    case OVERFLOW_IMPOSSIBLE:
      for (Variables_Set::const_iterator i = vars.begin(); i != vs_end; ++i)
        x.seq[*i].intersect_assign(integer_quadrant_itv);
      reset_empty_up_to_date();
      break;
    }
    PPL_ASSERT(x.OK());
    return;
  }

  PPL_ASSERT(cs_p != 0);
  const Constraint_System& cs = *cs_p;
  // A map associating interval constraints to variable indexes.
  typedef std::map<dimension_type, std::vector<const Constraint*> > map_type;
  map_type var_cs_map;
  for (Constraint_System::const_iterator i = cs.begin(),
         i_end = cs.end(); i != i_end; ++i) {
    const Constraint& c = *i;
    dimension_type c_num_vars = 0;
    dimension_type c_only_var = 0;
    if (Box_Helpers::extract_interval_constraint(c, c_num_vars, c_only_var)) {
      if (c_num_vars == 1) {
        // An interval constraint on variable index `c_only_var'.
        PPL_ASSERT(c_only_var < space_dim);
        // We do care about c if c_only_var is going to be wrapped.
        if (vars.find(c_only_var) != vs_end)
          var_cs_map[c_only_var].push_back(&c);
      }
      else {
        PPL_ASSERT(c_num_vars == 0);
        // Note: tautologies have been filtered out by iterators.
        PPL_ASSERT(c.is_inconsistent());
        x.set_empty();
        return;
      }
    }
  }

  PPL_DIRTY_TEMP(ITV, refinement_itv);
  const map_type::const_iterator var_cs_map_end = var_cs_map.end();
  // Loop through the variable indexes in `vars'.
  for (Variables_Set::const_iterator i = vars.begin(); i != vs_end; ++i) {
    const dimension_type v = *i;
    refinement_itv = integer_quadrant_itv;
    // Look for the refinement constraints for space dimension index `v'.
    map_type::const_iterator var_cs_map_iter = var_cs_map.find(v);
    if (var_cs_map_iter != var_cs_map_end) {
      // Refine interval for variable `v'.
      const map_type::mapped_type& var_cs = var_cs_map_iter->second;
      for (dimension_type j = var_cs.size(); j-- > 0; ) {
        const Constraint& c = *var_cs[j];
        refine_interval_no_check(refinement_itv,
                                 c.type(),
                                 c.inhomogeneous_term(),
                                 c.coefficient(Variable(v)));
      }
    }
    // Wrap space dimension index `v'.
    ITV& x_seq_v = x.seq[v];
    switch (o) {
    case OVERFLOW_WRAPS:
      x_seq_v.wrap_assign(w, r, refinement_itv);
      break;
    case OVERFLOW_UNDEFINED:
      if (!rational_quadrant_itv.contains(x_seq_v))
        x_seq_v.assign(UNIVERSE);
      break;
    case OVERFLOW_IMPOSSIBLE:
      x_seq_v.intersect_assign(refinement_itv);
      break;
    }
  }
  PPL_ASSERT(x.OK());
#endif
}

template <typename ITV>
void
Box<ITV>::drop_some_non_integer_points(Complexity_Class) {
  if (std::numeric_limits<typename ITV::boundary_type>::is_integer
      && !ITV::info_type::store_open)
    return;

  if (marked_empty())
    return;

  for (dimension_type k = seq.size(); k-- > 0; )
    seq[k].drop_some_non_integer_points();

  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::drop_some_non_integer_points(const Variables_Set& vars,
                                       Complexity_Class) {
  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dimension() < min_space_dim)
    throw_dimension_incompatible("drop_some_non_integer_points(vs, cmpl)",
                                 min_space_dim);

  if (std::numeric_limits<typename ITV::boundary_type>::is_integer
      && !ITV::info_type::store_open)
    return;

  if (marked_empty())
    return;

  for (Variables_Set::const_iterator v_i = vars.begin(),
         v_end = vars.end(); v_i != v_end; ++v_i)
    seq[*v_i].drop_some_non_integer_points();

  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::intersection_assign(const Box& y) {
  Box& x = *this;
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    x.throw_dimension_incompatible("intersection_assign(y)", y);

  // If one of the two boxes is empty, the intersection is empty.
  if (x.marked_empty())
    return;
  if (y.marked_empty()) {
    x.set_empty();
    return;
  }

  // If both boxes are zero-dimensional, then at this point they are
  // necessarily non-empty, so that their intersection is non-empty too.
  if (space_dim == 0)
    return;

  // FIXME: here we may conditionally exploit a capability of the
  // underlying interval to eagerly detect empty results.
  reset_empty_up_to_date();

  for (dimension_type k = space_dim; k-- > 0; )
    x.seq[k].intersect_assign(y.seq[k]);

  PPL_ASSERT(x.OK());
}

template <typename ITV>
void
Box<ITV>::upper_bound_assign(const Box& y) {
  Box& x = *this;

  // Dimension-compatibility check.
  if (x.space_dimension() != y.space_dimension())
    x.throw_dimension_incompatible("upper_bound_assign(y)", y);

  // The lub of a box with an empty box is equal to the first box.
  if (y.is_empty())
    return;
  if (x.is_empty()) {
    x = y;
    return;
  }

  for (dimension_type k = x.seq.size(); k-- > 0; )
    x.seq[k].join_assign(y.seq[k]);

  PPL_ASSERT(x.OK());
}

template <typename ITV>
void
Box<ITV>::concatenate_assign(const Box& y) {
  Box& x = *this;
  const dimension_type x_space_dim = x.space_dimension();
  const dimension_type y_space_dim = y.space_dimension();

  // If `y' is marked empty, the result will be empty too.
  if (y.marked_empty())
    x.set_empty();

  // If `y' is a 0-dim space box, there is nothing left to do.
  if (y_space_dim == 0)
    return;
  // The resulting space dimension must be at most the maximum.
  check_space_dimension_overflow(y.space_dimension(),
                                 max_space_dimension() - space_dimension(),
                                 "PPL::Box::",
                                 "concatenate_assign(y)",
                                 "concatenation exceeds the maximum "
                                 "allowed space dimension");
  // Here `y_space_dim > 0', so that a non-trivial concatenation will occur:
  // make sure that reallocation will occur once at most.
  x.seq.reserve(x_space_dim + y_space_dim);

  // If `x' is marked empty, then it is sufficient to adjust
  // the dimension of the vector space.
  if (x.marked_empty()) {
    x.seq.insert(x.seq.end(), y_space_dim, ITV(EMPTY));
    PPL_ASSERT(x.OK());
    return;
  }

  // Here neither `x' nor `y' are marked empty: concatenate them.
  std::copy(y.seq.begin(), y.seq.end(),
            std::back_insert_iterator<Sequence>(x.seq));
  // Update the `empty_up_to_date' flag.
  if (!y.status.test_empty_up_to_date())
    reset_empty_up_to_date();

  PPL_ASSERT(x.OK());
}

template <typename ITV>
void
Box<ITV>::difference_assign(const Box& y) {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("difference_assign(y)", y);

  Box& x = *this;
  if (x.is_empty() || y.is_empty())
    return;

  switch (space_dim) {
  case 0:
    // If `x' is zero-dimensional, then at this point both `x' and `y'
    // are the universe box, so that their difference is empty.
    x.set_empty();
    break;

  case 1:
    x.seq[0].difference_assign(y.seq[0]);
    if (x.seq[0].is_empty())
      x.set_empty();
    break;

  default:
    {
      dimension_type index_non_contained = space_dim;
      dimension_type number_non_contained = 0;
      for (dimension_type i = space_dim; i-- > 0; )
        if (!y.seq[i].contains(x.seq[i])) {
          if (++number_non_contained == 1)
            index_non_contained = i;
          else
            break;
        }

      switch (number_non_contained) {
      case 0:
        // `y' covers `x': the difference is empty.
        x.set_empty();
        break;
      case 1:
        x.seq[index_non_contained]
          .difference_assign(y.seq[index_non_contained]);
        if (x.seq[index_non_contained].is_empty())
          x.set_empty();
        break;
      default:
        // Nothing to do: the difference is `x'.
        break;
      }
    }
    break;
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
bool
Box<ITV>::simplify_using_context_assign(const Box& y) {
  Box& x = *this;
  const dimension_type num_dims = x.space_dimension();
  // Dimension-compatibility check.
  if (num_dims != y.space_dimension())
    x.throw_dimension_incompatible("simplify_using_context_assign(y)", y);

  // Filter away the zero-dimensional case.
  if (num_dims == 0) {
    if (y.marked_empty()) {
      x.set_nonempty();
      return false;
    }
    else
      return !x.marked_empty();
  }

  // Filter away the case when `y' is empty.
  if (y.is_empty()) {
    for (dimension_type i = num_dims; i-- > 0; )
      x.seq[i].assign(UNIVERSE);
    x.set_nonempty();
    return false;
  }

  if (x.is_empty()) {
    // Find in `y' a non-universe interval, if any.
    for (dimension_type i = 0; i < num_dims; ++i) {
      if (y.seq[i].is_universe())
        x.seq[i].assign(UNIVERSE);
      else {
        // Set x.seq[i] so as to contradict y.seq[i], if possible.
        ITV& seq_i = x.seq[i];
        seq_i.empty_intersection_assign(y.seq[i]);
        if (seq_i.is_empty()) {
          // We were not able to assign to `seq_i' a non-empty interval:
          // reset `seq_i' to the universe interval and keep searching.
          seq_i.assign(UNIVERSE);
          continue;
        }
        // We assigned to `seq_i' a non-empty interval:
        // set the other intervals to universe and return.
        for (++i; i < num_dims; ++i)
          x.seq[i].assign(UNIVERSE);
        x.set_nonempty();
        PPL_ASSERT(x.OK());
        return false;
      }
    }
    // All intervals in `y' are universe or could not be contradicted:
    // simplification can leave the empty box `x' as is.
    PPL_ASSERT(x.OK() && x.is_empty());
    return false;
  }

  // Loop index `i' is intentionally going upwards.
  for (dimension_type i = 0; i < num_dims; ++i) {
    if (!x.seq[i].simplify_using_context_assign(y.seq[i])) {
      PPL_ASSERT(!x.seq[i].is_empty());
      // The intersection of `x' and `y' is empty due to the i-th interval:
      // reset other intervals to UNIVERSE.
      for (dimension_type j = num_dims; j-- > i; )
        x.seq[j].assign(UNIVERSE);
      for (dimension_type j = i; j-- > 0; )
        x.seq[j].assign(UNIVERSE);
      PPL_ASSERT(x.OK());
      return false;
    }
  }
  PPL_ASSERT(x.OK());
  return true;
}

template <typename ITV>
void
Box<ITV>::time_elapse_assign(const Box& y) {
  Box& x = *this;
  const dimension_type x_space_dim = x.space_dimension();

  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    x.throw_dimension_incompatible("time_elapse_assign(y)", y);

  // Dealing with the zero-dimensional case.
  if (x_space_dim == 0) {
    if (y.marked_empty())
      x.set_empty();
    return;
  }

  // If either one of `x' or `y' is empty, the result is empty too.
  // Note: if possible, avoid cost of checking for emptiness.
  if (x.marked_empty() || y.marked_empty()
      || x.is_empty() || y.is_empty()) {
    x.set_empty();
    return;
  }

  for (dimension_type i = x_space_dim; i-- > 0; ) {
    ITV& x_seq_i = x.seq[i];
    const ITV& y_seq_i = y.seq[i];
    if (!x_seq_i.lower_is_boundary_infinity())
      if (y_seq_i.lower_is_boundary_infinity() || y_seq_i.lower() < 0)
        x_seq_i.lower_extend();
    if (!x_seq_i.upper_is_boundary_infinity())
      if (y_seq_i.upper_is_boundary_infinity() || y_seq_i.upper() > 0)
        x_seq_i.upper_extend();
  }
  PPL_ASSERT(x.OK());
}

template <typename ITV>
inline void
Box<ITV>::remove_space_dimensions(const Variables_Set& vars) {
  // The removal of no dimensions from any box is a no-op.
  // Note that this case also captures the only legal removal of
  // space dimensions from a box in a zero-dimensional space.
  if (vars.empty()) {
    PPL_ASSERT(OK());
    return;
  }

  const dimension_type old_space_dim = space_dimension();

  // Dimension-compatibility check.
  const dimension_type vsi_space_dim = vars.space_dimension();
  if (old_space_dim < vsi_space_dim)
    throw_dimension_incompatible("remove_space_dimensions(vs)",
                                 vsi_space_dim);

  const dimension_type new_space_dim = old_space_dim - vars.size();

  // If the box is empty (this must be detected), then resizing is all
  // what is needed.  If it is not empty and we are removing _all_ the
  // dimensions then, again, resizing suffices.
  if (is_empty() || new_space_dim == 0) {
    seq.resize(new_space_dim);
    PPL_ASSERT(OK());
    return;
  }

  // For each variable to be removed, we fill the corresponding interval
  // by shifting left those intervals that will not be removed.
  Variables_Set::const_iterator vsi = vars.begin();
  Variables_Set::const_iterator vsi_end = vars.end();
  dimension_type dst = *vsi;
  dimension_type src = dst + 1;
  for (++vsi; vsi != vsi_end; ++vsi) {
    const dimension_type vsi_next = *vsi;
    // All intervals in between are moved to the left.
    while (src < vsi_next)
      swap(seq[dst++], seq[src++]);
    ++src;
  }
  // Moving the remaining intervals.
  while (src < old_space_dim)
    swap(seq[dst++], seq[src++]);

  PPL_ASSERT(dst == new_space_dim);
  seq.resize(new_space_dim);

  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::remove_higher_space_dimensions(const dimension_type new_dimension) {
  // Dimension-compatibility check: the variable having
  // maximum index is the one occurring last in the set.
  const dimension_type space_dim = space_dimension();
  if (new_dimension > space_dim)
    throw_dimension_incompatible("remove_higher_space_dimensions(nd)",
                                 new_dimension);

  // The removal of no dimensions from any box is a no-op.
  // Note that this case also captures the only legal removal of
  // dimensions from a zero-dim space box.
  if (new_dimension == space_dim) {
    PPL_ASSERT(OK());
    return;
  }

  seq.resize(new_dimension);
  PPL_ASSERT(OK());
}

template <typename ITV>
template <typename Partial_Function>
void
Box<ITV>::map_space_dimensions(const Partial_Function& pfunc) {
  const dimension_type space_dim = space_dimension();
  if (space_dim == 0)
    return;

  if (pfunc.has_empty_codomain()) {
    // All dimensions vanish: the box becomes zero_dimensional.
    remove_higher_space_dimensions(0);
    return;
  }

  const dimension_type new_space_dim = pfunc.max_in_codomain() + 1;
  // If the box is empty, then simply adjust the space dimension.
  if (is_empty()) {
    remove_higher_space_dimensions(new_space_dim);
    return;
  }

  // We create a new Box with the new space dimension.
  Box<ITV> tmp(new_space_dim);
  // Map the intervals, exchanging the indexes.
  for (dimension_type i = 0; i < space_dim; ++i) {
    dimension_type new_i;
    if (pfunc.maps(i, new_i))
      swap(seq[i], tmp.seq[new_i]);
  }
  m_swap(tmp);
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::fold_space_dimensions(const Variables_Set& vars,
                                const Variable dest) {
  const dimension_type space_dim = space_dimension();
  // `dest' should be one of the dimensions of the box.
  if (dest.space_dimension() > space_dim)
    throw_dimension_incompatible("fold_space_dimensions(vs, v)", "v", dest);

  // The folding of no dimensions is a no-op.
  if (vars.empty())
    return;

  // All variables in `vars' should be dimensions of the box.
  if (vars.space_dimension() > space_dim)
    throw_dimension_incompatible("fold_space_dimensions(vs, v)",
                                 vars.space_dimension());

  // Moreover, `dest.id()' should not occur in `vars'.
  if (vars.find(dest.id()) != vars.end())
    throw_invalid_argument("fold_space_dimensions(vs, v)",
                           "v should not occur in vs");

  // Note: the check for emptiness is needed for correctness.
  if (!is_empty()) {
    // Join the interval corresponding to variable `dest' with the intervals
    // corresponding to the variables in `vars'.
    ITV& seq_v = seq[dest.id()];
    for (Variables_Set::const_iterator i = vars.begin(),
           vs_end = vars.end(); i != vs_end; ++i)
      seq_v.join_assign(seq[*i]);
  }
  remove_space_dimensions(vars);
}

template <typename ITV>
void
Box<ITV>::add_constraint_no_check(const Constraint& c) {
  PPL_ASSERT(c.space_dimension() <= space_dimension());

  dimension_type c_num_vars = 0;
  dimension_type c_only_var = 0;
  // Throw an exception if c is not an interval constraints.
  if (!Box_Helpers::extract_interval_constraint(c, c_num_vars, c_only_var))
    throw_invalid_argument("add_constraint(c)",
                           "c is not an interval constraint");

  // Throw an exception if c is a nontrivial strict constraint
  // and ITV does not support open boundaries.
  if (c.is_strict_inequality() && c_num_vars != 0
      && ITV::is_always_topologically_closed())
    throw_invalid_argument("add_constraint(c)",
                           "c is a nontrivial strict constraint");

  // Avoid doing useless work if the box is known to be empty.
  if (marked_empty())
    return;

  const Coefficient& n = c.inhomogeneous_term();
  if (c_num_vars == 0) {
    // Dealing with a trivial constraint.
    if (n < 0
        || (c.is_equality() && n != 0)
        || (c.is_strict_inequality() && n == 0))
      set_empty();
    return;
  }

  PPL_ASSERT(c_num_vars == 1);
  const Coefficient& d = c.coefficient(Variable(c_only_var));
  add_interval_constraint_no_check(c_only_var, c.type(), n, d);
}

template <typename ITV>
void
Box<ITV>::add_constraints_no_check(const Constraint_System& cs) {
  PPL_ASSERT(cs.space_dimension() <= space_dimension());
  // Note: even when the box is known to be empty, we need to go
  // through all the constraints to fulfill the method's contract
  // for what concerns exception throwing.
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i)
    add_constraint_no_check(*i);
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::add_congruence_no_check(const Congruence& cg) {
  PPL_ASSERT(cg.space_dimension() <= space_dimension());

  // Set aside the case of proper congruences.
  if (cg.is_proper_congruence()) {
    if (cg.is_inconsistent()) {
      set_empty();
      return;
    }
    else if (cg.is_tautological())
      return;
    else
      throw_invalid_argument("add_congruence(cg)",
                             "cg is a nontrivial proper congruence");
  }

  PPL_ASSERT(cg.is_equality());
  dimension_type cg_num_vars = 0;
  dimension_type cg_only_var = 0;
  // Throw an exception if c is not an interval congruence.
  if (!Box_Helpers::extract_interval_congruence(cg, cg_num_vars, cg_only_var))
    throw_invalid_argument("add_congruence(cg)",
                           "cg is not an interval congruence");

  // Avoid doing useless work if the box is known to be empty.
  if (marked_empty())
    return;

  const Coefficient& n = cg.inhomogeneous_term();
  if (cg_num_vars == 0) {
    // Dealing with a trivial equality congruence.
    if (n != 0)
      set_empty();
    return;
  }

  PPL_ASSERT(cg_num_vars == 1);
  const Coefficient& d = cg.coefficient(Variable(cg_only_var));
  add_interval_constraint_no_check(cg_only_var, Constraint::EQUALITY, n, d);
}

template <typename ITV>
void
Box<ITV>::add_congruences_no_check(const Congruence_System& cgs) {
  PPL_ASSERT(cgs.space_dimension() <= space_dimension());
  // Note: even when the box is known to be empty, we need to go
  // through all the congruences to fulfill the method's contract
  // for what concerns exception throwing.
  for (Congruence_System::const_iterator i = cgs.begin(),
         cgs_end = cgs.end(); i != cgs_end; ++i)
    add_congruence_no_check(*i);
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::refine_no_check(const Constraint& c) {
  PPL_ASSERT(c.space_dimension() <= space_dimension());
  PPL_ASSERT(!marked_empty());

  dimension_type c_num_vars = 0;
  dimension_type c_only_var = 0;
  // Non-interval constraints are approximated.
  if (!Box_Helpers::extract_interval_constraint(c, c_num_vars, c_only_var)) {
    propagate_constraint_no_check(c);
    return;
  }

  const Coefficient& n = c.inhomogeneous_term();
  if (c_num_vars == 0) {
    // Dealing with a trivial constraint.
    if (n < 0
        || (c.is_equality() && n != 0)
        || (c.is_strict_inequality() && n == 0))
      set_empty();
    return;
  }

  PPL_ASSERT(c_num_vars == 1);
  const Coefficient& d = c.coefficient(Variable(c_only_var));
  add_interval_constraint_no_check(c_only_var, c.type(), n, d);
}

template <typename ITV>
void
Box<ITV>::refine_no_check(const Constraint_System& cs) {
  PPL_ASSERT(cs.space_dimension() <= space_dimension());
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); !marked_empty() && i != cs_end; ++i)
    refine_no_check(*i);
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::refine_no_check(const Congruence& cg) {
  PPL_ASSERT(!marked_empty());

  PPL_ASSERT(cg.space_dimension() <= space_dimension());

  if (cg.is_proper_congruence()) {
    // A proper congruences is also an interval constraint
    // if and only if it is trivial.
    if (cg.is_inconsistent())
      set_empty();
    return;
  }

  PPL_ASSERT(cg.is_equality());
  Constraint c(cg);
  refine_no_check(c);
}

template <typename ITV>
void
Box<ITV>::refine_no_check(const Congruence_System& cgs) {
  PPL_ASSERT(cgs.space_dimension() <= space_dimension());
  for (Congruence_System::const_iterator i = cgs.begin(),
         cgs_end = cgs.end(); !marked_empty() && i != cgs_end; ++i)
    refine_no_check(*i);
  PPL_ASSERT(OK());
}

#if 1 // Alternative implementations for propagate_constraint_no_check.
namespace Implementation {

namespace Boxes {

inline bool
propagate_constraint_check_result(Result r, Ternary& open) {
  r = result_relation_class(r);
  switch (r) {
  case V_GT_MINUS_INFINITY:
  case V_LT_PLUS_INFINITY:
    return true;
  case V_LT:
  case V_GT:
    open = T_YES;
    return false;
  case V_LE:
  case V_GE:
    if (open == T_NO)
      open = T_MAYBE;
    return false;
  case V_EQ:
    return false;
  default:
    PPL_UNREACHABLE;
    return true;
  }
}

} // namespace Boxes

} // namespace Implementation


template <typename ITV>
void
Box<ITV>::propagate_constraint_no_check(const Constraint& c) {
  using namespace Implementation::Boxes;

  PPL_ASSERT(c.space_dimension() <= space_dimension());

  typedef
    typename Select_Temp_Boundary_Type<typename ITV::boundary_type>::type
    Temp_Boundary_Type;

  const dimension_type c_space_dim = c.space_dimension();
  const Constraint::Type c_type = c.type();
  const Coefficient& c_inhomogeneous_term = c.inhomogeneous_term();

  // Find a space dimension having a non-zero coefficient (if any).
  const dimension_type last_k
    = c.expression().last_nonzero(1, c_space_dim + 1);
  if (last_k == c_space_dim + 1) {
    // Constraint c is trivial: check if it is inconsistent.
    if (c_inhomogeneous_term < 0
        || (c_inhomogeneous_term == 0
            && c_type != Constraint::NONSTRICT_INEQUALITY))
      set_empty();
    return;
  }

  // Here constraint c is non-trivial.
  PPL_ASSERT(last_k <= c_space_dim);
  Temp_Boundary_Type t_bound;
  Temp_Boundary_Type t_a;
  Temp_Boundary_Type t_x;
  Ternary open;
  const Constraint::expr_type c_e = c.expression();
  for (Constraint::expr_type::const_iterator k = c_e.begin(),
         k_end = c_e.lower_bound(Variable(last_k)); k != k_end; ++k) {
    const Coefficient& a_k = *k;
    const Variable k_var = k.variable();
    const int sgn_a_k = sgn(a_k);
    if (sgn_a_k == 0)
      continue;
    Result r;
    if (sgn_a_k > 0) {
      open = (c_type == Constraint::STRICT_INEQUALITY) ? T_YES : T_NO;
      if (open == T_NO)
        maybe_reset_fpu_inexact<Temp_Boundary_Type>();
      r = assign_r(t_bound, c_inhomogeneous_term, ROUND_UP);
      if (propagate_constraint_check_result(r, open))
        goto maybe_refine_upper_1;
      r = neg_assign_r(t_bound, t_bound, ROUND_DOWN);
      if (propagate_constraint_check_result(r, open))
        goto maybe_refine_upper_1;
      for (Constraint::expr_type::const_iterator i = c_e.begin(),
            i_end = c_e.lower_bound(Variable(last_k)); i != i_end; ++i) {
        const Variable i_var = i.variable();
        if (i_var.id() == k_var.id())
          continue;
        const Coefficient& a_i = *i;
        const int sgn_a_i = sgn(a_i);
        ITV& x_i = seq[i_var.id()];
        if (sgn_a_i < 0) {
          if (x_i.lower_is_boundary_infinity())
            goto maybe_refine_upper_1;
          r = assign_r(t_a, a_i, ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_1;
          r = assign_r(t_x, x_i.lower(), ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_1;
          if (x_i.lower_is_open())
            open = T_YES;
          r = sub_mul_assign_r(t_bound, t_a, t_x, ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_1;
        }
        else {
          PPL_ASSERT(sgn_a_i > 0);
          if (x_i.upper_is_boundary_infinity())
            goto maybe_refine_upper_1;
          r = assign_r(t_a, a_i, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_1;
          r = assign_r(t_x, x_i.upper(), ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_1;
          if (x_i.upper_is_open())
            open = T_YES;
          r = sub_mul_assign_r(t_bound, t_a, t_x, ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_1;
        }
      }
      r = assign_r(t_a, a_k, ROUND_UP);
      if (propagate_constraint_check_result(r, open))
        goto maybe_refine_upper_1;
      r = div_assign_r(t_bound, t_bound, t_a, ROUND_DOWN);
      if (propagate_constraint_check_result(r, open))
        goto maybe_refine_upper_1;

      // Refine the lower bound of `seq[k]' with `t_bound'.
      if (open == T_MAYBE
          && maybe_check_fpu_inexact<Temp_Boundary_Type>() == 1)
        open = T_YES;
      {
        const Relation_Symbol rel
          = (open == T_YES) ? GREATER_THAN : GREATER_OR_EQUAL;
        seq[k_var.id()].add_constraint(i_constraint(rel, t_bound));
      }
      reset_empty_up_to_date();
    maybe_refine_upper_1:
      if (c_type != Constraint::EQUALITY)
        continue;
      open = T_NO;
      maybe_reset_fpu_inexact<Temp_Boundary_Type>();
      r = assign_r(t_bound, c_inhomogeneous_term, ROUND_DOWN);
      if (propagate_constraint_check_result(r, open))
        goto next_k;
      r = neg_assign_r(t_bound, t_bound, ROUND_UP);
      if (propagate_constraint_check_result(r, open))
        goto next_k;
      for (Constraint::expr_type::const_iterator i = c_e.begin(),
            i_end = c_e.lower_bound(Variable(c_space_dim)); i != i_end; ++i) {
        const Variable i_var = i.variable();
        if (i_var.id() == k_var.id())
          continue;
        const Coefficient& a_i = *i;
        const int sgn_a_i = sgn(a_i);
        ITV& x_i = seq[i_var.id()];
        if (sgn_a_i < 0) {
          if (x_i.upper_is_boundary_infinity())
            goto next_k;
          r = assign_r(t_a, a_i, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
          r = assign_r(t_x, x_i.upper(), ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
          if (x_i.upper_is_open())
            open = T_YES;
          r = sub_mul_assign_r(t_bound, t_a, t_x, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
        }
        else {
          PPL_ASSERT(sgn_a_i > 0);
          if (x_i.lower_is_boundary_infinity())
            goto next_k;
          r = assign_r(t_a, a_i, ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
          r = assign_r(t_x, x_i.lower(), ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
          if (x_i.lower_is_open())
            open = T_YES;
          r = sub_mul_assign_r(t_bound, t_a, t_x, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
        }
      }
      r = assign_r(t_a, a_k, ROUND_DOWN);
      if (propagate_constraint_check_result(r, open))
        goto next_k;
      r = div_assign_r(t_bound, t_bound, t_a, ROUND_UP);
      if (propagate_constraint_check_result(r, open))
        goto next_k;

      // Refine the upper bound of seq[k] with t_bound.
      if (open == T_MAYBE
          && maybe_check_fpu_inexact<Temp_Boundary_Type>() == 1)
        open = T_YES;
      const Relation_Symbol rel
        = (open == T_YES) ? LESS_THAN : LESS_OR_EQUAL;
      seq[k_var.id()].add_constraint(i_constraint(rel, t_bound));
      reset_empty_up_to_date();
    }
    else {
      PPL_ASSERT(sgn_a_k < 0);
      open = (c_type == Constraint::STRICT_INEQUALITY) ? T_YES : T_NO;
      if (open == T_NO)
        maybe_reset_fpu_inexact<Temp_Boundary_Type>();
      r = assign_r(t_bound, c_inhomogeneous_term, ROUND_UP);
      if (propagate_constraint_check_result(r, open))
        goto maybe_refine_upper_2;
      r = neg_assign_r(t_bound, t_bound, ROUND_DOWN);
      if (propagate_constraint_check_result(r, open))
        goto maybe_refine_upper_2;
      for (Constraint::expr_type::const_iterator i = c_e.begin(),
            i_end = c_e.lower_bound(Variable(c_space_dim)); i != i_end; ++i) {
        const Variable i_var = i.variable();
        if (i_var.id() == k_var.id())
          continue;
        const Coefficient& a_i = *i;
        const int sgn_a_i = sgn(a_i);
        ITV& x_i = seq[i_var.id()];
        if (sgn_a_i < 0) {
          if (x_i.lower_is_boundary_infinity())
            goto maybe_refine_upper_2;
          r = assign_r(t_a, a_i, ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_2;
          r = assign_r(t_x, x_i.lower(), ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_2;
          if (x_i.lower_is_open())
            open = T_YES;
          r = sub_mul_assign_r(t_bound, t_a, t_x, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_2;
        }
        else {
          PPL_ASSERT(sgn_a_i > 0);
          if (x_i.upper_is_boundary_infinity())
            goto maybe_refine_upper_2;
          r = assign_r(t_a, a_i, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_2;
          r = assign_r(t_x, x_i.upper(), ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_2;
          if (x_i.upper_is_open())
            open = T_YES;
          r = sub_mul_assign_r(t_bound, t_a, t_x, ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto maybe_refine_upper_2;
        }
      }
      r = assign_r(t_a, a_k, ROUND_UP);
      if (propagate_constraint_check_result(r, open))
        goto maybe_refine_upper_2;
      r = div_assign_r(t_bound, t_bound, t_a, ROUND_UP);
      if (propagate_constraint_check_result(r, open))
        goto maybe_refine_upper_2;

      // Refine the upper bound of seq[k] with t_bound.
      if (open == T_MAYBE
          && maybe_check_fpu_inexact<Temp_Boundary_Type>() == 1)
        open = T_YES;
      {
        const Relation_Symbol rel
          = (open == T_YES) ? LESS_THAN : LESS_OR_EQUAL;
        seq[k_var.id()].add_constraint(i_constraint(rel, t_bound));
      }
      reset_empty_up_to_date();
    maybe_refine_upper_2:
      if (c_type != Constraint::EQUALITY)
        continue;
      open = T_NO;
      maybe_reset_fpu_inexact<Temp_Boundary_Type>();
      r = assign_r(t_bound, c_inhomogeneous_term, ROUND_DOWN);
      if (propagate_constraint_check_result(r, open))
        goto next_k;
      r = neg_assign_r(t_bound, t_bound, ROUND_UP);
      if (propagate_constraint_check_result(r, open))
        goto next_k;
      for (Constraint::expr_type::const_iterator i = c_e.begin(),
            i_end = c_e.lower_bound(Variable(c_space_dim)); i != i_end; ++i) {
        const Variable i_var = i.variable();
        if (i_var.id() == k_var.id())
          continue;
        const Coefficient& a_i = *i;
        const int sgn_a_i = sgn(a_i);
        ITV& x_i = seq[i_var.id()];
        if (sgn_a_i < 0) {
          if (x_i.upper_is_boundary_infinity())
            goto next_k;
          r = assign_r(t_a, a_i, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
          r = assign_r(t_x, x_i.upper(), ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
          if (x_i.upper_is_open())
            open = T_YES;
          r = sub_mul_assign_r(t_bound, t_a, t_x, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
        }
        else {
          PPL_ASSERT(sgn_a_i > 0);
          if (x_i.lower_is_boundary_infinity())
            goto next_k;
          r = assign_r(t_a, a_i, ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
          r = assign_r(t_x, x_i.lower(), ROUND_DOWN);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
          if (x_i.lower_is_open())
            open = T_YES;
          r = sub_mul_assign_r(t_bound, t_a, t_x, ROUND_UP);
          if (propagate_constraint_check_result(r, open))
            goto next_k;
        }
      }
      r = assign_r(t_a, a_k, ROUND_DOWN);
      if (propagate_constraint_check_result(r, open))
        goto next_k;
      r = div_assign_r(t_bound, t_bound, t_a, ROUND_DOWN);
      if (propagate_constraint_check_result(r, open))
        goto next_k;

      // Refine the lower bound of seq[k] with t_bound.
      if (open == T_MAYBE
          && maybe_check_fpu_inexact<Temp_Boundary_Type>() == 1)
        open = T_YES;
      const Relation_Symbol rel
        = (open == T_YES) ? GREATER_THAN : GREATER_OR_EQUAL;
      seq[k_var.id()].add_constraint(i_constraint(rel, t_bound));
      reset_empty_up_to_date();
    }
  next_k:
    ;
  }
}

#else // Alternative implementations for propagate_constraint_no_check.

template <typename ITV>
void
Box<ITV>::propagate_constraint_no_check(const Constraint& c) {
  PPL_ASSERT(c.space_dimension() <= space_dimension());

  dimension_type c_space_dim = c.space_dimension();
  ITV k[c_space_dim];
  ITV p[c_space_dim];
  for (Constraint::expr_type::const_iterator i = c_e.begin(),
        i_end = c_e.lower_bound(Variable(c_space_dim)); i != i_end; ++i) {
    const Variable i_var = i.variable();
    k[i_var.id()] = *i;
    ITV& p_i = p[i_var.id()];
    p_i = seq[i_var.id()];
    p_i.mul_assign(p_i, k[i_var.id()]);
  }
  const Coefficient& inhomogeneous_term = c.inhomogeneous_term();
  for (Constraint::expr_type::const_iterator i = c_e.begin(),
        i_end = c_e.lower_bound(Variable(c_space_dim)); i != i_end; ++i) {
    const Variable i_var = i.variable();
    int sgn_coefficient_i = sgn(*i);
    ITV q(inhomogeneous_term);
    for (Constraint::expr_type::const_iterator j = c_e.begin(),
          j_end = c_e.lower_bound(Variable(c_space_dim)); j != j_end; ++j) {
      const Variable j_var = j.variable();
      if (i_var == j_var)
        continue;
      q.add_assign(q, p[j_var.id()]);
    }
    q.div_assign(q, k[i_var.id()]);
    q.neg_assign(q);
    Relation_Symbol rel;
    switch (c.type()) {
    case Constraint::EQUALITY:
      rel = EQUAL;
      break;
    case Constraint::NONSTRICT_INEQUALITY:
      rel = (sgn_coefficient_i > 0) ? GREATER_OR_EQUAL : LESS_OR_EQUAL;
      break;
    case Constraint::STRICT_INEQUALITY:
      rel = (sgn_coefficient_i > 0) ? GREATER_THAN : LESS_THAN;
      break;
    }
    seq[i_var.id()].add_constraint(i_constraint(rel, q));
    // FIXME: could/should we exploit the return value of add_constraint
    //        in case it is available?
    // FIXME: should we instead be lazy and do not even bother about
    //        the possibility the interval becomes empty apart from setting
    //        empty_up_to_date = false?
    if (seq[i_var.id()].is_empty()) {
      set_empty();
      break;
    }
  }

  PPL_ASSERT(OK());
}

#endif // Alternative implementations for propagate_constraint_no_check.

template <typename ITV>
void
Box<ITV>
::propagate_constraints_no_check(const Constraint_System& cs,
                                 const dimension_type max_iterations) {
  const dimension_type space_dim = space_dimension();
  PPL_ASSERT(cs.space_dimension() <= space_dim);

  const Constraint_System::const_iterator cs_begin = cs.begin();
  const Constraint_System::const_iterator cs_end = cs.end();
  const dimension_type propagation_weight
    = Implementation::num_constraints(cs) * space_dim;

  Sequence copy;
  bool changed;
  dimension_type num_iterations = 0;
  do {
    WEIGHT_BEGIN();
    ++num_iterations;
    copy = seq;
    for (Constraint_System::const_iterator i = cs_begin; i != cs_end; ++i)
      propagate_constraint_no_check(*i);

    WEIGHT_ADD_MUL(40, propagation_weight);
    // Check if the client has requested abandoning all expensive
    // computations.  If so, the exception specified by the client
    // is thrown now.
    maybe_abandon();

    // NOTE: if max_iterations == 0 (i.e., no iteration limit is set)
    // the following test will anyway trigger on wrap around.
    if (num_iterations == max_iterations)
      break;

    changed = (copy != seq);
  } while (changed);
}

template <typename ITV>
void
Box<ITV>::affine_image(const Variable var,
                       const Linear_Expression& expr,
                       Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("affine_image(v, e, d)", "d == 0");

  // Dimension-compatibility checks.
  const dimension_type space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("affine_image(v, e, d)", "e", expr);
  // `var' should be one of the dimensions of the polyhedron.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("affine_image(v, e, d)", "v", var);

  if (is_empty())
    return;

  Tmp_Interval_Type expr_value;
  Tmp_Interval_Type temp0;
  Tmp_Interval_Type temp1;
  expr_value.assign(expr.inhomogeneous_term());
  for (Linear_Expression::const_iterator i = expr.begin(),
          i_end = expr.end(); i != i_end; ++i) {
    temp0.assign(*i);
    temp1.assign(seq[i.variable().id()]);
    temp0.mul_assign(temp0, temp1);
    expr_value.add_assign(expr_value, temp0);
  }
  if (denominator != 1) {
    temp0.assign(denominator);
    expr_value.div_assign(expr_value, temp0);
  }
  seq[var.id()].assign(expr_value);

  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::affine_form_image(const Variable var,
                            const Linear_Form<ITV>& lf) {

  // Check that ITV has a floating point boundary type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<typename ITV::boundary_type>
            ::is_exact, "Box<ITV>::affine_form_image(Variable, Linear_Form):"
                        "ITV has not a floating point boundary type.");

  // Dimension-compatibility checks.
  const dimension_type space_dim = space_dimension();
  const dimension_type lf_space_dim = lf.space_dimension();
  if (space_dim < lf_space_dim)
    throw_dimension_incompatible("affine_form_image(var, lf)", "lf", lf);
  // `var' should be one of the dimensions of the polyhedron.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("affine_form_image(var, lf)", "var", var);

  if (is_empty())
    return;

  // Intervalization of 'lf'.
  ITV result = lf.inhomogeneous_term();
  for (dimension_type i = 0; i < lf_space_dim; ++i) {
    ITV current_addend = lf.coefficient(Variable(i));
    const ITV& curr_int = seq[i];
    current_addend *= curr_int;
    result += current_addend;
  }

  seq[var.id()].assign(result);
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::affine_preimage(const Variable var,
                          const Linear_Expression& expr,
                          Coefficient_traits::const_reference
                          denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("affine_preimage(v, e, d)", "d == 0");

  // Dimension-compatibility checks.
  const dimension_type x_space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (x_space_dim < expr_space_dim)
    throw_dimension_incompatible("affine_preimage(v, e, d)", "e", expr);
  // `var' should be one of the dimensions of the polyhedron.
  const dimension_type var_space_dim = var.space_dimension();
  if (x_space_dim < var_space_dim)
    throw_dimension_incompatible("affine_preimage(v, e, d)", "v", var);

  if (is_empty())
    return;

  const Coefficient& expr_v = expr.coefficient(var);
  const bool invertible = (expr_v != 0);
  if (!invertible) {
    Tmp_Interval_Type expr_value;
    Tmp_Interval_Type temp0;
    Tmp_Interval_Type temp1;
    expr_value.assign(expr.inhomogeneous_term());
    for (Linear_Expression::const_iterator i = expr.begin(),
            i_end = expr.end(); i != i_end; ++i) {
      temp0.assign(*i);
      temp1.assign(seq[i.variable().id()]);
      temp0.mul_assign(temp0, temp1);
      expr_value.add_assign(expr_value, temp0);
    }
    if (denominator != 1) {
      temp0.assign(denominator);
      expr_value.div_assign(expr_value, temp0);
    }
    ITV& x_seq_v = seq[var.id()];
    expr_value.intersect_assign(x_seq_v);
    if (expr_value.is_empty())
      set_empty();
    else
      x_seq_v.assign(UNIVERSE);
  }
  else {
    // The affine transformation is invertible.
    // CHECKME: for efficiency, would it be meaningful to avoid
    // the computation of inverse by partially evaluating the call
    // to affine_image?
    Linear_Expression inverse;
    inverse -= expr;
    inverse += (expr_v + denominator) * var;
    affine_image(var, inverse, expr_v);
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>
::bounded_affine_image(const Variable var,
                       const Linear_Expression& lb_expr,
                       const Linear_Expression& ub_expr,
                       Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("bounded_affine_image(v, lb, ub, d)", "d == 0");

  // Dimension-compatibility checks.
  const dimension_type space_dim = space_dimension();
  // The dimension of `lb_expr' and `ub_expr' should not be
  // greater than the dimension of `*this'.
  const dimension_type lb_space_dim = lb_expr.space_dimension();
  if (space_dim < lb_space_dim)
    throw_dimension_incompatible("bounded_affine_image(v, lb, ub, d)",
                                 "lb", lb_expr);
  const dimension_type ub_space_dim = ub_expr.space_dimension();
  if (space_dim < ub_space_dim)
    throw_dimension_incompatible("bounded_affine_image(v, lb, ub, d)",
                                 "ub", ub_expr);
    // `var' should be one of the dimensions of the box.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("affine_image(v, e, d)", "v", var);

  // Any image of an empty box is empty.
  if (is_empty())
    return;

  // Add the constraint implied by the `lb_expr' and `ub_expr'.
  if (denominator > 0)
    refine_with_constraint(lb_expr <= ub_expr);
  else
    refine_with_constraint(lb_expr >= ub_expr);

  // Check whether `var' occurs in `lb_expr' and/or `ub_expr'.
  if (lb_expr.coefficient(var) == 0) {
    // Here `var' can only occur in `ub_expr'.
    generalized_affine_image(var,
                             LESS_OR_EQUAL,
                             ub_expr,
                             denominator);
    if (denominator > 0)
      refine_with_constraint(lb_expr <= denominator*var);
    else
      refine_with_constraint(denominator*var <= lb_expr);
  }
  else if (ub_expr.coefficient(var) == 0) {
    // Here `var' can only occur in `lb_expr'.
    generalized_affine_image(var,
                             GREATER_OR_EQUAL,
                             lb_expr,
                             denominator);
    if (denominator > 0)
      refine_with_constraint(denominator*var <= ub_expr);
    else
      refine_with_constraint(ub_expr <= denominator*var);
  }
  else {
    // Here `var' occurs in both `lb_expr' and `ub_expr'.  As boxes
    // can only use the non-relational constraints, we find the
    // maximum/minimum values `ub_expr' and `lb_expr' obtain with the
    // box and use these instead of the `ub-expr' and `lb-expr'.
    PPL_DIRTY_TEMP(Coefficient, max_numer);
    PPL_DIRTY_TEMP(Coefficient, max_denom);
    bool max_included;
    PPL_DIRTY_TEMP(Coefficient, min_numer);
    PPL_DIRTY_TEMP(Coefficient, min_denom);
    bool min_included;
    ITV& seq_v = seq[var.id()];
    if (maximize(ub_expr, max_numer, max_denom, max_included)) {
      if (minimize(lb_expr, min_numer, min_denom, min_included)) {
        // The `ub_expr' has a maximum value and the `lb_expr'
        // has a minimum value for the box.
        // Set the bounds for `var' using the minimum for `lb_expr'.
        min_denom *= denominator;
        PPL_DIRTY_TEMP(mpq_class, q1);
        PPL_DIRTY_TEMP(mpq_class, q2);
        assign_r(q1.get_num(), min_numer, ROUND_NOT_NEEDED);
        assign_r(q1.get_den(), min_denom, ROUND_NOT_NEEDED);
        q1.canonicalize();
        // Now make the maximum of lb_expr the upper bound.  If the
        // maximum is not at a box point, then inequality is strict.
        max_denom *= denominator;
        assign_r(q2.get_num(), max_numer, ROUND_NOT_NEEDED);
        assign_r(q2.get_den(), max_denom, ROUND_NOT_NEEDED);
        q2.canonicalize();

        if (denominator > 0) {
          Relation_Symbol gr = min_included ? GREATER_OR_EQUAL : GREATER_THAN;
          Relation_Symbol lr = max_included ? LESS_OR_EQUAL : LESS_THAN;
          seq_v.build(i_constraint(gr, q1), i_constraint(lr, q2));
        }
        else {
          Relation_Symbol gr = max_included ? GREATER_OR_EQUAL : GREATER_THAN;
          Relation_Symbol lr = min_included ? LESS_OR_EQUAL : LESS_THAN;
          seq_v.build(i_constraint(gr, q2), i_constraint(lr, q1));
        }
      }
      else {
        // The `ub_expr' has a maximum value but the `lb_expr'
        // has no minimum value for the box.
        // Set the bounds for `var' using the maximum for `lb_expr'.
        PPL_DIRTY_TEMP(mpq_class, q);
        max_denom *= denominator;
        assign_r(q.get_num(), max_numer, ROUND_NOT_NEEDED);
        assign_r(q.get_den(), max_denom, ROUND_NOT_NEEDED);
        q.canonicalize();
        Relation_Symbol rel = (denominator > 0)
          ? (max_included ? LESS_OR_EQUAL : LESS_THAN)
          : (max_included ? GREATER_OR_EQUAL : GREATER_THAN);
        seq_v.build(i_constraint(rel, q));
      }
    }
    else if (minimize(lb_expr, min_numer, min_denom, min_included)) {
        // The `ub_expr' has no maximum value but the `lb_expr'
        // has a minimum value for the box.
        // Set the bounds for `var' using the minimum for `lb_expr'.
        min_denom *= denominator;
        PPL_DIRTY_TEMP(mpq_class, q);
        assign_r(q.get_num(), min_numer, ROUND_NOT_NEEDED);
        assign_r(q.get_den(), min_denom, ROUND_NOT_NEEDED);
        q.canonicalize();

        Relation_Symbol rel = (denominator > 0)
          ? (min_included ? GREATER_OR_EQUAL : GREATER_THAN)
          : (min_included ? LESS_OR_EQUAL : LESS_THAN);
        seq_v.build(i_constraint(rel, q));
    }
    else {
      // The `ub_expr' has no maximum value and the `lb_expr'
      // has no minimum value for the box.
      // So we set the bounds to be unbounded.
      seq_v.assign(UNIVERSE);
    }
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>
::bounded_affine_preimage(const Variable var,
                          const Linear_Expression& lb_expr,
                          const Linear_Expression& ub_expr,
                          Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  const dimension_type space_dim = space_dimension();
  if (denominator == 0)
    throw_invalid_argument("bounded_affine_preimage(v, lb, ub, d)", "d == 0");

  // Dimension-compatibility checks.
  // `var' should be one of the dimensions of the polyhedron.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("bounded_affine_preimage(v, lb, ub, d)",
                                 "v", var);
  // The dimension of `lb_expr' and `ub_expr' should not be
  // greater than the dimension of `*this'.
  const dimension_type lb_space_dim = lb_expr.space_dimension();
  if (space_dim < lb_space_dim)
    throw_dimension_incompatible("bounded_affine_preimage(v, lb, ub, d)",
                                 "lb", lb_expr);
  const dimension_type ub_space_dim = ub_expr.space_dimension();
  if (space_dim < ub_space_dim)
    throw_dimension_incompatible("bounded_affine_preimage(v, lb, ub, d)",
                                 "ub", ub_expr);

  // Any preimage of an empty polyhedron is empty.
  if (marked_empty())
    return;

  const bool negative_denom = (denominator < 0);
  const Coefficient& lb_var_coeff = lb_expr.coefficient(var);
  const Coefficient& ub_var_coeff = ub_expr.coefficient(var);

  // If the implied constraint between `ub_expr and `lb_expr' is
  // independent of `var', then impose it now.
  if (lb_var_coeff == ub_var_coeff) {
    if (negative_denom)
      refine_with_constraint(lb_expr >= ub_expr);
    else
      refine_with_constraint(lb_expr <= ub_expr);
  }

  ITV& seq_var = seq[var.id()];
  if (!seq_var.is_universe()) {
    // We want to work with a positive denominator,
    // so the sign and its (unsigned) value are separated.
    PPL_DIRTY_TEMP_COEFFICIENT(pos_denominator);
    pos_denominator = denominator;
    if (negative_denom)
      neg_assign(pos_denominator, pos_denominator);
    // Store all the information about the upper and lower bounds
    // for `var' before making this interval unbounded.
    bool open_lower = seq_var.lower_is_open();
    bool unbounded_lower = seq_var.lower_is_boundary_infinity();
    PPL_DIRTY_TEMP(mpq_class, q_seq_var_lower);
    PPL_DIRTY_TEMP(Coefficient, numer_lower);
    PPL_DIRTY_TEMP(Coefficient, denom_lower);
    if (!unbounded_lower) {
      assign_r(q_seq_var_lower, seq_var.lower(), ROUND_NOT_NEEDED);
      assign_r(numer_lower, q_seq_var_lower.get_num(), ROUND_NOT_NEEDED);
      assign_r(denom_lower, q_seq_var_lower.get_den(), ROUND_NOT_NEEDED);
      if (negative_denom)
        neg_assign(denom_lower, denom_lower);
      numer_lower *= pos_denominator;
      seq_var.lower_extend();
    }
    bool open_upper = seq_var.upper_is_open();
    bool unbounded_upper = seq_var.upper_is_boundary_infinity();
    PPL_DIRTY_TEMP(mpq_class, q_seq_var_upper);
    PPL_DIRTY_TEMP(Coefficient, numer_upper);
    PPL_DIRTY_TEMP(Coefficient, denom_upper);
    if (!unbounded_upper) {
      assign_r(q_seq_var_upper, seq_var.upper(), ROUND_NOT_NEEDED);
      assign_r(numer_upper, q_seq_var_upper.get_num(), ROUND_NOT_NEEDED);
      assign_r(denom_upper, q_seq_var_upper.get_den(), ROUND_NOT_NEEDED);
      if (negative_denom)
        neg_assign(denom_upper, denom_upper);
      numer_upper *= pos_denominator;
      seq_var.upper_extend();
    }

    if (!unbounded_lower) {
      // `lb_expr' is revised by removing the `var' component,
      // multiplying by `-' denominator of the lower bound for `var',
      // and adding the lower bound for `var' to the inhomogeneous term.
      Linear_Expression revised_lb_expr(ub_expr);
      revised_lb_expr -= ub_var_coeff * var;
      PPL_DIRTY_TEMP(Coefficient, d);
      neg_assign(d, denom_lower);
      revised_lb_expr *= d;
      revised_lb_expr += numer_lower;

      // Find the minimum value for the revised lower bound expression
      // and use this to refine the appropriate bound.
      bool included;
      PPL_DIRTY_TEMP(Coefficient, denom);
      if (minimize(revised_lb_expr, numer_lower, denom, included)) {
        denom_lower *= (denom * ub_var_coeff);
        PPL_DIRTY_TEMP(mpq_class, q);
        assign_r(q.get_num(), numer_lower, ROUND_NOT_NEEDED);
        assign_r(q.get_den(), denom_lower, ROUND_NOT_NEEDED);
        q.canonicalize();
        if (!included)
          open_lower = true;
        Relation_Symbol rel;
        if ((ub_var_coeff >= 0) ? !negative_denom : negative_denom)
          rel = open_lower ? GREATER_THAN : GREATER_OR_EQUAL;
        else
          rel = open_lower ? LESS_THAN : LESS_OR_EQUAL;
        seq_var.add_constraint(i_constraint(rel, q));
        if (seq_var.is_empty()) {
          set_empty();
          return;
        }
      }
    }

    if (!unbounded_upper) {
      // `ub_expr' is revised by removing the `var' component,
      // multiplying by `-' denominator of the upper bound for `var',
      // and adding the upper bound for `var' to the inhomogeneous term.
      Linear_Expression revised_ub_expr(lb_expr);
      revised_ub_expr -= lb_var_coeff * var;
      PPL_DIRTY_TEMP(Coefficient, d);
      neg_assign(d, denom_upper);
      revised_ub_expr *= d;
      revised_ub_expr += numer_upper;

      // Find the maximum value for the revised upper bound expression
      // and use this to refine the appropriate bound.
      bool included;
      PPL_DIRTY_TEMP(Coefficient, denom);
      if (maximize(revised_ub_expr, numer_upper, denom, included)) {
        denom_upper *= (denom * lb_var_coeff);
        PPL_DIRTY_TEMP(mpq_class, q);
        assign_r(q.get_num(), numer_upper, ROUND_NOT_NEEDED);
        assign_r(q.get_den(), denom_upper, ROUND_NOT_NEEDED);
        q.canonicalize();
        if (!included)
          open_upper = true;
        Relation_Symbol rel;
        if ((lb_var_coeff >= 0) ? !negative_denom : negative_denom)
          rel = open_upper ? LESS_THAN : LESS_OR_EQUAL;
        else
          rel = open_upper ? GREATER_THAN : GREATER_OR_EQUAL;
        seq_var.add_constraint(i_constraint(rel, q));
        if (seq_var.is_empty()) {
          set_empty();
          return;
        }
      }
    }
  }

  // If the implied constraint between `ub_expr and `lb_expr' is
  // dependent on `var', then impose on the new box.
  if (lb_var_coeff != ub_var_coeff) {
    if (denominator > 0)
      refine_with_constraint(lb_expr <= ub_expr);
    else
      refine_with_constraint(lb_expr >= ub_expr);
  }

  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>
::generalized_affine_image(const Variable var,
                           const Relation_Symbol relsym,
                           const Linear_Expression& expr,
                           Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("generalized_affine_image(v, r, e, d)", "d == 0");

  // Dimension-compatibility checks.
  const dimension_type space_dim = space_dimension();
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  if (space_dim < expr.space_dimension())
    throw_dimension_incompatible("generalized_affine_image(v, r, e, d)",
                                 "e", expr);
  // `var' should be one of the dimensions of the box.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("generalized_affine_image(v, r, e, d)",
                                 "v", var);

  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_image(v, r, e, d)",
                           "r is the disequality relation symbol");

  // First compute the affine image.
  affine_image(var, expr, denominator);

  if (relsym == EQUAL)
    // The affine relation is indeed an affine function.
    return;

  // Any image of an empty box is empty.
  if (is_empty())
    return;

  ITV& seq_var = seq[var.id()];
  switch (relsym) {
  case LESS_OR_EQUAL:
    seq_var.lower_extend();
    break;
  case LESS_THAN:
    seq_var.lower_extend();
    if (!seq_var.upper_is_boundary_infinity())
      seq_var.remove_sup();
    break;
  case GREATER_OR_EQUAL:
    seq_var.upper_extend();
    break;
  case GREATER_THAN:
    seq_var.upper_extend();
    if (!seq_var.lower_is_boundary_infinity())
      seq_var.remove_inf();
    break;
  default:
    // The EQUAL and NOT_EQUAL cases have been already dealt with.
    PPL_UNREACHABLE;
    break;
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>
::generalized_affine_preimage(const Variable var,
                              const Relation_Symbol relsym,
                              const Linear_Expression& expr,
                              Coefficient_traits::const_reference denominator)
{
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("generalized_affine_preimage(v, r, e, d)",
                           "d == 0");

  // Dimension-compatibility checks.
  const dimension_type space_dim = space_dimension();
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  if (space_dim < expr.space_dimension())
    throw_dimension_incompatible("generalized_affine_preimage(v, r, e, d)",
                                 "e", expr);
  // `var' should be one of the dimensions of the box.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(v, r, e, d)",
                                 "v", var);
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_preimage(v, r, e, d)",
                           "r is the disequality relation symbol");

  // Check whether the affine relation is indeed an affine function.
  if (relsym == EQUAL) {
    affine_preimage(var, expr, denominator);
    return;
  }

  // Compute the reversed relation symbol to simplify later coding.
  Relation_Symbol reversed_relsym;
  switch (relsym) {
  case LESS_THAN:
    reversed_relsym = GREATER_THAN;
    break;
  case LESS_OR_EQUAL:
    reversed_relsym = GREATER_OR_EQUAL;
    break;
  case GREATER_OR_EQUAL:
    reversed_relsym = LESS_OR_EQUAL;
    break;
  case GREATER_THAN:
    reversed_relsym = LESS_THAN;
    break;
  default:
    // The EQUAL and NOT_EQUAL cases have been already dealt with.
    PPL_UNREACHABLE;
    break;
  }

  // Check whether the preimage of this affine relation can be easily
  // computed as the image of its inverse relation.
  const Coefficient& var_coefficient = expr.coefficient(var);
  if (var_coefficient != 0) {
    Linear_Expression inverse_expr
      = expr - (denominator + var_coefficient) * var;
    PPL_DIRTY_TEMP_COEFFICIENT(inverse_denominator);
    neg_assign(inverse_denominator, var_coefficient);
    Relation_Symbol inverse_relsym
      = (sgn(denominator) == sgn(inverse_denominator))
      ? relsym
      : reversed_relsym;
    generalized_affine_image(var, inverse_relsym, inverse_expr,
                             inverse_denominator);
    return;
  }

  // Here `var_coefficient == 0', so that the preimage cannot
  // be easily computed by inverting the affine relation.
  // Shrink the box by adding the constraint induced
  // by the affine relation.
  // First, compute the maximum and minimum value reached by
  // `denominator*var' on the box as we need to use non-relational
  // expressions.
  PPL_DIRTY_TEMP(Coefficient, max_numer);
  PPL_DIRTY_TEMP(Coefficient, max_denom);
  bool max_included;
  bool bound_above = maximize(denominator*var, max_numer, max_denom, max_included);
  PPL_DIRTY_TEMP(Coefficient, min_numer);
  PPL_DIRTY_TEMP(Coefficient, min_denom);
  bool min_included;
  bool bound_below = minimize(denominator*var, min_numer, min_denom, min_included);
  // Use the correct relation symbol
  const Relation_Symbol corrected_relsym
    = (denominator > 0) ? relsym : reversed_relsym;
  // Revise the expression to take into account the denominator of the
  // maximum/minimum value for `var'.
  Linear_Expression revised_expr;
  PPL_DIRTY_TEMP_COEFFICIENT(d);
  if (corrected_relsym == LESS_THAN || corrected_relsym == LESS_OR_EQUAL) {
    if (bound_below) {
      revised_expr = expr;
      revised_expr.set_inhomogeneous_term(Coefficient_zero());
      revised_expr *= d;
    }
  }
  else {
    if (bound_above) {
      revised_expr = expr;
      revised_expr.set_inhomogeneous_term(Coefficient_zero());
      revised_expr *= max_denom;
    }
  }

  switch (corrected_relsym) {
  case LESS_THAN:
    if (bound_below)
      refine_with_constraint(min_numer < revised_expr);
    break;
  case LESS_OR_EQUAL:
    if (bound_below)
      (min_included)
        ? refine_with_constraint(min_numer <= revised_expr)
        : refine_with_constraint(min_numer < revised_expr);
    break;
  case GREATER_OR_EQUAL:
    if (bound_above)
      (max_included)
        ? refine_with_constraint(max_numer >= revised_expr)
        : refine_with_constraint(max_numer > revised_expr);
    break;
  case GREATER_THAN:
    if (bound_above)
      refine_with_constraint(max_numer > revised_expr);
    break;
  default:
    // The EQUAL and NOT_EQUAL cases have been already dealt with.
    PPL_UNREACHABLE;
    break;
  }
  // If the shrunk box is empty, its preimage is empty too.
  if (is_empty())
    return;
  ITV& seq_v = seq[var.id()];
  seq_v.assign(UNIVERSE);
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>
::generalized_affine_image(const Linear_Expression& lhs,
                           const Relation_Symbol relsym,
                           const Linear_Expression& rhs) {
  // Dimension-compatibility checks.
  // The dimension of `lhs' should not be greater than the dimension
  // of `*this'.
  dimension_type lhs_space_dim = lhs.space_dimension();
  const dimension_type space_dim = space_dimension();
  if (space_dim < lhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2)",
                                 "e1", lhs);
  // The dimension of `rhs' should not be greater than the dimension
  // of `*this'.
  const dimension_type rhs_space_dim = rhs.space_dimension();
  if (space_dim < rhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2)",
                                 "e2", rhs);

  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_image(e1, r, e2)",
                           "r is the disequality relation symbol");

  // Any image of an empty box is empty.
  if (marked_empty())
    return;

  // Compute the maximum and minimum value reached by the rhs on the box.
  PPL_DIRTY_TEMP(Coefficient, max_numer);
  PPL_DIRTY_TEMP(Coefficient, max_denom);
  bool max_included;
  bool max_rhs = maximize(rhs, max_numer, max_denom, max_included);
  PPL_DIRTY_TEMP(Coefficient, min_numer);
  PPL_DIRTY_TEMP(Coefficient, min_denom);
  bool min_included;
  bool min_rhs = minimize(rhs, min_numer, min_denom, min_included);

  // Check whether there is 0, 1 or more than one variable in the lhs
  // and record the variable with the highest dimension; set the box
  // intervals to be unbounded for all other dimensions with non-zero
  // coefficients in the lhs.
  bool has_var = false;
  dimension_type has_var_id = lhs.last_nonzero();

  if (has_var_id != 0) {
    has_var = true;
    --has_var_id;
    dimension_type other_var = lhs.first_nonzero(1, has_var_id + 1);
    --other_var;
    if (other_var != has_var_id) {
      // There is more than one dimension with non-zero coefficient, so
      // we cannot have any information about the dimensions in the lhs.
      ITV& seq_var = seq[has_var_id];
      seq_var.assign(UNIVERSE);
      // Since all but the highest dimension with non-zero coefficient
      // in the lhs have been set unbounded, it remains to set the
      // highest dimension in the lhs unbounded.
      ITV& seq_i = seq[other_var];
      seq_i.assign(UNIVERSE);
      PPL_ASSERT(OK());
      return;
    }
  }

  if (has_var) {
    // There is exactly one dimension with non-zero coefficient.
    ITV& seq_var = seq[has_var_id];

    // Compute the new bounds for this dimension defined by the rhs
    // expression.
    const Coefficient& inhomo = lhs.inhomogeneous_term();
    const Coefficient& coeff = lhs.coefficient(Variable(has_var_id));
    PPL_DIRTY_TEMP(mpq_class, q_max);
    PPL_DIRTY_TEMP(mpq_class, q_min);
    if (max_rhs) {
      max_numer -= inhomo * max_denom;
      max_denom *= coeff;
      assign_r(q_max.get_num(), max_numer, ROUND_NOT_NEEDED);
      assign_r(q_max.get_den(), max_denom, ROUND_NOT_NEEDED);
      q_max.canonicalize();
    }
    if (min_rhs) {
      min_numer -= inhomo * min_denom;
      min_denom *= coeff;
      assign_r(q_min.get_num(), min_numer, ROUND_NOT_NEEDED);
      assign_r(q_min.get_den(), min_denom, ROUND_NOT_NEEDED);
      q_min.canonicalize();
    }

    // The choice as to which bounds should be set depends on the sign
    // of the coefficient of the dimension `has_var_id' in the lhs.
    if (coeff > 0)
      // The coefficient of the dimension in the lhs is positive.
      switch (relsym) {
      case LESS_OR_EQUAL:
        if (max_rhs) {
          Relation_Symbol rel = max_included ? LESS_OR_EQUAL : LESS_THAN;
          seq_var.build(i_constraint(rel, q_max));
        }
        else
          seq_var.assign(UNIVERSE);
        break;
      case LESS_THAN:
        if (max_rhs)
          seq_var.build(i_constraint(LESS_THAN, q_max));
        else
          seq_var.assign(UNIVERSE);
        break;
      case EQUAL:
        {
          I_Constraint<mpq_class> l;
          I_Constraint<mpq_class> u;
          if (max_rhs)
            u.set(max_included ? LESS_OR_EQUAL : LESS_THAN, q_max);
          if (min_rhs)
            l.set(min_included ? GREATER_OR_EQUAL : GREATER_THAN, q_min);
          seq_var.build(l, u);
          break;
        }
      case GREATER_OR_EQUAL:
        if (min_rhs) {
          Relation_Symbol rel = min_included ? GREATER_OR_EQUAL : GREATER_THAN;
          seq_var.build(i_constraint(rel, q_min));
        }
        else
          seq_var.assign(UNIVERSE);
        break;
      case GREATER_THAN:
        if (min_rhs)
          seq_var.build(i_constraint(GREATER_THAN, q_min));
        else
          seq_var.assign(UNIVERSE);
        break;
      default:
        // The NOT_EQUAL case has been already dealt with.
        PPL_UNREACHABLE;
        break;
      }
    else
      // The coefficient of the dimension in the lhs is negative.
      switch (relsym) {
      case GREATER_OR_EQUAL:
        if (min_rhs) {
          Relation_Symbol rel = min_included ? LESS_OR_EQUAL : LESS_THAN;
          seq_var.build(i_constraint(rel, q_min));
        }
        else
          seq_var.assign(UNIVERSE);
        break;
      case GREATER_THAN:
        if (min_rhs)
          seq_var.build(i_constraint(LESS_THAN, q_min));
        else
          seq_var.assign(UNIVERSE);
        break;
      case EQUAL:
        {
          I_Constraint<mpq_class> l;
          I_Constraint<mpq_class> u;
          if (max_rhs)
            l.set(max_included ? GREATER_OR_EQUAL : GREATER_THAN, q_max);
          if (min_rhs)
            u.set(min_included ? LESS_OR_EQUAL : LESS_THAN, q_min);
          seq_var.build(l, u);
          break;
        }
      case LESS_OR_EQUAL:
        if (max_rhs) {
          Relation_Symbol rel = max_included ? GREATER_OR_EQUAL : GREATER_THAN;
          seq_var.build(i_constraint(rel, q_max));
        }
        else
          seq_var.assign(UNIVERSE);
        break;
      case LESS_THAN:
        if (max_rhs)
          seq_var.build(i_constraint(GREATER_THAN, q_max));
        else
          seq_var.assign(UNIVERSE);
        break;
      default:
        // The NOT_EQUAL case has been already dealt with.
        PPL_UNREACHABLE;
        break;
      }
  }

  else {
    // The lhs is a constant value, so we just need to add the
    // appropriate constraint.
    const Coefficient& inhomo = lhs.inhomogeneous_term();
    switch (relsym) {
    case LESS_THAN:
      refine_with_constraint(inhomo < rhs);
      break;
    case LESS_OR_EQUAL:
      refine_with_constraint(inhomo <= rhs);
      break;
    case EQUAL:
      refine_with_constraint(inhomo == rhs);
      break;
    case GREATER_OR_EQUAL:
      refine_with_constraint(inhomo >= rhs);
      break;
    case GREATER_THAN:
      refine_with_constraint(inhomo > rhs);
      break;
    default:
      // The NOT_EQUAL case has been already dealt with.
      PPL_UNREACHABLE;
      break;
    }
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
void
Box<ITV>::generalized_affine_preimage(const Linear_Expression& lhs,
                                      const Relation_Symbol relsym,
                                      const Linear_Expression& rhs) {
  // Dimension-compatibility checks.
  // The dimension of `lhs' should not be greater than the dimension
  // of `*this'.
  dimension_type lhs_space_dim = lhs.space_dimension();
  const dimension_type space_dim = space_dimension();
  if (space_dim < lhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2)",
                                 "e1", lhs);
  // The dimension of `rhs' should not be greater than the dimension
  // of `*this'.
  const dimension_type rhs_space_dim = rhs.space_dimension();
  if (space_dim < rhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2)",
                                 "e2", rhs);

  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_image(e1, r, e2)",
                           "r is the disequality relation symbol");

  // Any image of an empty box is empty.
  if (marked_empty())
    return;

  // For any dimension occurring in the lhs, swap and change the sign
  // of this component for the rhs and lhs.  Then use these in a call
  // to generalized_affine_image/3.
  Linear_Expression revised_lhs = lhs;
  Linear_Expression revised_rhs = rhs;
  for (Linear_Expression::const_iterator i = lhs.begin(),
         i_end = lhs.end(); i != i_end; ++i) {
    const Variable var = i.variable();
    PPL_DIRTY_TEMP_COEFFICIENT(tmp);
    tmp = *i;
    tmp += rhs.coefficient(var);
    sub_mul_assign(revised_rhs, tmp, var);
    sub_mul_assign(revised_lhs, tmp, var);
  }
  generalized_affine_image(revised_lhs, relsym, revised_rhs);
  PPL_ASSERT(OK());
}

template <typename ITV>
template <typename T, typename Iterator>
typename Enable_If<Is_Same<T, Box<ITV> >::value
                   && Is_Same_Or_Derived<Interval_Base, ITV>::value,
                   void>::type
Box<ITV>::CC76_widening_assign(const T& y, Iterator first, Iterator last) {
  if (y.is_empty())
    return;

  for (dimension_type i = seq.size(); i-- > 0; )
    seq[i].CC76_widening_assign(y.seq[i], first, last);

  PPL_ASSERT(OK());
}

template <typename ITV>
template <typename T>
typename Enable_If<Is_Same<T, Box<ITV> >::value
                   && Is_Same_Or_Derived<Interval_Base, ITV>::value,
                   void>::type
Box<ITV>::CC76_widening_assign(const T& y, unsigned* tp) {
  static typename ITV::boundary_type stop_points[] = {
    typename ITV::boundary_type(-2),
    typename ITV::boundary_type(-1),
    typename ITV::boundary_type(0),
    typename ITV::boundary_type(1),
    typename ITV::boundary_type(2)
  };

  Box& x = *this;
  // If there are tokens available, work on a temporary copy.
  if (tp != 0 && *tp > 0) {
    Box<ITV> x_tmp(x);
    x_tmp.CC76_widening_assign(y, 0);
    // If the widening was not precise, use one of the available tokens.
    if (!x.contains(x_tmp))
      --(*tp);
    return;
  }
  x.CC76_widening_assign(y,
                         stop_points,
                         stop_points
                         + sizeof(stop_points)/sizeof(stop_points[0]));
}

template <typename ITV>
void
Box<ITV>::get_limiting_box(const Constraint_System& cs,
                           Box& limiting_box) const {
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(cs.space_dimension() <= space_dimension());

  for (Constraint_System::const_iterator cs_i = cs.begin(),
         cs_end = cs.end(); cs_i != cs_end; ++cs_i) {
    const Constraint& c = *cs_i;
    dimension_type c_num_vars = 0;
    dimension_type c_only_var = 0;
    // Constraints that are not interval constraints are ignored.
    if (!Box_Helpers::extract_interval_constraint(c, c_num_vars, c_only_var))
      continue;
    // Trivial constraints are ignored.
    if (c_num_vars != 0) {
      // c is a non-trivial interval constraint.
      // add interval constraint to limiting box
      const Coefficient& n = c.inhomogeneous_term();
      const Coefficient& d = c.coefficient(Variable(c_only_var));
      if (interval_relation(seq[c_only_var], c.type(), n, d)
          == Poly_Con_Relation::is_included())
        limiting_box.add_interval_constraint_no_check(c_only_var, c.type(),
                                                      n, d);
    }
  }
}

template <typename ITV>
void
Box<ITV>::limited_CC76_extrapolation_assign(const Box& y,
                                            const Constraint_System& cs,
                                            unsigned* tp) {
  Box& x = *this;
  const dimension_type space_dim = x.space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("limited_CC76_extrapolation_assign(y, cs)",
                                 y);
  // `cs' must be dimension-compatible with the two boxes.
  const dimension_type cs_space_dim = cs.space_dimension();
  if (space_dim < cs_space_dim)
    throw_constraint_incompatible("limited_CC76_extrapolation_assign(y, cs)");

  // The limited CC76-extrapolation between two boxes in a
  // zero-dimensional space is also a zero-dimensional box
  if (space_dim == 0)
    return;

  // Assume `y' is contained in or equal to `*this'.
  PPL_EXPECT_HEAVY(copy_contains(*this, y));

  // If `*this' is empty, since `*this' contains `y', `y' is empty too.
  if (marked_empty())
    return;
  // If `y' is empty, we return.
  if (y.marked_empty())
    return;

  // Build a limiting box using all the constraints in cs
  // that are satisfied by *this.
  Box limiting_box(space_dim, UNIVERSE);
  get_limiting_box(cs, limiting_box);

  x.CC76_widening_assign(y, tp);

  // Intersect the widened box with the limiting box.
  intersection_assign(limiting_box);
}

template <typename ITV>
template <typename T>
typename Enable_If<Is_Same<T, Box<ITV> >::value
                   && Is_Same_Or_Derived<Interval_Base, ITV>::value,
                   void>::type
Box<ITV>::CC76_narrowing_assign(const T& y) {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("CC76_narrowing_assign(y)", y);

  // Assume `*this' is contained in or equal to `y'.
  PPL_EXPECT_HEAVY(copy_contains(y, *this));

  // If both boxes are zero-dimensional,
  // since `y' contains `*this', we simply return `*this'.
  if (space_dim == 0)
    return;

  // If `y' is empty, since `y' contains `this', `*this' is empty too.
  if (y.is_empty())
    return;
  // If `*this' is empty, we return.
  if (is_empty())
    return;

  // Replace each constraint in `*this' by the corresponding constraint
  // in `y' if the corresponding inhomogeneous terms are both finite.
  for (dimension_type i = space_dim; i-- > 0; ) {
    ITV& x_i = seq[i];
    const ITV& y_i = y.seq[i];
    if (!x_i.lower_is_boundary_infinity()
        && !y_i.lower_is_boundary_infinity()
        && x_i.lower() != y_i.lower())
      x_i.lower() = y_i.lower();
    if (!x_i.upper_is_boundary_infinity()
        && !y_i.upper_is_boundary_infinity()
        && x_i.upper() != y_i.upper())
      x_i.upper() = y_i.upper();
  }
  PPL_ASSERT(OK());
}

template <typename ITV>
Constraint_System
Box<ITV>::constraints() const {
  const dimension_type space_dim = space_dimension();
  Constraint_System cs;
  cs.set_space_dimension(space_dim);

  if (space_dim == 0) {
    if (marked_empty())
      cs = Constraint_System::zero_dim_empty();
    return cs;
  }

  if (marked_empty()) {
    cs.insert(Constraint::zero_dim_false());
    return cs;
  }

  for (dimension_type k = 0; k < space_dim; ++k) {
    const Variable v_k = Variable(k);
    PPL_DIRTY_TEMP(Coefficient, n);
    PPL_DIRTY_TEMP(Coefficient, d);
    bool closed = false;
    if (has_lower_bound(v_k, n, d, closed)) {
      if (closed)
        cs.insert(d * v_k >= n);
      else
        cs.insert(d * v_k > n);
    }
    if (has_upper_bound(v_k, n, d, closed)) {
      if (closed)
        cs.insert(d * v_k <= n);
      else
        cs.insert(d * v_k < n);
    }
  }
  return cs;
}

template <typename ITV>
Constraint_System
Box<ITV>::minimized_constraints() const {
  const dimension_type space_dim = space_dimension();
  Constraint_System cs;
  cs.set_space_dimension(space_dim);

  if (space_dim == 0) {
    if (marked_empty())
      cs = Constraint_System::zero_dim_empty();
    return cs;
  }

  // Make sure emptiness is detected.
  if (is_empty()) {
    cs.insert(Constraint::zero_dim_false());
    return cs;
  }

  for (dimension_type k = 0; k < space_dim; ++k) {
    const Variable v_k = Variable(k);
    PPL_DIRTY_TEMP(Coefficient, n);
    PPL_DIRTY_TEMP(Coefficient, d);
    bool closed = false;
    if (has_lower_bound(v_k, n, d, closed)) {
      if (closed)
        // Make sure equality constraints are detected.
        if (seq[k].is_singleton()) {
          cs.insert(d * v_k == n);
          continue;
        }
        else
          cs.insert(d * v_k >= n);
      else
        cs.insert(d * v_k > n);
    }
    if (has_upper_bound(v_k, n, d, closed)) {
      if (closed)
        cs.insert(d * v_k <= n);
      else
        cs.insert(d * v_k < n);
    }
  }
  return cs;
}

template <typename ITV>
Congruence_System
Box<ITV>::congruences() const {
  const dimension_type space_dim = space_dimension();
  Congruence_System cgs(space_dim);

  if (space_dim == 0) {
    if (marked_empty())
      cgs = Congruence_System::zero_dim_empty();
    return cgs;
  }

  // Make sure emptiness is detected.
  if (is_empty()) {
    cgs.insert(Congruence::zero_dim_false());
    return cgs;
  }

  for (dimension_type k = 0; k < space_dim; ++k) {
    const Variable v_k = Variable(k);
    PPL_DIRTY_TEMP(Coefficient, n);
    PPL_DIRTY_TEMP(Coefficient, d);
    bool closed = false;
    if (has_lower_bound(v_k, n, d, closed) && closed)
      // Make sure equality congruences are detected.
      if (seq[k].is_singleton())
        cgs.insert((d * v_k %= n) / 0);
  }
  return cgs;
}

template <typename ITV>
memory_size_type
Box<ITV>::external_memory_in_bytes() const {
  memory_size_type n = seq.capacity() * sizeof(ITV);
  for (dimension_type k = seq.size(); k-- > 0; )
    n += seq[k].external_memory_in_bytes();
  return n;
}

/*! \relates Parma_Polyhedra_Library::Box */
template <typename ITV>
std::ostream&
IO_Operators::operator<<(std::ostream& s, const Box<ITV>& box) {
  if (box.is_empty())
    s << "false";
  else if (box.is_universe())
    s << "true";
  else
    for (dimension_type k = 0,
           space_dim = box.space_dimension(); k < space_dim; ) {
      s << Variable(k) << " in " << box[k];
      ++k;
      if (k < space_dim)
        s << ", ";
      else
        break;
    }
  return s;
}

template <typename ITV>
void
Box<ITV>::ascii_dump(std::ostream& s) const {
  const char separator = ' ';
  status.ascii_dump(s);
  const dimension_type space_dim = space_dimension();
  s << "space_dim" << separator << space_dim;
  s << "\n";
  for (dimension_type i = 0; i < space_dim;  ++i)
    seq[i].ascii_dump(s);
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS(ITV, Box<ITV>)

template <typename ITV>
bool
Box<ITV>::ascii_load(std::istream& s) {
  if (!status.ascii_load(s))
    return false;

  std::string str;
  dimension_type space_dim;
  if (!(s >> str) || str != "space_dim")
    return false;
  if (!(s >> space_dim))
    return false;

  seq.clear();
  ITV seq_i;
  for (dimension_type i = 0; i < space_dim;  ++i) {
    if (seq_i.ascii_load(s))
      seq.push_back(seq_i);
    else
      return false;
  }

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

template <typename ITV>
void
Box<ITV>::throw_dimension_incompatible(const char* method,
                                       const Box& y) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "this->space_dimension() == " << this->space_dimension()
    << ", y->space_dimension() == " << y.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>
::throw_dimension_incompatible(const char* method,
                               dimension_type required_dim) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", required dimension == " << required_dim << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_dimension_incompatible(const char* method,
                                       const Constraint& c) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", c->space_dimension == " << c.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_dimension_incompatible(const char* method,
                                       const Congruence& cg) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", cg->space_dimension == " << cg.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_dimension_incompatible(const char* method,
                                       const Constraint_System& cs) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", cs->space_dimension == " << cs.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_dimension_incompatible(const char* method,
                                       const Congruence_System& cgs) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", cgs->space_dimension == " << cgs.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_dimension_incompatible(const char* method,
                                       const Generator& g) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", g->space_dimension == " << g.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_constraint_incompatible(const char* method) {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "the constraint is incompatible.";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_expression_too_complex(const char* method,
                                       const Linear_Expression& le) {
  using namespace IO_Operators;
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << le << " is too complex.";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_dimension_incompatible(const char* method,
                                       const char* le_name,
                                       const Linear_Expression& le) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", " << le_name << "->space_dimension() == "
    << le.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
template <typename C>
void
Box<ITV>::throw_dimension_incompatible(const char* method,
                                       const char* lf_name,
                                       const Linear_Form<C>& lf) const {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension()
    << ", " << lf_name << "->space_dimension() == "
    << lf.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename ITV>
void
Box<ITV>::throw_invalid_argument(const char* method, const char* reason) {
  std::ostringstream s;
  s << "PPL::Box::" << method << ":" << std::endl
    << reason;
  throw std::invalid_argument(s.str());
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Box */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
template <typename Specialization,
          typename Temp, typename To, typename ITV>
bool
l_m_distance_assign(Checked_Number<To, Extended_Number_Policy>& r,
                    const Box<ITV>& x, const Box<ITV>& y,
                    const Rounding_Dir dir,
                    Temp& tmp0, Temp& tmp1, Temp& tmp2) {
  const dimension_type x_space_dim = x.space_dimension();
  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    return false;

  // Zero-dim boxes are equal if and only if they are both empty or universe.
  if (x_space_dim == 0) {
    if (x.marked_empty() == y.marked_empty())
      assign_r(r, 0, ROUND_NOT_NEEDED);
    else
      assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
    return true;
  }

  // The distance computation requires a check for emptiness.
  (void) x.is_empty();
  (void) y.is_empty();
  // If one of two boxes is empty, then they are equal if and only if
  // the other box is empty too.
  if (x.marked_empty() || y.marked_empty()) {
    if (x.marked_empty() == y.marked_empty()) {
      assign_r(r, 0, ROUND_NOT_NEEDED);
      return true;
    }
    else
      goto pinf;
  }

  assign_r(tmp0, 0, ROUND_NOT_NEEDED);
  for (dimension_type i = x_space_dim; i-- > 0; ) {
    const ITV& x_i = x.seq[i];
    const ITV& y_i = y.seq[i];
    // Dealing with the lower bounds.
    if (x_i.lower_is_boundary_infinity()) {
      if (!y_i.lower_is_boundary_infinity())
        goto pinf;
    }
    else if (y_i.lower_is_boundary_infinity())
      goto pinf;
    else {
      const Temp* tmp1p;
      const Temp* tmp2p;
      if (x_i.lower() > y_i.lower()) {
        maybe_assign(tmp1p, tmp1, x_i.lower(), dir);
        maybe_assign(tmp2p, tmp2, y_i.lower(), inverse(dir));
      }
      else {
        maybe_assign(tmp1p, tmp1, y_i.lower(), dir);
        maybe_assign(tmp2p, tmp2, x_i.lower(), inverse(dir));
      }
      sub_assign_r(tmp1, *tmp1p, *tmp2p, dir);
      PPL_ASSERT(sgn(tmp1) >= 0);
      Specialization::combine(tmp0, tmp1, dir);
    }
    // Dealing with the lower bounds.
    if (x_i.upper_is_boundary_infinity())
      if (y_i.upper_is_boundary_infinity())
        continue;
      else
        goto pinf;
    else if (y_i.upper_is_boundary_infinity())
      goto pinf;
    else {
      const Temp* tmp1p;
      const Temp* tmp2p;
      if (x_i.upper() > y_i.upper()) {
        maybe_assign(tmp1p, tmp1, x_i.upper(), dir);
        maybe_assign(tmp2p, tmp2, y_i.upper(), inverse(dir));
      }
      else {
        maybe_assign(tmp1p, tmp1, y_i.upper(), dir);
        maybe_assign(tmp2p, tmp2, x_i.upper(), inverse(dir));
      }
      sub_assign_r(tmp1, *tmp1p, *tmp2p, dir);
      PPL_ASSERT(sgn(tmp1) >= 0);
      Specialization::combine(tmp0, tmp1, dir);
    }
  }
  Specialization::finalize(tmp0, dir);
  assign_r(r, tmp0, dir);
  return true;

 pinf:
  assign_r(r, PLUS_INFINITY, ROUND_NOT_NEEDED);
  return true;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Box_templates_hh)
