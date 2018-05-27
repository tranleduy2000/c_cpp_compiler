/* BD_Shape class implementation: non-inline template functions.
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

#ifndef PPL_BD_Shape_templates_hh
#define PPL_BD_Shape_templates_hh 1

#include "Generator_System_defs.hh"
#include "Generator_System_inlines.hh"
#include "Congruence_System_inlines.hh"
#include "Congruence_System_defs.hh"
#include "Interval_defs.hh"
#include "Linear_Form_defs.hh"
#include "Poly_Con_Relation_defs.hh"
#include "Poly_Gen_Relation_defs.hh"
#include "MIP_Problem_defs.hh"
#include "Variables_Set_defs.hh"
#include "Bit_Row_defs.hh"
#include "Temp_defs.hh"
#include "assert.hh"
#include <vector>
#include <deque>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace Parma_Polyhedra_Library {

template <typename T>
BD_Shape<T>::BD_Shape(const Congruence_System& cgs)
  : dbm(cgs.space_dimension() + 1),
    status(),
    redundancy_dbm() {
  add_congruences(cgs);
}

template <typename T>
BD_Shape<T>::BD_Shape(const Generator_System& gs)
  : dbm(gs.space_dimension() + 1), status(), redundancy_dbm() {
  const Generator_System::const_iterator gs_begin = gs.begin();
  const Generator_System::const_iterator gs_end = gs.end();
  if (gs_begin == gs_end) {
    // An empty generator system defines the empty BD shape.
    set_empty();
    return;
  }

  const dimension_type space_dim = space_dimension();
  DB_Row<N>& dbm_0 = dbm[0];
  PPL_DIRTY_TEMP(N, tmp);

  bool dbm_initialized = false;
  bool point_seen = false;
  // Going through all the points and closure points.
  for (Generator_System::const_iterator gs_i = gs_begin;
       gs_i != gs_end; ++gs_i) {
    const Generator& g = *gs_i;
    switch (g.type()) {
    case Generator::POINT:
      point_seen = true;
      // Intentionally fall through.
    case Generator::CLOSURE_POINT:
      if (!dbm_initialized) {
        // When handling the first (closure) point, we initialize the DBM.
        dbm_initialized = true;
        const Coefficient& d = g.divisor();
        // TODO: Check if the following loop can be optimized used
        // Generator::expr_type::const_iterator.
        for (dimension_type i = space_dim; i > 0; --i) {
          const Coefficient& g_i = g.expression().get(Variable(i - 1));
          DB_Row<N>& dbm_i = dbm[i];
          for (dimension_type j = space_dim; j > 0; --j)
            if (i != j) {
              const Coefficient& g_j = g.expression().get(Variable(j - 1));
              div_round_up(dbm_i[j], g_j - g_i, d);
            }
          div_round_up(dbm_i[0], -g_i, d);
        }
        for (dimension_type j = space_dim; j > 0; --j) {
          const Coefficient& g_j = g.expression().get(Variable(j - 1));
          div_round_up(dbm_0[j], g_j, d);
        }
        // Note: no need to initialize the first element of the main diagonal.
      }
      else {
        // This is not the first point: the DBM already contains
        // valid values and we must compute maxima.
        const Coefficient& d = g.divisor();
        // TODO: Check if the following loop can be optimized used
        // Generator::expr_type::const_iterator.
        for (dimension_type i = space_dim; i > 0; --i) {
          const Coefficient& g_i = g.expression().get(Variable(i - 1));
          DB_Row<N>& dbm_i = dbm[i];
          // The loop correctly handles the case when i == j.
          for (dimension_type j = space_dim; j > 0; --j) {
            const Coefficient& g_j = g.expression().get(Variable(j - 1));
            div_round_up(tmp, g_j - g_i, d);
            max_assign(dbm_i[j], tmp);
          }
          div_round_up(tmp, -g_i, d);
          max_assign(dbm_i[0], tmp);
        }
        for (dimension_type j = space_dim; j > 0; --j) {
          const Coefficient& g_j = g.expression().get(Variable(j - 1));
          div_round_up(tmp, g_j, d);
          max_assign(dbm_0[j], tmp);
        }
      }
      break;
    default:
      // Lines and rays temporarily ignored.
      break;
    }
  }

  if (!point_seen)
    // The generator system is not empty, but contains no points.
    throw_invalid_argument("BD_Shape(gs)",
                           "the non-empty generator system gs "
                           "contains no points.");

  // Going through all the lines and rays.
  for (Generator_System::const_iterator gs_i = gs_begin;
       gs_i != gs_end; ++gs_i) {
    const Generator& g = *gs_i;
    switch (g.type()) {
    case Generator::LINE:
      // TODO: Check if the following loop can be optimized used
      // Generator::expr_type::const_iterator.
      for (dimension_type i = space_dim; i > 0; --i) {
        const Coefficient& g_i = g.expression().get(Variable(i - 1));
        DB_Row<N>& dbm_i = dbm[i];
        // The loop correctly handles the case when i == j.
        for (dimension_type j = space_dim; j > 0; --j)
          if (g_i != g.expression().get(Variable(j - 1)))
            assign_r(dbm_i[j], PLUS_INFINITY, ROUND_NOT_NEEDED);
        if (g_i != 0)
          assign_r(dbm_i[0], PLUS_INFINITY, ROUND_NOT_NEEDED);
      }
      for (Generator::expr_type::const_iterator i = g.expression().begin(),
            i_end = g.expression().end(); i != i_end; ++i)
        assign_r(dbm_0[i.variable().space_dimension()],
                 PLUS_INFINITY, ROUND_NOT_NEEDED);
      break;
    case Generator::RAY:
      // TODO: Check if the following loop can be optimized used
      // Generator::expr_type::const_iterator.
      for (dimension_type i = space_dim; i > 0; --i) {
        const Coefficient& g_i = g.expression().get(Variable(i - 1));
        DB_Row<N>& dbm_i = dbm[i];
        // The loop correctly handles the case when i == j.
        for (dimension_type j = space_dim; j > 0; --j)
          if (g_i < g.expression().get(Variable(j - 1)))
            assign_r(dbm_i[j], PLUS_INFINITY, ROUND_NOT_NEEDED);
        if (g_i < 0)
          assign_r(dbm_i[0], PLUS_INFINITY, ROUND_NOT_NEEDED);
      }
      for (Generator::expr_type::const_iterator i = g.expression().begin(),
            i_end = g.expression().end(); i != i_end; ++i)
        if (*i > 0)
          assign_r(dbm_0[i.variable().space_dimension()],
                   PLUS_INFINITY, ROUND_NOT_NEEDED);
      break;
    default:
      // Points and closure points already dealt with.
      break;
    }
  }
  set_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
BD_Shape<T>::BD_Shape(const Polyhedron& ph, const Complexity_Class complexity)
  : dbm(), status(), redundancy_dbm() {
  const dimension_type num_dimensions = ph.space_dimension();

  if (ph.marked_empty()) {
    *this = BD_Shape<T>(num_dimensions, EMPTY);
    return;
  }

  if (num_dimensions == 0) {
    *this = BD_Shape<T>(num_dimensions, UNIVERSE);
    return;
  }

  // Build from generators when we do not care about complexity
  // or when the process has polynomial complexity.
  if (complexity == ANY_COMPLEXITY
      || (!ph.has_pending_constraints() && ph.generators_are_up_to_date())) {
    *this = BD_Shape<T>(ph.generators());
    return;
  }

  // We cannot afford exponential complexity, we do not have a complete set
  // of generators for the polyhedron, and the polyhedron is not trivially
  // empty or zero-dimensional.  Constraints, however, are up to date.
  PPL_ASSERT(ph.constraints_are_up_to_date());

  if (!ph.has_something_pending() && ph.constraints_are_minimized()) {
    // If the constraint system of the polyhedron is minimized,
    // the test `is_universe()' has polynomial complexity.
    if (ph.is_universe()) {
      *this = BD_Shape<T>(num_dimensions, UNIVERSE);
      return;
    }
  }

  // See if there is at least one inconsistent constraint in `ph.con_sys'.
  for (Constraint_System::const_iterator i = ph.con_sys.begin(),
         cs_end = ph.con_sys.end(); i != cs_end; ++i)
    if (i->is_inconsistent()) {
      *this = BD_Shape<T>(num_dimensions, EMPTY);
      return;
    }

  // If `complexity' allows it, use simplex to derive the exact (modulo
  // the fact that our BDSs are topologically closed) variable bounds.
  if (complexity == SIMPLEX_COMPLEXITY) {
    MIP_Problem lp(num_dimensions);
    lp.set_optimization_mode(MAXIMIZATION);

    const Constraint_System& ph_cs = ph.constraints();
    if (!ph_cs.has_strict_inequalities())
      lp.add_constraints(ph_cs);
    else
      // Adding to `lp' a topologically closed version of `ph_cs'.
      for (Constraint_System::const_iterator i = ph_cs.begin(),
             ph_cs_end = ph_cs.end(); i != ph_cs_end; ++i) {
        const Constraint& c = *i;
        if (c.is_strict_inequality()) {
          Linear_Expression expr(c.expression());
          lp.add_constraint(expr >= 0);
        }
        else
          lp.add_constraint(c);
      }

    // Check for unsatisfiability.
    if (!lp.is_satisfiable()) {
      *this = BD_Shape<T>(num_dimensions, EMPTY);
      return;
    }

    // Start with a universe BDS that will be refined by the simplex.
    *this = BD_Shape<T>(num_dimensions, UNIVERSE);
    // Get all the upper bounds.
    Generator g(point());
    PPL_DIRTY_TEMP_COEFFICIENT(numer);
    PPL_DIRTY_TEMP_COEFFICIENT(denom);
    for (dimension_type i = 1; i <= num_dimensions; ++i) {
      Variable x(i-1);
      // Evaluate optimal upper bound for `x <= ub'.
      lp.set_objective_function(x);
      if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
        g = lp.optimizing_point();
        lp.evaluate_objective_function(g, numer, denom);
        div_round_up(dbm[0][i], numer, denom);
      }
      // Evaluate optimal upper bound for `x - y <= ub'.
      for (dimension_type j = 1; j <= num_dimensions; ++j) {
        if (i == j)
          continue;
        Variable y(j-1);
        lp.set_objective_function(x - y);
        if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
          g = lp.optimizing_point();
          lp.evaluate_objective_function(g, numer, denom);
          div_round_up(dbm[j][i], numer, denom);
        }
      }
      // Evaluate optimal upper bound for `-x <= ub'.
      lp.set_objective_function(-x);
      if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
        g = lp.optimizing_point();
        lp.evaluate_objective_function(g, numer, denom);
        div_round_up(dbm[i][0], numer, denom);
      }
    }
    set_shortest_path_closed();
    PPL_ASSERT(OK());
    return;
  }

  // Extract easy-to-find bounds from constraints.
  PPL_ASSERT(complexity == POLYNOMIAL_COMPLEXITY);
  *this = BD_Shape<T>(num_dimensions, UNIVERSE);
  refine_with_constraints(ph.constraints());
}

template <typename T>
dimension_type
BD_Shape<T>::affine_dimension() const {
  const dimension_type space_dim = space_dimension();
  // A zero-space-dim shape always has affine dimension zero.
  if (space_dim == 0)
    return 0;

  // Shortest-path closure is necessary to detect emptiness
  // and all (possibly implicit) equalities.
  shortest_path_closure_assign();
  if (marked_empty())
    return 0;

  // The vector `predecessor' is used to represent equivalence classes:
  // `predecessor[i] == i' if and only if `i' is the leader of its
  // equivalence class (i.e., the minimum index in the class).
  std::vector<dimension_type> predecessor;
  compute_predecessors(predecessor);

  // Due to the fictitious variable `0', the affine dimension is one
  // less the number of equivalence classes.
  dimension_type affine_dim = 0;
  // Note: disregard the first equivalence class.
  for (dimension_type i = 1; i <= space_dim; ++i)
    if (predecessor[i] == i)
      ++affine_dim;

  return affine_dim;
}

template <typename T>
Congruence_System
BD_Shape<T>::minimized_congruences() const {
  // Shortest-path closure is necessary to detect emptiness
  // and all (possibly implicit) equalities.
  shortest_path_closure_assign();

  const dimension_type space_dim = space_dimension();
  Congruence_System cgs(space_dim);

  if (space_dim == 0) {
    if (marked_empty())
      cgs = Congruence_System::zero_dim_empty();
    return cgs;
  }

  if (marked_empty()) {
    cgs.insert(Congruence::zero_dim_false());
    return cgs;
  }

  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);

  // Compute leader information.
  std::vector<dimension_type> leaders;
  compute_leaders(leaders);

  // Go through the non-leaders to generate equality constraints.
  const DB_Row<N>& dbm_0 = dbm[0];
  for (dimension_type i = 1; i <= space_dim; ++i) {
    const dimension_type leader = leaders[i];
    if (i != leader) {
      // Generate the constraint relating `i' and its leader.
      if (leader == 0) {
        // A unary equality has to be generated.
        PPL_ASSERT(!is_plus_infinity(dbm_0[i]));
        numer_denom(dbm_0[i], numer, denom);
        cgs.insert(denom*Variable(i-1) == numer);
      }
      else {
        // A binary equality has to be generated.
        PPL_ASSERT(!is_plus_infinity(dbm[i][leader]));
        numer_denom(dbm[i][leader], numer, denom);
        cgs.insert(denom*Variable(leader-1) - denom*Variable(i-1) == numer);
      }
    }
  }
  return cgs;
}

template <typename T>
void
BD_Shape<T>::add_constraint(const Constraint& c) {
  // Dimension-compatibility check.
  if (c.space_dimension() > space_dimension())
    throw_dimension_incompatible("add_constraint(c)", c);

  // Get rid of strict inequalities.
  if (c.is_strict_inequality()) {
    if (c.is_inconsistent()) {
      set_empty();
      return;
    }
    if (c.is_tautological())
      return;
    // Nontrivial strict inequalities are not allowed.
    throw_invalid_argument("add_constraint(c)",
                           "strict inequalities are not allowed");
  }

  dimension_type num_vars = 0;
  dimension_type i = 0;
  dimension_type j = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  // Constraints that are not bounded differences are not allowed.
  if (!BD_Shape_Helpers::extract_bounded_difference(c, num_vars, i, j, coeff))
    throw_invalid_argument("add_constraint(c)",
                           "c is not a bounded difference constraint");

  const Coefficient& inhomo = c.inhomogeneous_term();
  if (num_vars == 0) {
    // Dealing with a trivial constraint (not a strict inequality).
    if (inhomo < 0
        || (inhomo != 0 && c.is_equality()))
      set_empty();
    return;
  }

  // Select the cell to be modified for the "<=" part of the constraint,
  // and set `coeff' to the absolute value of itself.
  const bool negative = (coeff < 0);
  if (negative)
    neg_assign(coeff);

  bool changed = false;
  N& x = negative ? dbm[i][j] : dbm[j][i];
  // Compute the bound for `x', rounding towards plus infinity.
  PPL_DIRTY_TEMP(N, d);
  div_round_up(d, inhomo, coeff);
  if (x > d) {
    x = d;
    changed = true;
  }

  if (c.is_equality()) {
    N& y = negative ? dbm[j][i] : dbm[i][j];
    // Also compute the bound for `y', rounding towards plus infinity.
    PPL_DIRTY_TEMP_COEFFICIENT(minus_c_term);
    neg_assign(minus_c_term, inhomo);
    div_round_up(d, minus_c_term, coeff);
    if (y > d) {
      y = d;
      changed = true;
    }
  }

  // In general, adding a constraint does not preserve the shortest-path
  // closure or reduction of the bounded difference shape.
  if (changed && marked_shortest_path_closed())
    reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::add_congruence(const Congruence& cg) {
  const dimension_type cg_space_dim = cg.space_dimension();
  // Dimension-compatibility check:
  // the dimension of `cg' can not be greater than space_dim.
  if (space_dimension() < cg_space_dim)
    throw_dimension_incompatible("add_congruence(cg)", cg);

  // Handle the case of proper congruences first.
  if (cg.is_proper_congruence()) {
    if (cg.is_tautological())
      return;
    if (cg.is_inconsistent()) {
      set_empty();
      return;
    }
    // Non-trivial and proper congruences are not allowed.
    throw_invalid_argument("add_congruence(cg)",
                           "cg is a non-trivial, proper congruence");
  }

  PPL_ASSERT(cg.is_equality());
  Constraint c(cg);
  add_constraint(c);
}

template <typename T>
void
BD_Shape<T>::refine_no_check(const Constraint& c) {
  PPL_ASSERT(!marked_empty());
  PPL_ASSERT(c.space_dimension() <= space_dimension());

  dimension_type num_vars = 0;
  dimension_type i = 0;
  dimension_type j = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  // Constraints that are not bounded differences are ignored.
  if (!BD_Shape_Helpers::extract_bounded_difference(c, num_vars, i, j, coeff))
    return;

  const Coefficient& inhomo = c.inhomogeneous_term();
  if (num_vars == 0) {
    // Dealing with a trivial constraint (might be a strict inequality).
    if (inhomo < 0
        || (c.is_equality() && inhomo != 0)
        || (c.is_strict_inequality() && inhomo == 0))
      set_empty();
    return;
  }

  // Select the cell to be modified for the "<=" part of the constraint,
  // and set `coeff' to the absolute value of itself.
  const bool negative = (coeff < 0);
  N& x = negative ? dbm[i][j] : dbm[j][i];
  N& y = negative ? dbm[j][i] : dbm[i][j];
  if (negative)
    neg_assign(coeff);

  bool changed = false;
  // Compute the bound for `x', rounding towards plus infinity.
  PPL_DIRTY_TEMP(N, d);
  div_round_up(d, inhomo, coeff);
  if (x > d) {
    x = d;
    changed = true;
  }

  if (c.is_equality()) {
    // Also compute the bound for `y', rounding towards plus infinity.
    PPL_DIRTY_TEMP_COEFFICIENT(minus_c_term);
    neg_assign(minus_c_term, inhomo);
    div_round_up(d, minus_c_term, coeff);
    if (y > d) {
      y = d;
      changed = true;
    }
  }

  // In general, adding a constraint does not preserve the shortest-path
  // closure or reduction of the bounded difference shape.
  if (changed && marked_shortest_path_closed())
    reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::concatenate_assign(const BD_Shape& y) {
  BD_Shape& x = *this;

  const dimension_type x_space_dim = x.space_dimension();
  const dimension_type y_space_dim = y.space_dimension();

  // If `y' is an empty 0-dim space bounded difference shape,
  // let `*this' become empty.
  if (y_space_dim == 0 && y.marked_empty()) {
    set_empty();
    return;
  }

  // If `x' is an empty 0-dim space BDS, then it is sufficient to adjust
  // the dimension of the vector space.
  if (x_space_dim == 0 && marked_empty()) {
    dbm.grow(y_space_dim + 1);
    PPL_ASSERT(OK());
    return;
  }
  // First we increase the space dimension of `x' by adding
  // `y.space_dimension()' new dimensions.
  // The matrix for the new system of constraints is obtained
  // by leaving the old system of constraints in the upper left-hand side
  // and placing the constraints of `y' in the lower right-hand side,
  // except the constraints as `y(i) >= cost' or `y(i) <= cost', that are
  // placed in the right position on the new matrix.
  add_space_dimensions_and_embed(y_space_dim);
  const dimension_type new_space_dim = x_space_dim + y_space_dim;
  for (dimension_type i = x_space_dim + 1; i <= new_space_dim; ++i) {
    DB_Row<N>& dbm_i = dbm[i];
    dbm_i[0] = y.dbm[i - x_space_dim][0];
    dbm[0][i] = y.dbm[0][i - x_space_dim];
    for (dimension_type j = x_space_dim + 1; j <= new_space_dim; ++j)
      dbm_i[j] = y.dbm[i - x_space_dim][j - x_space_dim];
  }

  if (marked_shortest_path_closed())
    reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
bool
BD_Shape<T>::contains(const BD_Shape& y) const {
  const BD_Shape<T>& x = *this;
  const dimension_type x_space_dim = x.space_dimension();

  // Dimension-compatibility check.
  if (x_space_dim != y.space_dimension())
    throw_dimension_incompatible("contains(y)", y);

  if (x_space_dim == 0) {
    // The zero-dimensional empty shape only contains another
    // zero-dimensional empty shape.
    // The zero-dimensional universe shape contains any other
    // zero-dimensional shape.
    return marked_empty() ? y.marked_empty() : true;
  }

  /*
    The `y' bounded difference shape must be closed.  As an example,
    consider the case where in `*this' we have the constraints

    x1 - x2 <= 1,
    x1      <= 3,
    x2      <= 2,

    and in `y' the constraints are

    x1 - x2 <= 0,
    x2      <= 1.

    Without closure the (erroneous) analysis of the inhomogeneous terms
    would conclude containment does not hold.  Closing `y' results into
    the "discovery" of the implicit constraint

    x1      <= 1,

    at which point the inhomogeneous terms can be examined to determine
    that containment does hold.
  */
  y.shortest_path_closure_assign();
  // An empty shape is contained in any other dimension-compatible shapes.
  if (y.marked_empty())
    return true;

  // If `x' is empty it can not contain `y' (which is not empty).
  if (x.is_empty())
    return false;

  // `*this' contains `y' if and only if every cell of `dbm'
  // is greater than or equal to the correspondent one of `y.dbm'.
  for (dimension_type i = x_space_dim + 1; i-- > 0; ) {
    const DB_Row<N>& x_dbm_i = x.dbm[i];
    const DB_Row<N>& y_dbm_i = y.dbm[i];
    for (dimension_type j = x_space_dim + 1; j-- > 0; )
      if (x_dbm_i[j] < y_dbm_i[j])
        return false;
  }
  return true;
}

template <typename T>
bool
BD_Shape<T>::is_disjoint_from(const BD_Shape& y) const {
  const dimension_type space_dim = space_dimension();
  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("is_disjoint_from(y)", y);

  // If one of the two bounded difference shape is empty,
  // then the two bounded difference shape are disjoint.
  shortest_path_closure_assign();
  if (marked_empty())
    return true;
  y.shortest_path_closure_assign();
  if (y.marked_empty())
    return true;

  // Two BDSs are disjoint when their intersection is empty.
  // That is if and only if there exists at least a bounded difference
  // such that the upper bound of the bounded difference in the first
  // BD_Shape is strictly less than the lower bound of
  // the corresponding bounded difference in the second BD_Shape
  // or vice versa.
  // For example: let be
  // in `*this':    -a_j_i <= v_j - v_i <= a_i_j;
  // and in `y':    -b_j_i <= v_j - v_i <= b_i_j;
  // `*this' and `y' are disjoint if
  // 1.) a_i_j < -b_j_i or
  // 2.) b_i_j < -a_j_i.
  PPL_DIRTY_TEMP(N, tmp);
  for (dimension_type i = space_dim+1; i-- > 0; ) {
    const DB_Row<N>& x_i = dbm[i];
    for (dimension_type j = space_dim+1; j-- > 0; ) {
      neg_assign_r(tmp, y.dbm[j][i], ROUND_UP);
      if (x_i[j] < tmp)
        return true;
    }
  }

  return false;
}

template <typename T>
bool
BD_Shape<T>::is_universe() const {
  if (marked_empty())
    return false;

  const dimension_type space_dim = space_dimension();
  // If the BDS is non-empty and zero-dimensional,
  // then it is necessarily the universe BDS.
  if (space_dim == 0)
    return true;

  // A bounded difference shape defining the universe BDS can only
  // contain trivial constraints.
  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    const DB_Row<N>& dbm_i = dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; )
      if (!is_plus_infinity(dbm_i[j]))
        return false;
  }
  return true;
}

template <typename T>
bool
BD_Shape<T>::is_bounded() const {
  shortest_path_closure_assign();
  const dimension_type space_dim = space_dimension();
  // A zero-dimensional or empty BDS is bounded.
  if (marked_empty() || space_dim == 0)
    return true;

  // A bounded difference shape defining the bounded BDS never can
  // contain trivial constraints.
  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    const DB_Row<N>& dbm_i = dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; )
      if (i != j)
        if (is_plus_infinity(dbm_i[j]))
          return false;
  }

  return true;
}

template <typename T>
bool
BD_Shape<T>::contains_integer_point() const {
  // Force shortest-path closure.
  if (is_empty())
    return false;

  const dimension_type space_dim = space_dimension();
  if (space_dim == 0)
    return true;

  // A non-empty BD_Shape defined by integer constraints
  // necessarily contains an integer point.
  if (std::numeric_limits<T>::is_integer)
    return true;

  // Build an integer BD_Shape z with bounds at least as tight as
  // those in *this and then recheck for emptiness.
  BD_Shape<mpz_class> bds_z(space_dim);
  typedef BD_Shape<mpz_class>::N Z;
  bds_z.reset_shortest_path_closed();
  PPL_DIRTY_TEMP(N, tmp);
  bool all_integers = true;
  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    DB_Row<Z>& z_i = bds_z.dbm[i];
    const DB_Row<N>& dbm_i = dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; ) {
      const N& dbm_i_j = dbm_i[j];
      if (is_plus_infinity(dbm_i_j))
        continue;
      if (is_integer(dbm_i_j))
        assign_r(z_i[j], dbm_i_j, ROUND_NOT_NEEDED);
      else {
        all_integers = false;
        Z& z_i_j = z_i[j];
        // Copy dbm_i_j into z_i_j, but rounding downwards.
        neg_assign_r(tmp, dbm_i_j, ROUND_NOT_NEEDED);
        assign_r(z_i_j, tmp, ROUND_UP);
        neg_assign_r(z_i_j, z_i_j, ROUND_NOT_NEEDED);
      }
    }
  }
  return all_integers || !bds_z.is_empty();
}

template <typename T>
bool
BD_Shape<T>::frequency(const Linear_Expression& expr,
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

  shortest_path_closure_assign();
  // For an empty BD shape, we simply return false.
  if (marked_empty())
    return false;

  // The BD shape has at least 1 dimension and is not empty.
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);
  PPL_DIRTY_TEMP(N, tmp);
  Linear_Expression le = expr;
  // Boolean to keep track of a variable `v' in expression `le'.
  // If we can replace `v' by an expression using variables other
  // than `v' and are already in `le', then this is set to true.

  PPL_DIRTY_TEMP_COEFFICIENT(val_denom);
  val_denom = 1;

  // TODO: This loop can be optimized more, if needed, exploiting the
  // (possible) sparseness of le.
  for (dimension_type i = dbm.num_rows(); i-- > 1; ) {
    const Variable v(i-1);
    coeff = le.coefficient(v);
    if (coeff == 0)
      continue;

    const DB_Row<N>& dbm_i = dbm[i];
    // Check if `v' is constant in the BD shape.
    assign_r(tmp, dbm_i[0], ROUND_NOT_NEEDED);
    if (is_additive_inverse(dbm[0][i], tmp)) {
      // If `v' is constant, replace it in `le' by the value.
      numer_denom(tmp, numer, denom);
      sub_mul_assign(le, coeff, v);
      le *= denom;
      le -= numer*coeff;
      val_denom *= denom;
      continue;
    }
    // Check the bounded differences with the other dimensions that
    // have non-zero coefficient in `le'.
    else {
      bool constant_v = false;
      for (Linear_Expression::const_iterator j = le.begin(),
            j_end = le.lower_bound(Variable(i - 1)); j != j_end; ++j) {
        const Variable vj = j.variable();
        const dimension_type j_dim = vj.space_dimension();
        assign_r(tmp, dbm_i[j_dim], ROUND_NOT_NEEDED);
        if (is_additive_inverse(dbm[j_dim][i], tmp)) {
          // The coefficient for `vj' in `le' is not 0
          // and the difference with `v' in the BD shape is constant.
          // So apply this equality to eliminate `v' in `le'.
          numer_denom(tmp, numer, denom);
          // Modifying le invalidates the iterators, but it's not a problem
          // since we are going to exit the loop.
          sub_mul_assign(le, coeff, v);
          add_mul_assign(le, coeff, vj);
          le *= denom;
          le -= numer*coeff;
          val_denom *= denom;
          constant_v = true;
          break;
        }
      }
      if (!constant_v)
        // The expression `expr' is not constant.
        return false;
    }
  }

  // The expression `expr' is constant.
  freq_n = 0;
  freq_d = 1;

  // Reduce `val_n' and `val_d'.
  normalize2(le.inhomogeneous_term(), val_denom, val_n, val_d);
  return true;
}

template <typename T>
bool
BD_Shape<T>::constrains(const Variable var) const {
  // `var' should be one of the dimensions of the BD shape.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dimension() < var_space_dim)
    throw_dimension_incompatible("constrains(v)", "v", var);

  shortest_path_closure_assign();
  // A BD shape known to be empty constrains all variables.
  // (Note: do not force emptiness check _yet_)
  if (marked_empty())
    return true;

  // Check whether `var' is syntactically constrained.
  const DB_Row<N>& dbm_v = dbm[var_space_dim];
  for (dimension_type i = dbm.num_rows(); i-- > 0; ) {
    if (!is_plus_infinity(dbm_v[i])
        || !is_plus_infinity(dbm[i][var_space_dim]))
      return true;
  }

  // `var' is not syntactically constrained:
  // now force an emptiness check.
  return is_empty();
}

