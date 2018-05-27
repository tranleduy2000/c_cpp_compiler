/* Octagonal_Shape class implementation: non-inline template functions.
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

#ifndef PPL_Octagonal_Shape_templates_hh
#define PPL_Octagonal_Shape_templates_hh 1

#include "Generator_System_defs.hh"
#include "Generator_System_inlines.hh"
#include "Congruence_System_defs.hh"
#include "Congruence_System_inlines.hh"
#include "Interval_defs.hh"
#include "Linear_Form_defs.hh"
#include "meta_programming.hh"
#include "assert.hh"
#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace Parma_Polyhedra_Library {

template <typename T>
Octagonal_Shape<T>::Octagonal_Shape(const Polyhedron& ph,
                                    const Complexity_Class complexity)
  : matrix(0), space_dim(0), status() {
  const dimension_type num_dimensions = ph.space_dimension();

  if (ph.marked_empty()) {
    *this = Octagonal_Shape(num_dimensions, EMPTY);
    return;
  }

  if (num_dimensions == 0) {
    *this = Octagonal_Shape(num_dimensions, UNIVERSE);
    return;
  }

  // Build from generators when we do not care about complexity
  // or when the process has polynomial complexity.
  if (complexity == ANY_COMPLEXITY
      || (!ph.has_pending_constraints() && ph.generators_are_up_to_date())) {
    *this = Octagonal_Shape(ph.generators());
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
      *this = Octagonal_Shape(num_dimensions, UNIVERSE);
      return;
    }
  }

  // See if there is at least one inconsistent constraint in `ph.con_sys'.
  for (Constraint_System::const_iterator i = ph.con_sys.begin(),
         cs_end = ph.con_sys.end(); i != cs_end; ++i)
    if (i->is_inconsistent()) {
      *this = Octagonal_Shape(num_dimensions, EMPTY);
      return;
    }

  // If `complexity' allows it, use simplex to derive the exact (modulo
  // the fact that our OSs are topologically closed) variable bounds.
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
      *this = Octagonal_Shape<T>(num_dimensions, EMPTY);
      return;
    }

    // Start with a universe OS that will be refined by the simplex.
    *this = Octagonal_Shape<T>(num_dimensions, UNIVERSE);
    // Get all the upper bounds.
    Generator g(point());
    PPL_DIRTY_TEMP_COEFFICIENT(numer);
    PPL_DIRTY_TEMP_COEFFICIENT(denom);
    for (dimension_type i = 0; i < num_dimensions; ++i) {
      Variable x(i);
      // Evaluate optimal upper bound for `x <= ub'.
      lp.set_objective_function(x);
      if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
        g = lp.optimizing_point();
        lp.evaluate_objective_function(g, numer, denom);
        numer *= 2;
        div_round_up(matrix[2*i + 1][2*i], numer, denom);
      }
      // Evaluate optimal upper bounds for `x + y <= ub'.
      for (dimension_type j = 0; j < i; ++j) {
        Variable y(j);
        lp.set_objective_function(x + y);
        if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
          g = lp.optimizing_point();
          lp.evaluate_objective_function(g, numer, denom);
          div_round_up(matrix[2*i + 1][2*j], numer, denom);
        }
      }
      // Evaluate optimal upper bound for `x - y <= ub'.
      for (dimension_type j = 0; j < num_dimensions; ++j) {
        if (i == j)
          continue;
        Variable y(j);
        lp.set_objective_function(x - y);
        if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
          g = lp.optimizing_point();
          lp.evaluate_objective_function(g, numer, denom);
          div_round_up(((i < j) ?
                        matrix[2*j][2*i]
                        : matrix[2*i + 1][2*j + 1]),
                       numer, denom);
        }
      }
      // Evaluate optimal upper bound for `y - x <= ub'.
      for (dimension_type j = 0; j < num_dimensions; ++j) {
        if (i == j)
          continue;
        Variable y(j);
        lp.set_objective_function(x - y);
        if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
          g = lp.optimizing_point();
          lp.evaluate_objective_function(g, numer, denom);
          div_round_up(((i < j)
                        ? matrix[2*j][2*i]
                        : matrix[2*i + 1][2*j + 1]),
                       numer, denom);
        }
      }
      // Evaluate optimal upper bound for `-x - y <= ub'.
      for (dimension_type j = 0; j < i; ++j) {
        Variable y(j);
        lp.set_objective_function(-x - y);
        if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
          g = lp.optimizing_point();
          lp.evaluate_objective_function(g, numer, denom);
          div_round_up(matrix[2*i][2*j + 1], numer, denom);
        }
      }
      // Evaluate optimal upper bound for `-x <= ub'.
      lp.set_objective_function(-x);
      if (lp.solve() == OPTIMIZED_MIP_PROBLEM) {
        g = lp.optimizing_point();
        lp.evaluate_objective_function(g, numer, denom);
        numer *= 2;
        div_round_up(matrix[2*i][2*i + 1], numer, denom);
      }
    }
    set_strongly_closed();
    PPL_ASSERT(OK());
    return;
  }

  // Extract easy-to-find bounds from constraints.
  PPL_ASSERT(complexity == POLYNOMIAL_COMPLEXITY);
  *this = Octagonal_Shape(num_dimensions, UNIVERSE);
  refine_with_constraints(ph.constraints());
}

template <typename T>
Octagonal_Shape<T>::Octagonal_Shape(const Generator_System& gs)
  : matrix(gs.space_dimension()),
    space_dim(gs.space_dimension()),
    status() {
  const Generator_System::const_iterator gs_begin = gs.begin();
  const Generator_System::const_iterator gs_end = gs.end();
  if (gs_begin == gs_end) {
    // An empty generator system defines the empty polyhedron.
    set_empty();
    return;
  }

  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  typename OR_Matrix<N>::row_iterator mat_begin = matrix.row_begin();

  PPL_DIRTY_TEMP(N, tmp);
  bool mat_initialized = false;
  bool point_seen = false;
  // Going through all the points and closure points.
  for (Generator_System::const_iterator k = gs_begin; k != gs_end; ++k) {
    const Generator& g = *k;
    switch (g.type()) {
    case Generator::POINT:
      point_seen = true;
      // Intentionally fall through.
    case Generator::CLOSURE_POINT:
      if (!mat_initialized) {
        // When handling the first (closure) point, we initialize the matrix.
        mat_initialized = true;
        const Coefficient& d = g.divisor();
        // TODO: This can be optimized more, if needed, exploiting the
        // (possible) sparseness of g. Also consider if OR_Matrix should be
        // sparse, too.
        for (dimension_type i = 0; i < space_dim; ++i) {
          const Coefficient& g_i = g.coefficient(Variable(i));
          const dimension_type di = 2*i;
          row_reference x_i = *(mat_begin + di);
          row_reference x_ii = *(mat_begin + (di + 1));
          for (dimension_type j = 0; j < i; ++j) {
            const Coefficient& g_j = g.coefficient(Variable(j));
            const dimension_type dj = 2*j;
            // Set for any point the hyperplanes passing in the point
            // and having the octagonal gradient.
            // Let be P = [P_1, P_2, ..., P_n] point.
            // Hyperplanes: X_i - X_j = P_i - P_j.
            div_round_up(x_i[dj], g_j - g_i, d);
            div_round_up(x_ii[dj + 1], g_i - g_j, d);
            // Hyperplanes: X_i + X_j = P_i + P_j.
            div_round_up(x_i[dj + 1], -g_j - g_i, d);
            div_round_up(x_ii[dj], g_i + g_j, d);
          }
          // Hyperplanes: X_i = P_i.
          div_round_up(x_i[di + 1], -g_i - g_i, d);
          div_round_up(x_ii[di], g_i + g_i, d);
        }
      }
      else {
        // This is not the first point: the matrix already contains
        // valid values and we must compute maxima.
        const Coefficient& d = g.divisor();
        // TODO: This can be optimized more, if needed, exploiting the
        // (possible) sparseness of g. Also consider if OR_Matrix should be
        // sparse, too.
        for (dimension_type i = 0; i < space_dim; ++i) {
          const Coefficient& g_i = g.coefficient(Variable(i));
          const dimension_type di = 2*i;
          row_reference x_i = *(mat_begin + di);
          row_reference x_ii = *(mat_begin + (di + 1));
          for (dimension_type j = 0; j < i; ++j) {
            const Coefficient& g_j = g.coefficient(Variable(j));
            const dimension_type dj = 2*j;
            // Set for any point the straight lines passing in the point
            // and having the octagonal gradient; compute maxima values.
            // Let be P = [P_1, P_2, ..., P_n] point.
            // Hyperplane: X_i - X_j = max (P_i - P_j, const).
            div_round_up(tmp, g_j - g_i, d);
            max_assign(x_i[dj], tmp);
            div_round_up(tmp, g_i - g_j, d);
            max_assign(x_ii[dj + 1], tmp);
            // Hyperplane: X_i + X_j = max (P_i + P_j, const).
            div_round_up(tmp, -g_j - g_i, d);
            max_assign(x_i[dj + 1], tmp);
            div_round_up(tmp, g_i + g_j, d);
            max_assign(x_ii[dj], tmp);
          }
          // Hyperplane: X_i = max (P_i, const).
          div_round_up(tmp, -g_i - g_i, d);
          max_assign(x_i[di + 1], tmp);
          div_round_up(tmp, g_i + g_i, d);
          max_assign(x_ii[di], tmp);
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
    throw_invalid_argument("Octagonal_Shape(gs)",
                           "the non-empty generator system gs "
                           "contains no points.");

  // Going through all the lines and rays.
  for (Generator_System::const_iterator k = gs_begin; k != gs_end; ++k) {
    const Generator& g = *k;
    switch (g.type()) {
    case Generator::LINE:
        // TODO: This can be optimized more, if needed, exploiting the
        // (possible) sparseness of g. Also consider if OR_Matrix should be
        // sparse, too.
        for (dimension_type i = 0; i < space_dim; ++i) {
          const Coefficient& g_i = g.coefficient(Variable(i));
          const dimension_type di = 2*i;
          row_reference x_i = *(mat_begin + di);
          row_reference x_ii = *(mat_begin + (di + 1));
          for (dimension_type j = 0; j < i; ++j) {
            const Coefficient& g_j = g.coefficient(Variable(j));
            const dimension_type dj = 2*j;
            // Set for any line the right limit.
            if (g_i != g_j) {
              // Hyperplane: X_i - X_j <=/>= +Inf.
              assign_r(x_i[dj], PLUS_INFINITY, ROUND_NOT_NEEDED);
              assign_r(x_ii[dj + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
            }
            if (g_i != -g_j) {
              // Hyperplane: X_i + X_j <=/>= +Inf.
              assign_r(x_i[dj + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
              assign_r(x_ii[dj], PLUS_INFINITY, ROUND_NOT_NEEDED);
            }
          }
          if (g_i != 0) {
            // Hyperplane: X_i <=/>= +Inf.
            assign_r(x_i[di + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
            assign_r(x_ii[di], PLUS_INFINITY, ROUND_NOT_NEEDED);
          }
        }
      break;
    case Generator::RAY:
        // TODO: This can be optimized more, if needed, exploiting the
        // (possible) sparseness of g. Also consider if OR_Matrix should be
        // sparse, too.
        for (dimension_type i = 0; i < space_dim; ++i) {
          const Coefficient& g_i = g.coefficient(Variable(i));
          const dimension_type di = 2*i;
          row_reference x_i = *(mat_begin + di);
          row_reference x_ii = *(mat_begin + (di + 1));
          for (dimension_type j = 0; j < i; ++j) {
            const Coefficient& g_j = g.coefficient(Variable(j));
            const dimension_type dj = 2*j;
            // Set for any ray the right limit in the case
            // of the binary constraints.
            if (g_i < g_j)
              // Hyperplane: X_i - X_j >= +Inf.
              assign_r(x_i[dj], PLUS_INFINITY, ROUND_NOT_NEEDED);
            if (g_i > g_j)
              // Hyperplane: X_i - X_j <= +Inf.
              assign_r(x_ii[dj + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
            if (g_i < -g_j)
              // Hyperplane: X_i + X_j >= +Inf.
              assign_r(x_i[dj + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
            if (g_i > -g_j)
              // Hyperplane: X_i + X_j <= +Inf.
              assign_r(x_ii[dj], PLUS_INFINITY, ROUND_NOT_NEEDED);
          }
          // Case: unary constraints.
          if (g_i < 0)
            // Hyperplane: X_i  = +Inf.
            assign_r(x_i[di + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
          if (g_i > 0)
            // Hyperplane: X_i  = +Inf.
            assign_r(x_ii[di], PLUS_INFINITY, ROUND_NOT_NEEDED);
        }
      break;
    default:
      // Points and closure points already dealt with.
      break;
    }
  }
  set_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::add_constraint(const Constraint& c) {
  const dimension_type c_space_dim = c.space_dimension();
  // Dimension-compatibility check.
  if (c_space_dim > space_dim)
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
  PPL_DIRTY_TEMP_COEFFICIENT(term);
  // Constraints that are not octagonal differences are not allowed.
  if (!Octagonal_Shape_Helper
    ::extract_octagonal_difference(c, c_space_dim, num_vars,
                                   i, j, coeff, term))
    throw_invalid_argument("add_constraint(c)",
                           "c is not an octagonal constraint");

  if (num_vars == 0) {
    // Dealing with a trivial constraint (not a strict inequality).
    if (c.inhomogeneous_term() < 0
        || (c.is_equality() && c.inhomogeneous_term() != 0))
      set_empty();
    return;
  }

  // Select the cell to be modified for the "<=" part of constraint.
  typename OR_Matrix<N>::row_iterator i_iter = matrix.row_begin() + i;
  typename OR_Matrix<N>::row_reference_type m_i = *i_iter;
  N& m_i_j = m_i[j];
  // Set `coeff' to the absolute value of itself.
  if (coeff < 0)
    neg_assign(coeff);

  bool is_oct_changed = false;
  // Compute the bound for `m_i_j', rounding towards plus infinity.
  PPL_DIRTY_TEMP(N, d);
  div_round_up(d, term, coeff);
  if (m_i_j > d) {
    m_i_j = d;
    is_oct_changed = true;
  }

  if (c.is_equality()) {
    // Select the cell to be modified for the ">=" part of constraint.
    if (i % 2 == 0)
      ++i_iter;
    else
      --i_iter;

    typename OR_Matrix<N>::row_reference_type m_ci = *i_iter;
    using namespace Implementation::Octagonal_Shapes;
    dimension_type cj = coherent_index(j);
    N& m_ci_cj = m_ci[cj];
    // Also compute the bound for `m_ci_cj', rounding towards plus infinity.
    neg_assign(term);
    div_round_up(d, term, coeff);
    if (m_ci_cj > d) {
      m_ci_cj = d;
      is_oct_changed = true;
    }
  }

  // This method does not preserve closure.
  if (is_oct_changed && marked_strongly_closed())
    reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::add_congruence(const Congruence& cg) {
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
template <typename Interval_Info>
void
Octagonal_Shape<T>::refine_with_linear_form_inequality(
                    const Linear_Form< Interval<T, Interval_Info> >& left,
                    const Linear_Form< Interval<T, Interval_Info> >& right) {

  // Check that T is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
                     "Octagonal_Shape<T>::refine_with_linear_form_inequality:"
                     " T not a floating point type.");

  // We assume that the analyzer will not try to apply an unreachable filter.
  PPL_ASSERT(!marked_empty());

  // Dimension-compatibility checks.
  // The dimensions of `left' and `right' should not be greater than the
  // dimension of `*this'.
  const dimension_type left_space_dim = left.space_dimension();
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

  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  typedef typename OR_Matrix<N>::const_row_iterator Row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type Row_reference;
  typedef Interval<T, Interval_Info> FP_Interval_Type;

  // FIXME: there is plenty of duplicate code in the following lines. We could
  // shorten it at the expense of a bit of efficiency.

  if (left_t == 0) {
    if (right_t == 0) {
      // The constraint involves constants only. Ignore it: it is up to
      // the analyzer to handle it.
      PPL_ASSERT(OK());
      return;
    }

    if (right_t == 1) {
      // The constraint has the form [a-, a+] <= [b-, b+] + [c-, c+] * x.
      // Reduce it to the constraint +/-x <= b+ - a- if [c-, c+] = +/-[1, 1].
      const FP_Interval_Type& right_w_coeff =
                              right.coefficient(Variable(right_w_id));
      if (right_w_coeff == 1) {
        const dimension_type n_right = right_w_id * 2;
        PPL_DIRTY_TEMP(N, b_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_b = right.inhomogeneous_term();
        sub_assign_r(b_plus_minus_a_minus, right_b.upper(), left_a.lower(),
                     ROUND_UP);
        mul_2exp_assign_r(b_plus_minus_a_minus, b_plus_minus_a_minus, 1,
                          ROUND_UP);
        add_octagonal_constraint(n_right, n_right + 1, b_plus_minus_a_minus);
        PPL_ASSERT(OK());
        return;
      }

      if (right_w_coeff == -1) {
        const dimension_type n_right = right_w_id * 2;
        PPL_DIRTY_TEMP(N, b_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_b = right.inhomogeneous_term();
        sub_assign_r(b_plus_minus_a_minus, right_b.upper(), left_a.lower(),
                     ROUND_UP);
        mul_2exp_assign_r(b_plus_minus_a_minus, b_plus_minus_a_minus, 1,
                          ROUND_UP);
        add_octagonal_constraint(n_right + 1, n_right, b_plus_minus_a_minus);
        PPL_ASSERT(OK());
        return;
      }
    }
  }
  else if (left_t == 1) {
    if (right_t == 0) {
      // The constraint has the form [b-, b+] + [c-, c+] * x <= [a-, a+]
      // Reduce it to the constraint +/-x <= a+ - b- if [c-, c+] = +/-[1, 1].
      const FP_Interval_Type& left_w_coeff =
                              left.coefficient(Variable(left_w_id));
      if (left_w_coeff == 1) {
        const dimension_type n_left = left_w_id * 2;
        PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
        const FP_Interval_Type& left_b = left.inhomogeneous_term();
        const FP_Interval_Type& right_a = right.inhomogeneous_term();
        sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                     ROUND_UP);
        mul_2exp_assign_r(a_plus_minus_b_minus, a_plus_minus_b_minus, 1,
                          ROUND_UP);
        add_octagonal_constraint(n_left + 1, n_left, a_plus_minus_b_minus);
        PPL_ASSERT(OK());
        return;
      }

      if (left_w_coeff == -1) {
        const dimension_type n_left = left_w_id * 2;
        PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
        const FP_Interval_Type& left_b = left.inhomogeneous_term();
        const FP_Interval_Type& right_a = right.inhomogeneous_term();
        sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                     ROUND_UP);
        mul_2exp_assign_r(a_plus_minus_b_minus, a_plus_minus_b_minus, 1,
                          ROUND_UP);
        add_octagonal_constraint(n_left, n_left + 1, a_plus_minus_b_minus);
        PPL_ASSERT(OK());
        return;
      }
    }

    if (right_t == 1) {
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
            || (is_left_coeff_minus_one && is_right_coeff_minus_one)) {
          // Here we have an identity or a constants-only constraint.
          PPL_ASSERT(OK());
          return;
        }
        if (is_left_coeff_one && is_right_coeff_minus_one) {
          // We fall back to a previous case
          // (but we do not need to multiply the result by two).
          const dimension_type n_left = left_w_id * 2;
          PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
          const FP_Interval_Type& left_b = left.inhomogeneous_term();
          const FP_Interval_Type& right_a = right.inhomogeneous_term();
          sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                       ROUND_UP);
          add_octagonal_constraint(n_left + 1, n_left, a_plus_minus_b_minus);
          PPL_ASSERT(OK());
          return;
        }
        if (is_left_coeff_minus_one && is_right_coeff_one) {
          // We fall back to a previous case
          // (but we do not need to multiply the result by two).
          const dimension_type n_left = left_w_id * 2;
          PPL_DIRTY_TEMP(N, a_plus_minus_b_minus);
          const FP_Interval_Type& left_b = left.inhomogeneous_term();
          const FP_Interval_Type& right_a = right.inhomogeneous_term();
          sub_assign_r(a_plus_minus_b_minus, right_a.upper(), left_b.lower(),
                       ROUND_UP);
          add_octagonal_constraint(n_left, n_left + 1, a_plus_minus_b_minus);
          PPL_ASSERT(OK());
          return;
        }
      }
      else if (is_left_coeff_one && is_right_coeff_one) {
        const dimension_type n_left = left_w_id * 2;
        const dimension_type n_right = right_w_id * 2;
        PPL_DIRTY_TEMP(N, c_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_c = right.inhomogeneous_term();
        sub_assign_r(c_plus_minus_a_minus, right_c.upper(), left_a.lower(),
                     ROUND_UP);
        if (left_w_id < right_w_id)
          add_octagonal_constraint(n_right, n_left, c_plus_minus_a_minus);
        else
          add_octagonal_constraint(n_left + 1, n_right + 1,
                                   c_plus_minus_a_minus);
        PPL_ASSERT(OK());
        return;
      }
      if (is_left_coeff_one && is_right_coeff_minus_one) {
        const dimension_type n_left = left_w_id * 2;
        const dimension_type n_right = right_w_id * 2;
        PPL_DIRTY_TEMP(N, c_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_c = right.inhomogeneous_term();
        sub_assign_r(c_plus_minus_a_minus, right_c.upper(), left_a.lower(),
                     ROUND_UP);
        if (left_w_id < right_w_id)
          add_octagonal_constraint(n_right + 1, n_left, c_plus_minus_a_minus);
        else
          add_octagonal_constraint(n_left + 1, n_right, c_plus_minus_a_minus);
        PPL_ASSERT(OK());
        return;
      }
      if (is_left_coeff_minus_one && is_right_coeff_one) {
        const dimension_type n_left = left_w_id * 2;
        const dimension_type n_right = right_w_id * 2;
        PPL_DIRTY_TEMP(N, c_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_c = right.inhomogeneous_term();
        sub_assign_r(c_plus_minus_a_minus, right_c.upper(), left_a.lower(),
                     ROUND_UP);
        if (left_w_id < right_w_id)
          add_octagonal_constraint(n_right, n_left + 1, c_plus_minus_a_minus);
        else
          add_octagonal_constraint(n_left, n_right + 1, c_plus_minus_a_minus);
        PPL_ASSERT(OK());
        return;
      }
      if (is_left_coeff_minus_one && is_right_coeff_minus_one) {
        const dimension_type n_left = left_w_id * 2;
        const dimension_type n_right = right_w_id * 2;
        PPL_DIRTY_TEMP(N, c_plus_minus_a_minus);
        const FP_Interval_Type& left_a = left.inhomogeneous_term();
        const FP_Interval_Type& right_c = right.inhomogeneous_term();
        sub_assign_r(c_plus_minus_a_minus, right_c.upper(), left_a.lower(),
                     ROUND_UP);
        if (left_w_id < right_w_id)
          add_octagonal_constraint(n_right + 1, n_left + 1,
                                   c_plus_minus_a_minus);
        else
          add_octagonal_constraint(n_left, n_right, c_plus_minus_a_minus);
        PPL_ASSERT(OK());
        return;
      }
    }
  }

  // General case.

  // FIRST, update the binary constraints for each pair of DIFFERENT variables
  // in `left' and `right'.

  // Declare temporaries outside of the loop.
  PPL_DIRTY_TEMP(N, low_coeff);
  PPL_DIRTY_TEMP(N, high_coeff);
  PPL_DIRTY_TEMP(N, upper_bound);

  Linear_Form<FP_Interval_Type> right_minus_left(right);
  right_minus_left -= left;

  dimension_type max_w_id = std::max(left_w_id, right_w_id);
  for (dimension_type first_v = 0; first_v < max_w_id; ++first_v) {
    for (dimension_type second_v = first_v + 1;
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
        dimension_type n_first_var = first_v * 2;
        dimension_type n_second_var = second_v * 2;
        linear_form_upper_bound(right_minus_left - first + second,
                                upper_bound);
        add_octagonal_constraint(n_second_var + 1, n_first_var + 1,
                                 upper_bound);
        linear_form_upper_bound(right_minus_left + first + second,
                                upper_bound);
        add_octagonal_constraint(n_second_var + 1, n_first_var, upper_bound);
        linear_form_upper_bound(right_minus_left - first - second,
                                upper_bound);
        add_octagonal_constraint(n_second_var, n_first_var + 1, upper_bound);
        linear_form_upper_bound(right_minus_left + first - second,
                                upper_bound);
        add_octagonal_constraint(n_second_var, n_first_var, upper_bound);
      }
    }
  }

  // Finally, update the unary constraints.
  for (dimension_type v = 0; v <= max_w_id; ++v) {
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
      dimension_type n_var = 2 * v;
      /*
        VERY DIRTY trick: since we need to keep the old unary constraints
        while computing the new ones, we momentarily keep the new coefficients
        in the main diagonal of the matrix. They will be moved later.
      */
      linear_form_upper_bound(right_minus_left + var, upper_bound);
      mul_2exp_assign_r(matrix[n_var + 1][n_var + 1], upper_bound, 1,
                        ROUND_UP);
      linear_form_upper_bound(right_minus_left - var, upper_bound);
      mul_2exp_assign_r(matrix[n_var][n_var], upper_bound, 1,
                        ROUND_UP);
    }
  }

  /*
    Now move the newly computed coefficients from the main diagonal to
    their proper place, and restore +infinity on the diagonal.
  */
  row_iterator m_ite = matrix.row_begin();
  row_iterator m_end = matrix.row_end();
  for (dimension_type i = 0; m_ite != m_end; i += 2) {
    row_reference upper = *m_ite;
    N& ul = upper[i];
    add_octagonal_constraint(i, i + 1, ul);
    assign_r(ul, PLUS_INFINITY, ROUND_NOT_NEEDED);
    ++m_ite;
    row_reference lower = *m_ite;
    N& lr = lower[i + 1];
    add_octagonal_constraint(i + 1, i, lr);
    assign_r(lr, PLUS_INFINITY, ROUND_NOT_NEEDED);
    ++m_ite;
  }
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::refine_no_check(const Constraint& c) {
  PPL_ASSERT(!marked_empty());
  const dimension_type c_space_dim = c.space_dimension();
  PPL_ASSERT(c_space_dim <= space_dim);

  dimension_type num_vars = 0;
  dimension_type i = 0;
  dimension_type j = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(term);
  // Constraints that are not octagonal differences are ignored.
  if (!Octagonal_Shape_Helper
    ::extract_octagonal_difference(c, c_space_dim, num_vars,
                                   i, j, coeff, term))
    return;

  if (num_vars == 0) {
    const Coefficient& c_inhomo = c.inhomogeneous_term();
    // Dealing with a trivial constraint (maybe a strict inequality).
    if (c_inhomo < 0
        || (c_inhomo != 0 && c.is_equality())
        || (c_inhomo == 0 && c.is_strict_inequality()))
      set_empty();
    return;
  }

  // Select the cell to be modified for the "<=" part of constraint.
  typename OR_Matrix<N>::row_iterator i_iter = matrix.row_begin() + i;
  typename OR_Matrix<N>::row_reference_type m_i = *i_iter;
  N& m_i_j = m_i[j];
  // Set `coeff' to the absolute value of itself.
  if (coeff < 0)
    neg_assign(coeff);

  bool is_oct_changed = false;
  // Compute the bound for `m_i_j', rounding towards plus infinity.
  PPL_DIRTY_TEMP(N, d);
  div_round_up(d, term, coeff);
  if (m_i_j > d) {
    m_i_j = d;
    is_oct_changed = true;
  }

  if (c.is_equality()) {
    // Select the cell to be modified for the ">=" part of constraint.
    if (i % 2 == 0)
      ++i_iter;
    else
      --i_iter;

    typename OR_Matrix<N>::row_reference_type m_ci = *i_iter;
    using namespace Implementation::Octagonal_Shapes;
    dimension_type cj = coherent_index(j);
    N& m_ci_cj = m_ci[cj];
    // Also compute the bound for `m_ci_cj', rounding towards plus infinity.
    neg_assign(term);
    div_round_up(d, term, coeff);
    if (m_ci_cj > d) {
      m_ci_cj = d;
      is_oct_changed = true;
    }
  }

  // This method does not preserve closure.
  if (is_oct_changed && marked_strongly_closed())
    reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
dimension_type
Octagonal_Shape<T>::affine_dimension() const {
  const dimension_type n_rows = matrix.num_rows();
  // A zero-space-dim shape always has affine dimension zero.
  if (n_rows == 0)
    return 0;

  // Strong closure is necessary to detect emptiness
  // and all (possibly implicit) equalities.
  strong_closure_assign();
  if (marked_empty())
    return 0;

  // The vector `leaders' is used to represent non-singular
  // equivalence classes:
  // `leaders[i] == i' if and only if `i' is the leader of its
  // equivalence class (i.e., the minimum index in the class).
  std::vector<dimension_type> leaders;
  compute_leaders(leaders);

  // Due to the splitting of variables, the affine dimension is the
  // number of non-singular positive zero-equivalence classes.
  dimension_type affine_dim = 0;
  for (dimension_type i = 0; i < n_rows; i += 2)
    // Note: disregard the singular equivalence class.
    if (leaders[i] == i && leaders[i + 1] == i + 1)
      ++affine_dim;

  return affine_dim;
}

template <typename T>
Congruence_System
Octagonal_Shape<T>::minimized_congruences() const {
  // Strong closure is necessary to detect emptiness
  // and all (possibly implicit) equalities.
  strong_closure_assign();
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

  // The vector `leaders' is used to represent equivalence classes:
  // `leaders[i] == i' if and only if `i' is the leader of its
  // equivalence class (i.e., the minimum index in the class).
  std::vector<dimension_type> leaders;
  compute_leaders(leaders);

  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);
  for (dimension_type i = 0, i_end = 2*space_dim; i != i_end; i += 2) {
    const dimension_type lead_i = leaders[i];
    if (i == lead_i) {
      if (leaders[i + 1] == i)
        // `i' is the leader of the singular equivalence class.
        goto singular;
      else
        // `i' is the leader of a non-singular equivalence class.
        continue;
    }
    else {
      // `i' is not a leader.
      if (leaders[i + 1] == lead_i)
        // `i' belongs to the singular equivalence class.
        goto singular;
      else
        // `i' does not belong to the singular equivalence class.
        goto non_singular;
    }

  singular:
    // `i' belongs to the singular equivalence class:
    // we have a unary equality constraint.
    {
      const Variable x(i/2);
      const N& c_ii_i = matrix[i + 1][i];
#ifndef NDEBUG
      const N& c_i_ii = matrix[i][i + 1];
      PPL_ASSERT(is_additive_inverse(c_i_ii, c_ii_i));
#endif
      numer_denom(c_ii_i, numer, denom);
      denom *= 2;
      cgs.insert(denom*x == numer);
    }
    continue;

  non_singular:
    // `i' does not belong to the singular equivalence class.
    // we have a binary equality constraint.
    {
      const N& c_i_li = matrix[i][lead_i];
#ifndef NDEBUG
      using namespace Implementation::Octagonal_Shapes;
      const N& c_ii_lii = matrix[i + 1][coherent_index(lead_i)];
      PPL_ASSERT(is_additive_inverse(c_ii_lii, c_i_li));
#endif
      const Variable x(lead_i/2);
      const Variable y(i/2);
      numer_denom(c_i_li, numer, denom);
      if (lead_i % 2 == 0)
        cgs.insert(denom*x - denom*y == numer);
      else
        cgs.insert(denom*x + denom*y + numer == 0);
    }
    continue;
  }
  return cgs;
}

