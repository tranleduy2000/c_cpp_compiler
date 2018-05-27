/* Grid class implementation (non-inline public functions).
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

#include "ppl-config.h"
#include "Grid_defs.hh"
#include "Topology_types.hh"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include "Polyhedron_defs.hh"
#include "assert.hh"
#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

// TODO: In the Grid constructors adapt and use the given system if it
//       is modifiable, instead of using a copy.

PPL::Grid::Grid(const Grid& y, Complexity_Class)
  : con_sys(),
    gen_sys(),
    status(y.status),
    space_dim(y.space_dim),
    dim_kinds(y.dim_kinds) {
  if (space_dim == 0) {
    con_sys = y.con_sys;
    gen_sys = y.gen_sys;
  }
  else {
    if (y.congruences_are_up_to_date())
      con_sys = y.con_sys;
    else
      con_sys.set_space_dimension(space_dim);
    if (y.generators_are_up_to_date())
      gen_sys = y.gen_sys;
    else
      gen_sys = Grid_Generator_System(y.space_dim);
  }
}

PPL::Grid::Grid(const Constraint_System& cs)
  : con_sys(check_space_dimension_overflow(cs.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(cs)",
                                           "the space dimension of cs "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(cs.space_dimension()) {
  space_dim = cs.space_dimension();

  if (space_dim == 0) {
    // See if an inconsistent constraint has been passed.
    for (Constraint_System::const_iterator i = cs.begin(),
           cs_end = cs.end(); i != cs_end; ++i)
      if (i->is_inconsistent()) {
        // Inconsistent constraint found: the grid is empty.
        status.set_empty();
        // Insert the zero dim false congruence system into `con_sys'.
        // `gen_sys' is already in empty form.
        con_sys.insert(Congruence::zero_dim_false());
        PPL_ASSERT(OK());
        return;
      }
    set_zero_dim_univ();
    PPL_ASSERT(OK());
    return;
  }

  Congruence_System cgs(cs.space_dimension());
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i)
    if (i->is_equality())
      cgs.insert(*i);
    else
      throw_invalid_constraints("Grid(cs)", "cs");
  construct(cgs);
}

PPL::Grid::Grid(Constraint_System& cs, Recycle_Input)
  : con_sys(check_space_dimension_overflow(cs.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(cs, recycle)",
                                           "the space dimension of cs "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(cs.space_dimension()) {
  space_dim = cs.space_dimension();

  if (space_dim == 0) {
    // See if an inconsistent constraint has been passed.
    for (Constraint_System::const_iterator i = cs.begin(),
           cs_end = cs.end(); i != cs_end; ++i)
      if (i->is_inconsistent()) {
        // Inconsistent constraint found: the grid is empty.
        status.set_empty();
        // Insert the zero dim false congruence system into `con_sys'.
        // `gen_sys' is already in empty form.
        con_sys.insert(Congruence::zero_dim_false());
        PPL_ASSERT(OK());
        return;
      }
    set_zero_dim_univ();
    PPL_ASSERT(OK());
    return;
  }

  Congruence_System cgs(space_dim);
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i)
    if (i->is_equality())
      cgs.insert(*i);
    else
      throw_invalid_constraint("Grid(cs)", "cs");
  construct(cgs);
}

PPL::Grid::Grid(const Polyhedron& ph,
                Complexity_Class complexity)
  : con_sys(check_space_dimension_overflow(ph.space_dimension(),
                                           max_space_dimension(),
                                           "PPL::Grid::",
                                           "Grid(ph)",
                                           "the space dimension of ph "
                                           "exceeds the maximum allowed "
                                           "space dimension")),
    gen_sys(ph.space_dimension()) {
  space_dim = ph.space_dimension();

  // A zero-dim polyhedron causes no complexity problems.
  if (space_dim == 0) {
    if (ph.is_empty())
      set_empty();
    else
      set_zero_dim_univ();
    return;
  }

  // A polyhedron known to be empty causes no complexity problems.
  if (ph.marked_empty()) {
    set_empty();
    return;
  }

  const bool use_constraints = ph.constraints_are_minimized()
    || !ph.generators_are_up_to_date();

  // Minimize the constraint description if it is needed and
  // the complexity allows it.
  if (use_constraints && complexity == ANY_COMPLEXITY)
    if (!ph.minimize()) {
      set_empty();
      return;
    }

  if (use_constraints) {
    // Only the equality constraints need be used.
    PPL_ASSERT(ph.constraints_are_up_to_date());
    const Constraint_System& cs = ph.constraints();
    Congruence_System cgs(space_dim);
    for (Constraint_System::const_iterator i = cs.begin(),
           cs_end = cs.end(); i != cs_end; ++i)
      if (i->is_equality())
        cgs.insert(*i);
    construct(cgs);
  }
  else {
    // First find a point or closure point and convert it to a
    // grid point and add to the (initially empty) set of grid generators.
    PPL_ASSERT(ph.generators_are_up_to_date());
    const Generator_System& gs = ph.generators();
    Grid_Generator_System ggs(space_dim);
    Linear_Expression point_expr;
    point_expr.set_space_dimension(space_dim);
    PPL_DIRTY_TEMP_COEFFICIENT(point_divisor);
    for (Generator_System::const_iterator g = gs.begin(),
           gs_end = gs.end(); g != gs_end; ++g) {
      if (g->is_point() || g->is_closure_point()) {
        point_expr.linear_combine(g->expr, Coefficient_one(), Coefficient_one(),
                                  1, space_dim + 1);
        point_divisor = g->divisor();
        ggs.insert(grid_point(point_expr, point_divisor));
        break;
      }
    }
    // Add grid lines for all the other generators.
    // If the polyhedron's generator is a (closure) point, the grid line must
    // have the direction given by a line that joins the grid point already
    // inserted and the new point.
    for (Generator_System::const_iterator g = gs.begin(),
           gs_end = gs.end(); g != gs_end; ++g) {
      Linear_Expression e;
      e.set_space_dimension(space_dim);
      if (g->is_point() || g->is_closure_point()) {
        e.linear_combine(point_expr, Coefficient_one(), g->divisor(),
                         1, space_dim + 1);
        e.linear_combine(g->expr, Coefficient_one(), -point_divisor,
                         1, space_dim + 1);
        if (e.all_homogeneous_terms_are_zero())
          continue;
      }
      else {
        e.linear_combine(g->expr, Coefficient_one(), Coefficient_one(),
                         1, space_dim + 1);
      }
      ggs.insert(grid_line(e));
    }
    construct(ggs);
  }
  PPL_ASSERT(OK());
}

PPL::Grid&
PPL::Grid::operator=(const Grid& y) {
  space_dim = y.space_dim;
  dim_kinds = y.dim_kinds;
  if (y.marked_empty())
    set_empty();
  else if (space_dim == 0)
    set_zero_dim_univ();
  else {
    status = y.status;
    if (y.congruences_are_up_to_date())
      con_sys = y.con_sys;
    if (y.generators_are_up_to_date())
      gen_sys = y.gen_sys;
  }
  return *this;
}

PPL::dimension_type
PPL::Grid::affine_dimension() const {
  if (space_dim == 0 || is_empty())
    return 0;

  if (generators_are_up_to_date()) {
    if (generators_are_minimized())
      return gen_sys.num_rows() - 1;
    if (!(congruences_are_up_to_date() && congruences_are_minimized()))
      return minimized_grid_generators().num_rows() - 1;
  }
  else
    minimized_congruences();
  PPL_ASSERT(congruences_are_minimized());
  dimension_type d = space_dim;
  for (dimension_type i = con_sys.num_rows(); i-- > 0; )
    if (con_sys[i].is_equality())
      --d;
  return d;
}

const PPL::Congruence_System&
PPL::Grid::congruences() const {
  if (marked_empty())
    return con_sys;

  if (space_dim == 0) {
    // Zero-dimensional universe.
    PPL_ASSERT(con_sys.num_rows() == 0 && con_sys.space_dimension() == 0);
    return con_sys;
  }

  if (!congruences_are_up_to_date())
    update_congruences();

  return con_sys;
}

const PPL::Congruence_System&
PPL::Grid::minimized_congruences() const {
  if (congruences_are_up_to_date() && !congruences_are_minimized()) {
    // Minimize the congruences.
    Grid& gr = const_cast<Grid&>(*this);
    if (gr.simplify(gr.con_sys, gr.dim_kinds))
      gr.set_empty();
    else
      gr.set_congruences_minimized();
  }
  return congruences();
}

const PPL::Grid_Generator_System&
PPL::Grid::grid_generators() const {
  if (space_dim == 0) {
    PPL_ASSERT(gen_sys.space_dimension() == 0
               && gen_sys.num_rows() == (marked_empty() ? 0U : 1U));
    return gen_sys;
  }

  if (marked_empty()) {
    PPL_ASSERT(gen_sys.has_no_rows());
    return gen_sys;
  }

  if (!generators_are_up_to_date() && !update_generators()) {
    // Updating found the grid empty.
    const_cast<Grid&>(*this).set_empty();
    return gen_sys;
  }

  return gen_sys;
}

const PPL::Grid_Generator_System&
PPL::Grid::minimized_grid_generators() const {
  if (space_dim == 0) {
    PPL_ASSERT(gen_sys.space_dimension() == 0
               && gen_sys.num_rows() == (marked_empty() ? 0U : 1U));
    return gen_sys;
  }

  if (marked_empty()) {
    PPL_ASSERT(gen_sys.has_no_rows());
    return gen_sys;
  }

  if (generators_are_up_to_date()) {
    if (!generators_are_minimized()) {
      // Minimize the generators.
      Grid& gr = const_cast<Grid&>(*this);
      gr.simplify(gr.gen_sys, gr.dim_kinds);
      gr.set_generators_minimized();
    }
  }
  else if (!update_generators()) {
    // Updating found the grid empty.
    const_cast<Grid&>(*this).set_empty();
    return gen_sys;
  }

  return gen_sys;
}

PPL::Poly_Con_Relation
PPL::Grid::relation_with(const Congruence& cg) const {
  // Dimension-compatibility check.
  if (space_dim < cg.space_dimension())
    throw_dimension_incompatible("relation_with(cg)", "cg", cg);

  if (marked_empty())
    return Poly_Con_Relation::saturates()
      && Poly_Con_Relation::is_included()
      && Poly_Con_Relation::is_disjoint();

  if (space_dim == 0) {
    if (cg.is_inconsistent())
      return Poly_Con_Relation::is_disjoint();
    else if (cg.is_equality())
      return Poly_Con_Relation::saturates()
        && Poly_Con_Relation::is_included();
    else if (cg.inhomogeneous_term() % cg.modulus() == 0)
      return Poly_Con_Relation::saturates()
        && Poly_Con_Relation::is_included();
  }

  if (!generators_are_up_to_date() && !update_generators())
    // Updating found the grid empty.
    return Poly_Con_Relation::saturates()
      && Poly_Con_Relation::is_included()
      && Poly_Con_Relation::is_disjoint();

  // Return one of the relations
  // 'strictly_intersects'   a strict subset of the grid points satisfy cg
  // 'is_included'           every grid point satisfies cg
  // 'is_disjoint'           cg and the grid occupy separate spaces.

  // There is always a point.

  // Scalar product of the congruence and the first point that
  // satisfies the congruence.
  PPL_DIRTY_TEMP_COEFFICIENT(point_sp);
  point_sp = 0;

  PPL_DIRTY_TEMP_COEFFICIENT(div);
  div = cg.modulus();

  PPL_DIRTY_TEMP_COEFFICIENT(sp);

  bool known_to_intersect = false;

  for (Grid_Generator_System::const_iterator i = gen_sys.begin(),
         i_end = gen_sys.end(); i != i_end; ++i) {
    const Grid_Generator& g = *i;
    Scalar_Products::assign(sp, cg, g);

    switch (g.type()) {

    case Grid_Generator::POINT:
      if (cg.is_proper_congruence())
        sp %= div;
      if (sp == 0) {
        // The point satisfies the congruence.
        if (point_sp == 0)
          // Any previous points satisfied the congruence.
          known_to_intersect = true;
        else
          return Poly_Con_Relation::strictly_intersects();
      }
      else {
        if (point_sp == 0) {
          if (known_to_intersect)
            return Poly_Con_Relation::strictly_intersects();
          // Assign `sp' to `point_sp' as `sp' is the scalar product
          // of cg and a point g and is non-zero.
          point_sp = sp;
        }
        else {
          // A previously considered point p failed to satisfy cg such that
          // `point_sp' = `scalar_prod(p, cg)'
          // so, if we consider the parameter g-p instead of g, we have
          // scalar_prod(g-p, cg) = scalar_prod(g, cg) - scalar_prod(p, cg)
          //                      = sp - point_sp
          sp -= point_sp;

          if (sp != 0) {
            // Find the GCD between sp and the previous GCD.
            gcd_assign(div, div, sp);
            if (point_sp % div == 0)
              // There is a point in the grid satisfying cg.
              return Poly_Con_Relation::strictly_intersects();
          }
        }
      }
      break;

    case Grid_Generator::PARAMETER:
      if (cg.is_proper_congruence())
        sp %= (div * g.divisor());
      if (sp == 0)
        // Parameter g satisfies the cg so the relation depends
        // entirely on the other generators.
        break;
      if (known_to_intersect)
        // At least one point satisfies cg.  However, the sum of such
        // a point and the parameter g fails to satisfy cg (due to g).
        return Poly_Con_Relation::strictly_intersects();
      // Find the GCD between sp and the previous GCD.
      gcd_assign(div, div, sp);
      if (point_sp != 0) {
        // At least one of any previously encountered points fails to
        // satisfy cg.
        if (point_sp % div == 0)
          // There is also a grid point that satisfies cg.
          return Poly_Con_Relation::strictly_intersects();
      }
      break;

    case Grid_Generator::LINE:
      if (sp == 0)
        // Line g satisfies the cg so the relation depends entirely on
        // the other generators.
        break;

      // Line g intersects the congruence.
      //
      // There is a point p in the grid.  Suppose <p*cg> = p_sp.  Then
      // (-p_sp/sp)*g + p is a point that satisfies cg: <((-p_sp/sp)*g
      // + p).cg> = -(p_sp/sp)*sp + p_sp) = 0.  If p does not satisfy
      // `cg' and hence is not in the grid defined by `cg', the grid
      // `*this' strictly intersects the `cg' grid.  On the other
      // hand, if `p' is in the grid defined by `cg' so that p_sp = 0,
      // then <p+g.cg> = p_sp + sp != 0; thus `p+g' is a point in
      // *this that does not satisfy `cg' and hence `p+g' is a point
      // in *this not in the grid defined by `cg'; therefore `*this'
      // strictly intersects the `cg' grid.
      return Poly_Con_Relation::strictly_intersects();
    }
  }

  if (point_sp == 0) {
    if (cg.is_equality())
      // Every generator satisfied the cg.
      return Poly_Con_Relation::is_included()
        && Poly_Con_Relation::saturates();
    else
      // Every generator satisfied the cg.
      return Poly_Con_Relation::is_included();
  }

  PPL_ASSERT(!known_to_intersect);
  return Poly_Con_Relation::is_disjoint();
}

PPL::Poly_Gen_Relation
PPL::Grid::relation_with(const Grid_Generator& g) const {
  // Dimension-compatibility check.
  if (space_dim < g.space_dimension())
    throw_dimension_incompatible("relation_with(g)", "g", g);

  // The empty grid cannot subsume a generator.
  if (marked_empty())
    return Poly_Gen_Relation::nothing();

  // A universe grid in a zero-dimensional space subsumes all the
  // generators of a zero-dimensional space.
  if (space_dim == 0)
    return Poly_Gen_Relation::subsumes();

  if (!congruences_are_up_to_date())
    update_congruences();

  return
    con_sys.satisfies_all_congruences(g)
    ? Poly_Gen_Relation::subsumes()
    : Poly_Gen_Relation::nothing();
}

PPL::Poly_Gen_Relation
PPL::Grid::relation_with(const Generator& g) const {
  const dimension_type g_space_dim = g.space_dimension();

  // Dimension-compatibility check.
  if (space_dim < g_space_dim)
    throw_dimension_incompatible("relation_with(g)", "g", g);

  // The empty grid cannot subsume a generator.
  if (marked_empty())
    return Poly_Gen_Relation::nothing();

  // A universe grid in a zero-dimensional space subsumes all the
  // generators of a zero-dimensional space.
  if (space_dim == 0)
    return Poly_Gen_Relation::subsumes();

  if (!congruences_are_up_to_date())
    update_congruences();

  const Linear_Expression expr(g.expression());
  Grid_Generator gg(grid_point());
  if (g.is_point() || g.is_closure_point())
    // Points and closure points are converted to grid points.
    gg = grid_point(expr, g.divisor());
  else
    // The generator is a ray or line.
    // In both cases, we convert it to a grid line
    gg = grid_line(expr);

  return
    con_sys.satisfies_all_congruences(gg)
    ? Poly_Gen_Relation::subsumes()
    : Poly_Gen_Relation::nothing();
}

PPL::Poly_Con_Relation
PPL::Grid::relation_with(const Constraint& c) const {
  // Dimension-compatibility check.
  if (space_dim < c.space_dimension())
    throw_dimension_incompatible("relation_with(c)", "c", c);

  if (c.is_equality()) {
    const Congruence cg(c);
    return relation_with(cg);
  }

  if (marked_empty())
    return Poly_Con_Relation::saturates()
      &&  Poly_Con_Relation::is_included()
      && Poly_Con_Relation::is_disjoint();

  if (space_dim == 0) {
    if (c.is_inconsistent())
      if (c.is_strict_inequality() && c.inhomogeneous_term() == 0)
        // The constraint 0 > 0 implicitly defines the hyperplane 0 = 0;
        // thus, the zero-dimensional point also saturates it.
        return Poly_Con_Relation::saturates()
          && Poly_Con_Relation::is_disjoint();
      else
        return Poly_Con_Relation::is_disjoint();
    else if (c.inhomogeneous_term() == 0)
      return Poly_Con_Relation::saturates()
        && Poly_Con_Relation::is_included();
    else
      // The zero-dimensional point saturates
      // neither the positivity constraint 1 >= 0,
      // nor the strict positivity constraint 1 > 0.
      return Poly_Con_Relation::is_included();
  }

  if (!generators_are_up_to_date() && !update_generators())
    // Updating found the grid empty.
    return Poly_Con_Relation::saturates()
      && Poly_Con_Relation::is_included()
      && Poly_Con_Relation::is_disjoint();

  // Return one of the relations
  // 'strictly_intersects'   a strict subset of the grid points satisfy c
  // 'is_included'           every grid point satisfies c
  // 'is_disjoint'           c and the grid occupy separate spaces.

  // There is always a point.

  bool point_is_included = false;
  bool point_saturates = false;
  const Grid_Generator* first_point = 0;

  for (Grid_Generator_System::const_iterator i = gen_sys.begin(),
         i_end = gen_sys.end(); i != i_end; ++i) {
    const Grid_Generator& g = *i;
    switch (g.type()) {
    case Grid_Generator::POINT:
      {
        if (first_point == 0) {
          first_point = &g;
          const int sign = Scalar_Products::sign(c, g);
          if (sign == 0)
            point_saturates = !c.is_strict_inequality();
          else if (sign > 0)
            point_is_included = !c.is_equality();
          break;
        }
        // Not the first point: convert `g' to be a parameter
        // and fall through into the parameter case.
        Grid_Generator& gen = const_cast<Grid_Generator&>(g);
        const Grid_Generator& point = *first_point;
        const Coefficient& p_div = point.divisor();
        const Coefficient& g_div = gen.divisor();
        gen.expr.linear_combine(point.expr, p_div, -g_div,
                                1, gen.expr.space_dimension());
        gen.expr.set_inhomogeneous_term(g_div * p_div);
        gen.strong_normalize();
        gen.set_is_parameter();
        PPL_ASSERT(gen.OK());
      }
      // Intentionally fall through.

    case Grid_Generator::PARAMETER:
    case Grid_Generator::LINE:
      {
        const int sign = c.is_strict_inequality()
          ? Scalar_Products::reduced_sign(c.expr, g.expr)
          : Scalar_Products::sign(c.expr, g.expr);
        if (sign != 0)
          return Poly_Con_Relation::strictly_intersects();
      }
      break;
    } // switch
  }

  // If this program point is reached, then all lines and parameters
  // saturate the constraint. Hence, the result is determined by
  // the previosly computed relation with the point.
  if (point_saturates)
    return Poly_Con_Relation::saturates()
      && Poly_Con_Relation::is_included();

  if (point_is_included)
    return Poly_Con_Relation::is_included();

  return Poly_Con_Relation::is_disjoint();
}

bool
PPL::Grid::is_empty() const {
  if (marked_empty())
    return true;
  // Try a fast-fail test: if generators are up-to-date then the
  // generator system (since it is well formed) contains a point.
  if (generators_are_up_to_date())
    return false;
  if (space_dim == 0)
    return false;
  if (congruences_are_minimized())
    // If the grid was empty it would be marked empty.
    return false;
  // Minimize the congruences to check if the grid is empty.
  Grid& gr = const_cast<Grid&>(*this);
  if (gr.simplify(gr.con_sys, gr.dim_kinds)) {
    gr.set_empty();
    return true;
  }
  gr.set_congruences_minimized();
  return false;
}

bool
PPL::Grid::is_universe() const {
  if (marked_empty())
    return false;

  if (space_dim == 0)
    return true;

  if (congruences_are_up_to_date()) {
    if (congruences_are_minimized())
      // The minimized universe congruence system has only one row,
      // the integrality congruence.
      return con_sys.num_rows() == 1 && con_sys[0].is_tautological();
  }
  else {
    update_congruences();
    return con_sys.num_rows() == 1 && con_sys[0].is_tautological();
  }

  // Test con_sys's inclusion in a universe generator system.

  // The zero dimension cases are handled above.
  for (dimension_type i = space_dim; i-- > 0; ) {
    Linear_Expression expr;
    expr.set_space_dimension(space_dim);
    expr += Variable(i);
    if (!con_sys.satisfies_all_congruences(grid_line(expr)))
      return false;
  }
#ifndef NDEBUG
  Linear_Expression expr;
  expr.set_space_dimension(space_dim);
  PPL_ASSERT(con_sys.satisfies_all_congruences(grid_point(expr)));
#endif
  return true;
}

bool
PPL::Grid::is_bounded() const {
  // A zero-dimensional or empty grid is bounded.
  if (space_dim == 0
      || marked_empty()
      || (!generators_are_up_to_date() && !update_generators()))
    return true;

  // TODO: Consider using con_sys when gen_sys is out of date.

  if (gen_sys.num_rows() > 1) {
    // Check if all generators are the same point.
    const Grid_Generator& first_point = gen_sys[0];
    if (first_point.is_line_or_parameter())
      return false;
    for (dimension_type row = gen_sys.num_rows(); row-- > 0; ) {
      const Grid_Generator& gen = gen_sys[row];
      if (gen.is_line_or_parameter() || gen != first_point)
        return false;
    }
  }
  return true;
}

bool
PPL::Grid::is_discrete() const {
  // A zero-dimensional or empty grid is discrete.
  if (space_dim == 0
      || marked_empty()
      || (!generators_are_up_to_date() && !update_generators()))
    return true;

  // Search for lines in the generator system.
  for (dimension_type row = gen_sys.num_rows(); row-- > 1; )
    if (gen_sys[row].is_line())
      return false;

  // The system of generators is composed only by
  // points and parameters: the grid is discrete.
  return true;
}

bool
PPL::Grid::is_topologically_closed() const {
  return true;
}

bool
PPL::Grid::contains_integer_point() const {
  // Empty grids have no points.
  if (marked_empty())
    return false;

  // A zero-dimensional, universe grid has, by convention, an
  // integer point.
  if (space_dim == 0)
    return true;

  // A grid has an integer point if its intersection with the integer
  // grid is non-empty.
  Congruence_System cgs;
  for (dimension_type var_index = space_dim; var_index-- > 0; )
    cgs.insert(Variable(var_index) %= 0);

  Grid gr = *this;
  gr.add_recycled_congruences(cgs);
  return !gr.is_empty();
}

bool
PPL::Grid::constrains(const Variable var) const {
  // `var' should be one of the dimensions of the grid.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("constrains(v)", "v", var);

  // An empty grid constrains all variables.
  if (marked_empty())
    return true;

  if (generators_are_up_to_date()) {
    // Since generators are up-to-date, the generator system (since it is
    // well formed) contains a point.  Hence the grid is not empty.
    if (congruences_are_up_to_date())
      // Here a variable is constrained if and only if it is
      // syntactically constrained.
      goto syntactic_check;

    if (generators_are_minimized()) {
      // Try a quick, incomplete check for the universe grid:
      // a universe grid constrains no variable.
      // Count the number of lines (they are linearly independent).
      dimension_type num_lines = 0;
      for (dimension_type i = gen_sys.num_rows(); i-- > 0; )
        if (gen_sys[i].is_line())
          ++num_lines;

      if (num_lines == space_dim)
        return false;
    }

    // Scan generators: perhaps we will find line(var).
    for (dimension_type i = gen_sys.num_rows(); i-- > 0; ) {
      const Grid_Generator& g_i = gen_sys[i];
      if (!g_i.is_line())
        continue;
      if (sgn(g_i.coefficient(var)) != 0) {
        if (g_i.expression().all_zeroes(1, var.space_dimension())
            && g_i.expression().all_zeroes(var.space_dimension() + 1, space_dim + 1))
          // The only nonzero coefficient in g_i is the one of var.
          return true;
      }
    }

    // We are still here: at least we know that the grid is not empty.
    update_congruences();
    goto syntactic_check;
  }

  // We must minimize to detect emptiness and obtain constraints.
  if (!minimize())
    return true;

 syntactic_check:
  for (dimension_type i = con_sys.num_rows(); i-- > 0; )
    if (con_sys[i].coefficient(var) != 0)
      return true;
  return false;
}

bool
PPL::Grid::OK(bool check_not_empty) const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  // Check whether the status information is legal.
  if (!status.OK())
    goto fail;

  if (marked_empty()) {
    if (check_not_empty) {
      // The caller does not want the grid to be empty.
#ifndef NDEBUG
      cerr << "Empty grid!" << endl;
#endif
      goto fail;
    }

    if (con_sys.space_dimension() != space_dim) {
#ifndef NDEBUG
      cerr << "The grid is in a space of dimension " << space_dim
           << " while the system of congruences is in a space of dimension "
           << con_sys.space_dimension()
           << endl;
#endif
      goto fail;
    }
    return true;
  }

  // A zero-dimensional universe grid is legal only if the system of
  // congruences `con_sys' is empty, and the generator system contains
  // one point.
  if (space_dim == 0) {
    if (con_sys.has_no_rows())
      if (gen_sys.num_rows() == 1 && gen_sys[0].is_point())
        return true;
#ifndef NDEBUG
    cerr << "Zero-dimensional grid should have an empty congruence" << endl
         << "system and a generator system of a single point." << endl;
#endif
    goto fail;
  }

  // A grid is defined by a system of congruences or a system of
  // generators.  At least one of them must be up to date.
  if (!congruences_are_up_to_date() && !generators_are_up_to_date()) {
#ifndef NDEBUG
    cerr << "Grid not empty, not zero-dimensional" << endl
         << "and with neither congruences nor generators up-to-date!"
         << endl;
#endif
    goto fail;
  }

  {
    // The expected number of columns in the congruence and generator
    // systems, if they are not empty.
    const dimension_type num_columns = space_dim + 1;

    // Here we check if the size of the matrices is consistent.
    // Let us suppose that all the matrices are up-to-date; this means:
    // `con_sys' : number of congruences x poly_num_columns
    // `gen_sys' : number of generators  x poly_num_columns
    if (congruences_are_up_to_date())
      if (con_sys.space_dimension() != space_dim) {
#ifndef NDEBUG
        cerr << "Incompatible size! (con_sys and space_dim)"
             << endl;
#endif
        goto fail;
      }

    if (generators_are_up_to_date()) {
      if (gen_sys.space_dimension() != space_dim) {
#ifndef NDEBUG
        cerr << "Incompatible size! (gen_sys and space_dim)"
             << endl;
#endif
        goto fail;
      }

      // A non-empty system of generators describing a grid is valid
      // if and only if it contains a point.
      if (!gen_sys.has_no_rows() && !gen_sys.has_points()) {
#ifndef NDEBUG
        cerr << "Non-empty generator system declared up-to-date "
             << "has no points!"
             << endl;
#endif
        goto fail;
      }

      if (generators_are_minimized()) {
        Grid_Generator_System gs = gen_sys;

        if (dim_kinds.size() != num_columns) {
#ifndef NDEBUG
          cerr << "Size of dim_kinds should equal the number of columns."
               << endl;
#endif
          goto fail;
        }

        if (!upper_triangular(gs, dim_kinds)) {
#ifndef NDEBUG
          cerr << "Reduced generators should be upper triangular."
               << endl;
#endif
          goto fail;
        }

        // Check that dim_kinds corresponds to the row kinds in gen_sys.
        for (dimension_type dim = space_dim,
               row = gen_sys.num_rows(); dim > 0; --dim) {
          if (dim_kinds[dim] == GEN_VIRTUAL)
            goto ok;
          if (gen_sys[--row].is_parameter_or_point()
              && dim_kinds[dim] == PARAMETER)
            goto ok;
          PPL_ASSERT(gen_sys[row].is_line());
          if (dim_kinds[dim] == LINE)
            goto ok;
#ifndef NDEBUG
          cerr << "Kinds in dim_kinds should match those in gen_sys."
               << endl;
#endif
          goto fail;
        ok:
          PPL_ASSERT(row <= dim);
        }

        // A reduced generator system must be the same as a temporary
        // reduced copy.
        Dimension_Kinds dim_kinds_copy = dim_kinds;
        // `gs' is minimized and marked_empty returned false, so `gs'
        // should contain rows.
        PPL_ASSERT(!gs.has_no_rows());
        simplify(gs, dim_kinds_copy);
        // gs contained rows before being reduced, so it should
        // contain at least a single point afterward.
        PPL_ASSERT(!gs.has_no_rows());
        for (dimension_type row = gen_sys.num_rows(); row-- > 0; ) {
          const Grid_Generator& g = gs[row];
          const Grid_Generator& g_copy = gen_sys[row];
          if (g.is_equal_to(g_copy))
            continue;
#ifndef NDEBUG
          cerr << "Generators are declared minimized,"
            " but they change under reduction.\n"
               << "Here is the generator system:\n";
          gen_sys.ascii_dump(cerr);
          cerr << "and here is the minimized form of the temporary copy:\n";
          gs.ascii_dump(cerr);
#endif
          goto fail;
        }
      }

    } // if (congruences_are_up_to_date())
  }

  if (congruences_are_up_to_date()) {
    // Check if the system of congruences is well-formed.
    if (!con_sys.OK())
      goto fail;

    Grid tmp_gr = *this;
    // Make a copy here, before changing tmp_gr, to check later.
    const Congruence_System cs_copy = tmp_gr.con_sys;

    // Clear the generators in tmp_gr.
    Grid_Generator_System gs(space_dim);
    tmp_gr.gen_sys.m_swap(gs);
    tmp_gr.clear_generators_up_to_date();

    if (!tmp_gr.update_generators()) {
      if (check_not_empty) {
        // Want to know the satisfiability of the congruences.
#ifndef NDEBUG
        cerr << "Unsatisfiable system of congruences!"
             << endl;
#endif
        goto fail;
      }
      // The grid is empty, all checks are done.
      return true;
    }

    if (congruences_are_minimized()) {
      // A reduced congruence system must be lower triangular.
      if (!lower_triangular(con_sys, dim_kinds)) {
#ifndef NDEBUG
        cerr << "Reduced congruences should be lower triangular." << endl;
#endif
        goto fail;
      }

      // If the congruences are minimized, all the elements in the
      // congruence system must be the same as those in the temporary,
      // minimized system `cs_copy'.
      if (!con_sys.is_equal_to(cs_copy)) {
#ifndef NDEBUG
        cerr << "Congruences are declared minimized, but they change under reduction!"
             << endl
             << "Here is the minimized form of the congruence system:"
             << endl;
        cs_copy.ascii_dump(cerr);
        cerr << endl;
#endif
        goto fail;
      }

      if (dim_kinds.size() != con_sys.space_dimension() + 1 /* inhomogeneous term */) {
#ifndef NDEBUG
        cerr << "Size of dim_kinds should equal the number of columns."
             << endl;
#endif
        goto fail;
      }

      // Check that dim_kinds corresponds to the row kinds in con_sys.
      for (dimension_type dim = space_dim, row = 0; dim > 0; --dim) {
        if (dim_kinds[dim] == CON_VIRTUAL)
            continue;
        if (con_sys[row++].is_proper_congruence()
            && dim_kinds[dim] == PROPER_CONGRUENCE)
          continue;
        PPL_ASSERT(con_sys[row-1].is_equality());
        if (dim_kinds[dim] == EQUALITY)
          continue;
#ifndef NDEBUG
        cerr << "Kinds in dim_kinds should match those in con_sys." << endl;
#endif
        goto fail;
      }
    }
  }

  return true;

 fail:
