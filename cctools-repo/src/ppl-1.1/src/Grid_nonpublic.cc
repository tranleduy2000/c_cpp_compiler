/* Grid class implementation
   (non-inline private or protected functions).
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
#include "Grid_Generator_defs.hh"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include "assert.hh"
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \ingroup PPL_defines
  \brief
  Controls the laziness level of the implementation.

  Temporarily used in a few of the function implementations to
  switch to an even more lazy algorithm. To be removed as soon as
  we collect enough information to decide which is the better
  implementation alternative.
*/
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
#define BE_LAZY 1

namespace PPL = Parma_Polyhedra_Library;

void
PPL::Grid::construct(dimension_type num_dimensions,
                     const Degenerate_Element kind) {
  space_dim = num_dimensions;

  if (kind == EMPTY) {
    // Set emptiness directly instead of with set_empty, as gen_sys is
    // already correctly initialized.
    status.set_empty();

    // Extend the zero dim false congruence system to the appropriate
    // dimension and then store it in `con_sys'.
    Congruence_System cgs(Congruence::zero_dim_false());
    cgs.set_space_dimension(space_dim);
    swap(con_sys, cgs);

    PPL_ASSERT(OK());
    return;
  }

  if (space_dim == 0) {
    set_zero_dim_univ();
    return;
  }

  // Initialize both systems to universe representations.
  set_congruences_minimized();
  set_generators_minimized();
  dim_kinds.resize(space_dim + 1);

  // Building a universe congruence system.
  // Extend the zero dim integrality congruence system to the
  // appropriate dimension and then store it in `con_sys'.
  Congruence_System cgs(Congruence::zero_dim_integrality());
  cgs.set_space_dimension(num_dimensions);
  // Recover minimal form after cgs(zdi) normalization.
  cgs.rows[0].expr.set_inhomogeneous_term(Coefficient_one());
  PPL_ASSERT(cgs.OK());
  swap(con_sys, cgs);

  // Building a universe grid generator system (and dim_kinds).
  gen_sys.set_space_dimension(space_dim);
  gen_sys.insert(grid_point());
  dim_kinds[0] = PROPER_CONGRUENCE /* a.k.a. PARAMETER */;
  for (dimension_type dim = 0; dim < space_dim; ++dim) {
    gen_sys.insert(grid_line(Variable(dim)));
    dim_kinds[1+dim] = CON_VIRTUAL /* a.k.a. LINE */;
  }
  PPL_ASSERT(OK());
}

void
PPL::Grid::construct(Congruence_System& cgs) {
  // Protecting against space dimension overflow is up to the caller.
  PPL_ASSERT(cgs.space_dimension() <= max_space_dimension());
  // Preparing con_sys and gen_sys is up to the caller.
  PPL_ASSERT(cgs.space_dimension() == con_sys.space_dimension());
  PPL_ASSERT(cgs.space_dimension() == gen_sys.space_dimension());
  PPL_ASSERT(con_sys.has_no_rows());
  PPL_ASSERT(gen_sys.has_no_rows());

  // Set the space dimension.
  space_dim = cgs.space_dimension();

  if (space_dim > 0) {
    // Stealing the rows from `cgs'.
    con_sys.m_swap(cgs);
    con_sys.normalize_moduli();
    set_congruences_up_to_date();
  }
  else {
    // Here `space_dim == 0'.
    // See if an inconsistent congruence has been passed.
    for (dimension_type i = cgs.num_rows(); i-- > 0; )
      if (cgs[i].is_inconsistent()) {
        // Inconsistent congruence found: the grid is empty.
        status.set_empty();
        // Insert the zero dim false congruence system into `con_sys'.
        // `gen_sys' is already in empty form.
        con_sys.insert(Congruence::zero_dim_false());
        PPL_ASSERT(OK());
        return;
      }
    set_zero_dim_univ();
  }
  PPL_ASSERT(OK());
}