template <typename T>
void
BD_Shape<T>
::compute_predecessors(std::vector<dimension_type>& predecessor) const {
  PPL_ASSERT(!marked_empty() && marked_shortest_path_closed());
  PPL_ASSERT(predecessor.size() == 0);
  // Variables are ordered according to their index.
  // The vector `predecessor' is used to indicate which variable
  // immediately precedes a given one in the corresponding equivalence class.
  // The `leader' of an equivalence class is the element having minimum
  // index: leaders are their own predecessors.
  const dimension_type predecessor_size = dbm.num_rows();
  // Initially, each variable is leader of its own zero-equivalence class.
  predecessor.reserve(predecessor_size);
  for (dimension_type i = 0; i < predecessor_size; ++i)
    predecessor.push_back(i);
  // Now compute actual predecessors.
  for (dimension_type i = predecessor_size; i-- > 1; )
    if (i == predecessor[i]) {
      const DB_Row<N>& dbm_i = dbm[i];
      for (dimension_type j = i; j-- > 0; )
        if (j == predecessor[j]
            && is_additive_inverse(dbm[j][i], dbm_i[j])) {
          // Choose as predecessor the variable having the smaller index.
          predecessor[i] = j;
          break;
        }
    }
}

template <typename T>
void
BD_Shape<T>::compute_leaders(std::vector<dimension_type>& leaders) const {
  PPL_ASSERT(!marked_empty() && marked_shortest_path_closed());
  PPL_ASSERT(leaders.size() == 0);
  // Compute predecessor information.
  compute_predecessors(leaders);
  // Flatten the predecessor chains so as to obtain leaders.
  PPL_ASSERT(leaders[0] == 0);
  for (dimension_type i = 1, l_size = leaders.size(); i != l_size; ++i) {
    const dimension_type leaders_i = leaders[i];
    PPL_ASSERT(leaders_i <= i);
    if (leaders_i != i) {
      const dimension_type leaders_leaders_i = leaders[leaders_i];
      PPL_ASSERT(leaders_leaders_i == leaders[leaders_leaders_i]);
      leaders[i] = leaders_leaders_i;
    }
  }
}

template <typename T>
bool
BD_Shape<T>::is_shortest_path_reduced() const {
  // If the BDS is empty, it is also reduced.
  if (marked_empty())
    return true;

  const dimension_type space_dim = space_dimension();
  // Zero-dimensional BDSs are necessarily reduced.
  if (space_dim == 0)
    return true;

  // A shortest-path reduced dbm is just a dbm with an indication of
  // those constraints that are redundant. If there is no indication
  // of the redundant constraints, then it cannot be reduced.
  if (!marked_shortest_path_reduced())
    return false;

  const BD_Shape x_copy = *this;
  x_copy.shortest_path_closure_assign();
  // If we just discovered emptiness, it cannot be reduced.
  if (x_copy.marked_empty())
    return false;

  // The vector `leader' is used to indicate which variables are equivalent.
  std::vector<dimension_type> leader(space_dim + 1);

  // We store the leader.
  for (dimension_type i = space_dim + 1; i-- > 0; )
    leader[i] = i;

  // Step 1: we store really the leader with the corrected value.
  // We search for the equivalent or zero-equivalent variables.
  // The variable(i-1) and variable(j-1) are equivalent if and only if
  // m_i_j == -(m_j_i).
  for (dimension_type i = 0; i < space_dim; ++i) {
    const DB_Row<N>& x_copy_dbm_i = x_copy.dbm[i];
    for (dimension_type j = i + 1; j <= space_dim; ++j)
      if (is_additive_inverse(x_copy.dbm[j][i], x_copy_dbm_i[j]))
        // Two equivalent variables have got the same leader
        // (the smaller variable).
        leader[j] = leader[i];
  }

  // Step 2: we check if there are redundant constraints in the zero_cycle
  // free bounded difference shape, considering only the leaders.
  // A constraint `c' is redundant, when there are two constraints such that
  // their sum is the same constraint with the inhomogeneous term
  // less than or equal to the `c' one.
  PPL_DIRTY_TEMP(N, c);
  for (dimension_type k = 0; k <= space_dim; ++k)
    if (leader[k] == k) {
      const DB_Row<N>& x_k = x_copy.dbm[k];
      for (dimension_type i = 0; i <= space_dim; ++i)
        if (leader[i] == i) {
          const DB_Row<N>& x_i = x_copy.dbm[i];
          const Bit_Row& redundancy_i = redundancy_dbm[i];
          const N& x_i_k = x_i[k];
          for (dimension_type j = 0; j <= space_dim; ++j)
            if (leader[j] == j) {
              const N& x_i_j = x_i[j];
              if (!is_plus_infinity(x_i_j)) {
                add_assign_r(c, x_i_k, x_k[j], ROUND_UP);
                if (x_i_j >= c && !redundancy_i[j])
                  return false;
              }
            }
        }
    }

  // The vector `var_conn' is used to check if there is a single cycle
  // that connected all zero-equivalent variables between them.
  // The value `space_dim + 1' is used to indicate that the equivalence
  // class contains a single variable.
  std::vector<dimension_type> var_conn(space_dim + 1);
  for (dimension_type i = space_dim + 1; i-- > 0; )
    var_conn[i] = space_dim + 1;

  // Step 3: we store really the `var_conn' with the right value, putting
  // the variable with the selected variable is connected:
  // we check the row of each variable:
  // a- each leader could be connected with only zero-equivalent one,
  // b- each non-leader with only another zero-equivalent one.
  for (dimension_type i = 0; i <= space_dim; ++i) {
    // It count with how many variables the selected variable is
    // connected.
    dimension_type t = 0;
    dimension_type leader_i = leader[i];
    // Case a: leader.
    if (leader_i == i) {
      for (dimension_type j = 0; j <= space_dim; ++j) {
        dimension_type leader_j = leader[j];
        // Only the connectedness with equivalent variables
        // is considered.
        if (j != leader_j)
          if (!redundancy_dbm[i][j]) {
            if (t == 1)
              // Two non-leaders cannot be connected with the same leader.
              return false;
            else
              if (leader_j != i)
                // The variables are not in the same equivalence class.
                return false;
              else {
                ++t;
                var_conn[i] = j;
              }
          }
      }
    }
    // Case b: non-leader.
    else {
      for (dimension_type j = 0; j <= space_dim; ++j) {
        if (!redundancy_dbm[i][j]) {
          dimension_type leader_j = leader[j];
          if (leader_i != leader_j)
            // The variables are not in the same equivalence class.
            return false;
          else {
            if (t == 1)
              // The variables cannot be connected with the same leader.
              return false;
            else {
              ++t;
              var_conn[i] = j;
            }
          }
          // A non-leader must be connected with
          // another variable.
          if (t == 0)
            return false;
        }
      }
    }
  }

  // The vector `just_checked' is used to check if
  // a variable is already checked.
  std::vector<bool> just_checked(space_dim + 1);
  for (dimension_type i = space_dim + 1; i-- > 0; )
    just_checked[i] = false;

  // Step 4: we check if there are single cycles that
  // connected all the zero-equivalent variables between them.
  for (dimension_type i = 0; i <= space_dim; ++i) {
    // We do not re-check the already considered single cycles.
    if (!just_checked[i]) {
      dimension_type v_con = var_conn[i];
      // We consider only the equivalence classes with
      // 2 or plus variables.
      if (v_con != space_dim + 1) {
        // There is a single cycle if taken a variable,
        // we return to this same variable.
        while (v_con != i) {
          just_checked[v_con] = true;
          v_con = var_conn[v_con];
          // If we re-pass to an already considered variable,
          // then we haven't a single cycle.
          if (just_checked[v_con])
            return false;
        }
      }
    }
    just_checked[i] = true;
  }

  // The system bounded differences is just reduced.
  return true;
}

template <typename T>
bool
BD_Shape<T>::bounds(const Linear_Expression& expr,
                    const bool from_above) const {
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  const dimension_type space_dim = space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible((from_above
                                  ? "bounds_from_above(e)"
                                  : "bounds_from_below(e)"), "e", expr);

  shortest_path_closure_assign();
  // A zero-dimensional or empty BDS bounds everything.
  if (space_dim == 0 || marked_empty())
    return true;

  // The constraint `c' is used to check if `expr' is a difference
  // bounded and, in this case, to select the cell.
  const Constraint& c = from_above ? expr <= 0 : expr >= 0;
  dimension_type num_vars = 0;
  dimension_type i = 0;
  dimension_type j = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  // Check if `c' is a BD constraint.
  if (BD_Shape_Helpers::extract_bounded_difference(c, num_vars, i, j, coeff)) {
    if (num_vars == 0)
      // Dealing with a trivial constraint.
      return true;
    // Select the cell to be checked.
    const N& x = (coeff < 0) ? dbm[i][j] : dbm[j][i];
    return !is_plus_infinity(x);
  }
  else {
    // Not a DB constraint: use the MIP solver.
    Optimization_Mode mode_bounds
      = from_above ? MAXIMIZATION : MINIMIZATION;
    MIP_Problem mip(space_dim, constraints(), expr, mode_bounds);
    // Problem is known to be feasible.
    return mip.solve() == OPTIMIZED_MIP_PROBLEM;
  }
}

template <typename T>
bool
BD_Shape<T>::max_min(const Linear_Expression& expr,
                     const bool maximize,
                     Coefficient& ext_n, Coefficient& ext_d,
                     bool& included) const {
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible((maximize
                                  ? "maximize(e, ...)"
                                  : "minimize(e, ...)"), "e", expr);
  // Deal with zero-dim BDS first.
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

  shortest_path_closure_assign();
  // For an empty BDS we simply return false.
  if (marked_empty())
    return false;

  // The constraint `c' is used to check if `expr' is a difference
  // bounded and, in this case, to select the cell.
  const Constraint& c = maximize ? expr <= 0 : expr >= 0;
  dimension_type num_vars = 0;
  dimension_type i = 0;
  dimension_type j = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  // Check if `c' is a BD constraint.
  if (!BD_Shape_Helpers::extract_bounded_difference(c, num_vars, i, j, coeff)) {
    Optimization_Mode mode_max_min
      = maximize ? MAXIMIZATION : MINIMIZATION;
    MIP_Problem mip(space_dim, constraints(), expr, mode_max_min);
    if (mip.solve() == OPTIMIZED_MIP_PROBLEM) {
      mip.optimal_value(ext_n, ext_d);
      included = true;
      return true;
    }
    else
      // Here`expr' is unbounded in `*this'.
      return false;
  }
  else {
    // Here `expr' is a bounded difference.
    if (num_vars == 0) {
      // Dealing with a trivial expression.
      ext_n = expr.inhomogeneous_term();
      ext_d = 1;
      included = true;
      return true;
    }

    // Select the cell to be checked.
    const N& x = (coeff < 0) ? dbm[i][j] : dbm[j][i];
    if (!is_plus_infinity(x)) {
      // Compute the maximize/minimize of `expr'.
      PPL_DIRTY_TEMP(N, d);
      const Coefficient& b = expr.inhomogeneous_term();
      PPL_DIRTY_TEMP_COEFFICIENT(minus_b);
      neg_assign(minus_b, b);
      const Coefficient& sc_b = maximize ? b : minus_b;
      assign_r(d, sc_b, ROUND_UP);
      // Set `coeff_expr' to the absolute value of coefficient of
      // a variable in `expr'.
      PPL_DIRTY_TEMP(N, coeff_expr);
      PPL_ASSERT(i != 0);
      const Coefficient& coeff_i = expr.get(Variable(i - 1));
      const int sign_i = sgn(coeff_i);
      if (sign_i > 0)
        assign_r(coeff_expr, coeff_i, ROUND_UP);
      else {
        PPL_DIRTY_TEMP_COEFFICIENT(minus_coeff_i);
        neg_assign(minus_coeff_i, coeff_i);
        assign_r(coeff_expr, minus_coeff_i, ROUND_UP);
      }
      // Approximating the maximum/minimum of `expr'.
      add_mul_assign_r(d, coeff_expr, x, ROUND_UP);
      numer_denom(d, ext_n, ext_d);
      if (!maximize)
        neg_assign(ext_n);
      included = true;
      return true;
    }

    // `expr' is unbounded.
    return false;
  }
}

template <typename T>
bool
BD_Shape<T>::max_min(const Linear_Expression& expr,
                     const bool maximize,
                     Coefficient& ext_n, Coefficient& ext_d,
                     bool& included,
                     Generator& g) const {
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible((maximize
                                  ? "maximize(e, ...)"
                                  : "minimize(e, ...)"), "e", expr);
  // Deal with zero-dim BDS first.
  if (space_dim == 0) {
    if (marked_empty())
      return false;
    else {
      ext_n = expr.inhomogeneous_term();
      ext_d = 1;
      included = true;
      g = point();
      return true;
    }
  }

  shortest_path_closure_assign();
  // For an empty BDS we simply return false.
  if (marked_empty())
    return false;

  Optimization_Mode mode_max_min
    = maximize ? MAXIMIZATION : MINIMIZATION;
  MIP_Problem mip(space_dim, constraints(), expr, mode_max_min);
  if (mip.solve() == OPTIMIZED_MIP_PROBLEM) {
    g = mip.optimizing_point();
    mip.evaluate_objective_function(g, ext_n, ext_d);
    included = true;
    return true;
  }
  // Here `expr' is unbounded in `*this'.
  return false;
}

template <typename T>
Poly_Con_Relation
BD_Shape<T>::relation_with(const Congruence& cg) const {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (cg.space_dimension() > space_dim)
    throw_dimension_incompatible("relation_with(cg)", cg);

  // If the congruence is an equality, find the relation with
  // the equivalent equality constraint.
  if (cg.is_equality()) {
    Constraint c(cg);
    return relation_with(c);
  }

  shortest_path_closure_assign();

  if (marked_empty())
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

  // Find the lower bound for a hyperplane with direction
  // defined by the congruence.
  Linear_Expression le = Linear_Expression(cg.expression());
  PPL_DIRTY_TEMP_COEFFICIENT(min_numer);
  PPL_DIRTY_TEMP_COEFFICIENT(min_denom);
  bool min_included;
  bool bounded_below = minimize(le, min_numer, min_denom, min_included);

  // If there is no lower bound, then some of the hyperplanes defined by
  // the congruence will strictly intersect the shape.
  if (!bounded_below)
    return Poly_Con_Relation::strictly_intersects();

  // TODO: Consider adding a max_and_min() method, performing both
  // maximization and minimization so as to possibly exploit
  // incrementality of the MIP solver.

  // Find the upper bound for a hyperplane with direction
  // defined by the congruence.
  PPL_DIRTY_TEMP_COEFFICIENT(max_numer);
  PPL_DIRTY_TEMP_COEFFICIENT(max_denom);
  bool max_included;
  bool bounded_above = maximize(le, max_numer, max_denom, max_included);

  // If there is no upper bound, then some of the hyperplanes defined by
  // the congruence will strictly intersect the shape.
  if (!bounded_above)
    return Poly_Con_Relation::strictly_intersects();

  PPL_DIRTY_TEMP_COEFFICIENT(signed_distance);

  // Find the position value for the hyperplane that satisfies the congruence
  // and is above the lower bound for the shape.
  PPL_DIRTY_TEMP_COEFFICIENT(min_value);
  min_value = min_numer / min_denom;
  const Coefficient& modulus = cg.modulus();
  signed_distance = min_value % modulus;
  min_value -= signed_distance;
  if (min_value * min_denom < min_numer)
    min_value += modulus;

  // Find the position value for the hyperplane that satisfies the congruence
  // and is below the upper bound for the shape.
  PPL_DIRTY_TEMP_COEFFICIENT(max_value);
  max_value = max_numer / max_denom;
  signed_distance = max_value % modulus;
  max_value += signed_distance;
  if (max_value * max_denom > max_numer)
    max_value -= modulus;

  // If the upper bound value is less than the lower bound value,
  // then there is an empty intersection with the congruence;
  // otherwise it will strictly intersect.
  if (max_value < min_value)
    return Poly_Con_Relation::is_disjoint();
  else
    return Poly_Con_Relation::strictly_intersects();
}


template <typename T>
Poly_Con_Relation
BD_Shape<T>::relation_with(const Constraint& c) const {
  const dimension_type c_space_dim = c.space_dimension();
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (c_space_dim > space_dim)
    throw_dimension_incompatible("relation_with(c)", c);

  shortest_path_closure_assign();

  if (marked_empty())
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

  dimension_type num_vars = 0;
  dimension_type i = 0;
  dimension_type j = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  if (!BD_Shape_Helpers::extract_bounded_difference(c, num_vars, i, j, coeff)) {
    // Constraints that are not bounded differences.
    // Use maximize() and minimize() to do much of the work.

    // Find the linear expression for the constraint and use that to
    // find if the expression is bounded from above or below and if it
    // is, find the maximum and minimum values.
    Linear_Expression le(c.expression());
    le.set_inhomogeneous_term(Coefficient_zero());

    PPL_DIRTY_TEMP(Coefficient, max_numer);
    PPL_DIRTY_TEMP(Coefficient, max_denom);
    bool max_included;
    PPL_DIRTY_TEMP(Coefficient, min_numer);
    PPL_DIRTY_TEMP(Coefficient, min_denom);
    bool min_included;
    bool bounded_above = maximize(le, max_numer, max_denom, max_included);
    bool bounded_below = minimize(le, min_numer, min_denom, min_included);
    if (!bounded_above) {
      if (!bounded_below)
        return Poly_Con_Relation::strictly_intersects();
      min_numer += c.inhomogeneous_term() * min_denom;
      switch (sgn(min_numer)) {
      case 1:
        if (c.is_equality())
          return Poly_Con_Relation::is_disjoint();
        return Poly_Con_Relation::is_included();
      case 0:
        if (c.is_strict_inequality() || c.is_equality())
          return Poly_Con_Relation::strictly_intersects();
        return Poly_Con_Relation::is_included();
      case -1:
        return Poly_Con_Relation::strictly_intersects();
      }
    }
    if (!bounded_below) {
      max_numer += c.inhomogeneous_term() * max_denom;
      switch (sgn(max_numer)) {
      case 1:
        return Poly_Con_Relation::strictly_intersects();
      case 0:
        if (c.is_strict_inequality())
          return Poly_Con_Relation::is_disjoint();
        return Poly_Con_Relation::strictly_intersects();
      case -1:
        return Poly_Con_Relation::is_disjoint();
      }
    }
    else {
      max_numer += c.inhomogeneous_term() * max_denom;
      min_numer += c.inhomogeneous_term() * min_denom;
      switch (sgn(max_numer)) {
      case 1:
        switch (sgn(min_numer)) {
        case 1:
          if (c.is_equality())
            return Poly_Con_Relation::is_disjoint();
          return Poly_Con_Relation::is_included();
        case 0:
          if (c.is_equality())
            return Poly_Con_Relation::strictly_intersects();
          if (c.is_strict_inequality())
            return Poly_Con_Relation::strictly_intersects();
          return Poly_Con_Relation::is_included();
        case -1:
          return Poly_Con_Relation::strictly_intersects();
        }
        PPL_UNREACHABLE;
        break;
      case 0:
        if (min_numer == 0) {
          if (c.is_strict_inequality())
            return Poly_Con_Relation::is_disjoint()
              && Poly_Con_Relation::saturates();
          return Poly_Con_Relation::is_included()
            && Poly_Con_Relation::saturates();
        }
        if (c.is_strict_inequality())
          return Poly_Con_Relation::is_disjoint();
        return Poly_Con_Relation::strictly_intersects();
      case -1:
        return Poly_Con_Relation::is_disjoint();
      }
    }
  }

  // Constraints that are bounded differences.
  if (num_vars == 0) {
    // Dealing with a trivial constraint.
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
      if (c.is_equality())
        return Poly_Con_Relation::is_disjoint();
      else
        return Poly_Con_Relation::is_included();
    }
  }

  // Select the cell to be checked for the "<=" part of the constraint,
  // and set `coeff' to the absolute value of itself.
  const bool negative = (coeff < 0);
  const N& x = negative ? dbm[i][j] : dbm[j][i];
  const N& y = negative ? dbm[j][i] : dbm[i][j];
  if (negative)
    neg_assign(coeff);
  // Deduce the relation/s of the constraint `c' of the form
  // `coeff*v - coeff*u </<=/== c.inhomogeneous_term()'
  // with the respectively constraints in `*this'
  // `-y <= v - u <= x'.
  // Let `d == c.inhomogeneous_term()/coeff'
  // and `d1 == -c.inhomogeneous_term()/coeff'.
  // The following variables of mpq_class type are used to be precise
  // when the bds is defined by integer constraints.
  PPL_DIRTY_TEMP(mpq_class, q_x);
  PPL_DIRTY_TEMP(mpq_class, q_y);
  PPL_DIRTY_TEMP(mpq_class, d);
  PPL_DIRTY_TEMP(mpq_class, d1);
  PPL_DIRTY_TEMP(mpq_class, c_denom);
  PPL_DIRTY_TEMP(mpq_class, q_denom);
  assign_r(c_denom, coeff, ROUND_NOT_NEEDED);
  assign_r(d, c.inhomogeneous_term(), ROUND_NOT_NEEDED);
  neg_assign_r(d1, d, ROUND_NOT_NEEDED);
  div_assign_r(d, d, c_denom, ROUND_NOT_NEEDED);
  div_assign_r(d1, d1, c_denom, ROUND_NOT_NEEDED);

  if (is_plus_infinity(x)) {
    if (!is_plus_infinity(y)) {
      // `*this' is in the following form:
      // `-y <= v - u'.
      // In this case `*this' is disjoint from `c' if
      // `-y > d' (`-y >= d' if c is a strict equality), i.e. if
      // `y < d1' (`y <= d1' if c is a strict equality).
      PPL_DIRTY_TEMP_COEFFICIENT(numer);
      PPL_DIRTY_TEMP_COEFFICIENT(denom);
      numer_denom(y, numer, denom);
      assign_r(q_denom, denom, ROUND_NOT_NEEDED);
      assign_r(q_y, numer, ROUND_NOT_NEEDED);
      div_assign_r(q_y, q_y, q_denom, ROUND_NOT_NEEDED);
      if (q_y < d1)
        return Poly_Con_Relation::is_disjoint();
      if (q_y == d1 && c.is_strict_inequality())
        return Poly_Con_Relation::is_disjoint();
    }

    // In all other cases `*this' intersects `c'.
    return Poly_Con_Relation::strictly_intersects();
  }

  // Here `x' is not plus-infinity.
  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);
  numer_denom(x, numer, denom);
  assign_r(q_denom, denom, ROUND_NOT_NEEDED);
  assign_r(q_x, numer, ROUND_NOT_NEEDED);
  div_assign_r(q_x, q_x, q_denom, ROUND_NOT_NEEDED);

  if (!is_plus_infinity(y)) {
    numer_denom(y, numer, denom);
    assign_r(q_denom, denom, ROUND_NOT_NEEDED);
    assign_r(q_y, numer, ROUND_NOT_NEEDED);
    div_assign_r(q_y, q_y, q_denom, ROUND_NOT_NEEDED);
    if (q_x == d && q_y == d1) {
      if (c.is_strict_inequality())
        return Poly_Con_Relation::saturates()
          && Poly_Con_Relation::is_disjoint();
      else
        return Poly_Con_Relation::saturates()
          && Poly_Con_Relation::is_included();
    }
    // `*this' is disjoint from `c' when
    // `-y > d' (`-y >= d' if c is a strict equality), i.e. if
    // `y < d1' (`y <= d1' if c is a strict equality).
    if (q_y < d1)
      return Poly_Con_Relation::is_disjoint();
    if (q_y == d1 && c.is_strict_inequality())
      return Poly_Con_Relation::is_disjoint();
  }

  // Here `y' can be also plus-infinity.
  // If `c' is an equality, `*this' is disjoint from `c' if
  // `x < d'.
  if (d > q_x) {
    if (c.is_equality())
      return Poly_Con_Relation::is_disjoint();
    else
      return Poly_Con_Relation::is_included();
  }

  if (d == q_x && c.is_nonstrict_inequality())
    return Poly_Con_Relation::is_included();

  // In all other cases `*this' intersects `c'.
  return Poly_Con_Relation::strictly_intersects();
}

template <typename T>
Poly_Gen_Relation
BD_Shape<T>::relation_with(const Generator& g) const {
  const dimension_type space_dim = space_dimension();
  const dimension_type g_space_dim = g.space_dimension();

  // Dimension-compatibility check.
  if (space_dim < g_space_dim)
    throw_dimension_incompatible("relation_with(g)", g);

  shortest_path_closure_assign();
  // The empty BDS cannot subsume a generator.
  if (marked_empty())
    return Poly_Gen_Relation::nothing();

  // A universe BDS in a zero-dimensional space subsumes
  // all the generators of a zero-dimensional space.
  if (space_dim == 0)
    return Poly_Gen_Relation::subsumes();

  const bool is_line = g.is_line();
  const bool is_line_or_ray = g.is_line_or_ray();

  // The relation between the BDS and the given generator is obtained
  // checking if the generator satisfies all the constraints in the BDS.
  // To check if the generator satisfies all the constraints it's enough
  // studying the sign of the scalar product between the generator and
  // all the constraints in the BDS.

  // Allocation of temporaries done once and for all.
  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);
  PPL_DIRTY_TEMP_COEFFICIENT(product);
  // We find in `*this' all the constraints.
  // TODO: This loop can be optimized more, if needed.
  for (dimension_type i = 0; i <= space_dim; ++i) {
    const Coefficient& g_coeff_y = (i > g_space_dim || i == 0)
      ? Coefficient_zero() : g.coefficient(Variable(i-1));
    const DB_Row<N>& dbm_i = dbm[i];
    for (dimension_type j = i + 1; j <= space_dim; ++j) {
      const Coefficient& g_coeff_x = (j > g_space_dim)
        ? Coefficient_zero() : g.coefficient(Variable(j-1));
      const N& dbm_ij = dbm_i[j];
      const N& dbm_ji = dbm[j][i];
      if (is_additive_inverse(dbm_ji, dbm_ij)) {
        // We have one equality constraint: denom*x - denom*y = numer.
        // Compute the scalar product.
        numer_denom(dbm_ij, numer, denom);
        product = g_coeff_y;
        product -= g_coeff_x;
        product *= denom;
        if (!is_line_or_ray)
          add_mul_assign(product, numer, g.divisor());
        if (product != 0)
          return Poly_Gen_Relation::nothing();
      }
      else {
        // We have 0, 1 or 2 binary inequality constraint/s.
        if (!is_plus_infinity(dbm_ij)) {
          // We have the binary inequality constraint:
          // denom*x - denom*y <= numer.
          // Compute the scalar product.
          numer_denom(dbm_ij, numer, denom);
          product = g_coeff_y;
          product -= g_coeff_x;
          product *= denom;
          if (!is_line_or_ray)
            add_mul_assign(product, numer, g.divisor());
          if (is_line) {
            if (product != 0)
              // Lines must saturate all constraints.
              return Poly_Gen_Relation::nothing();
          }
          else
            // `g' is either a ray, a point or a closure point.
            if (product < 0)
              return Poly_Gen_Relation::nothing();
        }

        if (!is_plus_infinity(dbm_ji)) {
          // We have the binary inequality constraint: denom*y - denom*x <= b.
          // Compute the scalar product.
          numer_denom(dbm_ji, numer, denom);
          product = 0;
          add_mul_assign(product, denom, g_coeff_x);
          add_mul_assign(product, -denom, g_coeff_y);
          if (!is_line_or_ray)
            add_mul_assign(product, numer, g.divisor());
          if (is_line) {
            if (product != 0)
              // Lines must saturate all constraints.
              return Poly_Gen_Relation::nothing();
          }
          else
            // `g' is either a ray, a point or a closure point.
            if (product < 0)
              return Poly_Gen_Relation::nothing();
        }
      }
    }
  }

  // The generator satisfies all the constraints.
  return Poly_Gen_Relation::subsumes();
}

template <typename T>
void
BD_Shape<T>::shortest_path_closure_assign() const {
  // Do something only if necessary.
  if (marked_empty() || marked_shortest_path_closed())
    return;
  const dimension_type num_dimensions = space_dimension();
  // Zero-dimensional BDSs are necessarily shortest-path closed.
  if (num_dimensions == 0)
    return;

  // Even though the BDS will not change, its internal representation
  // is going to be modified by the Floyd-Warshall algorithm.
  BD_Shape& x = const_cast<BD_Shape<T>&>(*this);

  // Fill the main diagonal with zeros.
  for (dimension_type h = num_dimensions + 1; h-- > 0; ) {
    PPL_ASSERT(is_plus_infinity(x.dbm[h][h]));
    assign_r(x.dbm[h][h], 0, ROUND_NOT_NEEDED);
  }

  PPL_DIRTY_TEMP(N, sum);
  for (dimension_type k = num_dimensions + 1; k-- > 0; ) {
    const DB_Row<N>& x_dbm_k = x.dbm[k];
    for (dimension_type i = num_dimensions + 1; i-- > 0; ) {
      DB_Row<N>& x_dbm_i = x.dbm[i];
      const N& x_dbm_i_k = x_dbm_i[k];
      if (!is_plus_infinity(x_dbm_i_k))
        for (dimension_type j = num_dimensions + 1; j-- > 0; ) {
          const N& x_dbm_k_j = x_dbm_k[j];
          if (!is_plus_infinity(x_dbm_k_j)) {
            // Rounding upward for correctness.
            add_assign_r(sum, x_dbm_i_k, x_dbm_k_j, ROUND_UP);
            min_assign(x_dbm_i[j], sum);
          }
        }
    }
  }

  // Check for emptiness: the BDS is empty if and only if there is a
  // negative value on the main diagonal of `dbm'.
  for (dimension_type h = num_dimensions + 1; h-- > 0; ) {
    N& x_dbm_hh = x.dbm[h][h];
    if (sgn(x_dbm_hh) < 0) {
      x.set_empty();
      return;
    }
    else {
      PPL_ASSERT(sgn(x_dbm_hh) == 0);
      // Restore PLUS_INFINITY on the main diagonal.
      assign_r(x_dbm_hh, PLUS_INFINITY, ROUND_NOT_NEEDED);
    }
  }

  // The BDS is not empty and it is now shortest-path closed.
  x.set_shortest_path_closed();
}