#ifndef NDEBUG
  cerr << "Here is the grid under check:" << endl;
  ascii_dump(cerr);
#endif
  return false;
}

void
PPL::Grid::add_constraints(const Constraint_System& cs) {
  // The dimension of `cs' must be at most `space_dim'.
  if (space_dim < cs.space_dimension())
    throw_dimension_incompatible("add_constraints(cs)", "cs", cs);
  if (marked_empty())
    return;

  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i) {
    add_constraint_no_check(*i);
    if (marked_empty())
      return;
  }
}

void
PPL::Grid::add_grid_generator(const Grid_Generator& g) {
  // The dimension of `g' must be at most space_dim.
  const dimension_type g_space_dim = g.space_dimension();
  if (space_dim < g_space_dim)
    throw_dimension_incompatible("add_grid_generator(g)", "g", g);

  // Deal with zero-dimension case first.
  if (space_dim == 0) {
    // Points and parameters are the only zero-dimension generators
    // that can be created.
    if (marked_empty()) {
      if (g.is_parameter())
        throw_invalid_generator("add_grid_generator(g)", "g");
      set_zero_dim_univ();
    }
    PPL_ASSERT(OK());
    return;
  }

  if (marked_empty()
      || (!generators_are_up_to_date() && !update_generators())) {
    // Here the grid is empty: the specification says we can only
    // insert a point.
    if (g.is_line_or_parameter())
      throw_invalid_generator("add_grid_generator(g)", "g");
    gen_sys.insert(g);
    clear_empty();
  }
  else {
    PPL_ASSERT(generators_are_up_to_date());
    gen_sys.insert(g);
    if (g.is_parameter_or_point())
      normalize_divisors(gen_sys);
  }

  // With the added generator, congruences are out of date.
  clear_congruences_up_to_date();

  clear_generators_minimized();
  set_generators_up_to_date();
  PPL_ASSERT(OK());
}