void
PPL::Grid::construct(Grid_Generator_System& ggs) {
  // Protecting against space dimension overflow is up to the caller.
  PPL_ASSERT(ggs.space_dimension() <= max_space_dimension());
  // Preparing con_sys and gen_sys is up to the caller.
  PPL_ASSERT(ggs.space_dimension() == con_sys.space_dimension());
  PPL_ASSERT(ggs.space_dimension() == gen_sys.space_dimension());
  PPL_ASSERT(con_sys.has_no_rows());
  PPL_ASSERT(gen_sys.has_no_rows());

  // Set the space dimension.
  space_dim = ggs.space_dimension();

  // An empty set of generators defines the empty grid.
  if (ggs.has_no_rows()) {
    status.set_empty();
    // Insert the zero dim false congruence system into `con_sys'.
    // `gen_sys' is already in empty form.
    con_sys.insert(Congruence::zero_dim_false());
    return;
  }

  // Non-empty valid generator systems have a supporting point, at least.
  if (!ggs.has_points())
    throw_invalid_generators("Grid(ggs)", "ggs");

  if (space_dim == 0)
    set_zero_dim_univ();
  else {
    // Steal the rows from `ggs'.
    gen_sys.m_swap(ggs);
    normalize_divisors(gen_sys);
    // Generators are now up-to-date.
    set_generators_up_to_date();
  }

  PPL_ASSERT(OK());
}

PPL::Grid::Three_Valued_Boolean
PPL::Grid::quick_equivalence_test(const Grid& y) const {
  // Private method: the caller must ensure the following.
  PPL_ASSERT(space_dim == y.space_dim);
  PPL_ASSERT(!marked_empty() && !y.marked_empty() && space_dim > 0);

  const Grid& x = *this;

  bool css_normalized = false;

  if (x.congruences_are_minimized() && y.congruences_are_minimized()) {
    // Equivalent minimized congruence systems have:
    //  - the same number of congruences; ...
    if (x.con_sys.num_rows() != y.con_sys.num_rows())
      return Grid::TVB_FALSE;
    //  - the same number of equalities; ...
    const dimension_type x_num_equalities = x.con_sys.num_equalities();
    if (x_num_equalities != y.con_sys.num_equalities())
      return Grid::TVB_FALSE;
    //  - and if there are no equalities, the same congruences.
    //    Delay this test: try cheaper tests on generators first.
    css_normalized = (x_num_equalities == 0);
  }

  if (x.generators_are_minimized() && y.generators_are_minimized()) {
    // Equivalent minimized generator systems have:
    //  - the same number of generators; ...
    if (x.gen_sys.num_rows() != y.gen_sys.num_rows())
      return Grid::TVB_FALSE;
    //  - the same number of lines; ...
    const dimension_type x_num_lines = x.gen_sys.num_lines();
    if (x_num_lines != y.gen_sys.num_lines())
      return Grid::TVB_FALSE;
      //  - and if there are no lines, the same generators.
    if (x_num_lines == 0) {
      // Check for syntactic identity.
      if (x.gen_sys == y.gen_sys)
        return Grid::TVB_TRUE;
      else
        return Grid::TVB_FALSE;
    }
  }

  // TODO: Consider minimizing the systems and re-performing these
  //       checks.

  if (css_normalized) {
    if (x.con_sys == y.con_sys)
      return Grid::TVB_TRUE;
    else
      return Grid::TVB_FALSE;
  }

  return Grid::TVB_DONT_KNOW;
}

bool
PPL::Grid::is_included_in(const Grid& y) const {
  // Private method: the caller must ensure the following.
  PPL_ASSERT(space_dim == y.space_dim);
  PPL_ASSERT(!marked_empty() && !y.marked_empty() && space_dim > 0);

  const Grid& x = *this;

#if BE_LAZY
  if (!x.generators_are_up_to_date() && !x.update_generators())
    // Updating found `x' empty.
    return true;
  if (!y.congruences_are_up_to_date())
    y.update_congruences();
#else
  if (!x.generators_are_minimized() && !x.minimize())
    // Minimizing found `x' empty.
    return true;
  if (!y.congruences_are_minimized())
    y.minimize();
#endif

  PPL_ASSERT(x.OK());
  PPL_ASSERT(y.OK());

  const Grid_Generator_System& gs = x.gen_sys;
  const Congruence_System& cgs = y.con_sys;

  const dimension_type num_rows = gs.num_rows();
  for (dimension_type i = num_rows; i-- > 0; )
    if (!cgs.satisfies_all_congruences(gs[i]))
      return false;

  // Inclusion holds.
  return true;
}