template <typename T>
void
BD_Shape<T>::incremental_shortest_path_closure_assign(Variable var) const {
  // Do something only if necessary.
  if (marked_empty() || marked_shortest_path_closed())
    return;
  const dimension_type num_dimensions = space_dimension();
  PPL_ASSERT(var.id() < num_dimensions);

  // Even though the BDS will not change, its internal representation
  // is going to be modified by the incremental Floyd-Warshall algorithm.
  BD_Shape& x = const_cast<BD_Shape&>(*this);

  // Fill the main diagonal with zeros.
  for (dimension_type h = num_dimensions + 1; h-- > 0; ) {
    PPL_ASSERT(is_plus_infinity(x.dbm[h][h]));
    assign_r(x.dbm[h][h], 0, ROUND_NOT_NEEDED);
  }

  // Using the incremental Floyd-Warshall algorithm.
  PPL_DIRTY_TEMP(N, sum);
  const dimension_type v = var.id() + 1;
  DB_Row<N>& x_v = x.dbm[v];
  // Step 1: Improve all constraints on variable `var'.
  for (dimension_type k = num_dimensions + 1; k-- > 0; ) {
    DB_Row<N>& x_k = x.dbm[k];
    const N& x_v_k = x_v[k];
    const N& x_k_v = x_k[v];
    const bool x_v_k_finite = !is_plus_infinity(x_v_k);
    const bool x_k_v_finite = !is_plus_infinity(x_k_v);
    // Specialize inner loop based on finiteness info.
    if (x_v_k_finite) {
      if (x_k_v_finite) {
        // Here both x_v_k and x_k_v are finite.
        for (dimension_type i = num_dimensions + 1; i-- > 0; ) {
          DB_Row<N>& x_i = x.dbm[i];
          const N& x_i_k = x_i[k];
          if (!is_plus_infinity(x_i_k)) {
            add_assign_r(sum, x_i_k, x_k_v, ROUND_UP);
            min_assign(x_i[v], sum);
          }
          const N& x_k_i = x_k[i];
          if (!is_plus_infinity(x_k_i)) {
            add_assign_r(sum, x_v_k, x_k_i, ROUND_UP);
            min_assign(x_v[i], sum);
          }
        }
      }
      else {
        // Here x_v_k is finite, but x_k_v is not.
        for (dimension_type i = num_dimensions + 1; i-- > 0; ) {
          const N& x_k_i = x_k[i];
          if (!is_plus_infinity(x_k_i)) {
            add_assign_r(sum, x_v_k, x_k_i, ROUND_UP);
            min_assign(x_v[i], sum);
          }
        }
      }
    }
    else if (x_k_v_finite) {
      // Here x_v_k is infinite, but x_k_v is finite.
      for (dimension_type i = num_dimensions + 1; i-- > 0; ) {
        DB_Row<N>& x_i = x.dbm[i];
        const N& x_i_k = x_i[k];
        if (!is_plus_infinity(x_i_k)) {
          add_assign_r(sum, x_i_k, x_k_v, ROUND_UP);
          min_assign(x_i[v], sum);
        }
      }
    }
    else
      // Here both x_v_k and x_k_v are infinite.
      continue;
  }

  // Step 2: improve the other bounds by using the precise bounds
  // for the constraints on `var'.
  for (dimension_type i = num_dimensions + 1; i-- > 0; ) {
    DB_Row<N>& x_i = x.dbm[i];
    const N& x_i_v = x_i[v];
    if (!is_plus_infinity(x_i_v)) {
      for (dimension_type j = num_dimensions + 1; j-- > 0; ) {
        const N& x_v_j = x_v[j];
        if (!is_plus_infinity(x_v_j)) {
          add_assign_r(sum, x_i_v, x_v_j, ROUND_UP);
          min_assign(x_i[j], sum);
        }
      }
    }
  }

  // Check for emptiness: the BDS is empty if and only if there is a
  // negative value on the main diagonal of `dbm'.
  for (dimension_type h = num_dimensions + 1; h-- > 0; ) {
    N& x_dbm_hh = x.dbm[h][h];
    if (sgn(x_dbm_hh) < 0) {
      x.set_empty();
      return;
    }
    else {
      PPL_ASSERT(sgn(x_dbm_hh) == 0);
      // Restore PLUS_INFINITY on the main diagonal.
      assign_r(x_dbm_hh, PLUS_INFINITY, ROUND_NOT_NEEDED);
    }
  }

  // The BDS is not empty and it is now shortest-path closed.
  x.set_shortest_path_closed();
}

template <typename T>
void
BD_Shape<T>::shortest_path_reduction_assign() const {
  // Do something only if necessary.
  if (marked_shortest_path_reduced())
    return;

  const dimension_type space_dim = space_dimension();
  // Zero-dimensional BDSs are necessarily reduced.
  if (space_dim == 0)
    return;

  // First find the tightest constraints for this BDS.
  shortest_path_closure_assign();

  // If `*this' is empty, then there is nothing to reduce.
  if (marked_empty())
    return;

  // Step 1: compute zero-equivalence classes.
  // Variables corresponding to indices `i' and `j' are zero-equivalent
  // if they lie on a zero-weight loop; since the matrix is shortest-path
  // closed, this happens if and only if dbm[i][j] == -dbm[j][i].
  std::vector<dimension_type> predecessor;
  compute_predecessors(predecessor);
  std::vector<dimension_type> leaders;
  compute_leader_indices(predecessor, leaders);
  const dimension_type num_leaders = leaders.size();

  Bit_Matrix redundancy(space_dim + 1, space_dim + 1);
  // Init all constraints to be redundant.
  // TODO: provide an appropriate method to set multiple bits.
  Bit_Row& red_0 = redundancy[0];
  for (dimension_type j = space_dim + 1; j-- > 0; )
    red_0.set(j);
  for (dimension_type i = space_dim + 1; i-- > 0; )
    redundancy[i] = red_0;

  // Step 2: flag non-redundant constraints in the (zero-cycle-free)
  // subsystem of bounded differences having only leaders as variables.
  PPL_DIRTY_TEMP(N, c);
  for (dimension_type l_i = 0; l_i < num_leaders; ++l_i) {
    const dimension_type i = leaders[l_i];
    const DB_Row<N>& dbm_i = dbm[i];
    Bit_Row& redundancy_i = redundancy[i];
    for (dimension_type l_j = 0; l_j < num_leaders; ++l_j) {
      const dimension_type j = leaders[l_j];
      if (redundancy_i[j]) {
        const N& dbm_i_j = dbm_i[j];
        redundancy_i.clear(j);
        for (dimension_type l_k = 0; l_k < num_leaders; ++l_k) {
          const dimension_type k = leaders[l_k];
          add_assign_r(c, dbm_i[k], dbm[k][j], ROUND_UP);
          if (dbm_i_j >= c) {
            redundancy_i.set(j);
            break;
          }
        }
      }
    }
  }

  // Step 3: flag non-redundant constraints in zero-equivalence classes.
  // Each equivalence class must have a single 0-cycle connecting
  // all the equivalent variables in increasing order.
  std::deque<bool> dealt_with(space_dim + 1, false);
  for (dimension_type i = space_dim + 1; i-- > 0; )
    // We only need to deal with non-singleton zero-equivalence classes
    // that haven't already been dealt with.
    if (i != predecessor[i] && !dealt_with[i]) {
      dimension_type j = i;
      while (true) {
        const dimension_type predecessor_j = predecessor[j];
        if (j == predecessor_j) {
          // We finally found the leader of `i'.
          PPL_ASSERT(redundancy[i][j]);
          redundancy[i].clear(j);
          // Here we dealt with `j' (i.e., `predecessor_j'), but it is useless
          // to update `dealt_with' because `j' is a leader.
          break;
        }
        // We haven't found the leader of `i' yet.
        PPL_ASSERT(redundancy[predecessor_j][j]);
        redundancy[predecessor_j].clear(j);
        dealt_with[predecessor_j] = true;
        j = predecessor_j;
      }
    }

  // Even though shortest-path reduction is not going to change the BDS,
  // it might change its internal representation.
  BD_Shape<T>& x = const_cast<BD_Shape<T>&>(*this);
  using std::swap;
  swap(x.redundancy_dbm, redundancy);
  x.set_shortest_path_reduced();

  PPL_ASSERT(is_shortest_path_reduced());
}

template <typename T>
void
BD_Shape<T>::upper_bound_assign(const BD_Shape& y) {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("upper_bound_assign(y)", y);

  // The upper bound of a BD shape `bd' with an empty shape is `bd'.
  y.shortest_path_closure_assign();
  if (y.marked_empty())
    return;
  shortest_path_closure_assign();
  if (marked_empty()) {
    *this = y;
    return;
  }

  // The bds-hull consists in constructing `*this' with the maximum
  // elements selected from `*this' and `y'.
  PPL_ASSERT(space_dim == 0 || marked_shortest_path_closed());
  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    DB_Row<N>& dbm_i = dbm[i];
    const DB_Row<N>& y_dbm_i = y.dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; ) {
      N& dbm_ij = dbm_i[j];
      const N& y_dbm_ij = y_dbm_i[j];
      if (dbm_ij < y_dbm_ij)
        dbm_ij = y_dbm_ij;
    }
  }
  // Shortest-path closure is maintained (if it was holding).
  // TODO: see whether reduction can be (efficiently!) maintained too.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();
  PPL_ASSERT(OK());
}

template <typename T>
bool
BD_Shape<T>::BFT00_upper_bound_assign_if_exact(const BD_Shape& y) {
  // Declare a const reference to *this (to avoid accidental modifications).
  const BD_Shape& x = *this;
  const dimension_type x_space_dim = x.space_dimension();

  // Private method: the caller must ensure the following.
  PPL_ASSERT(x_space_dim == y.space_dimension());

  // The zero-dim case is trivial.
  if (x_space_dim == 0) {
    upper_bound_assign(y);
    return true;
  }
  // If `x' or `y' is (known to be) empty, the upper bound is exact.
  if (x.marked_empty()) {
    *this = y;
    return true;
  }
  else if (y.is_empty())
    return true;
  else if (x.is_empty()) {
    *this = y;
    return true;
  }

  // Here both `x' and `y' are known to be non-empty.
  // Implementation based on Algorithm 4.1 (page 6) in [BemporadFT00TR],
  // tailored to the special case of BD shapes.

  Variable epsilon(x_space_dim);
  Linear_Expression zero_expr;
  zero_expr.set_space_dimension(x_space_dim + 1);
  Linear_Expression db_expr;
  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);

  // Step 1: compute the constraint system for the envelope env(x,y)
  // and put into x_cs_removed and y_cs_removed those non-redundant
  // constraints that are not in the constraint system for env(x,y).
  // While at it, also add the additional space dimension (epsilon).
  Constraint_System env_cs;
  Constraint_System x_cs_removed;
  Constraint_System y_cs_removed;
  x.shortest_path_reduction_assign();
  y.shortest_path_reduction_assign();
  for (dimension_type i = x_space_dim + 1; i-- > 0; ) {
    const Bit_Row& x_red_i = x.redundancy_dbm[i];
    const Bit_Row& y_red_i = y.redundancy_dbm[i];
    const DB_Row<N>& x_dbm_i = x.dbm[i];
    const DB_Row<N>& y_dbm_i = y.dbm[i];
    for (dimension_type j = x_space_dim + 1; j-- > 0; ) {
      if (x_red_i[j] && y_red_i[j])
        continue;
      if (!x_red_i[j]) {
        const N& x_dbm_ij = x_dbm_i[j];
        PPL_ASSERT(!is_plus_infinity(x_dbm_ij));
        numer_denom(x_dbm_ij, numer, denom);
        // Build skeleton DB constraint (having the right space dimension).
        db_expr = zero_expr;
        if (i > 0)
          db_expr += Variable(i-1);
        if (j > 0)
          db_expr -= Variable(j-1);
        if (denom != 1)
          db_expr *= denom;
        db_expr += numer;
        if (x_dbm_ij >= y_dbm_i[j])
          env_cs.insert(db_expr >= 0);
        else {
          db_expr += epsilon;
          x_cs_removed.insert(db_expr == 0);
        }
      }
      if (!y_red_i[j]) {
        const N& y_dbm_ij = y_dbm_i[j];
        const N& x_dbm_ij = x_dbm_i[j];
        PPL_ASSERT(!is_plus_infinity(y_dbm_ij));
        numer_denom(y_dbm_ij, numer, denom);
        // Build skeleton DB constraint (having the right space dimension).
        db_expr = zero_expr;
        if (i > 0)
          db_expr += Variable(i-1);
        if (j > 0)
          db_expr -= Variable(j-1);
        if (denom != 1)
          db_expr *= denom;
        db_expr += numer;
        if (y_dbm_ij >= x_dbm_ij) {
          // Check if same constraint was added when considering x_dbm_ij.
          if (!x_red_i[j] && x_dbm_ij == y_dbm_ij)
            continue;
          env_cs.insert(db_expr >= 0);
        }
        else {
          db_expr += epsilon;
          y_cs_removed.insert(db_expr == 0);
        }
      }
    }
  }

  if (x_cs_removed.empty())
    // No constraint of x was removed: y is included in x.
    return true;
  if (y_cs_removed.empty()) {
    // No constraint of y was removed: x is included in y.
    *this = y;
    return true;
  }

  // In preparation to Step 4: build the common part of LP problems,
  // i.e., the constraints corresponding to env(x,y),
  // where the additional space dimension (epsilon) has to be maximized.
  MIP_Problem env_lp(x_space_dim + 1, env_cs, epsilon, MAXIMIZATION);
  // Pre-solve `env_lp' to later exploit incrementality.
  env_lp.solve();
  PPL_ASSERT(env_lp.solve() != UNFEASIBLE_MIP_PROBLEM);

  // Implementing loop in Steps 3-6.
  for (Constraint_System::const_iterator i = x_cs_removed.begin(),
         i_end = x_cs_removed.end(); i != i_end; ++i) {
    MIP_Problem lp_i(env_lp);
    lp_i.add_constraint(*i);
    // Pre-solve to exploit incrementality.
    if (lp_i.solve() == UNFEASIBLE_MIP_PROBLEM)
      continue;
    for (Constraint_System::const_iterator j = y_cs_removed.begin(),
           j_end = y_cs_removed.end(); j != j_end; ++j) {
      MIP_Problem lp_ij(lp_i);
      lp_ij.add_constraint(*j);
      // Solve and check for a positive optimal value.
      switch (lp_ij.solve()) {
      case UNFEASIBLE_MIP_PROBLEM:
        // CHECKME: is the following actually impossible?
        PPL_UNREACHABLE;
        return false;
      case UNBOUNDED_MIP_PROBLEM:
        return false;
      case OPTIMIZED_MIP_PROBLEM:
        lp_ij.optimal_value(numer, denom);
        if (numer > 0)
          return false;
        break;
      }
    }
  }

  // The upper bound of x and y is indeed exact.
  upper_bound_assign(y);
  PPL_ASSERT(OK());
  return true;
}

template <typename T>
template <bool integer_upper_bound>
bool
BD_Shape<T>::BHZ09_upper_bound_assign_if_exact(const BD_Shape& y) {
  PPL_COMPILE_TIME_CHECK(!integer_upper_bound
                         || std::numeric_limits<T>::is_integer,
                         "BD_Shape<T>::BHZ09_upper_bound_assign_if_exact(y):"
                         " instantiating for integer upper bound,"
                         " but T in not an integer datatype.");

  // FIXME, CHECKME: what about inexact computations?
  // Declare a const reference to *this (to avoid accidental modifications).
  const BD_Shape& x = *this;
  const dimension_type x_space_dim = x.space_dimension();

  // Private method: the caller must ensure the following.
  PPL_ASSERT(x_space_dim == y.space_dimension());

  // The zero-dim case is trivial.
  if (x_space_dim == 0) {
    upper_bound_assign(y);
    return true;
  }
  // If `x' or `y' is (known to be) empty, the upper bound is exact.
  if (x.marked_empty()) {
    *this = y;
    return true;
  }
  else if (y.is_empty())
    return true;
  else if (x.is_empty()) {
    *this = y;
    return true;
  }

  // Here both `x' and `y' are known to be non-empty.
  x.shortest_path_reduction_assign();
  y.shortest_path_reduction_assign();
  PPL_ASSERT(x.marked_shortest_path_closed());
  PPL_ASSERT(y.marked_shortest_path_closed());
  // Pre-compute the upper bound of `x' and `y'.
  BD_Shape<T> ub(x);
  ub.upper_bound_assign(y);

  PPL_DIRTY_TEMP(N, lhs);
  PPL_DIRTY_TEMP(N, rhs);
  PPL_DIRTY_TEMP(N, temp_zero);
  assign_r(temp_zero, 0, ROUND_NOT_NEEDED);
  PPL_DIRTY_TEMP(N, temp_one);
  if (integer_upper_bound)
    assign_r(temp_one, 1, ROUND_NOT_NEEDED);

  for (dimension_type i = x_space_dim + 1; i-- > 0; ) {
    const DB_Row<N>& x_i = x.dbm[i];
    const Bit_Row& x_red_i = x.redundancy_dbm[i];
    const DB_Row<N>& y_i = y.dbm[i];
    const DB_Row<N>& ub_i = ub.dbm[i];
    for (dimension_type j = x_space_dim + 1; j-- > 0; ) {
      // Check redundancy of x_i_j.
      if (x_red_i[j])
        continue;
      // By non-redundancy, we know that i != j.
      PPL_ASSERT(i != j);
      const N& x_i_j = x_i[j];
      if (x_i_j < y_i[j]) {
        for (dimension_type k = x_space_dim + 1; k-- > 0; ) {
          const DB_Row<N>& x_k = x.dbm[k];
          const DB_Row<N>& y_k = y.dbm[k];
          const Bit_Row& y_red_k = y.redundancy_dbm[k];
          const DB_Row<N>& ub_k = ub.dbm[k];
          const N& ub_k_j = (k == j) ? temp_zero : ub_k[j];
          for (dimension_type ell = x_space_dim + 1; ell-- > 0; ) {
            // Check redundancy of y_k_ell.
            if (y_red_k[ell])
              continue;
            // By non-redundancy, we know that k != ell.
            PPL_ASSERT(k != ell);
            const N& y_k_ell = y_k[ell];
            if (y_k_ell < x_k[ell]) {
              // The first condition in BHZ09 theorem holds;
              // now check for the second condition.
              add_assign_r(lhs, x_i_j, y_k_ell, ROUND_UP);
              const N& ub_i_ell = (i == ell) ? temp_zero : ub_i[ell];
              add_assign_r(rhs, ub_i_ell, ub_k_j, ROUND_UP);
              if (integer_upper_bound) {
                // Note: adding 1 rather than 2 (as in Theorem 5.3)
                // so as to later test for < rather than <=.
                add_assign_r(lhs, lhs, temp_one, ROUND_NOT_NEEDED);
              }
              // Testing for < in both the rational and integer case.
              if (lhs < rhs)
                return false;
            }
          }
        }
      }
    }
  }
  // The upper bound of x and y is indeed exact.
  m_swap(ub);
  PPL_ASSERT(OK());
  return true;
}

template <typename T>
void
BD_Shape<T>::difference_assign(const BD_Shape& y) {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("difference_assign(y)", y);

  BD_Shape new_bd_shape(space_dim, EMPTY);

  BD_Shape& x = *this;

  x.shortest_path_closure_assign();
  // The difference of an empty bounded difference shape
  // and of a bounded difference shape `p' is empty.
  if (x.marked_empty())
    return;
  y.shortest_path_closure_assign();
  // The difference of a bounded difference shape `p'
  // and an empty bounded difference shape is `p'.
  if (y.marked_empty())
    return;

  // If both bounded difference shapes are zero-dimensional,
  // then at this point they are necessarily universe system of
  // bounded differences, so that their difference is empty.
  if (space_dim == 0) {
    x.set_empty();
    return;
  }

  // TODO: This is just an executable specification.
  //       Have to find a more efficient method.
  if (y.contains(x)) {
    x.set_empty();
    return;
  }

  // We take a constraint of the system y at the time and we
  // consider its complementary. Then we intersect the union
  // of these complementary constraints with the system x.
  const Constraint_System& y_cs = y.constraints();
  for (Constraint_System::const_iterator i = y_cs.begin(),
         y_cs_end = y_cs.end(); i != y_cs_end; ++i) {
    const Constraint& c = *i;
    // If the bounded difference shape `x' is included
    // in the bounded difference shape defined by `c',
    // then `c' _must_ be skipped, as adding its complement to `x'
    // would result in the empty bounded difference shape,
    // and as we would obtain a result that is less precise
    // than the bds-difference.
    if (x.relation_with(c).implies(Poly_Con_Relation::is_included()))
      continue;
    BD_Shape z = x;
    const Linear_Expression e(c.expression());
    z.add_constraint(e <= 0);
    if (!z.is_empty())
      new_bd_shape.upper_bound_assign(z);
    if (c.is_equality()) {
      z = x;
      z.add_constraint(e >= 0);
      if (!z.is_empty())
        new_bd_shape.upper_bound_assign(z);
    }
  }
  *this = new_bd_shape;
  PPL_ASSERT(OK());
}

template <typename T>
bool
BD_Shape<T>::simplify_using_context_assign(const BD_Shape& y) {
  BD_Shape& x = *this;
  const dimension_type dim = x.space_dimension();
  // Dimension-compatibility check.
  if (dim != y.space_dimension())
    throw_dimension_incompatible("simplify_using_context_assign(y)", y);

  // Filter away the zero-dimensional case.
  if (dim == 0) {
    if (y.marked_empty()) {
      x.set_zero_dim_univ();
      return false;
    }
    else
      return !x.marked_empty();
  }

  // Filter away the case where `x' contains `y'
  // (this subsumes the case when `y' is empty).
  y.shortest_path_closure_assign();
  if (x.contains(y)) {
    BD_Shape<T> res(dim, UNIVERSE);
    x.m_swap(res);
    return false;
  }

  // Filter away the case where `x' is empty.
  x.shortest_path_closure_assign();
  if (x.marked_empty()) {
    // Search for a constraint of `y' that is not a tautology.
    dimension_type i;
    dimension_type j;
    // Prefer unary constraints.
    i = 0;
    const DB_Row<N>& y_dbm_0 = y.dbm[0];
    for (j = 1; j <= dim; ++j) {
      if (!is_plus_infinity(y_dbm_0[j]))
        // FIXME: if N is a float or bounded integer type, then
        // we also need to check that we are actually able to construct
        // a constraint inconsistent with respect to this one.
        goto found;
    }
    j = 0;
    for (i = 1; i <= dim; ++i) {
      if (!is_plus_infinity(y.dbm[i][0]))
        // FIXME: if N is a float or bounded integer type, then
        // we also need to check that we are actually able to construct
        // a constraint inconsistent with respect to this one.
        goto found;
    }
    // Then search binary constraints.
    for (i = 1; i <= dim; ++i) {
      const DB_Row<N>& y_dbm_i = y.dbm[i];
      for (j = 1; j <= dim; ++j)
        if (!is_plus_infinity(y_dbm_i[j]))
          // FIXME: if N is a float or bounded integer type, then
          // we also need to check that we are actually able to construct
          // a constraint inconsistent with respect to this one.
          goto found;
    }
    // Not found: we were not able to build a constraint contradicting
    // one of the constraints in `y': `x' cannot be enlarged.
    return false;

  found:
    // Found: build a new BDS contradicting the constraint found.
    PPL_ASSERT(i <= dim && j <= dim && (i > 0 || j > 0));
    BD_Shape<T> res(dim, UNIVERSE);
    PPL_DIRTY_TEMP(N, tmp);
    assign_r(tmp, 1, ROUND_UP);
    add_assign_r(tmp, tmp, y.dbm[i][j], ROUND_UP);
    PPL_ASSERT(!is_plus_infinity(tmp));
    // CHECKME: round down is really meant.
    neg_assign_r(res.dbm[j][i], tmp, ROUND_DOWN);
    x.m_swap(res);
    return false;
  }

  // Here `x' and `y' are not empty and shortest-path closed;
  // also, `x' does not contain `y'.
  // Let `target' be the intersection of `x' and `y'.
  BD_Shape<T> target = x;
  target.intersection_assign(y);
  const bool bool_result = !target.is_empty();

  // Compute a reduced dbm for `x' and ...
  x.shortest_path_reduction_assign();
  // ... count the non-redundant constraints.
  dimension_type x_num_non_redundant = (dim+1)*(dim+1);
  for (dimension_type i = dim + 1; i-- > 0; )
    x_num_non_redundant -= x.redundancy_dbm[i].count_ones();
  PPL_ASSERT(x_num_non_redundant > 0);

  // Let `yy' be a copy of `y': we will keep adding to `yy'
  // the non-redundant constraints of `x',
  // stopping as soon as `yy' becomes equal to `target'.
  BD_Shape<T> yy = y;

  // The constraints added to `yy' will be recorded in `res' ...
  BD_Shape<T> res(dim, UNIVERSE);
  // ... and we will count them too.
  dimension_type res_num_non_redundant = 0;

  // Compute leader information for `x'.
  std::vector<dimension_type> x_leaders;
  x.compute_leaders(x_leaders);

  // First go through the unary equality constraints.
  const DB_Row<N>& x_dbm_0 = x.dbm[0];
  DB_Row<N>& yy_dbm_0 = yy.dbm[0];
  DB_Row<N>& res_dbm_0 = res.dbm[0];
  for (dimension_type j = 1; j <= dim; ++j) {
    // Unary equality constraints are encoded in entries dbm_0j and dbm_j0
    // provided index j has special variable index 0 as its leader.
    if (x_leaders[j] != 0)
      continue;
    PPL_ASSERT(!is_plus_infinity(x_dbm_0[j]));
    if (x_dbm_0[j] < yy_dbm_0[j]) {
      res_dbm_0[j] = x_dbm_0[j];
      ++res_num_non_redundant;
      // Tighten context `yy' using the newly added constraint.
      yy_dbm_0[j] = x_dbm_0[j];
      yy.reset_shortest_path_closed();
    }
    PPL_ASSERT(!is_plus_infinity(x.dbm[j][0]));
    if (x.dbm[j][0] < yy.dbm[j][0]) {
      res.dbm[j][0] = x.dbm[j][0];
      ++res_num_non_redundant;
      // Tighten context `yy' using the newly added constraint.
      yy.dbm[j][0] = x.dbm[j][0];
      yy.reset_shortest_path_closed();
    }
    // Restore shortest-path closure, if it was lost.
    if (!yy.marked_shortest_path_closed()) {
      Variable var_j(j-1);
      yy.incremental_shortest_path_closure_assign(var_j);
      if (target.contains(yy)) {
        // Target reached: swap `x' and `res' if needed.
        if (res_num_non_redundant < x_num_non_redundant) {
          res.reset_shortest_path_closed();
          x.m_swap(res);
        }
        return bool_result;
      }
    }
  }

  // Go through the binary equality constraints.
  // Note: no need to consider the case i == 1.
  for (dimension_type i = 2; i <= dim; ++i) {
    const dimension_type j = x_leaders[i];
    if (j == i || j == 0)
      continue;
    PPL_ASSERT(!is_plus_infinity(x.dbm[i][j]));
    if (x.dbm[i][j] < yy.dbm[i][j]) {
      res.dbm[i][j] = x.dbm[i][j];
      ++res_num_non_redundant;
      // Tighten context `yy' using the newly added constraint.
      yy.dbm[i][j] = x.dbm[i][j];
      yy.reset_shortest_path_closed();
    }
    PPL_ASSERT(!is_plus_infinity(x.dbm[j][i]));
    if (x.dbm[j][i] < yy.dbm[j][i]) {
      res.dbm[j][i] = x.dbm[j][i];
      ++res_num_non_redundant;
      // Tighten context `yy' using the newly added constraint.
      yy.dbm[j][i] = x.dbm[j][i];
      yy.reset_shortest_path_closed();
    }
    // Restore shortest-path closure, if it was lost.
    if (!yy.marked_shortest_path_closed()) {
      Variable var_j(j-1);
      yy.incremental_shortest_path_closure_assign(var_j);
      if (target.contains(yy)) {
        // Target reached: swap `x' and `res' if needed.
        if (res_num_non_redundant < x_num_non_redundant) {
          res.reset_shortest_path_closed();
          x.m_swap(res);
        }
        return bool_result;
      }
    }
  }

  // Finally go through the (proper) inequality constraints:
  // both indices i and j should be leaders.
  for (dimension_type i = 0; i <= dim; ++i) {
    if (i != x_leaders[i])
      continue;
    const DB_Row<N>& x_dbm_i = x.dbm[i];
    const Bit_Row& x_redundancy_dbm_i = x.redundancy_dbm[i];
    DB_Row<N>& yy_dbm_i = yy.dbm[i];
    DB_Row<N>& res_dbm_i = res.dbm[i];
    for (dimension_type j = 0; j <= dim; ++j) {
      if (j != x_leaders[j] || x_redundancy_dbm_i[j])
        continue;
      N& yy_dbm_ij = yy_dbm_i[j];
      const N& x_dbm_ij = x_dbm_i[j];
      if (x_dbm_ij < yy_dbm_ij) {
        res_dbm_i[j] = x_dbm_ij;
        ++res_num_non_redundant;
        // Tighten context `yy' using the newly added constraint.
        yy_dbm_ij = x_dbm_ij;
        yy.reset_shortest_path_closed();
        PPL_ASSERT(i > 0 || j > 0);
        Variable var(((i > 0) ? i : j) - 1);
        yy.incremental_shortest_path_closure_assign(var);
        if (target.contains(yy)) {
          // Target reached: swap `x' and `res' if needed.
          if (res_num_non_redundant < x_num_non_redundant) {
            res.reset_shortest_path_closed();
            x.m_swap(res);
          }
          return bool_result;
        }
      }
    }
  }
  // This point should be unreachable.
  PPL_UNREACHABLE;
  return false;
}