void
PPL::Grid::add_recycled_congruences(Congruence_System& cgs) {
  // Dimension-compatibility check.
  const dimension_type cgs_space_dim = cgs.space_dimension();
  if (space_dim < cgs_space_dim)
    throw_dimension_incompatible("add_recycled_congruences(cgs)", "cgs", cgs);

  if (cgs.has_no_rows())
    return;

  if (marked_empty())
    return;

  if (space_dim == 0) {
    // In a 0-dimensional space the congruences are trivial (e.g., 0
    // == 0 or 1 %= 0) or false (e.g., 1 == 0).  In a system of
    // congruences `begin()' and `end()' are equal if and only if the
    // system contains only trivial congruences.
    if (cgs.begin() != cgs.end())
      // There is a congruence, it must be false, the grid becomes empty.
      set_empty();
    return;
  }

  // The congruences are required.
  if (!congruences_are_up_to_date())
    update_congruences();

  // Swap (instead of copying) the coefficients of `cgs' (which is
  // writable).
  con_sys.insert(cgs, Recycle_Input());

  // Congruences may not be minimized and generators are out of date.
  clear_congruences_minimized();
  clear_generators_up_to_date();
  // Note: the congruence system may have become unsatisfiable, thus
  // we do not check for satisfiability.
  PPL_ASSERT(OK());
}

