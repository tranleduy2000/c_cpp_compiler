/* Polyhedron class implementation: non-inline template functions.
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

#ifndef PPL_Polyhedron_templates_hh
#define PPL_Polyhedron_templates_hh 1

#include "Generator_defs.hh"
#include "MIP_Problem_defs.hh"
#include "Interval_defs.hh"
#include "Linear_Form_defs.hh"
// For static method overflows.
#include "Floating_Point_Expression_defs.hh"
#include <algorithm>
#include <deque>

namespace Parma_Polyhedra_Library {

template <typename Interval>
Polyhedron::Polyhedron(Topology topol,
                       const Box<Interval>& box,
                       Complexity_Class)
  : con_sys(topol, default_con_sys_repr),
    gen_sys(topol, default_gen_sys_repr),
    sat_c(),
    sat_g() {
  // Initialize the space dimension as indicated by the box.
  space_dim = box.space_dimension();

  // Check for emptiness.
  if (box.is_empty()) {
    set_empty();
    return;
  }

  // Zero-dim universe polyhedron.
  if (space_dim == 0) {
    set_zero_dim_univ();
    return;
  }

  // Properly set the space dimension of `con_sys'.
  con_sys.set_space_dimension(space_dim);

  PPL_DIRTY_TEMP_COEFFICIENT(l_n);
  PPL_DIRTY_TEMP_COEFFICIENT(l_d);
  PPL_DIRTY_TEMP_COEFFICIENT(u_n);
  PPL_DIRTY_TEMP_COEFFICIENT(u_d);

  if (topol == NECESSARILY_CLOSED) {
    for (dimension_type k = space_dim; k-- > 0; ) {
      const Variable v_k = Variable(k);
      // See if we have a valid lower bound.
      bool l_closed = false;
      bool l_bounded = box.has_lower_bound(v_k, l_n, l_d, l_closed);
      // See if we have a valid upper bound.
      bool u_closed = false;
      bool u_bounded = box.has_upper_bound(v_k, u_n, u_d, u_closed);

      // See if we have an implicit equality constraint.
      if (l_bounded && u_bounded
          && l_closed && u_closed
          && l_n == u_n && l_d == u_d) {
        // Add the constraint `l_d*v_k == l_n'.
        con_sys.insert(l_d * v_k == l_n);
      }
      else {
        if (l_bounded)
          // Add the constraint `l_d*v_k >= l_n'.
          con_sys.insert(l_d * v_k >= l_n);
        if (u_bounded)
          // Add the constraint `u_d*v_k <= u_n'.
          con_sys.insert(u_d * v_k <= u_n);
      }
    }
  }
  else {
    // topol == NOT_NECESSARILY_CLOSED
    for (dimension_type k = space_dim; k-- > 0; ) {
      const Variable v_k = Variable(k);
      // See if we have a valid lower bound.
      bool l_closed = false;
      bool l_bounded = box.has_lower_bound(v_k, l_n, l_d, l_closed);
      // See if we have a valid upper bound.
      bool u_closed = false;
      bool u_bounded = box.has_upper_bound(v_k, u_n, u_d, u_closed);

      // See if we have an implicit equality constraint.
      if (l_bounded && u_bounded
          && l_closed && u_closed
          && l_n == u_n && l_d == u_d) {
        // Add the constraint `l_d*v_k == l_n'.
        con_sys.insert(l_d * v_k == l_n);
      }
      else {
        // Check if a lower bound constraint is required.
        if (l_bounded) {
          if (l_closed)
            // Add the constraint `l_d*v_k >= l_n'.
            con_sys.insert(l_d * v_k >= l_n);
          else
            // Add the constraint `l_d*v_k > l_n'.
            con_sys.insert(l_d * v_k > l_n);
        }
        // Check if an upper bound constraint is required.
        if (u_bounded) {
          if (u_closed)
            // Add the constraint `u_d*v_k <= u_n'.
            con_sys.insert(u_d * v_k <= u_n);
          else
            // Add the constraint `u_d*v_k < u_n'.
            con_sys.insert(u_d * v_k < u_n);
        }
      }
    }
  }

  // Adding the low-level constraints.
  con_sys.add_low_level_constraints();

  // Constraints are up-to-date.
  set_constraints_up_to_date();
  PPL_ASSERT_HEAVY(OK());
}

template <typename Partial_Function>
void
Polyhedron::map_space_dimensions(const Partial_Function& pfunc) {
  if (space_dim == 0)
    return;

  if (pfunc.has_empty_codomain()) {
    // All dimensions vanish: the polyhedron becomes zero_dimensional.
    if (marked_empty()
        || (has_pending_constraints()
            && !remove_pending_to_obtain_generators())
        || (!generators_are_up_to_date() && !update_generators())) {
      // Removing all dimensions from the empty polyhedron.
      space_dim = 0;
      con_sys.clear();
    }
    else
      // Removing all dimensions from a non-empty polyhedron.
      set_zero_dim_univ();

    PPL_ASSERT_HEAVY(OK());
    return;
  }

  const dimension_type new_space_dimension = pfunc.max_in_codomain() + 1;

  if (new_space_dimension == space_dim) {
    // The partial function `pfunc' is indeed total and thus specifies
    // a permutation, that is, a renaming of the dimensions.  For
    // maximum efficiency, we will simply permute the columns of the
    // constraint system and/or the generator system.

    std::vector<Variable> cycle;
    cycle.reserve(space_dim);

    // Used to mark elements as soon as they are inserted in a cycle.
    std::deque<bool> visited(space_dim);

    for (dimension_type i = space_dim; i-- > 0; ) {
      if (visited[i])
        continue;

      dimension_type j = i;
      do {
        visited[j] = true;
        // The following initialization is only to make the compiler happy.
        dimension_type k = 0;
        if (!pfunc.maps(j, k))
          throw_invalid_argument("map_space_dimensions(pfunc)",
                                 " pfunc is inconsistent");
        if (k == j)
          break;

        cycle.push_back(Variable(j));
        // Go along the cycle.
        j = k;
      } while (!visited[j]);

      // End of cycle.

      // Permute all that is up-to-date.  Notice that the contents of
      // the saturation matrices is unaffected by the permutation of
      // columns: they remain valid, if they were so.
      if (constraints_are_up_to_date())
        con_sys.permute_space_dimensions(cycle);

      if (generators_are_up_to_date())
        gen_sys.permute_space_dimensions(cycle);

      cycle.clear();
    }

    PPL_ASSERT_HEAVY(OK());
    return;
  }

  // If control gets here, then `pfunc' is not a permutation and some
  // dimensions must be projected away.

  // If there are pending constraints, using `generators()' we process them.
  const Generator_System& old_gensys = generators();

  if (old_gensys.has_no_rows()) {
    // The polyhedron is empty.
    Polyhedron new_polyhedron(topology(), new_space_dimension, EMPTY);
    m_swap(new_polyhedron);
    PPL_ASSERT_HEAVY(OK());
    return;
  }

  // Make a local copy of the partial function.
  std::vector<dimension_type> pfunc_maps(space_dim, not_a_dimension());
  for (dimension_type j = space_dim; j-- > 0; ) {
    dimension_type pfunc_j;
    if (pfunc.maps(j, pfunc_j))
      pfunc_maps[j] = pfunc_j;
  }

  Generator_System new_gensys;
  for (Generator_System::const_iterator i = old_gensys.begin(),
         old_gensys_end = old_gensys.end(); i != old_gensys_end; ++i) {
    const Generator& old_g = *i;
    const Generator::expr_type old_e = old_g.expression();
    Linear_Expression expr;
    expr.set_space_dimension(new_space_dimension);
    bool all_zeroes = true;
    for (Generator::expr_type::const_iterator j = old_e.begin(),
          j_end = old_e.end(); j != j_end; ++j) {
      const dimension_type mapped_id = pfunc_maps[j.variable().id()];
      if (mapped_id != not_a_dimension()) {
        add_mul_assign(expr, *j, Variable(mapped_id));
        all_zeroes = false;
      }
    }
    switch (old_g.type()) {
    case Generator::LINE:
      if (!all_zeroes)
        new_gensys.insert(line(expr));
      break;
    case Generator::RAY:
      if (!all_zeroes)
        new_gensys.insert(ray(expr));
      break;
    case Generator::POINT:
      // A point in the origin has all zero homogeneous coefficients.
      new_gensys.insert(point(expr, old_g.divisor()));
      break;
    case Generator::CLOSURE_POINT:
      // A closure point in the origin has all zero homogeneous coefficients.
      new_gensys.insert(closure_point(expr, old_g.divisor()));
      break;
    }
  }
  Polyhedron new_polyhedron(topology(), new_gensys);
  m_swap(new_polyhedron);
  PPL_ASSERT_HEAVY(OK(true));
}

template <typename FP_Format, typename Interval_Info>
void
Polyhedron::refine_with_linear_form_inequality(
  const Linear_Form< Interval<FP_Format, Interval_Info> >& left,
  const Linear_Form< Interval<FP_Format, Interval_Info> >& right,
  const bool is_strict) {

  // Check that FP_Format is indeed a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<FP_Format>::is_exact,
                         "Polyhedron::refine_with_linear_form_inequality:"
                         " FP_Format not a floating point type.");

  // Dimension compatibility checks.
  // The dimensions of left and right should not be greater than the
  // dimension of *this.
  const dimension_type left_space_dim = left.space_dimension();
  if (space_dim < left_space_dim)
    throw_dimension_incompatible(
          "refine_with_linear_form_inequality(l1, l2, s)", "l1", left);

  const dimension_type right_space_dim = right.space_dimension();
  if (space_dim < right_space_dim)
    throw_dimension_incompatible(
          "refine_with_linear_form_inequality(l1, l2, s)", "l2", right);

  // We assume that the analyzer will not refine an unreachable test.
  PPL_ASSERT(!marked_empty());

  typedef Interval<FP_Format, Interval_Info> FP_Interval_Type;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;

  if (Floating_Point_Expression<FP_Interval_Type, float_ieee754_single>::
      overflows(left))
    return;

  if (Floating_Point_Expression<FP_Interval_Type, float_ieee754_single>::
      overflows(right))
    return;

  // Overapproximate left - right.
  FP_Linear_Form left_minus_right(left);
  left_minus_right -= right;
  if (Floating_Point_Expression<FP_Interval_Type, float_ieee754_single>::
      overflows(left_minus_right))
    return;

  dimension_type lf_space_dim = left_minus_right.space_dimension();
  FP_Linear_Form lf_approx;
  overapproximate_linear_form(left_minus_right, lf_space_dim, lf_approx);
  if (Floating_Point_Expression<FP_Interval_Type, float_ieee754_single>::
      overflows(lf_approx))
    return;

  // Normalize left - right.
  Linear_Expression lf_approx_le;
  convert_to_integer_expression(lf_approx, lf_space_dim, lf_approx_le);

  // Finally, do the refinement.
  if (!is_strict || is_necessarily_closed())
    refine_with_constraint(lf_approx_le <= 0);
  else
    refine_with_constraint(lf_approx_le < 0);
}

template <typename FP_Format, typename Interval_Info>
void
Polyhedron::affine_form_image(const Variable var,
const Linear_Form<Interval <FP_Format, Interval_Info> >& lf) {

  // Check that FP_Format is indeed a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<FP_Format>::is_exact,
                         "Polyhedron::affine_form_image:"
                         " FP_Format not a floating point type.");

  // Dimension compatibility checks.
  // The dimension of lf should not be greater than the dimension of *this.
  const dimension_type lf_space_dim = lf.space_dimension();
  if (space_dim < lf_space_dim)
    throw_dimension_incompatible("affine_form_image(v, l, s)", "l", lf);

  // `var' should be one of the dimensions of the polyhedron.
  const dimension_type var_id = var.id();
  if (space_dim < var_id + 1)
    throw_dimension_incompatible("affine_form_image(v, l, s)", "v", var);

  // We assume that the analyzer will not perform an unreachable assignment.
  PPL_ASSERT(!marked_empty());

  typedef Interval<FP_Format, Interval_Info> FP_Interval_Type;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;

  if (Floating_Point_Expression<FP_Interval_Type, float_ieee754_single>::
      overflows(lf)) {
    *this = Polyhedron(topology(), space_dim, UNIVERSE);
    return;
  }

  // Overapproximate lf.
  FP_Linear_Form lf_approx;
  overapproximate_linear_form(lf, lf_space_dim, lf_approx);

  if (Floating_Point_Expression<FP_Interval_Type, float_ieee754_single>::
      overflows(lf_approx)) {
    *this = Polyhedron(topology(), space_dim, UNIVERSE);
    return;
  }

  // Normalize lf.
  Linear_Expression lf_approx_le;
  PPL_DIRTY_TEMP_COEFFICIENT(lo_coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(hi_coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(denominator);
  convert_to_integer_expressions(lf_approx, lf_space_dim, lf_approx_le,
                                 lo_coeff, hi_coeff, denominator);

  // Finally, do the assignment.
  bounded_affine_image(var, lf_approx_le + lo_coeff, lf_approx_le + hi_coeff,
                       denominator);
}

template <typename FP_Format, typename Interval_Info>
void
Polyhedron::overapproximate_linear_form
(const Linear_Form<Interval <FP_Format, Interval_Info> >& lf,
 const dimension_type lf_dimension,
 Linear_Form<Interval <FP_Format, Interval_Info> >& result) {

  // Check that FP_Format is indeed a floating point type.
  PPL_COMPILE_TIME_CHECK(!std::numeric_limits<FP_Format>::is_exact,
                         "Polyhedron::overapproximate_linear_form:"
                         " FP_Format not a floating point type.");

  typedef Interval<FP_Format, Interval_Info> FP_Interval_Type;
  typedef Linear_Form<FP_Interval_Type> FP_Linear_Form;

  // Build a Box from the Polyhedron so that we can extract upper and
  // lower bounds of variables easily.
  Box<FP_Interval_Type> box(*this);

  result = FP_Linear_Form(lf.inhomogeneous_term());
  // FIXME: this may not be policy-neutral.
  const FP_Interval_Type aux_divisor1(static_cast<FP_Format>(0.5));
  FP_Interval_Type aux_divisor2(aux_divisor1);
  aux_divisor2.lower() = static_cast<FP_Format>(-0.5);

  for (dimension_type i = 0; i < lf_dimension; ++i) {
    Variable curr_var(i);
    const FP_Interval_Type& curr_coeff = lf.coefficient(curr_var);
    PPL_ASSERT(curr_coeff.is_bounded());
    FP_Format curr_lb = curr_coeff.lower();
    FP_Format curr_ub = curr_coeff.upper();
    if (curr_lb != 0 || curr_ub != 0) {
      const FP_Interval_Type& curr_int = box.get_interval(curr_var);
      FP_Interval_Type curr_addend(curr_ub - curr_lb);
      curr_addend *= aux_divisor2;
      curr_addend *= curr_int;
      result += curr_addend;
      curr_addend = FP_Interval_Type(curr_lb + curr_ub);
      curr_addend *= aux_divisor1;
      FP_Linear_Form curr_addend_lf(curr_var);
      curr_addend_lf *= curr_addend;
      result += curr_addend_lf;
    }
  }
}

template <typename FP_Format, typename Interval_Info>
void
Polyhedron::convert_to_integer_expression(
                const Linear_Form<Interval <FP_Format, Interval_Info> >& lf,
                const dimension_type lf_dimension,
                Linear_Expression& result) {
  result = Linear_Expression();

  typedef Interval<FP_Format, Interval_Info> FP_Interval_Type;
  std::vector<Coefficient> numerators(lf_dimension+1);
  std::vector<Coefficient> denominators(lf_dimension+1);

  // Convert each floating point number to a pair <numerator, denominator>
  // and compute the lcm of all denominators.
  PPL_DIRTY_TEMP_COEFFICIENT(lcm);
  lcm = 1;
  const FP_Interval_Type& b = lf.inhomogeneous_term();
  // FIXME: are these checks numerator[i] != 0 really necessary?
  numer_denom(b.lower(), numerators[lf_dimension],
                         denominators[lf_dimension]);
  if (numerators[lf_dimension] != 0)
      lcm_assign(lcm, lcm, denominators[lf_dimension]);

  for (dimension_type i = 0; i < lf_dimension; ++i) {
    const FP_Interval_Type& curr_int = lf.coefficient(Variable(i));
    numer_denom(curr_int.lower(), numerators[i], denominators[i]);
    if (numerators[i] != 0)
      lcm_assign(lcm, lcm, denominators[i]);
  }

  for (dimension_type i = 0; i < lf_dimension; ++i) {
    if (numerators[i] != 0) {
      exact_div_assign(denominators[i], lcm, denominators[i]);
      numerators[i] *= denominators[i];
      result += numerators[i] * Variable(i);
    }
  }

  if (numerators[lf_dimension] != 0) {
    exact_div_assign(denominators[lf_dimension],
                     lcm, denominators[lf_dimension]);
    numerators[lf_dimension] *= denominators[lf_dimension];
    result += numerators[lf_dimension];
  }
}

template <typename FP_Format, typename Interval_Info>
void
Polyhedron::convert_to_integer_expressions(
                const Linear_Form<Interval <FP_Format, Interval_Info> >& lf,
                const dimension_type lf_dimension, Linear_Expression& res,
                Coefficient& res_low_coeff, Coefficient& res_hi_coeff,
                Coefficient& denominator) {
  res = Linear_Expression();

  typedef Interval<FP_Format, Interval_Info> FP_Interval_Type;
  std::vector<Coefficient> numerators(lf_dimension+2);
  std::vector<Coefficient> denominators(lf_dimension+2);

  // Convert each floating point number to a pair <numerator, denominator>
  // and compute the lcm of all denominators.
  Coefficient& lcm = denominator;
  lcm = 1;
  const FP_Interval_Type& b = lf.inhomogeneous_term();
  numer_denom(b.lower(), numerators[lf_dimension], denominators[lf_dimension]);
  // FIXME: are these checks numerator[i] != 0 really necessary?
  if (numerators[lf_dimension] != 0)
      lcm_assign(lcm, lcm, denominators[lf_dimension]);

  numer_denom(b.upper(), numerators[lf_dimension+1],
                         denominators[lf_dimension+1]);
  if (numerators[lf_dimension+1] != 0)
      lcm_assign(lcm, lcm, denominators[lf_dimension+1]);

  for (dimension_type i = 0; i < lf_dimension; ++i) {
    const FP_Interval_Type& curr_int = lf.coefficient(Variable(i));
    numer_denom(curr_int.lower(), numerators[i], denominators[i]);
    if (numerators[i] != 0)
      lcm_assign(lcm, lcm, denominators[i]);
  }

  for (dimension_type i = 0; i < lf_dimension; ++i) {
    if (numerators[i] != 0) {
      exact_div_assign(denominators[i], lcm, denominators[i]);
      numerators[i] *= denominators[i];
      res += numerators[i] * Variable(i);
    }
  }

  if (numerators[lf_dimension] != 0) {
    exact_div_assign(denominators[lf_dimension],
                     lcm, denominators[lf_dimension]);
    numerators[lf_dimension] *= denominators[lf_dimension];
    res_low_coeff = numerators[lf_dimension];
  }
  else
    res_low_coeff = Coefficient(0);

  if (numerators[lf_dimension+1] != 0) {
    exact_div_assign(denominators[lf_dimension+1],
                     lcm, denominators[lf_dimension+1]);
    numerators[lf_dimension+1] *= denominators[lf_dimension+1];
    res_hi_coeff = numerators[lf_dimension+1];
  }
  else
    res_hi_coeff = Coefficient(0);
}

template <typename C>
void
Polyhedron::throw_dimension_incompatible(const char* method,
                                         const char* lf_name,
                                         const Linear_Form<C>& lf) const {
  throw_dimension_incompatible(method, lf_name, lf.space_dimension());
}

template <typename Input>
Input&
Polyhedron::check_obj_space_dimension_overflow(Input& input,
                                               const Topology topol,
                                               const char* method,
                                               const char* reason) {
  check_space_dimension_overflow(input.space_dimension(),
                                 max_space_dimension(),
                                 topol, method, reason);
  return input;
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_Polyhedron_templates_hh)