bool
PPL::Grid::bounds(const Linear_Expression& expr,
                  const char* method_call) const {
  // The dimension of `expr' must be at most the dimension of *this.
  if (space_dim < expr.space_dimension())
    throw_dimension_incompatible(method_call, "e", expr);

  // A zero-dimensional or empty grid bounds everything.
  if (space_dim == 0
      || marked_empty()
      || (!generators_are_up_to_date() && !update_generators()))
    return true;
  if (!generators_are_minimized() && !minimize())
    // Minimizing found `this' empty.
    return true;

  return bounds_no_check(expr);
}

bool
PPL::Grid::bounds_no_check(const Linear_Expression& expr) const {
  // The dimension of `expr' must be at most the dimension of *this.
  PPL_ASSERT(space_dim > 0 && space_dim >= expr.space_dimension());
  PPL_ASSERT(generators_are_minimized() && !marked_empty());

  // The generators are up to date and minimized.
  for (dimension_type i = gen_sys.num_rows(); i-- > 0; ) {
    const Grid_Generator& g = gen_sys[i];
    // Only lines and parameters in `*this' can cause `expr' to be
    // unbounded.
    if (g.is_line_or_parameter()) {
      const int sp_sign = Scalar_Products::homogeneous_sign(expr, g);
      if (sp_sign != 0)
        // `*this' does not bound `expr'.
        return false;
    }
  }
  return true;
}

bool
PPL::Grid::frequency_no_check(const Linear_Expression& expr,
                     Coefficient& freq_n, Coefficient& freq_d,
                     Coefficient& val_n, Coefficient& val_d) const {

  // The dimension of `expr' must be at most the dimension of *this.
  PPL_ASSERT(space_dim >= expr.space_dimension());
  PPL_ASSERT(generators_are_minimized() && !marked_empty());

  // The generators are up to date and minimized and the grid is non-empty.

  // If the grid is bounded for the expression `expr',
  // then `expr' has a constant value and the frequency is 0.
  if (bounds_no_check(expr)) {
    freq_n = 0;
    freq_d = 1;
    // Find the value of the constant expression.
    const Grid_Generator& point = gen_sys[0];
    val_d = point.divisor();
    Scalar_Products::homogeneous_assign(val_n, expr, point);
    val_n += expr.inhomogeneous_term() * val_d;
    // Reduce `val_n' and `val_d'.
    PPL_DIRTY_TEMP_COEFFICIENT(gcd);
    gcd_assign(gcd, val_n, val_d);
    exact_div_assign(val_n, val_n, gcd);
    exact_div_assign(val_d, val_d, gcd);
    return true;
  }

  // The frequency is the gcd of the scalar products of the parameters
  // in `gen_sys'.
  const dimension_type num_rows = gen_sys.num_rows();
  PPL_DIRTY_TEMP_COEFFICIENT(sp);
  freq_n = 0;

  // As the generators are minimized, `gen_sys[0]' is a point
  // and considered later.
  for (dimension_type row = 1; row < num_rows; ++row) {
    const Grid_Generator& gen = gen_sys[row];
    Scalar_Products::homogeneous_assign(sp, expr, gen);
    if (gen.is_line()) {
      if (sgn(sp) != 0)
          return false;
      continue;
    }
    // `gen' must be a parameter.
    PPL_ASSERT(gen.is_parameter());
    if (sgn(sp) != 0)
    gcd_assign(freq_n, freq_n, sp);
  }
  const Grid_Generator& point = gen_sys[0];
  PPL_ASSERT(point.is_point());

  // The denominator of the frequency and of the value is
  // the divisor for the generators.
  freq_d = point.divisor();
  val_d = freq_d;

  // As point is a grid generator, homogeneous_assign() must be used.
  Scalar_Products::homogeneous_assign(val_n, expr, point);
  val_n += expr.inhomogeneous_term() * val_d;

  // Reduce `val_n' by the frequency `freq_n'.
  val_n %= freq_n;

  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  // Reduce `freq_n' and `freq_d'.
  gcd_assign(gcd, freq_n, freq_d);
  exact_div_assign(freq_n, freq_n, gcd);
  exact_div_assign(freq_d, freq_d, gcd);

  // Reduce `val_n' and `val_d'.
  gcd_assign(gcd, val_n, val_d);
  exact_div_assign(val_n, val_n, gcd);
  exact_div_assign(val_d, val_d, gcd);

  return true;
}