void
PPL::Grid::add_recycled_grid_generators(Grid_Generator_System& gs) {
  // Dimension-compatibility check.
  const dimension_type gs_space_dim = gs.space_dimension();
  if (space_dim < gs_space_dim)
    throw_dimension_incompatible("add_recycled_grid_generators(gs)", "gs", gs);

  // Adding no generators leaves the grid the same.
  if (gs.has_no_rows())
    return;

  // Adding valid generators to a zero-dimensional grid transforms it
  // to the zero-dimensional universe grid.
  if (space_dim == 0) {
    if (marked_empty())
      set_zero_dim_univ();
    else {
      PPL_ASSERT(gs.has_points());
    }
    PPL_ASSERT(OK(true));
    return;
  }

  if (!marked_empty()) {
    // The grid contains at least one point.

    if (!generators_are_up_to_date())
      update_generators();
    normalize_divisors(gs, gen_sys);

    gen_sys.insert(gs, Recycle_Input());

    // Congruences are out of date and generators are not minimized.
    clear_congruences_up_to_date();
    clear_generators_minimized();

    PPL_ASSERT(OK(true));
    return;
  }

  // The grid is empty.

  // `gs' must contain at least one point.
  if (!gs.has_points())
    throw_invalid_generators("add_recycled_grid_generators(gs)", "gs");

  // Adjust `gs' to the right dimension.
  gs.set_space_dimension(space_dim);

  gen_sys.m_swap(gs);

  normalize_divisors(gen_sys);

  // The grid is no longer empty and generators are up-to-date.
  set_generators_up_to_date();
  clear_empty();

  PPL_ASSERT(OK());
}

void
PPL::Grid::add_grid_generators(const Grid_Generator_System& gs) {
  // TODO: this is just an executable specification.
  Grid_Generator_System gs_copy = gs;
  add_recycled_grid_generators(gs_copy);
}

void
PPL::Grid::refine_with_constraint(const Constraint& c) {
  // The dimension of `c' must be at most `space_dim'.
  if (space_dim < c.space_dimension())
    throw_dimension_incompatible("refine_with_constraint(c)", "c", c);
  if (marked_empty())
    return;
  refine_no_check(c);
}

void
PPL::Grid::refine_with_constraints(const Constraint_System& cs) {
  // The dimension of `cs' must be at most `space_dim'.
  if (space_dim < cs.space_dimension())
    throw_dimension_incompatible("refine_with_constraints(cs)", "cs", cs);

  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); !marked_empty() && i != cs_end; ++i)
    refine_no_check(*i);
}

void
PPL::Grid::unconstrain(const Variable var) {
  // Dimension-compatibility check.
  if (space_dim < var.space_dimension())
    throw_dimension_incompatible("unconstrain(var)", var.space_dimension());

  // Do something only if the grid is non-empty.
  if (marked_empty()
      || (!generators_are_up_to_date() && !update_generators()))
    // Empty: do nothing.
    return;

  PPL_ASSERT(generators_are_up_to_date());
  Grid_Generator l = grid_line(var);
  gen_sys.insert(l, Recycle_Input());
  // With the added generator, congruences are out of date.
  clear_congruences_up_to_date();
  clear_generators_minimized();
  PPL_ASSERT(OK());
}

void
PPL::Grid::unconstrain(const Variables_Set& vars) {
  // The cylindrification with respect to no dimensions is a no-op.
  // This case also captures the only legal cylindrification
  // of a grid in a 0-dim space.
  if (vars.empty())
    return;

  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dim < min_space_dim)
    throw_dimension_incompatible("unconstrain(vs)", min_space_dim);

  // Do something only if the grid is non-empty.
  if (marked_empty()
      || (!generators_are_up_to_date() && !update_generators()))
    // Empty: do nothing.
    return;

  PPL_ASSERT(generators_are_up_to_date());
  // Since `gen_sys' is not empty, the space dimension of the inserted
  // generators are automatically adjusted.
  for (Variables_Set::const_iterator vsi = vars.begin(),
         vsi_end = vars.end(); vsi != vsi_end; ++vsi) {
    Grid_Generator l = grid_line(Variable(*vsi));
    gen_sys.insert(l, Recycle_Input());
  }
  // Constraints are no longer up-to-date.
  clear_generators_minimized();
  clear_congruences_up_to_date();
  PPL_ASSERT(OK());
}

void
PPL::Grid::intersection_assign(const Grid& y) {
  Grid& x = *this;
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("intersection_assign(y)", "y", y);

  // If one of the two grids is empty, the intersection is empty.
  if (x.marked_empty())
    return;
  if (y.marked_empty()) {
    x.set_empty();
    return;
  }

  // If both grids are zero-dimensional, then at this point they are
  // necessarily universe, so the intersection is also universe.
  if (x.space_dim == 0)
    return;

  // The congruences must be up-to-date.
  if (!x.congruences_are_up_to_date())
    x.update_congruences();
  if (!y.congruences_are_up_to_date())
    y.update_congruences();

  if (!y.con_sys.has_no_rows()) {
    x.con_sys.insert(y.con_sys);
    // Grid_Generators may be out of date and congruences may have changed
    // from minimal form.
    x.clear_generators_up_to_date();
    x.clear_congruences_minimized();
  }

  PPL_ASSERT(x.OK() && y.OK());
}

void
PPL::Grid::upper_bound_assign(const Grid& y) {
  Grid& x = *this;
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("upper_bound_assign(y)", "y", y);

  // The join of a grid `gr' with an empty grid is `gr'.
  if (y.marked_empty())
    return;
  if (x.marked_empty()) {
    x = y;
    return;
  }

  // If both grids are zero-dimensional, then they are necessarily
  // universe grids, and so is their join.
  if (x.space_dim == 0)
    return;

  // The generators must be up-to-date.
  if (!x.generators_are_up_to_date() && !x.update_generators()) {
    // Discovered `x' empty when updating generators.
    x = y;
    return;
  }
  if (!y.generators_are_up_to_date() && !y.update_generators())
    // Discovered `y' empty when updating generators.
    return;

  // Match the divisors of the x and y generator systems.
  Grid_Generator_System gs(y.gen_sys);
  normalize_divisors(x.gen_sys, gs);
  x.gen_sys.insert(gs, Recycle_Input());
  // Congruences may be out of date and generators may have lost
  // minimal form.
  x.clear_congruences_up_to_date();
  x.clear_generators_minimized();

  // At this point both `x' and `y' are not empty.
  PPL_ASSERT(x.OK(true) && y.OK(true));
}