template <typename T>
void
BD_Shape<T>::add_space_dimensions_and_embed(const dimension_type m) {
  // Adding no dimensions is a no-op.
  if (m == 0)
    return;

  const dimension_type space_dim = space_dimension();
  const dimension_type new_space_dim = space_dim + m;
  const bool was_zero_dim_univ = (!marked_empty() && space_dim == 0);

  // To embed an n-dimension space BDS in a (n+m)-dimension space,
  // we just add `m' rows and columns in the bounded difference shape,
  // initialized to PLUS_INFINITY.
  dbm.grow(new_space_dim + 1);

  // Shortest-path closure is maintained (if it was holding).
  // TODO: see whether reduction can be (efficiently!) maintained too.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();

  // If `*this' was the zero-dim space universe BDS,
  // the we can set the shortest-path closure flag.
  if (was_zero_dim_univ)
    set_shortest_path_closed();

  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::add_space_dimensions_and_project(const dimension_type m) {
  // Adding no dimensions is a no-op.
  if (m == 0)
    return;

  const dimension_type space_dim = space_dimension();

  // If `*this' was zero-dimensional, then we add `m' rows and columns.
  // If it also was non-empty, then we zero all the added elements
  // and set the flag for shortest-path closure.
  if (space_dim == 0) {
    dbm.grow(m + 1);
    if (!marked_empty()) {
      for (dimension_type i = m + 1; i-- > 0; ) {
        DB_Row<N>& dbm_i = dbm[i];
        for (dimension_type j = m + 1; j-- > 0; )
          if (i != j)
            assign_r(dbm_i[j], 0, ROUND_NOT_NEEDED);
      }
      set_shortest_path_closed();
    }
    PPL_ASSERT(OK());
    return;
  }

  // To project an n-dimension space bounded difference shape
  // in a (n+m)-dimension space, we add `m' rows and columns.
  // In the first row and column of the matrix we add `zero' from
  // the (n+1)-th position to the end.
  const dimension_type new_space_dim = space_dim + m;
  dbm.grow(new_space_dim + 1);

  // Bottom of the matrix and first row.
  DB_Row<N>& dbm_0 = dbm[0];
  for (dimension_type i = space_dim + 1; i <= new_space_dim; ++i) {
    assign_r(dbm[i][0], 0, ROUND_NOT_NEEDED);
    assign_r(dbm_0[i], 0, ROUND_NOT_NEEDED);
  }

  if (marked_shortest_path_closed())
    reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::remove_space_dimensions(const Variables_Set& vars) {
  // The removal of no dimensions from any BDS is a no-op.
  // Note that this case also captures the only legal removal of
  // space dimensions from a BDS in a 0-dim space.
  if (vars.empty()) {
    PPL_ASSERT(OK());
    return;
  }

  const dimension_type old_space_dim = space_dimension();

  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (old_space_dim < min_space_dim)
    throw_dimension_incompatible("remove_space_dimensions(vs)", min_space_dim);

  // Shortest-path closure is necessary to keep precision.
  shortest_path_closure_assign();

  // When removing _all_ dimensions from a BDS, we obtain the
  // zero-dimensional BDS.
  const dimension_type new_space_dim = old_space_dim - vars.size();
  if (new_space_dim == 0) {
    dbm.resize_no_copy(1);
    if (!marked_empty())
      // We set the zero_dim_univ flag.
      set_zero_dim_univ();
    PPL_ASSERT(OK());
    return;
  }

  // Handle the case of an empty BD_Shape.
  if (marked_empty()) {
    dbm.resize_no_copy(new_space_dim + 1);
    PPL_ASSERT(OK());
    return;
  }

  // Shortest-path closure is maintained.
  // TODO: see whether reduction can be (efficiently!) maintained too.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();

  // For each variable to remove, we fill the corresponding column and
  // row by shifting respectively left and above those
  // columns and rows, that will not be removed.
  Variables_Set::const_iterator vsi = vars.begin();
  Variables_Set::const_iterator vsi_end = vars.end();
  dimension_type dst = *vsi + 1;
  dimension_type src = dst + 1;
  for (++vsi; vsi != vsi_end; ++vsi) {
    const dimension_type vsi_next = *vsi + 1;
    // All other columns and rows are moved respectively to the left
    // and above.
    while (src < vsi_next) {
      using std::swap;
      swap(dbm[dst], dbm[src]);
      for (dimension_type i = old_space_dim + 1; i-- > 0; ) {
        DB_Row<N>& dbm_i = dbm[i];
        assign_or_swap(dbm_i[dst], dbm_i[src]);
      }
      ++dst;
      ++src;
    }
    ++src;
  }

  // Moving the remaining rows and columns.
  while (src <= old_space_dim) {
    using std::swap;
    swap(dbm[dst], dbm[src]);
    for (dimension_type i = old_space_dim + 1; i-- > 0; ) {
      DB_Row<N>& dbm_i = dbm[i];
      assign_or_swap(dbm_i[dst], dbm_i[src]);
    }
    ++src;
    ++dst;
  }

  // Update the space dimension.
  dbm.resize_no_copy(new_space_dim + 1);
  PPL_ASSERT(OK());
}

template <typename T>
template <typename Partial_Function>
void
BD_Shape<T>::map_space_dimensions(const Partial_Function& pfunc) {
  const dimension_type space_dim = space_dimension();
  // TODO: this implementation is just an executable specification.
  if (space_dim == 0)
    return;

  if (pfunc.has_empty_codomain()) {
    // All dimensions vanish: the BDS becomes zero_dimensional.
    remove_higher_space_dimensions(0);
    return;
  }

  const dimension_type new_space_dim = pfunc.max_in_codomain() + 1;
  // If we are going to actually reduce the space dimension,
  // then shortest-path closure is required to keep precision.
  if (new_space_dim < space_dim)
    shortest_path_closure_assign();

  // If the BDS is empty, then it is sufficient to adjust the
  // space dimension of the bounded difference shape.
  if (marked_empty()) {
    remove_higher_space_dimensions(new_space_dim);
    return;
  }

  // Shortest-path closure is maintained (if it was holding).
  // TODO: see whether reduction can be (efficiently!) maintained too.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();

  // We create a new matrix with the new space dimension.
  DB_Matrix<N> x(new_space_dim+1);
  // First of all we must map the unary constraints, because
  // there is the fictitious variable `zero', that can't be mapped
  // at all.
  DB_Row<N>& dbm_0 = dbm[0];
  DB_Row<N>& x_0 = x[0];
  for (dimension_type j = 1; j <= space_dim; ++j) {
    dimension_type new_j;
    if (pfunc.maps(j - 1, new_j)) {
      assign_or_swap(x_0[new_j + 1], dbm_0[j]);
      assign_or_swap(x[new_j + 1][0], dbm[j][0]);
    }
  }
  // Now we map the binary constraints, exchanging the indexes.
  for (dimension_type i = 1; i <= space_dim; ++i) {
    dimension_type new_i;
    if (pfunc.maps(i - 1, new_i)) {
      DB_Row<N>& dbm_i = dbm[i];
      ++new_i;
      DB_Row<N>& x_new_i = x[new_i];
      for (dimension_type j = i+1; j <= space_dim; ++j) {
        dimension_type new_j;
        if (pfunc.maps(j - 1, new_j)) {
          ++new_j;
          assign_or_swap(x_new_i[new_j], dbm_i[j]);
          assign_or_swap(x[new_j][new_i], dbm[j][i]);
        }
      }
    }
  }

  using std::swap;
  swap(dbm, x);
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::intersection_assign(const BD_Shape& y) {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("intersection_assign(y)", y);

  // If one of the two bounded difference shapes is empty,
  // the intersection is empty.
  if (marked_empty())
    return;
  if (y.marked_empty()) {
    set_empty();
    return;
  }

  // If both bounded difference shapes are zero-dimensional,
  // then at this point they are necessarily non-empty,
  // so that their intersection is non-empty too.
  if (space_dim == 0)
    return;

  // To intersect two bounded difference shapes we compare
  // the constraints and we choose the less values.
  bool changed = false;
  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    DB_Row<N>& dbm_i = dbm[i];
    const DB_Row<N>& y_dbm_i = y.dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; ) {
      N& dbm_ij = dbm_i[j];
      const N& y_dbm_ij = y_dbm_i[j];
      if (dbm_ij > y_dbm_ij) {
        dbm_ij = y_dbm_ij;
        changed = true;
      }
    }
  }

  if (changed && marked_shortest_path_closed())
    reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
template <typename Iterator>
void
BD_Shape<T>::CC76_extrapolation_assign(const BD_Shape& y,
                                       Iterator first, Iterator last,
                                       unsigned* tp) {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("CC76_extrapolation_assign(y)", y);

  // We assume that `y' is contained in or equal to `*this'.
  PPL_EXPECT_HEAVY(copy_contains(*this, y));

  // If both bounded difference shapes are zero-dimensional,
  // since `*this' contains `y', we simply return `*this'.
  if (space_dim == 0)
    return;

  shortest_path_closure_assign();
  // If `*this' is empty, since `*this' contains `y', `y' is empty too.
  if (marked_empty())
    return;
  y.shortest_path_closure_assign();
  // If `y' is empty, we return.
  if (y.marked_empty())
    return;

  // If there are tokens available, work on a temporary copy.
  if (tp != 0 && *tp > 0) {
    BD_Shape<T> x_tmp(*this);
    x_tmp.CC76_extrapolation_assign(y, first, last, 0);
    // If the widening was not precise, use one of the available tokens.
    if (!contains(x_tmp))
      --(*tp);
    return;
  }

  // Compare each constraint in `y' to the corresponding one in `*this'.
  // The constraint in `*this' is kept as is if it is stronger than or
  // equal to the constraint in `y'; otherwise, the inhomogeneous term
  // of the constraint in `*this' is further compared with elements taken
  // from a sorted container (the stop-points, provided by the user), and
  // is replaced by the first entry, if any, which is greater than or equal
  // to the inhomogeneous term. If no such entry exists, the constraint
  // is removed altogether.
  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    DB_Row<N>& dbm_i = dbm[i];
    const DB_Row<N>& y_dbm_i = y.dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; ) {
      N& dbm_ij = dbm_i[j];
      const N& y_dbm_ij = y_dbm_i[j];
      if (y_dbm_ij < dbm_ij) {
        Iterator k = std::lower_bound(first, last, dbm_ij);
        if (k != last) {
          if (dbm_ij < *k)
            assign_r(dbm_ij, *k, ROUND_UP);
        }
        else
          assign_r(dbm_ij, PLUS_INFINITY, ROUND_NOT_NEEDED);
      }
    }
  }
  reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::get_limiting_shape(const Constraint_System& cs,
                                BD_Shape& limiting_shape) const {
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(cs.space_dimension() <= space_dimension());

  shortest_path_closure_assign();
  bool changed = false;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_c_term);
  PPL_DIRTY_TEMP(N, d);
  PPL_DIRTY_TEMP(N, d1);
  for (Constraint_System::const_iterator cs_i = cs.begin(),
         cs_end = cs.end(); cs_i != cs_end; ++cs_i) {
    const Constraint& c = *cs_i;
    dimension_type num_vars = 0;
    dimension_type i = 0;
    dimension_type j = 0;
    // Constraints that are not bounded differences are ignored.
    if (BD_Shape_Helpers::extract_bounded_difference(c, num_vars, i, j, coeff)) {
      // Select the cell to be modified for the "<=" part of the constraint,
      // and set `coeff' to the absolute value of itself.
      const bool negative = (coeff < 0);
      const N& x = negative ? dbm[i][j] : dbm[j][i];
      const N& y = negative ? dbm[j][i] : dbm[i][j];
      DB_Matrix<N>& ls_dbm = limiting_shape.dbm;
      if (negative)
        neg_assign(coeff);
      // Compute the bound for `x', rounding towards plus infinity.
      div_round_up(d, c.inhomogeneous_term(), coeff);
      if (x <= d) {
        if (c.is_inequality()) {
          N& ls_x = negative ? ls_dbm[i][j] : ls_dbm[j][i];
          if (ls_x > d) {
            ls_x = d;
            changed = true;
          }
        }
        else {
          // Compute the bound for `y', rounding towards plus infinity.
          neg_assign(minus_c_term, c.inhomogeneous_term());
          div_round_up(d1, minus_c_term, coeff);
          if (y <= d1) {
            N& ls_x = negative ? ls_dbm[i][j] : ls_dbm[j][i];
            N& ls_y = negative ? ls_dbm[j][i] : ls_dbm[i][j];
            if ((ls_x >= d && ls_y > d1) || (ls_x > d && ls_y >= d1)) {
              ls_x = d;
              ls_y = d1;
              changed = true;
            }
          }
        }
      }
    }
  }

  // In general, adding a constraint does not preserve the shortest-path
  // closure of the bounded difference shape.
  if (changed && limiting_shape.marked_shortest_path_closed())
    limiting_shape.reset_shortest_path_closed();
}

template <typename T>
void
BD_Shape<T>::limited_CC76_extrapolation_assign(const BD_Shape& y,
                                               const Constraint_System& cs,
                                               unsigned* tp) {
  // Dimension-compatibility check.
  const dimension_type space_dim = space_dimension();
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("limited_CC76_extrapolation_assign(y, cs)",
                                 y);

  // `cs' must be dimension-compatible with the two systems
  // of bounded differences.
  const dimension_type cs_space_dim = cs.space_dimension();
  if (space_dim < cs_space_dim)
    throw_invalid_argument("limited_CC76_extrapolation_assign(y, cs)",
                           "cs is space_dimension incompatible");

  // Strict inequalities not allowed.
  if (cs.has_strict_inequalities())
    throw_invalid_argument("limited_CC76_extrapolation_assign(y, cs)",
                           "cs has strict inequalities");

  // The limited CC76-extrapolation between two systems of bounded
  // differences in a zero-dimensional space is a system of bounded
  // differences in a zero-dimensional space, too.
  if (space_dim == 0)
    return;

  // We assume that `y' is contained in or equal to `*this'.
  PPL_EXPECT_HEAVY(copy_contains(*this, y));

  // If `*this' is empty, since `*this' contains `y', `y' is empty too.
  if (marked_empty())
    return;
  // If `y' is empty, we return.
  if (y.marked_empty())
    return;

  BD_Shape<T> limiting_shape(space_dim, UNIVERSE);
  get_limiting_shape(cs, limiting_shape);
  CC76_extrapolation_assign(y, tp);
  intersection_assign(limiting_shape);
}

template <typename T>
void
BD_Shape<T>::BHMZ05_widening_assign(const BD_Shape& y, unsigned* tp) {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("BHMZ05_widening_assign(y)", y);

  // We assume that `y' is contained in or equal to `*this'.
  PPL_EXPECT_HEAVY(copy_contains(*this, y));

  // Compute the affine dimension of `y'.
  const dimension_type y_affine_dim = y.affine_dimension();
  // If the affine dimension of `y' is zero, then either `y' is
  // zero-dimensional, or it is empty, or it is a singleton.
  // In all cases, due to the inclusion hypothesis, the result is `*this'.
  if (y_affine_dim == 0)
    return;

  // If the affine dimension has changed, due to the inclusion hypothesis,
  // the result is `*this'.
  const dimension_type x_affine_dim = affine_dimension();
  PPL_ASSERT(x_affine_dim >= y_affine_dim);
  if (x_affine_dim != y_affine_dim)
    return;

  // If there are tokens available, work on a temporary copy.
  if (tp != 0 && *tp > 0) {
    BD_Shape<T> x_tmp(*this);
    x_tmp.BHMZ05_widening_assign(y, 0);
    // If the widening was not precise, use one of the available tokens.
    if (!contains(x_tmp))
      --(*tp);
    return;
  }

  // Here no token is available.
  PPL_ASSERT(marked_shortest_path_closed() && y.marked_shortest_path_closed());
  // Minimize `y'.
  y.shortest_path_reduction_assign();

  // Extrapolate unstable bounds, taking into account redundancy in `y'.
  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    DB_Row<N>& dbm_i = dbm[i];
    const DB_Row<N>& y_dbm_i = y.dbm[i];
    const Bit_Row& y_redundancy_i = y.redundancy_dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; ) {
      N& dbm_ij = dbm_i[j];
      // Note: in the following line the use of `!=' (as opposed to
      // the use of `<' that would seem -but is not- equivalent) is
      // intentional.
      if (y_redundancy_i[j] || y_dbm_i[j] != dbm_ij)
        assign_r(dbm_ij, PLUS_INFINITY, ROUND_NOT_NEEDED);
    }
  }
  // NOTE: this will also reset the shortest-path reduction flag,
  // even though the dbm is still in reduced form. However, the
  // current implementation invariant requires that any reduced dbm
  // is closed too.
  reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::limited_BHMZ05_extrapolation_assign(const BD_Shape& y,
                                                 const Constraint_System& cs,
                                                 unsigned* tp) {
  // Dimension-compatibility check.
  const dimension_type space_dim = space_dimension();
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("limited_BHMZ05_extrapolation_assign(y, cs)",
                                 y);
  // `cs' must be dimension-compatible with the two systems
  // of bounded differences.
  const dimension_type cs_space_dim = cs.space_dimension();
  if (space_dim < cs_space_dim)
    throw_invalid_argument("limited_BHMZ05_extrapolation_assign(y, cs)",
                           "cs is space-dimension incompatible");

  // Strict inequalities are not allowed.
  if (cs.has_strict_inequalities())
    throw_invalid_argument("limited_BHMZ05_extrapolation_assign(y, cs)",
                           "cs has strict inequalities");

  // The limited BHMZ05-extrapolation between two systems of bounded
  // differences in a zero-dimensional space is a system of bounded
  // differences in a zero-dimensional space, too.
  if (space_dim == 0)
    return;

  // We assume that `y' is contained in or equal to `*this'.
  PPL_EXPECT_HEAVY(copy_contains(*this, y));

  // If `*this' is empty, since `*this' contains `y', `y' is empty too.
  if (marked_empty())
    return;
  // If `y' is empty, we return.
  if (y.marked_empty())
    return;

  BD_Shape<T> limiting_shape(space_dim, UNIVERSE);
  get_limiting_shape(cs, limiting_shape);
  BHMZ05_widening_assign(y, tp);
  intersection_assign(limiting_shape);
}

template <typename T>
void
BD_Shape<T>::CC76_narrowing_assign(const BD_Shape& y) {
  const dimension_type space_dim = space_dimension();

  // Dimension-compatibility check.
  if (space_dim != y.space_dimension())
    throw_dimension_incompatible("CC76_narrowing_assign(y)", y);

  // We assume that `*this' is contained in or equal to `y'.
  PPL_EXPECT_HEAVY(copy_contains(y, *this));

  // If both bounded difference shapes are zero-dimensional,
  // since `y' contains `*this', we simply return `*this'.
  if (space_dim == 0)
    return;

  y.shortest_path_closure_assign();
  // If `y' is empty, since `y' contains `this', `*this' is empty too.
  if (y.marked_empty())
    return;
  shortest_path_closure_assign();
  // If `*this' is empty, we return.
  if (marked_empty())
    return;

  // Replace each constraint in `*this' by the corresponding constraint
  // in `y' if the corresponding inhomogeneous terms are both finite.
  bool changed = false;
  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    DB_Row<N>& dbm_i = dbm[i];
    const DB_Row<N>& y_dbm_i = y.dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; ) {
      N& dbm_ij = dbm_i[j];
      const N& y_dbm_ij = y_dbm_i[j];
      if (!is_plus_infinity(dbm_ij)
          && !is_plus_infinity(y_dbm_ij)
          && dbm_ij != y_dbm_ij) {
        dbm_ij = y_dbm_ij;
        changed = true;
      }
    }
  }
  if (changed && marked_shortest_path_closed())
    reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>
::deduce_v_minus_u_bounds(const dimension_type v,
                          const dimension_type last_v,
                          const Linear_Expression& sc_expr,
                          Coefficient_traits::const_reference sc_denom,
                          const N& ub_v) {
  PPL_ASSERT(sc_denom > 0);
  PPL_ASSERT(!is_plus_infinity(ub_v));
  // Deduce constraints of the form `v - u', where `u != v'.
  // Note: the shortest-path closure is able to deduce the constraint
  // `v - u <= ub_v - lb_u'. We can be more precise if variable `u'
  // played an active role in the computation of the upper bound for `v',
  // i.e., if the corresponding coefficient `q == expr_u/denom' is
  // greater than zero. In particular:
  // if `q >= 1',    then `v - u <= ub_v - ub_u';
  // if `0 < q < 1', then `v - u <= ub_v - (q*ub_u + (1-q)*lb_u)'.
  PPL_DIRTY_TEMP(mpq_class, mpq_sc_denom);
  assign_r(mpq_sc_denom, sc_denom, ROUND_NOT_NEEDED);
  const DB_Row<N>& dbm_0 = dbm[0];
  // Speculative allocation of temporaries to be used in the following loop.
  PPL_DIRTY_TEMP(mpq_class, minus_lb_u);
  PPL_DIRTY_TEMP(mpq_class, q);
  PPL_DIRTY_TEMP(mpq_class, ub_u);
  PPL_DIRTY_TEMP(N, up_approx);
  for (Linear_Expression::const_iterator u = sc_expr.begin(),
        u_end = sc_expr.lower_bound(Variable(last_v)); u != u_end; ++u) {
    const dimension_type u_dim = u.variable().space_dimension();
    if (u_dim == v)
      continue;
    const Coefficient& expr_u = *u;
    if (expr_u < 0)
      continue;
    PPL_ASSERT(expr_u > 0);
    if (expr_u >= sc_denom)
      // Deducing `v - u <= ub_v - ub_u'.
      sub_assign_r(dbm[u_dim][v], ub_v, dbm_0[u_dim], ROUND_UP);
    else {
      DB_Row<N>& dbm_u = dbm[u_dim];
      const N& dbm_u0 = dbm_u[0];
      if (!is_plus_infinity(dbm_u0)) {
        // Let `ub_u' and `lb_u' be the known upper and lower bound
        // for `u', respectively. Letting `q = expr_u/sc_denom' be the
        // rational coefficient of `u' in `sc_expr/sc_denom',
        // the upper bound for `v - u' is computed as
        // `ub_v - (q * ub_u + (1-q) * lb_u)', i.e.,
        // `ub_v + (-lb_u) - q * (ub_u + (-lb_u))'.
        assign_r(minus_lb_u, dbm_u0, ROUND_NOT_NEEDED);
        assign_r(q, expr_u, ROUND_NOT_NEEDED);
        div_assign_r(q, q, mpq_sc_denom, ROUND_NOT_NEEDED);
        assign_r(ub_u, dbm_0[u_dim], ROUND_NOT_NEEDED);
        // Compute `ub_u - lb_u'.
        add_assign_r(ub_u, ub_u, minus_lb_u, ROUND_NOT_NEEDED);
        // Compute `(-lb_u) - q * (ub_u - lb_u)'.
        sub_mul_assign_r(minus_lb_u, q, ub_u, ROUND_NOT_NEEDED);
        assign_r(up_approx, minus_lb_u, ROUND_UP);
        // Deducing `v - u <= ub_v - (q * ub_u + (1-q) * lb_u)'.
        add_assign_r(dbm_u[v], ub_v, up_approx, ROUND_UP);
      }
    }
  }
}

template <typename T>
void
BD_Shape<T>
::deduce_u_minus_v_bounds(const dimension_type v,
                          const dimension_type last_v,
                          const Linear_Expression& sc_expr,
                          Coefficient_traits::const_reference sc_denom,
                          const N& minus_lb_v) {
  PPL_ASSERT(sc_denom > 0);
  PPL_ASSERT(!is_plus_infinity(minus_lb_v));
  // Deduce constraints of the form `u - v', where `u != v'.
  // Note: the shortest-path closure is able to deduce the constraint
  // `u - v <= ub_u - lb_v'. We can be more precise if variable `u'
  // played an active role in the computation of the lower bound for `v',
  // i.e., if the corresponding coefficient `q == expr_u/denom' is
  // greater than zero. In particular:
  // if `q >= 1',    then `u - v <= lb_u - lb_v';
  // if `0 < q < 1', then `u - v <= (q*lb_u + (1-q)*ub_u) - lb_v'.
  PPL_DIRTY_TEMP(mpq_class, mpq_sc_denom);
  assign_r(mpq_sc_denom, sc_denom, ROUND_NOT_NEEDED);
  DB_Row<N>& dbm_0 = dbm[0];
  DB_Row<N>& dbm_v = dbm[v];
  // Speculative allocation of temporaries to be used in the following loop.
  PPL_DIRTY_TEMP(mpq_class, ub_u);
  PPL_DIRTY_TEMP(mpq_class, q);
  PPL_DIRTY_TEMP(mpq_class, minus_lb_u);
  PPL_DIRTY_TEMP(N, up_approx);
  // No need to consider indices greater than `last_v'.
  for (Linear_Expression::const_iterator u = sc_expr.begin(),
        u_end = sc_expr.lower_bound(Variable(last_v)); u != u_end; ++u) {
    const Variable u_var = u.variable();
    const dimension_type u_dim = u_var.space_dimension();
    if (u_var.space_dimension() == v)
      continue;
    const Coefficient& expr_u = *u;
    if (expr_u < 0)
      continue;
    PPL_ASSERT(expr_u > 0);
    if (expr_u >= sc_denom)
      // Deducing `u - v <= lb_u - lb_v',
      // i.e., `u - v <= (-lb_v) - (-lb_u)'.
      sub_assign_r(dbm_v[u_dim], minus_lb_v, dbm[u_dim][0], ROUND_UP);
    else {
      const N& dbm_0u = dbm_0[u_dim];
      if (!is_plus_infinity(dbm_0u)) {
        // Let `ub_u' and `lb_u' be the known upper and lower bound
        // for `u', respectively. Letting `q = expr_u/sc_denom' be the
        // rational coefficient of `u' in `sc_expr/sc_denom',
        // the upper bound for `u - v' is computed as
        // `(q * lb_u + (1-q) * ub_u) - lb_v', i.e.,
        // `ub_u - q * (ub_u + (-lb_u)) + minus_lb_v'.
        assign_r(ub_u, dbm_0u, ROUND_NOT_NEEDED);
        assign_r(q, expr_u, ROUND_NOT_NEEDED);
        div_assign_r(q, q, mpq_sc_denom, ROUND_NOT_NEEDED);
        assign_r(minus_lb_u, dbm[u_dim][0], ROUND_NOT_NEEDED);
        // Compute `ub_u - lb_u'.
        add_assign_r(minus_lb_u, minus_lb_u, ub_u, ROUND_NOT_NEEDED);
        // Compute `ub_u - q * (ub_u - lb_u)'.
        sub_mul_assign_r(ub_u, q, minus_lb_u, ROUND_NOT_NEEDED);
        assign_r(up_approx, ub_u, ROUND_UP);
        // Deducing `u - v <= (q*lb_u + (1-q)*ub_u) - lb_v'.
        add_assign_r(dbm_v[u_dim], up_approx, minus_lb_v, ROUND_UP);
      }
    }
  }
}

template <typename T>
void
BD_Shape<T>::forget_all_dbm_constraints(const dimension_type v) {
  PPL_ASSERT(0 < v && v <= dbm.num_rows());
  DB_Row<N>& dbm_v = dbm[v];
  for (dimension_type i = dbm.num_rows(); i-- > 0; ) {
    assign_r(dbm_v[i], PLUS_INFINITY, ROUND_NOT_NEEDED);
    assign_r(dbm[i][v], PLUS_INFINITY, ROUND_NOT_NEEDED);
  }
}

template <typename T>
void
BD_Shape<T>::forget_binary_dbm_constraints(const dimension_type v) {
  PPL_ASSERT(0 < v && v <= dbm.num_rows());
  DB_Row<N>& dbm_v = dbm[v];
  for (dimension_type i = dbm.num_rows()-1; i > 0; --i) {
    assign_r(dbm_v[i], PLUS_INFINITY, ROUND_NOT_NEEDED);
    assign_r(dbm[i][v], PLUS_INFINITY, ROUND_NOT_NEEDED);
  }
}