bool
PPL::Grid::max_min(const Linear_Expression& expr,
                   const char* method_call,
                   Coefficient& ext_n, Coefficient& ext_d, bool& included,
                   Generator* point) const {
  if (bounds(expr, method_call)) {
    if (marked_empty())
      return false;
    if (space_dim == 0) {
      ext_n = 0;
      ext_d = 1;
      included = true;
      if (point != 0)
        *point = Generator::point();
      return true;
    }
    // Grid::bounds above ensures the generators are up to date.
    if (!generators_are_minimized()) {
      // Minimize the generator system.
      Grid& gr = const_cast<Grid&>(*this);
      gr.simplify(gr.gen_sys, gr.dim_kinds);
      gr.set_generators_minimized();
    }

    const Grid_Generator& gen = gen_sys[0];
    Scalar_Products::homogeneous_assign(ext_n, expr, gen);
    ext_n += expr.inhomogeneous_term();
    ext_d = gen.divisor();
    // Reduce ext_n and ext_d.
    PPL_DIRTY_TEMP_COEFFICIENT(gcd);
    gcd_assign(gcd, ext_n, ext_d);
    exact_div_assign(ext_n, ext_n, gcd);
    exact_div_assign(ext_d, ext_d, gcd);

    included = true;
    if (point != 0) {
      const Linear_Expression g_expr(gen.expression());
      *point = Generator::point(g_expr, gen.divisor());
    }
    return true;
  }
  return false;
}

void
PPL::Grid::set_zero_dim_univ() {
  status.set_zero_dim_univ();
  space_dim = 0;
  con_sys.clear();
  gen_sys.clear();
  gen_sys.insert(grid_point());
}

void
PPL::Grid::set_empty() {
  status.set_empty();

  // Replace gen_sys with an empty system of the right dimension.
  Grid_Generator_System gs(space_dim);
  gen_sys.m_swap(gs);

  // Extend the zero dim false congruence system to the appropriate
  // dimension and then swap it with `con_sys'.
  Congruence_System cgs(Congruence::zero_dim_false());
  cgs.set_space_dimension(space_dim);
  swap(con_sys, cgs);
}

void
PPL::Grid::update_congruences() const {
  // The caller must ensure that the generators are up to date.
  PPL_ASSERT(space_dim > 0);
  PPL_ASSERT(!marked_empty());
  PPL_ASSERT(!gen_sys.has_no_rows());
  PPL_ASSERT(gen_sys.space_dimension() > 0);

  Grid& gr = const_cast<Grid&>(*this);

  if (!generators_are_minimized())
    gr.simplify(gr.gen_sys, gr.dim_kinds);

  // `gen_sys' contained rows before being reduced, so it should
  // contain at least a single point afterward.
  PPL_ASSERT(!gen_sys.has_no_rows());

  // Populate `con_sys' with congruences characterizing the grid
  // described by `gen_sys'.
  gr.conversion(gr.gen_sys, gr.con_sys, gr.dim_kinds);

  // Both systems are minimized.
  gr.set_congruences_minimized();
  gr.set_generators_minimized();
}

bool
PPL::Grid::update_generators() const {
  PPL_ASSERT(space_dim > 0);
  PPL_ASSERT(!marked_empty());
  PPL_ASSERT(congruences_are_up_to_date());

  Grid& x = const_cast<Grid&>(*this);

  if (!congruences_are_minimized())
    // Either the system of congruences is consistent, or the grid is
    // empty.
    if (simplify(x.con_sys, x.dim_kinds)) {
      x.set_empty();
      return false;
    }

  // Populate gen_sys with generators characterizing the grid
  // described by con_sys.
  conversion(x.con_sys, x.gen_sys, x.dim_kinds);

  // Both systems are minimized.
  x.set_congruences_minimized();
  x.set_generators_minimized();
  return true;
}