bool
PPL::Grid::upper_bound_assign_if_exact(const Grid& y) {
  const Grid& x = *this;

  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("upper_bound_assign_if_exact(y)", "y", y);

  if (x.marked_empty()
      || y.marked_empty()
      || x.space_dim == 0
      || x.is_included_in(y)
      || y.is_included_in(x)) {
    upper_bound_assign(y);
    return true;
  }

  // The above test 'x.is_included_in(y)' will ensure the generators of x
  // are up to date.
  PPL_ASSERT(generators_are_up_to_date());

  Grid x_copy = x;
  x_copy.upper_bound_assign(y);
  x_copy.difference_assign(y);
  if (x_copy.is_included_in(x)) {
    upper_bound_assign(y);
    return true;
  }

  return false;
}

void
PPL::Grid::difference_assign(const Grid& y) {
  Grid& x = *this;
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("difference_assign(y)", "y", y);

  if (y.marked_empty() || x.marked_empty())
    return;

  // If both grids are zero-dimensional, then they are necessarily
  // universe grids, so the result is empty.
  if (x.space_dim == 0) {
    x.set_empty();
    return;
  }

  if (y.contains(x)) {
    x.set_empty();
    return;
  }

  Grid new_grid(x.space_dim, EMPTY);

  const Congruence_System& y_cgs = y.congruences();
  for (Congruence_System::const_iterator i = y_cgs.begin(),
         y_cgs_end = y_cgs.end(); i != y_cgs_end; ++i) {
    const Congruence& cg = *i;

    // The 2-complement cg2 of cg = ((e %= 0) / m) is the congruence
    // defining the sets of points exactly half-way between successive
    // hyperplanes e = km and e = (k+1)m, for any integer k; that is,
    // the hyperplanes defined by 2e = (2k + 1)m, for any integer k.
    // Thus `cg2' is the congruence ((2e %= m) / 2m).

    // As the grid difference must be a grid, only add the
    // 2-complement congruence to x if the resulting grid includes all
    // the points in x that did not satisfy `cg'.

    // The 2-complement of cg can be included in the result only if x
    // holds points other than those in cg.
    if (x.relation_with(cg).implies(Poly_Con_Relation::is_included()))
      continue;

    if (cg.is_proper_congruence()) {
      const Linear_Expression e(cg.expression());
      // Congruence cg is ((e %= 0) / m).
      const Coefficient& m = cg.modulus();
      // If x is included in the grid defined by the congruences cg
      // and its 2-complement (i.e. the grid defined by the congruence
      // (2e %= 0) / m) then add the 2-complement to the potential
      // result.
      if (x.relation_with((2*e %= 0) / m)
          .implies(Poly_Con_Relation::is_included())) {
        Grid z = x;
        z.add_congruence_no_check((2*e %= m) / (2*m));
        new_grid.upper_bound_assign(z);
        continue;
      }
    }
    return;
  }

  *this = new_grid;

  PPL_ASSERT(OK());
}

namespace {

struct Ruled_Out_Pair {
  PPL::dimension_type congruence_index;
  PPL::dimension_type num_ruled_out;
};

struct Ruled_Out_Less_Than {
  bool operator()(const Ruled_Out_Pair& x,
                  const Ruled_Out_Pair& y) const {
    return x.num_ruled_out > y.num_ruled_out;
  }
};

} // namespace

bool
PPL::Grid::simplify_using_context_assign(const Grid& y) {
  Grid& x = *this;
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("simplify_using_context_assign(y)", "y", y);

  // Filter away the zero-dimensional case.
  if (x.space_dim == 0) {
    if (y.is_empty()) {
      set_zero_dim_univ();
      PPL_ASSERT(OK());
      return false;
    }
    else
      return !x.is_empty();
  }

  // If `y' is empty, the biggest enlargement for `x' is the universe.
  if (!y.minimize()) {
    Grid gr(x.space_dim, UNIVERSE);
    m_swap(gr);
    return false;
  }

  // If `x' is empty, the intersection is empty.
  if (!x.minimize()) {
    // Search for a congruence of `y' that is not a tautology.
    PPL_ASSERT(y.congruences_are_up_to_date());
    Grid gr(x.space_dim, UNIVERSE);
    for (dimension_type i = y.con_sys.num_rows(); i-- > 0; ) {
      const Congruence& y_con_sys_i = y.con_sys[i];
      if (!y_con_sys_i.is_tautological()) {
        // Found: we obtain a congruence `c' contradicting the one we
        // found, and assign to `x' the grid `gr' with `c' as
        // the only congruence.
        const Linear_Expression le(y_con_sys_i.expression());
        if (y_con_sys_i.is_equality()) {
          gr.refine_no_check(le == 1);
          break;
        }
        else {
          const Coefficient& y_modulus_i = y_con_sys_i.modulus();
          if (y_modulus_i > 1)
            gr.refine_no_check(le == 1);
          else {
            Linear_Expression le2 = le;
            le2 *= 2;
            gr.refine_no_check(le2 == y_modulus_i);
          }
          break;
        }
      }
    }
    m_swap(gr);
    PPL_ASSERT(OK());
    return false;
  }

  PPL_ASSERT(x.congruences_are_minimized()
         && y.generators_are_minimized());

  const Congruence_System& x_cs = x.con_sys;
  const dimension_type x_cs_num_rows = x_cs.num_rows();

  // Record into `redundant_by_y' the info about which congruences of
  // `x' are redundant in the context `y'.  Count the number of
  // redundancies found.
  std::vector<bool> redundant_by_y(x_cs_num_rows, false);
  dimension_type num_redundant_by_y = 0;
  for (dimension_type i = 0; i < x_cs_num_rows; ++i) {
    if (y.relation_with(x_cs[i]).implies(Poly_Con_Relation::is_included())) {
      redundant_by_y[i] = true;
      ++num_redundant_by_y;
    }
  }

  if (num_redundant_by_y < x_cs_num_rows) {

    // Some congruences were not identified as redundant.

    Congruence_System result_cs;
    const Congruence_System& y_cs = y.con_sys;
    const dimension_type y_cs_num_rows = y_cs.num_rows();
    // Compute into `z' the intersection of `x' and `y'.
    const bool x_first = (x_cs_num_rows > y_cs_num_rows);
    Grid z(x_first ? x : y);
    if (x_first)
      z.add_congruences(y_cs);
    else {
      // Only copy (and then recycle) the non-redundant congruences.
      Congruence_System tmp_cs;
      for (dimension_type i = 0; i < x_cs_num_rows; ++i) {
        if (!redundant_by_y[i])
          tmp_cs.insert(x_cs[i]);
      }
      z.add_recycled_congruences(tmp_cs);
    }

    // Congruences are added to `w' until it equals `z'.
    // We do not care about minimization or maximization, since
    // we are only interested in satisfiability.
    Grid w;
    w.add_space_dimensions_and_embed(x.space_dim);
    // First add the congruences for `y'.
    w.add_congruences(y_cs);

    // We apply the following heuristics here: congruences of `x' that
    // are not made redundant by `y' are added to `w' depending on
    // the number of generators of `y' they rule out (the more generators)
    // (they rule out, the sooner they are added).  Of course, as soon
    // as `w' becomes empty, we stop adding.
    std::vector<Ruled_Out_Pair>
      ruled_out_vec(x_cs_num_rows - num_redundant_by_y);

    PPL_DIRTY_TEMP_COEFFICIENT(sp);
    PPL_DIRTY_TEMP_COEFFICIENT(div);

    for (dimension_type i = 0, j = 0; i < x_cs_num_rows; ++i) {
      if (!redundant_by_y[i]) {
        const Congruence& c = x_cs[i];
        const Coefficient& modulus = c.modulus();
        div = modulus;

        const Grid_Generator_System& y_gs = y.gen_sys;
        dimension_type num_ruled_out_generators = 0;
        for (Grid_Generator_System::const_iterator k = y_gs.begin(),
               y_gs_end = y_gs.end(); k != y_gs_end; ++k) {
          const Grid_Generator& g = *k;
          // If the generator is not to be ruled out,
          // it must saturate the congruence.
          Scalar_Products::assign(sp, c, g);
          // If `c' is a proper congruence the scalar product must be
          // reduced modulo a (possibly scaled) modulus.
          if (c.is_proper_congruence()) {
            // If `g' is a parameter the congruence modulus must be scaled
            // up by the divisor of the generator.
            if (g.is_parameter())
              sp %= (div * g.divisor());
            else
              if (g.is_point())
                sp %= div;
          }
          if (sp == 0)
            continue;
          ++num_ruled_out_generators;
        }
        ruled_out_vec[j].congruence_index = i;
        ruled_out_vec[j].num_ruled_out = num_ruled_out_generators;
        ++j;
      }
    }
    std::sort(ruled_out_vec.begin(), ruled_out_vec.end(),
              Ruled_Out_Less_Than());

    const bool empty_intersection = (!z.minimize());

    // Add the congruences in the "ruled out" order to `w'
    // until the result is the intersection.
    for (std::vector<Ruled_Out_Pair>::const_iterator
           j = ruled_out_vec.begin(), ruled_out_vec_end = ruled_out_vec.end();
         j != ruled_out_vec_end;
         ++j) {
      const Congruence& c = x_cs[j->congruence_index];
      result_cs.insert(c);
      w.add_congruence(c);
      if ((empty_intersection && w.is_empty())
          || (!empty_intersection && w.is_included_in(z))) {
        Grid result_gr(x.space_dim, UNIVERSE);
        result_gr.add_congruences(result_cs);
        x.m_swap(result_gr);
        PPL_ASSERT(x.OK());
        return !empty_intersection;
      }
    }
    // Cannot exit from here.
    PPL_UNREACHABLE;
  }

  // All the congruences are redundant so that the simplified grid
  // is the universe.
  Grid result_gr(x.space_dim, UNIVERSE);
  x.m_swap(result_gr);
  PPL_ASSERT(x.OK());
  return true;
}