template <typename T>
void
BD_Shape<T>::unconstrain(const Variable var) {
  // Dimension-compatibility check.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dimension() < var_space_dim)
    throw_dimension_incompatible("unconstrain(var)", var_space_dim);

  // Shortest-path closure is necessary to detect emptiness
  // and all (possibly implicit) constraints.
  shortest_path_closure_assign();

  // If the shape is empty, this is a no-op.
  if (marked_empty())
    return;

  forget_all_dbm_constraints(var_space_dim);
  // Shortest-path closure is preserved, but not reduction.
  reset_shortest_path_reduced();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::unconstrain(const Variables_Set& vars) {
  // The cylindrification with respect to no dimensions is a no-op.
  // This case captures the only legal cylindrification in a 0-dim space.
  if (vars.empty())
    return;

  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dimension() < min_space_dim)
    throw_dimension_incompatible("unconstrain(vs)", min_space_dim);

  // Shortest-path closure is necessary to detect emptiness
  // and all (possibly implicit) constraints.
  shortest_path_closure_assign();

  // If the shape is empty, this is a no-op.
  if (marked_empty())
    return;

  for (Variables_Set::const_iterator vsi = vars.begin(),
         vsi_end = vars.end(); vsi != vsi_end; ++vsi)
    forget_all_dbm_constraints(*vsi + 1);
  // Shortest-path closure is preserved, but not reduction.
  reset_shortest_path_reduced();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::refine(const Variable var,
                    const Relation_Symbol relsym,
                    const Linear_Expression& expr,
                    Coefficient_traits::const_reference denominator) {
  PPL_ASSERT(denominator != 0);
  PPL_ASSERT(space_dimension() >= expr.space_dimension());
  const dimension_type v = var.id() + 1;
  PPL_ASSERT(v <= space_dimension());
  PPL_ASSERT(expr.coefficient(var) == 0);
  PPL_ASSERT(relsym != LESS_THAN && relsym != GREATER_THAN);

  const Coefficient& b = expr.inhomogeneous_term();
  // Number of non-zero coefficients in `expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Index of the last non-zero coefficient in `expr', if any.
  dimension_type w = expr.last_nonzero();

  if (w != 0) {
    ++t;
    if (!expr.all_zeroes(1, w))
      ++t;
  }

  // Since we are only able to record bounded differences, we can
  // precisely deal with the case of a single variable only if its
  // coefficient (taking into account the denominator) is 1.
  // If this is not the case, we fall back to the general case
  // so as to over-approximate the constraint.
  if (t == 1 && expr.get(Variable(w - 1)) != denominator)
    t = 2;

  // Now we know the form of `expr':
  // - If t == 0, then expr == b, with `b' a constant;
  // - If t == 1, then expr == a*w + b, where `w != v' and `a == denominator';
  // - If t == 2, the `expr' is of the general form.
  const DB_Row<N>& dbm_0 = dbm[0];
  PPL_DIRTY_TEMP_COEFFICIENT(minus_denom);
  neg_assign(minus_denom, denominator);

  if (t == 0) {
    // Case 1: expr == b.
    switch (relsym) {
    case EQUAL:
      // Add the constraint `var == b/denominator'.
      add_dbm_constraint(0, v, b, denominator);
      add_dbm_constraint(v, 0, b, minus_denom);
      break;
    case LESS_OR_EQUAL:
      // Add the constraint `var <= b/denominator'.
      add_dbm_constraint(0, v, b, denominator);
      break;
    case GREATER_OR_EQUAL:
      // Add the constraint `var >= b/denominator',
      // i.e., `-var <= -b/denominator',
      add_dbm_constraint(v, 0, b, minus_denom);
      break;
    default:
      // We already dealt with the other cases.
      PPL_UNREACHABLE;
      break;
    }
    return;
  }

  if (t == 1) {
    // Case 2: expr == a*w + b, w != v, a == denominator.
    PPL_ASSERT(expr.get(Variable(w - 1)) == denominator);
    PPL_DIRTY_TEMP(N, d);
    switch (relsym) {
    case EQUAL:
      // Add the new constraint `v - w <= b/denominator'.
      div_round_up(d, b, denominator);
      add_dbm_constraint(w, v, d);
      // Add the new constraint `v - w >= b/denominator',
      // i.e., `w - v <= -b/denominator'.
      div_round_up(d, b, minus_denom);
      add_dbm_constraint(v, w, d);
      break;
    case LESS_OR_EQUAL:
      // Add the new constraint `v - w <= b/denominator'.
      div_round_up(d, b, denominator);
      add_dbm_constraint(w, v, d);
      break;
    case GREATER_OR_EQUAL:
      // Add the new constraint `v - w >= b/denominator',
      // i.e., `w - v <= -b/denominator'.
      div_round_up(d, b, minus_denom);
      add_dbm_constraint(v, w, d);
      break;
    default:
      // We already dealt with the other cases.
      PPL_UNREACHABLE;
      break;
    }
    return;
  }

  // Here t == 2, so that either
  // expr == a_1*x_1 + a_2*x_2 + ... + a_n*x_n + b, where n >= 2, or
  // expr == a*w + b, w != v and a != denominator.
  const bool is_sc = (denominator > 0);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_b);
  neg_assign(minus_b, b);
  const Coefficient& sc_b = is_sc ? b : minus_b;
  const Coefficient& minus_sc_b = is_sc ? minus_b : b;
  const Coefficient& sc_denom = is_sc ? denominator : minus_denom;
  const Coefficient& minus_sc_denom = is_sc ? minus_denom : denominator;
  // NOTE: here, for optimization purposes, `minus_expr' is only assigned
  // when `denominator' is negative. Do not use it unless you are sure
  // it has been correctly assigned.
  Linear_Expression minus_expr;
  if (!is_sc)
    minus_expr = -expr;
  const Linear_Expression& sc_expr = is_sc ? expr : minus_expr;

  PPL_DIRTY_TEMP(N, sum);
  // Indices of the variables that are unbounded in `this->dbm'.
  PPL_UNINITIALIZED(dimension_type, pinf_index);
  // Number of unbounded variables found.
  dimension_type pinf_count = 0;

  // Speculative allocation of temporaries that are used in most
  // of the computational traces starting from this point (also loops).
  PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
  PPL_DIRTY_TEMP(N, coeff_i);

  switch (relsym) {
  case EQUAL:
    {
      PPL_DIRTY_TEMP(N, neg_sum);
      // Indices of the variables that are unbounded in `this->dbm'.
      PPL_UNINITIALIZED(dimension_type, neg_pinf_index);
      // Number of unbounded variables found.
      dimension_type neg_pinf_count = 0;

      // Compute an upper approximation for `expr' into `sum',
      // taking into account the sign of `denominator'.

      // Approximate the inhomogeneous term.
      assign_r(sum, sc_b, ROUND_UP);
      assign_r(neg_sum, minus_sc_b, ROUND_UP);

      // Approximate the homogeneous part of `sc_expr'.
      // Note: indices above `w' can be disregarded, as they all have
      // a zero coefficient in `expr'.
      for (Linear_Expression::const_iterator i = sc_expr.begin(),
            i_end = sc_expr.lower_bound(Variable(w)); i != i_end; ++i) {
        const dimension_type i_dim = i.variable().space_dimension();
        const Coefficient& sc_i = *i;
        const int sign_i = sgn(sc_i);
        PPL_ASSERT(sign_i != 0);
        if (sign_i > 0) {
          assign_r(coeff_i, sc_i, ROUND_UP);
          // Approximating `sc_expr'.
          if (pinf_count <= 1) {
            const N& approx_i = dbm_0[i_dim];
            if (!is_plus_infinity(approx_i))
              add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
            else {
              ++pinf_count;
              pinf_index = i_dim;
            }
          }
          // Approximating `-sc_expr'.
          if (neg_pinf_count <= 1) {
            const N& approx_minus_i = dbm[i_dim][0];
            if (!is_plus_infinity(approx_minus_i))
              add_mul_assign_r(neg_sum, coeff_i, approx_minus_i, ROUND_UP);
            else {
              ++neg_pinf_count;
              neg_pinf_index = i_dim;
            }
          }
        }
        else {
          PPL_ASSERT(sign_i < 0);
          neg_assign(minus_sc_i, sc_i);
          // Note: using temporary named `coeff_i' to store -coeff_i.
          assign_r(coeff_i, minus_sc_i, ROUND_UP);
          // Approximating `sc_expr'.
          if (pinf_count <= 1) {
            const N& approx_minus_i = dbm[i_dim][0];
            if (!is_plus_infinity(approx_minus_i))
              add_mul_assign_r(sum, coeff_i, approx_minus_i, ROUND_UP);
            else {
              ++pinf_count;
              pinf_index = i_dim;
            }
          }
          // Approximating `-sc_expr'.
          if (neg_pinf_count <= 1) {
            const N& approx_i = dbm_0[i_dim];
            if (!is_plus_infinity(approx_i))
              add_mul_assign_r(neg_sum, coeff_i, approx_i, ROUND_UP);
            else {
              ++neg_pinf_count;
              neg_pinf_index = i_dim;
            }
          }
        }
      }
      // Return immediately if no approximation could be computed.
      if (pinf_count > 1 && neg_pinf_count > 1) {
        PPL_ASSERT(OK());
        return;
      }

      // In the following, shortest-path closure will be definitely lost.
      reset_shortest_path_closed();

      // Before computing quotients, the denominator should be approximated
      // towards zero. Since `sc_denom' is known to be positive, this amounts to
      // rounding downwards, which is achieved as usual by rounding upwards
      // `minus_sc_denom' and negating again the result.
      PPL_DIRTY_TEMP(N, down_sc_denom);
      assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
      neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);

      // Exploit the upper approximation, if possible.
      if (pinf_count <= 1) {
        // Compute quotient (if needed).
        if (down_sc_denom != 1)
          div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
        // Add the upper bound constraint, if meaningful.
        if (pinf_count == 0) {
          // Add the constraint `v <= sum'.
          dbm[0][v] = sum;
          // Deduce constraints of the form `v - u', where `u != v'.
          deduce_v_minus_u_bounds(v, w, sc_expr, sc_denom, sum);
        }
        else
          // Here `pinf_count == 1'.
          if (pinf_index != v
              && sc_expr.get(Variable(pinf_index - 1)) == sc_denom)
            // Add the constraint `v - pinf_index <= sum'.
            dbm[pinf_index][v] = sum;
      }

      // Exploit the lower approximation, if possible.
      if (neg_pinf_count <= 1) {
        // Compute quotient (if needed).
        if (down_sc_denom != 1)
          div_assign_r(neg_sum, neg_sum, down_sc_denom, ROUND_UP);
        // Add the lower bound constraint, if meaningful.
        if (neg_pinf_count == 0) {
          // Add the constraint `v >= -neg_sum', i.e., `-v <= neg_sum'.
          DB_Row<N>& dbm_v = dbm[v];
          dbm_v[0] = neg_sum;
          // Deduce constraints of the form `u - v', where `u != v'.
          deduce_u_minus_v_bounds(v, w, sc_expr, sc_denom, neg_sum);
        }
        else
          // Here `neg_pinf_count == 1'.
          if (neg_pinf_index != v
              && sc_expr.get(Variable(neg_pinf_index - 1)) == sc_denom)
            // Add the constraint `v - neg_pinf_index >= -neg_sum',
            // i.e., `neg_pinf_index - v <= neg_sum'.
            dbm[v][neg_pinf_index] = neg_sum;
      }
    }
    break;

  case LESS_OR_EQUAL:
    // Compute an upper approximation for `expr' into `sum',
    // taking into account the sign of `denominator'.

    // Approximate the inhomogeneous term.
    assign_r(sum, sc_b, ROUND_UP);

    // Approximate the homogeneous part of `sc_expr'.
    // Note: indices above `w' can be disregarded, as they all have
    // a zero coefficient in `expr'.
    for (Linear_Expression::const_iterator i = sc_expr.begin(),
          i_end = sc_expr.lower_bound(Variable(w)); i != i_end; ++i) {
      const Coefficient& sc_i = *i;
      const dimension_type i_dim = i.variable().space_dimension();
      const int sign_i = sgn(sc_i);
      PPL_ASSERT(sign_i != 0);
      // Choose carefully: we are approximating `sc_expr'.
      const N& approx_i = (sign_i > 0) ? dbm_0[i_dim] : dbm[i_dim][0];
      if (is_plus_infinity(approx_i)) {
        if (++pinf_count > 1)
          break;
        pinf_index = i_dim;
        continue;
      }
      if (sign_i > 0)
        assign_r(coeff_i, sc_i, ROUND_UP);
      else {
        neg_assign(minus_sc_i, sc_i);
        assign_r(coeff_i, minus_sc_i, ROUND_UP);
      }
      add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
    }

    // Divide by the (sign corrected) denominator (if needed).
    if (sc_denom != 1) {
      // Before computing the quotient, the denominator should be
      // approximated towards zero. Since `sc_denom' is known to be
      // positive, this amounts to rounding downwards, which is achieved
      // by rounding upwards `minus_sc - denom' and negating again the result.
      PPL_DIRTY_TEMP(N, down_sc_denom);
      assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
      neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
      div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
    }

    if (pinf_count == 0) {
      // Add the constraint `v <= sum'.
      add_dbm_constraint(0, v, sum);
      // Deduce constraints of the form `v - u', where `u != v'.
      deduce_v_minus_u_bounds(v, w, sc_expr, sc_denom, sum);
    }
    else if (pinf_count == 1)
      if (expr.get(Variable(pinf_index - 1)) == denominator)
        // Add the constraint `v - pinf_index <= sum'.
        add_dbm_constraint(pinf_index, v, sum);
    break;

  case GREATER_OR_EQUAL:
    // Compute an upper approximation for `-sc_expr' into `sum'.
    // Note: approximating `-sc_expr' from above and then negating the
    // result is the same as approximating `sc_expr' from below.

    // Approximate the inhomogeneous term.
    assign_r(sum, minus_sc_b, ROUND_UP);

    // Approximate the homogeneous part of `-sc_expr'.
    for (Linear_Expression::const_iterator i = sc_expr.begin(),
          i_end = sc_expr.lower_bound(Variable(w)); i != i_end; ++i) {
      const Coefficient& sc_i = *i;
      const dimension_type i_dim = i.variable().space_dimension();
      const int sign_i = sgn(sc_i);
      PPL_ASSERT(sign_i != 0);
      // Choose carefully: we are approximating `-sc_expr'.
      const N& approx_i = (sign_i > 0) ? dbm[i_dim][0] : dbm_0[i_dim];
      if (is_plus_infinity(approx_i)) {
        if (++pinf_count > 1)
          break;
        pinf_index = i_dim;
        continue;
      }
      if (sign_i > 0)
        assign_r(coeff_i, sc_i, ROUND_UP);
      else {
        neg_assign(minus_sc_i, sc_i);
        assign_r(coeff_i, minus_sc_i, ROUND_UP);
      }
      add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
    }

    // Divide by the (sign corrected) denominator (if needed).
    if (sc_denom != 1) {
      // Before computing the quotient, the denominator should be
      // approximated towards zero. Since `sc_denom' is known to be positive,
      // this amounts to rounding downwards, which is achieved by rounding
      // upwards `minus_sc_denom' and negating again the result.
      PPL_DIRTY_TEMP(N, down_sc_denom);
      assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
      neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
      div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
    }

    if (pinf_count == 0) {
      // Add the constraint `v >= -sum', i.e., `-v <= sum'.
      add_dbm_constraint(v, 0, sum);
      // Deduce constraints of the form `u - v', where `u != v'.
      deduce_u_minus_v_bounds(v, w, sc_expr, sc_denom, sum);
    }
    else if (pinf_count == 1)
      if (pinf_index != v
          && expr.get(Variable(pinf_index - 1)) == denominator)
        // Add the constraint `v - pinf_index >= -sum',
        // i.e., `pinf_index - v <= sum'.
        add_dbm_constraint(v, pinf_index, sum);
    break;

  default:
    // We already dealt with the other cases.
    PPL_UNREACHABLE;
    break;
  }

  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::affine_image(const Variable var,
                          const Linear_Expression& expr,
                          Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("affine_image(v, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("affine_image(v, e, d)", "e", expr);

  // `var' should be one of the dimensions of the shape.
  const dimension_type v = var.id() + 1;
  if (v > space_dim)
    throw_dimension_incompatible("affine_image(v, e, d)", var.id());

  // The image of an empty BDS is empty too.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  const Coefficient& b = expr.inhomogeneous_term();
  // Number of non-zero coefficients in `expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Index of the last non-zero coefficient in `expr', if any.
  dimension_type w = expr.last_nonzero();

  if (w != 0) {
    ++t;
    if (!expr.all_zeroes(1, w))
      ++t;
  }

  // Now we know the form of `expr':
  // - If t == 0, then expr == b, with `b' a constant;
  // - If t == 1, then expr == a*w + b, where `w' can be `v' or another
  //   variable; in this second case we have to check whether `a' is
  //   equal to `denominator' or `-denominator', since otherwise we have
  //   to fall back on the general form;
  // - If t == 2, the `expr' is of the general form.
  PPL_DIRTY_TEMP_COEFFICIENT(minus_denom);
  neg_assign(minus_denom, denominator);

  if (t == 0) {
    // Case 1: expr == b.
    // Remove all constraints on `var'.
    forget_all_dbm_constraints(v);
    // Shortest-path closure is preserved, but not reduction.
    if (marked_shortest_path_reduced())
      reset_shortest_path_reduced();
    // Add the constraint `var == b/denominator'.
    add_dbm_constraint(0, v, b, denominator);
    add_dbm_constraint(v, 0, b, minus_denom);
    PPL_ASSERT(OK());
    return;
  }

  if (t == 1) {
    // Value of the one and only non-zero coefficient in `expr'.
    const Coefficient& a = expr.get(Variable(w - 1));
    if (a == denominator || a == minus_denom) {
      // Case 2: expr == a*w + b, with a == +/- denominator.
      if (w == v) {
        // `expr' is of the form: a*v + b.
        if (a == denominator) {
          if (b == 0)
            // The transformation is the identity function.
            return;
          else {
            // Translate all the constraints on `var',
            // adding or subtracting the value `b/denominator'.
            PPL_DIRTY_TEMP(N, d);
            div_round_up(d, b, denominator);
            PPL_DIRTY_TEMP(N, c);
            div_round_up(c, b, minus_denom);
            DB_Row<N>& dbm_v = dbm[v];
            for (dimension_type i = space_dim + 1; i-- > 0; ) {
              N& dbm_vi = dbm_v[i];
              add_assign_r(dbm_vi, dbm_vi, c, ROUND_UP);
              N& dbm_iv = dbm[i][v];
              add_assign_r(dbm_iv, dbm_iv, d, ROUND_UP);
            }
            // Both shortest-path closure and reduction are preserved.
          }
        }
        else {
          // Here `a == -denominator'.
          // Remove the binary constraints on `var'.
          forget_binary_dbm_constraints(v);
          // Swap the unary constraints on `var'.
          using std::swap;
          swap(dbm[v][0], dbm[0][v]);
          // Shortest-path closure is not preserved.
          reset_shortest_path_closed();
          if (b != 0) {
            // Translate the unary constraints on `var',
            // adding or subtracting the value `b/denominator'.
            PPL_DIRTY_TEMP(N, c);
            div_round_up(c, b, minus_denom);
            N& dbm_v0 = dbm[v][0];
            add_assign_r(dbm_v0, dbm_v0, c, ROUND_UP);
            PPL_DIRTY_TEMP(N, d);
            div_round_up(d, b, denominator);
            N& dbm_0v = dbm[0][v];
            add_assign_r(dbm_0v, dbm_0v, d, ROUND_UP);
          }
        }
      }
      else {
        // Here `w != v', so that `expr' is of the form
        // +/-denominator * w + b.
        // Remove all constraints on `var'.
        forget_all_dbm_constraints(v);
        // Shortest-path closure is preserved, but not reduction.
        if (marked_shortest_path_reduced())
          reset_shortest_path_reduced();
        if (a == denominator) {
          // Add the new constraint `v - w == b/denominator'.
          add_dbm_constraint(w, v, b, denominator);
          add_dbm_constraint(v, w, b, minus_denom);
        }
        else {
          // Here a == -denominator, so that we should be adding
          // the constraint `v + w == b/denominator'.
          // Approximate it by computing lower and upper bounds for `w'.
          const N& dbm_w0 = dbm[w][0];
          if (!is_plus_infinity(dbm_w0)) {
            // Add the constraint `v <= b/denominator - lower_w'.
            PPL_DIRTY_TEMP(N, d);
            div_round_up(d, b, denominator);
            add_assign_r(dbm[0][v], d, dbm_w0, ROUND_UP);
            reset_shortest_path_closed();
          }
          const N& dbm_0w = dbm[0][w];
          if (!is_plus_infinity(dbm_0w)) {
            // Add the constraint `v >= b/denominator - upper_w'.
            PPL_DIRTY_TEMP(N, c);
            div_round_up(c, b, minus_denom);
            add_assign_r(dbm[v][0], dbm_0w, c, ROUND_UP);
            reset_shortest_path_closed();
          }
        }
      }
      PPL_ASSERT(OK());
      return;
    }
  }

  // General case.
  // Either t == 2, so that
  // expr == a_1*x_1 + a_2*x_2 + ... + a_n*x_n + b, where n >= 2,
  // or t == 1, expr == a*w + b, but a <> +/- denominator.
  // We will remove all the constraints on `var' and add back
  // constraints providing upper and lower bounds for `var'.

  // Compute upper approximations for `expr' and `-expr'
  // into `pos_sum' and `neg_sum', respectively, taking into account
  // the sign of `denominator'.
  // Note: approximating `-expr' from above and then negating the
  // result is the same as approximating `expr' from below.
  const bool is_sc = (denominator > 0);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_b);
  neg_assign(minus_b, b);
  const Coefficient& sc_b = is_sc ? b : minus_b;
  const Coefficient& minus_sc_b = is_sc ? minus_b : b;
  const Coefficient& sc_denom = is_sc ? denominator : minus_denom;
  const Coefficient& minus_sc_denom = is_sc ? minus_denom : denominator;
  // NOTE: here, for optimization purposes, `minus_expr' is only assigned
  // when `denominator' is negative. Do not use it unless you are sure
  // it has been correctly assigned.
  Linear_Expression minus_expr;
  if (!is_sc)
    minus_expr = -expr;
  const Linear_Expression& sc_expr = is_sc ? expr : minus_expr;

  PPL_DIRTY_TEMP(N, pos_sum);
  PPL_DIRTY_TEMP(N, neg_sum);
  // Indices of the variables that are unbounded in `this->dbm'.
  PPL_UNINITIALIZED(dimension_type, pos_pinf_index);
  PPL_UNINITIALIZED(dimension_type, neg_pinf_index);
  // Number of unbounded variables found.
  dimension_type pos_pinf_count = 0;
  dimension_type neg_pinf_count = 0;

  // Approximate the inhomogeneous term.
  assign_r(pos_sum, sc_b, ROUND_UP);
  assign_r(neg_sum, minus_sc_b, ROUND_UP);

  // Approximate the homogeneous part of `sc_expr'.
  const DB_Row<N>& dbm_0 = dbm[0];
  // Speculative allocation of temporaries to be used in the following loop.
  PPL_DIRTY_TEMP(N, coeff_i);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);

  // Note: indices above `w' can be disregarded, as they all have
  // a zero coefficient in `sc_expr'.
  for (Linear_Expression::const_iterator i = sc_expr.begin(),
        i_end = sc_expr.lower_bound(Variable(w)); i != i_end; ++i) {
    const Coefficient& sc_i = *i;
    const dimension_type i_dim = i.variable().space_dimension();
    const int sign_i = sgn(sc_i);
    if (sign_i > 0) {
      assign_r(coeff_i, sc_i, ROUND_UP);
      // Approximating `sc_expr'.
      if (pos_pinf_count <= 1) {
        const N& up_approx_i = dbm_0[i_dim];
        if (!is_plus_infinity(up_approx_i))
          add_mul_assign_r(pos_sum, coeff_i, up_approx_i, ROUND_UP);
        else {
          ++pos_pinf_count;
          pos_pinf_index = i_dim;
        }
      }
      // Approximating `-sc_expr'.
      if (neg_pinf_count <= 1) {
        const N& up_approx_minus_i = dbm[i_dim][0];
        if (!is_plus_infinity(up_approx_minus_i))
          add_mul_assign_r(neg_sum, coeff_i, up_approx_minus_i, ROUND_UP);
        else {
          ++neg_pinf_count;
          neg_pinf_index = i_dim;
        }
      }
    }
    else {
      PPL_ASSERT(sign_i < 0);
      neg_assign(minus_sc_i, sc_i);
      // Note: using temporary named `coeff_i' to store -coeff_i.
      assign_r(coeff_i, minus_sc_i, ROUND_UP);
      // Approximating `sc_expr'.
      if (pos_pinf_count <= 1) {
        const N& up_approx_minus_i = dbm[i_dim][0];
        if (!is_plus_infinity(up_approx_minus_i))
          add_mul_assign_r(pos_sum, coeff_i, up_approx_minus_i, ROUND_UP);
        else {
          ++pos_pinf_count;
          pos_pinf_index = i_dim;
        }
      }
      // Approximating `-sc_expr'.
      if (neg_pinf_count <= 1) {
        const N& up_approx_i = dbm_0[i_dim];
        if (!is_plus_infinity(up_approx_i))
          add_mul_assign_r(neg_sum, coeff_i, up_approx_i, ROUND_UP);
        else {
          ++neg_pinf_count;
          neg_pinf_index = i_dim;
        }
      }
    }
  }

  // Remove all constraints on 'v'.
  forget_all_dbm_constraints(v);
  // Shortest-path closure is maintained, but not reduction.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();
  // Return immediately if no approximation could be computed.
  if (pos_pinf_count > 1 && neg_pinf_count > 1) {
    PPL_ASSERT(OK());
    return;
  }

  // In the following, shortest-path closure will be definitely lost.
  reset_shortest_path_closed();

  // Exploit the upper approximation, if possible.
  if (pos_pinf_count <= 1) {
    // Compute quotient (if needed).
    if (sc_denom != 1) {
      // Before computing quotients, the denominator should be approximated
      // towards zero. Since `sc_denom' is known to be positive, this amounts to
      // rounding downwards, which is achieved as usual by rounding upwards
      // `minus_sc_denom' and negating again the result.
      PPL_DIRTY_TEMP(N, down_sc_denom);
      assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
      neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
      div_assign_r(pos_sum, pos_sum, down_sc_denom, ROUND_UP);
    }
    // Add the upper bound constraint, if meaningful.
    if (pos_pinf_count == 0) {
      // Add the constraint `v <= pos_sum'.
      dbm[0][v] = pos_sum;
      // Deduce constraints of the form `v - u', where `u != v'.
      deduce_v_minus_u_bounds(v, w, sc_expr, sc_denom, pos_sum);
    }
    else
      // Here `pos_pinf_count == 1'.
      if (pos_pinf_index != v
          && sc_expr.get(Variable(pos_pinf_index - 1)) == sc_denom)
        // Add the constraint `v - pos_pinf_index <= pos_sum'.
        dbm[pos_pinf_index][v] = pos_sum;
  }

  // Exploit the lower approximation, if possible.
  if (neg_pinf_count <= 1) {
    // Compute quotient (if needed).
    if (sc_denom != 1) {
      // Before computing quotients, the denominator should be approximated
      // towards zero. Since `sc_denom' is known to be positive, this amounts to
      // rounding downwards, which is achieved as usual by rounding upwards
      // `minus_sc_denom' and negating again the result.
      PPL_DIRTY_TEMP(N, down_sc_denom);
      assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
      neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
      div_assign_r(neg_sum, neg_sum, down_sc_denom, ROUND_UP);
    }
    // Add the lower bound constraint, if meaningful.
    if (neg_pinf_count == 0) {
      // Add the constraint `v >= -neg_sum', i.e., `-v <= neg_sum'.
      DB_Row<N>& dbm_v = dbm[v];
      dbm_v[0] = neg_sum;
      // Deduce constraints of the form `u - v', where `u != v'.
      deduce_u_minus_v_bounds(v, w, sc_expr, sc_denom, neg_sum);
    }
    else
      // Here `neg_pinf_count == 1'.
      if (neg_pinf_index != v
          && sc_expr.get(Variable(neg_pinf_index - 1)) == sc_denom)
        // Add the constraint `v - neg_pinf_index >= -neg_sum',
        // i.e., `neg_pinf_index - v <= neg_sum'.
        dbm[v][neg_pinf_index] = neg_sum;
  }

  PPL_ASSERT(OK());
}

template <typename T>
template <typename Interval_Info>
void
BD_Shape<T>::affine_form_image(const Variable var,
                    const Linear_Form< Interval<T, Interval_Info> >& lf) {

  // Check that T is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
                    "BD_Shape<T>::affine_form_image(Variable, Linear_Form):"
                    " T not a floating point type.");

  // Dimension-compatibility checks.
  // The dimension of `lf' should not be greater than the dimension
  // of `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type lf_space_dim = lf.space_dimension();
  if (space_dim < lf_space_dim)
    throw_dimension_incompatible("affine_form_image(var_id, l)", "l", lf);

  // `var' should be one of the dimensions of the shape.
  const dimension_type var_id = var.id() + 1;
  if (space_dim < var_id)
    throw_dimension_incompatible("affine_form_image(var_id, l)", var.id());

  // The image of an empty BDS is empty too.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  // Number of non-zero coefficients in `lf': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Index of the last non-zero coefficient in `lf', if any.
  dimension_type w_id = 0;
  // Get information about the number of non-zero coefficients in `lf'.
  for (dimension_type i = lf_space_dim; i-- > 0; )
    if (lf.coefficient(Variable(i)) != 0) {
      if (t++ == 1)
        break;
      else
        w_id = i + 1;
    }

  typedef Interval<T, Interval_Info> FP_Interval_Type;

  const FP_Interval_Type& b = lf.inhomogeneous_term();

  // Now we know the form of `lf':
  // - If t == 0, then lf == b, with `b' a constant;
  // - If t == 1, then lf == a*w + b, where `w' can be `v' or another
  //   variable;
  // - If t == 2, the linear form 'lf' is of the general form.

  if (t == 0) {
    inhomogeneous_affine_form_image(var_id, b);
    PPL_ASSERT(OK());
    return;
  }
  else if (t == 1) {
    const FP_Interval_Type& w_coeff = lf.coefficient(Variable(w_id - 1));
    if (w_coeff == 1 || w_coeff == -1) {
      one_variable_affine_form_image(var_id, b, w_coeff, w_id, space_dim);
      PPL_ASSERT(OK());
      return;
    }
  }
  two_variables_affine_form_image(var_id, lf, space_dim);
  PPL_ASSERT(OK());
}