template <typename T>
void
Octagonal_Shape<T>::concatenate_assign(const Octagonal_Shape& y) {
  // If `y' is an empty 0-dim space octagon, let `*this' become empty.
  // If `y' is an universal 0-dim space octagon, we simply return.
  if (y.space_dim == 0) {
    if (y.marked_empty())
      set_empty();
    return;
  }

  // If `*this' is an empty 0-dim space octagon, then it is sufficient
  // to adjust the dimension of the vector space.
  if (space_dim == 0 && marked_empty()) {
    add_space_dimensions_and_embed(y.space_dim);
    return;
  }

  // This is the old number of rows in the matrix. It is equal to
  // the first index of columns to change.
  dimension_type old_num_rows = matrix.num_rows();
  // First we increase the space dimension of `*this' by adding
  // `y.space_dimension()' new dimensions.
  // The matrix for the new octagon is obtained
  // by leaving the old system of constraints in the upper left-hand side
  // (where they are at the present) and placing the constraints of `y' in the
  // lower right-hand side.
  add_space_dimensions_and_embed(y.space_dim);
  typename OR_Matrix<N>::const_element_iterator
    y_it = y.matrix.element_begin();
  for (typename OR_Matrix<N>::row_iterator
         i = matrix.row_begin() + old_num_rows,
         matrix_row_end = matrix.row_end(); i != matrix_row_end; ++i) {
    typename OR_Matrix<N>::row_reference_type r = *i;
    dimension_type rs_i = i.row_size();
    for (dimension_type j = old_num_rows; j < rs_i; ++j, ++y_it)
      r[j] = *y_it;
  }

  // The concatenation does not preserve the closure.
  if (marked_strongly_closed())
    reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
bool
Octagonal_Shape<T>::contains(const Octagonal_Shape& y) const {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("contains(y)", y);

  if (space_dim == 0) {
    // The zero-dimensional empty octagon only contains another
    // zero-dimensional empty octagon.
    // The zero-dimensional universe octagon contains any other
    // zero-dimensional octagon.
    return marked_empty() ? y.marked_empty() : true;
  }

  // `y' needs to be transitively closed.
  y.strong_closure_assign();
  // An empty octagon is in any other dimension-compatible octagons.
  if (y.marked_empty())
    return true;

  // If `*this' is empty it can not contain `y' (which is not empty).
  if (is_empty())
    return false;

  // `*this' contains `y' if and only if every element of `*this'
  // is greater than or equal to the correspondent one of `y'.
  for (typename OR_Matrix<N>::const_element_iterator
         i = matrix.element_begin(), j = y.matrix.element_begin(),
         matrix_element_end = matrix.element_end();
       i != matrix_element_end; ++i, ++j)
    if (*i < *j)
      return false;
  return true;
}

template <typename T>
bool
Octagonal_Shape<T>::is_disjoint_from(const Octagonal_Shape& y) const {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("is_disjoint_from(y)", y);

  // If one Octagonal_Shape is empty, the Octagonal_Shapes are disjoint.
  strong_closure_assign();
  if (marked_empty())
    return true;
  y.strong_closure_assign();
  if (y.marked_empty())
    return true;

  // Two Octagonal_Shapes are disjoint if and only if their
  // intersection is empty, i.e., if and only if there exists a
  // variable such that the upper bound of the constraint on that
  // variable in the first Octagonal_Shape is strictly less than the
  // lower bound of the corresponding constraint in the second
  // Octagonal_Shape or vice versa.

  const dimension_type n_rows = matrix.num_rows();

  typedef typename OR_Matrix<N>::const_row_iterator row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type row_reference;

  const row_iterator m_begin = matrix.row_begin();
  const row_iterator m_end = matrix.row_end();

  const row_iterator y_begin = y.matrix.row_begin();

  PPL_DIRTY_TEMP(N, neg_y_ci_cj);
  for (row_iterator i_iter = m_begin; i_iter != m_end; ++i_iter) {
    using namespace Implementation::Octagonal_Shapes;
    const dimension_type i = i_iter.index();
    const dimension_type ci = coherent_index(i);
    const dimension_type rs_i = i_iter.row_size();
    row_reference m_i = *i_iter;
    for (dimension_type j = 0; j < n_rows; ++j) {
      const dimension_type cj = coherent_index(j);
      row_reference m_cj = *(m_begin + cj);
      const N& m_i_j = (j < rs_i) ? m_i[j] : m_cj[ci];
      row_reference y_ci = *(y_begin + ci);
      row_reference y_j = *(y_begin + j);
      const N& y_ci_cj = (j < rs_i) ? y_ci[cj] : y_j[i];
      neg_assign_r(neg_y_ci_cj, y_ci_cj, ROUND_UP);
      if (m_i_j < neg_y_ci_cj)
        return true;
    }
  }
  return false;
}

template <typename T>
bool
Octagonal_Shape<T>::is_universe() const {
  // An empty octagon is not universe.
  if (marked_empty())
    return false;

  // If the octagon is non-empty and zero-dimensional,
  // then it is necessarily the universe octagon.
  if (space_dim == 0)
    return true;

  // An universe octagon can only contains trivial  constraints.
  for (typename OR_Matrix<N>::const_element_iterator
         i = matrix.element_begin(), matrix_element_end = matrix.element_end();
       i != matrix_element_end;
       ++i)
    if (!is_plus_infinity(*i))
      return false;

  return true;
}

template <typename T>
bool
Octagonal_Shape<T>::is_bounded() const {
  strong_closure_assign();
  // A zero-dimensional or empty octagon is bounded.
  if (marked_empty() || space_dim == 0)
    return true;

  // A bounded octagon never can contains trivial constraints.
  for (typename OR_Matrix<N>::const_row_iterator i = matrix.row_begin(),
         matrix_row_end = matrix.row_end(); i != matrix_row_end; ++i) {
    typename OR_Matrix<N>::const_row_reference_type x_i = *i;
    const dimension_type i_index = i.index();
    for (dimension_type j = i.row_size(); j-- > 0; )
      if (i_index != j)
        if (is_plus_infinity(x_i[j]))
          return false;
  }

  return true;
}

template <typename T>
bool
Octagonal_Shape<T>::contains_integer_point() const {
  // Force strong closure.
  if (is_empty())
    return false;
  const dimension_type space_dim = space_dimension();
  if (space_dim == 0)
    return true;

  // A strongly closed and consistent Octagonal_Shape defined by
  // integer constraints can only be empty due to tight coherence.
  if (std::numeric_limits<T>::is_integer)
    return !tight_coherence_would_make_empty();

  // Build an integer Octagonal_Shape oct_z with bounds at least as
  // tight as those in *this and then recheck for emptiness, also
  // exploiting tight-coherence.
  Octagonal_Shape<mpz_class> oct_z(space_dim);
  oct_z.reset_strongly_closed();

  typedef Octagonal_Shape<mpz_class>::N Z;
  bool all_integers = true;
  typename OR_Matrix<N>::const_element_iterator x_i = matrix.element_begin();
  for (typename OR_Matrix<Z>::element_iterator
         z_i = oct_z.matrix.element_begin(),
         z_end = oct_z.matrix.element_end(); z_i != z_end; ++z_i, ++x_i) {
    const N& d = *x_i;
    if (is_plus_infinity(d))
      continue;
    if (is_integer(d))
      assign_r(*z_i, d, ROUND_NOT_NEEDED);
    else {
      all_integers = false;
      assign_r(*z_i, d, ROUND_DOWN);
    }
  }
  // Restore strong closure.
  if (all_integers)
    // oct_z unchanged, so it is still strongly closed.
    oct_z.set_strongly_closed();
  else {
    // oct_z changed: recompute strong closure.
    oct_z.strong_closure_assign();
    if (oct_z.marked_empty())
      return false;
  }
  return !oct_z.tight_coherence_would_make_empty();
}

template <typename T>
bool
Octagonal_Shape<T>::frequency(const Linear_Expression& expr,
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

  strong_closure_assign();
  // For an empty Octagonal shape, we simply return false.
  if (marked_empty())
    return false;

  // The Octagonal shape has at least 1 dimension and is not empty.
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(coeff_j);
  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);
  Linear_Expression le = expr;
  // Boolean to keep track of a variable `v' in expression `le'.
  // If we can replace `v' by an expression using variables other
  // than `v' and are already in `le', then this is set to true.
  bool constant_v = false;

  typedef typename OR_Matrix<N>::const_row_iterator row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type row_reference;

  const row_iterator m_begin = matrix.row_begin();
  const row_iterator m_end = matrix.row_end();

  PPL_DIRTY_TEMP_COEFFICIENT(val_denom);
  val_denom = 1;

  for (row_iterator i_iter = m_begin; i_iter != m_end; i_iter += 2) {
    constant_v = false;
    dimension_type i = i_iter.index();
    const Variable v(i/2);
    coeff = le.coefficient(v);
    if (coeff == 0) {
      constant_v = true;
      continue;
    }
    // We check the unary constraints.
    row_reference m_i = *i_iter;
    row_reference m_ii = *(i_iter + 1);
    const N& m_i_ii = m_i[i + 1];
    const N& m_ii_i = m_ii[i];
    if ((!is_plus_infinity(m_i_ii) && !is_plus_infinity(m_ii_i))
        && (is_additive_inverse(m_i_ii, m_ii_i))) {
      // If `v' is constant, replace it in `le' by the value.
      numer_denom(m_i_ii, numer, denom);
      denom *= 2;
      le -= coeff*v;
      le *= denom;
      le -= numer*coeff;
      val_denom *= denom;
      constant_v = true;
      continue;
    }
    // Check the octagonal constraints between `v' and the other dimensions
    // that have non-zero coefficient in `le'.
    else {
      PPL_ASSERT(!constant_v);
      using namespace Implementation::Octagonal_Shapes;
      const dimension_type ci = coherent_index(i);
      for (row_iterator j_iter = i_iter; j_iter != m_end; j_iter += 2) {
        dimension_type j = j_iter.index();
        const Variable vj(j/2);
        coeff_j = le.coefficient(vj);
        if (coeff_j == 0)
          // The coefficient in `le' is 0, so do nothing.
          continue;
        const dimension_type cj = coherent_index(j);
        const dimension_type cjj = coherent_index(j + 1);

        row_reference m_j = *(m_begin + j);
        row_reference m_cj = *(m_begin + cj);
        const N& m_j_i = m_j[i];
        const N& m_i_j = m_cj[ci];
        if ((!is_plus_infinity(m_i_j) && !is_plus_infinity(m_j_i))
            && (is_additive_inverse(m_i_j, m_j_i))) {
          // The coefficient for `vj' in `le' is not 0
          // and the constraint with `v' is an equality.
          // So apply this equality to eliminate `v' in `le'.
          numer_denom(m_i_j, numer, denom);
          le -= coeff*v;
          le += coeff*vj;
          le *= denom;
          le -= numer*coeff;
          val_denom *= denom;
          constant_v = true;
          break;
        }

        m_j = *(m_begin + (j + 1));
        m_cj = *(m_begin + cjj);
        const N& m_j_i1 = m_j[i];
        const N& m_i_j1 = m_cj[ci];
        if ((!is_plus_infinity(m_i_j1) && !is_plus_infinity(m_j_i1))
            && (is_additive_inverse(m_i_j1, m_j_i1))) {
          // The coefficient for `vj' in `le' is not 0
          // and the constraint with `v' is an equality.
          // So apply this equality to eliminate `v' in `le'.
          numer_denom(m_i_j1, numer, denom);
          le -= coeff*v;
          le -= coeff*vj;
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
  if (!constant_v)
    // The expression `expr' is not constant.
    return false;

  // The expression 'expr' is constant.
  freq_n = 0;
  freq_d = 1;

  // Reduce `val_n' and `val_d'.
  normalize2(le.inhomogeneous_term(), val_denom, val_n, val_d);
  return true;
}

template <typename T>
bool
Octagonal_Shape<T>::constrains(const Variable var) const {
  // `var' should be one of the dimensions of the octagonal shape.
  const dimension_type var_space_dim = var.space_dimension();
  if (space_dimension() < var_space_dim)
    throw_dimension_incompatible("constrains(v)", "v", var);

  // An octagon known to be empty constrains all variables.
  // (Note: do not force emptiness check _yet_)
  if (marked_empty())
    return true;

  // Check whether `var' is syntactically constrained.
  const dimension_type n_v = 2*(var_space_dim - 1);
  typename OR_Matrix<N>::const_row_iterator m_iter = matrix.row_begin() + n_v;
  typename OR_Matrix<N>::const_row_reference_type r_v = *m_iter;
  typename OR_Matrix<N>::const_row_reference_type r_cv = *(++m_iter);
  for (dimension_type h = m_iter.row_size(); h-- > 0; ) {
    if (!is_plus_infinity(r_v[h]) || !is_plus_infinity(r_cv[h]))
      return true;
  }
  ++m_iter;
  for (typename OR_Matrix<N>::const_row_iterator m_end = matrix.row_end();
       m_iter != m_end; ++m_iter) {
    typename OR_Matrix<N>::const_row_reference_type r = *m_iter;
    if (!is_plus_infinity(r[n_v]) || !is_plus_infinity(r[n_v + 1]))
      return true;
  }

  // `var' is not syntactically constrained:
  // now force an emptiness check.
  return is_empty();
}

template <typename T>
bool
Octagonal_Shape<T>::is_strong_coherent() const {
  // This method is only used by method OK() so as to check if a
  // strongly closed matrix is also strong-coherent, as it must be.
  const dimension_type num_rows = matrix.num_rows();

  // Allocated here once and for all.
  PPL_DIRTY_TEMP(N, semi_sum);
  // The strong-coherence is: for every indexes i and j (and i != j)
  // matrix[i][j] <= (matrix[i][ci] + matrix[cj][j])/2
  // where ci = i + 1, if i is even number or
  //       ci = i - 1, if i is odd.
  // Ditto for cj.
  for (dimension_type i = num_rows; i-- > 0; ) {
    typename OR_Matrix<N>::const_row_iterator iter = matrix.row_begin() + i;
    typename OR_Matrix<N>::const_row_reference_type m_i = *iter;
    using namespace Implementation::Octagonal_Shapes;
    const N& m_i_ci = m_i[coherent_index(i)];
    for (dimension_type j = matrix.row_size(i); j-- > 0; )
      // Note: on the main diagonal only PLUS_INFINITY can occur.
      if (i != j) {
        const N& m_cj_j = matrix[coherent_index(j)][j];
        if (!is_plus_infinity(m_i_ci)
            && !is_plus_infinity(m_cj_j)) {
          // Compute (m_i_ci + m_cj_j)/2 into `semi_sum',
          // rounding the result towards plus infinity.
          add_assign_r(semi_sum, m_i_ci, m_cj_j, ROUND_UP);
          div_2exp_assign_r(semi_sum, semi_sum, 1, ROUND_UP);
          if (m_i[j] > semi_sum)
            return false;
        }
      }
  }
  return true;
}

template <typename T>
bool
Octagonal_Shape<T>::is_strongly_reduced() const {
  // This method is only used in assertions: efficiency is not a must.

  // An empty octagon is already transitively reduced.
  if (marked_empty())
    return true;

  Octagonal_Shape x = *this;
  // The matrix representing an OS is strongly reduced if, by removing
  // any constraint, the resulting matrix describes a different OS.
  for (typename OR_Matrix<N>::const_row_iterator iter = matrix.row_begin(),
         matrix_row_end = matrix.row_end(); iter != matrix_row_end; ++iter) {
    typename OR_Matrix<N>::const_row_reference_type m_i = *iter;
    const dimension_type i = iter.index();
    for (dimension_type j = iter.row_size(); j-- > 0; ) {
      if (!is_plus_infinity(m_i[j])) {
        Octagonal_Shape x_copy = *this;
        assign_r(x_copy.matrix[i][j], PLUS_INFINITY, ROUND_NOT_NEEDED);
        if (x == x_copy)
          return false;
      }
    }
  }
  // The octagon is just reduced.
  return true;
}

template <typename T>
bool
Octagonal_Shape<T>::bounds(const Linear_Expression& expr,
                           const bool from_above) const {
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible((from_above
                                  ? "bounds_from_above(e)"
                                  : "bounds_from_below(e)"), "e", expr);
  strong_closure_assign();

  // A zero-dimensional or empty octagon bounds everything.
  if (space_dim == 0 || marked_empty())
    return true;

  // The constraint `c' is used to check if `expr' is an octagonal difference
  // and, in this case, to select the cell.
  const Constraint& c = (from_above) ? expr <= 0 : expr >= 0;
  dimension_type num_vars = 0;
  dimension_type i = 0;
  dimension_type j = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(term);
  if (Octagonal_Shape_Helper
    ::extract_octagonal_difference(c, c.space_dimension(), num_vars,
                                   i, j, coeff, term)) {
    if (num_vars == 0)
      return true;
    // Select the cell to be checked.
    typename OR_Matrix<N>::const_row_iterator i_iter = matrix.row_begin() + i;
    typename OR_Matrix<N>::const_row_reference_type m_i = *i_iter;
    return !is_plus_infinity(m_i[j]);
  }
  else {
    // `c' is not an octagonal constraint: use the MIP solver.
    Optimization_Mode mode_bounds =
      from_above ? MAXIMIZATION : MINIMIZATION;
    MIP_Problem mip(space_dim, constraints(), expr, mode_bounds);
    return mip.solve() == OPTIMIZED_MIP_PROBLEM;
  }
}

template <typename T>
bool
Octagonal_Shape<T>::max_min(const Linear_Expression& expr,
                            const bool maximize,
                            Coefficient& ext_n, Coefficient& ext_d,
                            bool& included) const {
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible((maximize
                                  ? "maximize(e, ...)"
                                  : "minimize(e, ...)"), "e", expr);
  // Deal with zero-dim octagons first.
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

  strong_closure_assign();
  // For an empty OS we simply return false.
  if (marked_empty())
    return false;

  // The constraint `c' is used to check if `expr' is an octagonal difference
  // and, in this case, to select the cell.
  const Constraint& c = (maximize) ? expr <= 0 : expr >= 0;
  dimension_type num_vars = 0;
  dimension_type i = 0;
  dimension_type j = 0;
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(term);
  if (!Octagonal_Shape_Helper
    ::extract_octagonal_difference(c, c.space_dimension(), num_vars,
                                   i, j, coeff, term)) {
    // `c' is not an octagonal constraint: use the MIP solver.
    Optimization_Mode max_min = (maximize) ? MAXIMIZATION : MINIMIZATION;
    MIP_Problem mip(space_dim, constraints(), expr, max_min);
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
    // `c' is an octagonal constraint.
    if (num_vars == 0) {
      ext_n = expr.inhomogeneous_term();
      ext_d = 1;
      included = true;
      return true;
    }

    // Select the cell to be checked.
    typename OR_Matrix<N>::const_row_iterator i_iter = matrix.row_begin() + i;
    typename OR_Matrix<N>::const_row_reference_type m_i = *i_iter;
    PPL_DIRTY_TEMP(N, d);
    if (!is_plus_infinity(m_i[j])) {
      const Coefficient& b = expr.inhomogeneous_term();
      PPL_DIRTY_TEMP_COEFFICIENT(minus_b);
      neg_assign(minus_b, b);
      const Coefficient& sc_b = maximize ? b : minus_b;
      assign_r(d, sc_b, ROUND_UP);
      // Set `coeff_expr' to the absolute value of coefficient of a variable
      // of `expr'.
      PPL_DIRTY_TEMP(N, coeff_expr);
      const Coefficient& coeff_i = expr.coefficient(Variable(i/2));
      const int sign_i = sgn(coeff_i);
      if (sign_i > 0)
        assign_r(coeff_expr, coeff_i, ROUND_UP);
      else {
        PPL_DIRTY_TEMP_COEFFICIENT(minus_coeff_i);
        neg_assign(minus_coeff_i, coeff_i);
        assign_r(coeff_expr, minus_coeff_i, ROUND_UP);
      }
      // Approximating the maximum/minimum of `expr'.
      if (num_vars == 1) {
        PPL_DIRTY_TEMP(N, m_i_j);
        div_2exp_assign_r(m_i_j, m_i[j], 1, ROUND_UP);
        add_mul_assign_r(d, coeff_expr, m_i_j, ROUND_UP);
      }
      else
        add_mul_assign_r(d, coeff_expr, m_i[j], ROUND_UP);
      numer_denom(d, ext_n, ext_d);
      if (!maximize)
        neg_assign(ext_n);
      included = true;
      return true;
    }

    // The `expr' is unbounded.
    return false;
  }
}

template <typename T>
bool
Octagonal_Shape<T>::max_min(const Linear_Expression& expr,
                            const bool maximize,
                            Coefficient& ext_n, Coefficient& ext_d,
                            bool& included, Generator& g) const {
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible((maximize
                                  ? "maximize(e, ...)"
                                  : "minimize(e, ...)"), "e", expr);
  // Deal with zero-dim octagons first.
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

  strong_closure_assign();
  // For an empty OS we simply return false.
  if (marked_empty())
    return false;
  if (!is_universe()) {
    // We use MIP_Problems to handle constraints that are not
    // octagonal difference.
    Optimization_Mode max_min = (maximize) ? MAXIMIZATION : MINIMIZATION;
    MIP_Problem mip(space_dim, constraints(), expr, max_min);
    if (mip.solve() == OPTIMIZED_MIP_PROBLEM) {
      g = mip.optimizing_point();
      mip.evaluate_objective_function(g, ext_n, ext_d);
      included = true;
      return true;
    }
  }
  // The `expr' is unbounded.
  return false;
}

template <typename T>
Poly_Con_Relation
Octagonal_Shape<T>::relation_with(const Congruence& cg) const {
  dimension_type cg_space_dim = cg.space_dimension();

  // Dimension-compatibility check.
  if (cg_space_dim > space_dim)
    throw_dimension_incompatible("relation_with(cg)", cg);

  // If the congruence is an equality,
  // find the relation with the equivalent equality constraint.
  if (cg.is_equality()) {
    Constraint c(cg);
    return relation_with(c);
  }

  strong_closure_assign();

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
  Linear_Expression le(cg.expression());
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
Octagonal_Shape<T>::relation_with(const Constraint& c) const {
  dimension_type c_space_dim = c.space_dimension();

  // Dimension-compatibility check.
  if (c_space_dim > space_dim)
    throw_dimension_incompatible("relation_with(c)", c);

  // The closure needs to make explicit the implicit constraints.
  strong_closure_assign();

  if (marked_empty())
    return Poly_Con_Relation::saturates()
      && Poly_Con_Relation::is_included()
      && Poly_Con_Relation::is_disjoint();

  if (space_dim == 0) {
    // Trivially false zero-dimensional constraint.
    if ((c.is_equality() && c.inhomogeneous_term() != 0)
        || (c.is_inequality() && c.inhomogeneous_term() < 0))
      return Poly_Con_Relation::is_disjoint();
    else if (c.is_strict_inequality() && c.inhomogeneous_term() == 0)
      // The constraint 0 > 0 implicitly defines the hyperplane 0 = 0;
      // thus, the zero-dimensional point also saturates it.
      return Poly_Con_Relation::saturates()
        && Poly_Con_Relation::is_disjoint();

    // Trivially true zero-dimensional constraint.
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
  PPL_DIRTY_TEMP_COEFFICIENT(c_term);
  if (!Octagonal_Shape_Helper
    ::extract_octagonal_difference(c, c_space_dim, num_vars,
                                   i, j, coeff, c_term)) {
    // Constraints that are not octagonal differences.
    // Use maximize() and minimize() to do much of the work.

    // Find the linear expression for the constraint and use that to
    // find if the expression is bounded from above or below and if it
    // is, find the maximum and minimum values.
    Linear_Expression le;
    le.set_space_dimension(c.space_dimension());
    le.linear_combine(c.expr, Coefficient_one(), Coefficient_one(),
                      1, c_space_dim + 1);

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

  // Select the cell to be checked for the "<=" part of constraint.
  typename OR_Matrix<N>::const_row_iterator i_iter = matrix.row_begin() + i;
  typename OR_Matrix<N>::const_row_reference_type m_i = *i_iter;
  const N& m_i_j = m_i[j];
  // Set `coeff' to the absolute value of itself.
  if (coeff < 0)
    neg_assign(coeff);

  // Select the cell to be checked for the ">=" part of constraint.
  // Select the right row of the cell.
  if (i % 2 == 0)
    ++i_iter;
  else
    --i_iter;
  typename OR_Matrix<N>::const_row_reference_type m_ci = *i_iter;
  using namespace Implementation::Octagonal_Shapes;
  const N& m_ci_cj = m_ci[coherent_index(j)];
  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);
  // The following variables of mpq_class type are used to be precise
  // when the octagon is defined by integer constraints.
  PPL_DIRTY_TEMP(mpq_class, q_x);
  PPL_DIRTY_TEMP(mpq_class, q_y);
  PPL_DIRTY_TEMP(mpq_class, d);
  PPL_DIRTY_TEMP(mpq_class, d1);
  PPL_DIRTY_TEMP(mpq_class, c_denom);
  PPL_DIRTY_TEMP(mpq_class, q_denom);
  assign_r(c_denom, coeff, ROUND_NOT_NEEDED);
  assign_r(d, c_term, ROUND_NOT_NEEDED);
  neg_assign_r(d1, d, ROUND_NOT_NEEDED);
  div_assign_r(d, d, c_denom, ROUND_NOT_NEEDED);
  div_assign_r(d1, d1, c_denom, ROUND_NOT_NEEDED);

  if (is_plus_infinity(m_i_j)) {
    if (!is_plus_infinity(m_ci_cj)) {
      // `*this' is in the following form:
      // `-m_ci_cj <= v - u'.
      // In this case `*this' is disjoint from `c' if
      // `-m_ci_cj > d' (`-m_ci_cj >= d' if c is a strict inequality),
      // i.e., if `m_ci_cj < d1' (`m_ci_cj <= d1'
      // if c is a strict inequality).
      numer_denom(m_ci_cj, numer, denom);
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

  // Here `m_i_j' is not plus-infinity.
  numer_denom(m_i_j, numer, denom);
  assign_r(q_denom, denom, ROUND_NOT_NEEDED);
  assign_r(q_x, numer, ROUND_NOT_NEEDED);
  div_assign_r(q_x, q_x, q_denom, ROUND_NOT_NEEDED);

  if (!is_plus_infinity(m_ci_cj)) {
    numer_denom(m_ci_cj, numer, denom);
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
    // `m_ci_cj < d1' (`m_ci_cj <= d1' if `c' is a strict inequality).
    if (q_y < d1)
      return Poly_Con_Relation::is_disjoint();
    if (q_y == d1 && c.is_strict_inequality())
      return Poly_Con_Relation::is_disjoint();
  }

  // Here `m_ci_cj' can be also plus-infinity.
  // If `c' is an equality, `*this' is disjoint from `c' if
  // `m_i_j < d'.
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
Octagonal_Shape<T>::relation_with(const Generator& g) const {
  const dimension_type g_space_dim = g.space_dimension();

  // Dimension-compatibility check.
  if (space_dim < g_space_dim)
    throw_dimension_incompatible("relation_with(g)", g);

  // The closure needs to make explicit the implicit constraints and if the
  // octagon is empty.
  strong_closure_assign();

  // The empty octagon cannot subsume a generator.
  if (marked_empty())
    return Poly_Gen_Relation::nothing();

  // A universe octagon in a zero-dimensional space subsumes
  // all the generators of a zero-dimensional space.
  if (space_dim == 0)
    return Poly_Gen_Relation::subsumes();

  const bool is_line = g.is_line();
  const bool is_line_or_ray = g.is_line_or_ray();

  // The relation between the octagon and the given generator is obtained
  // checking if the generator satisfies all the constraints in the octagon.
  // To check if the generator satisfies all the constraints it's enough
  // studying the sign of the scalar product between the generator and
  // all the constraints in the octagon.

  typedef typename OR_Matrix<N>::const_row_iterator row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type row_reference;

  const row_iterator m_begin = matrix.row_begin();
  const row_iterator m_end = matrix.row_end();

  PPL_DIRTY_TEMP_COEFFICIENT(numer);
  PPL_DIRTY_TEMP_COEFFICIENT(denom);
  PPL_DIRTY_TEMP_COEFFICIENT(product);

  // We find in `*this' all the constraints.
  for (row_iterator i_iter = m_begin; i_iter != m_end; i_iter += 2) {
    dimension_type i = i_iter.index();
    row_reference m_i = *i_iter;
    row_reference m_ii = *(i_iter + 1);
    const N& m_i_ii = m_i[i + 1];
    const N& m_ii_i = m_ii[i];
    // We have the unary constraints.
    const Variable x(i/2);
    const Coefficient& g_coeff_x
      = (x.space_dimension() > g_space_dim)
      ? Coefficient_zero()
      : g.coefficient(x);
    if (is_additive_inverse(m_i_ii, m_ii_i)) {
      // The constraint has form ax = b.
      // To satisfy the constraint it is necessary that the scalar product
      // is not zero. The scalar product has the form
      // 'denom * g_coeff_x - numer * g.divisor()'.
      numer_denom(m_ii_i, numer, denom);
      denom *= 2;
      product = denom * g_coeff_x;
      // Note that if the generator `g' is a line or a ray,
      // its divisor is zero.
      if (!is_line_or_ray) {
        neg_assign(numer);
        add_mul_assign(product, numer, g.divisor());
      }
      if (product != 0)
        return Poly_Gen_Relation::nothing();
    }
    // We have 0, 1 or 2 inequality constraints.
    else {
      if (!is_plus_infinity(m_i_ii)) {
        // The constraint has form -ax <= b.
        // If the generator is a line it's necessary to check if
        // the scalar product is not zero, if it is positive otherwise.
        numer_denom(m_i_ii, numer, denom);
        denom *= -2;
        product = denom * g_coeff_x;
        // Note that if the generator `g' is a line or a ray,
        // its divisor is zero.
        if (!is_line_or_ray) {
          neg_assign(numer);
          add_mul_assign(product, numer, g.divisor());
        }
        if (is_line && product != 0)
          return Poly_Gen_Relation::nothing();
        else
          // If the generator is not a line it's necessary to check
          // that the scalar product sign is not positive and the scalar
          // product has the form
          // '-denom * g.coeff_x - numer * g.divisor()'.
          if (product > 0)
            return Poly_Gen_Relation::nothing();
      }
      if (!is_plus_infinity(m_ii_i)) {
        // The constraint has form ax <= b.
        numer_denom(m_ii_i, numer, denom);
        denom *= 2;
        product = denom * g_coeff_x;
         // Note that if the generator `g' is a line or a ray,
        // its divisor is zero.
        if (!is_line_or_ray) {
          neg_assign(numer);
          add_mul_assign(product, numer , g.divisor());
        }
        if (is_line && product != 0)
          return Poly_Gen_Relation::nothing();
        else
          // If the generator is not a line it's necessary to check
          // that the scalar product sign is not positive and the scalar
          // product has the form
          // 'denom * g_coeff_x - numer * g.divisor()'.
          if (product > 0)
            return Poly_Gen_Relation::nothing();
      }
    }
  }

  // We have the binary constraints.
  for (row_iterator i_iter = m_begin ; i_iter != m_end; i_iter += 2) {
    dimension_type i = i_iter.index();
    row_reference m_i = *i_iter;
    row_reference m_ii = *(i_iter + 1);
    for (dimension_type j = 0; j < i; j += 2) {
      const N& m_i_j = m_i[j];
      const N& m_ii_jj = m_ii[j + 1];
      const N& m_ii_j = m_ii[j];
      const N& m_i_jj = m_i[j + 1];
      const Variable x(j/2);
      const Variable y(i/2);
      const Coefficient& g_coeff_x
        = (x.space_dimension() > g_space_dim)
        ? Coefficient_zero()
        : g.coefficient(x);
      const Coefficient& g_coeff_y
        = (y.space_dimension() > g_space_dim)
        ? Coefficient_zero()
        : g.coefficient(y);

      const bool difference_is_equality = is_additive_inverse(m_ii_jj, m_i_j);
      if (difference_is_equality) {
        // The constraint has form a*x - a*y = b.
        // The scalar product has the form
        // 'denom * coeff_x - denom * coeff_y - numer * g.divisor()'.
        // To satisfy the constraint it's necessary that the scalar product
        // is not zero.
        numer_denom(m_i_j, numer, denom);
        product = denom * g_coeff_x;
        neg_assign(denom);
        add_mul_assign(product, denom, g_coeff_y);
        // Note that if the generator `g' is a line or a ray,
        // its divisor is zero.
        if (!is_line_or_ray) {
          neg_assign(numer);
          add_mul_assign(product, numer, g.divisor());
        }
        if (product != 0)
          return Poly_Gen_Relation::nothing();
      }
      else {
        if (!is_plus_infinity(m_i_j)) {
          // The constraint has form a*x - a*y <= b.
          // The scalar product has the form
          // 'denom * coeff_x - denom * coeff_y - numer * g.divisor()'.
          // If the generator is not a line it's necessary to check
          // that the scalar product sign is not positive.
          numer_denom(m_i_j, numer, denom);
          product = denom * g_coeff_x;
          neg_assign(denom);
          add_mul_assign(product, denom, g_coeff_y);
          // Note that if the generator `g' is a line or a ray,
          // its divisor is zero.
          if (!is_line_or_ray) {
            neg_assign(numer);
            add_mul_assign(product, numer, g.divisor());
          }
          if (is_line && product != 0)
            return Poly_Gen_Relation::nothing();
          else if (product > 0)
            return Poly_Gen_Relation::nothing();
        }
        if (!is_plus_infinity(m_ii_jj)) {
          // The constraint has form -a*x + a*y <= b.
          // The scalar product has the form
          // '-denom * coeff_x + denom * coeff_y - numer * g.divisor()'.
          // If the generator is not a line it's necessary to check
          // that the scalar product sign is not positive.
          numer_denom(m_ii_jj, numer, denom);
          product = denom * g_coeff_y;
          neg_assign(denom);
          add_mul_assign(product, denom, g_coeff_x);
          // Note that if the generator `g' is a line or a ray,
          // its divisor is zero.
          if (!is_line_or_ray) {
            neg_assign(numer);
            add_mul_assign(product, numer, g.divisor());
          }
          if (is_line && product != 0)
            return Poly_Gen_Relation::nothing();
          else if (product > 0)
            return Poly_Gen_Relation::nothing();
        }
      }

      const bool sum_is_equality = is_additive_inverse(m_i_jj, m_ii_j);
      if (sum_is_equality) {
        // The constraint has form a*x + a*y = b.
        // The scalar product has the form
        // 'denom * coeff_x + denom * coeff_y - numer * g.divisor()'.
        // To satisfy the constraint it's necessary that the scalar product
        // is not zero.
        numer_denom(m_ii_j, numer, denom);
        product = denom * g_coeff_x;
        add_mul_assign(product, denom, g_coeff_y);
        // Note that if the generator `g' is a line or a ray,
        // its divisor is zero.
        if (!is_line_or_ray) {
          neg_assign(numer);
          add_mul_assign(product, numer, g.divisor());
        }
        if (product != 0)
          return Poly_Gen_Relation::nothing();
      }
      else {
        if (!is_plus_infinity(m_i_jj)) {
          // The constraint has form -a*x - a*y <= b.
          // The scalar product has the form
          // '-denom * coeff_x - denom * coeff_y - numer * g.divisor()'.
          // If the generator is not a line it's necessary to check
          // that the scalar product sign is not positive.
          numer_denom(m_i_jj, numer, denom);
          neg_assign(denom);
          product = denom * g_coeff_x;
          add_mul_assign(product, denom, g_coeff_y);
          // Note that if the generator `g' is a line or a ray,
          // its divisor is zero.
          if (!is_line_or_ray) {
            neg_assign(numer);
            add_mul_assign(product, numer, g.divisor());
          }
          if (is_line && product != 0)
            return Poly_Gen_Relation::nothing();
          else if (product > 0)
            return Poly_Gen_Relation::nothing();
        }
        if (!is_plus_infinity(m_ii_j)) {
          // The constraint has form a*x + a*y <= b.
          // The scalar product has the form
          // 'denom * coeff_x + denom * coeff_y - numer * g.divisor()'.
          // If the generator is not a line it's necessary to check
          // that the scalar product sign is not positive.
          numer_denom(m_ii_j, numer, denom);
          product = denom * g_coeff_x;
          add_mul_assign(product, denom, g_coeff_y);
          // Note that if the generator `g' is a line or a ray,
          // its divisor is zero.
          if (!is_line_or_ray) {
            neg_assign(numer);
            add_mul_assign(product, numer, g.divisor());
          }
          if (is_line && product != 0)
            return Poly_Gen_Relation::nothing();
          else if (product > 0)
            return Poly_Gen_Relation::nothing();
        }
      }
    }
  }
  // If this point is reached the constraint 'g' satisfies
  // all the constraints in the octagon.
  return Poly_Gen_Relation::subsumes();
}

template <typename T>
void
Octagonal_Shape<T>::strong_closure_assign() const {
  // Do something only if necessary (zero-dim implies strong closure).
  if (marked_empty() || marked_strongly_closed() || space_dim == 0)
    return;

  // Even though the octagon will not change, its internal representation
  // is going to be modified by the closure algorithm.
  Octagonal_Shape& x = const_cast<Octagonal_Shape<T>&>(*this);

  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;

  const dimension_type n_rows = x.matrix.num_rows();
  const row_iterator m_begin = x.matrix.row_begin();
  const row_iterator m_end = x.matrix.row_end();

  // Fill the main diagonal with zeros.
  for (row_iterator i = m_begin; i != m_end; ++i) {
    PPL_ASSERT(is_plus_infinity((*i)[i.index()]));
    assign_r((*i)[i.index()], 0, ROUND_NOT_NEEDED);
  }

  // This algorithm is given by two steps: the first one is a simple
  // adaptation of the `shortest-path closure' using the Floyd-Warshall
  // algorithm; the second one is the `strong-coherence' algorithm.
  // It is important to note that after the strong-coherence,
  // the octagon is still shortest-path closed and hence, strongly closed.

  // Recall that, given an index `h', we indicate with `ch' the coherent
  // index, i.e., the index such that:
  //   ch = h + 1, if h is an even number;
  //   ch = h - 1, if h is an odd number.

  typename OR_Matrix<N>::element_iterator iter_ij;
  std::vector<N> vec_k(n_rows);
  std::vector<N> vec_ck(n_rows);
  PPL_DIRTY_TEMP(N, sum1);
  PPL_DIRTY_TEMP(N, sum2);
  row_reference x_k;
  row_reference x_ck;
  row_reference x_i;
  row_reference x_ci;

  // Since the index `j' of the inner loop will go from 0 up to `i',
  // the three nested loops have to be executed twice.
  for (int twice = 0; twice < 2; ++twice) {

    row_iterator x_k_iter = m_begin;
    row_iterator x_i_iter = m_begin;
    for (dimension_type k = 0; k < n_rows; k += 2) {
      const dimension_type ck = k + 1;
      // Re-initialize the element iterator.
      iter_ij = x.matrix.element_begin();
      // Compute the row references `x_k' and `x_ck'.
      x_k  = *x_k_iter;
      ++x_k_iter;
      x_ck = *x_k_iter;
      ++x_k_iter;

      for (dimension_type i = 0; i <= k; i += 2) {
        const dimension_type ci = i + 1;
        // Storing x_k_i == x_ci_ck.
        vec_k[i] = x_k[i];
        // Storing x_k_ci == x_i_ck.
        vec_k[ci] = x_k[ci];
        // Storing x_ck_i == x_ci_k.
        vec_ck[i] = x_ck[i];
        // Storing x_ck_ci == x_i_k.
        vec_ck[ci] = x_ck[ci];
      }
      x_i_iter = x_k_iter;
      for (dimension_type i = k + 2; i < n_rows; i += 2) {
        const dimension_type ci = i + 1;
        x_i = *x_i_iter;
        ++x_i_iter;
        x_ci = *x_i_iter;
        ++x_i_iter;
        // Storing x_k_i == x_ci_ck.
        vec_k[i] = x_ci[ck];
        // Storing x_k_ci == x_i_ck.
        vec_k[ci] = x_i[ck];
        // Storing x_ck_i == x_ci_k.
        vec_ck[i] = x_ci[k];
        // Storing x_ck_ci == x_i_k.
        vec_ck[ci] = x_i[k];
      }

      for (dimension_type i = 0; i < n_rows; ++i) {
        using namespace Implementation::Octagonal_Shapes;
        const dimension_type ci = coherent_index(i);
        const N& vec_k_ci = vec_k[ci];
        const N& vec_ck_ci = vec_ck[ci];
        // Unfolding two iterations on `j': this ensures that
        // the loop exit condition `j <= i' is OK.
        for (dimension_type j = 0; j <= i; ) {
          // First iteration: compute
          //
          // <CODE>
          //   sum1 = x_i_k + x_k_j == x_ck_ci + x_k_j;
          //   sum2 = x_i_ck + x_ck_j == x_k_ci + x_ck_j;
          // </CODE>
          add_assign_r(sum1, vec_ck_ci, vec_k[j], ROUND_UP);
          add_assign_r(sum2, vec_k_ci, vec_ck[j], ROUND_UP);
          min_assign(sum1, sum2);
          min_assign(*iter_ij, sum1);
          // Exiting the first iteration: loop index control.
          ++j;
          ++iter_ij;
          // Second iteration: ditto.
          add_assign_r(sum1, vec_ck_ci, vec_k[j], ROUND_UP);
          add_assign_r(sum2, vec_k_ci, vec_ck[j], ROUND_UP);
          min_assign(sum1, sum2);
          min_assign(*iter_ij, sum1);
          // Exiting the second iteration: loop index control.
          ++j;
          ++iter_ij;
        }
      }
    }
  }

  // Check for emptiness: the octagon is empty if and only if there is a
  // negative value in the main diagonal.
  for (row_iterator i = m_begin; i != m_end; ++i) {
    N& x_i_i = (*i)[i.index()];
    if (sgn(x_i_i) < 0) {
      x.set_empty();
      return;
    }
    else {
      PPL_ASSERT(sgn(x_i_i) == 0);
      // Restore PLUS_INFINITY on the main diagonal.
      assign_r(x_i_i, PLUS_INFINITY, ROUND_NOT_NEEDED);
    }
  }

  // Step 2: we enforce the strong coherence.
  x.strong_coherence_assign();
  // The octagon is not empty and it is now strongly closed.
  x.set_strongly_closed();
}

template <typename T>
void
Octagonal_Shape<T>::strong_coherence_assign() {
  // The strong-coherence is: for every indexes i and j
  // m_i_j <= (m_i_ci + m_cj_j)/2
  // where ci = i + 1, if i is even number or
  //       ci = i - 1, if i is odd.
  // Ditto for cj.
  PPL_DIRTY_TEMP(N, semi_sum);
  for (typename OR_Matrix<N>::row_iterator i_iter = matrix.row_begin(),
         i_end = matrix.row_end(); i_iter != i_end; ++i_iter) {
    typename OR_Matrix<N>::row_reference_type x_i = *i_iter;
    const dimension_type i = i_iter.index();
    using namespace Implementation::Octagonal_Shapes;
    const N& x_i_ci = x_i[coherent_index(i)];
    // Avoid to do unnecessary sums.
    if (!is_plus_infinity(x_i_ci))
      for (dimension_type j = 0, rs_i = i_iter.row_size(); j < rs_i; ++j)
        if (i != j) {
          const N& x_cj_j = matrix[coherent_index(j)][j];
          if (!is_plus_infinity(x_cj_j)) {
            add_assign_r(semi_sum, x_i_ci, x_cj_j, ROUND_UP);
            div_2exp_assign_r(semi_sum, semi_sum, 1, ROUND_UP);
            min_assign(x_i[j], semi_sum);
          }
        }
  }
}

template <typename T>
bool
Octagonal_Shape<T>::tight_coherence_would_make_empty() const {
  PPL_ASSERT(std::numeric_limits<N>::is_integer);
  PPL_ASSERT(marked_strongly_closed());
  const dimension_type space_dim = space_dimension();
  for (dimension_type i = 0; i < 2*space_dim; i += 2) {
    const dimension_type ci = i + 1;
    const N& mat_i_ci = matrix[i][ci];
    if (!is_plus_infinity(mat_i_ci)
        // Check for oddness of `mat_i_ci'.
        && !is_even(mat_i_ci)
        // Check for zero-equivalence of `i' and `ci'.
        && is_additive_inverse(mat_i_ci, matrix[ci][i]))
      return true;
  }
  return false;
}

template <typename T>
void
Octagonal_Shape<T>::tight_closure_assign() {
  PPL_COMPILE_TIME_CHECK(std::numeric_limits<T>::is_integer,
                         "Octagonal_Shape<T>::tight_closure_assign():"
                         " T in not an integer datatype.");
  // FIXME: this is just an executable specification.
  // (The following call could be replaced by shortest-path closure.)
  strong_closure_assign();
  if (marked_empty())
    return;
  if (tight_coherence_would_make_empty())
    set_empty();
  else {
    // Tighten the unary constraints.
    PPL_DIRTY_TEMP(N, temp_one);
    assign_r(temp_one, 1, ROUND_NOT_NEEDED);
    const dimension_type space_dim = space_dimension();
    for (dimension_type i = 0; i < 2*space_dim; i += 2) {
      const dimension_type ci = i + 1;
      N& mat_i_ci = matrix[i][ci];
      if (!is_plus_infinity(mat_i_ci) && !is_even(mat_i_ci))
        sub_assign_r(mat_i_ci, mat_i_ci, temp_one, ROUND_UP);
      N& mat_ci_i = matrix[ci][i];
      if (!is_plus_infinity(mat_ci_i) && !is_even(mat_ci_i))
        sub_assign_r(mat_ci_i, mat_ci_i, temp_one, ROUND_UP);
    }
    // Propagate tightened unary constraints.
    strong_coherence_assign();
  }
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>
::incremental_strong_closure_assign(const Variable var) const {
  // `var' should be one of the dimensions of the octagon.
  if (var.id() >= space_dim)
    throw_dimension_incompatible("incremental_strong_closure_assign(v)",
                                 var.id());

  // Do something only if necessary.
  if (marked_empty() || marked_strongly_closed())
    return;

  Octagonal_Shape& x = const_cast<Octagonal_Shape<T>&>(*this);

  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;

  const row_iterator m_begin = x.matrix.row_begin();
  const row_iterator m_end = x.matrix.row_end();

  // Fill the main diagonal with zeros.
  for (row_iterator i = m_begin; i != m_end; ++i) {
    PPL_ASSERT(is_plus_infinity((*i)[i.index()]));
    assign_r((*i)[i.index()], 0, ROUND_NOT_NEEDED);
  }

  // Using the incremental Floyd-Warshall algorithm.
  // Step 1: Improve all constraints on variable `var'.
  const dimension_type v = 2*var.id();
  const dimension_type cv = v + 1;
  row_iterator v_iter = m_begin + v;
  row_iterator cv_iter = v_iter + 1;
  row_reference x_v = *v_iter;
  row_reference x_cv = *cv_iter;
  const dimension_type rs_v = v_iter.row_size();
  const dimension_type n_rows = x.matrix.num_rows();
  PPL_DIRTY_TEMP(N, sum);
  using namespace Implementation::Octagonal_Shapes;
  for (row_iterator k_iter = m_begin; k_iter != m_end; ++k_iter) {
    const dimension_type k = k_iter.index();
    const dimension_type ck = coherent_index(k);
    const dimension_type rs_k = k_iter.row_size();
    row_reference x_k = *k_iter;
    row_reference x_ck = (k % 2 != 0) ? *(k_iter-1) : *(k_iter + 1);

    for (row_iterator i_iter = m_begin; i_iter != m_end; ++i_iter) {
      const dimension_type i = i_iter.index();
      const dimension_type ci = coherent_index(i);
      const dimension_type rs_i = i_iter.row_size();
      row_reference x_i = *i_iter;
      row_reference x_ci = (i % 2 != 0) ? *(i_iter-1) : *(i_iter + 1);

      const N& x_i_k = (k < rs_i) ? x_i[k] : x_ck[ci];
      if (!is_plus_infinity(x_i_k)) {
        const N& x_k_v = (v < rs_k) ? x_k[v] : x_cv[ck];
        if (!is_plus_infinity(x_k_v)) {
          add_assign_r(sum, x_i_k, x_k_v, ROUND_UP);
          N& x_i_v = (v < rs_i) ? x_i[v] : x_cv[ci];
          min_assign(x_i_v, sum);
        }
        const N& x_k_cv = (cv < rs_k) ? x_k[cv] : x_v[ck];
        if (!is_plus_infinity(x_k_cv)) {
          add_assign_r(sum, x_i_k, x_k_cv, ROUND_UP);
          N& x_i_cv = (cv < rs_i) ? x_i[cv] : x_v[ci];
          min_assign(x_i_cv, sum);
        }
      }
      const N& x_k_i = (i < rs_k) ? x_k[i] : x_ci[ck];
      if (!is_plus_infinity(x_k_i)) {
        const N& x_v_k = (k < rs_v) ? x_v[k] : x_ck[cv];
        if (!is_plus_infinity(x_v_k)) {
          N& x_v_i = (i < rs_v) ? x_v[i] : x_ci[cv];
          add_assign_r(sum, x_v_k, x_k_i, ROUND_UP);
          min_assign(x_v_i, sum);
        }
        const N& x_cv_k = (k < rs_v) ? x_cv[k] : x_ck[v];
        if (!is_plus_infinity(x_cv_k)) {
          N& x_cv_i = (i < rs_v) ? x_cv[i] : x_ci[v];
          add_assign_r(sum, x_cv_k, x_k_i, ROUND_UP);
          min_assign(x_cv_i, sum);
        }
      }

    }
  }

  // Step 2: improve the other bounds by using the precise bounds
  // for the constraints on `var'.
  for (row_iterator i_iter = m_begin; i_iter != m_end; ++i_iter) {
    const dimension_type i = i_iter.index();
    const dimension_type ci = coherent_index(i);
    const dimension_type rs_i = i_iter.row_size();
    row_reference x_i = *i_iter;
    const N& x_i_v = (v < rs_i) ? x_i[v] : x_cv[ci];
    // TODO: see if it is possible to optimize this inner loop
    // by splitting it into several parts, so as to avoid
    // conditional expressions.
    for (dimension_type j = 0; j < n_rows; ++j) {
      const dimension_type cj = coherent_index(j);
      row_reference x_cj = *(m_begin + cj);
      N& x_i_j = (j < rs_i) ? x_i[j] : x_cj[ci];
      if (!is_plus_infinity(x_i_v)) {
        const N& x_v_j = (j < rs_v) ? x_v[j] : x_cj[cv];
        if (!is_plus_infinity(x_v_j)) {
          add_assign_r(sum, x_i_v, x_v_j, ROUND_UP);
          min_assign(x_i_j, sum);
        }
      }
      const N& x_i_cv = (cv < rs_i) ? x_i[cv] : x_v[ci];
      if (!is_plus_infinity(x_i_cv)) {
        const N& x_cv_j = (j < rs_v) ? x_cv[j] : x_cj[v];
        if (!is_plus_infinity(x_cv_j)) {
          add_assign_r(sum, x_i_cv, x_cv_j, ROUND_UP);
          min_assign(x_i_j, sum);
        }
      }
    }
  }

  // Check for emptiness: the octagon is empty if and only if there is a
  // negative value on the main diagonal.
  for (row_iterator i = m_begin; i != m_end; ++i) {
    N& x_i_i = (*i)[i.index()];
    if (sgn(x_i_i) < 0) {
      x.set_empty();
      return;
    }
    else {
      // Restore PLUS_INFINITY on the main diagonal.
      PPL_ASSERT(sgn(x_i_i) == 0);
      assign_r(x_i_i, PLUS_INFINITY, ROUND_NOT_NEEDED);
    }
  }

  // Step 3: we enforce the strong coherence.
  x.strong_coherence_assign();
  // The octagon is not empty and it is now strongly closed.
  x.set_strongly_closed();
}

template <typename T>
void
Octagonal_Shape<T>
::compute_successors(std::vector<dimension_type>& successor) const {
  PPL_ASSERT(!marked_empty() && marked_strongly_closed());
  PPL_ASSERT(successor.size() == 0);
  // Variables are ordered according to their index.
  // The vector `successor' is used to indicate which variable
  // immediately follows a given one in the corresponding equivalence class.
  const dimension_type successor_size = matrix.num_rows();
  // Initially, each variable is successor of its own zero-equivalence class.
  successor.reserve(successor_size);
  for (dimension_type i = 0; i < successor_size; ++i)
    successor.push_back(i);
  // Now compute actual successors.
  for (dimension_type i = successor_size; i-- > 0; )  {
    typename OR_Matrix<N>::const_row_iterator i_iter = matrix.row_begin() + i;
    typename OR_Matrix<N>::const_row_reference_type m_i = *i_iter;
    typename OR_Matrix<N>::const_row_reference_type m_ci
      = (i % 2 != 0) ? *(i_iter-1) : *(i_iter + 1);
    for (dimension_type j = 0; j < i; ++j) {
      // FIXME: what is the following, commented-out for?
    //for (dimension_type j = i; j-- > 0; ) {
      using namespace Implementation::Octagonal_Shapes;
      dimension_type cj = coherent_index(j);
      if (is_additive_inverse(m_ci[cj], m_i[j]))
        // Choose as successor the variable having the greatest index.
        successor[j] = i;
    }
  }
}

template <typename T>
void
Octagonal_Shape<T>
::compute_leaders(std::vector<dimension_type>& leaders) const {
  PPL_ASSERT(!marked_empty() && marked_strongly_closed());
  PPL_ASSERT(leaders.size() == 0);
  // Variables are ordered according to their index.
  // The vector `leaders' is used to indicate the smallest variable
  // that belongs to the corresponding equivalence class.
  const dimension_type leader_size = matrix.num_rows();
  // Initially, each variable is leader of its own zero-equivalence class.
  leaders.reserve(leader_size);
  for (dimension_type i = 0; i < leader_size; ++i)
    leaders.push_back(i);
  // Now compute actual leaders.
  for (typename OR_Matrix<N>::const_row_iterator i_iter = matrix.row_begin(),
         matrix_row_end = matrix.row_end();
       i_iter != matrix_row_end; ++i_iter) {
    typename OR_Matrix<N>::const_row_reference_type m_i = *i_iter;
    dimension_type i = i_iter.index();
    typename OR_Matrix<N>::const_row_reference_type m_ci
      = (i % 2 != 0) ? *(i_iter-1) : *(i_iter + 1);
    for (dimension_type j = 0; j < i; ++j) {
      using namespace Implementation::Octagonal_Shapes;
      dimension_type cj = coherent_index(j);
      if (is_additive_inverse(m_ci[cj], m_i[j]))
        // Choose as leader the variable having the smaller index.
        leaders[i] = leaders[j];
    }
  }
}

template <typename T>
void
Octagonal_Shape<T>
::compute_leaders(std::vector<dimension_type>& successor,
                  std::vector<dimension_type>& no_sing_leaders,
                  bool& exist_sing_class,
                  dimension_type& sing_leader) const {
  PPL_ASSERT(!marked_empty() && marked_strongly_closed());
  PPL_ASSERT(no_sing_leaders.size() == 0);
  dimension_type successor_size = successor.size();
  std::deque<bool> dealt_with(successor_size, false);
  for (dimension_type i = 0; i < successor_size; ++i) {
    dimension_type next_i = successor[i];
    if (!dealt_with[i]) {
      // The index is a leader.
      // Now check if it is a leader of a singular class or not.
      using namespace Implementation::Octagonal_Shapes;
      if (next_i == coherent_index(i)) {
        exist_sing_class = true;
        sing_leader = i;
      }
      else
        no_sing_leaders.push_back(i);
    }
    // The following index is not a leader.
    dealt_with[next_i] = true;
  }
}

template <typename T>
void
Octagonal_Shape<T>::strong_reduction_assign() const {
  // Zero-dimensional octagonal shapes are necessarily reduced.
  if (space_dim == 0)
    return;
  strong_closure_assign();
  // If `*this' is empty, then there is nothing to reduce.
  if (marked_empty())
    return;

  // Detect non-redundant constraints.
  std::vector<Bit_Row> non_red;
  non_redundant_matrix_entries(non_red);

  // Throw away redundant constraints.
  Octagonal_Shape<T>& x = const_cast<Octagonal_Shape<T>&>(*this);
#ifndef NDEBUG
  const Octagonal_Shape x_copy_before(x);
#endif
  typename OR_Matrix<N>::element_iterator x_i = x.matrix.element_begin();
  for (dimension_type i = 0; i < 2 * space_dim; ++i) {
    const Bit_Row& non_red_i = non_red[i];
    for (dimension_type j = 0,
           j_end = OR_Matrix<N>::row_size(i); j < j_end; ++j, ++x_i) {
      if (!non_red_i[j])
        assign_r(*x_i, PLUS_INFINITY, ROUND_NOT_NEEDED);
    }
  }
  x.reset_strongly_closed();
#ifndef NDEBUG
  const Octagonal_Shape x_copy_after(x);
  PPL_ASSERT(x_copy_before == x_copy_after);
  PPL_ASSERT(x.is_strongly_reduced());
  PPL_ASSERT(x.OK());
#endif
}

template <typename T>
void
Octagonal_Shape<T>
::non_redundant_matrix_entries(std::vector<Bit_Row>& non_redundant) const {
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(space_dim > 0 && !marked_empty() && marked_strongly_closed());
  PPL_ASSERT(non_redundant.empty());

  // Initialize `non_redundant' as if it was an OR_Matrix of booleans
  // (initially set to false).
  non_redundant.resize(2*space_dim);

  // Step 1: compute zero-equivalence classes.
  // Variables corresponding to indices `i' and `j' are zero-equivalent
  // if they lie on a zero-weight loop; since the matrix is strongly
  // closed, this happens if and only if matrix[i][j] == -matrix[ci][cj].
  std::vector<dimension_type> no_sing_leaders;
  dimension_type sing_leader = 0;
  bool exist_sing_class = false;
  std::vector<dimension_type> successor;
  compute_successors(successor);
  compute_leaders(successor, no_sing_leaders, exist_sing_class, sing_leader);
  const dimension_type num_no_sing_leaders = no_sing_leaders.size();


  // Step 2: flag redundant constraints in `redundancy'.
  // Go through non-singular leaders first.
  for (dimension_type li = 0; li < num_no_sing_leaders; ++li) {
    const dimension_type i = no_sing_leaders[li];
    using namespace Implementation::Octagonal_Shapes;
    const dimension_type ci = coherent_index(i);
    typename OR_Matrix<N>::const_row_reference_type
      m_i = *(matrix.row_begin() + i);
    if (i % 2 == 0) {
      // Each positive equivalence class must have a single 0-cycle
      // connecting all equivalent variables in increasing order.
      // Note: by coherence assumption, the variables in the
      // corresponding negative equivalence class are
      // automatically connected.
      if (i != successor[i]) {
        dimension_type j = i;
        dimension_type next_j = successor[j];
        while (j != next_j) {
          non_redundant[next_j].set(j);
          j = next_j;
          next_j = successor[j];
        }
        const dimension_type cj = coherent_index(j);
        non_redundant[cj].set(ci);
      }
    }

    dimension_type rs_li = (li % 2 != 0) ? li : (li + 1);
    // Check if the constraint is redundant.
    PPL_DIRTY_TEMP(N, tmp);
    for (dimension_type lj = 0 ; lj <= rs_li; ++lj) {
      const dimension_type j = no_sing_leaders[lj];
      const dimension_type cj = coherent_index(j);
      const N& m_i_j = m_i[j];
      const N& m_i_ci = m_i[ci];
      bool to_add = true;
      // Control if the constraint is redundant by strong-coherence,
      // that is:
      // m_i_j >= (m_i_ci + m_cj_j)/2,   where j != ci.
      if (j != ci) {
        add_assign_r(tmp, m_i_ci, matrix[cj][j], ROUND_UP);
        div_2exp_assign_r(tmp, tmp, 1, ROUND_UP);
        if (m_i_j >= tmp)
          // The constraint is redundant.
          continue;
      }
      // Control if the constraint is redundant by strong closure, that is
      // if there is a path from i to j (i = i_0, ... , i_n = j), such that
      // m_i_j = sum_{k=0}^{n-1} m_{i_k}_{i_(k + 1)}.
      // Since the octagon is already strongly closed, the above relation
      // is reduced to three case, in accordance with k, i, j inter-depend:
      // exit k such that
      // 1.) m_i_j >= m_i_k   + m_cj_ck,   if k < j < i; or
      // 2.) m_i_j >= m_i_k   + m_k,_j,    if j < k < i; or
      // 3.) m_i_j >= m_ck_ci + m_k_j,     if j < i < k.
      // Note: `i > j'.
      for (dimension_type lk = 0; lk < num_no_sing_leaders; ++lk) {
        const dimension_type k = no_sing_leaders[lk];
        if (k != i && k != j) {
          dimension_type ck = coherent_index(k);
          if (k < j)
            // Case 1.
            add_assign_r(tmp, m_i[k], matrix[cj][ck], ROUND_UP);
          else if (k < i)
            // Case 2.
            add_assign_r(tmp, m_i[k], matrix[k][j], ROUND_UP);
          else
            // Case 3.
            add_assign_r(tmp, matrix[ck][ci], matrix[k][j], ROUND_UP);

          // Checks if the constraint is redundant.
          if (m_i_j >= tmp) {
            to_add = false;
            break;
          }
        }
      }

      if (to_add)
        // The constraint is not redundant.
        non_redundant[i].set(j);
    }
  }

  // If there exist a singular equivalence class, then it must have a
  // single 0-cycle connecting all the positive and negative equivalent
  // variables.
  // Note: the singular class is not connected with the other classes.
  if (exist_sing_class) {
    non_redundant[sing_leader].set(sing_leader + 1);
    if (successor[sing_leader + 1] != sing_leader + 1) {
      dimension_type j = sing_leader;
      dimension_type next_j = successor[j + 1];
      while (next_j != j + 1) {
        non_redundant[next_j].set(j);
        j = next_j;
        next_j = successor[j + 1];
      }
      non_redundant[j + 1].set(j);
    }
    else
      non_redundant[sing_leader + 1].set(sing_leader);
  }
}

template <typename T>
void
Octagonal_Shape<T>::upper_bound_assign(const Octagonal_Shape& y) {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("upper_bound_assign(y)", y);

  // The hull of an octagon `x' with an empty octagon is `x'.
  y.strong_closure_assign();
  if (y.marked_empty())
    return;
  strong_closure_assign();
  if (marked_empty()) {
    *this = y;
    return;
  }

  // The oct-hull is obtained by computing maxima.
  typename OR_Matrix<N>::const_element_iterator j = y.matrix.element_begin();
  for (typename OR_Matrix<N>::element_iterator i = matrix.element_begin(),
         matrix_element_end = matrix.element_end();
       i != matrix_element_end; ++i, ++j)
    max_assign(*i, *j);

  // The result is still closed.
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::difference_assign(const Octagonal_Shape& y) {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("difference_assign(y)", y);

  Octagonal_Shape& x = *this;

  // Being lazy here is only harmful.
  // We close.
  x.strong_closure_assign();
  // The difference of an empty octagon and of an octagon `p' is empty.
  if (x.marked_empty())
    return;
  // The difference of a octagon `p' and an empty octagon is `p'.
  if (y.marked_empty())
    return;

  // If both octagons are zero-dimensional,
  // then at this point they are necessarily universe octagons,
  // so that their difference is empty.
  if (x.space_dim == 0) {
    x.set_empty();
    return;
  }

  // TODO: This is just an executable specification.
  //       Have to find a more efficient method.
  if (y.contains(x)) {
    x.set_empty();
    return;
  }

  Octagonal_Shape new_oct(space_dim, EMPTY);
  // We take a constraint of the octagon y at the time and we
  // consider its complementary. Then we intersect the union
  // of these complementary constraints with the octagon x.
  const Constraint_System& y_cs = y.constraints();
  for (Constraint_System::const_iterator i = y_cs.begin(),
         y_cs_end = y_cs.end(); i != y_cs_end; ++i) {
    const Constraint& c = *i;
    // If the octagon `x' is included the octagon defined by `c',
    // then `c' _must_ be skipped, as adding its complement to `x'
    // would result in the empty octagon, and as we would obtain
    // a result that is less precise than the difference.
    if (x.relation_with(c).implies(Poly_Con_Relation::is_included()))
      continue;
    Octagonal_Shape z = x;
    const Linear_Expression e(c.expression());
    z.add_constraint(e <= 0);
    if (!z.is_empty())
      new_oct.upper_bound_assign(z);
    if (c.is_equality()) {
      z = x;
      z.add_constraint(e >= 0);
      if (!z.is_empty())
        new_oct.upper_bound_assign(z);
    }
  }
  *this = new_oct;
  PPL_ASSERT(OK());
}

template <typename T>
bool
Octagonal_Shape<T>::simplify_using_context_assign(const Octagonal_Shape& y) {
  Octagonal_Shape& x = *this;
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
  if (x.contains(y)) {
    Octagonal_Shape<T> res(dim, UNIVERSE);
    x.m_swap(res);
    return false;
  }

  typedef typename OR_Matrix<N>::row_iterator Row_Iter;
  typedef typename OR_Matrix<N>::const_row_iterator Row_CIter;
  typedef typename OR_Matrix<N>::element_iterator Elem_Iter;
  typedef typename OR_Matrix<N>::const_element_iterator Elem_CIter;

  // Filter away the case where `x' is empty.
  x.strong_closure_assign();
  if (x.marked_empty()) {
    // Search for a constraint of `y' that is not a tautology.
    dimension_type i;
    dimension_type j;
    // Prefer unary constraints.
    for (i = 0; i < 2*dim; i += 2) {
      // FIXME: if N is a float or bounded integer type, then
      // we also need to check that we are actually able to construct
      // a constraint inconsistent with respect to this one.
      // Use something like !is_maximal()?
      if (!is_plus_infinity(y.matrix_at(i, i + 1))) {
        j = i + 1;
        goto found;
      }
      // Use something like !is_maximal()?
      if (!is_plus_infinity(y.matrix_at(i + 1, i))) {
        j = i;
        ++i;
        goto found;
      }
    }
    // Then search binary constraints.
    // TODO: use better iteration scheme.
    for (i = 2; i < 2*dim; ++i)
      for (j = 0; j < i; ++j) {
        // Use something like !is_maximal()?
        if (!is_plus_infinity(y.matrix_at(i, j)))
          goto found;
      }

    // Not found: we were not able to build a constraint contradicting
    // one of the constraints in `y': `x' cannot be enlarged.
    return false;

  found:
    // Found: build a new OS contradicting the constraint found.
    PPL_ASSERT(i < dim && j < dim && i != j);
    Octagonal_Shape<T> res(dim, UNIVERSE);
    // FIXME: compute a proper contradicting constraint.
    PPL_DIRTY_TEMP(N, tmp);
    assign_r(tmp, 1, ROUND_UP);
    add_assign_r(tmp, tmp, y.matrix_at(i, j), ROUND_UP);
    // CHECKME: round down is really meant.
    neg_assign_r(res.matrix_at(j, i), tmp, ROUND_DOWN);
    PPL_ASSERT(!is_plus_infinity(res.matrix_at(j, i)));
    x.m_swap(res);
    return false;
  }

  // Here `x' and `y' are not empty and strongly closed;
  // also, `x' does not contain `y'.
  // Let `target' be the intersection of `x' and `y'.
  Octagonal_Shape<T> target = x;
  target.intersection_assign(y);
  const bool bool_result = !target.is_empty();

  // Compute redundancy information for x and ...
  // TODO: provide a nicer data structure for redundancy.
  std::vector<Bit_Row> x_non_redundant;
  x.non_redundant_matrix_entries(x_non_redundant);
  // ... count the non-redundant constraints.
  dimension_type x_num_non_redundant = 0;
  for (size_t i = x_non_redundant.size(); i-- > 0 ; )
    x_num_non_redundant += x_non_redundant[i].count_ones();
  PPL_ASSERT(x_num_non_redundant > 0);

  // Let `yy' be a copy of `y': we will keep adding to `yy'
  // the non-redundant constraints of `x',
  // stopping as soon as `yy' becomes equal to `target'.
  Octagonal_Shape<T> yy = y;

  // The constraints added to `yy' will be recorded in `res' ...
  Octagonal_Shape<T> res(dim, UNIVERSE);
  // ... and we will count them too.
  dimension_type res_num_non_redundant = 0;

  // Compute leader information for `x'.
  std::vector<dimension_type> x_leaders;
  x.compute_leaders(x_leaders);

  // First go through the unary equality constraints.
  // Find the leader of the singular equivalence class (it is even!).
  dimension_type sing_leader;
  for (sing_leader = 0; sing_leader < 2*dim; sing_leader += 2) {
    if (sing_leader == x_leaders[sing_leader]) {
      const N& x_s_ss = x.matrix_at(sing_leader, sing_leader + 1);
      const N& x_ss_s = x.matrix_at(sing_leader + 1, sing_leader);
      if (is_additive_inverse(x_s_ss, x_ss_s))
        // Singular leader found.
        break;
    }
  }

  // Unary equalities have `sing_leader' as a leader.
  for (dimension_type i = sing_leader; i < 2*dim; i += 2) {
    if (x_leaders[i] != sing_leader)
      continue;
    // Found a unary equality constraint:
    // see if any of the two inequalities have to be added.
    const N& x_i_ii = x.matrix_at(i, i + 1);
    N& yy_i_ii = yy.matrix_at(i, i + 1);
    if (x_i_ii < yy_i_ii) {
      // The \leq inequality is not implied by context.
      res.matrix_at(i, i + 1) = x_i_ii;
      ++res_num_non_redundant;
      // Tighten context `yy' using the newly added constraint.
      yy_i_ii = x_i_ii;
      yy.reset_strongly_closed();
    }
    const N& x_ii_i = x.matrix_at(i + 1, i);
    N& yy_ii_i = yy.matrix_at(i + 1, i);
    if (x_ii_i < yy_ii_i) {
      // The \geq inequality is not implied by context.
      res.matrix_at(i + 1, i) = x_ii_i;
      ++res_num_non_redundant;
      // Tighten context `yy' using the newly added constraint.
      yy_ii_i = x_ii_i;
      yy.reset_strongly_closed();
    }
    // Restore strong closure, if it was lost.
    if (!yy.marked_strongly_closed()) {
      Variable var_i(i/2);
      yy.incremental_strong_closure_assign(var_i);
      if (target.contains(yy)) {
        // Target reached: swap `x' and `res' if needed.
        if (res_num_non_redundant < x_num_non_redundant) {
          res.reset_strongly_closed();
          x.m_swap(res);
        }
        return bool_result;
      }
    }
  }

  // Go through the binary equality constraints.
  for (dimension_type i = 0; i < 2*dim; ++i) {
    const dimension_type j = x_leaders[i];
    if (j == i || j == sing_leader)
      continue;
    const N& x_i_j = x.matrix_at(i, j);
    PPL_ASSERT(!is_plus_infinity(x_i_j));
    N& yy_i_j = yy.matrix_at(i, j);
    if (x_i_j < yy_i_j) {
      res.matrix_at(i, j) = x_i_j;
      ++res_num_non_redundant;
      // Tighten context `yy' using the newly added constraint.
      yy_i_j = x_i_j;
      yy.reset_strongly_closed();
    }
    const N& x_j_i = x.matrix_at(j, i);
    N& yy_j_i = yy.matrix_at(j, i);
    PPL_ASSERT(!is_plus_infinity(x_j_i));
    if (x_j_i < yy_j_i) {
      res.matrix_at(j, i) = x_j_i;
      ++res_num_non_redundant;
      // Tighten context `yy' using the newly added constraint.
      yy_j_i = x_j_i;
      yy.reset_strongly_closed();
    }
    // Restore strong closure, if it was lost.
    if (!yy.marked_strongly_closed()) {
      Variable var_j(j/2);
      yy.incremental_strong_closure_assign(var_j);
      if (target.contains(yy)) {
        // Target reached: swap `x' and `res' if needed.
        if (res_num_non_redundant < x_num_non_redundant) {
          res.reset_strongly_closed();
          x.m_swap(res);
        }
        return bool_result;
      }
    }
  }

  // Finally go through the (proper) inequality constraints:
  // both indices i and j should be leaders.
  // FIXME: improve iteration scheme (are we doing twice the work?)
  for (dimension_type i = 0; i < 2*dim; ++i) {
    if (i != x_leaders[i])
      continue;
    const Bit_Row& x_non_redundant_i = x_non_redundant[i];
    for (dimension_type j = 0; j < 2*dim; ++j) {
      if (j != x_leaders[j])
        continue;
      if (i >= j) {
        if (!x_non_redundant_i[j])
          continue;
      }
      else if (!x_non_redundant[j][i])
        continue;
      N& yy_i_j = yy.matrix_at(i, j);
      const N& x_i_j = x.matrix_at(i, j);
      if (x_i_j < yy_i_j) {
        res.matrix_at(i, j) = x_i_j;
        ++res_num_non_redundant;
        // Tighten context `yy' using the newly added constraint.
        yy_i_j = x_i_j;
        yy.reset_strongly_closed();
        Variable var(i/2);
        yy.incremental_strong_closure_assign(var);
        if (target.contains(yy)) {
          // Target reached: swap `x' and `res' if needed.
          if (res_num_non_redundant < x_num_non_redundant) {
            res.reset_strongly_closed();
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
Octagonal_Shape<T>::add_space_dimensions_and_embed(dimension_type m) {
  // Adding no dimensions is a no-op.
  if (m == 0)
    return;

  const dimension_type new_dim = space_dim + m;
  const bool was_zero_dim_univ = !marked_empty() && space_dim == 0;

  // To embed an n-dimension space octagon in a (n + m)-dimension space,
  // we just add `m' variables in the matrix of constraints.
  matrix.grow(new_dim);
  space_dim = new_dim;
  // If `*this' was the zero-dim space universe octagon,
  // then we can set the strongly closure flag.
  if (was_zero_dim_univ)
    set_strongly_closed();

  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::add_space_dimensions_and_project(dimension_type m) {
  // Adding no dimensions is a no-op.
  if (m == 0)
    return;

  const dimension_type n = matrix.num_rows();

  // To project an n-dimension space OS in a (space_dim + m)-dimension space,
  // we just add `m' columns and rows in the matrix of constraints.
  add_space_dimensions_and_embed(m);
  // We insert 0 where it needs.
  // Attention: now num_rows of matrix is update!
  for (typename OR_Matrix<N>::row_iterator i = matrix.row_begin() + n,
         matrix_row_end =  matrix.row_end(); i != matrix_row_end; i += 2) {
    typename OR_Matrix<N>::row_reference_type x_i = *i;
    typename OR_Matrix<N>::row_reference_type x_ci = *(i + 1);
    const dimension_type ind = i.index();
    assign_r(x_i[ind + 1], 0, ROUND_NOT_NEEDED);
    assign_r(x_ci[ind], 0, ROUND_NOT_NEEDED);
  }

  if (marked_strongly_closed())
    reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::remove_space_dimensions(const Variables_Set& vars) {
  // The removal of no dimensions from any octagon is a no-op.
  // Note that this case also captures the only legal removal of
  // dimensions from a octagon in a 0-dim space.
  if (vars.empty()) {
    PPL_ASSERT(OK());
    return;
  }

  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dim < min_space_dim)
    throw_dimension_incompatible("remove_space_dimensions(vs)", min_space_dim);

  const dimension_type new_space_dim = space_dim - vars.size();

  strong_closure_assign();
  // When removing _all_ dimensions from an octagon,
  // we obtain the zero-dimensional octagon.
  if (new_space_dim == 0) {
    matrix.shrink(0);
    if (!marked_empty())
      // We set the zero_dim_univ flag.
      set_zero_dim_univ();
    space_dim = 0;
    PPL_ASSERT(OK());
    return;
  }

  // We consider each variable and we check if it has to be removed.
  // If it has to be removed, we pass to the next one, then we will
  // overwrite its representation in the matrix.
  typedef typename OR_Matrix<N>::element_iterator Elem_Iter;
  typedef typename std::iterator_traits<Elem_Iter>::difference_type diff_t;

  dimension_type first = *vars.begin();
  const dimension_type first_size = 2 * first * (first + 1);
  Elem_Iter iter = matrix.element_begin() + static_cast<diff_t>(first_size);

  for (dimension_type i = first + 1; i < space_dim; ++i) {
    if (vars.count(i) == 0) {
      typename OR_Matrix<N>::row_iterator row_iter = matrix.row_begin() + 2*i;
      typename OR_Matrix<N>::row_reference_type row_ref = *row_iter;
      typename OR_Matrix<N>::row_reference_type row_ref1 = *(++row_iter);
      // Beware: first we shift the cells corresponding to the first
      // row of variable(j), then we shift the cells corresponding to the
      // second row. We recall that every variable is represented
      // in the `matrix' by two rows and two columns.
      for (dimension_type j = 0; j <= i; ++j)
        if (vars.count(j) == 0) {
          assign_or_swap(*(iter++), row_ref[2*j]);
          assign_or_swap(*(iter++), row_ref[2*j + 1]);
        }
      for (dimension_type j = 0; j <= i; ++j)
        if (vars.count(j) == 0) {
          assign_or_swap(*(iter++), row_ref1[2*j]);
          assign_or_swap(*(iter++), row_ref1[2*j + 1]);
        }
    }
  }
  // Update the space dimension.
  matrix.shrink(new_space_dim);
  space_dim = new_space_dim;
  PPL_ASSERT(OK());
}

template <typename T>
template <typename Partial_Function>
void
Octagonal_Shape<T>::map_space_dimensions(const Partial_Function& pfunc) {
  if (space_dim == 0)
    return;

  if (pfunc.has_empty_codomain()) {
    // All dimensions vanish: the octagon becomes zero_dimensional.
    remove_higher_space_dimensions(0);
    return;
  }

  const dimension_type new_space_dim = pfunc.max_in_codomain() + 1;
  // If we are going to actually reduce the space dimension,
  // then shortest-path closure is required to keep precision.
  if (new_space_dim < space_dim)
    strong_closure_assign();

  // If the octagon is empty, then it is sufficient to adjust
  // the space dimension of the octagon.
  if (marked_empty()) {
    remove_higher_space_dimensions(new_space_dim);
    return;
  }

  // We create a new matrix with the new space dimension.
  OR_Matrix<N> x(new_space_dim);

  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;

  row_iterator m_begin = x.row_begin();

  for (row_iterator i_iter = matrix.row_begin(), i_end = matrix.row_end();
       i_iter != i_end; i_iter += 2) {
    dimension_type new_i;
    dimension_type i = i_iter.index()/2;
    // We copy and place in the position into `x' the only cells of
    // the `matrix' that refer to both mapped variables,
    // the variable `i' and `j'.
    if (pfunc.maps(i, new_i)) {
      row_reference r_i = *i_iter;
      row_reference r_ii = *(i_iter + 1);
      dimension_type double_new_i = 2*new_i;
      row_iterator x_iter = m_begin + double_new_i;
      row_reference x_i = *x_iter;
      row_reference x_ii = *(x_iter + 1);
      for (dimension_type j = 0; j <= i; ++j) {
        dimension_type new_j;
        // If also the second variable is mapped, we work.
        if (pfunc.maps(j, new_j)) {
          dimension_type dj = 2*j;
          dimension_type double_new_j = 2*new_j;
          // Mapped the constraints, exchanging the indexes.
          // Attention: our matrix is pseudo-triangular.
          // If new_j > new_i, we must consider, as rows, the rows of
          // the variable new_j, and not of new_i ones.
          if (new_i >= new_j) {
            assign_or_swap(x_i[double_new_j], r_i[dj]);
            assign_or_swap(x_ii[double_new_j], r_ii[dj]);
            assign_or_swap(x_ii[double_new_j + 1], r_ii[dj + 1]);
            assign_or_swap(x_i[double_new_j + 1], r_i[dj + 1]);
          }
          else {
            row_iterator x_j_iter = m_begin + double_new_j;
            row_reference x_j = *x_j_iter;
            row_reference x_jj = *(x_j_iter + 1);
            assign_or_swap(x_jj[double_new_i + 1], r_i[dj]);
            assign_or_swap(x_jj[double_new_i], r_ii[dj]);
            assign_or_swap(x_j[double_new_i + 1], r_i[dj + 1]);
            assign_or_swap(x_j[double_new_i], r_ii[dj + 1]);
          }

        }
      }
    }
  }

  using std::swap;
  swap(matrix, x);
  space_dim = new_space_dim;
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::intersection_assign(const Octagonal_Shape& y) {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("intersection_assign(y)", y);

  // If one of the two octagons is empty, the intersection is empty.
  if (marked_empty())
    return;
  if (y.marked_empty()) {
    set_empty();
    return;
  }
  // If both octagons are zero-dimensional,then at this point
  // they are necessarily non-empty,
  // so that their intersection is non-empty too.
  if (space_dim == 0)
    return;

  // To intersect two octagons we compare the constraints
  // and we choose the less values.
  bool changed = false;

  typename OR_Matrix<N>::const_element_iterator j = y.matrix.element_begin();
  for (typename OR_Matrix<N>::element_iterator i = matrix.element_begin(),
         matrix_element_end = matrix.element_end();
       i != matrix_element_end;
       ++i, ++j) {
    N& elem = *i;
    const N& y_elem = *j;
    if (y_elem < elem) {
      elem = y_elem;
      changed = true;
    }
  }

  // This method not preserve the closure.
  if (changed && marked_strongly_closed())
    reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
template <typename Iterator>
void
Octagonal_Shape<T>::CC76_extrapolation_assign(const Octagonal_Shape& y,
                                              Iterator first, Iterator last,
                                              unsigned* tp) {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("CC76_extrapolation_assign(y)", y);

  // Assume `y' is contained in or equal to `*this'.
  PPL_EXPECT_HEAVY(copy_contains(*this, y));

  // If both octagons are zero-dimensional,
  // since `*this' contains `y', we simply return `*this'.
  if (space_dim == 0)
    return;

  strong_closure_assign();
  // If `*this' is empty, since `*this' contains `y', `y' is empty too.
  if (marked_empty())
    return;
  y.strong_closure_assign();
  // If `y' is empty, we return.
  if (y.marked_empty())
    return;

  // If there are tokens available, work on a temporary copy.
  if (tp != 0 && *tp > 0) {
    Octagonal_Shape x_tmp(*this);
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
  typename OR_Matrix<N>::const_element_iterator j = y.matrix.element_begin();
  for (typename OR_Matrix<N>::element_iterator i = matrix.element_begin(),
         matrix_element_end = matrix.element_end();
       i != matrix_element_end;
       ++i, ++j) {
    const N& y_elem = *j;
    N& elem = *i;
    if (y_elem < elem) {
      Iterator k = std::lower_bound(first, last, elem);
      if (k != last) {
        if (elem < *k)
          assign_r(elem, *k, ROUND_UP);
      }
      else
        assign_r(elem, PLUS_INFINITY, ROUND_NOT_NEEDED);
    }
  }

  reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>
::get_limiting_octagon(const Constraint_System& cs,
                       Octagonal_Shape& limiting_octagon) const {
  const dimension_type cs_space_dim = cs.space_dimension();
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(cs_space_dim <= space_dim);

  strong_closure_assign();
  bool is_oct_changed = false;

  // Allocate temporaries outside of the loop.
  PPL_DIRTY_TEMP_COEFFICIENT(coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(term);
  PPL_DIRTY_TEMP(N, d);

  for (Constraint_System::const_iterator cs_i = cs.begin(),
         cs_end = cs.end(); cs_i != cs_end; ++cs_i) {
    const Constraint& c = *cs_i;
    dimension_type num_vars = 0;
    dimension_type i = 0;
    dimension_type j = 0;
    // Constraints that are not octagonal differences are ignored.
    if (!Octagonal_Shape_Helper
      ::extract_octagonal_difference(c, cs_space_dim, num_vars, i, j,
                                     coeff, term))
      continue;

    typedef typename OR_Matrix<N>::const_row_iterator Row_iterator;
    typedef typename OR_Matrix<N>::const_row_reference_type Row_reference;
    typedef typename OR_Matrix<N>::row_iterator row_iterator;
    typedef typename OR_Matrix<N>::row_reference_type row_reference;
    Row_iterator m_begin = matrix.row_begin();
    // Select the cell to be modified for the "<=" part of the constraint.
    Row_iterator i_iter = m_begin + i;
    Row_reference m_i = *i_iter;
    OR_Matrix<N>& lo_mat = limiting_octagon.matrix;
    row_iterator lo_iter = lo_mat.row_begin() + i;
    row_reference lo_m_i = *lo_iter;
    N& lo_m_i_j = lo_m_i[j];
    if (coeff < 0)
      neg_assign(coeff);
    // Compute the bound for `m_i_j', rounding towards plus infinity.
    div_round_up(d, term, coeff);
    if (m_i[j] <= d)
      if (c.is_inequality()) {
        if (lo_m_i_j > d) {
          lo_m_i_j = d;
          is_oct_changed = true;
        }
        else {
          // Select the right row of the cell.
          if (i % 2 == 0) {
            ++i_iter;
            ++lo_iter;
          }
          else {
            --i_iter;
            --lo_iter;
          }
          Row_reference m_ci = *i_iter;
          row_reference lo_m_ci = *lo_iter;
          // Select the right column of the cell.
          using namespace Implementation::Octagonal_Shapes;
          dimension_type cj = coherent_index(j);
          N& lo_m_ci_cj = lo_m_ci[cj];
          neg_assign(term);
          div_round_up(d, term, coeff);
          if (m_ci[cj] <= d && lo_m_ci_cj > d) {
            lo_m_ci_cj = d;
            is_oct_changed = true;
          }
        }
      }
  }
  // In general, adding a constraint does not preserve the strongly
  // closure of the octagon.
  if (is_oct_changed && limiting_octagon.marked_strongly_closed())
    limiting_octagon.reset_strongly_closed();
}

template <typename T>
void
Octagonal_Shape<T>
::limited_CC76_extrapolation_assign(const Octagonal_Shape& y,
                                    const Constraint_System& cs,
                                    unsigned* tp) {

  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("limited_CC76_extrapolation_assign(y, cs)",
                                 y);
  // `cs' must be dimension-compatible with the two octagons.
  const dimension_type cs_space_dim = cs.space_dimension();
  if (space_dim < cs_space_dim)
    throw_constraint_incompatible("limited_CC76_extrapolation_assign(y, cs)");

  // Strict inequalities not allowed.
  if (cs.has_strict_inequalities())
    throw_constraint_incompatible("limited_CC76_extrapolation_assign(y, cs)");

  // The limited CC76-extrapolation between two octagons in a
  // zero-dimensional space is a octagon in a zero-dimensional
  // space, too.
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

  Octagonal_Shape limiting_octagon(space_dim, UNIVERSE);
  get_limiting_octagon(cs, limiting_octagon);
  CC76_extrapolation_assign(y, tp);
  intersection_assign(limiting_octagon);
}

template <typename T>
void
Octagonal_Shape<T>::BHMZ05_widening_assign(const Octagonal_Shape& y,
                                           unsigned* tp) {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("BHMZ05_widening_assign(y)", y);

  // Assume `y' is contained in or equal to `*this'.
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
    Octagonal_Shape x_tmp(*this);
    x_tmp.BHMZ05_widening_assign(y, 0);
    // If the widening was not precise, use one of the available tokens.
    if (!contains(x_tmp))
      --(*tp);
    return;
  }

  // Here no token is available.
  PPL_ASSERT(marked_strongly_closed() && y.marked_strongly_closed());
  // Minimize `y'.
  y.strong_reduction_assign();

  // Extrapolate unstable bounds.
  typename OR_Matrix<N>::const_element_iterator j = y.matrix.element_begin();
  for (typename OR_Matrix<N>::element_iterator i = matrix.element_begin(),
       matrix_element_end = matrix.element_end();
       i != matrix_element_end;
       ++i, ++j) {
    N& elem = *i;
      // Note: in the following line the use of `!=' (as opposed to
      // the use of `<' that would seem -but is not- equivalent) is
      // intentional.
    if (*j != elem)
      assign_r(elem, PLUS_INFINITY, ROUND_NOT_NEEDED);
  }
  reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>
::limited_BHMZ05_extrapolation_assign(const Octagonal_Shape& y,
                                      const Constraint_System& cs,
                                      unsigned* tp) {

  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("limited_BHMZ05_extrapolation_assign(y, cs)",
                                 y);
  // `cs' must be dimension-compatible with the two octagons.
  const dimension_type cs_space_dim = cs.space_dimension();
  if (space_dim < cs_space_dim)
    throw_constraint_incompatible("limited_CH78_extrapolation_assign(y, cs)");

  // Strict inequalities not allowed.
  if (cs.has_strict_inequalities())
    throw_constraint_incompatible("limited_CH78_extrapolation_assign(y, cs)");

  // The limited BHMZ05-extrapolation between two octagons in a
  // zero-dimensional space is a octagon in a zero-dimensional
  // space, too.
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

  Octagonal_Shape limiting_octagon(space_dim, UNIVERSE);
  get_limiting_octagon(cs, limiting_octagon);
  BHMZ05_widening_assign(y, tp);
  intersection_assign(limiting_octagon);
}

template <typename T>
void
Octagonal_Shape<T>::CC76_narrowing_assign(const Octagonal_Shape& y) {
  // Dimension-compatibility check.
  if (space_dim != y.space_dim)
    throw_dimension_incompatible("CC76_narrowing_assign(y)", y);

  // Assume `*this' is contained in or equal to `y'.
  PPL_EXPECT_HEAVY(copy_contains(y, *this));

  // If both octagons are zero-dimensional, since `*this' contains `y',
  // we simply return '*this'.
  if (space_dim == 0)
    return;

  y.strong_closure_assign();
  // If `y' is empty, since `y' contains `*this', `*this' is empty too.
  if (y.marked_empty())
    return;
  strong_closure_assign();
  // If `*this' is empty, we return.
  if (marked_empty())
    return;

  // We consider a constraint of `*this', if its value is `plus_infinity',
  // we take the value of the corresponding constraint of `y'.
  bool is_oct_changed = false;
  typename OR_Matrix<N>::const_element_iterator j = y.matrix.element_begin();
  for (typename OR_Matrix<N>::element_iterator i = matrix.element_begin(),
         matrix_element_end = matrix.element_end();
       i != matrix_element_end;
       ++i, ++j) {
    if (!is_plus_infinity(*i)
        && !is_plus_infinity(*j)
        && *i != *j) {
      *i = *j;
      is_oct_changed = true;
    }
  }

  if (is_oct_changed && marked_strongly_closed())
    reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>
::deduce_v_pm_u_bounds(const dimension_type v_id,
                       const dimension_type last_id,
                       const Linear_Expression& sc_expr,
                       Coefficient_traits::const_reference sc_denom,
                       const N& ub_v) {
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(sc_denom > 0);
  PPL_ASSERT(!is_plus_infinity(ub_v));

  PPL_DIRTY_TEMP(mpq_class, mpq_sc_denom);
  assign_r(mpq_sc_denom, sc_denom, ROUND_NOT_NEEDED);

  // No need to consider indices greater than `last_id'.
  const dimension_type n_v = 2*v_id;
  typename OR_Matrix<N>::row_reference_type m_cv = matrix[n_v + 1];

  // Speculatively allocate temporaries out of the loop.
  PPL_DIRTY_TEMP(N, half);
  PPL_DIRTY_TEMP(mpq_class, minus_lb_u);
  PPL_DIRTY_TEMP(mpq_class, q);
  PPL_DIRTY_TEMP(mpq_class, minus_q);
  PPL_DIRTY_TEMP(mpq_class, ub_u);
  PPL_DIRTY_TEMP(mpq_class, lb_u);
  PPL_DIRTY_TEMP(N, up_approx);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_expr_u);

  for (Linear_Expression::const_iterator u = sc_expr.begin(),
      u_end = sc_expr.lower_bound(Variable(last_id + 1)); u != u_end; ++u) {
    const dimension_type u_id = u.variable().id();
    // Skip the case when `u_id == v_id'.
    if (u_id == v_id)
      continue;
    const Coefficient& expr_u = *u;

    const dimension_type n_u = u_id*2;
    // If `expr_u' is positive, we can improve `v - u'.
    if (expr_u > 0) {
      if (expr_u >= sc_denom) {
        // Here q >= 1: deducing `v - u <= ub_v - ub_u'.
        // We avoid to check if `ub_u' is plus infinity, because
        // it is used for the computation of `ub_v'.
        // Let half = m_cu_u / 2.
        div_2exp_assign_r(half, matrix[n_u + 1][n_u], 1, ROUND_UP);
        N& m_v_minus_u = (n_v < n_u) ? matrix[n_u][n_v] : m_cv[n_u + 1];
        sub_assign_r(m_v_minus_u, ub_v, half, ROUND_UP);
      }
      else {
        // Here 0 < q < 1.
        typename OR_Matrix<N>::row_reference_type m_u = matrix[n_u];
        const N& m_u_cu = m_u[n_u + 1];
        if (!is_plus_infinity(m_u_cu)) {
          // Let `ub_u' and `lb_u' be the known upper and lower bound
          // for `u', respectively. The upper bound for `v - u' is
          // computed as `ub_v - (q * ub_u + (1-q) * lb_u)',
          // i.e., `ub_v + (-lb_u) - q * (ub_u + (-lb_u))'.
          assign_r(minus_lb_u, m_u_cu, ROUND_NOT_NEEDED);
          div_2exp_assign_r(minus_lb_u, minus_lb_u, 1, ROUND_NOT_NEEDED);
          assign_r(q, expr_u, ROUND_NOT_NEEDED);
          div_assign_r(q, q, mpq_sc_denom, ROUND_NOT_NEEDED);
          assign_r(ub_u, matrix[n_u + 1][n_u], ROUND_NOT_NEEDED);
          div_2exp_assign_r(ub_u, ub_u, 1, ROUND_NOT_NEEDED);
          // Compute `ub_u - lb_u'.
          add_assign_r(ub_u, ub_u, minus_lb_u, ROUND_NOT_NEEDED);
          // Compute `(-lb_u) - q * (ub_u - lb_u)'.
          sub_mul_assign_r(minus_lb_u, q, ub_u, ROUND_NOT_NEEDED);
          assign_r(up_approx, minus_lb_u, ROUND_UP);
          // Deducing `v - u <= ub_v - (q * ub_u + (1-q) * lb_u)'.
          N& m_v_minus_u = (n_v < n_u) ? m_u[n_v] : m_cv[n_u + 1];
          add_assign_r(m_v_minus_u, ub_v, up_approx, ROUND_UP);
        }
      }
    }
    else {
      PPL_ASSERT(expr_u < 0);
      // If `expr_u' is negative, we can improve `v + u'.
      neg_assign(minus_expr_u, expr_u);
      if (minus_expr_u >= sc_denom) {
        // Here q <= -1: Deducing `v + u <= ub_v + lb_u'.
        // We avoid to check if `lb_u' is plus infinity, because
        // it is used for the computation of `ub_v'.
        // Let half = m_u_cu / 2.
        div_2exp_assign_r(half, matrix[n_u][n_u + 1], 1, ROUND_UP);
        N& m_v_plus_u = (n_v < n_u) ? matrix[n_u + 1][n_v] : m_cv[n_u];
        sub_assign_r(m_v_plus_u, ub_v, half, ROUND_UP);
      }
      else {
        // Here -1 < q < 0.
        typename OR_Matrix<N>::row_reference_type m_cu = matrix[n_u + 1];
        const N& m_cu_u = m_cu[n_u];
        if (!is_plus_infinity(m_cu_u)) {
          // Let `ub_u' and `lb_u' be the known upper and lower bound
          // for `u', respectively. The upper bound for `v + u' is
          // computed as `ub_v + ((-q) * lb_u + (1 + q) * ub_u)',
          // i.e., `ub_v + ub_u + (-q) * (lb_u - ub_u)'.
          assign_r(ub_u, m_cu[n_u], ROUND_NOT_NEEDED);
          div_2exp_assign_r(ub_u, ub_u, 1, ROUND_NOT_NEEDED);
          assign_r(minus_q, minus_expr_u, ROUND_NOT_NEEDED);
          div_assign_r(minus_q, minus_q, mpq_sc_denom, ROUND_NOT_NEEDED);
          assign_r(lb_u, matrix[n_u][n_u + 1], ROUND_NOT_NEEDED);
          div_2exp_assign_r(lb_u, lb_u, 1, ROUND_NOT_NEEDED);
          neg_assign_r(lb_u, lb_u, ROUND_NOT_NEEDED);
          // Compute `lb_u - ub_u'.
          sub_assign_r(lb_u, lb_u, ub_u, ROUND_NOT_NEEDED);
          // Compute `ub_u + (-q) * (lb_u - ub_u)'.
          add_mul_assign_r(ub_u, minus_q, lb_u, ROUND_NOT_NEEDED);
          assign_r(up_approx, ub_u, ROUND_UP);
          // Deducing `v + u <= ub_v + ((-q) * lb_u + (1 + q) * ub_u)'.
          N& m_v_plus_u = (n_v < n_u) ? m_cu[n_v] : m_cv[n_u];
          add_assign_r(m_v_plus_u, ub_v, up_approx, ROUND_UP);
        }
      }
    }
  }
}

template <typename T>
void
Octagonal_Shape<T>
::deduce_minus_v_pm_u_bounds(const dimension_type v_id,
                             const dimension_type last_id,
                             const Linear_Expression& sc_expr,
                             Coefficient_traits::const_reference sc_denom,
                             const N& minus_lb_v) {
  // Private method: the caller has to ensure the following.
  PPL_ASSERT(sc_denom > 0);
  PPL_ASSERT(!is_plus_infinity(minus_lb_v));

  PPL_DIRTY_TEMP(mpq_class, mpq_sc_denom);
  assign_r(mpq_sc_denom, sc_denom, ROUND_NOT_NEEDED);

  // No need to consider indices greater than `last_id'.
  const dimension_type n_v = 2*v_id;
  typename OR_Matrix<N>::row_reference_type m_v = matrix[n_v];

  // Speculatively allocate temporaries out of the loop.
  PPL_DIRTY_TEMP(N, half);
  PPL_DIRTY_TEMP(mpq_class, ub_u);
  PPL_DIRTY_TEMP(mpq_class, q);
  PPL_DIRTY_TEMP(mpq_class, minus_lb_u);
  PPL_DIRTY_TEMP(N, up_approx);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_expr_u);

  for (Linear_Expression::const_iterator u = sc_expr.begin(),
      u_end = sc_expr.lower_bound(Variable(last_id + 1)); u != u_end; ++u) {
    const dimension_type u_id = u.variable().id();
    // Skip the case when `u_id == v_id'.
    if (u_id == v_id)
      continue;
    const Coefficient& expr_u = *u;

    const dimension_type n_u = u_id*2;
    // If `expr_u' is positive, we can improve `-v + u'.
    if (expr_u > 0) {
      if (expr_u >= sc_denom) {
        // Here q >= 1: deducing `-v + u <= lb_u - lb_v',
        // i.e., `u - v <= (-lb_v) - (-lb_u)'.
        // We avoid to check if `lb_u' is plus infinity, because
        // it is used for the computation of `lb_v'.
        // Let half = m_u_cu / 2.
        div_2exp_assign_r(half, matrix[n_u][n_u + 1], 1, ROUND_UP);
        N& m_u_minus_v = (n_v < n_u) ? matrix[n_u + 1][n_v + 1] : m_v[n_u];
        sub_assign_r(m_u_minus_v, minus_lb_v, half, ROUND_UP);
      }
      else {
        // Here 0 < q < 1.
        typename OR_Matrix<N>::row_reference_type m_cu = matrix[n_u + 1];
        const N& m_cu_u = m_cu[n_u];
        if (!is_plus_infinity(m_cu_u)) {
          // Let `ub_u' and `lb_u' be the known upper and lower bound
          // for `u', respectively. The upper bound for `u - v' is
          // computed as `(q * lb_u + (1-q) * ub_u) - lb_v',
          // i.e., `ub_u - q * (ub_u + (-lb_u)) + minus_lb_v'.
          assign_r(ub_u, m_cu[n_u], ROUND_NOT_NEEDED);
          div_2exp_assign_r(ub_u, ub_u, 1, ROUND_NOT_NEEDED);
          assign_r(q, expr_u, ROUND_NOT_NEEDED);
          div_assign_r(q, q, mpq_sc_denom, ROUND_NOT_NEEDED);
          assign_r(minus_lb_u, matrix[n_u][n_u + 1], ROUND_NOT_NEEDED);
          div_2exp_assign_r(minus_lb_u, minus_lb_u, 1, ROUND_NOT_NEEDED);
          // Compute `ub_u - lb_u'.
          add_assign_r(minus_lb_u, ub_u, minus_lb_u, ROUND_NOT_NEEDED);
          // Compute `ub_u - q * (ub_u - lb_u)'.
          sub_mul_assign_r(ub_u, q, minus_lb_u, ROUND_NOT_NEEDED);
          assign_r(up_approx, ub_u, ROUND_UP);
          // Deducing `u - v <= -lb_v - (q * lb_u + (1-q) * ub_u)'.
          N& m_u_minus_v = (n_v < n_u) ? m_cu[n_v + 1] : m_v[n_u];
          add_assign_r(m_u_minus_v, minus_lb_v, up_approx, ROUND_UP);
        }
      }
    }
    else {
      PPL_ASSERT(expr_u < 0);
      // If `expr_u' is negative, we can improve `-v - u'.
      neg_assign(minus_expr_u, expr_u);
      if (minus_expr_u >= sc_denom) {
        // Here q <= -1: Deducing `-v - u <= -lb_v - ub_u'.
        // We avoid to check if `ub_u' is plus infinity, because
        // it is used for the computation of `lb_v'.
        // Let half = m_cu_u / 2.
        div_2exp_assign_r(half, matrix[n_u + 1][n_u], 1, ROUND_UP);
        N& m_minus_v_minus_u = (n_v < n_u)
          ? matrix[n_u][n_v + 1]
          : m_v[n_u + 1];
        sub_assign_r(m_minus_v_minus_u, minus_lb_v, half, ROUND_UP);
      }
      else {
        // Here -1 < q < 0.
        typename OR_Matrix<N>::row_reference_type m_u = matrix[n_u];
        const N& m_u_cu = m_u[n_u + 1];
        if (!is_plus_infinity(m_u_cu)) {
          // Let `ub_u' and `lb_u' be the known upper and lower bound
          // for `u', respectively. The upper bound for `-v - u' is
          // computed as `-lb_v - ((-q)*ub_u + (1 + q)*lb_u)',
          // i.e., `minus_lb_v - lb_u + q*(ub_u - lb_u)'.
          assign_r(ub_u, matrix[n_u + 1][n_u], ROUND_NOT_NEEDED);
          div_2exp_assign_r(ub_u, ub_u, 1, ROUND_NOT_NEEDED);
          assign_r(q, expr_u, ROUND_NOT_NEEDED);
          div_assign_r(q, q, mpq_sc_denom, ROUND_NOT_NEEDED);
          assign_r(minus_lb_u, m_u[n_u + 1], ROUND_NOT_NEEDED);
          div_2exp_assign_r(minus_lb_u, minus_lb_u, 1, ROUND_NOT_NEEDED);
          // Compute `ub_u - lb_u'.
          add_assign_r(ub_u, ub_u, minus_lb_u, ROUND_NOT_NEEDED);
          // Compute `-lb_u + q*(ub_u - lb_u)'.
          add_mul_assign_r(minus_lb_u, q, ub_u, ROUND_NOT_NEEDED);
          assign_r(up_approx, minus_lb_u, ROUND_UP);
          // Deducing `-v - u <= -lb_v - ((-q) * ub_u + (1 + q) * lb_u)'.
          N& m_minus_v_minus_u = (n_v < n_u) ? m_u[n_v + 1] : m_v[n_u + 1];
          add_assign_r(m_minus_v_minus_u, minus_lb_v, up_approx, ROUND_UP);
        }
      }
    }
  }
}

template <typename T>
void
Octagonal_Shape<T>
::forget_all_octagonal_constraints(const dimension_type v_id) {
  PPL_ASSERT(v_id < space_dim);
  const dimension_type n_v = 2*v_id;
  typename OR_Matrix<N>::row_iterator m_iter = matrix.row_begin() + n_v;
  typename OR_Matrix<N>::row_reference_type r_v = *m_iter;
  typename OR_Matrix<N>::row_reference_type r_cv = *(++m_iter);
  for (dimension_type h = m_iter.row_size(); h-- > 0; ) {
    assign_r(r_v[h], PLUS_INFINITY, ROUND_NOT_NEEDED);
    assign_r(r_cv[h], PLUS_INFINITY, ROUND_NOT_NEEDED);
  }
  ++m_iter;
  for (typename OR_Matrix<N>::row_iterator m_end = matrix.row_end();
       m_iter != m_end; ++m_iter) {
    typename OR_Matrix<N>::row_reference_type r = *m_iter;
    assign_r(r[n_v], PLUS_INFINITY, ROUND_NOT_NEEDED);
    assign_r(r[n_v + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
  }
}

template <typename T>
void
Octagonal_Shape<T>
::forget_binary_octagonal_constraints(const dimension_type v_id) {
  PPL_ASSERT(v_id < space_dim);
  const dimension_type n_v = 2*v_id;
  typename OR_Matrix<N>::row_iterator m_iter = matrix.row_begin() + n_v;
  typename OR_Matrix<N>::row_reference_type r_v = *m_iter;
  typename OR_Matrix<N>::row_reference_type r_cv = *(++m_iter);
  for (dimension_type k = n_v; k-- > 0; ) {
    assign_r(r_v[k], PLUS_INFINITY, ROUND_NOT_NEEDED);
    assign_r(r_cv[k], PLUS_INFINITY, ROUND_NOT_NEEDED);
  }
  ++m_iter;
  for (typename OR_Matrix<N>::row_iterator m_end = matrix.row_end();
       m_iter != m_end; ++m_iter) {
    typename OR_Matrix<N>::row_reference_type r = *m_iter;
    assign_r(r[n_v], PLUS_INFINITY, ROUND_NOT_NEEDED);
    assign_r(r[n_v + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
  }
}

template <typename T>
void
Octagonal_Shape<T>::unconstrain(const Variable var) {
  // Dimension-compatibility check.
  const dimension_type var_id = var.id();
  if (space_dimension() < var_id + 1)
    throw_dimension_incompatible("unconstrain(var)", var_id + 1);

  // Enforce strong closure for precision.
  strong_closure_assign();

  // If the shape is empty, this is a no-op.
  if (marked_empty())
    return;

  forget_all_octagonal_constraints(var_id);
  // Strong closure is preserved.
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::unconstrain(const Variables_Set& vars) {
  // The cylindrification with respect to no dimensions is a no-op.
  // This case captures the only legal cylindrification in a 0-dim space.
  if (vars.empty())
    return;

  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dimension() < min_space_dim)
    throw_dimension_incompatible("unconstrain(vs)", min_space_dim);

  // Enforce strong closure for precision.
  strong_closure_assign();

  // If the shape is empty, this is a no-op.
  if (marked_empty())
    return;

  for (Variables_Set::const_iterator vsi = vars.begin(),
         vsi_end = vars.end(); vsi != vsi_end; ++vsi)
    forget_all_octagonal_constraints(*vsi);
  // Strong closure is preserved.
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::refine(const Variable var,
                           const Relation_Symbol relsym,
                           const Linear_Expression& expr,
                           Coefficient_traits::const_reference denominator) {
  PPL_ASSERT(denominator != 0);
  PPL_ASSERT(space_dim >= expr.space_dimension());
  const dimension_type var_id = var.id();
  PPL_ASSERT(var_id <= space_dim);
  PPL_ASSERT(expr.coefficient(var) == 0);
  PPL_ASSERT(relsym != LESS_THAN && relsym != GREATER_THAN);

  const Coefficient& b = expr.inhomogeneous_term();
  // Number of non-zero coefficients in `expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;

  // Variable index of the last non-zero coefficient in `expr', if any.
  dimension_type w_id = expr.last_nonzero();

  if (w_id != 0) {
    ++t;
    if (!expr.all_zeroes(1, w_id))
      ++t;
    --w_id;
  }

  // Now we know the form of `expr':
  // - If t == 0, then expr == b, with `b' a constant;
  // - If t == 1, then expr == a*j + b, where `j != v';
  // - If t == 2, then `expr' is of the general form.
  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  typedef typename OR_Matrix<N>::const_row_iterator Row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type Row_reference;

  const row_iterator m_begin = matrix.row_begin();
  const dimension_type n_var = 2*var_id;
  PPL_DIRTY_TEMP_COEFFICIENT(minus_denom);
  neg_assign(minus_denom, denominator);

  // Since we are only able to record octagonal differences, we can
  // precisely deal with the case of a single variable only if its
  // coefficient (taking into account the denominator) is 1.
  // If this is not the case, we fall back to the general case
  // so as to over-approximate the constraint.
  if (t == 1 && expr.coefficient(Variable(w_id)) != denominator
      && expr.coefficient(Variable(w_id)) != minus_denom)
    t = 2;

  if (t == 0) {
    // Case 1: expr == b.
    PPL_DIRTY_TEMP_COEFFICIENT(two_b);
    two_b = 2*b;
    switch (relsym) {
    case EQUAL:
      // Add the constraint `var == b/denominator'.
      add_octagonal_constraint(n_var + 1, n_var, two_b, denominator);
      add_octagonal_constraint(n_var, n_var + 1, two_b, minus_denom);
      break;
    case LESS_OR_EQUAL:
      // Add the constraint `var <= b/denominator'.
      add_octagonal_constraint(n_var + 1, n_var, two_b, denominator);
      break;
    case GREATER_OR_EQUAL:
      // Add the constraint `var >= b/denominator',
      // i.e., `-var <= -b/denominator',
      add_octagonal_constraint(n_var, n_var + 1, two_b, minus_denom);
      break;
    default:
      // We already dealt with the other cases.
      PPL_UNREACHABLE;
      break;
    }
  }
  else if (t == 1) {
    // Value of the one and only non-zero coefficient in `expr'.
    const Coefficient& w_coeff = expr.coefficient(Variable(w_id));
    const dimension_type n_w = 2*w_id;
    switch (relsym) {
    case EQUAL:
      if (w_coeff == denominator)
        // Add the new constraint `var - w = b/denominator'.
        if (var_id < w_id) {
          add_octagonal_constraint(n_w, n_var, b, denominator);
          add_octagonal_constraint(n_w + 1, n_var + 1, b, minus_denom);
        }
        else {
          add_octagonal_constraint(n_var + 1, n_w + 1, b, denominator);
          add_octagonal_constraint(n_var, n_w, b, minus_denom);
        }
      else
        // Add the new constraint `var + w = b/denominator'.
        if (var_id < w_id) {
          add_octagonal_constraint(n_w + 1, n_var, b, denominator);
          add_octagonal_constraint(n_w, n_var + 1, b, minus_denom);
        }
        else {
          add_octagonal_constraint(n_var + 1, n_w, b, denominator);
          add_octagonal_constraint(n_var, n_w + 1, b, minus_denom);
        }
      break;
    case LESS_OR_EQUAL:
      {
        PPL_DIRTY_TEMP(N, d);
        div_round_up(d, b, denominator);
        // Note that: `w_id != v', so that `expr' is of the form
        // w_coeff * w + b, with `w_id != v'.
        if (w_coeff == denominator) {
          // Add the new constraints `v - w <= b/denominator'.
          if (var_id < w_id)
            add_octagonal_constraint(n_w, n_var, d);
          else
            add_octagonal_constraint(n_var + 1, n_w + 1, d);
        }
        else if (w_coeff == minus_denom) {
          // Add the new constraints `v + w <= b/denominator'.
          if (var_id < w_id)
            add_octagonal_constraint(n_w + 1, n_var, d);
          else
            add_octagonal_constraint(n_var + 1, n_w, d);
        }
        break;
      }

    case GREATER_OR_EQUAL:
      {
        PPL_DIRTY_TEMP(N, d);
        div_round_up(d, b, minus_denom);
        // Note that: `w_id != v', so that `expr' is of the form
        // w_coeff * w + b, with `w_id != v'.
        if (w_coeff == denominator) {
          // Add the new constraint `v - w >= b/denominator',
          // i.e.,  `-v + w <= -b/denominator'.
          if (var_id < w_id)
            add_octagonal_constraint(n_w + 1, n_var + 1, d);
          else
            add_octagonal_constraint(n_var, n_w, d);
        }
        else if (w_coeff == minus_denom) {
          // Add the new constraints `v + w >= b/denominator',
          // i.e.,  `-v - w <= -b/denominator'.
          if (var_id < w_id)
            add_octagonal_constraint(n_w, n_var + 1, d);
          else
            add_octagonal_constraint(n_var, n_w + 1, d);
        }
        break;
      }

    default:
      // We already dealt with the other cases.
      PPL_UNREACHABLE;
      break;
    }
  }
  else {
    // Here t == 2, so that
    // expr == a_1*x_1 + a_2*x_2 + ... + a_n*x_n + b, where n >= 2.
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
    // Index of variable that is unbounded in `this'.
    PPL_UNINITIALIZED(dimension_type, pinf_index);
    // Number of unbounded variables found.
    dimension_type pinf_count = 0;

    switch (relsym) {
    case EQUAL:
      {
        PPL_DIRTY_TEMP(N, neg_sum);
        // Index of variable that is unbounded in `this'.
        PPL_UNINITIALIZED(dimension_type, neg_pinf_index);
        // Number of unbounded variables found.
        dimension_type neg_pinf_count = 0;

        // Approximate the inhomogeneous term.
        assign_r(sum, sc_b, ROUND_UP);
        assign_r(neg_sum, minus_sc_b, ROUND_UP);

        // Approximate the homogeneous part of `sc_expr'.
        PPL_DIRTY_TEMP(N, coeff_i);
        PPL_DIRTY_TEMP(N, half);
        PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
        PPL_DIRTY_TEMP(N, minus_coeff_i);
        // Note: indices above `w' can be disregarded, as they all have
        // a zero coefficient in `sc_expr'.
        for (Row_iterator m_iter = m_begin,
               m_iter_end = m_begin + (2 * w_id + 2);
             m_iter != m_iter_end; ) {
          const dimension_type n_i = m_iter.index();
          const dimension_type id = n_i/2;
          Row_reference m_i = *m_iter;
          ++m_iter;
          Row_reference m_ci = *m_iter;
          ++m_iter;
          const Coefficient& sc_i = sc_expr.coefficient(Variable(id));
          const int sign_i = sgn(sc_i);
          if (sign_i > 0) {
            assign_r(coeff_i, sc_i, ROUND_UP);
            // Approximating `sc_expr'.
            if (pinf_count <= 1) {
              const N& double_approx_i = m_ci[n_i];
              if (!is_plus_infinity(double_approx_i)) {
                // Let half = double_approx_i / 2.
                div_2exp_assign_r(half, double_approx_i, 1, ROUND_UP);
                add_mul_assign_r(sum, coeff_i, half, ROUND_UP);
              }
              else {
                ++pinf_count;
                pinf_index = id;
              }
            }
            // Approximating `-sc_expr'.
            if (neg_pinf_count <= 1) {
              const N& double_approx_minus_i = m_i[n_i + 1];
              if (!is_plus_infinity(double_approx_minus_i)) {
                // Let half = double_approx_minus_i / 2.
                div_2exp_assign_r(half, double_approx_minus_i, 1, ROUND_UP);
                add_mul_assign_r(neg_sum, coeff_i, half, ROUND_UP);
              }
              else {
                ++neg_pinf_count;
                neg_pinf_index = id;
              }
            }
          }
          else if (sign_i < 0) {
            neg_assign_r(minus_sc_i, sc_i, ROUND_NOT_NEEDED);
            assign_r(minus_coeff_i, minus_sc_i, ROUND_UP);
            // Approximating `sc_expr'.
            if (pinf_count <= 1) {
              const N& double_approx_minus_i = m_i[n_i + 1];
              if (!is_plus_infinity(double_approx_minus_i)) {
                // Let half = double_approx_minus_i / 2.
                div_2exp_assign_r(half, double_approx_minus_i, 1, ROUND_UP);
                add_mul_assign_r(sum, minus_coeff_i, half, ROUND_UP);
              }
              else {
                ++pinf_count;
                pinf_index = id;
              }
            }
            // Approximating `-sc_expr'.
            if (neg_pinf_count <= 1) {
              const N& double_approx_i = m_ci[n_i];
              if (!is_plus_infinity(double_approx_i)) {
                // Let half = double_approx_i / 2.
                div_2exp_assign_r(half, double_approx_i, 1, ROUND_UP);
                add_mul_assign_r(neg_sum, minus_coeff_i, half, ROUND_UP);
              }
              else {
                ++neg_pinf_count;
                neg_pinf_index = id;
              }
            }
          }
        }
        // Return immediately if no approximation could be computed.
        if (pinf_count > 1 && neg_pinf_count > 1) {
          PPL_ASSERT(OK());
          return;
        }

        // In the following, strong closure will be definitely lost.
        reset_strongly_closed();

        // Exploit the upper approximation, if possible.
        if (pinf_count <= 1) {
          // Compute quotient (if needed).
          if (sc_denom != 1) {
            // Before computing quotients, the denominator should be
            // approximated towards zero. Since `sc_denom' is known to be
            // positive, this amounts to rounding downwards, which is
            // achieved as usual by rounding upwards `minus_sc_denom'
            // and negating again the result.
            PPL_DIRTY_TEMP(N, down_sc_denom);
            assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
            neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
            div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
          }
          // Add the upper bound constraint, if meaningful.
          if (pinf_count == 0) {
            // Add the constraint `v <= sum'.
            PPL_DIRTY_TEMP(N, double_sum);
            mul_2exp_assign_r(double_sum, sum, 1, ROUND_UP);
            matrix[n_var + 1][n_var] = double_sum;
            // Deduce constraints of the form `v +/- u', where `u != v'.
            deduce_v_pm_u_bounds(var_id, w_id, sc_expr, sc_denom, sum);
          }
          else
            // Here `pinf_count == 1'.
            if (pinf_index != var_id) {
              const Coefficient& ppi
                = sc_expr.coefficient(Variable(pinf_index));
              if (ppi == sc_denom)
                // Add the constraint `v - pinf_index <= sum'.
                if (var_id < pinf_index)
                  matrix[2*pinf_index][n_var] = sum;
                else
                  matrix[n_var + 1][2*pinf_index + 1] = sum;
              else
                if (ppi == minus_sc_denom) {
                  // Add the constraint `v + pinf_index <= sum'.
                  if (var_id < pinf_index)
                    matrix[2*pinf_index + 1][n_var] = sum;
                  else
                    matrix[n_var + 1][2*pinf_index] = sum;
                }
            }
        }

        // Exploit the lower approximation, if possible.
        if (neg_pinf_count <= 1) {
          // Compute quotient (if needed).
          if (sc_denom != 1) {
            // Before computing quotients, the denominator should be
            // approximated towards zero. Since `sc_denom' is known to be
            // positive, this amounts to rounding downwards, which is
            // achieved as usual by rounding upwards `minus_sc_denom'
            // and negating again the result.
            PPL_DIRTY_TEMP(N, down_sc_denom);
            assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
            neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
            div_assign_r(neg_sum, neg_sum, down_sc_denom, ROUND_UP);
          }
          // Add the lower bound constraint, if meaningful.
          if (neg_pinf_count == 0) {
            // Add the constraint `v >= -neg_sum', i.e., `-v <= neg_sum'.
            PPL_DIRTY_TEMP(N, double_neg_sum);
            mul_2exp_assign_r(double_neg_sum, neg_sum, 1, ROUND_UP);
            matrix[n_var][n_var + 1] = double_neg_sum;
            // Deduce constraints of the form `-v +/- u', where `u != v'.
            deduce_minus_v_pm_u_bounds(var_id, w_id, sc_expr, sc_denom,
                                       neg_sum);
          }
          else
            // Here `neg_pinf_count == 1'.
            if (neg_pinf_index != var_id) {
              const Coefficient& npi
                = sc_expr.coefficient(Variable(neg_pinf_index));
              if (npi == sc_denom)
                // Add the constraint `v - neg_pinf_index >= -neg_sum',
                // i.e., `neg_pinf_index - v <= neg_sum'.
                if (neg_pinf_index < var_id)
                  matrix[n_var][2*neg_pinf_index] = neg_sum;
                else
                  matrix[2*neg_pinf_index + 1][n_var + 1] = neg_sum;
              else
                if (npi == minus_sc_denom) {
                  // Add the constraint `v + neg_pinf_index >= -neg_sum',
                  // i.e., `-neg_pinf_index - v <= neg_sum'.
                  if (neg_pinf_index < var_id)
                    matrix[n_var][2*neg_pinf_index + 1] = neg_sum;
                  else
                    matrix[2*neg_pinf_index][n_var + 1] = neg_sum;
                }
            }
        }
        break;
      }

    case LESS_OR_EQUAL:
      {
        // Compute an upper approximation for `expr' into `sum',
        // taking into account the sign of `denominator'.

        // Approximate the inhomogeneous term.
        assign_r(sum, sc_b, ROUND_UP);

        // Approximate the homogeneous part of `sc_expr'.
        PPL_DIRTY_TEMP(N, coeff_i);
        PPL_DIRTY_TEMP(N, approx_i);
        PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
        // Note: indices above `w_id' can be disregarded, as they all have
        // a zero coefficient in `expr'.
        for (row_iterator m_iter = m_begin,
               m_iter_end = m_begin + (2 * w_id + 2);
             m_iter != m_iter_end; ) {
          const dimension_type n_i = m_iter.index();
          const dimension_type id = n_i/2;
          row_reference m_i = *m_iter;
          ++m_iter;
          row_reference m_ci = *m_iter;
          ++m_iter;
          const Coefficient& sc_i = sc_expr.coefficient(Variable(id));
          const int sign_i = sgn(sc_i);
          if (sign_i == 0)
            continue;
          // Choose carefully: we are approximating `sc_expr'.
          const N& double_approx_i = (sign_i > 0) ? m_ci[n_i] : m_i[n_i + 1];
          if (is_plus_infinity(double_approx_i)) {
            if (++pinf_count > 1)
              break;
            pinf_index = id;
            continue;
          }
          if (sign_i > 0)
            assign_r(coeff_i, sc_i, ROUND_UP);
          else {
            neg_assign(minus_sc_i, sc_i);
            assign_r(coeff_i, minus_sc_i, ROUND_UP);
          }
          div_2exp_assign_r(approx_i, double_approx_i, 1, ROUND_UP);
          add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
        }
        // Divide by the (sign corrected) denominator (if needed).
        if (sc_denom != 1) {
          // Before computing the quotient, the denominator should be
          // approximated towards zero. Since `sc_denom' is known to be
          // positive, this amounts to rounding downwards, which is achieved
          // by rounding upwards `minus_sc-denom' and negating again the result.
          PPL_DIRTY_TEMP(N, down_sc_denom);
          assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
          neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
          div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
        }

        if (pinf_count == 0) {
          // Add the constraint `v <= sum'.
          PPL_DIRTY_TEMP(N, double_sum);
          mul_2exp_assign_r(double_sum, sum, 1, ROUND_UP);
          add_octagonal_constraint(n_var + 1, n_var, double_sum);
          // Deduce constraints of the form `v +/- u', where `u != v'.
          deduce_v_pm_u_bounds(var_id, w_id, sc_expr, sc_denom, sum);
        }
        else if (pinf_count == 1) {
          dimension_type pinf_ind = 2*pinf_index;
          if (expr.coefficient(Variable(pinf_index)) == denominator ) {
            // Add the constraint `v - pinf_index <= sum'.
            if (var_id < pinf_index)
              add_octagonal_constraint(pinf_ind, n_var, sum);
            else
              add_octagonal_constraint(n_var + 1, pinf_ind + 1, sum);
          }
          else {
            if (expr.coefficient(Variable(pinf_index)) == minus_denom) {
              // Add the constraint `v + pinf_index <= sum'.
              if (var_id < pinf_index)
                add_octagonal_constraint(pinf_ind + 1, n_var, sum);
              else
                add_octagonal_constraint(n_var + 1, pinf_ind, sum);
            }
          }
        }
        break;
      }

    case GREATER_OR_EQUAL:
      {
        // Compute an upper approximation for `-sc_expr' into `sum'.
        // Note: approximating `-sc_expr' from above and then negating the
        // result is the same as approximating `sc_expr' from below.

        // Approximate the inhomogeneous term.
        assign_r(sum, minus_sc_b, ROUND_UP);

        // Approximate the homogeneous part of `-sc_expr'.
        PPL_DIRTY_TEMP(N, coeff_i);
        PPL_DIRTY_TEMP(N, approx_i);
        PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
        for (row_iterator m_iter = m_begin,
               m_iter_end = m_begin + (2 * w_id + 2);
             m_iter != m_iter_end; ) {
          const dimension_type n_i = m_iter.index();
          const dimension_type id = n_i/2;
          row_reference m_i = *m_iter;
          ++m_iter;
          row_reference m_ci = *m_iter;
          ++m_iter;
          const Coefficient& sc_i = sc_expr.coefficient(Variable(id));
          const int sign_i = sgn(sc_i);
          if (sign_i == 0)
            continue;
          // Choose carefully: we are approximating `-sc_expr'.
          const N& double_approx_i = (sign_i > 0) ? m_i[n_i + 1] : m_ci[n_i];
          if (is_plus_infinity(double_approx_i)) {
            if (++pinf_count > 1)
              break;
            pinf_index = id;
            continue;
          }
          if (sign_i > 0)
            assign_r(coeff_i, sc_i, ROUND_UP);
          else {
            neg_assign(minus_sc_i, sc_i);
            assign_r(coeff_i, minus_sc_i, ROUND_UP);
          }
          div_2exp_assign_r(approx_i, double_approx_i, 1, ROUND_UP);
          add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
        }

        // Divide by the (sign corrected) denominator (if needed).
        if (sc_denom != 1) {
          // Before computing the quotient, the denominator should be
          // approximated towards zero. Since `sc_denom' is known to be
          // positive, this amounts to rounding downwards, which is
          // achieved by rounding upwards `minus_sc_denom' and
          // negating again the result.
          PPL_DIRTY_TEMP(N, down_sc_denom);
          assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
          neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
          div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
        }

        if (pinf_count == 0) {
          // Add the constraint `v >= -neg_sum', i.e., `-v <= neg_sum'.
          PPL_DIRTY_TEMP(N, double_sum);
          mul_2exp_assign_r(double_sum, sum, 1, ROUND_UP);
          add_octagonal_constraint(n_var, n_var + 1, double_sum);
          // Deduce constraints of the form `-v +/- u', where `u != v'.
          deduce_minus_v_pm_u_bounds(var_id, pinf_index, sc_expr, sc_denom,
                                     sum);
        }
        else if (pinf_count == 1) {
          dimension_type pinf_ind = 2*pinf_index;
          if (expr.coefficient(Variable(pinf_index)) == denominator) {
            // Add the constraint `v - pinf_index >= -sum',
            // i.e., `pinf_index - v <= sum'.
            if (pinf_index < var_id)
              add_octagonal_constraint(n_var, pinf_ind, sum);
            else
              add_octagonal_constraint(pinf_ind + 1, n_var, sum);
          }
          else {
            if (expr.coefficient(Variable(pinf_index)) == minus_denom) {
              // Add the constraint `v + pinf_index >= -sum',
              // i.e., `-pinf_index - v <= sum'.
              if (pinf_index < var_id)
                add_octagonal_constraint(n_var, pinf_ind + 1, sum);
              else
                add_octagonal_constraint(pinf_ind, n_var + 1, sum);
            }
          }
        }
        break;
      }

    default:
      // We already dealt with the other cases.
      PPL_UNREACHABLE;
      break;
    }
  }
}

template <typename T>
void
Octagonal_Shape<T>::affine_image(const Variable var,
                                 const Linear_Expression& expr,
                                 Coefficient_traits::const_reference
                                 denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("affine_image(v, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("affine_image(v, e, d)", "e", expr);

  // `var' should be one of the dimensions of the octagon.
  const dimension_type var_id = var.id();
  if (space_dim < var_id + 1)
    throw_dimension_incompatible("affine_image(v, e, d)", var_id + 1);

  strong_closure_assign();
  // The image of an empty octagon is empty too.
  if (marked_empty())
    return;

  // Number of non-zero coefficients in `expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Variable-index of the last non-zero coefficient in `expr', if any.
  dimension_type w_id = expr.last_nonzero();

  if (w_id != 0) {
    ++t;
    if (!expr.all_zeroes(1, w_id))
      ++t;
    --w_id;
  }

  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  typedef typename OR_Matrix<N>::const_row_iterator Row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type Row_reference;
  using std::swap;

  const dimension_type n_var = 2*var_id;
  const Coefficient& b = expr.inhomogeneous_term();
  PPL_DIRTY_TEMP_COEFFICIENT(minus_denom);
  neg_assign_r(minus_denom, denominator, ROUND_NOT_NEEDED);

  // `w' is the variable with index `w_id'.
  // Now we know the form of `expr':
  // - If t == 0, then expr == b, with `b' a constant;
  // - If t == 1, then expr == a*w + b, where `w' can be `v' or another
  //   variable; in this second case we have to check whether `a' is
  //   equal to `denominator' or `-denominator', since otherwise we have
  //   to fall back on the general form;
  // - If t == 2, the `expr' is of the general form.

  if (t == 0) {
    // Case 1: expr == b.
    // Remove all constraints on `var'.
    forget_all_octagonal_constraints(var_id);
    PPL_DIRTY_TEMP_COEFFICIENT(two_b);
    two_b = 2*b;
    // Add the constraint `var == b/denominator'.
    add_octagonal_constraint(n_var + 1, n_var, two_b, denominator);
    add_octagonal_constraint(n_var, n_var + 1, two_b, minus_denom);
    PPL_ASSERT(OK());
    return;
  }

  if (t == 1) {
    // The one and only non-zero homogeneous coefficient in `expr'.
    const Coefficient& w_coeff = expr.coefficient(Variable(w_id));
    if (w_coeff == denominator || w_coeff == minus_denom) {
      // Case 2: expr = w_coeff*w + b, with w_coeff = +/- denominator.
      if (w_id == var_id) {
        // Here `expr' is of the form: +/- denominator * v + b.
        const bool sign_symmetry = (w_coeff != denominator);
        if (!sign_symmetry && b == 0)
          // The transformation is the identity function.
          return;
        // Translate all the constraints on `var' adding or
        // subtracting the value `b/denominator'.
        PPL_DIRTY_TEMP(N, d);
        div_round_up(d, b, denominator);
        PPL_DIRTY_TEMP(N, minus_d);
        div_round_up(minus_d, b, minus_denom);
        if (sign_symmetry)
          swap(d, minus_d);
        const row_iterator m_begin = matrix.row_begin();
        const row_iterator m_end = matrix.row_end();
        row_iterator m_iter = m_begin + n_var;
        row_reference m_v = *m_iter;
        ++m_iter;
        row_reference m_cv = *m_iter;
        ++m_iter;
        // NOTE: delay update of unary constraints on `var'.
        for (dimension_type j = n_var; j-- > 0; ) {
          N& m_v_j = m_v[j];
          add_assign_r(m_v_j, m_v_j, minus_d, ROUND_UP);
          N& m_cv_j = m_cv[j];
          add_assign_r(m_cv_j, m_cv_j, d, ROUND_UP);
          if (sign_symmetry)
            swap(m_v_j, m_cv_j);
        }
        for ( ; m_iter != m_end; ++m_iter) {
          row_reference m_i = *m_iter;
          N& m_i_v = m_i[n_var];
          add_assign_r(m_i_v, m_i_v, d, ROUND_UP);
          N& m_i_cv = m_i[n_var + 1];
          add_assign_r(m_i_cv, m_i_cv, minus_d, ROUND_UP);
          if (sign_symmetry)
            swap(m_i_v, m_i_cv);
        }
        // Now update unary constraints on var.
        mul_2exp_assign_r(d, d, 1, ROUND_UP);
        N& m_cv_v = m_cv[n_var];
        add_assign_r(m_cv_v, m_cv_v, d, ROUND_UP);
        mul_2exp_assign_r(minus_d, minus_d, 1, ROUND_UP);
        N& m_v_cv = m_v[n_var + 1];
        add_assign_r(m_v_cv, m_v_cv, minus_d, ROUND_UP);
        if (sign_symmetry)
          swap(m_cv_v, m_v_cv);
        // Note: strong closure is preserved.
      }
      else {
        // Here `w != var', so that `expr' is of the form
        // +/-denominator * w + b.
        // Remove all constraints on `var'.
        forget_all_octagonal_constraints(var_id);
        const dimension_type n_w = 2*w_id;
        // Add the new constraint `var - w = b/denominator'.
        if (w_coeff == denominator) {
          if (var_id < w_id) {
            add_octagonal_constraint(n_w, n_var, b, denominator);
            add_octagonal_constraint(n_w + 1, n_var + 1, b, minus_denom);
          }
          else {
            add_octagonal_constraint(n_var + 1, n_w + 1, b, denominator);
            add_octagonal_constraint(n_var, n_w, b, minus_denom);
          }
        }
        else {
          // Add the new constraint `var + w = b/denominator'.
          if (var_id < w_id) {
            add_octagonal_constraint(n_w + 1, n_var, b, denominator);
            add_octagonal_constraint(n_w, n_var + 1, b, minus_denom);
          }
          else {
            add_octagonal_constraint(n_var + 1, n_w, b, denominator);
            add_octagonal_constraint(n_var, n_w + 1, b, minus_denom);
          }
        }
        incremental_strong_closure_assign(var);
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
  neg_assign_r(minus_b, b, ROUND_NOT_NEEDED);

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
  // Indices of the variables that are unbounded in `this->matrix'.
  PPL_UNINITIALIZED(dimension_type, pos_pinf_index);
  PPL_UNINITIALIZED(dimension_type, neg_pinf_index);
  // Number of unbounded variables found.
  dimension_type pos_pinf_count = 0;
  dimension_type neg_pinf_count = 0;

  // Approximate the inhomogeneous term.
  assign_r(pos_sum, sc_b, ROUND_UP);
  assign_r(neg_sum, minus_sc_b, ROUND_UP);

  // Approximate the homogeneous part of `sc_expr'.
  PPL_DIRTY_TEMP(N, coeff_i);
  PPL_DIRTY_TEMP(N, minus_coeff_i);
  PPL_DIRTY_TEMP(N, half);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
  // Note: indices above `w' can be disregarded, as they all have
  // a zero coefficient in `sc_expr'.
  const row_iterator m_begin = matrix.row_begin();
  for (Row_iterator m_iter = m_begin, m_iter_end = m_begin + (2 * w_id + 2);
       m_iter != m_iter_end; ) {
    const dimension_type n_i = m_iter.index();
    const dimension_type id = n_i/2;
    Row_reference m_i = *m_iter;
    ++m_iter;
    Row_reference m_ci = *m_iter;
    ++m_iter;
    const Coefficient& sc_i = sc_expr.coefficient(Variable(id));
    const int sign_i = sgn(sc_i);
    if (sign_i > 0) {
      assign_r(coeff_i, sc_i, ROUND_UP);
      // Approximating `sc_expr'.
      if (pos_pinf_count <= 1) {
        const N& double_up_approx_i = m_ci[n_i];
        if (!is_plus_infinity(double_up_approx_i)) {
          // Let half = double_up_approx_i / 2.
          div_2exp_assign_r(half, double_up_approx_i, 1, ROUND_UP);
          add_mul_assign_r(pos_sum, coeff_i, half, ROUND_UP);
        }
        else {
          ++pos_pinf_count;
          pos_pinf_index = id;
        }
      }
      // Approximating `-sc_expr'.
      if (neg_pinf_count <= 1) {
        const N& double_up_approx_minus_i = m_i[n_i + 1];
        if (!is_plus_infinity(double_up_approx_minus_i)) {
          // Let half = double_up_approx_minus_i / 2.
          div_2exp_assign_r(half, double_up_approx_minus_i, 1, ROUND_UP);
          add_mul_assign_r(neg_sum, coeff_i, half, ROUND_UP);
        }
        else {
          ++neg_pinf_count;
          neg_pinf_index = id;
        }
      }
    }
    else if (sign_i < 0) {
      neg_assign_r(minus_sc_i, sc_i, ROUND_NOT_NEEDED);
      assign_r(minus_coeff_i, minus_sc_i, ROUND_UP);
      // Approximating `sc_expr'.
      if (pos_pinf_count <= 1) {
        const N& double_up_approx_minus_i = m_i[n_i + 1];
        if (!is_plus_infinity(double_up_approx_minus_i)) {
          // Let half = double_up_approx_minus_i / 2.
          div_2exp_assign_r(half, double_up_approx_minus_i, 1, ROUND_UP);
          add_mul_assign_r(pos_sum, minus_coeff_i, half, ROUND_UP);
        }
        else {
          ++pos_pinf_count;
          pos_pinf_index = id;
        }
      }
      // Approximating `-sc_expr'.
      if (neg_pinf_count <= 1) {
        const N& double_up_approx_i = m_ci[n_i];
        if (!is_plus_infinity(double_up_approx_i)) {
          // Let half = double_up_approx_i / 2.
          div_2exp_assign_r(half, double_up_approx_i, 1, ROUND_UP);
          add_mul_assign_r(neg_sum, minus_coeff_i, half, ROUND_UP);
        }
        else {
          ++neg_pinf_count;
          neg_pinf_index = id;
        }
      }
    }
  }

  // Remove all constraints on `var'.
  forget_all_octagonal_constraints(var_id);
  // Return immediately if no approximation could be computed.
  if (pos_pinf_count > 1 && neg_pinf_count > 1) {
    PPL_ASSERT(OK());
    return;
  }

  // In the following, strong closure will be definitely lost.
  reset_strongly_closed();

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
      PPL_DIRTY_TEMP(N, double_pos_sum);
      mul_2exp_assign_r(double_pos_sum, pos_sum, 1, ROUND_UP);
      matrix[n_var + 1][n_var] = double_pos_sum;
      // Deduce constraints of the form `v +/- u', where `u != v'.
      deduce_v_pm_u_bounds(var_id, w_id, sc_expr, sc_denom, pos_sum);
    }
    else
      // Here `pos_pinf_count == 1'.
      if (pos_pinf_index != var_id) {
        const Coefficient& ppi = sc_expr.coefficient(Variable(pos_pinf_index));
        if (ppi == sc_denom)
          // Add the constraint `v - pos_pinf_index <= pos_sum'.
          if (var_id < pos_pinf_index)
            matrix[2*pos_pinf_index][n_var] = pos_sum;
          else
            matrix[n_var + 1][2*pos_pinf_index + 1] = pos_sum;
        else
          if (ppi == minus_sc_denom) {
            // Add the constraint `v + pos_pinf_index <= pos_sum'.
            if (var_id < pos_pinf_index)
              matrix[2*pos_pinf_index + 1][n_var] = pos_sum;
            else
              matrix[n_var + 1][2*pos_pinf_index] = pos_sum;
          }
      }
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
      PPL_DIRTY_TEMP(N, double_neg_sum);
      mul_2exp_assign_r(double_neg_sum, neg_sum, 1, ROUND_UP);
      matrix[n_var][n_var + 1] = double_neg_sum;
      // Deduce constraints of the form `-v +/- u', where `u != v'.
      deduce_minus_v_pm_u_bounds(var_id, w_id, sc_expr, sc_denom, neg_sum);
    }
    else
      // Here `neg_pinf_count == 1'.
      if (neg_pinf_index != var_id) {
        const Coefficient& npi = sc_expr.coefficient(Variable(neg_pinf_index));
        if (npi == sc_denom)
          // Add the constraint `v - neg_pinf_index >= -neg_sum',
          // i.e., `neg_pinf_index - v <= neg_sum'.
          if (neg_pinf_index < var_id)
            matrix[n_var][2*neg_pinf_index] = neg_sum;
          else
            matrix[2*neg_pinf_index + 1][n_var + 1] = neg_sum;
        else
          if (npi == minus_sc_denom) {
            // Add the constraint `v + neg_pinf_index >= -neg_sum',
            // i.e., `-neg_pinf_index - v <= neg_sum'.
            if (neg_pinf_index < var_id)
              matrix[n_var][2*neg_pinf_index + 1] = neg_sum;
            else
              matrix[2*neg_pinf_index][n_var + 1] = neg_sum;
          }
      }
  }

  incremental_strong_closure_assign(var);
  PPL_ASSERT(OK());
}

template <typename T>
template <typename Interval_Info>
void
Octagonal_Shape<T>::affine_form_image(const Variable var,
                    const Linear_Form< Interval<T, Interval_Info> >& lf) {
  // Check that T is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
    "Octagonal_Shape<T>::affine_form_image(Variable, Linear_Form):"
    " T is not a floating point type.");

  // Dimension-compatibility checks.
  // The dimension of `lf' should not be greater than the dimension
  // of `*this'.
  const dimension_type lf_space_dim = lf.space_dimension();
  if (space_dim < lf_space_dim)
    throw_dimension_incompatible("affine_form_image(v, l)", "l", lf);

  // `var' should be one of the dimensions of the octagon.
  const dimension_type var_id = var.id();
  if (space_dim < var_id + 1)
    throw_dimension_incompatible("affine_form_image(v, l)", var.id() + 1);

  strong_closure_assign();
  // The image of an empty octagon is empty too.
  if (marked_empty())
    return;

  // Number of non-zero coefficients in `lf': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Variable-index of the last non-zero coefficient in `lf', if any.
  dimension_type w_id = 0;

  // Get information about the number of non-zero coefficients in `lf'.
  for (dimension_type i = lf_space_dim; i-- > 0; )
    if (lf.coefficient(Variable(i)) != 0) {
      if (t++ == 1)
        break;
      else
        w_id = i;
    }

  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  typedef typename OR_Matrix<N>::const_row_iterator Row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type Row_reference;
  typedef Interval<T, Interval_Info> FP_Interval_Type;
  using std::swap;

  const dimension_type n_var = 2*var_id;
  const FP_Interval_Type& b = lf.inhomogeneous_term();

  // `w' is the variable with index `w_id'.
  // Now we know the form of `lf':
  // - If t == 0, then lf == [lb, ub];
  // - If t == 1, then lf == a*w + [lb, ub], where `w' can be `v' or another
  //   variable;
  // - If t == 2, the `lf' is of the general form.

  PPL_DIRTY_TEMP(N, b_ub);
  assign_r(b_ub, b.upper(), ROUND_NOT_NEEDED);
  PPL_DIRTY_TEMP(N, b_mlb);
  neg_assign_r(b_mlb, b.lower(), ROUND_NOT_NEEDED);

  if (t == 0) {
    // Case 1: lf = [lb, ub].
    forget_all_octagonal_constraints(var_id);
    mul_2exp_assign_r(b_mlb, b_mlb, 1, ROUND_UP);
    mul_2exp_assign_r(b_ub, b_ub, 1, ROUND_UP);
    // Add the constraint `var >= lb && var <= ub'.
    add_octagonal_constraint(n_var + 1, n_var, b_ub);
    add_octagonal_constraint(n_var, n_var + 1, b_mlb);
    PPL_ASSERT(OK());
    return;
  }

  // True if `b' is in [0, 0].
  bool is_b_zero = (b_mlb == 0 && b_ub == 0);

  if (t == 1) {
    // The one and only non-zero homogeneous coefficient in `lf'.
    const FP_Interval_Type& w_coeff = lf.coefficient(Variable(w_id));
    // True if `w_coeff' is in [1, 1].
    bool is_w_coeff_one = (w_coeff == 1);
    // True if `w_coeff' is in [-1, -1].
    bool is_w_coeff_minus_one = (w_coeff == -1);
    if (is_w_coeff_one || is_w_coeff_minus_one) {
      // Case 2: lf = w_coeff*w + b, with w_coeff = [+/-1, +/-1].
      if (w_id == var_id) {
        // Here lf = w_coeff*v + b, with w_coeff = [+/-1, +/-1].
        if (is_w_coeff_one && is_b_zero)
          // The transformation is the identity function.
          return;
        // Translate all the constraints on `var' by adding the value
        // `b_ub' or subtracting the value `b_lb'.
        if (is_w_coeff_minus_one)
          swap(b_ub, b_mlb);
        const row_iterator m_begin = matrix.row_begin();
        const row_iterator m_end = matrix.row_end();
        row_iterator m_iter = m_begin + n_var;
        row_reference m_v = *m_iter;
        ++m_iter;
        row_reference m_cv = *m_iter;
        ++m_iter;
        // NOTE: delay update of unary constraints on `var'.
        for (dimension_type j = n_var; j-- > 0; ) {
          N& m_v_j = m_v[j];
          add_assign_r(m_v_j, m_v_j, b_mlb, ROUND_UP);
          N& m_cv_j = m_cv[j];
          add_assign_r(m_cv_j, m_cv_j, b_ub, ROUND_UP);
          if (is_w_coeff_minus_one)
            swap(m_v_j, m_cv_j);
        }
        for ( ; m_iter != m_end; ++m_iter) {
          row_reference m_i = *m_iter;
          N& m_i_v = m_i[n_var];
          add_assign_r(m_i_v, m_i_v, b_ub, ROUND_UP);
          N& m_i_cv = m_i[n_var + 1];
          add_assign_r(m_i_cv, m_i_cv, b_mlb, ROUND_UP);
          if (is_w_coeff_minus_one)
            swap(m_i_v, m_i_cv);
        }
        // Now update unary constraints on var.
        mul_2exp_assign_r(b_ub, b_ub, 1, ROUND_UP);
        N& m_cv_v = m_cv[n_var];
        add_assign_r(m_cv_v, m_cv_v, b_ub, ROUND_UP);
        mul_2exp_assign_r(b_mlb, b_mlb, 1, ROUND_UP);
        N& m_v_cv = m_v[n_var + 1];
        add_assign_r(m_v_cv, m_v_cv, b_mlb, ROUND_UP);
        if (is_w_coeff_minus_one)
          swap(m_cv_v, m_v_cv);
        // Note: strong closure is preserved.
      }
      else {
        // Here `w != var', so that `lf' is of the form
        // [+/-1, +/-1] * w + b.
        // Remove all constraints on `var'.
        forget_all_octagonal_constraints(var_id);
        const dimension_type n_w = 2*w_id;
        if (is_w_coeff_one)
          // Add the new constraints `var - w >= b_lb'
          // `and var - w <= b_ub'.
          if (var_id < w_id) {
            add_octagonal_constraint(n_w, n_var, b_ub);
            add_octagonal_constraint(n_w + 1, n_var + 1, b_mlb);
          }
          else {
            add_octagonal_constraint(n_var + 1, n_w + 1, b_ub);
            add_octagonal_constraint(n_var, n_w, b_mlb);
          }
        else
          // Add the new constraints `var + w >= b_lb'
          // `and var + w <= b_ub'.
          if (var_id < w_id) {
            add_octagonal_constraint(n_w + 1, n_var, b_ub);
            add_octagonal_constraint(n_w, n_var + 1, b_mlb);
          }
          else {
            add_octagonal_constraint(n_var + 1, n_w, b_ub);
            add_octagonal_constraint(n_var, n_w + 1, b_mlb);
          }
        incremental_strong_closure_assign(var);
      }
      PPL_ASSERT(OK());
      return;
    }
  }

  // General case.
  // Either t == 2, so that
  // expr == i_1*x_1 + i_2*x_2 + ... + i_n*x_n + b, where n >= 2,
  // or t == 1, expr == i*w + b, but i <> [+/-1, +/-1].

  // In the following, strong closure will be definitely lost.
  reset_strongly_closed();

  Linear_Form<FP_Interval_Type> minus_lf(lf);
  minus_lf.negate();

  // Declare temporaries outside the loop.
  PPL_DIRTY_TEMP(N, upper_bound);

  row_iterator m_iter = matrix.row_begin();
  m_iter += n_var;
  row_reference var_ite = *m_iter;
  ++m_iter;
  row_reference var_cv_ite = *m_iter;
  ++m_iter;
  row_iterator m_end = matrix.row_end();

  // Update binary constraints on var FIRST.
  for (dimension_type curr_var = var_id,
         n_curr_var = n_var - 2; curr_var-- > 0; ) {
    Variable current(curr_var);
    linear_form_upper_bound(lf + current, upper_bound);
    assign_r(var_cv_ite[n_curr_var], upper_bound, ROUND_NOT_NEEDED);
    linear_form_upper_bound(lf - current, upper_bound);
    assign_r(var_cv_ite[n_curr_var + 1], upper_bound, ROUND_NOT_NEEDED);
    linear_form_upper_bound(minus_lf + current, upper_bound);
    assign_r(var_ite[n_curr_var], upper_bound, ROUND_NOT_NEEDED);
    linear_form_upper_bound(minus_lf - current, upper_bound);
    assign_r(var_ite[n_curr_var + 1], upper_bound, ROUND_NOT_NEEDED);
    n_curr_var -= 2;
  }
  for (dimension_type curr_var = var_id + 1; m_iter != m_end; ++m_iter) {
    row_reference m_v_ite = *m_iter;
    ++m_iter;
    row_reference m_cv_ite = *m_iter;
    Variable current(curr_var);
    linear_form_upper_bound(lf + current, upper_bound);
    assign_r(m_cv_ite[n_var], upper_bound, ROUND_NOT_NEEDED);
    linear_form_upper_bound(lf - current, upper_bound);
    assign_r(m_v_ite[n_var], upper_bound, ROUND_NOT_NEEDED);
    linear_form_upper_bound(minus_lf + current, upper_bound);
    assign_r(m_cv_ite[n_var + 1], upper_bound, ROUND_NOT_NEEDED);
    linear_form_upper_bound(minus_lf - current, upper_bound);
    assign_r(m_v_ite[n_var + 1], upper_bound, ROUND_NOT_NEEDED);
    ++curr_var;
  }

  // Finally, update unary constraints on var.
  PPL_DIRTY_TEMP(N, lf_ub);
  linear_form_upper_bound(lf, lf_ub);
  PPL_DIRTY_TEMP(N, minus_lf_ub);
  linear_form_upper_bound(minus_lf, minus_lf_ub);
  mul_2exp_assign_r(lf_ub, lf_ub, 1, ROUND_UP);
  assign_r(matrix[n_var + 1][n_var], lf_ub, ROUND_NOT_NEEDED);
  mul_2exp_assign_r(minus_lf_ub, minus_lf_ub, 1, ROUND_UP);
  assign_r(matrix[n_var][n_var + 1], minus_lf_ub, ROUND_NOT_NEEDED);

  PPL_ASSERT(OK());
}

template <typename T>
template <typename Interval_Info>
void
Octagonal_Shape<T>::
linear_form_upper_bound(const Linear_Form< Interval<T, Interval_Info> >& lf,
                        N& result) const {

  // Check that T is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
                     "Octagonal_Shape<T>::linear_form_upper_bound:"
                     " T not a floating point type.");

  const dimension_type lf_space_dimension = lf.space_dimension();
  PPL_ASSERT(lf_space_dimension <= space_dim);

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
    const FP_Interval_Type& curr_coefficient =
                            lf.coefficient(Variable(curr_var));
    assign_r(curr_lb, curr_coefficient.lower(), ROUND_NOT_NEEDED);
    assign_r(curr_ub, curr_coefficient.upper(), ROUND_NOT_NEEDED);
    if (curr_lb != 0 || curr_ub != 0) {
      assign_r(curr_var_ub, matrix[n_var + 1][n_var], ROUND_NOT_NEEDED);
      div_2exp_assign_r(curr_var_ub, curr_var_ub, 1, ROUND_UP);
      neg_assign_r(curr_minus_var_ub, matrix[n_var][n_var + 1],
                   ROUND_NOT_NEEDED);
      div_2exp_assign_r(curr_minus_var_ub, curr_minus_var_ub, 1, ROUND_DOWN);
      // Optimize the most common case: curr = +/-[1, 1].
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

    n_var += 2;
  }
}

template <typename T>
void
Octagonal_Shape<T>::
interval_coefficient_upper_bound(const N& var_ub, const N& minus_var_ub,
                                 const N& int_ub, const N& int_lb,
                                 N& result) {

  // Check that T is a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<T>::is_exact,
                     "Octagonal_Shape<T>::interval_coefficient_upper_bound:"
                     " T not a floating point type.");

  // NOTE: we store the first comparison term directly into result.
  PPL_DIRTY_TEMP(N, second_comparison_term);
  PPL_DIRTY_TEMP(N, third_comparison_term);
  PPL_DIRTY_TEMP(N, fourth_comparison_term);

  assign_r(result, 0, ROUND_NOT_NEEDED);
  assign_r(second_comparison_term, 0, ROUND_NOT_NEEDED);
  assign_r(third_comparison_term, 0, ROUND_NOT_NEEDED);
  assign_r(fourth_comparison_term, 0, ROUND_NOT_NEEDED);

  add_mul_assign_r(result, var_ub, int_ub, ROUND_UP);
  add_mul_assign_r(second_comparison_term, minus_var_ub, int_ub, ROUND_UP);
  add_mul_assign_r(third_comparison_term, var_ub, int_lb, ROUND_UP);
  add_mul_assign_r(fourth_comparison_term, minus_var_ub, int_lb, ROUND_UP);

  assign_r(result, std::max(result, second_comparison_term), ROUND_NOT_NEEDED);
  assign_r(result, std::max(result, third_comparison_term), ROUND_NOT_NEEDED);
  assign_r(result, std::max(result, fourth_comparison_term), ROUND_NOT_NEEDED);
}

template <typename T>
void
Octagonal_Shape<T>::affine_preimage(const Variable var,
                                    const Linear_Expression& expr,
                                    Coefficient_traits::const_reference
                                    denominator) {

  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("affine_preimage(v, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("affine_preimage(v, e, d)", "e", expr);

  // `var' should be one of the dimensions of the octagon.
  dimension_type var_id = var.id();
  if (space_dim < var_id + 1)
    throw_dimension_incompatible("affine_preimage(v, e, d)", var_id + 1);

  strong_closure_assign();
  // The image of an empty octagon is empty too.
  if (marked_empty())
    return;

  const Coefficient& b = expr.inhomogeneous_term();

  // Number of non-zero coefficients in `expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;

  // Variable-index of the last non-zero coefficient in `expr', if any.
  dimension_type w_id = expr.last_nonzero();

  if (w_id != 0) {
    ++t;
    if (!expr.all_zeroes(1, w_id))
      ++t;
    --w_id;
  }

  // `w' is the variable with index `w_id'.
  // Now we know the form of `expr':
  // - If t == 0, then expr == b, with `b' a constant;
  // - If t == 1, then expr == a*w + b, where `w' can be `v' or another
  //   variable; in this second case we have to check whether `a' is
  //   equal to `denominator' or `-denominator', since otherwise we have
  //   to fall back on the general form;
  // - If t == 2, the `expr' is of the general form.

  if (t == 0) {
    // Case 1: expr = n; remove all constraints on `var'.
    forget_all_octagonal_constraints(var_id);
    PPL_ASSERT(OK());
    return;
  }

  if (t == 1) {
    // Value of the one and only non-zero coefficient in `expr'.
    const Coefficient& w_coeff = expr.coefficient(Variable(w_id));
    if (w_coeff == denominator || w_coeff == -denominator) {
      // Case 2: expr = w_coeff*w + b, with w_coeff = +/- denominator.
      if (w_id == var_id) {
        // Apply affine_image() on the inverse of this transformation.
        affine_image(var, denominator*var - b, w_coeff);
      }
      else {
        // `expr == w_coeff*w + b', where `w != var'.
        // Remove all constraints on `var'.
        forget_all_octagonal_constraints(var_id);
        PPL_ASSERT(OK());
      }
      return;
    }
  }
  // General case.
  // Either t == 2, so that
  // expr = a_1*x_1 + a_2*x_2 + ... + a_n*x_n + b, where n >= 2,
  // or t = 1, expr = a*w + b, but a <> +/- denominator.
  const Coefficient& coeff_v = expr.coefficient(var);
  if (coeff_v != 0) {
    if (coeff_v > 0) {
      // The transformation is invertible.
      Linear_Expression inverse = ((coeff_v + denominator)*var);
      inverse -= expr;
      affine_image(var, inverse, coeff_v);
    }
    else {
      // The transformation is invertible.
      PPL_DIRTY_TEMP_COEFFICIENT(minus_coeff_v);
      neg_assign(minus_coeff_v, coeff_v);
      Linear_Expression inverse = ((minus_coeff_v - denominator)*var);
      inverse += expr;
      affine_image(var, inverse, minus_coeff_v);
    }
  }
  else {
    // The transformation is not invertible: all constraints on `var' are lost.
    forget_all_octagonal_constraints(var_id);
    PPL_ASSERT(OK());
  }
}

template <typename T>
void
Octagonal_Shape<T>
::generalized_affine_image(const Variable var,
                           const Relation_Symbol relsym,
                           const Linear_Expression&  expr ,
                           Coefficient_traits::const_reference denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("generalized_affine_image(v, r, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("generalized_affine_image(v, r, e, d)", "e",
                                 expr);

  // `var' should be one of the dimensions of the octagon.
  dimension_type var_id = var.id();
  if (space_dim < var_id + 1)
    throw_dimension_incompatible("generalized_affine_image(v, r, e, d)",
                                 var_id + 1);

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

  strong_closure_assign();
  // The image of an empty octagon is empty too.
  if (marked_empty())
    return;

  // Number of non-zero coefficients in `expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Variable-index of the last non-zero coefficient in `expr', if any.
  dimension_type w_id = expr.last_nonzero();

  if (w_id != 0) {
    ++t;
    if (!expr.all_zeroes(1, w_id))
      ++t;
    --w_id;
  }

  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  typedef typename OR_Matrix<N>::const_row_iterator Row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type Row_reference;

  const row_iterator m_begin = matrix.row_begin();
  const row_iterator m_end = matrix.row_end();
  const dimension_type n_var = 2*var_id;
  const Coefficient& b = expr.inhomogeneous_term();
  PPL_DIRTY_TEMP_COEFFICIENT(minus_denom);
  neg_assign_r(minus_denom, denominator, ROUND_NOT_NEEDED);

  // `w' is the variable with index `w_id'.
  // Now we know the form of `expr':
  // - If t == 0, then expr == b, with `b' a constant;
  // - If t == 1, then expr == a*w + b, where `w' can be `v' or another
  //   variable; in this second case we have to check whether `a' is
  //   equal to `denominator' or `-denominator', since otherwise we have
  //   to fall back on the general form;
  // - If t == 2, the `expr' is of the general form.

  if (t == 0) {
    // Case 1: expr = b.
    PPL_DIRTY_TEMP_COEFFICIENT(two_b);
    two_b = 2*b;
    // Remove all constraints on `var'.
    forget_all_octagonal_constraints(var_id);
    // Strong closure is lost.
    reset_strongly_closed();
    switch (relsym) {
    case LESS_OR_EQUAL:
      // Add the constraint `var <= b/denominator'.
      add_octagonal_constraint(n_var + 1, n_var, two_b, denominator);
      break;
    case GREATER_OR_EQUAL:
      // Add the constraint `var >= n/denominator',
      // i.e., `-var <= -b/denominator'.
      add_octagonal_constraint(n_var, n_var + 1, two_b, minus_denom);
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
    // The one and only non-zero homogeneous coefficient in `expr'.
    const Coefficient& w_coeff = expr.coefficient(Variable(w_id));
    if (w_coeff == denominator || w_coeff == minus_denom) {
      // Case 2: expr == w_coeff*w + b, with w_coeff == +/- denominator.
      switch (relsym) {
      case LESS_OR_EQUAL:
        {
          PPL_DIRTY_TEMP(N, d);
          div_round_up(d, b, denominator);
          if (w_id == var_id) {
            // Here `expr' is of the form: +/- denominator * v + b.
            // Strong closure is not preserved.
            reset_strongly_closed();
            if (w_coeff == denominator) {
              // Translate all the constraints of the form `v - w <= cost'
              // into the constraint `v - w <= cost + b/denominator';
              // forget each constraint `w - v <= cost1'.
              row_iterator m_iter = m_begin + n_var;
              row_reference m_v = *m_iter;
              N& m_v_cv = m_v[n_var + 1];
              ++m_iter;
              row_reference m_cv = *m_iter;
              N& m_cv_v = m_cv[n_var];
              ++m_iter;
              // NOTE: delay update of m_v_cv and m_cv_v.
              for ( ; m_iter != m_end; ++m_iter) {
                row_reference m_i = *m_iter;
                N& m_i_v = m_i[n_var];
                add_assign_r(m_i_v, m_i_v, d, ROUND_UP);
                assign_r(m_i[n_var + 1], PLUS_INFINITY, ROUND_NOT_NEEDED);
              }
              for (dimension_type k = n_var; k-- > 0; ) {
                assign_r(m_v[k], PLUS_INFINITY, ROUND_NOT_NEEDED);
                add_assign_r(m_cv[k], m_cv[k], d, ROUND_UP);
              }
              mul_2exp_assign_r(d, d, 1, ROUND_UP);
              add_assign_r(m_cv_v, m_cv_v, d, ROUND_UP);
              assign_r(m_v_cv, PLUS_INFINITY, ROUND_NOT_NEEDED);
            }
            else {
              // Here `w_coeff == -denominator'.
              // `expr' is of the form: -a*var + b.
              N& m_v_cv = matrix[n_var][n_var + 1];
              mul_2exp_assign_r(d, d, 1, ROUND_UP);
              add_assign_r(matrix[n_var + 1][n_var], m_v_cv, d, ROUND_UP);
              assign_r(m_v_cv, PLUS_INFINITY, ROUND_NOT_NEEDED);
              forget_binary_octagonal_constraints(var_id);
            }
          }
          else {
            // Here `w != v', so that `expr' is the form
            // +/- denominator*w + b.
            // Remove all constraints on `v'.
            forget_all_octagonal_constraints(var_id);
            const dimension_type n_w = 2*w_id;
            if (w_coeff == denominator) {
              // Add the new constraint `v - w <= b/denominator'.
              if (var_id < w_id)
                add_octagonal_constraint(n_w, n_var, b, denominator);
              else
                add_octagonal_constraint(n_var + 1, n_w + 1, b, denominator);
            }
            else {
              // Add the new constraint `v + w <= b/denominator'.
              if (var_id < w_id)
                add_octagonal_constraint(n_w + 1, n_var, b, denominator);
              else
                add_octagonal_constraint(n_var + 1, n_w, b, denominator);
            }
          }
          break;
        }

      case GREATER_OR_EQUAL:
        {
          PPL_DIRTY_TEMP(N, d);
          div_round_up(d, b, minus_denom);
          if (w_id == var_id) {
            // Here `expr' is of the form: +/- denominator * v + b.
            // Strong closure is not preserved.
            reset_strongly_closed();
            if (w_coeff == denominator) {
              // Translate each constraint `w - v <= cost'
              // into the constraint `w - v <= cost - b/denominator';
              // forget each constraint `v - w <= cost1'.
              row_iterator m_iter = m_begin + n_var;
              row_reference m_v = *m_iter;
              N& m_v_cv = m_v[n_var + 1];
              ++m_iter;
              row_reference m_cv = *m_iter;
              N& m_cv_v = m_cv[n_var];
              ++m_iter;
              // NOTE: delay update of m_v_cv and m_cv_v.
              for ( ; m_iter != m_end; ++m_iter) {
                row_reference m_i = *m_iter;
                assign_r(m_i[n_var], PLUS_INFINITY, ROUND_NOT_NEEDED);
                add_assign_r(m_i[n_var + 1], m_i[n_var + 1], d, ROUND_UP);
              }
              for (dimension_type k = n_var; k-- > 0; ) {
                add_assign_r(m_v[k], m_v[k], d, ROUND_UP);
                assign_r(m_cv[k], PLUS_INFINITY, ROUND_NOT_NEEDED);
              }
              mul_2exp_assign_r(d, d, 1, ROUND_UP);
              add_assign_r(m_v_cv, m_v_cv, d, ROUND_UP);
              assign_r(m_cv_v, PLUS_INFINITY, ROUND_NOT_NEEDED);
            }
            else {
              // Here `w_coeff == -denominator'.
              // `expr' is of the form: -a*var + b.
              N& m_cv_v = matrix[n_var + 1][n_var];
              mul_2exp_assign_r(d, d, 1, ROUND_UP);
              add_assign_r(matrix[n_var][n_var + 1], m_cv_v, d, ROUND_UP);
              assign_r(m_cv_v, PLUS_INFINITY, ROUND_NOT_NEEDED);
              forget_binary_octagonal_constraints(var_id);
            }
          }
          else {
            // Here `w != v', so that `expr' is of the form
            // +/-denominator * w + b, with `w != v'.
            // Remove all constraints on `v'.
            forget_all_octagonal_constraints(var_id);
            const dimension_type n_w = 2*w_id;
            // We have got an expression of the following form:
            // var1 + n, with `var1' != `var'.
            // We remove all constraints of the form `var (+/- var1) >= const'
            // and we add the new constraint `var +/- var1 >= n/denominator'.
            if (w_coeff == denominator) {
              // Add the new constraint `var - w >= b/denominator',
              // i.e., `w - var <= -b/denominator'.
              if (var_id < w_id)
                add_octagonal_constraint(n_w + 1, n_var + 1, b, minus_denom);
              else
                add_octagonal_constraint(n_var, n_w, b, minus_denom);
            }
            else {
              // Add the new constraint `var + w >= b/denominator',
              // i.e., `-w - var <= -b/denominator'.
              if (var_id < w_id)
                add_octagonal_constraint(n_w, n_var + 1, b, minus_denom);
              else
                add_octagonal_constraint(n_var, n_w + 1, b, minus_denom);
            }
          }
          break;
        }

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
  // Index of variable that is unbounded in `this->matrix'.
  PPL_UNINITIALIZED(dimension_type, pinf_index);
  // Number of unbounded variables found.
  dimension_type pinf_count = 0;

  switch (relsym) {
  case LESS_OR_EQUAL:
    {
      // Compute an upper approximation for `sc_expr' into `sum'.

      // Approximate the inhomogeneous term.
      assign_r(sum, sc_b, ROUND_UP);
      // Approximate the homogeneous part of `sc_expr'.
      PPL_DIRTY_TEMP(N, coeff_i);
      PPL_DIRTY_TEMP(N, approx_i);
      PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
      // Note: indices above `w' can be disregarded, as they all have
      // a zero coefficient in `sc_expr'.
      for (Row_iterator m_iter = m_begin, m_iter_end = m_begin + (2 * w_id + 2);
           m_iter != m_iter_end; ) {
        const dimension_type n_i = m_iter.index();
        const dimension_type id = n_i/2;
        Row_reference m_i = *m_iter;
        ++m_iter;
        Row_reference m_ci = *m_iter;
        ++m_iter;
        const Coefficient& sc_i = sc_expr.coefficient(Variable(id));
        const int sign_i = sgn(sc_i);
        if (sign_i == 0)
          continue;
        // Choose carefully: we are approximating `sc_expr'.
        const N& double_approx_i = (sign_i > 0) ? m_ci[n_i] : m_i[n_i + 1];
        if (is_plus_infinity(double_approx_i)) {
          if (++pinf_count > 1)
            break;
          pinf_index = id;
          continue;
        }
        if (sign_i > 0)
          assign_r(coeff_i, sc_i, ROUND_UP);
        else {
          neg_assign(minus_sc_i, sc_i);
          assign_r(coeff_i, minus_sc_i, ROUND_UP);
        }
        div_2exp_assign_r(approx_i, double_approx_i, 1, ROUND_UP);
        add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
      }
      // Remove all constraints on `v'.
      forget_all_octagonal_constraints(var_id);
      reset_strongly_closed();
      // Return immediately if no approximation could be computed.
      if (pinf_count > 1) {
        PPL_ASSERT(OK());
        return;
      }

      // Divide by the (sign corrected) denominator (if needed).
      if (sc_denom != 1) {
        // Before computing the quotient, the denominator should be
        // approximated towards zero. Since `sc_denom' is known to be
        // positive, this amounts to rounding downwards, which is
        // achieved as usual by rounding upwards
        // `minus_sc_denom' and negating again the result.
        PPL_DIRTY_TEMP(N, down_sc_denom);
        assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
        neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
        div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
      }

      if (pinf_count == 0) {
        // Add the constraint `v <= pos_sum'.
        PPL_DIRTY_TEMP(N, double_sum);
        mul_2exp_assign_r(double_sum, sum, 1, ROUND_UP);
        matrix[n_var + 1][n_var] = double_sum;
        // Deduce constraints of the form `v +/- u', where `u != v'.
        deduce_v_pm_u_bounds(var_id, w_id, sc_expr, sc_denom, sum);
      }
      else if (pinf_count == 1)
        if (pinf_index != var_id) {
          const Coefficient& pi = expr.coefficient(Variable(pinf_index));
          if (pi == denominator ) {
            // Add the constraint `v - pinf_index <= sum'.
            if (var_id < pinf_index)
              matrix[2*pinf_index][n_var] = sum;
            else
              matrix[n_var + 1][2*pinf_index + 1] = sum;
          }
          else {
            if (pi == minus_denom) {
              // Add the constraint `v + pinf_index <= sum'.
              if (var_id < pinf_index)
                matrix[2*pinf_index + 1][n_var] = sum;
              else
                matrix[n_var + 1][2*pinf_index] = sum;
            }
          }
        }
      break;
    }

  case GREATER_OR_EQUAL:
    {
      // Compute an upper approximation for `-sc_expr' into `sum'.
      // Note: approximating `-sc_expr' from above and then negating the
      // result is the same as approximating `sc_expr' from below.

      // Approximate the inhomogeneous term.
      assign_r(sum, minus_sc_b, ROUND_UP);
      PPL_DIRTY_TEMP(N, coeff_i);
      PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
      PPL_DIRTY_TEMP(N, approx_i);
      // Approximate the homogeneous part of `-sc_expr'.
      for (Row_iterator m_iter = m_begin, m_iter_end = m_begin + (2 * w_id + 2);
           m_iter != m_iter_end; ) {
        const dimension_type n_i = m_iter.index();
        const dimension_type id = n_i/2;
        Row_reference m_i = *m_iter;
        ++m_iter;
        Row_reference m_ci = *m_iter;
        ++m_iter;
        const Coefficient& sc_i = sc_expr.coefficient(Variable(id));
        const int sign_i = sgn(sc_i);
        if (sign_i == 0)
          continue;
        // Choose carefully: we are approximating `-sc_expr'.
        const N& double_approx_i = (sign_i > 0) ? m_i[n_i + 1] : m_ci[n_i];
        if (is_plus_infinity(double_approx_i)) {
          if (++pinf_count > 1)
            break;
          pinf_index = id;
          continue;
        }
        if (sign_i > 0)
          assign_r(coeff_i, sc_i, ROUND_UP);
        else {
          neg_assign(minus_sc_i, sc_i);
          assign_r(coeff_i, minus_sc_i, ROUND_UP);
        }
        div_2exp_assign_r(approx_i, double_approx_i, 1, ROUND_UP);
        add_mul_assign_r(sum, coeff_i, approx_i, ROUND_UP);
      }

      // Remove all constraints on `var'.
      forget_all_octagonal_constraints(var_id);
      reset_strongly_closed();
      // Return immediately if no approximation could be computed.
      if (pinf_count > 1) {
        PPL_ASSERT(OK());
        return;
      }

      // Divide by the (sign corrected) denominator (if needed).
      if (sc_denom != 1) {
        // Before computing the quotient, the denominator should be
        // approximated towards zero. Since `sc_denom' is known to be
        // positive, this amounts to rounding downwards, which is
        // achieved as usual by rounding upwards
        // `minus_sc_denom' and negating again the result.
        PPL_DIRTY_TEMP(N, down_sc_denom);
        assign_r(down_sc_denom, minus_sc_denom, ROUND_UP);
        neg_assign_r(down_sc_denom, down_sc_denom, ROUND_UP);
        div_assign_r(sum, sum, down_sc_denom, ROUND_UP);
      }

      if (pinf_count == 0) {
        // Add the constraint `v >= -neg_sum', i.e., `-v <= neg_sum'.
        PPL_DIRTY_TEMP(N, double_sum);
        mul_2exp_assign_r(double_sum, sum, 1, ROUND_UP);
        matrix[n_var][n_var + 1] = double_sum;
        // Deduce constraints of the form `-v +/- u', where `u != v'.
        deduce_minus_v_pm_u_bounds(var_id, pinf_index, sc_expr, sc_denom, sum);
      }
      else if (pinf_count == 1)
        if (pinf_index != var_id) {
          const Coefficient& pi = expr.coefficient(Variable(pinf_index));
          if (pi == denominator) {
            // Add the constraint `v - pinf_index >= -sum',
            // i.e., `pinf_index - v <= sum'.
            if (pinf_index < var_id)
              matrix[n_var][2*pinf_index] = sum;
            else
              matrix[2*pinf_index + 1][n_var + 1] = sum;
          }
          else {
            if (pi == minus_denom) {
              // Add the constraint `v + pinf_index >= -sum',
              // i.e., `-pinf_index - v <= sum'.
              if (pinf_index < var_id)
                matrix[n_var][2*pinf_index + 1] = sum;
              else
                matrix[2*pinf_index][n_var + 1] = sum;
            }
          }
        }
      break;
    }

  default:
    // We already dealt with the other cases.
    PPL_UNREACHABLE;
    break;
  }
  incremental_strong_closure_assign(var);
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::generalized_affine_image(const Linear_Expression& lhs,
                                             const Relation_Symbol relsym,
                                             const Linear_Expression& rhs) {
  // Dimension-compatibility checks.
  // The dimension of `lhs' should not be greater than the dimension
  // of `*this'.
  dimension_type lhs_space_dim = lhs.space_dimension();
  if (space_dim < lhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2)",
                                 "e1", lhs);

  // The dimension of `rhs' should not be greater than the dimension
  // of `*this'.
  const dimension_type rhs_space_dim = rhs.space_dimension();
  if (space_dim < rhs_space_dim)
    throw_dimension_incompatible("generalized_affine_image(e1, r, e2)",
                                 "e2", rhs);

  // Strict relation symbols are not admitted for octagons.
  if (relsym == LESS_THAN || relsym == GREATER_THAN)
    throw_invalid_argument("generalized_affine_image(e1, r, e2)",
                           "r is a strict relation symbol");
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_image(e1, r, e2)",
                           "r is the disequality relation symbol");

  strong_closure_assign();
  // The image of an empty octagon is empty.
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
    // Note that this constraint is an octagonal difference if `t_rhs <= 1'
    // or `t_rhs > 1' and `rhs == a*v - a*w + b_rhs' or
    // `rhs == a*v + a*w + b_rhs'. If `rhs' is of a
    // more general form, it will be simply ignored.
    // TODO: if it is not an octagonal difference, should we compute
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
      for (dimension_type i = lhs_vars.size(); i-- > 0; ) {
        dimension_type lhs_vars_i = lhs_vars[i].id();
        forget_all_octagonal_constraints(lhs_vars_i);
      }
      // Constrain the left hand side expression so that it is related to
      // the right hand side expression as dictated by `relsym'.
      // TODO: if the following constraint is NOT an octagonal difference,
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

      for (dimension_type i = lhs_vars.size(); i-- > 0; ) {
        dimension_type lhs_vars_i = lhs_vars[i].id();
        forget_all_octagonal_constraints(lhs_vars_i);
      }

#else // Currently unnecessarily complex computation.

      // More accurate computation that is worth doing only if
      // the following TODO note is accurately dealt with.

      // To ease the computation, we add an additional dimension.
      const Variable new_var(space_dim);
      add_space_dimensions_and_embed(1);
      // Constrain the new dimension to be equal to `rhs'.
      // NOTE: calling affine_image() instead of refine_no_check()
      // ensures some approximation is tried even when the constraint
      // is not an octagonal constraint.
      affine_image(new_var, rhs);
      // Existentially quantify all variables in the lhs.
      // NOTE: enforce strong closure for precision.
      strong_closure_assign();
      PPL_ASSERT(!marked_empty());
      for (dimension_type i = lhs_vars.size(); i-- > 0; ) {
        dimension_type lhs_vars_i = lhs_vars[i].id();
        forget_all_octagonal_constraints(lhs_vars_i);
      }
      // Constrain the new dimension so that it is related to
      // the left hand side as dictated by `relsym'.
      // TODO: each one of the following constraints is definitely NOT
      // an octagonal difference (since it has 3 variables at least).
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
Octagonal_Shape<T>::bounded_affine_image(const Variable var,
                                         const Linear_Expression& lb_expr,
                                         const Linear_Expression& ub_expr,
                                         Coefficient_traits::const_reference
                                         denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("bounded_affine_image(v, lb, ub, d)", "d == 0");

  // `var' should be one of the dimensions of the octagon.
  const dimension_type var_id = var.id();
  if (space_dim < var_id + 1)
    throw_dimension_incompatible("bounded_affine_image(v, lb, ub, d)",
                                 var_id + 1);

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

  strong_closure_assign();
  // The image of an empty octagon is empty too.
  if (marked_empty())
    return;

  // Number of non-zero coefficients in `lb_expr': will be set to
  // 0, 1, or 2, the latter value meaning any value greater than 1.
  dimension_type t = 0;
  // Variable-index of the last non-zero coefficient in `lb_expr', if any.
  dimension_type w_id = lb_expr.last_nonzero();

  if (w_id != 0) {
    ++t;
    if (!lb_expr.all_zeroes(1, w_id))
      ++t;
    --w_id;
  }

  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  typedef typename OR_Matrix<N>::const_row_iterator Row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type Row_reference;

  const row_iterator m_begin = matrix.row_begin();
  const dimension_type n_var = 2*var_id;
  const Coefficient& b = lb_expr.inhomogeneous_term();
  PPL_DIRTY_TEMP_COEFFICIENT(minus_denom);
  neg_assign_r(minus_denom, denominator, ROUND_NOT_NEEDED);

  // `w' is the variable with index `w_id'.
  // Now we know the form of `lb_expr':
  // - If t == 0, then lb_expr == b, with `b' a constant;
  // - If t == 1, then lb_expr == a*w + b, where `w' can be `v' or another
  //   variable; in this second case we have to check whether `a' is
  //   equal to `denominator' or `-denominator', since otherwise we have
  //   to fall back on the general form;
  // - If t == 2, the `lb_expr' is of the general form.

  if (t == 0) {
    // Case 1: lb_expr == b.
    generalized_affine_image(var,
                             LESS_OR_EQUAL,
                             ub_expr,
                             denominator);
    PPL_DIRTY_TEMP_COEFFICIENT(two_b);
    two_b = 2*b;
    // Add the constraint `var >= b/denominator'.
    add_octagonal_constraint(n_var, n_var + 1, two_b, minus_denom);
    PPL_ASSERT(OK());
    return;
  }

  if (t == 1) {
    // The one and only non-zero homogeneous coefficient in `lb_expr'.
    const Coefficient& w_coeff = lb_expr.coefficient(Variable(w_id));
    if (w_coeff == denominator || w_coeff == minus_denom) {
      // Case 2: lb_expr = w_coeff*w + b, with w_coeff = +/- denominator.
      if (w_id == var_id) {
        // Here `var' occurs in `lb_expr'.
        // To ease the computation, we add an additional dimension.
        const Variable new_var(space_dim);
        add_space_dimensions_and_embed(1);
        // Constrain the new dimension to be equal to `lb_expr'.
        // Here `lb_expr' is of the form: +/- denominator * v + b.
        affine_image(new_var, lb_expr, denominator);
        // Enforce the strong closure for precision.
        strong_closure_assign();
        PPL_ASSERT(!marked_empty());
        // Apply the affine upper bound.
        generalized_affine_image(var,
                                 LESS_OR_EQUAL,
                                 ub_expr,
                                 denominator);
        // Now apply the affine lower bound, as recorded in `new_var'
        refine_no_check(var >= new_var);
        // Remove the temporarily added dimension.
        remove_higher_space_dimensions(space_dim-1);
        return;
      }
      else {
        // Apply the affine upper bound.
        generalized_affine_image(var,
                                 LESS_OR_EQUAL,
                                 ub_expr,
                                 denominator);
        // Here `w != var', so that `lb_expr' is of the form
        // +/-denominator * w + b.
        const dimension_type n_w = 2*w_id;
        // Add the new constraint `var - w >= b/denominator'.
        if (w_coeff == denominator)
          if (var_id < w_id)
            add_octagonal_constraint(n_w + 1, n_var + 1, b, minus_denom);
          else
            add_octagonal_constraint(n_var, n_w, b, minus_denom);
        else {
          // Add the new constraint `var + w >= b/denominator'.
          if (var_id < w_id)
            add_octagonal_constraint(n_w, n_var + 1, b, minus_denom);
          else
            add_octagonal_constraint(n_var, n_w + 1, b, minus_denom);
        }
        PPL_ASSERT(OK());
        return;
      }
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
  neg_assign_r(minus_b, b, ROUND_NOT_NEEDED);

  const Coefficient& minus_sc_b = is_sc ? minus_b : b;
  const Coefficient& sc_denom = is_sc ? denominator : minus_denom;
  const Coefficient& minus_sc_denom = is_sc ? minus_denom : denominator;
  // NOTE: here, for optimization purposes, `minus_expr' is only assigned
  // when `denominator' is negative. Do not use it unless you are sure
  // it has been correctly assigned.
  Linear_Expression minus_expr;
  if (!is_sc)
    minus_expr = -lb_expr;
  const Linear_Expression& sc_expr = is_sc ? lb_expr : minus_expr;

  PPL_DIRTY_TEMP(N, neg_sum);
  // Indices of the variables that are unbounded in `this->matrix'.
  PPL_UNINITIALIZED(dimension_type, neg_pinf_index);
  // Number of unbounded variables found.
  dimension_type neg_pinf_count = 0;

  // Approximate the inhomogeneous term.
  assign_r(neg_sum, minus_sc_b, ROUND_UP);

  // Approximate the homogeneous part of `sc_expr'.
  PPL_DIRTY_TEMP(N, coeff_i);
  PPL_DIRTY_TEMP(N, minus_coeff_i);
  PPL_DIRTY_TEMP(N, half);
  PPL_DIRTY_TEMP_COEFFICIENT(minus_sc_i);
  // Note: indices above `w' can be disregarded, as they all have
  // a zero coefficient in `sc_expr'.
  for (Row_iterator m_iter = m_begin, m_iter_end = m_begin + (2 * w_id + 2);
       m_iter != m_iter_end; ) {
    const dimension_type n_i = m_iter.index();
    const dimension_type id = n_i/2;
    Row_reference m_i = *m_iter;
    ++m_iter;
    Row_reference m_ci = *m_iter;
    ++m_iter;
    const Coefficient& sc_i = sc_expr.coefficient(Variable(id));
    const int sign_i = sgn(sc_i);
    if (sign_i > 0) {
      assign_r(coeff_i, sc_i, ROUND_UP);
      // Approximating `-sc_expr'.
      if (neg_pinf_count <= 1) {
        const N& double_up_approx_minus_i = m_i[n_i + 1];
        if (!is_plus_infinity(double_up_approx_minus_i)) {
          // Let half = double_up_approx_minus_i / 2.
          div_2exp_assign_r(half, double_up_approx_minus_i, 1, ROUND_UP);
          add_mul_assign_r(neg_sum, coeff_i, half, ROUND_UP);
        }
        else {
          ++neg_pinf_count;
          neg_pinf_index = id;
        }
      }
    }
    else if (sign_i < 0) {
      neg_assign_r(minus_sc_i, sc_i, ROUND_NOT_NEEDED);
      assign_r(minus_coeff_i, minus_sc_i, ROUND_UP);
      // Approximating `-sc_expr'.
      if (neg_pinf_count <= 1) {
        const N& double_up_approx_i = m_ci[n_i];
        if (!is_plus_infinity(double_up_approx_i)) {
          // Let half = double_up_approx_i / 2.
          div_2exp_assign_r(half, double_up_approx_i, 1, ROUND_UP);
          add_mul_assign_r(neg_sum, minus_coeff_i, half, ROUND_UP);
        }
        else {
          ++neg_pinf_count;
          neg_pinf_index = id;
        }
      }
    }
  }

  // Apply the affine upper bound.
  generalized_affine_image(var,
                           LESS_OR_EQUAL,
                           ub_expr,
                           denominator);

  // Return immediately if no approximation could be computed.
  if (neg_pinf_count > 1) {
    return;
  }

  // In the following, strong closure will be definitely lost.
  reset_strongly_closed();

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
      PPL_DIRTY_TEMP(N, double_neg_sum);
      mul_2exp_assign_r(double_neg_sum, neg_sum, 1, ROUND_UP);
      matrix[n_var][n_var + 1] = double_neg_sum;
      // Deduce constraints of the form `-v +/- u', where `u != v'.
      deduce_minus_v_pm_u_bounds(var_id, w_id, sc_expr, sc_denom, neg_sum);
    }
    else
      // Here `neg_pinf_count == 1'.
      if (neg_pinf_index != var_id) {
        const Coefficient& npi = sc_expr.coefficient(Variable(neg_pinf_index));
        if (npi == sc_denom)
          // Add the constraint `v - neg_pinf_index >= -neg_sum',
          // i.e., `neg_pinf_index - v <= neg_sum'.
          if (neg_pinf_index < var_id)
            matrix[n_var][2*neg_pinf_index] = neg_sum;
          else
            matrix[2*neg_pinf_index + 1][n_var + 1] = neg_sum;
        else
          if (npi == minus_sc_denom) {
            // Add the constraint `v + neg_pinf_index >= -neg_sum',
            // i.e., `-neg_pinf_index - v <= neg_sum'.
            if (neg_pinf_index < var_id)
              matrix[n_var][2*neg_pinf_index + 1] = neg_sum;
            else
              matrix[2*neg_pinf_index][n_var + 1] = neg_sum;
          }
      }
  }

  PPL_ASSERT(OK());
}


template <typename T>
void
Octagonal_Shape<T>
::generalized_affine_preimage(const Variable var,
                              const Relation_Symbol relsym,
                              const Linear_Expression& expr,
                              Coefficient_traits::const_reference
                              denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("generalized_affine_preimage(v, r, e, d)", "d == 0");

  // Dimension-compatibility checks.
  // The dimension of `expr' should not be greater than the dimension
  // of `*this'.
  const dimension_type expr_space_dim = expr.space_dimension();
  if (space_dim < expr_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(v, r, e, d)",
                                 "e", expr);

  // `var' should be one of the dimensions of the octagon.
  const dimension_type var_id = var.id();
  if (space_dim < var_id + 1)
    throw_dimension_incompatible("generalized_affine_preimage(v, r, e, d)",
                                 var_id + 1);

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

  // The image of an empty octagon is empty too.
  strong_closure_assign();
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

  // Here `var_coefficient == 0', so that the preimage cannot
  // be easily computed by inverting the affine relation.
  // Shrink the Octagonal_Shape by adding the constraint induced
  // by the affine relation.
  refine(var, relsym, expr, denominator);

  // If the shrunk OS is empty, its preimage is empty too; ...
  if (is_empty())
    return;
  // ...  otherwise, since the relation was not invertible,
  // we just forget all constraints on `var'.
  forget_all_octagonal_constraints(var_id);
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>
::generalized_affine_preimage(const Linear_Expression& lhs,
                              const Relation_Symbol relsym,
                              const Linear_Expression& rhs) {
  // Dimension-compatibility checks.
  // The dimension of `lhs' should not be greater than the dimension
  // of `*this'.
  dimension_type lhs_space_dim = lhs.space_dimension();
  if (space_dim < lhs_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(e1, r, e2)",
                                 "e1", lhs);

  // The dimension of `rhs' should not be greater than the dimension
  // of `*this'.
  const dimension_type rhs_space_dim = rhs.space_dimension();
  if (space_dim < rhs_space_dim)
    throw_dimension_incompatible("generalized_affine_preimage(e1, r, e2)",
                                 "e2", rhs);

  // Strict relation symbols are not admitted for octagons.
  if (relsym == LESS_THAN || relsym == GREATER_THAN)
    throw_invalid_argument("generalized_affine_preimage(e1, r, e2)",
                           "r is a strict relation symbol");
  // The relation symbol cannot be a disequality.
  if (relsym == NOT_EQUAL)
    throw_invalid_argument("generalized_affine_preimage(e1, r, e2)",
                           "r is the disequality relation symbol");

  strong_closure_assign();
  // The image of an empty octagon is empty.
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
    j_lhs--;
  }

  const Coefficient& b_lhs = lhs.inhomogeneous_term();

  // If all variables have a zero coefficient, then `lhs' is a constant:
  // in this case, preimage and image happen to be the same.
  if (t_lhs == 0) {
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
      // TODO: if the following constraint is NOT an octagonal difference,
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

      // Any image of an empty octagon is empty.
      if (is_empty())
        return;
      // Existentially quantify all variables in the lhs.
      for (dimension_type i = lhs_vars.size(); i-- > 0; ) {
        dimension_type lhs_vars_i = lhs_vars[i].id();
        forget_all_octagonal_constraints(lhs_vars_i);
      }
    }
    else {
      // Some variables in `lhs' also occur in `rhs'.

      // More accurate computation that is worth doing only if
      // the following TODO note is accurately dealt with.

      // To ease the computation, we add an additional dimension.
      const Variable new_var(space_dim);
      add_space_dimensions_and_embed(1);
      // Constrain the new dimension to be equal to `rhs'.
      // NOTE: calling affine_image() instead of refine_no_check()
      // ensures some approximation is tried even when the constraint
      // is not an octagonal difference.
      affine_image(new_var, lhs);
      // Existentially quantify all variables in the lhs.
      // NOTE: enforce strong closure for precision.
      strong_closure_assign();
      PPL_ASSERT(!marked_empty());
      for (dimension_type i = lhs_vars.size(); i-- > 0; ) {
        dimension_type lhs_vars_i = lhs_vars[i].id();
        forget_all_octagonal_constraints(lhs_vars_i);
      }
      // Constrain the new dimension so that it is related to
      // the left hand side as dictated by `relsym'.
      // Note: if `rhs == v + b_rhs' or `rhs == -v + b_rhs' or `rhs == b_rhs',
      // one of the following constraints will be added, because they
      // are octagonal differences.
      // Else the following constraints are NOT octagonal differences,
      // so the method refine_no_check() will ignore them.
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
      remove_higher_space_dimensions(space_dim-1);
    }
  }
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::bounded_affine_preimage(const Variable var,
                                            const Linear_Expression& lb_expr,
                                            const Linear_Expression& ub_expr,
                                            Coefficient_traits::const_reference
                                            denominator) {
  // The denominator cannot be zero.
  if (denominator == 0)
    throw_invalid_argument("bounded_affine_preimage(v, lb, ub, d)", "d == 0");

  // `var' should be one of the dimensions of the octagon.
  const dimension_type var_id = var.id();
  if (space_dim < var_id + 1)
    throw_dimension_incompatible("bounded_affine_preimage(v, lb, ub, d)",
                                 var_id + 1);

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

  strong_closure_assign();
  // The image of an empty octagon is empty too.
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

  const Coefficient& expr_v = lb_expr.coefficient(var);
  // Here `var' occurs in `lb_expr' and `ub_expr'.
  // To ease the computation, we add an additional dimension.
  const Variable new_var(space_dim);
  add_space_dimensions_and_embed(1);
  const Linear_Expression lb_inverse
    = lb_expr - (expr_v + denominator)*var;
  PPL_DIRTY_TEMP_COEFFICIENT(inverse_denom);
  neg_assign(inverse_denom, expr_v);
  affine_image(new_var, lb_inverse, inverse_denom);
  strong_closure_assign();
  PPL_ASSERT(!marked_empty());
  generalized_affine_preimage(var, LESS_OR_EQUAL,
                              ub_expr, denominator);
  if (sgn(denominator) == sgn(inverse_denom))
    refine_no_check(var >= new_var) ;
  else
    refine_no_check(var <= new_var);
  // Remove the temporarily added dimension.
  remove_higher_space_dimensions(space_dim-1);
}

template <typename T>
Constraint_System
Octagonal_Shape<T>::constraints() const {
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

  typedef typename OR_Matrix<N>::const_row_iterator row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type row_reference;

  row_iterator m_begin = matrix.row_begin();
  row_iterator m_end = matrix.row_end();

  PPL_DIRTY_TEMP_COEFFICIENT(a);
  PPL_DIRTY_TEMP_COEFFICIENT(b);

  // Go through all the unary constraints in `matrix'.
  for (row_iterator i_iter = m_begin; i_iter != m_end; ) {
    const dimension_type i = i_iter.index();
    const Variable x(i/2);
    const N& c_i_ii = (*i_iter)[i + 1];
    ++i_iter;
    const N& c_ii_i = (*i_iter)[i];
    ++i_iter;
    // Go through unary constraints.
    if (is_additive_inverse(c_i_ii, c_ii_i)) {
      // We have a unary equality constraint.
      numer_denom(c_ii_i, b, a);
      a *= 2;
      cs.insert(a*x == b);
    }
    else {
      // We have 0, 1 or 2 inequality constraints.
      if (!is_plus_infinity(c_i_ii)) {
        numer_denom(c_i_ii, b, a);
        a *= 2;
        cs.insert(-a*x <= b);
      }
      if (!is_plus_infinity(c_ii_i)) {
        numer_denom(c_ii_i, b, a);
        a *= 2;
        cs.insert(a*x <= b);
      }
    }
  }
  //  Go through all the binary constraints in `matrix'.
  for (row_iterator i_iter = m_begin; i_iter != m_end; ) {
    const dimension_type i = i_iter.index();
    row_reference r_i = *i_iter;
    ++i_iter;
    row_reference r_ii = *i_iter;
    ++i_iter;
    const Variable y(i/2);
    for (dimension_type j = 0; j < i; j += 2) {
      const N& c_i_j = r_i[j];
      const N& c_ii_jj = r_ii[j + 1];
      const Variable x(j/2);
      if (is_additive_inverse(c_ii_jj, c_i_j)) {
        // We have an equality constraint of the form a*x - a*y = b.
        numer_denom(c_i_j, b, a);
        cs.insert(a*x - a*y == b);
      }
      else {
        // We have 0, 1 or 2 inequality constraints.
        if (!is_plus_infinity(c_i_j)) {
          numer_denom(c_i_j, b, a);
          cs.insert(a*x - a*y <= b);
        }
        if (!is_plus_infinity(c_ii_jj)) {
          numer_denom(c_ii_jj, b, a);
          cs.insert(a*y - a*x <= b);
        }
      }

      const N& c_ii_j = r_ii[j];
      const N& c_i_jj = r_i[j + 1];
      if (is_additive_inverse(c_i_jj, c_ii_j)) {
        // We have an equality constraint of the form a*x + a*y = b.
        numer_denom(c_ii_j, b, a);
        cs.insert(a*x + a*y == b);
      }
      else {
        // We have 0, 1 or 2 inequality constraints.
        if (!is_plus_infinity(c_i_jj)) {
          numer_denom(c_i_jj, b, a);
          cs.insert(-a*x - a*y <= b);
        }
        if (!is_plus_infinity(c_ii_j)) {
          numer_denom(c_ii_j, b, a);
          cs.insert(a*x + a*y <= b);
        }
      }
    }
  }
  return cs;
}

template <typename T>
void
Octagonal_Shape<T>::expand_space_dimension(Variable var, dimension_type m) {
  // `var' should be one of the dimensions of the vector space.
  const dimension_type var_id = var.id();
  if (var_id + 1 > space_dim)
    throw_dimension_incompatible("expand_space_dimension(v, m)", var_id + 1);

  // The space dimension of the resulting octagon should not
  // overflow the maximum allowed space dimension.
  if (m > max_space_dimension() - space_dim)
    throw_invalid_argument("expand_dimension(v, m)",
                           "adding m new space dimensions exceeds "
                           "the maximum allowed space dimension");

  // Nothing to do, if no dimensions must be added.
  if (m == 0)
    return;

  // Keep track of the dimension before adding the new ones.
  const dimension_type old_num_rows = matrix.num_rows();

  // Add the required new dimensions.
  add_space_dimensions_and_embed(m);

  // For each constraints involving variable `var', we add a
  // similar constraint with the new variable substituted for
  // variable `var'.
  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  typedef typename OR_Matrix<N>::const_row_iterator Row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type Row_reference;

  const row_iterator m_begin = matrix.row_begin();
  const row_iterator m_end = matrix.row_end();
  const dimension_type n_var = 2*var_id;
  Row_iterator v_iter = m_begin + n_var;
  Row_reference m_v = *v_iter;
  Row_reference m_cv = *(v_iter + 1);

  for (row_iterator i_iter = m_begin + old_num_rows; i_iter != m_end;
       i_iter += 2) {
    row_reference m_i = *i_iter;
    row_reference m_ci = *(i_iter + 1);
    const dimension_type i = i_iter.index();
    const dimension_type ci = i + 1;
    m_i[ci] = m_v[n_var + 1];
    m_ci[i] = m_cv[n_var];
    for (dimension_type j = 0; j < n_var; ++j) {
      m_i[j] = m_v[j];
      m_ci[j] = m_cv[j];
    }
    for (dimension_type j = n_var + 2; j < old_num_rows; ++j) {
      row_iterator j_iter = m_begin + j;
      row_reference m_cj = (j % 2 != 0) ? *(j_iter-1) : *(j_iter + 1);
      m_i[j] = m_cj[n_var + 1];
      m_ci[j] = m_cj[n_var];
    }
  }
  // In general, adding a constraint does not preserve the strong closure
  // of the octagon.
  if (marked_strongly_closed())
    reset_strongly_closed();
  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>::fold_space_dimensions(const Variables_Set& vars,
                                          Variable dest) {
  // `dest' should be one of the dimensions of the octagon.
  if (dest.space_dimension() > space_dim)
    throw_dimension_incompatible("fold_space_dimensions(vs, v)", "v", dest);

  // The folding of no dimensions is a no-op.
  if (vars.empty())
    return;

  // All variables in `vars' should be dimensions of the octagon.
  if (vars.space_dimension() > space_dim)
    throw_dimension_incompatible("fold_space_dimensions(vs, v)",
                                 vars.space_dimension());

  // Moreover, `dest.id()' should not occur in `vars'.
  if (vars.find(dest.id()) != vars.end())
    throw_invalid_argument("fold_space_dimensions(vs, v)",
                           "v should not occur in vs");

  // Recompute the elements of the row and the column corresponding
  // to variable `dest' by taking the join of their value with the
  // value of the corresponding elements in the row and column of the
  // variable `vars'.
  typedef typename OR_Matrix<N>::row_iterator row_iterator;
  typedef typename OR_Matrix<N>::row_reference_type row_reference;

  const row_iterator m_begin = matrix.row_begin();

  strong_closure_assign();
  const dimension_type n_rows = matrix.num_rows();
  const dimension_type n_dest = 2*dest.id();
  row_iterator v_iter = m_begin + n_dest;
  row_reference m_v = *v_iter;
  row_reference m_cv = *(v_iter + 1);
  for (Variables_Set::const_iterator i = vars.begin(),
         vs_end = vars.end(); i != vs_end; ++i) {
    const dimension_type tbf_id = *i;
    const dimension_type tbf_var = 2*tbf_id;
    row_iterator tbf_iter = m_begin + tbf_var;
    row_reference m_tbf = *tbf_iter;
    row_reference m_ctbf = *(tbf_iter + 1);
    max_assign(m_v[n_dest + 1], m_tbf[tbf_var + 1]);
    max_assign(m_cv[n_dest], m_ctbf[tbf_var]);

    const dimension_type min_id = std::min(n_dest, tbf_var);
    const dimension_type max_id = std::max(n_dest, tbf_var);

    using namespace Implementation::Octagonal_Shapes;
    for (dimension_type j = 0; j < min_id; ++j) {
      const dimension_type cj = coherent_index(j);
      max_assign(m_v[j], m_tbf[j]);
      max_assign(m_cv[j], m_ctbf[j]);
      max_assign(m_cv[cj], m_ctbf[cj]);
      max_assign(m_v[cj], m_tbf[cj]);
    }
    for (dimension_type j = min_id + 2; j < max_id; ++j) {
      const dimension_type cj = coherent_index(j);
      row_iterator j_iter = m_begin + j;
      row_reference m_j = *j_iter;
      row_reference m_cj = (j % 2 != 0) ? *(j_iter-1) : *(j_iter + 1);
      if (n_dest == min_id) {
        max_assign(m_cj[n_dest + 1], m_tbf[j]);
        max_assign(m_cj[n_dest], m_ctbf[j]);
        max_assign(m_j[n_dest], m_ctbf[cj]);
        max_assign(m_j[n_dest + 1], m_tbf[cj]);
      }
      else {
        max_assign(m_v[j], m_cj[tbf_var + 1]);
        max_assign(m_cv[j], m_cj[tbf_var]);
        max_assign(m_cv[cj], m_j[tbf_var]);
        max_assign(m_v[cj], m_j[tbf_var + 1]);
      }
    }
    for (dimension_type j = max_id + 2; j < n_rows; ++j) {
      row_iterator j_iter = m_begin + j;
      row_reference m_j = *j_iter;
      row_reference m_cj = (j % 2 != 0) ? *(j_iter-1) : *(j_iter + 1);
      max_assign(m_cj[n_dest + 1], m_cj[tbf_var + 1]);
      max_assign(m_cj[n_dest], m_cj[tbf_var]);
      max_assign(m_j[n_dest], m_j[tbf_var]);
      max_assign(m_j[n_dest + 1], m_j[tbf_var + 1]);
    }
  }
  remove_space_dimensions(vars);
}

template <typename T>
bool
Octagonal_Shape<T>::upper_bound_assign_if_exact(const Octagonal_Shape& y) {
  // FIXME, CHECKME: what about inexact computations?

  // Declare a const reference to *this (to avoid accidental modifications).
  const Octagonal_Shape& x = *this;
  const dimension_type x_space_dim = x.space_dimension();

  if (x_space_dim != y.space_dimension())
    throw_dimension_incompatible("upper_bound_assign_if_exact(y)", y);

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
  PPL_ASSERT(x.marked_strongly_closed());
  PPL_ASSERT(y.marked_strongly_closed());
  // Pre-compute the upper bound of `x' and `y'.
  Octagonal_Shape<T> ub(x);
  ub.upper_bound_assign(y);

  // Compute redundancy information for x and y.
  // TODO: provide a nicer data structure for redundancy.
  std::vector<Bit_Row> x_non_red;
  x.non_redundant_matrix_entries(x_non_red);
  std::vector<Bit_Row> y_non_red;
  y.non_redundant_matrix_entries(y_non_red);

  PPL_DIRTY_TEMP(N, lhs);
  PPL_DIRTY_TEMP(N, lhs_copy);
  PPL_DIRTY_TEMP(N, rhs);
  PPL_DIRTY_TEMP(N, temp_zero);
  assign_r(temp_zero, 0, ROUND_NOT_NEEDED);

  typedef typename OR_Matrix<N>::const_row_iterator row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type row_reference;
  const dimension_type n_rows = x.matrix.num_rows();
  const row_iterator x_m_begin = x.matrix.row_begin();
  const row_iterator y_m_begin = y.matrix.row_begin();
  const row_iterator ub_m_begin = ub.matrix.row_begin();

  for (dimension_type i = n_rows; i-- > 0; ) {
    const Bit_Row& x_non_red_i = x_non_red[i];
    using namespace Implementation::Octagonal_Shapes;
    const dimension_type ci = coherent_index(i);
    const dimension_type row_size_i = OR_Matrix<N>::row_size(i);
    row_reference x_i = *(x_m_begin + i);
    row_reference y_i = *(y_m_begin + i);
    row_reference ub_i = *(ub_m_begin + i);
    const N& ub_i_ci = ub_i[ci];
    for (dimension_type j = row_size_i; j-- > 0; ) {
      // Check redundancy of x_i_j.
      if (!x_non_red_i[j])
        continue;
      const N& x_i_j = x_i[j];
      // Check 1st condition in BHZ09 theorem.
      if (x_i_j >= y_i[j])
        continue;
      const dimension_type cj = coherent_index(j);
      const dimension_type row_size_cj = OR_Matrix<N>::row_size(cj);
      row_reference ub_cj = *(ub_m_begin + cj);
      const N& ub_cj_j = ub_cj[j];
      for (dimension_type k = 0; k < n_rows; ++k) {
        const Bit_Row& y_non_red_k = y_non_red[k];
        const dimension_type ck = coherent_index(k);
        const dimension_type row_size_k = OR_Matrix<N>::row_size(k);
        row_reference x_k = *(x_m_begin + k);
        row_reference y_k = *(y_m_begin + k);
        row_reference ub_k = *(ub_m_begin + k);
        const N& ub_k_ck = ub_k[ck];
        // Be careful: for each index h, the diagonal element m[h][h]
        // is (by convention) +infty in our implementation; however,
        // BHZ09 theorem assumes that it is equal to 0.
        const N& ub_k_j
          = (k == j)
          ? temp_zero
          : ((j < row_size_k) ? ub_k[j] : ub_cj[ck]);
        const N& ub_i_ck
          = (i == ck)
          ? temp_zero
          : ((ck < row_size_i) ? ub_i[ck] : ub_k[ci]);

        for (dimension_type ell = row_size_k; ell-- > 0; ) {
          // Check redundancy of y_k_ell.
          if (!y_non_red_k[ell])
            continue;
          const N& y_k_ell = y_k[ell];
          // Check 2nd condition in BHZ09 theorem.
          if (y_k_ell >= x_k[ell])
            continue;
          const dimension_type cell = coherent_index(ell);
          row_reference ub_cell = *(ub_m_begin + cell);
          const N& ub_i_ell
            = (i == ell)
            ? temp_zero
            : ((ell < row_size_i) ? ub_i[ell] : ub_cell[ci]);
          const N& ub_cj_ell
            = (cj == ell)
            ? temp_zero
            : ((ell < row_size_cj) ? ub_cj[ell] : ub_cell[j]);
          // Check 3rd condition in BHZ09 theorem.
          add_assign_r(lhs, x_i_j, y_k_ell, ROUND_UP);
          add_assign_r(rhs, ub_i_ell, ub_k_j, ROUND_UP);
          if (lhs >= rhs)
            continue;
          // Check 4th condition in BHZ09 theorem.
          add_assign_r(rhs, ub_i_ck, ub_cj_ell, ROUND_UP);
          if (lhs >= rhs)
            continue;
          // Check 5th condition in BHZ09 theorem.
          assign_r(lhs_copy, lhs, ROUND_NOT_NEEDED);
          add_assign_r(lhs, lhs_copy, x_i_j, ROUND_UP);
          add_assign_r(rhs, ub_i_ell, ub_i_ck, ROUND_UP);
          add_assign_r(rhs, rhs, ub_cj_j, ROUND_UP);
          if (lhs >= rhs)
            continue;
          // Check 6th condition in BHZ09 theorem.
          add_assign_r(rhs, ub_k_j, ub_cj_ell, ROUND_UP);
          add_assign_r(rhs, rhs, ub_i_ci, ROUND_UP);
          if (lhs >= rhs)
            continue;
          // Check 7th condition of BHZ09 theorem.
          add_assign_r(lhs, lhs_copy, y_k_ell, ROUND_UP);
          add_assign_r(rhs, ub_i_ell, ub_cj_ell, ROUND_UP);
          add_assign_r(rhs, rhs, ub_k_ck, ROUND_UP);
          if (lhs >= rhs)
            continue;
          // Check 8th (last) condition in BHZ09 theorem.
          add_assign_r(rhs, ub_k_j, ub_i_ck, ROUND_UP);
          add_assign_r(rhs, rhs, ub_cell[ell], ROUND_UP);
          if (lhs < rhs)
            // All 8 conditions are satisfied:
            // upper bound is not exact.
            return false;
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
bool
Octagonal_Shape<T>
::integer_upper_bound_assign_if_exact(const Octagonal_Shape& y) {
  PPL_COMPILE_TIME_CHECK(std::numeric_limits<T>::is_integer,
                         "Octagonal_Shape<T>::"
                         "integer_upper_bound_assign_if_exact(y):"
                         " T in not an integer datatype.");
  // Declare a const reference to *this (to avoid accidental modifications).
  const Octagonal_Shape& x = *this;
  const dimension_type x_space_dim = x.space_dimension();

  if (x_space_dim != y.space_dimension())
    throw_dimension_incompatible("integer_upper_bound_assign_if_exact(y)", y);

  // The zero-dim case is trivial.
  if (x_space_dim == 0) {
    upper_bound_assign(y);
    return true;
  }

  // If `x' or `y' is (known to) contain no integral point,
  // then the integer upper bound can be computed exactly by tight closure.
  if (x.marked_empty()) {
    *this = y;
    tight_closure_assign();
    return true;
  }
  else if (y.marked_empty()) {
    tight_closure_assign();
    return true;
  }
  else if (x.is_empty() || x.tight_coherence_would_make_empty()) {
    *this = y;
    tight_closure_assign();
    return true;
  }
  else if (y.is_empty() || y.tight_coherence_would_make_empty()) {
    tight_closure_assign();
    return true;
  }

  // Here both `x' and `y' are known to be non-empty (and Z-consistent).
  PPL_ASSERT(x.marked_strongly_closed());
  PPL_ASSERT(y.marked_strongly_closed());
  // Pre-compute the integer upper bound of `x' and `y':
  // have to take copies, since tight closure might modify the rational shape.
  Octagonal_Shape<T> tx(x);
  tx.tight_closure_assign();
  Octagonal_Shape<T> ty(y);
  ty.tight_closure_assign();
  Octagonal_Shape<T> ub(tx);
  ub.upper_bound_assign(ty);

  // Compute redundancy information for tx and ty.
  // TODO: provide a nicer data structure for redundancy.
  // NOTE: there is no need to identify all redundancies, since this is
  // an optimization; hence we reuse the strong-reduction helper methods.
  std::vector<Bit_Row> tx_non_red;
  tx.non_redundant_matrix_entries(tx_non_red);
  std::vector<Bit_Row> ty_non_red;
  ty.non_redundant_matrix_entries(ty_non_red);

  PPL_DIRTY_TEMP(N, lhs_i_j);
  PPL_DIRTY_TEMP(N, lhs_k_ell);
  PPL_DIRTY_TEMP(N, lhs);
  PPL_DIRTY_TEMP(N, lhs_copy);
  PPL_DIRTY_TEMP(N, rhs);
  PPL_DIRTY_TEMP(N, temp_zero);
  assign_r(temp_zero, 0, ROUND_NOT_NEEDED);
  PPL_DIRTY_TEMP(N, temp_one);
  assign_r(temp_one, 1, ROUND_NOT_NEEDED);
  PPL_DIRTY_TEMP(N, temp_two);
  assign_r(temp_two, 2, ROUND_NOT_NEEDED);

  typedef typename OR_Matrix<N>::const_row_iterator row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type row_reference;
  const dimension_type n_rows = tx.matrix.num_rows();
  const row_iterator tx_m_begin = tx.matrix.row_begin();
  const row_iterator ty_m_begin = ty.matrix.row_begin();
  const row_iterator ub_m_begin = ub.matrix.row_begin();

  for (dimension_type i = n_rows; i-- > 0; ) {
    const Bit_Row& tx_non_red_i = tx_non_red[i];
    using namespace Implementation::Octagonal_Shapes;
    const dimension_type ci = coherent_index(i);
    const dimension_type row_size_i = OR_Matrix<N>::row_size(i);
    row_reference tx_i = *(tx_m_begin + i);
    row_reference ty_i = *(ty_m_begin + i);
    row_reference ub_i = *(ub_m_begin + i);
    const N& ub_i_ci = ub_i[ci];
    for (dimension_type j = row_size_i; j-- > 0; ) {
      // Check redundancy of tx_i_j.
      if (!tx_non_red_i[j])
        continue;
      const N& tx_i_j = tx_i[j];
      const dimension_type cj = coherent_index(j);
      const N& eps_i_j = (i == cj) ? temp_two : temp_one;
      // Check condition 1a in BHZ09 Theorem 6.8.
      add_assign_r(lhs_i_j, tx_i_j, eps_i_j, ROUND_NOT_NEEDED);
      if (lhs_i_j > ty_i[j])
        continue;
      const dimension_type row_size_cj = OR_Matrix<N>::row_size(cj);
      row_reference ub_cj = *(ub_m_begin + cj);
      const N& ub_cj_j = ub_cj[j];
      for (dimension_type k = 0; k < n_rows; ++k) {
        const Bit_Row& ty_non_red_k = ty_non_red[k];
        const dimension_type ck = coherent_index(k);
        const dimension_type row_size_k = OR_Matrix<N>::row_size(k);
        row_reference tx_k = *(tx_m_begin + k);
        row_reference ty_k = *(ty_m_begin + k);
        row_reference ub_k = *(ub_m_begin + k);
        const N& ub_k_ck = ub_k[ck];
        // Be careful: for each index h, the diagonal element m[h][h]
        // is (by convention) +infty in our implementation; however,
        // BHZ09 theorem assumes that it is equal to 0.
        const N& ub_k_j
          = (k == j)
          ? temp_zero
          : ((j < row_size_k) ? ub_k[j] : ub_cj[ck]);
        const N& ub_i_ck
          = (i == ck)
          ? temp_zero
          : ((ck < row_size_i) ? ub_i[ck] : ub_k[ci]);

        for (dimension_type ell = row_size_k; ell-- > 0; ) {
          // Check redundancy of y_k_ell.
          if (!ty_non_red_k[ell])
            continue;
          const N& ty_k_ell = ty_k[ell];
          const dimension_type cell = coherent_index(ell);
          const N& eps_k_ell = (k == cell) ? temp_two : temp_one;
          // Check condition 1b in BHZ09 Theorem 6.8.
          add_assign_r(lhs_k_ell, ty_k_ell, eps_k_ell, ROUND_NOT_NEEDED);
          if (lhs_k_ell > tx_k[ell])
            continue;
          row_reference ub_cell = *(ub_m_begin + cell);
          const N& ub_i_ell
            = (i == ell)
            ? temp_zero
            : ((ell < row_size_i) ? ub_i[ell] : ub_cell[ci]);
          const N& ub_cj_ell
            = (cj == ell)
            ? temp_zero
            : ((ell < row_size_cj) ? ub_cj[ell] : ub_cell[j]);
          // Check condition 2a in BHZ09 Theorem 6.8.
          add_assign_r(lhs, lhs_i_j, lhs_k_ell, ROUND_NOT_NEEDED);
          add_assign_r(rhs, ub_i_ell, ub_k_j, ROUND_NOT_NEEDED);
          if (lhs > rhs)
            continue;
          // Check condition 2b in BHZ09 Theorem 6.8.
          add_assign_r(rhs, ub_i_ck, ub_cj_ell, ROUND_NOT_NEEDED);
          if (lhs > rhs)
            continue;
          // Check condition 3a in BHZ09 Theorem 6.8.
          assign_r(lhs_copy, lhs, ROUND_NOT_NEEDED);
          add_assign_r(lhs, lhs, lhs_i_j, ROUND_NOT_NEEDED);
          add_assign_r(rhs, ub_i_ell, ub_i_ck, ROUND_NOT_NEEDED);
          add_assign_r(rhs, rhs, ub_cj_j, ROUND_NOT_NEEDED);
          if (lhs > rhs)
            continue;
          // Check condition 3b in BHZ09 Theorem 6.8.
          add_assign_r(rhs, ub_k_j, ub_cj_ell, ROUND_NOT_NEEDED);
          add_assign_r(rhs, rhs, ub_i_ci, ROUND_NOT_NEEDED);
          if (lhs > rhs)
            continue;
          // Check condition 4a in BHZ09 Theorem 6.8.
          add_assign_r(lhs, lhs_copy, lhs_k_ell, ROUND_NOT_NEEDED);
          add_assign_r(rhs, ub_i_ell, ub_cj_ell, ROUND_NOT_NEEDED);
          add_assign_r(rhs, rhs, ub_k_ck, ROUND_NOT_NEEDED);
          if (lhs > rhs)
            continue;
          // Check condition 4b in BHZ09 Theorem 6.8.
          add_assign_r(rhs, ub_k_j, ub_i_ck, ROUND_NOT_NEEDED);
          add_assign_r(rhs, rhs, ub_cell[ell], ROUND_NOT_NEEDED);
          if (lhs <= rhs)
            // All 8 conditions are satisfied:
            // integer upper bound is not exact.
            return false;
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
Octagonal_Shape<T>::drop_some_non_integer_points(Complexity_Class) {
  if (std::numeric_limits<T>::is_integer)
    return;

  const dimension_type space_dim = space_dimension();
  strong_closure_assign();
  if (space_dim == 0 || marked_empty())
    return;

  for (typename OR_Matrix<N>::element_iterator i = matrix.element_begin(),
         i_end = matrix.element_end(); i != i_end; ++i)
    drop_some_non_integer_points_helper(*i);

  // Unary constraints should have an even integer boundary.
  PPL_DIRTY_TEMP(N, temp_one);
  assign_r(temp_one, 1, ROUND_NOT_NEEDED);
  for (dimension_type i = 0; i < 2*space_dim; i += 2) {
    const dimension_type ci = i + 1;
    N& mat_i_ci = matrix[i][ci];
    if (!is_plus_infinity(mat_i_ci) && !is_even(mat_i_ci)) {
      sub_assign_r(mat_i_ci, mat_i_ci, temp_one, ROUND_UP);
      reset_strongly_closed();
    }
    N& mat_ci_i = matrix[ci][i];
    if (!is_plus_infinity(mat_ci_i) && !is_even(mat_ci_i)) {
      sub_assign_r(mat_ci_i, mat_ci_i, temp_one, ROUND_UP);
      reset_strongly_closed();
    }
  }

  PPL_ASSERT(OK());
}

template <typename T>
void
Octagonal_Shape<T>
::drop_some_non_integer_points(const Variables_Set& vars,
                               Complexity_Class) {
  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dimension() < min_space_dim)
    throw_dimension_incompatible("drop_some_non_integer_points(vs, cmpl)",
                                 min_space_dim);

  if (std::numeric_limits<T>::is_integer || min_space_dim == 0)
    return;

  strong_closure_assign();
  if (marked_empty())
    return;

  PPL_DIRTY_TEMP(N, temp_one);
  assign_r(temp_one, 1, ROUND_NOT_NEEDED);

  const Variables_Set::const_iterator v_begin = vars.begin();
  const Variables_Set::const_iterator v_end = vars.end();
  PPL_ASSERT(v_begin != v_end);
  typedef typename OR_Matrix<N>::row_reference_type row_reference;
  for (Variables_Set::const_iterator v_i = v_begin; v_i != v_end; ++v_i) {
    const dimension_type i = 2 * (*v_i);
    const dimension_type ci = i + 1;
    row_reference m_i = matrix[i];
    row_reference m_ci = matrix[ci];

    // Unary constraints: should be even integers.
    N& m_i_ci = m_i[ci];
    if (!is_plus_infinity(m_i_ci)) {
      drop_some_non_integer_points_helper(m_i_ci);
      if (!is_even(m_i_ci)) {
        sub_assign_r(m_i_ci, m_i_ci, temp_one, ROUND_UP);
        reset_strongly_closed();
      }
    }
    N& m_ci_i = m_ci[i];
    if (!is_plus_infinity(m_ci_i)) {
      drop_some_non_integer_points_helper(m_ci_i);
      if (!is_even(m_ci_i)) {
        sub_assign_r(m_ci_i, m_ci_i, temp_one, ROUND_UP);
        reset_strongly_closed();
      }
    }

    // Binary constraints (note: only consider j < i).
    for (Variables_Set::const_iterator v_j = v_begin; v_j != v_i; ++v_j) {
      const dimension_type j = 2 * (*v_j);
      const dimension_type cj = j + 1;
      drop_some_non_integer_points_helper(m_i[j]);
      drop_some_non_integer_points_helper(m_i[cj]);
      drop_some_non_integer_points_helper(m_ci[j]);
      drop_some_non_integer_points_helper(m_ci[cj]);
    }
  }
  PPL_ASSERT(OK());
}

template <typename T>
template <typename U>
void
Octagonal_Shape<T>
::export_interval_constraints(U& dest) const {
  if (space_dim > dest.space_dimension())
    throw std::invalid_argument(
               "Octagonal_Shape<T>::export_interval_constraints");

  strong_closure_assign();

  if (marked_empty()) {
    dest.set_empty();
    return;
  }

  PPL_DIRTY_TEMP(N, lb);
  PPL_DIRTY_TEMP(N, ub);
  for (dimension_type i = space_dim; i-- > 0; ) {
    const dimension_type ii = 2*i;
    const dimension_type cii = ii + 1;

    // Set the upper bound.
    const N& twice_ub = matrix[cii][ii];
    if (!is_plus_infinity(twice_ub)) {
      assign_r(ub, twice_ub, ROUND_NOT_NEEDED);
      div_2exp_assign_r(ub, ub, 1, ROUND_UP);
      // FIXME: passing a raw value may not be general enough.
      if (!dest.restrict_upper(i, ub.raw_value()))
        return;
    }

    // Set the lower bound.
    const N& twice_lb = matrix[ii][cii];
    if (!is_plus_infinity(twice_lb)) {
      assign_r(lb, twice_lb, ROUND_NOT_NEEDED);
      neg_assign_r(lb, lb, ROUND_NOT_NEEDED);
      div_2exp_assign_r(lb, lb, 1, ROUND_DOWN);
      // FIXME: passing a raw value may not be general enough.
      if (!dest.restrict_lower(i, lb.raw_value()))
        return;
    }
  }

}

/*! \relates Parma_Polyhedra_Library::Octagonal_Shape */
template <typename T>
std::ostream&
IO_Operators::operator<<(std::ostream& s, const Octagonal_Shape<T>& oct) {
  // Handle special cases first.
  if (oct.marked_empty()) {
    s << "false";
    return s;
  }
  if (oct.is_universe()) {
    s << "true";
    return s;
  }

  typedef typename Octagonal_Shape<T>::coefficient_type N;
  typedef typename OR_Matrix<N>::const_row_iterator row_iterator;
  typedef typename OR_Matrix<N>::const_row_reference_type row_reference;

  // Records whether or not we still have to print the first constraint.
  bool first = true;

  row_iterator m_begin = oct.matrix.row_begin();
  row_iterator m_end = oct.matrix.row_end();

  // Temporaries.
  PPL_DIRTY_TEMP(N, negation);
  PPL_DIRTY_TEMP(N, half);
  // Go through all the unary constraints.
  // (Note: loop iterator is incremented in the loop body.)
  for (row_iterator i_iter = m_begin; i_iter != m_end; ) {
    const dimension_type i = i_iter.index();
    const Variable v_i(i/2);
    const N& c_i_ii = (*i_iter)[i + 1];
    ++i_iter;
    const N& c_ii_i = (*i_iter)[i];
    ++i_iter;
    // Check whether or not it is an equality constraint.
    if (is_additive_inverse(c_i_ii, c_ii_i)) {
      // It is an equality.
      PPL_ASSERT(!is_plus_infinity(c_i_ii) && !is_plus_infinity(c_ii_i));
      if (first)
        first = false;
      else
        s << ", ";
      // If the value bound can NOT be divided by 2 exactly,
      // then we output the constraint `2*v_i = bound'.
      if (div_2exp_assign_r(half, c_ii_i, 1,
                            ROUND_UP | ROUND_STRICT_RELATION)
          == V_EQ)
        s << v_i << " = " << half;
      else
        s << "2*" << v_i << " = " << c_ii_i;
    }
    else {
      // We will print unary non-strict inequalities, if any.
      if (!is_plus_infinity(c_i_ii)) {
        if (first)
          first = false;
        else
          s << ", ";
        neg_assign_r(negation, c_i_ii, ROUND_NOT_NEEDED);
        // If the value bound can NOT be divided by 2 exactly,
        // then we output the constraint `2*v_i >= negation'.
        if (div_2exp_assign_r(half, negation, 1,
                              ROUND_UP | ROUND_STRICT_RELATION)
            == V_EQ)
          s << v_i << " >= " << half;
        else
          s << "2*" << v_i << " >= " << negation;
      }
      if (!is_plus_infinity(c_ii_i)) {
        if (first)
          first = false;
        else
          s << ", ";
        // If the value bound can NOT be divided by 2 exactly,
        // then we output the constraint `2*v_i <= bound'.
        if (div_2exp_assign_r(half, c_ii_i, 1,
                              ROUND_UP | ROUND_STRICT_RELATION)
            == V_EQ)
          s << v_i << " <= " << half;
        else
          s << "2*" << v_i << " <= " << c_ii_i;
      }
    }
  }

  // Go through all the binary constraints.
  // (Note: loop iterator is incremented in the loop body.)
  for (row_iterator i_iter = m_begin; i_iter != m_end; ) {
    const dimension_type i = i_iter.index();
    const Variable v_i(i/2);
    row_reference r_i = *i_iter;
    ++i_iter;
    row_reference r_ii = *i_iter;
    ++i_iter;

    for (dimension_type j = 0; j < i; j += 2) {
      const Variable v_j(j/2);
      // Print binary differences.
      const N& c_ii_jj = r_ii[j + 1];
      const N& c_i_j = r_i[j];
      // Check whether or not it is an equality constraint.
      if (is_additive_inverse(c_ii_jj, c_i_j)) {
        // It is an equality.
        PPL_ASSERT(!is_plus_infinity(c_i_j) && !is_plus_infinity(c_ii_jj));
        if (first)
          first = false;
        else
          s << ", ";
        if (sgn(c_i_j) >= 0)
          s << v_j << " - " << v_i << " = " << c_i_j;
        else
          s << v_i << " - " << v_j << " = " << c_ii_jj;
      }
      else {
        // We will print non-strict inequalities, if any.
        if (!is_plus_infinity(c_i_j)) {
          if (first)
            first = false;
          else
            s << ", ";
          if (sgn(c_i_j) >= 0)
            s << v_j << " - " << v_i << " <= " << c_i_j;
          else {
            neg_assign_r(negation, c_i_j, ROUND_DOWN);
            s << v_i << " - " << v_j << " >= " << negation;
          }
        }
        if (!is_plus_infinity(c_ii_jj)) {
          if (first)
            first = false;
          else
            s << ", ";
          if (sgn(c_ii_jj) >= 0)
            s << v_i << " - " << v_j << " <= " << c_ii_jj;
          else {
            neg_assign_r(negation, c_ii_jj, ROUND_DOWN);
            s << v_j << " - " << v_i << " >= " << negation;
          }
        }
      }
      // Print binary sums.
      const N& c_i_jj = r_i[j + 1];
      const N& c_ii_j = r_ii[j];
      // Check whether or not it is an equality constraint.
      if (is_additive_inverse(c_i_jj, c_ii_j)) {
        // It is an equality.
        PPL_ASSERT(!is_plus_infinity(c_i_jj) && !is_plus_infinity(c_ii_j));
        if (first)
          first = false;
        else
          s << ", ";
        s << v_j << " + " << v_i << " = " << c_ii_j;
      }
      else {
        // We will print non-strict inequalities, if any.
        if (!is_plus_infinity(c_i_jj)) {
          if (first)
            first = false;
          else
            s << ", ";
          neg_assign_r(negation, c_i_jj, ROUND_DOWN);
          s << v_j << " + " << v_i << " >= " << negation;
        }
        if (!is_plus_infinity(c_ii_j)) {
          if (first)
            first = false;
          else
            s << ", ";
          s << v_j << " + " << v_i << " <= " << c_ii_j;
        }
      }
    }
  }
  return s;
}

template <typename T>
void
Octagonal_Shape<T>::ascii_dump(std::ostream& s) const {
  s << "space_dim "
    << space_dim
    << "\n";
  status.ascii_dump(s);
  s << "\n";
  matrix.ascii_dump(s);
}

PPL_OUTPUT_TEMPLATE_DEFINITIONS(T, Octagonal_Shape<T>)

template <typename T>
bool
Octagonal_Shape<T>::ascii_load(std::istream& s) {
  std::string str;

  if (!(s >> str) || str != "space_dim")
    return false;

  if (!(s >> space_dim))
    return false;

  if (!status.ascii_load(s))
    return false;

  if (!matrix.ascii_load(s))
    return false;

  PPL_ASSERT(OK());
  return true;
}

template <typename T>
memory_size_type
Octagonal_Shape<T>::external_memory_in_bytes() const {
  return matrix.external_memory_in_bytes();
}

template <typename T>
bool
Octagonal_Shape<T>::OK() const {
  // Check whether the matrix is well-formed.
  if (!matrix.OK())
    return false;

  // Check whether the status information is legal.
  if (!status.OK())
    return false;

  // All empty octagons are OK.
  if (marked_empty())
    return true;

  // 0-dim universe octagon is OK.
  if (space_dim == 0)
    return true;

  // MINUS_INFINITY cannot occur at all.
  for (typename OR_Matrix<N>::const_row_iterator i = matrix.row_begin(),
         matrix_row_end = matrix.row_end(); i != matrix_row_end; ++i) {
    typename OR_Matrix<N>::const_row_reference_type x_i = *i;
    for (dimension_type j = i.row_size(); j-- > 0; )
      if (is_minus_infinity(x_i[j])) {
#ifndef NDEBUG
        using namespace Parma_Polyhedra_Library::IO_Operators;
        std::cerr << "Octagonal_Shape::"
                  << "matrix[" << i.index() << "][" << j << "] = "
                  << x_i[j] << "!"
                  << std::endl;
#endif
        return false;
      }
  }

  // On the main diagonal only PLUS_INFINITY can occur.
  for (typename OR_Matrix<N>::const_row_iterator i = matrix.row_begin(),
         m_end = matrix.row_end(); i != m_end; ++i) {
    typename OR_Matrix<N>::const_row_reference_type r = *i;
    const N& m_i_i = r[i.index()];
    if (!is_plus_infinity(m_i_i)) {
#ifndef NDEBUG
      const dimension_type j = i.index();
      using namespace Parma_Polyhedra_Library::IO_Operators;
      std::cerr << "Octagonal_Shape::matrix[" << j << "][" << j << "] = "
                << m_i_i << "!  (+inf was expected.)\n";
#endif
      return false;
    }
  }

  // The following tests might result in false alarms when using floating
  // point coefficients: they are only meaningful if the coefficient type
  // base is exact (since otherwise strong closure is approximated).
  if (std::numeric_limits<coefficient_type_base>::is_exact) {

    // Check whether the closure information is legal.
    if (marked_strongly_closed()) {
      Octagonal_Shape x = *this;
      x.reset_strongly_closed();
      x.strong_closure_assign();
      if (x.matrix != matrix) {
#ifndef NDEBUG
        std::cerr << "Octagonal_Shape is marked as strongly closed "
                  << "but it is not!\n";
#endif
        return false;
      }
    }

    // A closed octagon must be strong-coherent.
    if (marked_strongly_closed())
      if (!is_strong_coherent()) {
#ifndef NDEBUG
        std::cerr << "Octagonal_Shape is not strong-coherent!\n";
#endif
        return false;
      }
  }

  // All checks passed.
  return true;
}


template <typename T>
void
Octagonal_Shape<T>
::throw_dimension_incompatible(const char* method,
                               const Octagonal_Shape& y) const {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension()
    << ", y->space_dimension() == " << y.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
Octagonal_Shape<T>
::throw_dimension_incompatible(const char* method,
                               dimension_type required_dim) const {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension()
    << ", required dimension == " << required_dim << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
Octagonal_Shape<T>::throw_dimension_incompatible(const char* method,
                                                 const Constraint& c) const {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension()
    << ", c->space_dimension == " << c.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
Octagonal_Shape<T>::throw_dimension_incompatible(const char* method,
                                                 const Congruence& cg) const {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension()
    << ", cg->space_dimension == " << cg.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
Octagonal_Shape<T>::throw_dimension_incompatible(const char* method,
                                                 const Generator& g) const {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension()
    << ", g->space_dimension == " << g.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
Octagonal_Shape<T>::throw_constraint_incompatible(const char* method) {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << "the constraint is incompatible.";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
Octagonal_Shape<T>::throw_expression_too_complex(const char* method,
                                                 const Linear_Expression& le) {
  using namespace IO_Operators;
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << le << " is too complex.";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
Octagonal_Shape<T>
::throw_dimension_incompatible(const char* method,
                               const char* le_name,
                               const Linear_Expression& le) const {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension()
    << ", " << le_name << "->space_dimension() == "
    << le.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
template <typename C>
void
Octagonal_Shape<T>
::throw_dimension_incompatible(const char* method,
                               const char* lf_name,
                               const Linear_Form<C>& lf) const {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << "this->space_dimension() == " << space_dimension()
    << ", " << lf_name << "->space_dimension() == "
    << lf.space_dimension() << ".";
  throw std::invalid_argument(s.str());
}

template <typename T>
void
Octagonal_Shape<T>::throw_invalid_argument(const char* method,
                                           const char* reason) {
  std::ostringstream s;
  s << "PPL::Octagonal_Shape::" << method << ":\n"
    << reason << ".";
  throw std::invalid_argument(s.str());
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Octagonal_Shape_templates_hh)