void
PPL::Grid::affine_image(const Variable var,
                        const Linear_Expression& expr,
                        Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("affine_image(v, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' must be at most the dimension of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("affine_image(v, e, d)", "e", expr);
  // `var' must be one of the dimensions of the grid.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("affine_image(v, e, d)", "v", var);

  if (marked_empty())
    return;

  Coefficient_traits::const_reference expr_var = expr.coefficient(var);

  if (var_space_dim <= expr_space_dim
      && expr_var != 0) {
    // The transformation is invertible.
    if (generators_are_up_to_date()) {
      // Grid_Generator_System::affine_image() requires the third argument
      // to be a positive Coefficient.
      if (denominator > 0)
        gen_sys.affine_image(var, expr, denominator);
      else
        gen_sys.affine_image(var, -expr, -denominator);
      clear_generators_minimized();
      // Strong normalization in gs::affine_image may have modified
      // divisors.
      normalize_divisors(gen_sys);
    }
    if (congruences_are_up_to_date()) {
      // To build the inverse transformation,
      // after copying and negating `expr',
      // we exchange the roles of `expr[var_space_dim]' and `denominator'.
      Linear_Expression inverse;
      if (expr_var > 0) {
        inverse = -expr;
        inverse.set_coefficient(var, denominator);
        con_sys.affine_preimage(var, inverse, expr_var);
      }
      else {
        // The new denominator is negative: we negate everything once
        // more, as Congruence_System::affine_preimage() requires the
        // third argument to be positive.
        inverse = expr;
        inverse.set_coefficient(var, -denominator);
        con_sys.affine_preimage(var, inverse, -expr_var);
      }
      clear_congruences_minimized();
    }
  }
  else {
    // The transformation is not invertible.
    // We need an up-to-date system of generators.
    if (!generators_are_up_to_date())
      minimize();
    if (!marked_empty()) {
      // Grid_Generator_System::affine_image() requires the third argument
      // to be a positive Coefficient.
      if (denominator > 0)
        gen_sys.affine_image(var, expr, denominator);
      else
        gen_sys.affine_image(var, -expr, -denominator);

      clear_congruences_up_to_date();
      clear_generators_minimized();
      // Strong normalization in gs::affine_image may have modified
      // divisors.
      normalize_divisors(gen_sys);
    }
  }
  PPL_ASSERT(OK());
}

void
PPL::Grid::
affine_preimage(const Variable var,
                const Linear_Expression& expr,
                Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("affine_preimage(v, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("affine_preimage(v, e, d)", "e", expr);
  // `var' should be one of the dimensions of the grid.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("affine_preimage(v, e, d)", "v", var);

  if (marked_empty())
    return;

  Coefficient_traits::const_reference expr_var = expr.coefficient(var);

  if (var_space_dim <= expr_space_dim && expr_var != 0) {
    // The transformation is invertible.
    if (congruences_are_up_to_date()) {
      // Congruence_System::affine_preimage() requires the third argument
      // to be a positive Coefficient.
      if (denominator > 0)
        con_sys.affine_preimage(var, expr, denominator);
      else
        con_sys.affine_preimage(var, -expr, -denominator);
      clear_congruences_minimized();
    }
    if (generators_are_up_to_date()) {
      // To build the inverse transformation,
      // after copying and negating `expr',
      // we exchange the roles of `expr[var_space_dim]' and `denominator'.
      Linear_Expression inverse;
      if (expr_var > 0) {
        inverse = -expr;
        inverse.set_coefficient(var, denominator);
        gen_sys.affine_image(var, inverse, expr_var);
      }
      else {
        // The new denominator is negative: we negate everything once
        // more, as Grid_Generator_System::affine_image() requires the
        // third argument to be positive.
        inverse = expr;
        inverse.set_coefficient(var, -denominator);
        gen_sys.affine_image(var, inverse, -expr_var);
      }
      clear_generators_minimized();
    }
  }
  else {
    // The transformation is not invertible.
    // We need an up-to-date system of congruences.
    if (!congruences_are_up_to_date())
      minimize();
    // Congruence_System::affine_preimage() requires the third argument
    // to be a positive Coefficient.
    if (denominator > 0)
      con_sys.affine_preimage(var, expr, denominator);
    else
      con_sys.affine_preimage(var, -expr, -denominator);

    clear_generators_up_to_date();
    clear_congruences_minimized();
  }
  PPL_ASSERT(OK());
}

void
PPL::Grid::
generalized_affine_image(const Variable var,
                         const Relation_Symbol relsym,
                         const Linear_Expression& expr,
                         Coefficient_traits::const_reference denominator,
                         Coefficient_traits::const_reference modulus) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("generalized_affine_image(v, r, e, d, m)",
                           "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("generalized_affine_image(v, r, e, d, m)",
                                 "e", expr);
  // `var' should be one of the dimensions of the grid.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("generalized_affine_image(v, r, e, d, m)",
                                 "v", var);
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_image(v, r, e, d, m)",
                           "r is the disequality relation symbol");

  // Any image of an empty grid is empty.
  if (marked_empty())
    return;

  // If relsym is not EQUAL, then we return a safe approximation
  // by adding a line in the direction of var.
  if (relsym != EQUAL) {

    if (modulus != 0)
      throw_invalid_argument("generalized_affine_image(v, r, e, d, m)",
                             "r != EQUAL && m != 0");

    if (!generators_are_up_to_date())
      minimize();

    // Any image of an empty grid is empty.
    if (marked_empty())
      return;

    add_grid_generator(grid_line(var));

    PPL_ASSERT(OK());
    return;
  }

  PPL_ASSERT(relsym == EQUAL);

  affine_image(var, expr, denominator);

  if (modulus == 0)
    return;

  // Modulate dimension `var' according to `modulus'.

  if (!generators_are_up_to_date())
    minimize();

  // Test if minimization, possibly in affine_image, found an empty
  // grid.
  if (marked_empty())
    return;

  if (modulus < 0)
    gen_sys.insert(parameter(-modulus * var));
  else
    gen_sys.insert(parameter(modulus * var));

  normalize_divisors(gen_sys);

  clear_generators_minimized();
  clear_congruences_up_to_date();

  PPL_ASSERT(OK());
}

void
PPL::Grid::
generalized_affine_preimage(const Variable var,
                            const Relation_Symbol relsym,
                            const Linear_Expression& expr,
                            Coefficient_traits::const_reference denominator,
                            Coefficient_traits::const_reference modulus) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("generalized_affine_preimage(v, r, e, d, m)",
                           "d == 0");

  // The dimension of `expr' should be at most the dimension of
  // `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(v, r, e, d, m)",
                                 "e", expr);
  // `var' should be one of the dimensions of the grid.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(v, r, e, d, m)",
                                 "v", var);
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_preimage(v, r, e, d, m)",
                           "r is the disequality relation symbol");

  // If relsym is not EQUAL, then we return a safe approximation
  // by adding a line in the direction of var.
  if (relsym != EQUAL) {

    if (modulus != 0)
      throw_invalid_argument("generalized_affine_preimage(v, r, e, d, m)",
                             "r != EQUAL && m != 0");

    if (!generators_are_up_to_date())
      minimize();

    // Any image of an empty grid is empty.
    if (marked_empty())
      return;

    add_grid_generator(grid_line(var));

    PPL_ASSERT(OK());
    return;
  }

  PPL_ASSERT(relsym == EQUAL);
  // Any image of an empty grid is empty.
  if (marked_empty())
    return;

  // Check whether the affine relation is an affine function.
  if (modulus == 0) {
    affine_preimage(var, expr, denominator);
    return;
  }

  // Check whether the preimage of this affine relation can be easily
  // computed as the image of its inverse relation.
  const Coefficient& var_coefficient = expr.coefficient(var);
  if (var_space_dim <= expr_space_dim && var_coefficient != 0) {
    const Linear_Expression inverse_expr
      = expr - (denominator + var_coefficient) * var;
    PPL_DIRTY_TEMP_COEFFICIENT(inverse_denominator);
    neg_assign(inverse_denominator, var_coefficient);
    if (modulus < 0)
      generalized_affine_image(var, EQUAL, inverse_expr, inverse_denominator,
                               - modulus);
    else
      generalized_affine_image(var, EQUAL, inverse_expr, inverse_denominator,
                               modulus);
    return;
  }

  // Here `var_coefficient == 0', so that the preimage cannot be
  // easily computed by inverting the affine relation.  Add the
  // congruence induced by the affine relation.
  {
    Congruence cg((denominator*var %= expr) / denominator);
    if (modulus < 0)
      cg /= -modulus;
    else
      cg /= modulus;
    add_congruence_no_check(cg);
  }

  // If the resulting grid is empty, its preimage is empty too.
  // Note: DO check for emptiness here, as we will later add a line.
  if (is_empty())
    return;
  add_grid_generator(grid_line(var));
  PPL_ASSERT(OK());
}

void
PPL::Grid::
generalized_affine_image(const Linear_Expression& lhs,
                         const Relation_Symbol relsym,
                         const Linear_Expression& rhs,
                         Coefficient_traits::const_reference modulus) {
  // Dimension-compatibility checks.
  // The dimension of `lhs' should be at most the dimension of
  // `*this'.
  dimension_type lhs_space_dim = lhs.space_dimension();
  if (space_dim < lhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2, m)",
                                 "e1", lhs);
  // The dimension of `rhs' should be at most the dimension of
  // `*this'.
  const dimension_type rhs_space_dim = rhs.space_dimension();
  if (space_dim < rhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2, m)",
                                 "e2", rhs);
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_image(e1, r, e2, m)",
                           "r is the disequality relation symbol");

  // Any image of an empty grid is empty.
  if (marked_empty())
    return;

  // If relsym is not EQUAL, then we return a safe approximation
  // by adding a line in the direction of var.
  if (relsym != EQUAL) {

    if (modulus != 0)
      throw_invalid_argument("generalized_affine_image(e1, r, e2, m)",
                             "r != EQUAL && m != 0");

    if (!generators_are_up_to_date())
      minimize();

    // Any image of an empty grid is empty.
    if (marked_empty())
      return;

    for (Linear_Expression::const_iterator i = lhs.begin(), i_end = lhs.end();
          i != i_end; ++i)
      add_grid_generator(grid_line(i.variable()));

    PPL_ASSERT(OK());
    return;
  }

  PPL_ASSERT(relsym == EQUAL);

  PPL_DIRTY_TEMP_COEFFICIENT(tmp_modulus);
  tmp_modulus = modulus;
  if (tmp_modulus < 0)
    neg_assign(tmp_modulus);

  // Compute the actual space dimension of `lhs',
  // i.e., the highest dimension having a non-zero coefficient in `lhs'.

  lhs_space_dim = lhs.last_nonzero();
  if (lhs_space_dim == 0) {
    // All variables have zero coefficients, so `lhs' is a constant.
    add_congruence_no_check((lhs %= rhs) / tmp_modulus);
    return;
  }

  // Gather in `new_lines' the collections of all the lines having the
  // direction of variables occurring in `lhs'.
  Grid_Generator_System new_lines;
  for (Linear_Expression::const_iterator i = lhs.begin(),
        i_end = lhs.lower_bound(Variable(lhs_space_dim)); i != i_end; ++i)
    new_lines.insert(grid_line(i.variable()));

  const dimension_type num_common_dims = std::min(lhs_space_dim, rhs_space_dim);
  if (lhs.have_a_common_variable(rhs, Variable(0), Variable(num_common_dims))) {
    // Some variables in `lhs' also occur in `rhs'.
    // To ease the computation, add an additional dimension.
    const Variable new_var(space_dim);
    add_space_dimensions_and_embed(1);

    // Constrain the new dimension to be equal to the right hand side.
    // TODO: Use add_congruence() when it has been updated.
    Congruence_System new_cgs1(new_var == rhs);
    add_recycled_congruences(new_cgs1);
    if (!is_empty()) {
      // The grid still contains points.

      // Existentially quantify all the variables occurring in the left
      // hand side expression.

      // Adjust `new_lines' to the right dimension.
      new_lines.set_space_dimension(space_dim);
      // Add the lines to `gen_sys' (first make sure they are up-to-date).
      update_generators();
      gen_sys.insert(new_lines, Recycle_Input());
      normalize_divisors(gen_sys);
      // Update the flags.
      clear_congruences_up_to_date();
      clear_generators_minimized();

      // Constrain the new dimension so that it is congruent to the left
      // hand side expression modulo `modulus'.
      // TODO: Use add_congruence() when it has been updated.
      Congruence_System new_cgs2((lhs %= new_var) / tmp_modulus);
      add_recycled_congruences(new_cgs2);
    }

    // Remove the temporarily added dimension.
    remove_higher_space_dimensions(space_dim-1);
  }
  else {
    // `lhs' and `rhs' variables are disjoint:
    // there is no need to add a further dimension.

    // Only add the lines and congruence if there are points.
    if (is_empty())
      return;

    // Existentially quantify all the variables occurring in the left hand
    // side expression.
    add_recycled_grid_generators(new_lines);

    // Constrain the left hand side expression so that it is congruent to
    // the right hand side expression modulo `modulus'.
    add_congruence_no_check((lhs %= rhs) / tmp_modulus);
  }

  PPL_ASSERT(OK());
}