// Case 1: var = b, where b = [-b_mlb, b_ub]
template <typename T>
template <typename Interval_Info>
void
BD_Shape<T>
::inhomogeneous_affine_form_image(const dimension_type& var_id,
                                  const Interval<T, Interval_Info>& b) {
  PPL_DIRTY_TEMP(N, b_ub);
  assign_r(b_ub, b.upper(), ROUND_NOT_NEEDED);
  PPL_DIRTY_TEMP(N, b_mlb);
  neg_assign_r(b_mlb, b.lower(), ROUND_NOT_NEEDED);

  // Remove all constraints on `var'.
  forget_all_dbm_constraints(var_id);
  // Shortest-path closure is preserved, but not reduction.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();
    // Add the constraint `var >= lb && var <= ub'.
    add_dbm_constraint(0, var_id, b_ub);
    add_dbm_constraint(var_id, 0, b_mlb);
    return;
}

// case 2: var = (+/-1) * w + [-b_mlb, b_ub], where `w' can be `var'
// or another variable.
template <typename T>
template <typename Interval_Info>
void BD_Shape<T>
::one_variable_affine_form_image(const dimension_type& var_id,
                            const Interval<T, Interval_Info>& b,
                            const Interval<T, Interval_Info>& w_coeff,
                            const dimension_type& w_id,
                            const dimension_type& space_dim) {

  PPL_DIRTY_TEMP(N, b_ub);
  assign_r(b_ub, b.upper(), ROUND_NOT_NEEDED);
  PPL_DIRTY_TEMP(N, b_mlb);
  neg_assign_r(b_mlb, b.lower(), ROUND_NOT_NEEDED);

  // True if `w_coeff' is in [1, 1].
  bool is_w_coeff_one = (w_coeff == 1);

  if (w_id == var_id) {
    // True if `b' is in [b_mlb, b_ub] and that is [0, 0].
    bool is_b_zero = (b_mlb == 0 && b_ub == 0);
    // Here `lf' is of the form: [+/-1, +/-1] * v + b.
    if (is_w_coeff_one) {
      if (is_b_zero)
        // The transformation is the identity function.
        return;
      else {
        // Translate all the constraints on `var' by adding the value
        // `b_ub' or subtracting the value `b_mlb'.
        DB_Row<N>& dbm_v = dbm[var_id];
        for (dimension_type i = space_dim + 1; i-- > 0; ) {
          N& dbm_vi = dbm_v[i];
          add_assign_r(dbm_vi, dbm_vi, b_mlb, ROUND_UP);
          N& dbm_iv = dbm[i][var_id];
          add_assign_r(dbm_iv, dbm_iv, b_ub, ROUND_UP);
        }
        // Both shortest-path closure and reduction are preserved.
      }
    }
    else {
      // Here `w_coeff = [-1, -1].
      // Remove the binary constraints on `var'.
      forget_binary_dbm_constraints(var_id);
      using std::swap;
      swap(dbm[var_id][0], dbm[0][var_id]);
      // Shortest-path closure is not preserved.
      reset_shortest_path_closed();
      if (!is_b_zero) {
        // Translate the unary constraints on `var' by adding the value
        // `b_ub' or subtracting the value `b_mlb'.
        N& dbm_v0 = dbm[var_id][0];
        add_assign_r(dbm_v0, dbm_v0, b_mlb, ROUND_UP);
        N& dbm_0v = dbm[0][var_id];
        add_assign_r(dbm_0v, dbm_0v, b_ub, ROUND_UP);
      }
    }
  }
  else {
    // Here `w != var', so that `lf' is of the form
    // [+/-1, +/-1] * w + b.
    // Remove all constraints on `var'.
    forget_all_dbm_constraints(var_id);
    // Shortest-path closure is preserved, but not reduction.
    if (marked_shortest_path_reduced())
      reset_shortest_path_reduced();
    if (is_w_coeff_one) {
      // Add the new constraints `var - w >= b_mlb'
      // `and var - w <= b_ub'.
      add_dbm_constraint(w_id, var_id, b_ub);
      add_dbm_constraint(var_id, w_id, b_mlb);
    }
    else {
      // We have to add the constraint `v + w == b', over-approximating it
      // by computing lower and upper bounds for `w'.
      const N& mlb_w = dbm[w_id][0];
      if (!is_plus_infinity(mlb_w)) {
        // Add the constraint `v <= ub - lb_w'.
        add_assign_r(dbm[0][var_id], b_ub, mlb_w, ROUND_UP);
        reset_shortest_path_closed();
      }
      const N& ub_w = dbm[0][w_id];
      if (!is_plus_infinity(ub_w)) {
        // Add the constraint `v >= lb - ub_w'.
        add_assign_r(dbm[var_id][0], ub_w, b_mlb, ROUND_UP);
        reset_shortest_path_closed();
      }
    }
  }
  return;
}

// General case.
// Either t == 2, so that
// lf == i_1*x_1 + i_2*x_2 + ... + i_n*x_n + b, where n >= 2,
// or t == 1, lf == i*w + b, but i <> [+/-1, +/-1].
template <typename T>
template <typename Interval_Info>
void BD_Shape<T>
::two_variables_affine_form_image(const dimension_type& var_id,
           const Linear_Form< Interval<T, Interval_Info> >& lf,
                             const dimension_type& space_dim) {
  // Shortest-path closure is maintained, but not reduction.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();

  reset_shortest_path_closed();

  Linear_Form< Interval<T, Interval_Info> > minus_lf(lf);
  minus_lf.negate();

  // Declare temporaries outside the loop.
  PPL_DIRTY_TEMP(N, upper_bound);

  // Update binary constraints on var FIRST.
  for (dimension_type curr_var = 1; curr_var < var_id; ++curr_var) {
    Variable current(curr_var - 1);
    linear_form_upper_bound(lf - current, upper_bound);
    assign_r(dbm[curr_var][var_id], upper_bound, ROUND_NOT_NEEDED);
    linear_form_upper_bound(minus_lf + current, upper_bound);
    assign_r(dbm[var_id][curr_var], upper_bound, ROUND_NOT_NEEDED);
  }
  for (dimension_type curr_var = var_id + 1; curr_var <= space_dim;
                                                      ++curr_var) {
    Variable current(curr_var - 1);
    linear_form_upper_bound(lf - current, upper_bound);
    assign_r(dbm[curr_var][var_id], upper_bound, ROUND_NOT_NEEDED);
    linear_form_upper_bound(minus_lf + current, upper_bound);
    assign_r(dbm[var_id][curr_var], upper_bound, ROUND_NOT_NEEDED);
  }
  // Finally, update unary constraints on var.
  PPL_DIRTY_TEMP(N, lf_ub);
  linear_form_upper_bound(lf, lf_ub);
  PPL_DIRTY_TEMP(N, minus_lf_ub);
  linear_form_upper_bound(minus_lf, minus_lf_ub);
  assign_r(dbm[0][var_id], lf_ub, ROUND_NOT_NEEDED);
  assign_r(dbm[var_id][0], minus_lf_ub, ROUND_NOT_NEEDED);
}

template <typename T>
template <typename Interval_Info>
void BD_Shape<T>::refine_with_linear_form_inequality(
                   const Linear_Form< Interval<T, Interval_Info> >& left,
                   const Linear_Form< Interval<T, Interval_Info> >& right) {
    // Check that T is a floating point type.
    PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
                    "Octagonal_Shape<T>::refine_with_linear_form_inequality:"
                    " T not a floating point type.");

    //We assume that the analyzer will not try to apply an unreachable filter.
    PPL_ASSERT(!marked_empty());

    // Dimension-compatibility checks.
    // The dimensions of `left' and `right' should not be greater than the
    // dimension of `*this'.
    const dimension_type left_space_dim = left.space_dimension();
    const dimension_type space_dim = space_dimension();
    if (space_dim < left_space_dim)
      throw_dimension_incompatible(
          "refine_with_linear_form_inequality(left, right)", "left", left);

    const dimension_type right_space_dim = right.space_dimension();
    if (space_dim < right_space_dim)
      throw_dimension_incompatible(
          "refine_with_linear_form_inequality(left, right)", "right", right);

  // Number of non-zero coefficients in `left': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type left_t = 0;
  // Variable-index of the last non-zero coefficient in `left', if any.
  dimension_type left_w_id = 0;
  // Number of non-zero coefficients in `right': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type right_t = 0;
  // Variable-index of the last non-zero coefficient in `right', if any.
  dimension_type right_w_id = 0;

  typedef Interval<T, Interval_Info> FP_Interval_Type;

  // Get information about the number of non-zero coefficients in `left'.
  for (dimension_type i = left_space_dim; i-- > 0; )
    if (left.coefficient(Variable(i)) != 0) {
      if (left_t++ == 1)
        break;
      else
        left_w_id = i;
    }

  // Get information about the number of non-zero coefficients in `right'.
  for (dimension_type i = right_space_dim; i-- > 0; )
    if (right.coefficient(Variable(i)) != 0) {
      if (right_t++ == 1)
        break;
      else
        right_w_id = i;
    }

  const FP_Interval_Type& left_w_coeff =
          left.coefficient(Variable(left_w_id));
  const FP_Interval_Type& right_w_coeff =
          right.coefficient(Variable(right_w_id));

  if (left_t == 0) {
    if (right_t == 0) {
      // The constraint involves constants only. Ignore it: it is up to
      // the analyzer to handle it.
      PPL_ASSERT(OK());
      return;
    }
    else if (right_w_coeff == 1 || right_w_coeff == -1) {
      left_inhomogeneous_refine(right_t, right_w_id, left, right);
      PPL_ASSERT(OK());
      return;
    }
  }
  else if (left_t == 1) {
    if (left_w_coeff == 1 || left_w_coeff == -1) {
      if (right_t == 0 || (right_w_coeff == 1 || right_w_coeff == -1)) {
        left_one_var_refine(left_w_id, right_t, right_w_id, left, right);
        PPL_ASSERT(OK());
        return;
      }
    }
  }

  // General case.
  general_refine(left_w_id, right_w_id, left, right);
  PPL_ASSERT(OK());
} // end of refine_with_linear_form_inequality

template <typename T>
template <typename U>
void
BD_Shape<T>
::export_interval_constraints(U& dest) const {
  const dimension_type space_dim = space_dimension();
  if (space_dim > dest.space_dimension())
    throw std::invalid_argument(
               "BD_Shape<T>::export_interval_constraints");

  // Expose all the interval constraints.
  shortest_path_closure_assign();

  if (marked_empty()) {
    dest.set_empty();
    PPL_ASSERT(OK());
    return;
  }

  PPL_DIRTY_TEMP(N, tmp);
  const DB_Row<N>& dbm_0 = dbm[0];
  for (dimension_type i = space_dim; i-- > 0; ) {
    // Set the upper bound.
    const N& u = dbm_0[i+1];
    if (!is_plus_infinity(u))
      if (!dest.restrict_upper(i, u.raw_value()))
        return;

    // Set the lower bound.
    const N& negated_l = dbm[i+1][0];
    if (!is_plus_infinity(negated_l)) {
      neg_assign_r(tmp, negated_l, ROUND_DOWN);
      if (!dest.restrict_lower(i, tmp.raw_value()))
        return;
    }
  }

  PPL_ASSERT(OK());
}

template <typename T>
template <typename Interval_Info>
void
BD_Shape<T>::left_inhomogeneous_refine(const dimension_type& right_t,
                                       const dimension_type& right_w_id,
                    const Linear_Form< Interval<T, Interval_Info> >& left,
                    const Linear_Form< Interval<T, Interval_Info> >& right) {

  typedef Interval<T, Interval_Info> FP_Interval_Type;

  if (right_t == 1) {
    // The constraint has the form [a-, a+] <= [b-, b+] + [c-, c+] * x.
    // Reduce it to the constraint +/-x <= b+ - a- if [c-, c+] = +/-[1, 1].
      const FP_Interval_Type& right_w_coeff =
                              right.coefficient(Variable(right_w_id));
      if (right_w_coeff == 1) {
        PPL_DIRTY_TEMP(N, b_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_b = right.inhomogeneous_term();
        sub_assign_r(b_plus_minus_a_minus, right_b.upper(), left_a.lower(),
                     ROUND_UP);
        add_dbm_constraint(right_w_id+1, 0, b_plus_minus_a_minus);
        return;
      }

      if (right_w_coeff == -1) {
        PPL_DIRTY_TEMP(N, b_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_b = right.inhomogeneous_term();
        sub_assign_r(b_plus_minus_a_minus, right_b.upper(), left_a.lower(),
                     ROUND_UP);
        add_dbm_constraint(0, right_w_id+1, b_plus_minus_a_minus);
        return;
      }
    }
} // end of left_inhomogeneous_refine


template <typename T>
template <typename Interval_Info>
void
BD_Shape<T>
::left_one_var_refine(const dimension_type& left_w_id,
                      const dimension_type& right_t,
                      const dimension_type& right_w_id,
                const Linear_Form< Interval<T, Interval_Info> >& left,
                const Linear_Form< Interval<T, Interval_Info> >& right) {

  typedef Interval<T, Interval_Info> FP_Interval_Type;

    if (right_t == 0) {
      // The constraint has the form [b-, b+] + [c-, c+] * x <= [a-, a+]
      // Reduce it to the constraint +/-x <= a+ - b- if [c-, c+] = +/-[1, 1].
      const FP_Interval_Type& left_w_coeff =
        left.coefficient(Variable(left_w_id));

      if (left_w_coeff == 1) {
        PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
        const FP_Interval_Type& left_b = left.inhomogeneous_term();
        const FP_Interval_Type& right_a = right.inhomogeneous_term();
        sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                     ROUND_UP);
        add_dbm_constraint(0, left_w_id+1, a_plus_minus_b_minus);
        return;
      }

      if (left_w_coeff == -1) {
        PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
        const FP_Interval_Type& left_b = left.inhomogeneous_term();
        const FP_Interval_Type& right_a = right.inhomogeneous_term();
        sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                     ROUND_UP);
        add_dbm_constraint(left_w_id+1, 0, a_plus_minus_b_minus);
        return;
      }
    }
    else if (right_t == 1) {
      // The constraint has the form
      // [a-, a+] + [b-, b+] * x <= [c-, c+] + [d-, d+] * y.
      // Reduce it to the constraint +/-x +/-y <= c+ - a-
      // if [b-, b+] = +/-[1, 1] and [d-, d+] = +/-[1, 1].
      const FP_Interval_Type& left_w_coeff =
                              left.coefficient(Variable(left_w_id));

      const FP_Interval_Type& right_w_coeff =
                              right.coefficient(Variable(right_w_id));

      bool is_left_coeff_one = (left_w_coeff == 1);
      bool is_left_coeff_minus_one = (left_w_coeff == -1);
      bool is_right_coeff_one = (right_w_coeff == 1);
      bool is_right_coeff_minus_one = (right_w_coeff == -1);
      if (left_w_id == right_w_id) {
        if ((is_left_coeff_one && is_right_coeff_one)
            ||
            (is_left_coeff_minus_one && is_right_coeff_minus_one)) {
          // Here we have an identity or a constants-only constraint.
          return;
        }
        if (is_left_coeff_one && is_right_coeff_minus_one) {
          // We fall back to a previous case.
          PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
          const FP_Interval_Type& left_b = left.inhomogeneous_term();
          const FP_Interval_Type& right_a = right.inhomogeneous_term();
          sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                       ROUND_UP);
          div_2exp_assign_r(a_plus_minus_b_minus, a_plus_minus_b_minus, 1,
                            ROUND_UP);
          add_dbm_constraint(0, left_w_id + 1, a_plus_minus_b_minus);
          return;
        }
        if (is_left_coeff_minus_one && is_right_coeff_one) {
          // We fall back to a previous case.
          PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
          const FP_Interval_Type& left_b = left.inhomogeneous_term();
          const FP_Interval_Type& right_a = right.inhomogeneous_term();
          sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                       ROUND_UP);
          div_2exp_assign_r(a_plus_minus_b_minus, a_plus_minus_b_minus, 1,
                            ROUND_UP);
          add_dbm_constraint(right_w_id + 1, 0, a_plus_minus_b_minus);
          return;
        }
      }
      else if (is_left_coeff_minus_one && is_right_coeff_one) {
        // over-approximate (if is it possible) the inequality
        // -B + [b1, b2] <= A + [a1, a2] by adding the constraints
        // -B <= upper_bound(A) + (a2 - b1) and
        // -A <= upper_bound(B) + (a2 - b1)
        PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
        const FP_Interval_Type& left_b = left.inhomogeneous_term();
        const FP_Interval_Type& right_a = right.inhomogeneous_term();
        sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                       ROUND_UP);
        PPL_DIRTY_TEMP(N, ub);
        ub = dbm[0][right_w_id + 1];
        if (!is_plus_infinity(ub)) {
          add_assign_r(ub, ub, a_plus_minus_b_minus, ROUND_UP);
          add_dbm_constraint(left_w_id + 1, 0, ub);
        }
        ub = dbm[0][left_w_id + 1];
        if (!is_plus_infinity(ub)) {
          add_assign_r(ub, ub, a_plus_minus_b_minus, ROUND_UP);
          add_dbm_constraint(right_w_id + 1, 0, ub);
        }
        return;
      }
      if (is_left_coeff_one && is_right_coeff_minus_one) {
        // over-approximate (if is it possible) the inequality
        // B + [b1, b2] <= -A + [a1, a2] by adding the constraints
        // B <= upper_bound(-A) + (a2 - b1) and
        // A <= upper_bound(-B) + (a2 - b1)
        PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
        const FP_Interval_Type& left_b = left.inhomogeneous_term();
        const FP_Interval_Type& right_a = right.inhomogeneous_term();
        sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                       ROUND_UP);
        PPL_DIRTY_TEMP(N, ub);
        ub = dbm[right_w_id + 1][0];
        if (!is_plus_infinity(ub)) {
          add_assign_r(ub, ub, a_plus_minus_b_minus, ROUND_UP);
          add_dbm_constraint(0, left_w_id + 1, ub);
        }
        ub = dbm[left_w_id + 1][0];
        if (!is_plus_infinity(ub)) {
          add_assign_r(ub, ub, a_plus_minus_b_minus, ROUND_UP);
          add_dbm_constraint(0, right_w_id + 1, ub);
        }
            return;
      }
      if (is_left_coeff_one && is_right_coeff_one) {
        PPL_DIRTY_TEMP(N, c_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_c = right.inhomogeneous_term();
        sub_assign_r(c_plus_minus_a_minus, right_c.upper(), left_a.lower(),
                     ROUND_UP);
        add_dbm_constraint(right_w_id+1, left_w_id+1, c_plus_minus_a_minus);
        return;
      }
      if (is_left_coeff_minus_one && is_right_coeff_minus_one) {
        PPL_DIRTY_TEMP(N, c_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_c = right.inhomogeneous_term();
        sub_assign_r(c_plus_minus_a_minus, right_c.upper(), left_a.lower(),
                     ROUND_UP);
        add_dbm_constraint(left_w_id+1, right_w_id+1, c_plus_minus_a_minus);
        return;
      }
    }
}

template <typename T>
template <typename Interval_Info>
void
BD_Shape<T>
::general_refine(const dimension_type& left_w_id,
                 const dimension_type& right_w_id,
                 const Linear_Form< Interval<T, Interval_Info> >& left,
                 const Linear_Form< Interval<T, Interval_Info> >& right) {

  typedef Interval<T, Interval_Info> FP_Interval_Type;
  Linear_Form<FP_Interval_Type> right_minus_left(right);
  right_minus_left -= left;

  // Declare temporaries outside of the loop.
  PPL_DIRTY_TEMP(N, low_coeff);
  PPL_DIRTY_TEMP(N, high_coeff);
  PPL_DIRTY_TEMP(N, upper_bound);

  dimension_type max_w_id = std::max(left_w_id, right_w_id);

  for (dimension_type first_v = 0; first_v < max_w_id; ++first_v) {
    for (dimension_type second_v = first_v+1;
         second_v <= max_w_id; ++second_v) {
      const FP_Interval_Type& lfv_coefficient =
        left.coefficient(Variable(first_v));
      const FP_Interval_Type& lsv_coefficient =
        left.coefficient(Variable(second_v));
      const FP_Interval_Type& rfv_coefficient =
        right.coefficient(Variable(first_v));
      const FP_Interval_Type& rsv_coefficient =
        right.coefficient(Variable(second_v));
      // We update the constraints only when both variables appear in at
      // least one argument.
      bool do_update = false;
      assign_r(low_coeff, lfv_coefficient.lower(), ROUND_NOT_NEEDED);
      assign_r(high_coeff, lfv_coefficient.upper(), ROUND_NOT_NEEDED);
      if (low_coeff != 0 || high_coeff != 0) {
        assign_r(low_coeff, lsv_coefficient.lower(), ROUND_NOT_NEEDED);
        assign_r(high_coeff, lsv_coefficient.upper(), ROUND_NOT_NEEDED);
        if (low_coeff != 0 || high_coeff != 0)
          do_update = true;
        else {
          assign_r(low_coeff, rsv_coefficient.lower(), ROUND_NOT_NEEDED);
          assign_r(high_coeff, rsv_coefficient.upper(), ROUND_NOT_NEEDED);
          if (low_coeff != 0 || high_coeff != 0)
            do_update = true;
        }
      }
      else {
        assign_r(low_coeff, rfv_coefficient.lower(), ROUND_NOT_NEEDED);
        assign_r(high_coeff, rfv_coefficient.upper(), ROUND_NOT_NEEDED);
        if (low_coeff != 0 || high_coeff != 0) {
          assign_r(low_coeff, lsv_coefficient.lower(), ROUND_NOT_NEEDED);
          assign_r(high_coeff, lsv_coefficient.upper(), ROUND_NOT_NEEDED);
          if (low_coeff != 0 || high_coeff != 0)
            do_update = true;
          else {
            assign_r(low_coeff, rsv_coefficient.lower(), ROUND_NOT_NEEDED);
            assign_r(high_coeff, rsv_coefficient.upper(), ROUND_NOT_NEEDED);
            if (low_coeff != 0 || high_coeff != 0)
              do_update = true;
          }
        }
      }

      if (do_update) {
        Variable first(first_v);
        Variable second(second_v);
        dimension_type n_first_var = first_v +1 ;
        dimension_type n_second_var = second_v + 1;
        linear_form_upper_bound(right_minus_left - first + second,
                                upper_bound);
        add_dbm_constraint(n_first_var, n_second_var, upper_bound);
        linear_form_upper_bound(right_minus_left + first - second,
                                upper_bound);
        add_dbm_constraint(n_second_var, n_first_var, upper_bound);
      }
    }
  }

  // Finally, update the unary constraints.
  for (dimension_type v = 0; v < max_w_id; ++v) {
    const FP_Interval_Type& lv_coefficient =
      left.coefficient(Variable(v));
    const FP_Interval_Type& rv_coefficient =
      right.coefficient(Variable(v));
    // We update the constraints only if v appears in at least one of the
    // two arguments.
    bool do_update = false;
    assign_r(low_coeff, lv_coefficient.lower(), ROUND_NOT_NEEDED);
    assign_r(high_coeff, lv_coefficient.upper(), ROUND_NOT_NEEDED);
    if (low_coeff != 0 || high_coeff != 0)
      do_update = true;
    else {
      assign_r(low_coeff, rv_coefficient.lower(), ROUND_NOT_NEEDED);
      assign_r(high_coeff, rv_coefficient.upper(), ROUND_NOT_NEEDED);
      if (low_coeff != 0 || high_coeff != 0)
        do_update = true;
    }

    if (do_update) {
      Variable var(v);
      dimension_type n_var = v + 1;
      linear_form_upper_bound(right_minus_left + var, upper_bound);
      add_dbm_constraint(0, n_var, upper_bound);
      linear_form_upper_bound(right_minus_left - var, upper_bound);
      add_dbm_constraint(n_var, 0, upper_bound);
    }
  }

}

template <typename T>
template <typename Interval_Info>
void
BD_Shape<T>::
linear_form_upper_bound(const Linear_Form< Interval<T, Interval_Info> >& lf,
                        N& result) const {

  // Check that T is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
                     "BD_Shape<T>::linear_form_upper_bound:"
                     " T not a floating point type.");

  const dimension_type lf_space_dimension = lf.space_dimension();
  PPL_ASSERT(lf_space_dimension <= space_dimension());

  typedef Interval<T, Interval_Info> FP_Interval_Type;

  PPL_DIRTY_TEMP(N, curr_lb);
  PPL_DIRTY_TEMP(N, curr_ub);
  PPL_DIRTY_TEMP(N, curr_var_ub);
  PPL_DIRTY_TEMP(N, curr_minus_var_ub);

  PPL_DIRTY_TEMP(N, first_comparison_term);
  PPL_DIRTY_TEMP(N, second_comparison_term);

  PPL_DIRTY_TEMP(N, negator);

  assign_r(result, lf.inhomogeneous_term().upper(), ROUND_NOT_NEEDED);

  for (dimension_type curr_var = 0, n_var = 0; curr_var < lf_space_dimension;
       ++curr_var) {
    n_var = curr_var + 1;
    const FP_Interval_Type&
      curr_coefficient = lf.coefficient(Variable(curr_var));
    assign_r(curr_lb, curr_coefficient.lower(), ROUND_NOT_NEEDED);
    assign_r(curr_ub, curr_coefficient.upper(), ROUND_NOT_NEEDED);
    if (curr_lb != 0 || curr_ub != 0) {
      assign_r(curr_var_ub, dbm[0][n_var], ROUND_NOT_NEEDED);
      neg_assign_r(curr_minus_var_ub, dbm[n_var][0], ROUND_NOT_NEEDED);
      // Optimize the most commons cases: curr = +/-[1, 1].
      if (curr_lb == 1 && curr_ub == 1) {
        add_assign_r(result, result, std::max(curr_var_ub, curr_minus_var_ub),
                     ROUND_UP);
      }
      else if (curr_lb == -1 && curr_ub == -1) {
        neg_assign_r(negator, std::min(curr_var_ub, curr_minus_var_ub),
                     ROUND_NOT_NEEDED);
        add_assign_r(result, result, negator, ROUND_UP);
      }
      else {
        // Next addend will be the maximum of four quantities.
        assign_r(first_comparison_term, 0, ROUND_NOT_NEEDED);
        assign_r(second_comparison_term, 0, ROUND_NOT_NEEDED);
        add_mul_assign_r(first_comparison_term, curr_var_ub, curr_ub,
                         ROUND_UP);
        add_mul_assign_r(second_comparison_term, curr_var_ub, curr_lb,
                         ROUND_UP);
        assign_r(first_comparison_term, std::max(first_comparison_term,
                                                 second_comparison_term),
                 ROUND_NOT_NEEDED);
        assign_r(second_comparison_term, 0, ROUND_NOT_NEEDED);
        add_mul_assign_r(second_comparison_term, curr_minus_var_ub, curr_ub,
                         ROUND_UP);
        assign_r(first_comparison_term, std::max(first_comparison_term,
                                                 second_comparison_term),
                 ROUND_NOT_NEEDED);
        assign_r(second_comparison_term, 0, ROUND_NOT_NEEDED);
        add_mul_assign_r(second_comparison_term, curr_minus_var_ub, curr_lb,
                         ROUND_UP);
        assign_r(first_comparison_term, std::max(first_comparison_term,
                                                 second_comparison_term),
                 ROUND_NOT_NEEDED);

        add_assign_r(result, result, first_comparison_term, ROUND_UP);
      }
    }
  }
}