bool
PPL::Grid::minimize() const {
  // 0-dimension and empty grids are already minimized.
  if (marked_empty())
    return false;
  if (space_dim == 0)
    return true;

  // Are both systems already minimized?
  if (congruences_are_minimized() && generators_are_minimized())
    return true;

  // Invoke update_generators, update_congruences or simplify,
  // depending on the state of the systems.
  if (congruences_are_up_to_date()) {
    if (generators_are_up_to_date()) {
      Grid& gr = const_cast<Grid&>(*this);
      // Only one of the systems can be minimized here.
      if (congruences_are_minimized()) {
        // Minimize the generator system.
        gr.simplify(gr.gen_sys, gr.dim_kinds);
        gr.set_generators_minimized();
      }
      else {
#ifndef NDEBUG
        // Both systems are up to date, and the empty case is handled
        // above, so the grid should contain points.
        bool empty = simplify(gr.con_sys, gr.dim_kinds);
        PPL_ASSERT(!empty);
#else
        simplify(gr.con_sys, gr.dim_kinds);
#endif
        gr.set_congruences_minimized();
        if (!generators_are_minimized()) {
          // Minimize the generator system.
          gr.simplify(gr.gen_sys, gr.dim_kinds);
          gr.set_generators_minimized();
        }
      }
    }
    else {
      // Updating the generators may reveal that `*this' is empty.
      const bool ret = update_generators();
      PPL_ASSERT(OK());
      return ret;
    }
  }
  else {
    PPL_ASSERT(generators_are_up_to_date());
    update_congruences();
  }
  PPL_ASSERT(OK());
  return true;
}

void
PPL::Grid::normalize_divisors(Grid_Generator_System& sys,
                              Grid_Generator_System& gen_sys) {
#ifndef NDEBUG
  const dimension_type num_rows = gen_sys.num_rows();
#endif
  PPL_ASSERT(num_rows > 0);

  // Find the first point in gen_sys.
  dimension_type row = 0;
  while (gen_sys[row].is_line_or_parameter()) {
    ++row;
    // gen_sys should have at least one point.
    PPL_ASSERT(row < num_rows);
  }
  const Grid_Generator& first_point = gen_sys[row];
  const Coefficient& gen_sys_divisor = first_point.divisor();

#ifndef NDEBUG
  // Check that the divisors in gen_sys are equal.
  for (dimension_type i = row + 1; i < num_rows; ++i) {
    const Grid_Generator& g = gen_sys[i];
    if (g.is_parameter_or_point())
      PPL_ASSERT(gen_sys_divisor == g.divisor());
  }
#endif // !defined(NDEBUG)

  PPL_DIRTY_TEMP_COEFFICIENT(divisor);
  divisor = gen_sys_divisor;
  // Adjust sys to include the gen_sys divisor.
  normalize_divisors(sys, divisor);
  if (divisor != gen_sys_divisor)
    // Adjust gen_sys to use the new divisor.
    //
    // The points and parameters in gen_sys share a common divisor
    // value, so the new divisor will be the LCM of this common
    // divisor and `divisor', hence the third argument.
    normalize_divisors(gen_sys, divisor, &first_point);
}

void
PPL::Grid::normalize_divisors(Grid_Generator_System& sys,
                              Coefficient& divisor,
                              const Grid_Generator* first_point) {
  PPL_ASSERT(divisor >= 0);
  if (sys.space_dimension() > 0 && divisor > 0) {
    const dimension_type num_rows = sys.num_rows();

    if (first_point != 0)
      lcm_assign(divisor, divisor, (*first_point).divisor());
    else {
      PPL_ASSERT(num_rows > 0);
      // Move to the first point or parameter.
      dimension_type row = 0;
      while (sys[row].is_line())
        if (++row == num_rows)
          // All rows are lines.
          return;

      // Calculate the LCM of the given divisor and the divisor of
      // every point or parameter.
      while (row < num_rows) {
        const Grid_Generator& g = sys[row];
        if (g.is_parameter_or_point())
          lcm_assign(divisor, divisor, g.divisor());
        ++row;
      }
    }

    // Represent every point and every parameter using the newly
    // calculated divisor.
    for (dimension_type i = num_rows; i-- > 0; )
      sys.sys.rows[i].scale_to_divisor(divisor);

    // Put the rows back into the linear system.
    PPL_ASSERT(sys.sys.OK());
  }
}

void
PPL::Grid::add_congruence_no_check(const Congruence& cg) {
  PPL_ASSERT(!marked_empty());
  PPL_ASSERT(space_dim >= cg.space_dimension());

  // Dealing with a zero-dimensional space grid first.
  if (space_dim == 0) {
    if (cg.is_inconsistent())
      set_empty();
    return;
  }

  if (!congruences_are_up_to_date())
    update_congruences();

  con_sys.insert(cg);

  clear_congruences_minimized();
  set_congruences_up_to_date();
  clear_generators_up_to_date();

  // Note: the congruence system may have become unsatisfiable, thus
  // we do not check for satisfiability.
  PPL_ASSERT(OK());
}