void
PPL::Grid::
generalized_affine_preimage(const Linear_Expression& lhs,
                            const Relation_Symbol relsym,
                            const Linear_Expression& rhs,
                            Coefficient_traits::const_reference modulus) {
  // The dimension of `lhs' must be at most the dimension of `*this'.
  dimension_type lhs_space_dim = lhs.space_dimension();
  if (space_dim < lhs_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(e1, r, e2, m)",
                                 "lhs", lhs);
  // The dimension of `rhs' must be at most the dimension of `*this'.
  const dimension_type rhs_space_dim = rhs.space_dimension();
  if (space_dim < rhs_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(e1, r, e2, m)",
                                 "e2", rhs);
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_preimage(e1, r, e2, m)",
                           "r is the disequality relation symbol");

  // Any preimage of an empty grid is empty.
  if (marked_empty())
    return;

  // If relsym is not EQUAL, then we return a safe approximation
  // by adding a line in the direction of var.
  if (relsym != EQUAL) {

    if (modulus != 0)
      throw_invalid_argument("generalized_affine_preimage(e1, r, e2, m)",
                             "r != EQUAL && m != 0");

    if (!generators_are_up_to_date())
      minimize();

    // Any image of an empty grid is empty.
    if (marked_empty())
      return;

    for (Linear_Expression::const_iterator i = lhs.begin(), i_end = lhs.end();
          i != i_end; ++i)
      add_grid_generator(grid_line(i.variable()));

    PPL_ASSERT(OK());
    return;
  }

  PPL_ASSERT(relsym == EQUAL);

  PPL_DIRTY_TEMP_COEFFICIENT(tmp_modulus);
  tmp_modulus = modulus;
  if (tmp_modulus < 0)
    neg_assign(tmp_modulus);

  // Compute the actual space dimension of `lhs',
  // i.e., the highest dimension having a non-zero coefficient in `lhs'.
  lhs_space_dim = lhs.last_nonzero();
  if (lhs_space_dim == 0) {
    // All variables have zero coefficients, so `lhs' is a constant.
    // In this case, preimage and image happen to be the same.
    add_congruence_no_check((lhs %= rhs) / tmp_modulus);
    return;
  }

  // Gather in `new_lines' the collections of all the lines having
  // the direction of variables occurring in `lhs'.
  Grid_Generator_System new_lines;
  for (Linear_Expression::const_iterator i = lhs.begin(),
        i_end = lhs.lower_bound(Variable(lhs_space_dim)); i != i_end; ++i)
      new_lines.insert(grid_line(i.variable()));

  const dimension_type num_common_dims
    = std::min(lhs_space_dim, rhs_space_dim);
  if (lhs.have_a_common_variable(rhs, Variable(0), Variable(num_common_dims))) {
    // Some variables in `lhs' also occur in `rhs'.
    // To ease the computation, add an additional dimension.
    const Variable new_var(space_dim);
    add_space_dimensions_and_embed(1);

    // Constrain the new dimension to be equal to `lhs'
    // TODO: Use add_congruence() when it has been updated.
    Congruence_System new_cgs1(new_var == lhs);
    add_recycled_congruences(new_cgs1);
    if (!is_empty()) {
      // The grid still contains points.

      // Existentially quantify all the variables occurring in the left
      // hand side

      // Adjust `new_lines' to the right dimension.
      new_lines.set_space_dimension(space_dim);
      // Add the lines to `gen_sys' (first make sure they are up-to-date).
      update_generators();
      gen_sys.insert(new_lines, Recycle_Input());
      normalize_divisors(gen_sys);
      // Update the flags.
      clear_congruences_up_to_date();
      clear_generators_minimized();

      // Constrain the new dimension so that it is related to
      // the right hand side modulo `modulus'.
      // TODO: Use add_congruence() when it has been updated.
      Congruence_System new_cgs2((rhs %= new_var) / tmp_modulus);
      add_recycled_congruences(new_cgs2);
    }

    // Remove the temporarily added dimension.
    remove_higher_space_dimensions(space_dim-1);
  }
  else {
    // `lhs' and `rhs' variables are disjoint:
    // there is no need to add a further dimension.

    // Constrain the left hand side expression so that it is congruent to
    // the right hand side expression modulo `mod'.
    add_congruence_no_check((lhs %= rhs) / tmp_modulus);

    // Any image of an empty grid is empty.
    if (is_empty())
      return;

    // Existentially quantify all the variables occurring in `lhs'.
    add_recycled_grid_generators(new_lines);
  }
  PPL_ASSERT(OK());
}

void
PPL::Grid::
bounded_affine_image(const Variable var,
                     const Linear_Expression& lb_expr,
                     const Linear_Expression& ub_expr,
                     Coefficient_traits::const_reference denominator) {

  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("bounded_affine_image(v, lb, ub, d)", "d == 0");

  // Dimension-compatibility checks.
  // `var' should be one of the dimensions of the grid.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dim < var_space_dim)
    throw_dimension_incompatible("bounded_affine_image(v, lb, ub, d)",
                                 "v", var);
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

  // Any image of an empty grid is empty.
  if (marked_empty())
    return;

  // In all other cases, generalized_affine_preimage() must
  // just add a line in the direction of var.
  generalized_affine_image(var,
                           LESS_OR_EQUAL,
                           ub_expr,
                           denominator);

  PPL_ASSERT(OK());
}


void
PPL::Grid::
bounded_affine_preimage(const Variable var,
                        const Linear_Expression& lb_expr,
                        const Linear_Expression& ub_expr,
                        Coefficient_traits::const_reference denominator) {

  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("bounded_affine_preimage(v, lb, ub, d)", "d == 0");

  // Dimension-compatibility checks.
  // `var' should be one of the dimensions of the grid.
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

  // Any preimage of an empty grid is empty.
  if (marked_empty())
    return;

  // In all other cases, generalized_affine_preimage() must
  // just add a line in the direction of var.
  generalized_affine_preimage(var,
                              LESS_OR_EQUAL,
                              ub_expr,
                              denominator);

  PPL_ASSERT(OK());
}

void
PPL::Grid::time_elapse_assign(const Grid& y) {
  Grid& x = *this;
  // Check dimension-compatibility.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("time_elapse_assign(y)", "y", y);

  // Deal with the zero-dimensional case.
  if (x.space_dim == 0) {
    if (y.marked_empty())
      x.set_empty();
    return;
  }

  // If either one of `x' or `y' is empty, the result is empty too.
  if (x.marked_empty())
    return;
  if (y.marked_empty()
      || (!x.generators_are_up_to_date() && !x.update_generators())
      || (!y.generators_are_up_to_date() && !y.update_generators())) {
    x.set_empty();
    return;
  }

  // At this point both generator systems are up-to-date.
  Grid_Generator_System gs = y.gen_sys;
  const dimension_type gs_num_rows = gs.num_rows();

  normalize_divisors(gs, gen_sys);

  for (dimension_type i = gs_num_rows; i-- > 0; ) {
    Grid_Generator& g = gs.sys.rows[i];
    if (g.is_point())
      // Transform the point into a parameter.
      g.set_is_parameter();
  }

  PPL_ASSERT(gs.sys.OK());

  if (gs_num_rows == 0)
    // `y' was the grid containing a single point at the origin, so
    // the result is `x'.
    return;

  // Append `gs' to the generators of `x'.

  gen_sys.insert(gs, Recycle_Input());

  x.clear_congruences_up_to_date();
  x.clear_generators_minimized();

  PPL_ASSERT(x.OK(true) && y.OK(true));
}

bool
PPL::Grid::frequency(const Linear_Expression& expr,
                     Coefficient& freq_n, Coefficient& freq_d,
                     Coefficient& val_n, Coefficient& val_d) const {
  // The dimension of `expr' must be at most the dimension of *this.
  if (space_dim < expr.space_dimension())
    throw_dimension_incompatible("frequency(e, ...)", "e", expr);

  // Space dimension is 0: if empty, then return false;
  // otherwise the frequency is 1 and the value is 0.
  if (space_dim == 0) {
    if (is_empty())
      return false;
    freq_n = 0;
    freq_d = 1;
    val_n = 0;
    val_d = 1;
    return true;
  }
  if (!generators_are_minimized() && !minimize())
    // Minimizing found `this' empty.
    return false;

  return frequency_no_check(expr, freq_n, freq_d, val_n, val_d);
}

/*! \relates Parma_Polyhedra_Library::Grid */
bool
PPL::operator==(const Grid& x, const Grid& y) {
  if (x.space_dim != y.space_dim)
    return false;

  if (x.marked_empty())
    return y.is_empty();
  if (y.marked_empty())
    return x.is_empty();
  if (x.space_dim == 0)
    return true;

  switch (x.quick_equivalence_test(y)) {
  case Grid::TVB_TRUE:
    return true;

  case Grid::TVB_FALSE:
    return false;

  default:
    if (x.is_included_in(y)) {
      if (x.marked_empty())
        return y.is_empty();
      return y.is_included_in(x);
    }
    return false;
  }
}