template <typename T>
void
BD_Shape<T>::affine_preimage(const Variable var,
                             const Linear_Expression& expr,
                             Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("affine_preimage(v, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("affine_preimage(v, e, d)", "e", expr);

  // `var' should be one of the dimensions of
  // the bounded difference shapes.
  const dimension_type v = var.id() + 1;
  if (v > space_dim)
    throw_dimension_incompatible("affine_preimage(v, e, d)", var.id());

  // The image of an empty BDS is empty too.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  const Coefficient& b = expr.inhomogeneous_term();
  // Number of non-zero coefficients in `expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Index of the last non-zero coefficient in `expr', if any.
  dimension_type j = expr.last_nonzero();

  if (j != 0) {
    ++t;
    if (!expr.all_zeroes(1, j))
      ++t;
  }

  // Now we know the form of `expr':
  // - If t == 0, then expr = b, with `b' a constant;
  // - If t == 1, then expr = a*w + b, where `w' can be `v' or another
  //   variable; in this second case we have to check whether `a' is
  //   equal to `denominator' or `-denominator', since otherwise we have
  //   to fall back on the general form;
  // - If t > 1, the `expr' is of the general form.
  if (t == 0) {
    // Case 1: expr = n; remove all constraints on `var'.
    forget_all_dbm_constraints(v);
    // Shortest-path closure is preserved, but not reduction.
    if (marked_shortest_path_reduced())
      reset_shortest_path_reduced();
    PPL_ASSERT(OK());
    return;
  }

  if (t == 1) {
    // Value of the one and only non-zero coefficient in `expr'.
    const Coefficient& a = expr.get(Variable(j - 1));
    if (a == denominator || a == -denominator) {
      // Case 2: expr = a*w + b, with a = +/- denominator.
      if (j == var.space_dimension())
        // Apply affine_image() on the inverse of this transformation.
        affine_image(var, denominator*var - b, a);
      else {
        // `expr == a*w + b', where `w != v'.
        // Remove all constraints on `var'.
        forget_all_dbm_constraints(v);
        // Shortest-path closure is preserved, but not reduction.
        if (marked_shortest_path_reduced())
          reset_shortest_path_reduced();
        PPL_ASSERT(OK());
      }
      return;
    }
  }

  // General case.
  // Either t == 2, so that
  // expr = a_1*x_1 + a_2*x_2 + ... + a_n*x_n + b, where n >= 2,
  // or t = 1, expr = a*w + b, but a <> +/- denominator.
  const Coefficient& expr_v = expr.coefficient(var);
  if (expr_v != 0) {
    // The transformation is invertible.
    Linear_Expression inverse((expr_v + denominator)*var);
    inverse -= expr;
    affine_image(var, inverse, expr_v);
  }
  else {
    // Transformation not invertible: all constraints on `var' are lost.
    forget_all_dbm_constraints(v);
    // Shortest-path closure is preserved, but not reduction.
    if (marked_shortest_path_reduced())
      reset_shortest_path_reduced();
  }
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>
::bounded_affine_image(const Variable var,
                       const Linear_Expression& lb_expr,
                       const Linear_Expression& ub_expr,
                       Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("bounded_affine_image(v, lb, ub, d)", "d == 0");

  // Dimension-compatibility checks.
  // `var' should be one of the dimensions of the BD_Shape.
  const dimension_type bds_space_dim = space_dimension();
  const dimension_type v = var.id() + 1;
  if (v > bds_space_dim)
    throw_dimension_incompatible("bounded_affine_image(v, lb, ub, d)",
                                 "v", var);
  // The dimension of `lb_expr' and `ub_expr' should not be
  // greater than the dimension of `*this'.
  const dimension_type lb_space_dim = lb_expr.space_dimension();
  if (bds_space_dim < lb_space_dim)
    throw_dimension_incompatible("bounded_affine_image(v, lb, ub, d)",
                                 "lb", lb_expr);
  const dimension_type ub_space_dim = ub_expr.space_dimension();
  if (bds_space_dim < ub_space_dim)
    throw_dimension_incompatible("bounded_affine_image(v, lb, ub, d)",
                                 "ub", ub_expr);

  // Any image of an empty BDS is empty.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  const Coefficient& b = ub_expr.inhomogeneous_term();
  // Number of non-zero coefficients in `ub_expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Index of the last non-zero coefficient in `ub_expr', if any.
  dimension_type w = ub_expr.last_nonzero();

  if (w != 0) {
    ++t;
    if (!ub_expr.all_zeroes(1, w))
      ++t;
  }

  // Now we know the form of `ub_expr':
  // - If t == 0, then ub_expr == b, with `b' a constant;
  // - If t == 1, then ub_expr == a*w + b, where `w' can be `v' or another
  //   variable; in this second case we have to check whether `a' is
  //   equal to `denominator' or `-denominator', since otherwise we have
  //   to fall back on the general form;
  // - If t == 2, the `ub_expr' is of the general form.
  PPL_DIRTY_TEMP_COEFFICIENT(minus_denom);
  neg_assign(minus_denom, denominator);

  if (t == 0) {
    // Case 1: ub_expr == b.
    generalized_affine_image(var,
                             GREATER_OR_EQUAL,
                             lb_expr,
                             denominator);
    // Add the constraint `var <= b/denominator'.
    add_dbm_constraint(0, v, b, denominator);
    PPL_ASSERT(OK());
    return;
  }

  if (t == 1) {
    // Value of the one and only non-zero coefficient in `ub_expr'.
    const Coefficient& a = ub_expr.get(Variable(w - 1));
    if (a == denominator || a == minus_denom) {
      // Case 2: expr == a*w + b, with a == +/- denominator.
      if (w == v) {
        // Here `var' occurs in `ub_expr'.
        // To ease the computation, we add an additional dimension.
        const Variable new_var(bds_space_dim);
        add_space_dimensions_and_embed(1);
        // Constrain the new dimension to be equal to `ub_expr'.
        affine_image(new_var, ub_expr, denominator);
        // NOTE: enforce shortest-path closure for precision.
        shortest_path_closure_assign();
        PPL_ASSERT(!marked_empty());
        // Apply the affine lower bound.
        generalized_affine_image(var,
                                 GREATER_OR_EQUAL,
                                 lb_expr,
                                 denominator);
        // Now apply the affine upper bound, as recorded in `new_var'.
        add_constraint(var <= new_var);
        // Remove the temporarily added dimension.
        remove_higher_space_dimensions(bds_space_dim);
        return;
      }
      else {
        // Here `w != v', so that `expr' is of the form
        // +/-denominator * w + b.
        // Apply the affine lower bound.
        generalized_affine_image(var,
                                 GREATER_OR_EQUAL,
                                 lb_expr,
                                 denominator);
        if (a == denominator) {
          // Add the new constraint `v - w == b/denominator'.
          add_dbm_constraint(w, v, b, denominator);
        }
        else {
          // Here a == -denominator, so that we should be adding
          // the constraint `v + w == b/denominator'.
          // Approximate it by computing lower and upper bounds for `w'.
          const N& dbm_w0 = dbm[w][0];
          if (!is_plus_infinity(dbm_w0)) {
            // Add the constraint `v <= b/denominator - lower_w'.
            PPL_DIRTY_TEMP(N, d);
            div_round_up(d, b, denominator);
            add_assign_r(dbm[0][v], d, dbm_w0, ROUND_UP);
            reset_shortest_path_closed();
          }
        }
        PPL_ASSERT(OK());
        return;
      }
    }
  }

  // General case.
  // Either t == 2, so that
  // ub_expr == a_1*x_1 + a_2*x_2 + ... + a_n*x_n + b, where n >= 2,
  // or t == 1, ub_expr == a*w + b, but a <> +/- denominator.
  // We will remove all the constraints on `var' and add back
  // constraints providing upper and lower bounds for `var'.

  // Compute upper approximations for `ub_expr' into `pos_sum'
  // taking into account the sign of `denominator'.
  const bool is_sc = (denominator > 0);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_b);
  neg_assign(minus_b, b);
  const Coefficient& sc_b = is_sc ? b : minus_b;
  const Coefficient& sc_denom = is_sc ? denominator : minus_denom;
  const Coefficient& minus_sc_denom = is_sc ? minus_denom : denominator;
  // NOTE: here, for optimization purposes, `minus_expr' is only assigned
  // when `denominator' is negative. Do not use it unless you are sure
  // it has been correctly assigned.
  Linear_Expression minus_expr;
  if (!is_sc)
    minus_expr = -ub_expr;
  const Linear_Expression& sc_expr = is_sc ? ub_expr : minus_expr;

  PPL_DIRTY_TEMP(N, pos_sum);
  // Index of the variable that are unbounded in `this->dbm'.
  PPL_UNINITIALIZED(dimension_type, pos_pinf_index);
  // Number of unbounded variables found.
  dimension_type pos_pinf_count = 0;

  // Approximate the inhomogeneous term.
  assign_r(pos_sum, sc_b, ROUND_UP);

  // Approximate the homogeneous part of `sc_expr'.
  const DB_Row<N>& dbm_0 = dbm[0];
  // Speculative allocation of temporaries to be used in the following loop.
  PPL_DIRTY_TEMP(N, coeff_i);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
  // Note: indices above `w' can be disregarded, as they all have
  // a zero coefficient in `sc_expr'.
  for (Linear_Expression::const_iterator i = sc_expr.begin(),
        i_end = sc_expr.lower_bound(Variable(w)); i != i_end; ++i) {
    const Coefficient& sc_i = *i;
    const dimension_type i_dim = i.variable().space_dimension();
    const int sign_i = sgn(sc_i);
    if (sign_i > 0) {
      assign_r(coeff_i, sc_i, ROUND_UP);
      // Approximating `sc_expr'.
      if (pos_pinf_count <= 1) {
        const N& up_approx_i = dbm_0[i_dim];
        if (!is_plus_infinity(up_approx_i))
          add_mul_assign_r(pos_sum, coeff_i, up_approx_i, ROUND_UP);
        else {
          ++pos_pinf_count;
          pos_pinf_index = i_dim;
        }
      }
    }
    else {
      PPL_ASSERT(sign_i < 0);
      neg_assign(minus_sc_i, sc_i);
      // Note: using temporary named `coeff_i' to store -coeff_i.
      assign_r(coeff_i, minus_sc_i, ROUND_UP);
      // Approximating `sc_expr'.
      if (pos_pinf_count <= 1) {
        const N& up_approx_minus_i = dbm[i_dim][0];
        if (!is_plus_infinity(up_approx_minus_i))
          add_mul_assign_r(pos_sum, coeff_i, up_approx_minus_i, ROUND_UP);
        else {
          ++pos_pinf_count;
          pos_pinf_index = i_dim;
        }
      }
    }
  }
  // Apply the affine lower bound.
  generalized_affine_image(var,
                           GREATER_OR_EQUAL,
                           lb_expr,
                           denominator);
  // Return immediately if no approximation could be computed.
  if (pos_pinf_count > 1) {
    return;
  }

  // In the following, shortest-path closure will be definitely lost.
  reset_shortest_path_closed();

  // Exploit the upper approximation, if possible.
  if (pos_pinf_count <= 1) {
    // Compute quotient (if needed).
    if (sc_denom != 1) {
      // Before computing quotients, the denominator should be approximated
      // towards zero. Since `sc_denom' is known to be positive, this amounts to
      // rounding downwards, which is achieved as usual by rounding upwards
      // `minus_sc_denom' and negating again the result.
      PPL_DIRTY_TEMP(N, down_sc_denom);
      assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
      neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
      div_assign_r(pos_sum, pos_sum, down_sc_denom, ROUND_UP);
    }
    // Add the upper bound constraint, if meaningful.
    if (pos_pinf_count == 0) {
      // Add the constraint `v <= pos_sum'.
      dbm[0][v] = pos_sum;
      // Deduce constraints of the form `v - u', where `u != v'.
      deduce_v_minus_u_bounds(v, w, sc_expr, sc_denom, pos_sum);
    }
    else
      // Here `pos_pinf_count == 1'.
      if (pos_pinf_index != v
          && sc_expr.get(Variable(pos_pinf_index - 1)) == sc_denom)
        // Add the constraint `v - pos_pinf_index <= pos_sum'.
        dbm[pos_pinf_index][v] = pos_sum;
  }
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>
::bounded_affine_preimage(const Variable var,
                          const Linear_Expression& lb_expr,
                          const Linear_Expression& ub_expr,
                          Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("bounded_affine_preimage(v, lb, ub, d)", "d == 0");

  // Dimension-compatibility checks.
  // `var' should be one of the dimensions of the BD_Shape.
  const dimension_type space_dim = space_dimension();
  const dimension_type v = var.id() + 1;
  if (v > space_dim)
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

  // Any preimage of an empty BDS is empty.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  if (ub_expr.coefficient(var) == 0) {
    refine(var, LESS_OR_EQUAL, ub_expr, denominator);
    generalized_affine_preimage(var, GREATER_OR_EQUAL,
                                lb_expr, denominator);
    return;
  }
  if (lb_expr.coefficient(var) == 0) {
    refine(var, GREATER_OR_EQUAL, lb_expr, denominator);
    generalized_affine_preimage(var, LESS_OR_EQUAL,
                                ub_expr, denominator);
    return;
  }

  const Coefficient& lb_expr_v = lb_expr.coefficient(var);
  // Here `var' occurs in `lb_expr' and `ub_expr'.
  // To ease the computation, we add an additional dimension.
  const Variable new_var(space_dim);
  add_space_dimensions_and_embed(1);
  const Linear_Expression lb_inverse
    = lb_expr - (lb_expr_v + denominator)*var;
  PPL_DIRTY_TEMP_COEFFICIENT(lb_inverse_denom);
  neg_assign(lb_inverse_denom, lb_expr_v);
  affine_image(new_var, lb_inverse, lb_inverse_denom);
  shortest_path_closure_assign();
  PPL_ASSERT(!marked_empty());
  generalized_affine_preimage(var, LESS_OR_EQUAL,
                              ub_expr, denominator);
  if (sgn(denominator) == sgn(lb_inverse_denom))
    add_constraint(var >= new_var);
  else
    add_constraint(var <= new_var);
  // Remove the temporarily added dimension.
  remove_higher_space_dimensions(space_dim);
}

template <typename T>
void
BD_Shape<T>::generalized_affine_image(const Variable var,
                                      const Relation_Symbol relsym,
                                      const Linear_Expression& expr,
                                      Coefficient_traits::const_reference
                                      denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("generalized_affine_image(v, r, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("generalized_affine_image(v, r, e, d)",
                                 "e", expr);

  // `var' should be one of the dimensions of the BDS.
  const dimension_type v = var.id() + 1;
  if (v > space_dim)
    throw_dimension_incompatible("generalized_affine_image(v, r, e, d)",
                                 var.id());

  // The relation symbol cannot be a strict relation symbol.
  if (relsym == LESS_THAN || relsym == GREATER_THAN)
    throw_invalid_argument("generalized_affine_image(v, r, e, d)",
                           "r is a strict relation symbol");
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_image(v, r, e, d)",
                           "r is the disequality relation symbol");

  if (relsym == EQUAL) {
    // The relation symbol is "=":
    // this is just an affine image computation.
    affine_image(var, expr, denominator);
    return;
  }

  // The image of an empty BDS is empty too.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  const Coefficient& b = expr.inhomogeneous_term();
  // Number of non-zero coefficients in `expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Index of the last non-zero coefficient in `expr', if any.
  dimension_type w = expr.last_nonzero();

  if (w != 0) {
    ++t;
    if (!expr.all_zeroes(1, w))
      ++t;
  }

  // Now we know the form of `expr':
  // - If t == 0, then expr == b, with `b' a constant;
  // - If t == 1, then expr == a*w + b, where `w' can be `v' or another
  //   variable; in this second case we have to check whether `a' is
  //   equal to `denominator' or `-denominator', since otherwise we have
  //   to fall back on the general form;
  // - If t == 2, the `expr' is of the general form.
  DB_Row<N>& dbm_0 = dbm[0];
  DB_Row<N>& dbm_v = dbm[v];
  PPL_DIRTY_TEMP_COEFFICIENT(minus_denom);
  neg_assign(minus_denom, denominator);

  if (t == 0) {
    // Case 1: expr == b.
    // Remove all constraints on `var'.
    forget_all_dbm_constraints(v);
    // Both shortest-path closure and reduction are lost.
    reset_shortest_path_closed();
    switch (relsym) {
    case LESS_OR_EQUAL:
      // Add the constraint `var <= b/denominator'.
      add_dbm_constraint(0, v, b, denominator);
      break;
    case GREATER_OR_EQUAL:
      // Add the constraint `var >= b/denominator',
      // i.e., `-var <= -b/denominator',
      add_dbm_constraint(v, 0, b, minus_denom);
      break;
    default:
      // We already dealt with the other cases.
      PPL_UNREACHABLE;
      break;
    }
    PPL_ASSERT(OK());
    return;
  }

  if (t == 1) {
    // Value of the one and only non-zero coefficient in `expr'.
    const Coefficient& a = expr.get(Variable(w - 1));
    if (a == denominator || a == minus_denom) {
      // Case 2: expr == a*w + b, with a == +/- denominator.
      PPL_DIRTY_TEMP(N, d);
      switch (relsym) {
      case LESS_OR_EQUAL:
        div_round_up(d, b, denominator);
        if (w == v) {
          // `expr' is of the form: a*v + b.
          // Shortest-path closure and reduction are not preserved.
          reset_shortest_path_closed();
          if (a == denominator) {
            // Translate each constraint `v - w <= dbm_wv'
            // into the constraint `v - w <= dbm_wv + b/denominator';
            // forget each constraint `w - v <= dbm_vw'.
            for (dimension_type i = space_dim + 1; i-- > 0; ) {
              N& dbm_iv = dbm[i][v];
              add_assign_r(dbm_iv, dbm_iv, d, ROUND_UP);
              assign_r(dbm_v[i], PLUS_INFINITY, ROUND_NOT_NEEDED);
            }
          }
          else {
            // Here `a == -denominator'.
            // Translate the constraint `0 - v <= dbm_v0'
            // into the constraint `0 - v <= dbm_v0 + b/denominator'.
            N& dbm_v0 = dbm_v[0];
            add_assign_r(dbm_0[v], dbm_v0, d, ROUND_UP);
            // Forget all the other constraints on `v'.
            assign_r(dbm_v0, PLUS_INFINITY, ROUND_NOT_NEEDED);
            forget_binary_dbm_constraints(v);
          }
        }
        else {
          // Here `w != v', so that `expr' is of the form
          // +/-denominator * w + b, with `w != v'.
          // Remove all constraints on `v'.
          forget_all_dbm_constraints(v);
          // Shortest-path closure is preserved, but not reduction.
          if (marked_shortest_path_reduced())
            reset_shortest_path_reduced();
          if (a == denominator)
            // Add the new constraint `v - w <= b/denominator'.
            add_dbm_constraint(w, v, d);
          else {
            // Here a == -denominator, so that we should be adding
            // the constraint `v <= b/denominator - w'.
            // Approximate it by computing a lower bound for `w'.
            const N& dbm_w0 = dbm[w][0];
            if (!is_plus_infinity(dbm_w0)) {
              // Add the constraint `v <= b/denominator - lb_w'.
              add_assign_r(dbm_0[v], d, dbm_w0, ROUND_UP);
              // Shortest-path closure is not preserved.
              reset_shortest_path_closed();
            }
          }
        }
        break;

      case GREATER_OR_EQUAL:
        div_round_up(d, b, minus_denom);
        if (w == v) {
          // `expr' is of the form: a*w + b.
          // Shortest-path closure and reduction are not preserved.
          reset_shortest_path_closed();
          if (a == denominator) {
            // Translate each constraint `w - v <= dbm_vw'
            // into the constraint `w - v <= dbm_vw - b/denominator';
            // forget each constraint `v - w <= dbm_wv'.
            for (dimension_type i = space_dim + 1; i-- > 0; ) {
              N& dbm_vi = dbm_v[i];
              add_assign_r(dbm_vi, dbm_vi, d, ROUND_UP);
              assign_r(dbm[i][v], PLUS_INFINITY, ROUND_NOT_NEEDED);
            }
          }
          else {
            // Here `a == -denominator'.
            // Translate the constraint `0 - v <= dbm_v0'
            // into the constraint `0 - v <= dbm_0v - b/denominator'.
            N& dbm_0v = dbm_0[v];
            add_assign_r(dbm_v[0], dbm_0v, d, ROUND_UP);
            // Forget all the other constraints on `v'.
            assign_r(dbm_0v, PLUS_INFINITY, ROUND_NOT_NEEDED);
            forget_binary_dbm_constraints(v);
          }
        }
        else {
          // Here `w != v', so that `expr' is of the form
          // +/-denominator * w + b, with `w != v'.
          // Remove all constraints on `v'.
          forget_all_dbm_constraints(v);
          // Shortest-path closure is preserved, but not reduction.
          if (marked_shortest_path_reduced())
            reset_shortest_path_reduced();
          if (a == denominator)
            // Add the new constraint `v - w >= b/denominator',
            // i.e., `w - v <= -b/denominator'.
            add_dbm_constraint(v, w, d);
          else {
            // Here a == -denominator, so that we should be adding
            // the constraint `v >= -w + b/denominator',
            // i.e., `-v <= w - b/denominator'.
            // Approximate it by computing an upper bound for `w'.
            const N& dbm_0w = dbm_0[w];
            if (!is_plus_infinity(dbm_0w)) {
              // Add the constraint `-v <= ub_w - b/denominator'.
              add_assign_r(dbm_v[0], dbm_0w, d, ROUND_UP);
              // Shortest-path closure is not preserved.
              reset_shortest_path_closed();
            }
          }
        }
        break;

      default:
        // We already dealt with the other cases.
        PPL_UNREACHABLE;
        break;
      }
      PPL_ASSERT(OK());
      return;
    }
  }

  // General case.
  // Either t == 2, so that
  // expr == a_1*x_1 + a_2*x_2 + ... + a_n*x_n + b, where n >= 2,
  // or t == 1, expr == a*w + b, but a <> +/- denominator.
  // We will remove all the constraints on `v' and add back
  // a constraint providing an upper or a lower bound for `v'
  // (depending on `relsym').
  const bool is_sc = (denominator > 0);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_b);
  neg_assign(minus_b, b);
  const Coefficient& sc_b = is_sc ? b : minus_b;
  const Coefficient& minus_sc_b = is_sc ? minus_b : b;
  const Coefficient& sc_denom = is_sc ? denominator : minus_denom;
  const Coefficient& minus_sc_denom = is_sc ? minus_denom : denominator;
  // NOTE: here, for optimization purposes, `minus_expr' is only assigned
  // when `denominator' is negative. Do not use it unless you are sure
  // it has been correctly assigned.
  Linear_Expression minus_expr;
  if (!is_sc)
    minus_expr = -expr;
  const Linear_Expression& sc_expr = is_sc ? expr : minus_expr;

  PPL_DIRTY_TEMP(N, sum);
  // Index of variable that is unbounded in `this->dbm'.
  PPL_UNINITIALIZED(dimension_type, pinf_index);
  // Number of unbounded variables found.
  dimension_type pinf_count = 0;

  // Speculative allocation of temporaries to be used in the following loops.
  PPL_DIRTY_TEMP(N, coeff_i);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);

  switch (relsym) {
  case LESS_OR_EQUAL:
    // Compute an upper approximation for `sc_expr' into `sum'.

    // Approximate the inhomogeneous term.
    assign_r(sum, sc_b, ROUND_UP);
    // Approximate the homogeneous part of `sc_expr'.
    // Note: indices above `w' can be disregarded, as they all have
    // a zero coefficient in `sc_expr'.
    PPL_ASSERT(w != 0);
    for (Linear_Expression::const_iterator i = sc_expr.begin(),
        i_end = sc_expr.lower_bound(Variable(w)); i != i_end; ++i) {
      const Coefficient& sc_i = *i;
      const dimension_type i_dim = i.variable().space_dimension();
      const int sign_i = sgn(sc_i);
      PPL_ASSERT(sign_i != 0);
      // Choose carefully: we are approximating `sc_expr'.
      const N& approx_i = (sign_i > 0) ? dbm_0[i_dim] : dbm[i_dim][0];
      if (is_plus_infinity(approx_i)) {
        if (++pinf_count > 1)
          break;
        pinf_index = i_dim;
        continue;
      }
      if (sign_i > 0)
        assign_r(coeff_i, sc_i, ROUND_UP);
      else {
        neg_assign(minus_sc_i, sc_i);
        assign_r(coeff_i, minus_sc_i, ROUND_UP);
      }
      add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
    }

    // Remove all constraints on `v'.
    forget_all_dbm_constraints(v);
    // Shortest-path closure is preserved, but not reduction.
    if (marked_shortest_path_reduced())
      reset_shortest_path_reduced();
    // Return immediately if no approximation could be computed.
    if (pinf_count > 1) {
      PPL_ASSERT(OK());
      return;
    }

    // Divide by the (sign corrected) denominator (if needed).
    if (sc_denom != 1) {
      // Before computing the quotient, the denominator should be approximated
      // towards zero. Since `sc_denom' is known to be positive, this amounts to
      // rounding downwards, which is achieved as usual by rounding upwards
      // `minus_sc_denom' and negating again the result.
      PPL_DIRTY_TEMP(N, down_sc_denom);
      assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
      neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
      div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
    }

    if (pinf_count == 0) {
      // Add the constraint `v <= sum'.
      add_dbm_constraint(0, v, sum);
      // Deduce constraints of the form `v - u', where `u != v'.
      deduce_v_minus_u_bounds(v, w, sc_expr, sc_denom, sum);
    }
    else if (pinf_count == 1)
      if (pinf_index != v && expr.get(Variable(pinf_index - 1)) == denominator)
        // Add the constraint `v - pinf_index <= sum'.
        add_dbm_constraint(pinf_index, v, sum);
    break;

  case GREATER_OR_EQUAL:
    // Compute an upper approximation for `-sc_expr' into `sum'.
    // Note: approximating `-sc_expr' from above and then negating the
    // result is the same as approximating `sc_expr' from below.

    // Approximate the inhomogeneous term.
    assign_r(sum, minus_sc_b, ROUND_UP);
    // Approximate the homogeneous part of `-sc_expr'.
    for (Linear_Expression::const_iterator i = sc_expr.begin(),
        i_end = sc_expr.lower_bound(Variable(w)); i != i_end; ++i) {
      const Coefficient& sc_i = *i;
      const int sign_i = sgn(sc_i);
      PPL_ASSERT(sign_i != 0);
      const dimension_type i_dim = i.variable().space_dimension();
      // Choose carefully: we are approximating `-sc_expr'.
      const N& approx_i = (sign_i > 0) ? dbm[i_dim][0] : dbm_0[i_dim];
      if (is_plus_infinity(approx_i)) {
        if (++pinf_count > 1)
          break;
        pinf_index = i_dim;
        continue;
      }
      if (sign_i > 0)
        assign_r(coeff_i, sc_i, ROUND_UP);
      else {
        neg_assign(minus_sc_i, sc_i);
        assign_r(coeff_i, minus_sc_i, ROUND_UP);
      }
      add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
    }

    // Remove all constraints on `var'.
    forget_all_dbm_constraints(v);
    // Shortest-path closure is preserved, but not reduction.
    if (marked_shortest_path_reduced())
      reset_shortest_path_reduced();
    // Return immediately if no approximation could be computed.
    if (pinf_count > 1) {
      PPL_ASSERT(OK());
      return;
    }

    // Divide by the (sign corrected) denominator (if needed).
    if (sc_denom != 1) {
      // Before computing the quotient, the denominator should be approximated
      // towards zero. Since `sc_denom' is known to be positive, this amounts to
      // rounding downwards, which is achieved as usual by rounding upwards
      // `minus_sc_denom' and negating again the result.
      PPL_DIRTY_TEMP(N, down_sc_denom);
      assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
      neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
      div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
    }

    if (pinf_count == 0) {
      // Add the constraint `v >= -sum', i.e., `-v <= sum'.
      add_dbm_constraint(v, 0, sum);
      // Deduce constraints of the form `u - v', where `u != v'.
      deduce_u_minus_v_bounds(v, w, sc_expr, sc_denom, sum);
    }
    else if (pinf_count == 1)
      if (pinf_index != v && expr.get(Variable(pinf_index - 1)) == denominator)
        // Add the constraint `v - pinf_index >= -sum',
        // i.e., `pinf_index - v <= sum'.
        add_dbm_constraint(v, pinf_index, sum);
    break;

  default:
    // We already dealt with the other cases.
    PPL_UNREACHABLE;
    break;
  }
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::generalized_affine_image(const Linear_Expression& lhs,
                                      const Relation_Symbol relsym,
                                      const Linear_Expression& rhs) {
  // Dimension-compatibility checks.
  // The dimension of `lhs' should not be greater than the dimension
  // of `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type lhs_space_dim = lhs.space_dimension();
  if (space_dim < lhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2)",
                                 "e1", lhs);

  // The dimension of `rhs' should not be greater than the dimension
  // of `*this'.
  const dimension_type rhs_space_dim = rhs.space_dimension();
  if (space_dim < rhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2)",
                                 "e2", rhs);

  // Strict relation symbols are not admitted for BDSs.
  if (relsym == LESS_THAN || relsym == GREATER_THAN)
    throw_invalid_argument("generalized_affine_image(e1, r, e2)",
                           "r is a strict relation symbol");
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_image(e1, r, e2)",
                           "r is the disequality relation symbol");

  // The image of an empty BDS is empty.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  // Number of non-zero coefficients in `lhs': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t_lhs = 0;
  // Index of the last non-zero coefficient in `lhs', if any.
  dimension_type j_lhs = lhs.last_nonzero();

  if (j_lhs != 0) {
    ++t_lhs;
    if (!lhs.all_zeroes(1, j_lhs))
      ++t_lhs;
    --j_lhs;
  }

  const Coefficient& b_lhs = lhs.inhomogeneous_term();

  if (t_lhs == 0) {
    // `lhs' is a constant.
    // In principle, it is sufficient to add the constraint `lhs relsym rhs'.
    // Note that this constraint is a bounded difference if `t_rhs <= 1'
    // or `t_rhs > 1' and `rhs == a*v - a*w + b_rhs'. If `rhs' is of a
    // more general form, it will be simply ignored.
    // TODO: if it is not a bounded difference, should we compute
    // approximations for this constraint?
    switch (relsym) {
    case LESS_OR_EQUAL:
      refine_no_check(lhs <= rhs);
      break;
    case EQUAL:
      refine_no_check(lhs == rhs);
      break;
    case GREATER_OR_EQUAL:
      refine_no_check(lhs >= rhs);
      break;
    default:
      // We already dealt with the other cases.
      PPL_UNREACHABLE;
      break;
    }
  }
  else if (t_lhs == 1) {
    // Here `lhs == a_lhs * v + b_lhs'.
    // Independently from the form of `rhs', we can exploit the
    // method computing generalized affine images for a single variable.
    Variable v(j_lhs);
    // Compute a sign-corrected relation symbol.
    const Coefficient& denom = lhs.coefficient(v);
    Relation_Symbol new_relsym = relsym;
    if (denom < 0) {
      if (relsym == LESS_OR_EQUAL)
        new_relsym = GREATER_OR_EQUAL;
      else if (relsym == GREATER_OR_EQUAL)
        new_relsym = LESS_OR_EQUAL;
    }
    Linear_Expression expr = rhs - b_lhs;
    generalized_affine_image(v, new_relsym, expr, denom);
  }
  else {
    // Here `lhs' is of the general form, having at least two variables.
    // Compute the set of variables occurring in `lhs'.
    std::vector<Variable> lhs_vars;
    for (Linear_Expression::const_iterator i = lhs.begin(), i_end = lhs.end();
          i != i_end; ++i)
      lhs_vars.push_back(i.variable());

    const dimension_type num_common_dims = std::min(lhs_space_dim, rhs_space_dim);
    if (!lhs.have_a_common_variable(rhs, Variable(0), Variable(num_common_dims))) {
      // `lhs' and `rhs' variables are disjoint.
      // Existentially quantify all variables in the lhs.
      for (dimension_type i = lhs_vars.size(); i-- > 0; )
        forget_all_dbm_constraints(lhs_vars[i].id() + 1);
      // Constrain the left hand side expression so that it is related to
      // the right hand side expression as dictated by `relsym'.
      // TODO: if the following constraint is NOT a bounded difference,
      // it will be simply ignored. Should we compute approximations for it?
      switch (relsym) {
      case LESS_OR_EQUAL:
        refine_no_check(lhs <= rhs);
        break;
      case EQUAL:
        refine_no_check(lhs == rhs);
        break;
      case GREATER_OR_EQUAL:
        refine_no_check(lhs >= rhs);
        break;
      default:
        // We already dealt with the other cases.
        PPL_UNREACHABLE;
        break;
      }
    }
    else {
      // Some variables in `lhs' also occur in `rhs'.

#if 1 // Simplified computation (see the TODO note below).

      for (dimension_type i = lhs_vars.size(); i-- > 0; )
        forget_all_dbm_constraints(lhs_vars[i].id() + 1);

#else // Currently unnecessarily complex computation.

      // More accurate computation that is worth doing only if
      // the following TODO note is accurately dealt with.

      // To ease the computation, we add an additional dimension.
      const Variable new_var(space_dim);
      add_space_dimensions_and_embed(1);
      // Constrain the new dimension to be equal to `rhs'.
      // NOTE: calling affine_image() instead of refine_no_check()
      // ensures some approximation is tried even when the constraint
      // is not a bounded difference.
      affine_image(new_var, rhs);
      // Existentially quantify all variables in the lhs.
      // NOTE: enforce shortest-path closure for precision.
      shortest_path_closure_assign();
      PPL_ASSERT(!marked_empty());
      for (dimension_type i = lhs_vars.size(); i-- > 0; )
        forget_all_dbm_constraints(lhs_vars[i].id() + 1);
      // Constrain the new dimension so that it is related to
      // the left hand side as dictated by `relsym'.
      // TODO: each one of the following constraints is definitely NOT
      // a bounded differences (since it has 3 variables at least).
      // Thus, the method refine_no_check() will simply ignore it.
      // Should we compute approximations for this constraint?
      switch (relsym) {
      case LESS_OR_EQUAL:
        refine_no_check(lhs <= new_var);
        break;
      case EQUAL:
        refine_no_check(lhs == new_var);
        break;
      case GREATER_OR_EQUAL:
        refine_no_check(lhs >= new_var);
        break;
      default:
        // We already dealt with the other cases.
        PPL_UNREACHABLE;
        break;
      }
      // Remove the temporarily added dimension.
      remove_higher_space_dimensions(space_dim-1);
#endif // Currently unnecessarily complex computation.
    }
  }

  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::generalized_affine_preimage(const Variable var,
                                         const Relation_Symbol relsym,
                                         const Linear_Expression& expr,
                                         Coefficient_traits::const_reference
                                         denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("generalized_affine_preimage(v, r, e, d)",
                           "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type space_dim = space_dimension();
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(v, r, e, d)",
                                 "e", expr);

  // `var' should be one of the dimensions of the BDS.
  const dimension_type v = var.id() + 1;
  if (v > space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(v, r, e, d)",
                                 var.id());

  // The relation symbol cannot be a strict relation symbol.
  if (relsym == LESS_THAN || relsym == GREATER_THAN)
    throw_invalid_argument("generalized_affine_preimage(v, r, e, d)",
                           "r is a strict relation symbol");
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_preimage(v, r, e, d)",
                           "r is the disequality relation symbol");

  if (relsym == EQUAL) {
    // The relation symbol is "=":
    // this is just an affine preimage computation.
    affine_preimage(var, expr, denominator);
    return;
  }

  // The preimage of an empty BDS is empty too.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  // Check whether the preimage of this affine relation can be easily
  // computed as the image of its inverse relation.
  const Coefficient& expr_v = expr.coefficient(var);
  if (expr_v != 0) {
    const Relation_Symbol reversed_relsym = (relsym == LESS_OR_EQUAL)
      ? GREATER_OR_EQUAL : LESS_OR_EQUAL;
    const Linear_Expression inverse
      = expr - (expr_v + denominator)*var;
    PPL_DIRTY_TEMP_COEFFICIENT(inverse_denom);
    neg_assign(inverse_denom, expr_v);
    const Relation_Symbol inverse_relsym
      = (sgn(denominator) == sgn(inverse_denom)) ? relsym : reversed_relsym;
    generalized_affine_image(var, inverse_relsym, inverse, inverse_denom);
    return;
  }

  refine(var, relsym, expr, denominator);
  // If the shrunk BD_Shape is empty, its preimage is empty too; ...
  if (is_empty())
    return;
  // ...  otherwise, since the relation was not invertible,
  // we just forget all constraints on `v'.
  forget_all_dbm_constraints(v);
  // Shortest-path closure is preserved, but not reduction.
  if (marked_shortest_path_reduced())
    reset_shortest_path_reduced();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::generalized_affine_preimage(const Linear_Expression& lhs,
                                         const Relation_Symbol relsym,
                                         const Linear_Expression& rhs) {
  // Dimension-compatibility checks.
  // The dimension of `lhs' should not be greater than the dimension
  // of `*this'.
  const dimension_type bds_space_dim = space_dimension();
  const dimension_type lhs_space_dim = lhs.space_dimension();
  if (bds_space_dim < lhs_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(e1, r, e2)",
                                 "e1", lhs);

  // The dimension of `rhs' should not be greater than the dimension
  // of `*this'.
  const dimension_type rhs_space_dim = rhs.space_dimension();
  if (bds_space_dim < rhs_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(e1, r, e2)",
                                 "e2", rhs);

  // Strict relation symbols are not admitted for BDSs.
  if (relsym == LESS_THAN || relsym == GREATER_THAN)
    throw_invalid_argument("generalized_affine_preimage(e1, r, e2)",
                           "r is a strict relation symbol");
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_preimage(e1, r, e2)",
                           "r is the disequality relation symbol");

  // The preimage of an empty BDS is empty.
  shortest_path_closure_assign();
  if (marked_empty())
    return;

  // Number of non-zero coefficients in `lhs': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t_lhs = 0;
  // Index of the last non-zero coefficient in `lhs', if any.
  dimension_type j_lhs = lhs.last_nonzero();

  if (j_lhs != 0) {
    ++t_lhs;
    if (!lhs.all_zeroes(1, j_lhs))
      ++t_lhs;
    --j_lhs;
  }

  const Coefficient& b_lhs = lhs.inhomogeneous_term();

  if (t_lhs == 0) {
    // `lhs' is a constant.
    // In this case, preimage and image happen to be the same.
    generalized_affine_image(lhs, relsym, rhs);
    return;
  }
  else if (t_lhs == 1) {
    // Here `lhs == a_lhs * v + b_lhs'.
    // Independently from the form of `rhs', we can exploit the
    // method computing generalized affine preimages for a single variable.
    Variable v(j_lhs);
    // Compute a sign-corrected relation symbol.
    const Coefficient& denom = lhs.coefficient(v);
    Relation_Symbol new_relsym = relsym;
    if (denom < 0) {
      if (relsym == LESS_OR_EQUAL)
        new_relsym = GREATER_OR_EQUAL;
      else if (relsym == GREATER_OR_EQUAL)
        new_relsym = LESS_OR_EQUAL;
    }
    Linear_Expression expr = rhs - b_lhs;
    generalized_affine_preimage(v, new_relsym, expr, denom);
  }
  else {
    // Here `lhs' is of the general form, having at least two variables.
    // Compute the set of variables occurring in `lhs'.
    std::vector<Variable> lhs_vars;
    for (Linear_Expression::const_iterator i = lhs.begin(), i_end = lhs.end();
          i != i_end; ++i)
      lhs_vars.push_back(i.variable());

    const dimension_type num_common_dims = std::min(lhs_space_dim, rhs_space_dim);
    if (!lhs.have_a_common_variable(rhs, Variable(0), Variable(num_common_dims))) {
      // `lhs' and `rhs' variables are disjoint.

      // Constrain the left hand side expression so that it is related to
      // the right hand side expression as dictated by `relsym'.
      // TODO: if the following constraint is NOT a bounded difference,
      // it will be simply ignored. Should we compute approximations for it?
      switch (relsym) {
      case LESS_OR_EQUAL:
        refine_no_check(lhs <= rhs);
        break;
      case EQUAL:
        refine_no_check(lhs == rhs);
        break;
      case GREATER_OR_EQUAL:
        refine_no_check(lhs >= rhs);
        break;
      default:
        // We already dealt with the other cases.
        PPL_UNREACHABLE;
        break;
      }

      // If the shrunk BD_Shape is empty, its preimage is empty too; ...
      if (is_empty())
        return;
      // Existentially quantify all variables in the lhs.
      for (dimension_type i = lhs_vars.size(); i-- > 0; )
        forget_all_dbm_constraints(lhs_vars[i].id() + 1);
    }
    else {

      // Some variables in `lhs' also occur in `rhs'.
      // To ease the computation, we add an additional dimension.
      const Variable new_var(bds_space_dim);
      add_space_dimensions_and_embed(1);
      // Constrain the new dimension to be equal to `lhs'.
      // NOTE: calling affine_image() instead of refine_no_check()
      // ensures some approximation is tried even when the constraint
      // is not a bounded difference.
      affine_image(new_var, lhs);
      // Existentiallly quantify all variables in the lhs.
      // NOTE: enforce shortest-path closure for precision.
      shortest_path_closure_assign();
      PPL_ASSERT(!marked_empty());
      for (dimension_type i = lhs_vars.size(); i-- > 0; )
        forget_all_dbm_constraints(lhs_vars[i].id() + 1);
      // Constrain the new dimension so that it is related to
      // the left hand side as dictated by `relsym'.
      // Note: if `rhs == a_rhs*v + b_rhs' where `a_rhs' is in {0, 1},
      // then one of the following constraints will be added,
      // since it is a bounded difference. Else the method
      // refine_no_check() will ignore it, because the
      // constraint is NOT a bounded difference.
      switch (relsym) {
      case LESS_OR_EQUAL:
        refine_no_check(new_var <= rhs);
        break;
      case EQUAL:
        refine_no_check(new_var == rhs);
        break;
      case GREATER_OR_EQUAL:
        refine_no_check(new_var >= rhs);
        break;
      default:
        // We already dealt with the other cases.
        PPL_UNREACHABLE;
        break;
      }
      // Remove the temporarily added dimension.
      remove_higher_space_dimensions(bds_space_dim);
    }
  }

  PPL_ASSERT(OK());
}

