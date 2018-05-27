/* Polyhedron class implementation
   (non-inline widening-related member functions).
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
#include "Polyhedron_defs.hh"
#include "BHRZ03_Certificate_defs.hh"
#include "Rational_Box.hh"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include "assert.hh"
#include <iostream>
#include <stdexcept>
#include <deque>

namespace PPL = Parma_Polyhedra_Library;

void
PPL::Polyhedron
::select_CH78_constraints(const Polyhedron& y,
                          Constraint_System& cs_selection) const {
  // Private method: the caller must ensure the following conditions.
  PPL_ASSERT(topology() == y.topology()
         && topology() == cs_selection.topology()
         && space_dim == y.space_dim);
  PPL_ASSERT(!marked_empty()
         && !has_pending_constraints()
         && generators_are_up_to_date());
  PPL_ASSERT(!y.marked_empty()
         && !y.has_something_pending()
         && y.constraints_are_minimized());

  // A constraint in `y.con_sys' is copied to `cs_selection'
  // if it is satisfied by all the generators of `gen_sys'.

  // Note: the loop index `i' goes upward to avoid reversing
  // the ordering of the chosen constraints.
  for (dimension_type i = 0, end = y.con_sys.num_rows(); i < end; ++i) {
    const Constraint& c = y.con_sys[i];
    if (gen_sys.satisfied_by_all_generators(c))
      cs_selection.insert(c);
  }
}

void
PPL::Polyhedron
::select_H79_constraints(const Polyhedron& y,
                         Constraint_System& cs_selected,
                         Constraint_System& cs_not_selected) const {
  // Private method: the caller must ensure the following conditions
  // (beside the inclusion `y <= x').
  PPL_ASSERT(topology() == y.topology()
         && topology() == cs_selected.topology()
         && topology() == cs_not_selected.topology());
  PPL_ASSERT(space_dim == y.space_dim);
  PPL_ASSERT(!marked_empty()
         && !has_pending_generators()
         && constraints_are_up_to_date());
  PPL_ASSERT(!y.marked_empty()
         && !y.has_something_pending()
         && y.constraints_are_minimized()
         && y.generators_are_up_to_date());

  // FIXME: this is a workaround for NNC polyhedra.
  if (!y.is_necessarily_closed()) {
    // Force strong minimization of constraints.
    y.strongly_minimize_constraints();
    // Recompute generators (without compromising constraint minimization).
    y.update_generators();
  }

  // Obtain a sorted copy of `y.sat_g'.
  if (!y.sat_g_is_up_to_date())
    y.update_sat_g();
  Bit_Matrix tmp_sat_g = y.sat_g;
  // Remove from `tmp_sat_g' the rows corresponding to tautologies
  // (i.e., the positivity or epsilon-bounding constraints):
  // this is needed in order to widen the polyhedron and not the
  // corresponding homogenized polyhedral cone.
  const Constraint_System& y_cs = y.con_sys;
  const dimension_type old_num_rows = y_cs.num_rows();
  dimension_type num_rows = old_num_rows;
  for (dimension_type i = 0; i < num_rows; ++i)
    if (y_cs[i].is_tautological()) {
      using std::swap;
      --num_rows;
      swap(tmp_sat_g[i], tmp_sat_g[num_rows]);
    }
  tmp_sat_g.remove_trailing_rows(old_num_rows - num_rows);
  tmp_sat_g.sort_rows();

  // A constraint in `con_sys' is copied to `cs_selected'
  // if its behavior with respect to `y.gen_sys' is the same
  // as that of another constraint in `y.con_sys'.
  // otherwise it is copied to `cs_not_selected'.
  // Namely, we check whether the saturation row `buffer'
  // (built starting from the given constraint and `y.gen_sys')
  // is a row of the saturation matrix `tmp_sat_g'.

  // CHECKME: the following comment is only applicable when `y.gen_sys'
  // is minimized. In that case, the comment suggests that it would be
  // possible to use a fast (but incomplete) redundancy test based on
  // the number of saturators in `buffer'.
  // NOTE: If the considered constraint of `con_sys' does not
  // satisfy the saturation rule (see Section \ref prelims), then
  // it will not appear in the resulting constraint system,
  // because `tmp_sat_g' is built starting from a minimized polyhedron.

  // The size of `buffer' will reach sat.num_columns() bits.
  Bit_Row buffer;
  // Note: the loop index `i' goes upward to avoid reversing
  // the ordering of the chosen constraints.
  for (dimension_type i = 0, end = con_sys.num_rows(); i < end; ++i) {
    const Constraint& ci = con_sys[i];
    // The saturation row `buffer' is built considering
    // the `i'-th constraint of the polyhedron `x' and
    // all the generators of the polyhedron `y'.
    buffer.clear();
    for (dimension_type j = y.gen_sys.num_rows(); j-- > 0; ) {
      const int sp_sgn = Scalar_Products::sign(ci, y.gen_sys[j]);
      // We are assuming that `y <= x'.
      PPL_ASSERT(sp_sgn >= 0
             || (!is_necessarily_closed()
                 && ci.is_strict_inequality()
                 && y.gen_sys[j].is_point()));
      if (sp_sgn > 0)
        buffer.set(j);
    }
    // We check whether `buffer' is a row of `tmp_sat_g',
    // exploiting its sortedness in order to have faster comparisons.
    if (tmp_sat_g.sorted_contains(buffer))
      cs_selected.insert(ci);
    else
      cs_not_selected.insert(ci);
  }
}

void
PPL::Polyhedron::H79_widening_assign(const Polyhedron& y, unsigned* tp) {
  Polyhedron& x = *this;
  // Topology compatibility check.
  const Topology topol = x.topology();
  if (topol != y.topology())
    throw_topology_incompatible("H79_widening_assign(y)", "y", y);
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("H79_widening_assign(y)", "y", y);

  // Assume `y' is contained in or equal to `x'.
  PPL_EXPECT_HEAVY(copy_contains(x, y));

  // If any argument is zero-dimensional or empty,
  // the H79-widening behaves as the identity function.
  if (x.space_dim == 0 || x.marked_empty() || y.marked_empty())
    return;

  // `y.gen_sys' should be in minimal form and
  // `y.sat_g' should be up-to-date.
  if (y.is_necessarily_closed()) {
    if (!y.minimize())
      // `y' is empty: the result is `x'.
      return;
  }
  else {
    // Dealing with a NNC polyhedron.
    // To obtain a correct reasoning when comparing
    // the constraints of `x' with the generators of `y',
    // we enforce the inclusion relation holding between
    // the two NNC polyhedra `x' and `y' (i.e., `y <= x')
    // to also hold for the corresponding eps-representations:
    // this is obtained by intersecting the two eps-representations.
    Polyhedron& yy = const_cast<Polyhedron&>(y);
    yy.intersection_assign(x);
    if (yy.is_empty())
      // The result is `x'.
      return;
  }

  // If we only have the generators of `x' and the dimensions of
  // the two polyhedra are the same, we can compute the standard
  // widening by using the specification in [CousotH78], therefore
  // avoiding converting from generators to constraints.
  if (x.has_pending_generators() || !x.constraints_are_up_to_date()) {
    Constraint_System CH78_cs(topol);
    x.select_CH78_constraints(y, CH78_cs);

    if (CH78_cs.num_rows() == y.con_sys.num_rows()) {
      // Having selected all the constraints, the result is `y'.
      x = y;
      return;
    }
    // Otherwise, check if `x' and `y' have the same dimension.
    // Note that `y.con_sys' is minimized and `CH78_cs' has no redundant
    // constraints, since it is a subset of the former.
    else if (CH78_cs.num_equalities() == y.con_sys.num_equalities()) {
      // Let `x' be defined by the constraints in `CH78_cs'.
      Polyhedron CH78(topol, x.space_dim, UNIVERSE);
      CH78.add_recycled_constraints(CH78_cs);

      // Check whether we are using the widening-with-tokens technique
      // and there still are tokens available.
      if (tp != 0 && *tp > 0) {
        // There are tokens available. If `CH78' is not a subset of `x',
        // then it is less precise and we use one of the available tokens.
        if (!x.contains(CH78))
          --(*tp);
      }
      else
        // No tokens.
        x.m_swap(CH78);
      PPL_ASSERT_HEAVY(x.OK(true));
      return;
    }
  }

  // As the dimension of `x' is strictly greater than the dimension of `y',
  // we have to compute the standard widening by selecting a subset of
  // the constraints of `x'.
  // `x.con_sys' is just required to be up-to-date, because:
  // - if `x.con_sys' is unsatisfiable, then by assumption
  //   also `y' is empty, so that the resulting polyhedron is `x';
  // - redundant constraints in `x.con_sys' do not affect the result
  //   of the widening, because if they are selected they will be
  //   redundant even in the result.
  if (has_pending_generators())
    process_pending_generators();
  else if (!x.constraints_are_up_to_date())
    x.update_constraints();

  // Copy into `H79_cs' the constraints of `x' that are common to `y',
  // according to the definition of the H79 widening.
  Constraint_System H79_cs(topol);
  Constraint_System x_minus_H79_cs(topol);
  x.select_H79_constraints(y, H79_cs, x_minus_H79_cs);

  if (x_minus_H79_cs.has_no_rows())
    // We selected all of the constraints of `x',
    // thus the result of the widening is `x'.
    return;
  else {
    // We selected a strict subset of the constraints of `x'.
    // NOTE: as `x.con_sys' was not necessarily in minimal form,
    // this does not imply that the result strictly includes `x'.
    // Let `H79' be defined by the constraints in `H79_cs'.
    Polyhedron H79(topol, x.space_dim, UNIVERSE);
    H79.add_recycled_constraints(H79_cs);

    // Check whether we are using the widening-with-tokens technique
    // and there still are tokens available.
    if (tp != 0 && *tp > 0) {
      // There are tokens available. If `H79' is not a subset of `x',
      // then it is less precise and we use one of the available tokens.
      if (!x.contains(H79))
        --(*tp);
    }
    else
      // No tokens.
      x.m_swap(H79);
    PPL_ASSERT_HEAVY(x.OK(true));
  }
}

void
PPL::Polyhedron::limited_H79_extrapolation_assign(const Polyhedron& y,
                                                  const Constraint_System& cs,
                                                  unsigned* tp) {
  Polyhedron& x = *this;

  const dimension_type cs_num_rows = cs.num_rows();
  // If `cs' is empty, we fall back to ordinary, non-limited widening.
  if (cs_num_rows == 0) {
    x.H79_widening_assign(y, tp);
    return;
  }

  // Topology compatibility check.
  if (x.is_necessarily_closed()) {
    if (!y.is_necessarily_closed())
      throw_topology_incompatible("limited_H79_extrapolation_assign(y, cs)",
                                  "y", y);
    if (cs.has_strict_inequalities())
      throw_topology_incompatible("limited_H79_extrapolation_assign(y, cs)",
                                  "cs", cs);
  }
  else if (y.is_necessarily_closed())
    throw_topology_incompatible("limited_H79_extrapolation_assign(y, cs)",
                                "y", y);

  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("limited_H79_extrapolation_assign(y, cs)",
                                 "y", y);
  // `cs' must be dimension-compatible with the two polyhedra.
  const dimension_type cs_space_dim = cs.space_dimension();
  if (x.space_dim < cs_space_dim)
    throw_dimension_incompatible("limited_H79_extrapolation_assign(y, cs)",
                                 "cs", cs);

  // Assume `y' is contained in or equal to `x'.
  PPL_EXPECT_HEAVY(copy_contains(x, y));

  if (y.marked_empty())
    return;
  if (x.marked_empty())
    return;

  // The limited H79-widening between two polyhedra in a
  // zero-dimensional space is a polyhedron in a zero-dimensional
  // space, too.
  if (x.space_dim == 0)
    return;

  if (!y.minimize())
    // We have just discovered that `y' is empty.
    return;

  // Update the generators of `x': these are used to select,
  // from the constraints in `cs', those that must be added
  // to the resulting polyhedron.
  if ((x.has_pending_constraints() && !x.process_pending_constraints())
      || (!x.generators_are_up_to_date() && !x.update_generators()))
    // We have just discovered that `x' is empty.
    return;

  Constraint_System new_cs;
  // The constraints to be added must be satisfied by all the
  // generators of `x'.  We can disregard `y' because `y <= x'.
  const Generator_System& x_gen_sys = x.gen_sys;
  // Iterate upwards here so as to keep the relative ordering of constraints.
  // Not really an issue: just aesthetics.
  for (dimension_type i = 0; i < cs_num_rows; ++i) {
    const Constraint& c = cs[i];
    if (x_gen_sys.satisfied_by_all_generators(c))
      new_cs.insert(c);
  }
  x.H79_widening_assign(y, tp);
  x.add_recycled_constraints(new_cs);
  PPL_ASSERT_HEAVY(OK());
}

void
PPL::Polyhedron::bounded_H79_extrapolation_assign(const Polyhedron& y,
                                                  const Constraint_System& cs,
                                                  unsigned* tp) {
  Rational_Box x_box(*this, ANY_COMPLEXITY);
  const Rational_Box y_box(y, ANY_COMPLEXITY);
  x_box.CC76_widening_assign(y_box);
  limited_H79_extrapolation_assign(y, cs, tp);
  Constraint_System x_box_cs = x_box.constraints();
  add_recycled_constraints(x_box_cs);
}

bool
PPL::Polyhedron
::BHRZ03_combining_constraints(const Polyhedron& y,
                               const BHRZ03_Certificate& y_cert,
                               const Polyhedron& H79,
                               const Constraint_System& x_minus_H79_cs) {
  Polyhedron& x = *this;
  // It is assumed that `y <= x <= H79'.
  PPL_ASSERT(x.topology() == y.topology()
         && x.topology() == H79.topology()
         && x.topology() == x_minus_H79_cs.topology());
  PPL_ASSERT(x.space_dim == y.space_dim
         && x.space_dim == H79.space_dim
         && x.space_dim == x_minus_H79_cs.space_dimension());
  PPL_ASSERT(!x.marked_empty() && !x.has_something_pending()
         && x.constraints_are_minimized() && x.generators_are_minimized());
  PPL_ASSERT(!y.marked_empty() && !y.has_something_pending()
         && y.constraints_are_minimized() && y.generators_are_minimized());
  PPL_ASSERT(!H79.marked_empty() && !H79.has_something_pending()
         && H79.constraints_are_minimized() && H79.generators_are_minimized());

  // We will choose from `x_minus_H79_cs' many subsets of constraints,
  // that will be collected (one at a time) in `combining_cs'.
  // For each group collected, we compute an average constraint,
  // that will be stored in `new_cs'.

  // There is no point in applying this technique when `x_minus_H79_cs'
  // has one constraint at most (no ``new'' constraint can be computed).
  const dimension_type x_minus_H79_cs_num_rows = x_minus_H79_cs.num_rows();
  if (x_minus_H79_cs_num_rows <= 1)
    return false;

  const Topology topol = x.topology();
  Constraint_System combining_cs(topol);
  Constraint_System new_cs(topol);

  // Consider the points that belong to both `x.gen_sys' and `y.gen_sys'.
  // For NNC polyhedra, the role of points is played by closure points.
  const bool closed = x.is_necessarily_closed();
  for (dimension_type i = y.gen_sys.num_rows(); i-- > 0; ) {
    const Generator& g = y.gen_sys[i];
    if ((g.is_point() && closed) || (g.is_closure_point() && !closed)) {
      // If in `H79.con_sys' there is already an inequality constraint
      // saturating this point, then there is no need to produce another
      // constraint.
      bool lies_on_the_boundary_of_H79 = false;
      const Constraint_System& H79_cs = H79.con_sys;
      for (dimension_type j = H79_cs.num_rows(); j-- > 0; ) {
        const Constraint& c = H79_cs[j];
        if (c.is_inequality() && Scalar_Products::sign(c, g) == 0) {
          lies_on_the_boundary_of_H79 = true;
          break;
        }
      }
      if (lies_on_the_boundary_of_H79)
        continue;

      // Consider all the constraints in `x_minus_H79_cs'
      // that are saturated by the point `g'.
      combining_cs.clear();
      for (dimension_type j = x_minus_H79_cs_num_rows; j-- > 0; ) {
        const Constraint& c = x_minus_H79_cs[j];
        if (Scalar_Products::sign(c, g) == 0)
          combining_cs.insert(c);
      }
      // Build a new constraint by combining all the chosen constraints.
      const dimension_type combining_cs_num_rows = combining_cs.num_rows();
      if (combining_cs_num_rows > 0) {
        if (combining_cs_num_rows == 1)
          // No combination is needed.
          new_cs.insert(combining_cs[0]);
        else {
          Linear_Expression e(0);
          bool strict_inequality = false;
          for (dimension_type h = combining_cs_num_rows; h-- > 0; ) {
            if (combining_cs[h].is_strict_inequality())
              strict_inequality = true;
            e += Linear_Expression(combining_cs[h].expression());
          }

          if (!e.all_homogeneous_terms_are_zero()) {
            if (strict_inequality)
              new_cs.insert(e > 0);
            else
              new_cs.insert(e >= 0);
          }
        }
      }
    }
  }

  // If none of the collected constraints strictly intersects `H79',
  // then the technique was unsuccessful.
  bool improves_upon_H79 = false;
  const Poly_Con_Relation si = Poly_Con_Relation::strictly_intersects();
  for (dimension_type i = new_cs.num_rows(); i-- > 0; )
    if (H79.relation_with(new_cs[i]) == si) {
      improves_upon_H79 = true;
      break;
    }
  if (!improves_upon_H79)
    return false;

  // The resulting polyhedron is obtained by adding the constraints
  // in `new_cs' to polyhedron `H79'.
  Polyhedron result = H79;
  result.add_recycled_constraints(new_cs);
  // Force minimization.
  result.minimize();

  // Check for stabilization with respect to `y_cert' and improvement
  // over `H79'.
  if (y_cert.is_stabilizing(result) && !result.contains(H79)) {
    // The technique was successful.
    x.m_swap(result);
    PPL_ASSERT_HEAVY(x.OK(true));
    return true;
  }
  else
    // The technique was unsuccessful.
    return false;
}

bool
PPL::Polyhedron::BHRZ03_evolving_points(const Polyhedron& y,
                                        const BHRZ03_Certificate& y_cert,
                                        const Polyhedron& H79) {
  Polyhedron& x = *this;
  // It is assumed that `y <= x <= H79'.
  PPL_ASSERT(x.topology() == y.topology()
         && x.topology() == H79.topology());
  PPL_ASSERT(x.space_dim == y.space_dim
         && x.space_dim == H79.space_dim);
  PPL_ASSERT(!x.marked_empty() && !x.has_something_pending()
         && x.constraints_are_minimized() && x.generators_are_minimized());
  PPL_ASSERT(!y.marked_empty() && !y.has_something_pending()
         && y.constraints_are_minimized() && y.generators_are_minimized());
  PPL_ASSERT(!H79.marked_empty() && !H79.has_something_pending()
         && H79.constraints_are_minimized() && H79.generators_are_minimized());

  // For each point in `x.gen_sys' that is not in `y',
  // this technique tries to identify a set of rays that:
  //  - are included in polyhedron `H79';
  //  - when added to `y' will subsume the point.
  Generator_System candidate_rays;

  const dimension_type x_gen_sys_num_rows = x.gen_sys.num_rows();
  const dimension_type y_gen_sys_num_rows = y.gen_sys.num_rows();
  const bool closed = x.is_necessarily_closed();
  for (dimension_type i = x_gen_sys_num_rows; i-- > 0; ) {
    const Generator& g1 = x.gen_sys[i];
    // For C polyhedra, we choose a point of `x.gen_sys'
    // that is not included in `y'.
    // In the case of NNC polyhedra, we can restrict attention to
    // closure points (considering also points will only add redundancy).
    if (((g1.is_point() && closed) || (g1.is_closure_point() && !closed))
        && y.relation_with(g1) == Poly_Gen_Relation::nothing()) {
      // For each point (resp., closure point) `g2' in `y.gen_sys',
      // where `g1' and `g2' are different,
      // build the candidate ray `g1 - g2'.
      for (dimension_type j = y_gen_sys_num_rows; j-- > 0; ) {
        const Generator& g2 = y.gen_sys[j];
        if ((g2.is_point() && closed)
            || (g2.is_closure_point() && !closed)) {
          PPL_ASSERT(compare(g1, g2) != 0);
          Generator ray_from_g2_to_g1 = g1;
          ray_from_g2_to_g1.linear_combine(g2, 0);
          candidate_rays.insert(ray_from_g2_to_g1);
        }
      }
    }
  }

  // Be non-intrusive.
  Polyhedron result = x;
  result.add_recycled_generators(candidate_rays);
  result.intersection_assign(H79);
  // Force minimization.
  result.minimize();

  // Check for stabilization with respect to `y_cert' and improvement
  // over `H79'.
  if (y_cert.is_stabilizing(result) && !result.contains(H79)) {
    // The technique was successful.
    x.m_swap(result);
    PPL_ASSERT_HEAVY(x.OK(true));
    return true;
  }
  else
    // The technique was unsuccessful.
    return false;
}

void
PPL::Polyhedron::modify_according_to_evolution(Linear_Expression& ray,
                                               const Linear_Expression& x,
                                               const Linear_Expression& y) {
  PPL_DIRTY_TEMP_COEFFICIENT(tmp);
  std::deque<bool> considered(x.space_dimension());
  Linear_Expression::const_iterator x_end = x.end();
  Linear_Expression::const_iterator y_end = y.end();
  Linear_Expression::const_iterator y_k = y.begin();
  for (Linear_Expression::const_iterator x_k = x.begin(); x_k != x_end; ++x_k) {
    const Variable k_var = x_k.variable();
    const dimension_type k = k_var.id();
    if (considered[k])
      continue;

    while (y_k != y_end && y_k.variable().id() < k)
      ++y_k;

    if (y_k == y_end)
      break;

    const Variable y_k_var = y_k.variable();

    // Note that y_k_var.id() may be greater than k.

    Linear_Expression::const_iterator y_h = y_k;
    // Do *not* increment y_h, since it may be after k already.
    Linear_Expression::const_iterator x_h = x_k;
    ++x_h;
    for ( ; x_h != x_end; ++x_h) {
      const dimension_type h = x_h.variable().id();
      if (considered[h])
        continue;

      while (y_h != y_end && y_h.variable().id() < h)
        ++y_h;

      // Note that y_h may be y_end, and y_h.variable().id() may not be k.

      if (y_h != y_end && y_h.variable().id() == h)
        tmp = (*x_k) * (*y_h);
      else
        tmp = 0;

      if (y_k_var.id() == k) {
        // The following line optimizes the computation of
        // <CODE> tmp -= x[h] * y[k]; </CODE>
        Parma_Polyhedra_Library::sub_mul_assign(tmp, *x_h, *y_k);
      }

      const int clockwise = sgn(tmp);
      const int first_or_third_quadrant = sgn(*x_k) * sgn(*x_h);
      switch (clockwise * first_or_third_quadrant) {
      case -1:
        ray.set_coefficient(k_var, Coefficient_zero());
        considered[k] = true;
        break;
      case 1:
        ray.set_coefficient(Variable(h), Coefficient_zero());
        considered[h] = true;
        break;
      default:
        break;
      }
    }
  }
  ray.normalize();
}

bool
PPL::Polyhedron::BHRZ03_evolving_rays(const Polyhedron& y,
                                      const BHRZ03_Certificate& y_cert,
                                      const Polyhedron& H79) {
  Polyhedron& x = *this;
  // It is assumed that `y <= x <= H79'.
  PPL_ASSERT(x.topology() == y.topology()
         && x.topology() == H79.topology());
  PPL_ASSERT(x.space_dim == y.space_dim
         && x.space_dim == H79.space_dim);
  PPL_ASSERT(!x.marked_empty() && !x.has_something_pending()
         && x.constraints_are_minimized() && x.generators_are_minimized());
  PPL_ASSERT(!y.marked_empty() && !y.has_something_pending()
         && y.constraints_are_minimized() && y.generators_are_minimized());
  PPL_ASSERT(!H79.marked_empty() && !H79.has_something_pending()
         && H79.constraints_are_minimized() && H79.generators_are_minimized());

  const dimension_type x_gen_sys_num_rows = x.gen_sys.num_rows();
  const dimension_type y_gen_sys_num_rows = y.gen_sys.num_rows();

  // Candidate rays are kept in a temporary generator system.
  Generator_System candidate_rays;
  for (dimension_type i = x_gen_sys_num_rows; i-- > 0; ) {
    const Generator& x_g = x.gen_sys[i];
    // We choose a ray of `x' that does not belong to `y'.
    if (x_g.is_ray() && y.relation_with(x_g) == Poly_Gen_Relation::nothing()) {
      for (dimension_type j = y_gen_sys_num_rows; j-- > 0; ) {
        const Generator& y_g = y.gen_sys[j];
        if (y_g.is_ray()) {
          Generator new_ray(x_g);
          // Modify `new_ray' according to the evolution of `x_g' with
          // respect to `y_g'.
          modify_according_to_evolution(new_ray.expr, x_g.expr, y_g.expr);
          PPL_ASSERT(new_ray.OK());
          candidate_rays.insert(new_ray);
        }
      }
    }
  }

  // If there are no candidate rays, we cannot obtain stabilization.
  if (candidate_rays.has_no_rows())
    return false;

  // Be non-intrusive.
  Polyhedron result = x;
  result.add_recycled_generators(candidate_rays);
  result.intersection_assign(H79);
  // Force minimization.
  result.minimize();

  // Check for stabilization with respect to `y' and improvement over `H79'.
  if (y_cert.is_stabilizing(result) && !result.contains(H79)) {
    // The technique was successful.
    x.m_swap(result);
    PPL_ASSERT_HEAVY(x.OK(true));
    return true;
  }
  else
    // The technique was unsuccessful.
    return false;
}

void
PPL::Polyhedron::BHRZ03_widening_assign(const Polyhedron& y, unsigned* tp) {
  Polyhedron& x = *this;
  // Topology compatibility check.
  if (x.topology() != y.topology())
    throw_topology_incompatible("BHRZ03_widening_assign(y)", "y", y);
  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("BHRZ03_widening_assign(y)", "y", y);

  // Assume `y' is contained in or equal to `x'.
  PPL_EXPECT_HEAVY(copy_contains(x, y));

  // If any argument is zero-dimensional or empty,
  // the BHRZ03-widening behaves as the identity function.
  if (x.space_dim == 0 || x.marked_empty() || y.marked_empty())
    return;

  // `y.con_sys' and `y.gen_sys' should be in minimal form.
  if (!y.minimize())
    // `y' is empty: the result is `x'.
    return;
  // `x.con_sys' and `x.gen_sys' should be in minimal form.
  x.minimize();

  // Compute certificate info for polyhedron `y'.
  const BHRZ03_Certificate y_cert(y);

  // If the iteration is stabilizing, the resulting polyhedron is `x'.
  // At this point, also check if the two polyhedra are the same
  // (exploiting the knowledge that `y <= x').
  if (y_cert.is_stabilizing(x) || y.contains(x)) {
    PPL_ASSERT_HEAVY(OK());
    return;
  }

  // Here the iteration is not immediately stabilizing.
  // If we are using the widening-with-tokens technique and
  // there are tokens available, use one of them and return `x'.
  if (tp != 0 && *tp > 0) {
    --(*tp);
    PPL_ASSERT_HEAVY(OK());
    return;
  }

  // Copy into `H79_cs' the constraints that are common to `x' and `y',
  // according to the definition of the H79 widening.
  // The other ones are copied into `x_minus_H79_cs'.
  const Topology topol = x.topology();
  Constraint_System H79_cs(topol);
  Constraint_System x_minus_H79_cs(topol);
  x.select_H79_constraints(y, H79_cs, x_minus_H79_cs);

  // We cannot have selected all of the rows, since otherwise
  // the iteration should have been immediately stabilizing.
  PPL_ASSERT(!x_minus_H79_cs.has_no_rows());
  // Be careful to obtain the right space dimension
  // (because `H79_cs' may be empty).
  Polyhedron H79(topol, x.space_dim, UNIVERSE);
  H79.add_recycled_constraints(H79_cs);
  // Force minimization.
  H79.minimize();

  // NOTE: none of the following widening heuristics is intrusive:
  // they will modify `x' only when returning successfully.
  if (x.BHRZ03_combining_constraints(y, y_cert, H79, x_minus_H79_cs))
    return;

  PPL_ASSERT_HEAVY(H79.OK() && x.OK() && y.OK());

  if (x.BHRZ03_evolving_points(y, y_cert, H79))
    return;

  PPL_ASSERT_HEAVY(H79.OK() && x.OK() && y.OK());

  if (x.BHRZ03_evolving_rays(y, y_cert, H79))
    return;

  PPL_ASSERT_HEAVY(H79.OK() && x.OK() && y.OK());

  // No previous technique was successful: fall back to the H79 widening.
  x.m_swap(H79);
  PPL_ASSERT_HEAVY(x.OK(true));

  // The H79 widening is always stabilizing.
  PPL_ASSERT(y_cert.is_stabilizing(x));
}

void
PPL::Polyhedron
::limited_BHRZ03_extrapolation_assign(const Polyhedron& y,
                                      const Constraint_System& cs,
                                      unsigned* tp) {
  Polyhedron& x = *this;
  const dimension_type cs_num_rows = cs.num_rows();
  // If `cs' is empty, we fall back to ordinary, non-limited widening.
  if (cs_num_rows == 0) {
    x.BHRZ03_widening_assign(y, tp);
    return;
  }

  // Topology compatibility check.
  if (x.is_necessarily_closed()) {
    if (!y.is_necessarily_closed())
      throw_topology_incompatible("limited_BHRZ03_extrapolation_assign(y, cs)",
                                  "y", y);
    if (cs.has_strict_inequalities())
      throw_topology_incompatible("limited_BHRZ03_extrapolation_assign(y, cs)",
                                  "cs", cs);
  }
  else if (y.is_necessarily_closed())
    throw_topology_incompatible("limited_BHRZ03_extrapolation_assign(y, cs)",
                                "y", y);

  // Dimension-compatibility check.
  if (x.space_dim != y.space_dim)
    throw_dimension_incompatible("limited_BHRZ03_extrapolation_assign(y, cs)",
                                 "y", y);
  // `cs' must be dimension-compatible with the two polyhedra.
  const dimension_type cs_space_dim = cs.space_dimension();
  if (x.space_dim < cs_space_dim)
    throw_dimension_incompatible("limited_BHRZ03_extrapolation_assign(y, cs)",
                                 "cs", cs);

  // Assume `y' is contained in or equal to `x'.
  PPL_EXPECT_HEAVY(copy_contains(x, y));

  if (y.marked_empty())
    return;
  if (x.marked_empty())
    return;

  // The limited BHRZ03-widening between two polyhedra in a
  // zero-dimensional space is a polyhedron in a zero-dimensional
  // space, too.
  if (x.space_dim == 0)
    return;

  if (!y.minimize())
    // We have just discovered that `y' is empty.
    return;

  // Update the generators of `x': these are used to select,
  // from the constraints in `cs', those that must be added
  // to the resulting polyhedron.
  if ((x.has_pending_constraints() && !x.process_pending_constraints())
      || (!x.generators_are_up_to_date() && !x.update_generators()))
    // We have just discovered that `x' is empty.
    return;

  Constraint_System new_cs;
  // The constraints to be added must be satisfied by all the
  // generators of `x'. We can disregard `y' because `y <= x'.
  const Generator_System& x_gen_sys = x.gen_sys;
  // Iterate upwards here so as to keep the relative ordering of constraints.
  // Not really an issue: just aesthetics.
  for (dimension_type i = 0; i < cs_num_rows; ++i) {
    const Constraint& c = cs[i];
    if (x_gen_sys.satisfied_by_all_generators(c))
      new_cs.insert(c);
  }
  x.BHRZ03_widening_assign(y, tp);
  x.add_recycled_constraints(new_cs);
  PPL_ASSERT_HEAVY(OK());
}

void
PPL::Polyhedron
::bounded_BHRZ03_extrapolation_assign(const Polyhedron& y,
                                      const Constraint_System& cs,
                                      unsigned* tp) {
  Rational_Box x_box(*this, ANY_COMPLEXITY);
  const Rational_Box y_box(y, ANY_COMPLEXITY);
  x_box.CC76_widening_assign(y_box);
  limited_BHRZ03_extrapolation_assign(y, cs, tp);
  Constraint_System x_box_cs = x_box.constraints();
  add_recycled_constraints(x_box_cs);
}