bool
PPL::Grid::contains(const Grid& y) const {
  const Grid& x = *this;

  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("contains(y)", "y", y);

  if (y.marked_empty())
    return true;
  if (x.marked_empty())
    return y.is_empty();
  if (y.space_dim == 0)
    return true;
  if (x.quick_equivalence_test(y) == Grid::TVB_TRUE)
    return true;
  return y.is_included_in(x);
}

bool
PPL::Grid::is_disjoint_from(const Grid& y) const {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("is_disjoint_from(y)", "y", y);
  Grid z = *this;
  z.intersection_assign(y);
  return z.is_empty();
}

void
PPL::Grid::ascii_dump(std::ostream& s) const {
  using std::endl;

  s << "space_dim "
    << space_dim
    << endl;
  status.ascii_dump(s);
  s << "con_sys ("
    << (congruences_are_up_to_date() ? "" : "not_")
    << "up-to-date)"
    << endl;
  con_sys.ascii_dump(s);
  s << "gen_sys ("
    << (generators_are_up_to_date() ? "" : "not_")
    << "up-to-date)"
    << endl;
  gen_sys.ascii_dump(s);
  s << "dimension_kinds";
  if ((generators_are_up_to_date() && generators_are_minimized())
      || (congruences_are_up_to_date() && congruences_are_minimized()))
    for (Dimension_Kinds::const_iterator i = dim_kinds.begin();
         i != dim_kinds.end();
         ++i)
      s << " " << *i;
  s << endl;
}

PPL_OUTPUT_DEFINITIONS(Grid)

bool
PPL::Grid::ascii_load(std::istream& s) {
  std::string str;

  if (!(s >> str) || str != "space_dim")
    return false;

  if (!(s >> space_dim))
    return false;

  if (!status.ascii_load(s))
    return false;

  if (!(s >> str) || str != "con_sys")
    return false;

  if (!(s >> str))
    return false;
  if (str == "(up-to-date)")
    set_congruences_up_to_date();
  else if (str != "(not_up-to-date)")
    return false;

  if (!con_sys.ascii_load(s))
    return false;

  if (!(s >> str) || str != "gen_sys")
    return false;

  if (!(s >> str))
    return false;
  if (str == "(up-to-date)")
    set_generators_up_to_date();
  else if (str != "(not_up-to-date)")
    return false;

  if (!gen_sys.ascii_load(s))
    return false;

  if (!(s >> str) || str != "dimension_kinds")
    return false;

  if (!marked_empty()
      && ((generators_are_up_to_date() && generators_are_minimized())
          || (congruences_are_up_to_date() && congruences_are_minimized()))) {
    dim_kinds.resize(space_dim + 1);
    for (Dimension_Kinds::size_type dim = 0; dim <= space_dim; ++dim) {
      short unsigned int dim_kind;
      if (!(s >> dim_kind))
        return false;
      switch(dim_kind) {
      case 0: dim_kinds[dim] = PARAMETER; break;
      case 1: dim_kinds[dim] = LINE; break;
      case 2: dim_kinds[dim] = GEN_VIRTUAL; break;
      default: return false;
      }
    }
  }

  // Check invariants.
  PPL_ASSERT(OK());
  return true;
}

PPL::memory_size_type
PPL::Grid::external_memory_in_bytes() const {
  return
    con_sys.external_memory_in_bytes()
    + gen_sys.external_memory_in_bytes();
}

void
PPL::Grid::wrap_assign(const Variables_Set& vars,
                       Bounded_Integer_Type_Width w,
                       Bounded_Integer_Type_Representation r,
                       Bounded_Integer_Type_Overflow o,
                       const Constraint_System* cs_p,
                       unsigned /* complexity_threshold */,
                       bool /* wrap_individually */) {

  // Dimension-compatibility check of `*cs_p', if any.
  if (cs_p != 0) {
    const dimension_type cs_p_space_dim  = cs_p->space_dimension();
    if (cs_p->space_dimension() > space_dim)
      throw_dimension_incompatible("wrap_assign(vs, ...)", cs_p_space_dim);
  }

  // Wrapping no variable is a no-op.
  if (vars.empty())
    return;

  // Dimension-compatibility check of `vars'.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dim < min_space_dim)
    throw_dimension_incompatible("wrap_assign(vs, ...)", min_space_dim);

  // Wrapping an empty grid is a no-op.
  if (marked_empty())
    return;
  if (!generators_are_minimized() && !minimize())
    // Minimizing found `this' empty.
    return;

  // Set the wrap frequency for variables of width `w'.
  // This is independent of the signedness `s'.
  PPL_DIRTY_TEMP_COEFFICIENT(wrap_frequency);
  mul_2exp_assign(wrap_frequency, Coefficient_one(), w);
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

  // Generators are up-to-date and minimized.
  const Grid gr = *this;

  // Overflow is impossible or wraps.
  if (o == OVERFLOW_IMPOSSIBLE || o == OVERFLOW_WRAPS) {
    PPL_DIRTY_TEMP_COEFFICIENT(f_n);
    PPL_DIRTY_TEMP_COEFFICIENT(f_d);
    PPL_DIRTY_TEMP_COEFFICIENT(v_n);
    PPL_DIRTY_TEMP_COEFFICIENT(v_d);
    for (Variables_Set::const_iterator i = vars.begin(),
           vars_end = vars.end(); i != vars_end; ++i) {
      const Variable x(*i);
      // Find the frequency and a value for `x' in `gr'.
      if (!gr.frequency_no_check(x, f_n, f_d, v_n, v_d))
        continue;
      if (f_n == 0) {

        // `x' is a constant in `gr'.
        if (v_d != 1) {
          // The value for `x' is not integral (`frequency_no_check()'
          // ensures that `v_n' and `v_d' have no common divisors).
          set_empty();
          return;
        }

        // `x' is a constant and has an integral value.
        if ((v_n > max_value) || (v_n < min_value)) {
          // The value is outside the range of the bounded integer type.
          if (o == OVERFLOW_IMPOSSIBLE) {
            // Then `x' has no possible value and hence set empty.
            set_empty();
            return;
          }
          PPL_ASSERT(o == OVERFLOW_WRAPS);
          // The value v_n for `x' is wrapped modulo the 'wrap_frequency'.
          v_n %= wrap_frequency;
          // `v_n' is the value closest to 0 and may be negative.
          if (r == UNSIGNED && v_n < 0)
            v_n += wrap_frequency;
          unconstrain(x);
          add_constraint(x == v_n);
        }
        continue;
      }

      // `x' is not a constant in `gr'.
      PPL_ASSERT(f_n != 0);

      if (f_d % v_d != 0) {
        // Then `x' has no integral value and hence `gr' is set empty.
        set_empty();
        return;
      }
      if (f_d != 1) {
        // `x' has non-integral values, so add the integrality
        // congruence for `x'.
        add_congruence((x %= 0) / 1);
      }
      if (o == OVERFLOW_WRAPS && f_n != wrap_frequency)
        // We know that `x' is not a constant, so, if overflow wraps,
        // `x' may wrap to a value modulo the `wrap_frequency'.
        add_grid_generator(parameter(wrap_frequency * x));
      else if ((o == OVERFLOW_IMPOSSIBLE && 2*f_n >= wrap_frequency)
               || (f_n == wrap_frequency)) {
        // In these cases, `x' can only take a unique (ie constant)
        // value.
        if (r == UNSIGNED && v_n < 0) {
          // `v_n' is the value closest to 0 and may be negative.
          v_n += f_n;
        }
        unconstrain(x);
        add_constraint(x == v_n);
      }
      else {
        // If overflow is impossible but the grid frequency is less than
        // half the wrap frequency, then there is more than one possible
        // value for `x' in the range of the bounded integer type,
        // so the grid is unchanged.
        PPL_ASSERT(o == OVERFLOW_IMPOSSIBLE && 2*f_n < wrap_frequency);
      }
    }
    return;
  }

  PPL_ASSERT(o == OVERFLOW_UNDEFINED);
  // If overflow is undefined, then all we know is that the variable
  // may take any integer within the range of the bounded integer type.
  const Grid_Generator& point = gr.gen_sys[0];
  const Coefficient& div = point.divisor();
  max_value *= div;
  min_value *= div;
  for (Variables_Set::const_iterator i = vars.begin(),
         vars_end = vars.end(); i != vars_end; ++i) {
    const Variable x(*i);
    if (!gr.bounds_no_check(x)) {
      // `x' is not a constant in `gr'.
      // We know that `x' is not a constant, so `x' may wrap to any
      // value `x + z' where z is an integer.
      if (point.coefficient(x) % div != 0) {
        // We know that `x' can take non-integral values, so enforce
        // integrality.
        unconstrain(x);
        add_congruence(x %= 0);
      }
      else
        // `x' has at least one integral value.
        // `x' may also take other non-integral values,
        // but checking could be costly so we ignore this.
        add_grid_generator(parameter(x));
    }
    else {
      // `x' is a constant `v' in `gr'.
      const Coefficient& coeff_x = point.coefficient(x);
      // `x' should be integral.
      if (coeff_x % div != 0) {
        set_empty();
        return;
      }
      // If the value `v' for `x' is not within the range for the
      // bounded integer type, then `x' may wrap to any value `v + z'
      // where `z' is an integer; otherwise `x' is unchanged.
      if (coeff_x > max_value || coeff_x < min_value) {
        add_grid_generator(parameter(x));
      }
    }
  }
}

void
PPL::Grid::drop_some_non_integer_points(Complexity_Class) {
  if (marked_empty() || space_dim == 0)
    return;

  // By adding integral congruences for each dimension to the
  // congruence system, defining \p *this, the grid will keep only
  // those points that have integral coordinates. All points in \p
  // *this with non-integral coordinates are removed.
  for (dimension_type i = space_dim; i-- > 0; )
    add_congruence(Variable(i) %= 0);

  PPL_ASSERT(OK());
}

void
PPL::Grid::drop_some_non_integer_points(const Variables_Set& vars,
                                        Complexity_Class) {
  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dimension() < min_space_dim)
    throw_dimension_incompatible("drop_some_non_integer_points(vs, cmpl)",
                                 min_space_dim);

  if (marked_empty() || min_space_dim == 0)
    return;

  // By adding the integral congruences for each dimension in vars to
  // the congruence system defining \p *this, the grid will keep only
  // those points that have integer coordinates for all the dimensions
  // in vars. All points in \p *this with non-integral coordinates for
  // the dimensions in vars are removed.
  for (Variables_Set::const_iterator i = vars.begin(),
         vars_end = vars.end(); i != vars_end; ++i)
    add_congruence(Variable(*i) %= 0);

  PPL_ASSERT(OK());
}

/*! \relates Parma_Polyhedra_Library::Grid */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const Grid& gr) {
  if (gr.is_empty())
    s << "false";
  else if (gr.is_universe())
    s << "true";
  else
    s << gr.minimized_congruences();
  return s;
}