template <typename T>
Constraint_System
BD_Shape<T>::constraints() const {
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

  if (marked_shortest_path_reduced()) {
    // Disregard redundant constraints.
    cs = minimized_constraints();
    return cs;
  }

  PPL_DIRTY_TEMP_COEFFICIENT(a);
  PPL_DIRTY_TEMP_COEFFICIENT(b);
  // Go through all the unary constraints in `dbm'.
  const DB_Row<N>& dbm_0 = dbm[0];
  for (dimension_type j = 1; j <= space_dim; ++j) {
    const Variable x(j-1);
    const N& dbm_0j = dbm_0[j];
    const N& dbm_j0 = dbm[j][0];
    if (is_additive_inverse(dbm_j0, dbm_0j)) {
      // We have a unary equality constraint.
      numer_denom(dbm_0j, b, a);
      cs.insert(a*x == b);
    }
    else {
      // We have 0, 1 or 2 unary inequality constraints.
      if (!is_plus_infinity(dbm_0j)) {
        numer_denom(dbm_0j, b, a);
        cs.insert(a*x <= b);
      }
      if (!is_plus_infinity(dbm_j0)) {
        numer_denom(dbm_j0, b, a);
        cs.insert(-a*x <= b);
      }
    }
  }

  // Go through all the binary constraints in `dbm'.
  for (dimension_type i = 1; i <= space_dim; ++i) {
    const Variable y(i-1);
    const DB_Row<N>& dbm_i = dbm[i];
    for (dimension_type j = i + 1; j <= space_dim; ++j) {
      const Variable x(j-1);
      const N& dbm_ij = dbm_i[j];
      const N& dbm_ji = dbm[j][i];
      if (is_additive_inverse(dbm_ji, dbm_ij)) {
        // We have a binary equality constraint.
        numer_denom(dbm_ij, b, a);
        cs.insert(a*x - a*y == b);
      }
      else {
        // We have 0, 1 or 2 binary inequality constraints.
        if (!is_plus_infinity(dbm_ij)) {
          numer_denom(dbm_ij, b, a);
          cs.insert(a*x - a*y <= b);
        }
        if (!is_plus_infinity(dbm_ji)) {
          numer_denom(dbm_ji, b, a);
          cs.insert(a*y - a*x <= b);
        }
      }
    }
  }
  return cs;
}

template <typename T>
Constraint_System
BD_Shape<T>::minimized_constraints() const {
  shortest_path_reduction_assign();
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

  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);

  // Compute leader information.
  std::vector<dimension_type> leaders;
  compute_leaders(leaders);
  std::vector<dimension_type> leader_indices;
  compute_leader_indices(leaders, leader_indices);
  const dimension_type num_leaders = leader_indices.size();

  // Go through the non-leaders to generate equality constraints.
  const DB_Row<N>& dbm_0 = dbm[0];
  for (dimension_type i = 1; i <= space_dim; ++i) {
    const dimension_type leader = leaders[i];
    if (i != leader) {
      // Generate the constraint relating `i' and its leader.
      if (leader == 0) {
        // A unary equality has to be generated.
        PPL_ASSERT(!is_plus_infinity(dbm_0[i]));
        numer_denom(dbm_0[i], numer, denom);
        cs.insert(denom*Variable(i-1) == numer);
      }
      else {
        // A binary equality has to be generated.
        PPL_ASSERT(!is_plus_infinity(dbm[i][leader]));
        numer_denom(dbm[i][leader], numer, denom);
        cs.insert(denom*Variable(leader-1) - denom*Variable(i-1) == numer);
      }
    }
  }

  // Go through the leaders to generate inequality constraints.
  // First generate all the unary inequalities.
  const Bit_Row& red_0 = redundancy_dbm[0];
  for (dimension_type l_i = 1; l_i < num_leaders; ++l_i) {
    const dimension_type i = leader_indices[l_i];
    if (!red_0[i]) {
      numer_denom(dbm_0[i], numer, denom);
      cs.insert(denom*Variable(i-1) <= numer);
    }
    if (!redundancy_dbm[i][0]) {
      numer_denom(dbm[i][0], numer, denom);
      cs.insert(-denom*Variable(i-1) <= numer);
    }
  }
  // Then generate all the binary inequalities.
  for (dimension_type l_i = 1; l_i < num_leaders; ++l_i) {
    const dimension_type i = leader_indices[l_i];
    const DB_Row<N>& dbm_i = dbm[i];
    const Bit_Row& red_i = redundancy_dbm[i];
    for (dimension_type l_j = l_i + 1; l_j < num_leaders; ++l_j) {
      const dimension_type j = leader_indices[l_j];
      if (!red_i[j]) {
        numer_denom(dbm_i[j], numer, denom);
        cs.insert(denom*Variable(j-1) - denom*Variable(i-1) <= numer);
      }
      if (!redundancy_dbm[j][i]) {
        numer_denom(dbm[j][i], numer, denom);
        cs.insert(denom*Variable(i-1) - denom*Variable(j-1) <= numer);
      }
    }
  }
  return cs;
}

template <typename T>
void
BD_Shape<T>::expand_space_dimension(Variable var, dimension_type m) {
  dimension_type old_dim = space_dimension();
  // `var' should be one of the dimensions of the vector space.
  if (var.space_dimension() > old_dim)
    throw_dimension_incompatible("expand_space_dimension(v, m)", "v", var);

  // The space dimension of the resulting BDS should not
  // overflow the maximum allowed space dimension.
  if (m > max_space_dimension() - space_dimension())
    throw_invalid_argument("expand_dimension(v, m)",
                           "adding m new space dimensions exceeds "
                           "the maximum allowed space dimension");

  // Nothing to do, if no dimensions must be added.
  if (m == 0)
    return;

  // Add the required new dimensions.
  add_space_dimensions_and_embed(m);

  // For each constraints involving variable `var', we add a
  // similar constraint with the new variable substituted for
  // variable `var'.
  const dimension_type v_id = var.id() + 1;
  const DB_Row<N>& dbm_v = dbm[v_id];
  for (dimension_type i = old_dim + 1; i-- > 0; ) {
    DB_Row<N>& dbm_i = dbm[i];
    const N& dbm_i_v = dbm[i][v_id];
    const N& dbm_v_i = dbm_v[i];
    for (dimension_type j = old_dim+1; j < old_dim+m+1; ++j) {
      dbm_i[j] = dbm_i_v;
      dbm[j][i] = dbm_v_i;
    }
  }
  // In general, adding a constraint does not preserve the shortest-path
  // closure or reduction of the bounded difference shape.
  if (marked_shortest_path_closed())
    reset_shortest_path_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::fold_space_dimensions(const Variables_Set& vars,
                                   Variable dest) {
  const dimension_type space_dim = space_dimension();
  // `dest' should be one of the dimensions of the BDS.
  if (dest.space_dimension() > space_dim)
    throw_dimension_incompatible("fold_space_dimensions(vs, v)",
                                 "v", dest);

  // The folding of no dimensions is a no-op.
  if (vars.empty())
    return;

  // All variables in `vars' should be dimensions of the BDS.
  if (vars.space_dimension() > space_dim)
    throw_dimension_incompatible("fold_space_dimensions(vs, v)",
                                 vars.space_dimension());

  // Moreover, `dest.id()' should not occur in `vars'.
  if (vars.find(dest.id()) != vars.end())
    throw_invalid_argument("fold_space_dimensions(vs, v)",
                           "v should not occur in vs");

  shortest_path_closure_assign();
  if (!marked_empty()) {
    // Recompute the elements of the row and the column corresponding
    // to variable `dest' by taking the join of their value with the
    // value of the corresponding elements in the row and column of the
    // variable `vars'.
    const dimension_type v_id = dest.id() + 1;
    DB_Row<N>& dbm_v = dbm[v_id];
    for (Variables_Set::const_iterator i = vars.begin(),
           vs_end = vars.end(); i != vs_end; ++i) {
      const dimension_type to_be_folded_id = *i + 1;
      const DB_Row<N>& dbm_to_be_folded_id = dbm[to_be_folded_id];
      for (dimension_type j = space_dim + 1; j-- > 0; ) {
        max_assign(dbm[j][v_id], dbm[j][to_be_folded_id]);
        max_assign(dbm_v[j], dbm_to_be_folded_id[j]);
      }
    }
  }
  remove_space_dimensions(vars);
}

template <typename T>
void
BD_Shape<T>::drop_some_non_integer_points(Complexity_Class) {
  if (std::numeric_limits<T>::is_integer)
    return;

  const dimension_type space_dim = space_dimension();
  shortest_path_closure_assign();
  if (space_dim == 0 || marked_empty())
    return;

  for (dimension_type i = space_dim + 1; i-- > 0; ) {
    DB_Row<N>& dbm_i = dbm[i];
    for (dimension_type j = space_dim + 1; j-- > 0; )
      if (i != j)
        drop_some_non_integer_points_helper(dbm_i[j]);
  }
  PPL_ASSERT(OK());
}

template <typename T>
void
BD_Shape<T>::drop_some_non_integer_points(const Variables_Set& vars,
                                          Complexity_Class) {
  // Dimension-compatibility check.
  const dimension_type space_dim = space_dimension();
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dim < min_space_dim)
    throw_dimension_incompatible("drop_some_non_integer_points(vs, cmpl)",
                                 min_space_dim);

  if (std::numeric_limits<T>::is_integer || min_space_dim == 0)
    return;

  shortest_path_closure_assign();
  if (marked_empty())
    return;

  const Variables_Set::const_iterator v_begin = vars.begin();
  const Variables_Set::const_iterator v_end = vars.end();
  PPL_ASSERT(v_begin != v_end);
  // Unary constraints on a variable occurring in `vars'.
  DB_Row<N>& dbm_0 = dbm[0];
  for (Variables_Set::const_iterator v_i = v_begin; v_i != v_end; ++v_i) {
    const dimension_type i = *v_i + 1;
    drop_some_non_integer_points_helper(dbm_0[i]);
    drop_some_non_integer_points_helper(dbm[i][0]);
  }

  // Binary constraints where both variables occur in `vars'.
  for (Variables_Set::const_iterator v_i = v_begin; v_i != v_end; ++v_i) {
    const dimension_type i = *v_i + 1;
    DB_Row<N>& dbm_i = dbm[i];
    for (Variables_Set::const_iterator v_j = v_begin; v_j != v_end; ++v_j) {
      const dimension_type j = *v_j + 1;
      if (i != j)
        drop_some_non_integer_points_helper(dbm_i[j]);
    }
  }
  PPL_ASSERT(OK());
}

/*! \relates Parma_Polyhedra_Library::BD_Shape */
template <typename T>
std::ostream&
IO_Operators::operator<<(std::ostream& s, const BD_Shape<T>& bds) {
  typedef typename BD_Shape<T>::coefficient_type N;
  if (bds.is_universe())
    s << "true";
  else {
    // We control empty bounded difference shape.
    dimension_type n = bds.space_dimension();
    if (bds.marked_empty())
      s << "false";
    else {
      PPL_DIRTY_TEMP(N, v);
      bool first = true;
      for (dimension_type i = 0; i <= n; ++i)
        for (dimension_type j = i + 1; j <= n; ++j) {
          const N& c_i_j = bds.dbm[i][j];
          const N& c_j_i = bds.dbm[j][i];
          if (is_additive_inverse(c_j_i, c_i_j)) {
            // We will print an equality.
            if (first)
              first = false;
            else
              s << ", ";
            if (i == 0) {
              // We have got a equality constraint with one variable.
              s << Variable(j - 1);
              s << " = " << c_i_j;
            }
            else {
              // We have got a equality constraint with two variables.
              if (sgn(c_i_j) >= 0) {
                s << Variable(j - 1);
                s << " - ";
                s << Variable(i - 1);
                s << " = " << c_i_j;
              }
              else {
                s << Variable(i - 1);
                s << " - ";
                s << Variable(j - 1);
                s << " = " << c_j_i;
              }
            }
          }
          else {
            // We will print a non-strict inequality.
            if (!is_plus_infinity(c_j_i)) {
              if (first)
                first = false;
              else
                s << ", ";
              if (i == 0) {
                // We have got a constraint with only one variable.
                s << Variable(j - 1);
                neg_assign_r(v, c_j_i, ROUND_DOWN);
                s << " >= " << v;
              }
              else {
                // We have got a constraint with two variables.
                if (sgn(c_j_i) >= 0) {
                  s << Variable(i - 1);
                  s << " - ";
                  s << Variable(j - 1);
                  s << " <= " << c_j_i;
                }
                else {
                  s << Variable(j - 1);
                  s << " - ";
                  s << Variable(i - 1);
                  neg_assign_r(v, c_j_i, ROUND_DOWN);
                  s << " >= " << v;
                }
              }
            }
            if (!is_plus_infinity(c_i_j)) {
              if (first)
                first = false;
              else
                s << ", ";
              if (i == 0) {
                // We have got a constraint with only one variable.
                s << Variable(j - 1);
                s << " <= " << c_i_j;
              }
              else {
                // We have got a constraint with two variables.
                if (sgn(c_i_j) >= 0) {
                  s << Variable(j - 1);
                  s << " - ";
                  s << Variable(i - 1);
                  s << " <= " << c_i_j;
                }
                else {
                  s << Variable(i - 1);
                  s << " - ";
                  s << Variable(j - 1);
                  neg_assign_r(v, c_i_j, ROUND_DOWN);
                  s << " >= " << v;
                }
              }
            }
          }
        }
    }
  }
  return s;
}

template <typename T>
void
BD_Shape<T>::ascii_dump(std::ostream& s) const {
  status.ascii_dump(s);
  s << "\n";
  dbm.ascii_dump(s);
  s << "\n";
  redundancy_dbm.ascii_dump(s);
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS(T, BD_Shape<T>)

template <typename T>
bool
BD_Shape<T>::ascii_load(std::istream& s) {
  if (!status.ascii_load(s))
    return false;
  if (!dbm.ascii_load(s))
    return false;
  if (!redundancy_dbm.ascii_load(s))
    return false;
  return true;
}

template <typename T>
memory_size_type
BD_Shape<T>::external_memory_in_bytes() const {
  return dbm.external_memory_in_bytes()
    + redundancy_dbm.external_memory_in_bytes();
}

template <typename T>
bool
BD_Shape<T>::OK() const {
  // Check whether the difference-bound matrix is well-formed.
  if (!dbm.OK())
    return false;

  // Check whether the status information is legal.
  if (!status.OK())
    return false;

  // An empty BDS is OK.
  if (marked_empty())
    return true;

  // MINUS_INFINITY cannot occur at all.
  for (dimension_type i = dbm.num_rows(); i-- > 0; )
    for (dimension_type j = dbm.num_rows(); j-- > 0; )
      if (is_minus_infinity(dbm[i][j])) {
#ifndef NDEBUG
        using namespace Parma_Polyhedra_Library::IO_Operators;
        std::cerr << "BD_Shape::dbm[" << i << "][" << j << "] = "
                  << dbm[i][j] << "!"
                  << std::endl;
#endif
        return false;
      }

  // On the main diagonal only PLUS_INFINITY can occur.
  for (dimension_type i = dbm.num_rows(); i-- > 0; )
    if (!is_plus_infinity(dbm[i][i])) {
#ifndef NDEBUG
      using namespace Parma_Polyhedra_Library::IO_Operators;
      std::cerr << "BD_Shape::dbm[" << i << "][" << i << "] = "
                << dbm[i][i] << "!  (+inf was expected.)"
                << std::endl;
#endif
      return false;
    }

  // Check whether the shortest-path closure information is legal.
  if (marked_shortest_path_closed()) {
    BD_Shape x = *this;
    x.reset_shortest_path_closed();
    x.shortest_path_closure_assign();
    if (x.dbm != dbm) {
#ifndef NDEBUG
      std::cerr << "BD_Shape is marked as closed but it is not!"
                << std::endl;
#endif
      return false;
    }
  }

  // The following tests might result in false alarms when using floating
  // point coefficients: they are only meaningful if the coefficient type
  // base is exact (since otherwise shortest-path closure is approximated).
  if (std::numeric_limits<coefficient_type_base>::is_exact) {

    // Check whether the shortest-path reduction information is legal.
    if (marked_shortest_path_reduced()) {
      // A non-redundant constraint cannot be equal to PLUS_INFINITY.
      for (dimension_type i = dbm.num_rows(); i-- > 0; )
        for (dimension_type j = dbm.num_rows(); j-- > 0; )
          if (!redundancy_dbm[i][j] && is_plus_infinity(dbm[i][j])) {
#ifndef NDEBUG
            using namespace Parma_Polyhedra_Library::IO_Operators;
            std::cerr << "BD_Shape::dbm[" << i << "][" << j << "] = "
                      << dbm[i][j] << " is marked as non-redundant!"
                      << std::endl;
#endif
            return false;
          }

      BD_Shape x = *this;
      x.reset_shortest_path_reduced();
      x.shortest_path_reduction_assign();
      if (x.redundancy_dbm != redundancy_dbm) {
#ifndef NDEBUG
        std::cerr << "BD_Shape is marked as reduced but it is not!"
                  << std::endl;
#endif
        return false;
      }
    }
  }

  // All checks passed.
  return true;
}

template <typename T>
void
BD_Shape<T>::throw_dimension_incompatible(const char* method,
                                          const BD_Shape& y) const {
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", y->space_dimension() == " << y.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
BD_Shape<T>::throw_dimension_incompatible(const char* method,
                                          dimension_type required_dim) const {
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", required dimension == " << required_dim << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
BD_Shape<T>::throw_dimension_incompatible(const char* method,
                                          const Constraint& c) const {
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", c->space_dimension == " << c.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
BD_Shape<T>::throw_dimension_incompatible(const char* method,
                                          const Congruence& cg) const {
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", cg->space_dimension == " << cg.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
BD_Shape<T>::throw_dimension_incompatible(const char* method,
                                          const Generator& g) const {
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", g->space_dimension == " << g.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
BD_Shape<T>::throw_expression_too_complex(const char* method,
                                          const Linear_Expression& le) {
  using namespace IO_Operators;
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << le << " is too complex.";
  throw std::invalid_argument(s.str());
}


template <typename T>
void
BD_Shape<T>::throw_dimension_incompatible(const char* method,
                                          const char* le_name,
                                          const Linear_Expression& le) const {
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", " << le_name << "->space_dimension() == "
    << le.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
template<typename Interval_Info>
void
BD_Shape<T>::throw_dimension_incompatible(const char* method,
                                          const char* lf_name,
                                          const Linear_Form< Interval<T,
                                          Interval_Info> >& lf) const {
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", " << lf_name << "->space_dimension() == "
    << lf.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
BD_Shape<T>::throw_invalid_argument(const char* method, const char* reason) {
  std::ostringstream s;
  s << "PPL::BD_Shape::" << method << ":" << std::endl
    << reason << ".";
  throw std::invalid_argument(s.str());
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_BD_Shape_templates_hh)