void
PPL::Grid::add_constraint_no_check(const Constraint& c) {
  PPL_ASSERT(!marked_empty());
  PPL_ASSERT(space_dim >= c.space_dimension());

  if (c.is_inequality()) {
    // Only trivial inequalities can be handled.
    if (c.is_inconsistent()) {
      set_empty();
      return;
    }
    if (c.is_tautological())
      return;
    // Non-trivial inequality constraints are not allowed.
    throw_invalid_constraint("add_constraint(c)", "c");
  }

  PPL_ASSERT(c.is_equality());
  const Congruence cg(c);
  add_congruence_no_check(cg);
}

void
PPL::Grid::refine_no_check(const Constraint& c) {
  PPL_ASSERT(!marked_empty());
  PPL_ASSERT(space_dim >= c.space_dimension());

  if (c.is_equality()) {
    const Congruence cg(c);
    add_congruence_no_check(cg);
  }
  else if (c.is_inconsistent())
    set_empty();
}

void
PPL::Grid::throw_invalid_argument(const char* method, const char* reason) {
  std::ostringstream s;
  s << "PPL::Grid::" << method << ":" << std::endl
    << reason << ".";
  throw std::invalid_argument(s.str());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* other_name,
                                        dimension_type other_dim) const {
  std::ostringstream s;
  s << "PPL::Grid::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension() << ", "
    << other_name << ".space_dimension() == " << other_dim << ".";
  throw std::invalid_argument(s.str());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* gr_name,
                                        const Grid& gr) const {
  throw_dimension_incompatible(method, gr_name, gr.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* le_name,
                                        const Linear_Expression& le) const {
  throw_dimension_incompatible(method, le_name, le.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* cg_name,
                                        const Congruence& cg) const {
  throw_dimension_incompatible(method, cg_name, cg.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* c_name,
                                        const Constraint& c) const {
  throw_dimension_incompatible(method, c_name, c.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* g_name,
                                        const Grid_Generator& g) const {
  throw_dimension_incompatible(method, g_name, g.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* g_name,
                                        const Generator& g) const {
  throw_dimension_incompatible(method, g_name, g.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* cgs_name,
                                        const Congruence_System& cgs) const {
  throw_dimension_incompatible(method, cgs_name, cgs.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* cs_name,
                                        const Constraint_System& cs) const {
  throw_dimension_incompatible(method, cs_name, cs.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* gs_name,
                                        const Grid_Generator_System& gs) const {
  throw_dimension_incompatible(method, gs_name, gs.space_dimension());
}

void
PPL::Grid::throw_dimension_incompatible(const char* method,
                                        const char* var_name,
                                        const Variable var) const {
  std::ostringstream s;
  s << "PPL::Grid::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension() << ", "
    << var_name << ".space_dimension() == " << var.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

void
PPL::Grid::
throw_dimension_incompatible(const char* method,
                             dimension_type required_space_dim) const {
  std::ostringstream s;
  s << "PPL::Grid::" << method << ":" << std::endl
    << "this->space_dimension() == " << space_dimension()
    << ", required space dimension == " << required_space_dim << ".";
  throw std::invalid_argument(s.str());
}

void
PPL::Grid::throw_invalid_constraint(const char* method,
                                    const char* c_name) {
  std::ostringstream s;
  s << "PPL::Grid::" << method << ":" << std::endl
    << c_name << " is not an equality constraint.";
  throw std::invalid_argument(s.str());
}

void
PPL::Grid::throw_invalid_constraints(const char* method,
                                    const char* cs_name) {
  std::ostringstream s;
  s << "PPL::Grid::" << method << ":" << std::endl
    << "the constraint system " << cs_name
    << " contains inequalities.";
  throw std::invalid_argument(s.str());
}

void
PPL::Grid::throw_invalid_generator(const char* method,
                                   const char* g_name) {
  std::ostringstream s;
  s << "PPL::Grid::" << method << ":" << std::endl
    << "*this is an empty grid and "
    << g_name << " is not a point.";
  throw std::invalid_argument(s.str());
}

void
PPL::Grid::throw_invalid_generators(const char* method,
                                    const char* gs_name) {
  std::ostringstream s;
  s << "PPL::Grid::" << method << ":" << std::endl
    << "*this is an empty grid and" << std::endl
    << "the non-empty generator system " << gs_name << " contains no points.";
  throw std::invalid_argument(s.str());
}
