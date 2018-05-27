/* MIP_Problem class implementation: non-inline functions.
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
#include "MIP_Problem_defs.hh"
#include "globals_defs.hh"
#include "Checked_Number_defs.hh"
#include "Linear_Expression_defs.hh"
#include "Constraint_defs.hh"
#include "Constraint_System_defs.hh"
#include "Constraint_System_inlines.hh"
#include "Generator_defs.hh"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include "math_utilities_defs.hh"

#include <stdexcept>
#include <deque>
#include <vector>
#include <algorithm>
#include <cmath>

// TODO: Remove this when the sparse working cost has been tested enough.
#if PPL_USE_SPARSE_MATRIX

// These are needed for the linear_combine() method that takes a Dense_Row and
// a Sparse_Row.
#include "Dense_Row_defs.hh"
#include "Sparse_Row_defs.hh"

#endif // PPL_USE_SPARSE_MATRIX

#ifndef PPL_NOISY_SIMPLEX
#define PPL_NOISY_SIMPLEX 0
#endif

#ifndef PPL_SIMPLEX_USE_MIP_HEURISTIC
#define PPL_SIMPLEX_USE_MIP_HEURISTIC 1
#endif

#if PPL_NOISY_SIMPLEX
#include <iostream>
#endif


namespace PPL = Parma_Polyhedra_Library;

#if PPL_NOISY_SIMPLEX
namespace {

unsigned long num_iterations = 0;

unsigned mip_recursion_level = 0;

} // namespace
#endif // PPL_NOISY_SIMPLEX

PPL::MIP_Problem::MIP_Problem(const dimension_type dim)
  : external_space_dim(dim),
    internal_space_dim(0),
    tableau(),
    working_cost(0),
    mapping(),
    base(),
    status(PARTIALLY_SATISFIABLE),
    pricing(PRICING_STEEPEST_EDGE_FLOAT),
    initialized(false),
    input_cs(),
    inherited_constraints(0),
    first_pending_constraint(0),
    input_obj_function(),
    opt_mode(MAXIMIZATION),
    last_generator(point()),
    i_variables() {
  // Check for space dimension overflow.
  if (dim > max_space_dimension())
    throw std::length_error("PPL::MIP_Problem::MIP_Problem(dim, cs, obj, "
                            "mode):\n"
                            "dim exceeds the maximum allowed "
                            "space dimension.");
  PPL_ASSERT(OK());
}

PPL::MIP_Problem::MIP_Problem(const dimension_type dim,
                              const Constraint_System& cs,
                              const Linear_Expression& obj,
                              const Optimization_Mode mode)
  : external_space_dim(dim),
    internal_space_dim(0),
    tableau(),
    working_cost(0),
    mapping(),
    base(),
    status(PARTIALLY_SATISFIABLE),
    pricing(PRICING_STEEPEST_EDGE_FLOAT),
    initialized(false),
    input_cs(),
    inherited_constraints(0),
    first_pending_constraint(0),
    input_obj_function(obj),
    opt_mode(mode),
    last_generator(point()),
    i_variables() {
  // Check for space dimension overflow.
  if (dim > max_space_dimension())
    throw std::length_error("PPL::MIP_Problem::MIP_Problem(dim, cs, obj, "
                            "mode):\n"
                            "dim exceeds the maximum allowed"
                            "space dimension.");
  // Check the objective function.
  if (obj.space_dimension() > dim) {
    std::ostringstream s;
    s << "PPL::MIP_Problem::MIP_Problem(dim, cs, obj,"
      << " mode):\n"
      << "obj.space_dimension() == "<< obj.space_dimension()
      << " exceeds dim == "<< dim << ".";
    throw std::invalid_argument(s.str());
  }
  // Check the constraint system.
  if (cs.space_dimension() > dim) {
    std::ostringstream s;
    s << "PPL::MIP_Problem::MIP_Problem(dim, cs, obj, mode):\n"
      << "cs.space_dimension == " << cs.space_dimension()
      << " exceeds dim == " << dim << ".";
    throw std::invalid_argument(s.str());
  }
  if (cs.has_strict_inequalities())
    throw std::invalid_argument("PPL::MIP_Problem::"
                                "MIP_Problem(d, cs, obj, m):\n"
                                "cs contains strict inequalities.");
  // Actually copy the constraints.
  for (Constraint_System::const_iterator
         i = cs.begin(), i_end = cs.end(); i != i_end; ++i)
    add_constraint_helper(*i);

  PPL_ASSERT(OK());
}

void
PPL::MIP_Problem::add_constraint(const Constraint& c) {
  if (space_dimension() < c.space_dimension()) {
    std::ostringstream s;
    s << "PPL::MIP_Problem::add_constraint(c):\n"
      << "c.space_dimension() == "<< c.space_dimension() << " exceeds "
         "this->space_dimension == " << space_dimension() << ".";
    throw std::invalid_argument(s.str());
  }
  if (c.is_strict_inequality())
    throw std::invalid_argument("PPL::MIP_Problem::add_constraint(c):\n"
                                "c is a strict inequality.");
  add_constraint_helper(c);
  if (status != UNSATISFIABLE)
    status = PARTIALLY_SATISFIABLE;
  PPL_ASSERT(OK());
}

void
PPL::MIP_Problem::add_constraints(const Constraint_System& cs) {
  if (space_dimension() < cs.space_dimension()) {
    std::ostringstream s;
    s << "PPL::MIP_Problem::add_constraints(cs):\n"
      << "cs.space_dimension() == " << cs.space_dimension()
      << " exceeds this->space_dimension() == " << this->space_dimension()
      << ".";
    throw std::invalid_argument(s.str());
  }
  if (cs.has_strict_inequalities())
    throw std::invalid_argument("PPL::MIP_Problem::add_constraints(cs):\n"
                                "cs contains strict inequalities.");
  for (Constraint_System::const_iterator
         i = cs.begin(), i_end = cs.end(); i != i_end; ++i)
    add_constraint_helper(*i);
  if (status != UNSATISFIABLE)
    status = PARTIALLY_SATISFIABLE;
  PPL_ASSERT(OK());
}

void
PPL::MIP_Problem::set_objective_function(const Linear_Expression& obj) {
  if (space_dimension() < obj.space_dimension()) {
    std::ostringstream s;
    s << "PPL::MIP_Problem::set_objective_function(obj):\n"
      << "obj.space_dimension() == " << obj.space_dimension()
      << " exceeds this->space_dimension == " << space_dimension()
      << ".";
    throw std::invalid_argument(s.str());
  }
  input_obj_function = obj;
  if (status == UNBOUNDED || status == OPTIMIZED)
    status = SATISFIABLE;
  PPL_ASSERT(OK());
}

const PPL::Generator&
PPL::MIP_Problem::feasible_point() const {
  if (is_satisfiable())
    return last_generator;
  else
    throw std::domain_error("PPL::MIP_Problem::feasible_point():\n"
                            "*this is not satisfiable.");
}

const PPL::Generator&
PPL::MIP_Problem::optimizing_point() const {
  if (solve() == OPTIMIZED_MIP_PROBLEM)
    return last_generator;
  else
    throw std::domain_error("PPL::MIP_Problem::optimizing_point():\n"
                            "*this does not have an optimizing point.");
}

bool
PPL::MIP_Problem::is_satisfiable() const {
  // Check `status' to filter out trivial cases.
  switch (status) {
  case UNSATISFIABLE:
    PPL_ASSERT(OK());
    return false;
  case SATISFIABLE:
    // Intentionally fall through
  case UNBOUNDED:
    // Intentionally fall through.
  case OPTIMIZED:
    PPL_ASSERT(OK());
    return true;
  case PARTIALLY_SATISFIABLE:
    {
      MIP_Problem& x = const_cast<MIP_Problem&>(*this);
      // LP case.
      if (x.i_variables.empty())
        return x.is_lp_satisfiable();

      // MIP case.
      {
        // Temporarily relax the MIP into an LP problem.
        RAII_Temporary_Real_Relaxation relaxed(x);
        Generator p = point();
        relaxed.lp.is_lp_satisfiable();
#if PPL_NOISY_SIMPLEX
        mip_recursion_level = 0;
#endif // PPL_NOISY_SIMPLEX
        if (is_mip_satisfiable(relaxed.lp, relaxed.i_vars, p)) {
          x.last_generator = p;
          x.status = SATISFIABLE;
        }
        else
          x.status = UNSATISFIABLE;
      } // `relaxed' destroyed here: relaxation automatically reset.
      return (x.status == SATISFIABLE);
    }
  }
  // We should not be here!
  PPL_UNREACHABLE;
  return false;
}

PPL::MIP_Problem_Status
PPL::MIP_Problem::solve() const{
  switch (status) {
  case UNSATISFIABLE:
    PPL_ASSERT(OK());
    return UNFEASIBLE_MIP_PROBLEM;
  case UNBOUNDED:
    PPL_ASSERT(OK());
    return UNBOUNDED_MIP_PROBLEM;
  case OPTIMIZED:
    PPL_ASSERT(OK());
    return OPTIMIZED_MIP_PROBLEM;
  case SATISFIABLE:
    // Intentionally fall through
  case PARTIALLY_SATISFIABLE:
    {
      MIP_Problem& x = const_cast<MIP_Problem&>(*this);
      if (x.i_variables.empty()) {
        // LP case.
        if (x.is_lp_satisfiable()) {
          x.second_phase();
          if (x.status == UNBOUNDED)
            return UNBOUNDED_MIP_PROBLEM;
          else {
            PPL_ASSERT(x.status == OPTIMIZED);
            return OPTIMIZED_MIP_PROBLEM;
          }
        }
        return UNFEASIBLE_MIP_PROBLEM;
      }

      // MIP case.
      MIP_Problem_Status return_value;
      Generator g = point();
      {
        // Temporarily relax the MIP into an LP problem.
        RAII_Temporary_Real_Relaxation relaxed(x);
        if (relaxed.lp.is_lp_satisfiable())
          relaxed.lp.second_phase();
        else {
          x.status = UNSATISFIABLE;
          // NOTE: `relaxed' destroyed: relaxation automatically reset.
          return UNFEASIBLE_MIP_PROBLEM;
        }
        PPL_DIRTY_TEMP(mpq_class, incumbent_solution);
        bool have_incumbent_solution = false;

        MIP_Problem lp_copy(relaxed.lp, Inherit_Constraints());
        PPL_ASSERT(lp_copy.integer_space_dimensions().empty());
        return_value = solve_mip(have_incumbent_solution,
                                 incumbent_solution, g,
                                 lp_copy, relaxed.i_vars);
      } // `relaxed' destroyed here: relaxation automatically reset.

      switch (return_value) {
      case UNFEASIBLE_MIP_PROBLEM:
        x.status = UNSATISFIABLE;
        break;
      case UNBOUNDED_MIP_PROBLEM:
        x.status = UNBOUNDED;
        // A feasible point has been set in `solve_mip()', so that
        // a call to `feasible_point' will be successful.
        x.last_generator = g;
        break;
      case OPTIMIZED_MIP_PROBLEM:
        x.status = OPTIMIZED;
        // Set the internal generator.
        x.last_generator = g;
        break;
      }
      PPL_ASSERT(OK());
      return return_value;
    }
  }
  // We should not be here!
  PPL_UNREACHABLE;
  return UNFEASIBLE_MIP_PROBLEM;
}

void
PPL::MIP_Problem::add_space_dimensions_and_embed(const dimension_type m) {
  // The space dimension of the resulting MIP problem should not
  // overflow the maximum allowed space dimension.
  if (m > max_space_dimension() - space_dimension())
    throw std::length_error("PPL::MIP_Problem::"
                            "add_space_dimensions_and_embed(m):\n"
                            "adding m new space dimensions exceeds "
                            "the maximum allowed space dimension.");
  external_space_dim += m;
  if (status != UNSATISFIABLE)
    status = PARTIALLY_SATISFIABLE;
  PPL_ASSERT(OK());
}

void
PPL::MIP_Problem
::add_to_integer_space_dimensions(const Variables_Set& i_vars) {
  if (i_vars.space_dimension() > external_space_dim)
    throw std::invalid_argument("PPL::MIP_Problem::"
                                "add_to_integer_space_dimension(i_vars):\n"
                                "*this and i_vars are dimension"
                                "incompatible.");
  const dimension_type original_size = i_variables.size();
  i_variables.insert(i_vars.begin(), i_vars.end());
  // If a new integral variable was inserted, set the internal status to
  // PARTIALLY_SATISFIABLE.
  if (i_variables.size() != original_size && status != UNSATISFIABLE)
    status = PARTIALLY_SATISFIABLE;
}

bool
PPL::MIP_Problem::is_in_base(const dimension_type var_index,
                             dimension_type& row_index) const {
  for (row_index = base.size(); row_index-- > 0; )
    if (base[row_index] == var_index)
      return true;
  return false;
}

PPL::dimension_type
PPL::MIP_Problem::merge_split_variable(dimension_type var_index) {
  // Initialize the return value to a dummy index.
  dimension_type unfeasible_tableau_row = not_a_dimension();

  const dimension_type removing_column = mapping[1+var_index].second;

  // Check if the negative part of the split variable is in base:
  // if so, the corresponding row of the tableau becomes non-feasible.
  {
    dimension_type base_index;
    if (is_in_base(removing_column, base_index)) {
      // Set the return value.
      unfeasible_tableau_row = base_index;
      // Reset base[base_index] to zero to remember non-feasibility.
      base[base_index] = 0;
      // Since the negative part of the variable is in base,
      // the positive part can not be in base too.
      PPL_ASSERT(!is_in_base(mapping[1+var_index].first, base_index));
    }
  }

  tableau.remove_column(removing_column);

  // var_index is no longer split.
  mapping[1+var_index].second = 0;

  // Adjust data structures, `shifting' the proper columns to the left by 1.
  const dimension_type base_size = base.size();
  for (dimension_type i = base_size; i-- > 0; ) {
    if (base[i] > removing_column)
      --base[i];
  }
  const dimension_type mapping_size = mapping.size();
  for (dimension_type i = mapping_size; i-- > 0; ) {
    if (mapping[i].first > removing_column)
      --mapping[i].first;
    if (mapping[i].second > removing_column)
      --mapping[i].second;
  }

  return unfeasible_tableau_row;
}

bool
PPL::MIP_Problem::is_satisfied(const Constraint& c, const Generator& g) {
  // Scalar_Products::sign() requires the second argument to be at least
  // as large as the first one.
  const int sp_sign
    = (g.space_dimension() <= c.space_dimension())
    ? Scalar_Products::sign(g, c)
    : Scalar_Products::sign(c, g);
  return c.is_inequality() ? (sp_sign >= 0) : (sp_sign == 0);
}

bool
PPL::MIP_Problem::is_saturated(const Constraint& c, const Generator& g) {
  // Scalar_Products::sign() requires the space dimension of the second
  // argument to be at least as large as the one of the first one.
  const int sp_sign
    = (g.space_dimension() <= c.space_dimension())
    ? Scalar_Products::sign(g, c)
    : Scalar_Products::sign(c, g);
  return sp_sign == 0;
}

bool
PPL::MIP_Problem
::parse_constraints(dimension_type& additional_tableau_rows,
                    dimension_type& additional_slack_variables,
                    std::deque<bool>& is_tableau_constraint,
                    std::deque<bool>& is_satisfied_inequality,
                    std::deque<bool>& is_nonnegative_variable,
                    std::deque<bool>& is_remergeable_variable) const {
  // Initially all containers are empty.
  PPL_ASSERT(is_tableau_constraint.empty()
             && is_satisfied_inequality.empty()
             && is_nonnegative_variable.empty()
             && is_remergeable_variable.empty());

  const dimension_type cs_space_dim = external_space_dim;
  const dimension_type cs_num_rows = input_cs.size();
  const dimension_type cs_num_pending
    = cs_num_rows - first_pending_constraint;

  // Counters determining the change in dimensions of the tableau:
  // initialized here, they will be updated while examining `input_cs'.
  additional_tableau_rows = cs_num_pending;
  additional_slack_variables = 0;

  // Resize containers appropriately.

  // On exit, `is_tableau_constraint[i]' will be true if and only if
  // `input_cs[first_pending_constraint + i]' is neither a tautology
  // (e.g., 1 >= 0) nor a non-negativity constraint (e.g., X >= 0).
  is_tableau_constraint.insert(is_tableau_constraint.end(),
                               cs_num_pending, true);

  // On exit, `is_satisfied_inequality[i]' will be true if and only if
  // `input_cs[first_pending_constraint + i]' is an inequality and it is
  // satisfied by `last_generator'.
  is_satisfied_inequality.insert(is_satisfied_inequality.end(),
                                 cs_num_pending, false);

  // On exit, `is_nonnegative_variable[j]' will be true if and only if
  // Variable(j) is bound to be nonnegative in `input_cs'.
  is_nonnegative_variable.insert(is_nonnegative_variable.end(),
                                 cs_space_dim, false);

  // On exit, `is_remergeable_variable[j]' will be true if and only if
  // Variable(j) was initially split and is now remergeable.
  is_remergeable_variable.insert(is_remergeable_variable.end(),
                                 internal_space_dim, false);

  // Check for variables that are already known to be nonnegative
  // due to non-pending constraints.
  const dimension_type mapping_size = mapping.size();
  if (mapping_size > 0) {
    // Note: mapping[0] is associated to the cost function.
    for (dimension_type i = std::min(mapping_size - 1, cs_space_dim);
         i-- > 0; )
      if (mapping[i + 1].second == 0)
        is_nonnegative_variable[i] = true;
  }

  // Process each pending constraint in `input_cs' and
  //  - detect variables that are constrained to be nonnegative;
  //  - detect (non-negativity or tautology) pending constraints that
  //    will not be part of the tableau;
  //  - count the number of new slack variables.
  for (dimension_type i = cs_num_rows; i-- > first_pending_constraint; ) {
    const Constraint& cs_i = *(input_cs[i]);
    const dimension_type cs_i_end = cs_i.space_dimension() + 1;

    const dimension_type nonzero_coeff_column_index
      = cs_i.expression().first_nonzero(1, cs_i_end);
    const bool found_a_nonzero_coeff = (nonzero_coeff_column_index != cs_i_end);
    const bool found_many_nonzero_coeffs
      = (found_a_nonzero_coeff
         && !cs_i.expression().all_zeroes(nonzero_coeff_column_index + 1,
                                               cs_i_end));

    // If more than one coefficient is nonzero,
    // continue with next constraint.
    if (found_many_nonzero_coeffs) {
      if (cs_i.is_inequality())
        ++additional_slack_variables;
      // CHECKME: Is it true that in the first phase we can apply
      // `is_satisfied()' with the generator `point()'?  If so, the following
      // code works even if we do not have a feasible point.
      // Check for satisfiability of the inequality. This can be done if we
      // have a feasible point of *this.
      if (cs_i.is_inequality() && is_satisfied(cs_i, last_generator))
        is_satisfied_inequality[i - first_pending_constraint] = true;
      continue;
    }

    if (!found_a_nonzero_coeff) {
      // All coefficients are 0.
      // The constraint is either trivially true or trivially false.
      if (cs_i.is_inequality()) {
        if (cs_i.inhomogeneous_term() < 0)
          // A constraint such as -1 >= 0 is trivially false.
          return false;
      }
      else
        // The constraint is an equality.
        if (cs_i.inhomogeneous_term() != 0)
          // A constraint such as 1 == 0 is trivially false.
          return false;
      // Here the constraint is trivially true.
      is_tableau_constraint[i - first_pending_constraint] = false;
      --additional_tableau_rows;
      continue;
    }
    else {
      // Here we have only one nonzero coefficient.
      /*

      We have the following methods:
      A) Do split the variable and do add the constraint
         in the tableau.
      B) Do not split the variable and do add the constraint
         in the tableau.
      C) Do not split the variable and do not add the constraint
         in the tableau.

      Let the constraint be (a*v + b relsym 0).
      These are the 12 possible combinations we can have:
                a |  b | relsym | method
      ----------------------------------
      1)       >0 | >0 |   >=   |   A
      2)       >0 | >0 |   ==   |   A
      3)       <0 | <0 |   >=   |   A
      4)       >0 | =0 |   ==   |   B
      5)       >0 | <0 |   ==   |   B
      Note:    <0 | >0 |   ==   | impossible by strong normalization
      Note:    <0 | =0 |   ==   | impossible by strong normalization
      Note:    <0 | <0 |   ==   | impossible by strong normalization
      6)       >0 | <0 |   >=   |   B
      7)       >0 | =0 |   >=   |   C
      8)       <0 | >0 |   >=   |   A
      9)       <0 | =0 |   >=   |   A

      The next lines will apply the correct method to each case.
      */

      // The variable index is not equal to the column index.
      const dimension_type nonzero_var_index = nonzero_coeff_column_index - 1;

      const int sgn_a = sgn(cs_i.coefficient(Variable(nonzero_var_index)));
      const int sgn_b = sgn(cs_i.inhomogeneous_term());

      // Cases 1-3: apply method A.
      if (sgn_a == sgn_b) {
        if (cs_i.is_inequality())
          ++additional_slack_variables;
      }
      // Cases 4-5: apply method B.
      else if (cs_i.is_equality())
        is_nonnegative_variable[nonzero_var_index] = true;
      // Case 6: apply method B.
      else if (sgn_b < 0) {
        is_nonnegative_variable[nonzero_var_index] = true;
        ++additional_slack_variables;
      }
      // Case 7: apply method C.
      else if (sgn_a > 0) {
        if (!is_nonnegative_variable[nonzero_var_index]) {
          is_nonnegative_variable[nonzero_var_index] = true;
          if (nonzero_coeff_column_index < mapping_size) {
            // Remember to merge back the positive and negative parts.
            PPL_ASSERT(nonzero_var_index < internal_space_dim);
            is_remergeable_variable[nonzero_var_index] = true;
          }
        }
        is_tableau_constraint[i - first_pending_constraint] = false;
        --additional_tableau_rows;
      }
      // Cases 8-9: apply method A.
      else {
        PPL_ASSERT(cs_i.is_inequality());
        ++additional_slack_variables;
      }
    }
  }
  return true;
}

void
PPL::MIP_Problem::process_pending_constraints() {
  // Check the pending constraints to adjust the data structures.
  // If `false' is returned, they are trivially unfeasible.
  dimension_type additional_tableau_rows = 0;
  dimension_type additional_slack_vars = 0;
  std::deque<bool> is_tableau_constraint;
  std::deque<bool> is_satisfied_inequality;
  std::deque<bool> is_nonnegative_variable;
  std::deque<bool> is_remergeable_variable;
  if (!parse_constraints(additional_tableau_rows,
                         additional_slack_vars,
                         is_tableau_constraint,
                         is_satisfied_inequality,
                         is_nonnegative_variable,
                         is_remergeable_variable)) {
    status = UNSATISFIABLE;
    return;
  }

  // Merge back any variable that was previously split into a positive
  // and a negative part and is now known to be nonnegative.
  std::vector<dimension_type> unfeasible_tableau_rows;
  for (dimension_type i = internal_space_dim; i-- > 0; ) {
    if (!is_remergeable_variable[i])
      continue;
    // TODO: merging all rows in a single shot may be more efficient
    // as it would require a single call to permute_columns().
    const dimension_type unfeasible_row = merge_split_variable(i);
    if (unfeasible_row != not_a_dimension())
      unfeasible_tableau_rows.push_back(unfeasible_row);
  }

  const dimension_type old_tableau_num_rows = tableau.num_rows();
  const dimension_type old_tableau_num_cols = tableau.num_columns();
  const dimension_type first_free_tableau_index = old_tableau_num_cols - 1;

  // Update mapping for the new problem variables (if any).
  dimension_type additional_problem_vars = 0;
  if (external_space_dim > internal_space_dim) {
    const dimension_type space_diff = external_space_dim - internal_space_dim;
    for (dimension_type i = 0, j = 0; i < space_diff; ++i) {
      // Let `mapping' associate the variable index with the corresponding
      // tableau column: split the variable into positive and negative
      // parts if it is not known to be nonnegative.
      const dimension_type positive = first_free_tableau_index + j;
      if (is_nonnegative_variable[internal_space_dim + i]) {
        // Do not split.
        mapping.push_back(std::make_pair(positive, 0));
        ++j;
        ++additional_problem_vars;
      }
      else {
        // Split: negative index is positive + 1.
        mapping.push_back(std::make_pair(positive, positive + 1));
        j += 2;
        additional_problem_vars += 2;
      }
    }
  }

  // Resize the tableau: first add additional rows ...
  if (additional_tableau_rows > 0)
    tableau.add_zero_rows(additional_tableau_rows);

  // ... then add additional columns.
  // We need columns for additional (split) problem variables, additional
  // slack variables and additional artificials.
  // The number of artificials to be added is computed as:
  // * number of pending constraints entering the tableau
  //     minus
  // * pending constraints that are inequalities and are already satisfied
  //   by `last_generator'
  //     plus
  // * number of non-pending constraints that are no longer satisfied
  //   due to re-merging of split variables.

  const dimension_type num_satisfied_inequalities
    = static_cast<dimension_type>(std::count(is_satisfied_inequality.begin(),
                                             is_satisfied_inequality.end(),
                                             true));
  const dimension_type unfeasible_tableau_rows_size
    = unfeasible_tableau_rows.size();

  PPL_ASSERT(additional_tableau_rows >= num_satisfied_inequalities);
  const dimension_type additional_artificial_vars
    = (additional_tableau_rows - num_satisfied_inequalities)
    + unfeasible_tableau_rows_size;

  const dimension_type additional_tableau_columns
    = additional_problem_vars
    + additional_slack_vars
    + additional_artificial_vars;

  if (additional_tableau_columns > 0)
    tableau.add_zero_columns(additional_tableau_columns);

  // Dimensions of the tableau after resizing.
  const dimension_type tableau_num_rows = tableau.num_rows();
  const dimension_type tableau_num_cols = tableau.num_columns();

  // The following vector will be useful know if a constraint is feasible
  // and does not require an additional artificial variable.
  std::deque<bool> worked_out_row (tableau_num_rows, false);

  // Sync the `base' vector size to the new tableau: fill with zeros
  // to encode that these rows are not OK and must be adjusted.
  base.insert(base.end(), additional_tableau_rows, 0);
  const dimension_type base_size = base.size();

  // These indexes will be used to insert slack and artificial variables
  // in the appropriate position.
  dimension_type slack_index
    = tableau_num_cols - additional_artificial_vars - 1;
  dimension_type artificial_index = slack_index;

  // The first column index of the tableau that contains an
  // artificial variable. Encode with 0 the fact the there are not
  // artificial variables.
  const dimension_type begin_artificials
    = (additional_artificial_vars > 0)
    ? artificial_index
    : 0;

  // Proceed with the insertion of the constraints.
  for (dimension_type k = tableau_num_rows,
       i = input_cs.size() - first_pending_constraint; i-- > 0; ) {
    if (!is_tableau_constraint[i])
      continue;
    // Copy the original constraint in the tableau.
    Row& tableau_k = tableau[--k];
    Row::iterator itr = tableau_k.end();

    const Constraint& c = *(input_cs[i + first_pending_constraint]);
    const Constraint::expr_type c_e = c.expression();
    for (Constraint::expr_type::const_iterator j = c_e.begin(),
           j_end = c_e.end(); j != j_end; ++j) {
      Coefficient_traits::const_reference coeff_sd = *j;
      const std::pair<dimension_type, dimension_type> mapped
        = mapping[j.variable().space_dimension()];
      itr = tableau_k.insert(itr, mapped.first, coeff_sd);
      // Split if needed.
      if (mapped.second != 0) {
        itr = tableau_k.insert(itr, mapped.second);
        neg_assign(*itr, coeff_sd);
      }
    }
    Coefficient_traits::const_reference inhomo = c.inhomogeneous_term();
    if (inhomo != 0) {
      tableau_k.insert(itr, mapping[0].first, inhomo);
      // Split if needed.
      if (mapping[0].second != 0) {
        itr = tableau_k.insert(itr, mapping[0].second);
        neg_assign(*itr, inhomo);
      }
    }

    // Add the slack variable, if needed.
    if (c.is_inequality()) {
      neg_assign(tableau_k[--slack_index], Coefficient_one());
      // If the constraint is already satisfied, we will not use artificial
      // variables to compute a feasible base: this to speed up
      // the algorithm.
      if (is_satisfied_inequality[i]) {
        base[k] = slack_index;
        worked_out_row[k] = true;
      }
    }
    for (dimension_type j = base_size; j-- > 0; )
      if (k != j && base[j] != 0 && tableau_k.get(base[j]) != 0)
        linear_combine(tableau_k, tableau[j], base[j]);
  }

  // Let all inhomogeneous terms in the tableau be nonpositive,
  // so as to simplify the insertion of artificial variables
  // (the coefficient of each artificial variable will be 1).
  for (dimension_type i = tableau_num_rows; i-- > 0 ; ) {
    Row& tableau_i = tableau[i];
    if (tableau_i.get(0) > 0) {
      for (Row::iterator
           j = tableau_i.begin(), j_end = tableau_i.end(); j != j_end; ++j)
        neg_assign(*j);
    }
  }

  // Reset the working cost function to have the right size.
  working_cost = working_cost_type(tableau_num_cols);

  // Set up artificial variables: these will have coefficient 1 in the
  // constraint, will enter the base and will have coefficient -1 in
  // the cost function.

  // This is used as a hint for insertions in working_cost.
  working_cost_type::iterator cost_itr = working_cost.end();

  // First go through non-pending constraints that became unfeasible
  // due to re-merging of split variables.
  for (dimension_type i = 0; i < unfeasible_tableau_rows_size; ++i) {
    tableau[unfeasible_tableau_rows[i]].insert(artificial_index,
                                               Coefficient_one());
    cost_itr = working_cost.insert(cost_itr, artificial_index);
    *cost_itr = -1;
    base[unfeasible_tableau_rows[i]] = artificial_index;
    ++artificial_index;
  }
  // Then go through newly added tableau rows, disregarding inequalities
  // that are already satisfied by `last_generator' (this information
  // is encoded in `worked_out_row').
  for (dimension_type i = old_tableau_num_rows; i < tableau_num_rows; ++i) {
    if (worked_out_row[i])
      continue;
    tableau[i].insert(artificial_index, Coefficient_one());
    cost_itr = working_cost.insert(cost_itr, artificial_index);
    *cost_itr = -1;
    base[i] = artificial_index;
    ++artificial_index;
  }
  // One past the last tableau column index containing an artificial variable.
  const dimension_type end_artificials = artificial_index;

  // Set the extra-coefficient of the cost functions to record its sign.
  // This is done to keep track of the possible sign's inversion.
  const dimension_type last_obj_index = working_cost.size() - 1;
  working_cost.insert(cost_itr, last_obj_index, Coefficient_one());

  // Express the problem in terms of the variables in base.
  {
    working_cost_type::const_iterator itr = working_cost.end();
    for (dimension_type i = tableau_num_rows; i-- > 0; ) {
      itr = working_cost.lower_bound(itr, base[i]);
      if (itr != working_cost.end() && itr.index() == base[i] && *itr != 0) {
        linear_combine(working_cost, tableau[i], base[i]);
        // itr has been invalidated by the call to linear_combine().
        itr = working_cost.end();
      }
    }
  }

  // Deal with zero dimensional problems.
  if (space_dimension() == 0) {
    status = OPTIMIZED;
    last_generator = point();
    return;
  }
  // Deal with trivial cases.
  // If there is no constraint in the tableau, then the feasible region
  // is only delimited by non-negativity constraints. Therefore,
  // the problem is unbounded as soon as the cost function has
  // a variable with a positive coefficient.
  if (tableau_num_rows == 0) {
    if (is_unbounded_obj_function(input_obj_function, mapping, opt_mode)) {
      // Ensure the right space dimension is obtained.
      last_generator = point();
      last_generator.set_space_dimension(space_dimension());
      status = UNBOUNDED;
      return;
    }

    // The problem is neither trivially unfeasible nor trivially unbounded.
    // The tableau was successful computed and the caller has to figure
    // out which case applies.
    status = OPTIMIZED;
    // Ensure the right space dimension is obtained.
    last_generator = point();
    last_generator.set_space_dimension(space_dimension());
    return;
  }

  // Now we are ready to solve the first phase.
  const bool first_phase_successful
    = (get_control_parameter(PRICING) == PRICING_STEEPEST_EDGE_FLOAT)
    ? compute_simplex_using_steepest_edge_float()
    : compute_simplex_using_exact_pricing();

#if PPL_NOISY_SIMPLEX
  std::cout << "MIP_Problem::process_pending_constraints(): "
            << "1st phase ended at iteration " << num_iterations
            << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX

  if (!first_phase_successful || working_cost.get(0) != 0) {
    // The feasible region is empty.
    status = UNSATISFIABLE;
    return;
  }

  // Prepare *this for a possible second phase.
  if (begin_artificials != 0)
    erase_artificials(begin_artificials, end_artificials);
  compute_generator();
  status = SATISFIABLE;
}

namespace {

// NOTE: the following two `assign' helper functions are needed to
// handle the assignment of a Coefficient to a double in method
//     MIP_Problem::steepest_edge_float_entering_index().
// We cannot use assign_r(double, Coefficient, Rounding_Dir) as it would
// lead to a compilation error on those platforms (e.g., ARM) where
// controlled floating point rounding is not available (even if the
// rounding mode would be set to ROUND_IGNORE).

inline void
assign(double& d, const mpz_class& c) {
  d = c.get_d();
}

template <typename T, typename Policy>
inline void
assign(double& d,
       const Parma_Polyhedra_Library::Checked_Number<T, Policy>& c) {
  d = raw_value(c);
}

} // namespace

PPL::dimension_type
PPL::MIP_Problem::steepest_edge_float_entering_index() const {
  const dimension_type tableau_num_rows = tableau.num_rows();
  const dimension_type tableau_num_columns = tableau.num_columns();
  PPL_ASSERT(tableau_num_rows == base.size());
  double current_value = 0.0;
  // Due to our integer implementation, the `1' term in the denominator
  // of the original formula has to be replaced by `squared_lcm_basis'.
  double float_tableau_value;
  double float_tableau_denom;
  dimension_type entering_index = 0;
  const int cost_sign = sgn(working_cost.get(working_cost.size() - 1));

  // These two implementation work for both sparse and dense matrices.
  // However, when using sparse matrices the first one is fast and the second
  // one is slow, and when using dense matrices the first one is slow and
  // the second one is fast.
#if PPL_USE_SPARSE_MATRIX

  const dimension_type tableau_num_columns_minus_1 = tableau_num_columns - 1;
  // This is static to improve performance.
  // A vector of <column_index, challenger_denom> pairs, ordered by
  // column_index.
  static std::vector<std::pair<dimension_type, double> > columns;
  columns.clear();
  // (working_cost.size() - 2) is an upper bound only.
  columns.reserve(working_cost.size() - 2);
  {
    working_cost_type::const_iterator i = working_cost.lower_bound(1);
    // Note that find() is used instead of lower_bound().
    working_cost_type::const_iterator i_end
      = working_cost.find(tableau_num_columns_minus_1);
    for ( ; i != i_end; ++i)
      if (sgn(*i) == cost_sign)
        columns.push_back(std::make_pair(i.index(), 1.0));
  }
  for (dimension_type i = tableau_num_rows; i-- > 0; ) {
    const Row& tableau_i = tableau[i];
    assign(float_tableau_denom, tableau_i.get(base[i]));
    Row::const_iterator j = tableau_i.begin();
    Row::const_iterator j_end = tableau_i.end();
    std::vector<std::pair<dimension_type, double> >::iterator k
      = columns.begin();
    std::vector<std::pair<dimension_type, double> >::iterator k_end
      = columns.end();
    while (j != j_end && k != k_end) {
      const dimension_type column = j.index();
      while (k != k_end && column > k->first)
        ++k;
      if (k == k_end)
        break;
      if (k->first > column) {
        j = tableau_i.lower_bound(j, k->first);
      }
      else {
        PPL_ASSERT(k->first == column);
        PPL_ASSERT(tableau_i.get(base[i]) != 0);
        WEIGHT_BEGIN();
        assign(float_tableau_value, *j);
        float_tableau_value /= float_tableau_denom;
        float_tableau_value *= float_tableau_value;
        k->second += float_tableau_value;
        WEIGHT_ADD(22);
        ++j;
        ++k;
      }
    }
  }
  // The candidates are processed backwards to get the same result in both
  // this implementation and the dense implementation below.
  for (std::vector<std::pair<dimension_type, double> >::const_reverse_iterator
       i = columns.rbegin(), i_end = columns.rend(); i != i_end; ++i) {
    const double challenger_value = sqrt(i->second);
    if (entering_index == 0 || challenger_value > current_value) {
      current_value = challenger_value;
      entering_index = i->first;
    }
  }

#else // !PPL_USE_SPARSE_MATRIX

  double challenger_numer = 0.0;
  double challenger_denom = 0.0;
  for (dimension_type j = tableau_num_columns - 1; j-- > 1; ) {
    Coefficient_traits::const_reference cost_j = working_cost.get(j);
    if (sgn(cost_j) == cost_sign) {
      WEIGHT_BEGIN();
      // We cannot compute the (exact) square root of abs(\Delta x_j).
      // The workaround is to compute the square of `cost[j]'.
      assign(challenger_numer, cost_j);
      challenger_numer = std::abs(challenger_numer);
      // Due to our integer implementation, the `1' term in the denominator
      // of the original formula has to be replaced by `squared_lcm_basis'.
      challenger_denom = 1.0;
      for (dimension_type i = tableau_num_rows; i-- > 0; ) {
        const Row& tableau_i = tableau[i];
        Coefficient_traits::const_reference tableau_ij = tableau_i[j];
        if (tableau_ij != 0) {
          PPL_ASSERT(tableau_i[base[i]] != 0);
          assign(float_tableau_value, tableau_ij);
          assign(float_tableau_denom, tableau_i[base[i]]);
          float_tableau_value /= float_tableau_denom;
          challenger_denom += float_tableau_value * float_tableau_value;
        }
      }
      double challenger_value = sqrt(challenger_denom);
      // Initialize `current_value' during the first iteration.
      // Otherwise update if the challenger wins.
      if (entering_index == 0 || challenger_value > current_value) {
        current_value = challenger_value;
        entering_index = j;
      }
      WEIGHT_ADD_MUL(10, tableau_num_rows);
    }
  }

#endif // !PPL_USE_SPARSE_MATRIX

  return entering_index;
}

PPL::dimension_type
PPL::MIP_Problem::steepest_edge_exact_entering_index() const {
  using std::swap;
  const dimension_type tableau_num_rows = tableau.num_rows();
  PPL_ASSERT(tableau_num_rows == base.size());
  // The square of the lcm of all the coefficients of variables in base.
  PPL_DIRTY_TEMP_COEFFICIENT(squared_lcm_basis);
  // The normalization factor for each coefficient in the tableau.
  std::vector<Coefficient> norm_factor(tableau_num_rows);
  {
    WEIGHT_BEGIN();
    // Compute the lcm of all the coefficients of variables in base.
    PPL_DIRTY_TEMP_COEFFICIENT(lcm_basis);
    lcm_basis = 1;
    for (dimension_type i = tableau_num_rows; i-- > 0; )
      lcm_assign(lcm_basis, lcm_basis, tableau[i].get(base[i]));
    // Compute normalization factors.
    for (dimension_type i = tableau_num_rows; i-- > 0; )
      exact_div_assign(norm_factor[i], lcm_basis, tableau[i].get(base[i]));
    // Compute the square of `lcm_basis', exploiting the fact that
    // `lcm_basis' will no longer be needed.
    lcm_basis *= lcm_basis;
    swap(squared_lcm_basis, lcm_basis);
    WEIGHT_ADD_MUL(444, tableau_num_rows);
  }

  PPL_DIRTY_TEMP_COEFFICIENT(challenger_numer);
  PPL_DIRTY_TEMP_COEFFICIENT(scalar_value);
  PPL_DIRTY_TEMP_COEFFICIENT(challenger_value);
  PPL_DIRTY_TEMP_COEFFICIENT(current_value);

  PPL_DIRTY_TEMP_COEFFICIENT(current_numer);
  PPL_DIRTY_TEMP_COEFFICIENT(current_denom);
  dimension_type entering_index = 0;
  const int cost_sign = sgn(working_cost.get(working_cost.size() - 1));

  // These two implementation work for both sparse and dense matrices.
  // However, when using sparse matrices the first one is fast and the second
  // one is slow, and when using dense matrices the first one is slow and
  // the second one is fast.
#if PPL_USE_SPARSE_MATRIX

  const dimension_type tableau_num_columns = tableau.num_columns();
  const dimension_type tableau_num_columns_minus_1 = tableau_num_columns - 1;
  // This is static to improve performance.
  // A pair (i, x) means that sgn(working_cost[i]) == cost_sign and x
  // is the denominator of the challenger, for the column i.
  static std::vector<std::pair<dimension_type, Coefficient> > columns;
  columns.clear();
  // tableau_num_columns - 2 is only an upper bound on the required elements.
  // This helps to reduce the number of calls to new [] and delete [] and
  // the construction/destruction of Coefficient objects.
  columns.reserve(tableau_num_columns - 2);
  {
    working_cost_type::const_iterator i = working_cost.lower_bound(1);
    // Note that find() is used instead of lower_bound.
    working_cost_type::const_iterator i_end
      = working_cost.find(tableau_num_columns_minus_1);
    for ( ; i != i_end; ++i)
      if (sgn(*i) == cost_sign)
        columns.push_back(std::pair<dimension_type, Coefficient>
                          (i.index(), squared_lcm_basis));
  }
  for (dimension_type i = tableau_num_rows; i-- > 0; ) {
    const Row& tableau_i = tableau[i];
    Row::const_iterator j = tableau_i.begin();
    Row::const_iterator j_end = tableau_i.end();
    std::vector<std::pair<dimension_type, Coefficient> >::iterator
      k = columns.begin();
    std::vector<std::pair<dimension_type, Coefficient> >::iterator
      k_end = columns.end();
    while (j != j_end) {
      while (k != k_end && j.index() > k->first)
        ++k;
      if (k == k_end)
        break;
      PPL_ASSERT(j.index() <= k->first);
      if (j.index() < k->first)
        j = tableau_i.lower_bound(j, k->first);
      else {
        Coefficient_traits::const_reference tableau_ij = *j;
        WEIGHT_BEGIN();
#if PPL_USE_SPARSE_MATRIX
        scalar_value = tableau_ij * norm_factor[i];
        add_mul_assign(k->second, scalar_value, scalar_value);
#else
        // The test against 0 gives rise to a consistent speed up in the dense
        // implementation: see
        // http://www.cs.unipr.it/pipermail/ppl-devel/2009-February/
        // 014000.html
        if (tableau_ij != 0) {
          scalar_value = tableau_ij * norm_factor[i];
          add_mul_assign(k->second, scalar_value, scalar_value);
        }
#endif
        WEIGHT_ADD_MUL(47, tableau_num_rows);
        ++k;
        ++j;
      }
    }
  }
  working_cost_type::const_iterator itr = working_cost.end();
  for (std::vector<std::pair<dimension_type, Coefficient> >::reverse_iterator
       k = columns.rbegin(), k_end = columns.rend(); k != k_end; ++k) {
    itr = working_cost.lower_bound(itr, k->first);
    if (itr != working_cost.end() && itr.index() == k->first) {
      // We cannot compute the (exact) square root of abs(\Delta x_j).
      // The workaround is to compute the square of `cost[j]'.
      challenger_numer = (*itr) * (*itr);
      // Initialization during the first loop.
      if (entering_index == 0) {
        swap(current_numer, challenger_numer);
        swap(current_denom, k->second);
        entering_index = k->first;
        continue;
      }
      challenger_value = challenger_numer * current_denom;
      current_value = current_numer * k->second;
      // Update the values, if the challenger wins.
      if (challenger_value > current_value) {
        swap(current_numer, challenger_numer);
        swap(current_denom, k->second);
        entering_index = k->first;
      }
    }
    else {
      PPL_ASSERT(working_cost.get(k->first) == 0);
      // Initialization during the first loop.
      if (entering_index == 0) {
        current_numer = 0;
        swap(current_denom, k->second);
        entering_index = k->first;
        continue;
      }
      // Update the values, if the challenger wins.
      if (0 > sgn(current_numer) * sgn(k->second)) {
        current_numer = 0;
        swap(current_denom, k->second);
        entering_index = k->first;
      }
    }
  }

#else // !PPL_USE_SPARSE_MATRIX

  PPL_DIRTY_TEMP_COEFFICIENT(challenger_denom);
  for (dimension_type j = tableau.num_columns() - 1; j-- > 1; ) {
    Coefficient_traits::const_reference cost_j = working_cost[j];
    if (sgn(cost_j) == cost_sign) {
      WEIGHT_BEGIN();
      // We cannot compute the (exact) square root of abs(\Delta x_j).
      // The workaround is to compute the square of `cost[j]'.
      challenger_numer = cost_j * cost_j;
      // Due to our integer implementation, the `1' term in the denominator
      // of the original formula has to be replaced by `squared_lcm_basis'.
      challenger_denom = squared_lcm_basis;
      for (dimension_type i = tableau_num_rows; i-- > 0; ) {
        Coefficient_traits::const_reference tableau_ij = tableau[i][j];
        // The test against 0 gives rise to a consistent speed up: see
        // http://www.cs.unipr.it/pipermail/ppl-devel/2009-February/
        // 014000.html
        if (tableau_ij != 0) {
          scalar_value = tableau_ij * norm_factor[i];
          add_mul_assign(challenger_denom, scalar_value, scalar_value);
        }
      }
      // Initialization during the first loop.
      if (entering_index == 0) {
        swap(current_numer, challenger_numer);
        swap(current_denom, challenger_denom);
        entering_index = j;
        continue;
      }
      challenger_value = challenger_numer * current_denom;
      current_value = current_numer * challenger_denom;
      // Update the values, if the challenger wins.
      if (challenger_value > current_value) {
        swap(current_numer, challenger_numer);
        swap(current_denom, challenger_denom);
        entering_index = j;
      }
      WEIGHT_ADD_MUL(47, tableau_num_rows);
    }
  }

#endif // !PPL_USE_SPARSE_MATRIX

  return entering_index;
}


// See page 47 of [PapadimitriouS98].
PPL::dimension_type
PPL::MIP_Problem::textbook_entering_index() const {
  // The variable entering the base is the first one whose coefficient
  // in the cost function has the same sign the cost function itself.
  // If no such variable exists, then we met the optimality condition
  // (and return 0 to the caller).

  // Get the "sign" of the cost function.
  const dimension_type cost_sign_index = working_cost.size() - 1;
  const int cost_sign = sgn(working_cost.get(cost_sign_index));
  PPL_ASSERT(cost_sign != 0);

  working_cost_type::const_iterator i = working_cost.lower_bound(1);
  // Note that find() is used instead of lower_bound() because they are
  // equivalent when searching the last element in the row.
  working_cost_type::const_iterator i_end
    = working_cost.find(cost_sign_index);
  for ( ; i != i_end; ++i)
    if (sgn(*i) == cost_sign)
      return i.index();
  // No variable has to enter the base:
  // the cost function was optimized.
  return 0;
}

void
PPL::MIP_Problem::linear_combine(Row& x, const Row& y,
                                 const dimension_type k) {
  PPL_ASSERT(x.size() == y.size());
  WEIGHT_BEGIN();
  const dimension_type x_size = x.size();
  Coefficient_traits::const_reference x_k = x.get(k);
  Coefficient_traits::const_reference y_k = y.get(k);
  PPL_ASSERT(y_k != 0 && x_k != 0);
  // Let g be the GCD between `x[k]' and `y[k]'.
  // For each i the following computes
  //   x[i] = x[i]*y[k]/g - y[i]*x[k]/g.
  PPL_DIRTY_TEMP_COEFFICIENT(normalized_x_k);
  PPL_DIRTY_TEMP_COEFFICIENT(normalized_y_k);
  normalize2(x_k, y_k, normalized_x_k, normalized_y_k);

  neg_assign(normalized_y_k);
  x.linear_combine(y, normalized_y_k, normalized_x_k);

  PPL_ASSERT(x.get(k) == 0);

#if PPL_USE_SPARSE_MATRIX
  PPL_ASSERT(x.find(k) == x.end());
#endif

  x.normalize();
  WEIGHT_ADD_MUL(31, x_size);
}

// TODO: Remove this when the sparse working cost has been tested enough.
#if PPL_USE_SPARSE_MATRIX

void
PPL::MIP_Problem::linear_combine(Dense_Row& x,
                                 const Sparse_Row& y,
                                 const dimension_type k) {
  PPL_ASSERT(x.size() == y.size());
  WEIGHT_BEGIN();
  const dimension_type x_size = x.size();
  Coefficient_traits::const_reference x_k = x.get(k);
  Coefficient_traits::const_reference y_k = y.get(k);
  PPL_ASSERT(y_k != 0 && x_k != 0);
  // Let g be the GCD between `x[k]' and `y[k]'.
  // For each i the following computes
  //   x[i] = x[i]*y[k]/g - y[i]*x[k]/g.
  PPL_DIRTY_TEMP_COEFFICIENT(normalized_x_k);
  PPL_DIRTY_TEMP_COEFFICIENT(normalized_y_k);
  normalize2(x_k, y_k, normalized_x_k, normalized_y_k);

  neg_assign(normalized_y_k);
  Parma_Polyhedra_Library::linear_combine(x, y, normalized_y_k, normalized_x_k);

  PPL_ASSERT(x[k] == 0);

  x.normalize();
  WEIGHT_ADD_MUL(83, x_size);
}

#endif // defined(PPL_USE_SPARSE_MATRIX)

bool
PPL::MIP_Problem::is_unbounded_obj_function(
  const Linear_Expression& x,
  const std::vector<std::pair<dimension_type, dimension_type> >& mapping,
  Optimization_Mode optimization_mode) {

  for (Linear_Expression::const_iterator i = x.begin(),
         i_end = x.end(); i != i_end; ++i) {
    // If a the value of a variable in the objective function is
    // different from zero, the final status is unbounded.
    // In the first part the variable is constrained to be greater or equal
    // than zero.
    if (mapping[i.variable().space_dimension()].second != 0)
      return true;
    if (optimization_mode == MAXIMIZATION) {
      if (*i > 0)
        return true;
    }
    else {
      PPL_ASSERT(optimization_mode == MINIMIZATION);
      if (*i < 0)
        return true;
    }
  }
  return false;
}

// See pages 42-43 of [PapadimitriouS98].
void
PPL::MIP_Problem::pivot(const dimension_type entering_var_index,
                        const dimension_type exiting_base_index) {
  const Row& tableau_out = tableau[exiting_base_index];
  // Linearly combine the constraints.
  for (dimension_type i = tableau.num_rows(); i-- > 0; ) {
    Row& tableau_i = tableau[i];
    if (i != exiting_base_index && tableau_i.get(entering_var_index) != 0)
      linear_combine(tableau_i, tableau_out, entering_var_index);
  }
  // Linearly combine the cost function.
  if (working_cost.get(entering_var_index) != 0)
    linear_combine(working_cost, tableau_out, entering_var_index);
  // Adjust the base.
  base[exiting_base_index] = entering_var_index;
}

// See pages 47 and 50 of [PapadimitriouS98].
PPL::dimension_type
PPL::MIP_Problem
::get_exiting_base_index(const dimension_type entering_var_index) const {
  // The variable exiting the base should be associated to a tableau
  // constraint such that the ratio
  // tableau[i][entering_var_index] / tableau[i][base[i]]
  // is strictly positive and minimal.

  // Find the first tableau constraint `c' having a positive value for
  // tableau[i][entering_var_index] / tableau[i][base[i]]
  const dimension_type tableau_num_rows = tableau.num_rows();
  dimension_type exiting_base_index = tableau_num_rows;
  for (dimension_type i = 0; i < tableau_num_rows; ++i) {
    const Row& t_i = tableau[i];
    const int num_sign = sgn(t_i.get(entering_var_index));
    if (num_sign != 0 && num_sign == sgn(t_i.get(base[i]))) {
      exiting_base_index = i;
      break;
    }
  }
  // Check for unboundedness.
  if (exiting_base_index == tableau_num_rows)
    return tableau_num_rows;

  // Reaching this point means that a variable will definitely exit the base.
  PPL_DIRTY_TEMP_COEFFICIENT(lcm);
  PPL_DIRTY_TEMP_COEFFICIENT(current_min);
  PPL_DIRTY_TEMP_COEFFICIENT(challenger);
  Coefficient t_e0 = tableau[exiting_base_index].get(0);
  Coefficient t_ee = tableau[exiting_base_index].get(entering_var_index);
  for (dimension_type i = exiting_base_index + 1; i < tableau_num_rows; ++i) {
    const Row& t_i = tableau[i];
    Coefficient_traits::const_reference t_ie = t_i.get(entering_var_index);
    const int t_ie_sign = sgn(t_ie);
    if (t_ie_sign != 0 && t_ie_sign == sgn(t_i.get(base[i]))) {
      WEIGHT_BEGIN();
      Coefficient_traits::const_reference t_i0 = t_i.get(0);
      lcm_assign(lcm, t_ee, t_ie);
      exact_div_assign(current_min, lcm, t_ee);
      current_min *= t_e0;
      abs_assign(current_min);
      exact_div_assign(challenger, lcm, t_ie);
      challenger *= t_i0;
      abs_assign(challenger);
      current_min -= challenger;
      const int sign = sgn(current_min);
      if (sign > 0
          || (sign == 0 && base[i] < base[exiting_base_index])) {
        exiting_base_index = i;
        t_e0 = t_i0;
        t_ee = t_ie;
      }
      WEIGHT_ADD(642);
    }
  }
  return exiting_base_index;
}

// See page 49 of [PapadimitriouS98].
bool
PPL::MIP_Problem::compute_simplex_using_steepest_edge_float() {
  // We may need to temporarily switch to the textbook pricing.
  const unsigned long allowed_non_increasing_loops = 200;
  unsigned long non_increased_times = 0;
  bool textbook_pricing = false;

  PPL_DIRTY_TEMP_COEFFICIENT(cost_sgn_coeff);
  PPL_DIRTY_TEMP_COEFFICIENT(current_numer);
  PPL_DIRTY_TEMP_COEFFICIENT(current_denom);
  PPL_DIRTY_TEMP_COEFFICIENT(challenger);
  PPL_DIRTY_TEMP_COEFFICIENT(current);

  cost_sgn_coeff = working_cost.get(working_cost.size() - 1);
  current_numer = working_cost.get(0);
  if (cost_sgn_coeff < 0)
    neg_assign(current_numer);
  abs_assign(current_denom, cost_sgn_coeff);
  PPL_ASSERT(tableau.num_columns() == working_cost.size());
  const dimension_type tableau_num_rows = tableau.num_rows();

  while (true) {
    // Choose the index of the variable entering the base, if any.
    const dimension_type entering_var_index
      = textbook_pricing
      ? textbook_entering_index()
      : steepest_edge_float_entering_index();

    // If no entering index was computed, the problem is solved.
    if (entering_var_index == 0)
      return true;

    // Choose the index of the row exiting the base.
    const dimension_type exiting_base_index
      = get_exiting_base_index(entering_var_index);
    // If no exiting index was computed, the problem is unbounded.
    if (exiting_base_index == tableau_num_rows)
      return false;

    // Check if the client has requested abandoning all expensive
    // computations. If so, the exception specified by the client
    // is thrown now.
    maybe_abandon();

    // We have not reached the optimality or unbounded condition:
    // compute the new base and the corresponding vertex of the
    // feasible region.
    pivot(entering_var_index, exiting_base_index);

    WEIGHT_BEGIN();
    // Now begins the objective function's value check to choose between
    // the `textbook' and the float `steepest-edge' technique.
    cost_sgn_coeff = working_cost.get(working_cost.size() - 1);

    challenger = working_cost.get(0);
    if (cost_sgn_coeff < 0)
      neg_assign(challenger);
    challenger *= current_denom;
    abs_assign(current, cost_sgn_coeff);
    current *= current_numer;
#if PPL_NOISY_SIMPLEX
    ++num_iterations;
    if (num_iterations % 200 == 0)
      std::cout << "Primal simplex: iteration " << num_iterations
                << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
    // If the following condition fails, probably there's a bug.
    PPL_ASSERT(challenger >= current);
    // If the value of the objective function does not improve,
    // keep track of that.
    if (challenger == current) {
      ++non_increased_times;
      // In the following case we will proceed using the `textbook'
      // technique, until the objective function is not improved.
      if (non_increased_times > allowed_non_increasing_loops)
        textbook_pricing = true;
    }
    // The objective function has an improvement:
    // reset `non_increased_times' and `textbook_pricing'.
    else {
      non_increased_times = 0;
      textbook_pricing = false;
    }
    current_numer = working_cost.get(0);
    if (cost_sgn_coeff < 0)
      neg_assign(current_numer);
    abs_assign(current_denom, cost_sgn_coeff);
    WEIGHT_ADD(433);
  }
}

bool
PPL::MIP_Problem::compute_simplex_using_exact_pricing() {
  PPL_ASSERT(tableau.num_columns() == working_cost.size());
  PPL_ASSERT(get_control_parameter(PRICING) == PRICING_STEEPEST_EDGE_EXACT
         || get_control_parameter(PRICING) == PRICING_TEXTBOOK);

  const dimension_type tableau_num_rows = tableau.num_rows();
  const bool textbook_pricing
    = (PRICING_TEXTBOOK == get_control_parameter(PRICING));

  while (true) {
    // Choose the index of the variable entering the base, if any.
    const dimension_type entering_var_index
      = textbook_pricing
      ? textbook_entering_index()
      : steepest_edge_exact_entering_index();
    // If no entering index was computed, the problem is solved.
    if (entering_var_index == 0)
      return true;

    // Choose the index of the row exiting the base.
    const dimension_type exiting_base_index
      = get_exiting_base_index(entering_var_index);
    // If no exiting index was computed, the problem is unbounded.
    if (exiting_base_index == tableau_num_rows)
      return false;

    // Check if the client has requested abandoning all expensive
    // computations. If so, the exception specified by the client
    // is thrown now.
    maybe_abandon();

    // We have not reached the optimality or unbounded condition:
    // compute the new base and the corresponding vertex of the
    // feasible region.
    pivot(entering_var_index, exiting_base_index);
#if PPL_NOISY_SIMPLEX
    ++num_iterations;
    if (num_iterations % 200 == 0)
      std::cout << "Primal simplex: iteration " << num_iterations
                << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
  }
}


// See pages 55-56 of [PapadimitriouS98].
void
PPL::MIP_Problem::erase_artificials(const dimension_type begin_artificials,
                                    const dimension_type end_artificials) {
  PPL_ASSERT(0 < begin_artificials && begin_artificials < end_artificials);

  const dimension_type old_last_column = tableau.num_columns() - 1;
  dimension_type tableau_n_rows = tableau.num_rows();
  // Step 1: try to remove from the base all the remaining slack variables.
  for (dimension_type i = 0; i < tableau_n_rows; ++i)
    if (begin_artificials <= base[i] && base[i] < end_artificials) {
      // Search for a non-zero element to enter the base.
      Row& tableau_i = tableau[i];
      bool redundant = true;
      Row::const_iterator j = tableau_i.begin();
      Row::const_iterator j_end = tableau_i.end();
      // Skip the first element
      if (j != j_end && j.index() == 0)
        ++j;
      for ( ; (j != j_end) && (j.index() < begin_artificials); ++j)
        if (*j != 0) {
          pivot(j.index(), i);
          redundant = false;
          break;
        }
      if (redundant) {
        // No original variable entered the base:
        // the constraint is redundant and should be deleted.
        --tableau_n_rows;
        if (i < tableau_n_rows) {
          // Replace the redundant row with the last one,
          // taking care of adjusting the iteration index.
          tableau_i.m_swap(tableau[tableau_n_rows]);
          base[i] = base[tableau_n_rows];
          --i;
        }
        tableau.remove_trailing_rows(1);
        base.pop_back();
      }
    }

  // Step 2: Adjust data structures so as to enter phase 2 of the simplex.

  // Resize the tableau.
  const dimension_type num_artificials = end_artificials - begin_artificials;
  tableau.remove_trailing_columns(num_artificials);

  // Zero the last column of the tableau.
  const dimension_type new_last_column = tableau.num_columns() - 1;
  for (dimension_type i = tableau_n_rows; i-- > 0; )
    tableau[i].reset(new_last_column);

  // ... then properly set the element in the (new) last column,
  // encoding the kind of optimization; ...
  {
    // This block is equivalent to
    //
    // <CODE>
    //   working_cost[new_last_column] = working_cost.get(old_last_column);
    // </CODE>
    //
    // but it avoids storing zeroes.
    Coefficient_traits::const_reference old_cost
      = working_cost.get(old_last_column);
    if (old_cost == 0)
      working_cost.reset(new_last_column);
    else
      working_cost.insert(new_last_column, old_cost);
  }

  // ... and finally remove redundant columns.
  const dimension_type working_cost_new_size
    = working_cost.size() - num_artificials;
  working_cost.shrink(working_cost_new_size);
}

// See page 55 of [PapadimitriouS98].
void
PPL::MIP_Problem::compute_generator() const {
  // Early exit for 0-dimensional problems.
  if (external_space_dim == 0) {
    MIP_Problem& x = const_cast<MIP_Problem&>(*this);
    x.last_generator = point();
    return;
  }

  // We will store in numer[] and in denom[] the numerators and
  // the denominators of every variable of the original problem.
  std::vector<Coefficient> numer(external_space_dim);
  std::vector<Coefficient> denom(external_space_dim);
  dimension_type row = 0;

  PPL_DIRTY_TEMP_COEFFICIENT(lcm);
  // Speculatively allocate temporaries out of loop.
  PPL_DIRTY_TEMP_COEFFICIENT(split_numer);
  PPL_DIRTY_TEMP_COEFFICIENT(split_denom);

  // We start to compute numer[] and denom[].
  for (dimension_type i = external_space_dim; i-- > 0; ) {
    Coefficient& numer_i = numer[i];
    Coefficient& denom_i = denom[i];
    // Get the value of the variable from the tableau
    // (if it is not a basic variable, the value is 0).
    const dimension_type original_var = mapping[i+1].first;
    if (is_in_base(original_var, row)) {
      const Row& t_row = tableau[row];
      Coefficient_traits::const_reference t_row_original_var
        = t_row.get(original_var);
      if (t_row_original_var > 0) {
        neg_assign(numer_i, t_row.get(0));
        denom_i = t_row_original_var;
      }
      else {
        numer_i = t_row.get(0);
        neg_assign(denom_i, t_row_original_var);
      }
    }
    else {
      numer_i = 0;
      denom_i = 1;
    }
    // Check whether the variable was split.
    const dimension_type split_var = mapping[i+1].second;
    if (split_var != 0) {
      // The variable was split: get the value for the negative component,
      // having index mapping[i+1].second .
      // Like before, we he have to check if the variable is in base.
      if (is_in_base(split_var, row)) {
        const Row& t_row = tableau[row];
        Coefficient_traits::const_reference t_row_split_var
          = t_row.get(split_var);
        if (t_row_split_var > 0) {
          neg_assign(split_numer, t_row.get(0));
          split_denom = t_row_split_var;
        }
        else {
          split_numer = t_row.get(0);
          neg_assign(split_denom, t_row_split_var);
        }
        // We compute the lcm to compute subsequently the difference
        // between the 2 variables.
        lcm_assign(lcm, denom_i, split_denom);
        exact_div_assign(denom_i, lcm, denom_i);
        exact_div_assign(split_denom, lcm, split_denom);
        numer_i *= denom_i;
        sub_mul_assign(numer_i, split_numer, split_denom);
        if (numer_i == 0)
          denom_i = 1;
        else
          denom_i = lcm;
      }
      // Note: if the negative component was not in base, then
      // it has value zero and there is nothing left to do.
    }
  }

  // Compute the lcm of all denominators.
  PPL_ASSERT(external_space_dim > 0);
  lcm = denom[0];
  for (dimension_type i = 1; i < external_space_dim; ++i)
    lcm_assign(lcm, lcm, denom[i]);
  // Use the denominators to store the numerators' multipliers
  // and then compute the normalized numerators.
  for (dimension_type i = external_space_dim; i-- > 0; ) {
    exact_div_assign(denom[i], lcm, denom[i]);
    numer[i] *= denom[i];
  }

  // Finally, build the generator.
  Linear_Expression expr;
  for (dimension_type i = external_space_dim; i-- > 0; )
    add_mul_assign(expr, numer[i], Variable(i));

  MIP_Problem& x = const_cast<MIP_Problem&>(*this);
  x.last_generator = point(expr, lcm);
}

void
PPL::MIP_Problem::second_phase() {
  // Second_phase requires that *this is satisfiable.
  PPL_ASSERT(status == SATISFIABLE
             || status == UNBOUNDED
             || status == OPTIMIZED);
  // In the following cases the problem is already solved.
  if (status == UNBOUNDED || status == OPTIMIZED)
    return;

  // Build the objective function for the second phase.
  Row new_cost;
  input_obj_function.get_row(new_cost);

  // Negate the cost function if we are minimizing.
  if (opt_mode == MINIMIZATION)
    for (Row::iterator i = new_cost.begin(),
           i_end = new_cost.end(); i != i_end; ++i)
      neg_assign(*i);

  const dimension_type cost_zero_size = working_cost.size();

  // Substitute properly the cost function in the `costs' matrix.
  {
    working_cost_type tmp_cost(cost_zero_size, cost_zero_size);
    swap(tmp_cost, working_cost);
  }

  {
    working_cost_type::iterator itr
      = working_cost.insert(cost_zero_size - 1, Coefficient_one());

    // Split the variables in the cost function.
    for (Row::const_iterator i = new_cost.lower_bound(1),
           i_end = new_cost.end(); i != i_end; ++i) {
      const dimension_type index = i.index();
      const dimension_type original_var = mapping[index].first;
      const dimension_type split_var = mapping[index].second;
      itr = working_cost.insert(itr, original_var, *i);
      if (mapping[index].second != 0) {
        itr = working_cost.insert(itr, split_var);
        neg_assign(*itr, *i);
      }
    }
  }

  // Here the first phase problem succeeded with optimum value zero.
  // Express the old cost function in terms of the computed base.
  {
    working_cost_type::iterator itr = working_cost.end();
    for (dimension_type i = tableau.num_rows(); i-- > 0; ) {
      const dimension_type base_i = base[i];
      itr = working_cost.lower_bound(itr, base_i);
      if (itr != working_cost.end() && itr.index() == base_i && *itr != 0) {
        linear_combine(working_cost, tableau[i], base_i);
        itr = working_cost.end();
      }
    }
  }

  // Solve the second phase problem.
  const bool second_phase_successful
    = (get_control_parameter(PRICING) == PRICING_STEEPEST_EDGE_FLOAT)
    ? compute_simplex_using_steepest_edge_float()
    : compute_simplex_using_exact_pricing();
  compute_generator();
#if PPL_NOISY_SIMPLEX
  std::cout << "MIP_Problem::second_phase(): 2nd phase ended at iteration "
            << num_iterations
            << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
  status = second_phase_successful ? OPTIMIZED : UNBOUNDED;
  PPL_ASSERT(OK());
}

void
PPL::MIP_Problem
::evaluate_objective_function(const Generator& evaluating_point,
                              Coefficient& numer,
                              Coefficient& denom) const {
  const dimension_type ep_space_dim = evaluating_point.space_dimension();
  if (space_dimension() < ep_space_dim)
    throw std::invalid_argument("PPL::MIP_Problem::"
                                "evaluate_objective_function(p, n, d):\n"
                                "*this and p are dimension incompatible.");
  if (!evaluating_point.is_point())
    throw std::invalid_argument("PPL::MIP_Problem::"
                                "evaluate_objective_function(p, n, d):\n"
                                "p is not a point.");

  // Compute the smallest space dimension  between `input_obj_function'
  // and `evaluating_point'.
  const dimension_type working_space_dim
    = std::min(ep_space_dim, input_obj_function.space_dimension());
  input_obj_function.scalar_product_assign(numer,
                                           evaluating_point.expr,
                                           0, working_space_dim + 1);

  // Numerator and denominator should be coprime.
  normalize2(numer, evaluating_point.divisor(), numer, denom);
}

bool
PPL::MIP_Problem::is_lp_satisfiable() const {
#if PPL_NOISY_SIMPLEX
  num_iterations = 0;
#endif // PPL_NOISY_SIMPLEX
  switch (status) {
  case UNSATISFIABLE:
    return false;
  case SATISFIABLE:
    // Intentionally fall through.
  case UNBOUNDED:
    // Intentionally fall through.
  case OPTIMIZED:
    // Intentionally fall through.
    return true;
  case PARTIALLY_SATISFIABLE:
    {
      MIP_Problem& x = const_cast<MIP_Problem&>(*this);
      // This code tries to handle the case that happens if the tableau is
      // empty, so it must be initialized.
      if (tableau.num_columns() == 0) {
        // Add two columns, the first that handles the inhomogeneous term and
        // the second that represent the `sign'.
        x.tableau.add_zero_columns(2);
        // Sync `mapping' for the inhomogeneous term.
        x.mapping.push_back(std::make_pair(0, 0));
        // The internal data structures are ready, so prepare for more
        // assertion to be checked.
        x.initialized = true;
      }

      // Apply incrementality to the pending constraint system.
      x.process_pending_constraints();
      // Update `first_pending_constraint': no more pending.
      x.first_pending_constraint = input_cs.size();
      // Update also `internal_space_dim'.
      x.internal_space_dim = x.external_space_dim;
      PPL_ASSERT(OK());
      return status != UNSATISFIABLE;
    }
  }
  // We should not be here!
  PPL_UNREACHABLE;
  return false;
}

PPL::MIP_Problem_Status
PPL::MIP_Problem::solve_mip(bool& have_incumbent_solution,
                            mpq_class& incumbent_solution_value,
                            Generator& incumbent_solution_point,
                            MIP_Problem& mip,
                            const Variables_Set& i_vars) {
  // Solve the problem as a non MIP one, it must be done internally.
  PPL::MIP_Problem_Status mip_status;
  if (mip.is_lp_satisfiable()) {
    mip.second_phase();
    mip_status = (mip.status == OPTIMIZED) ? OPTIMIZED_MIP_PROBLEM
      : UNBOUNDED_MIP_PROBLEM;
  }
  else
    return UNFEASIBLE_MIP_PROBLEM;

  PPL_DIRTY_TEMP(mpq_class, tmp_rational);

  Generator p = point();
  PPL_DIRTY_TEMP_COEFFICIENT(tmp_coeff1);
  PPL_DIRTY_TEMP_COEFFICIENT(tmp_coeff2);

  if (mip_status == UNBOUNDED_MIP_PROBLEM)
    p = mip.last_generator;
  else {
    PPL_ASSERT(mip_status == OPTIMIZED_MIP_PROBLEM);
    // Do not call optimizing_point().
    p = mip.last_generator;
    mip.evaluate_objective_function(p, tmp_coeff1, tmp_coeff2);
    assign_r(tmp_rational.get_num(), tmp_coeff1, ROUND_NOT_NEEDED);
    assign_r(tmp_rational.get_den(), tmp_coeff2, ROUND_NOT_NEEDED);
    PPL_ASSERT(is_canonical(tmp_rational));
    if (have_incumbent_solution
        && ((mip.optimization_mode() == MAXIMIZATION
              && tmp_rational <= incumbent_solution_value)
            || (mip.optimization_mode() == MINIMIZATION
                && tmp_rational >= incumbent_solution_value)))
      // Abandon this path.
      return mip_status;
  }

  bool found_satisfiable_generator = true;
  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  Coefficient_traits::const_reference p_divisor = p.divisor();
  dimension_type non_int_dim = mip.space_dimension();
  // TODO: This can be optimized more, exploiting the (possible)
  // sparseness of p, if the size of i_vars is expected to be greater than
  // the number of nonzeroes in p in most cases.
  for (Variables_Set::const_iterator v_begin = i_vars.begin(),
         v_end = i_vars.end(); v_begin != v_end; ++v_begin) {
    gcd_assign(gcd, p.coefficient(Variable(*v_begin)), p_divisor);
    if (gcd != p_divisor) {
      non_int_dim = *v_begin;
      found_satisfiable_generator = false;
      break;
    }
  }
  if (found_satisfiable_generator) {
    // All the coordinates of `point' are satisfiable.
    if (mip_status == UNBOUNDED_MIP_PROBLEM) {
      // This is a point that belongs to the MIP_Problem.
      // In this way we are sure that we will return every time
      // a feasible point if requested by the user.
      incumbent_solution_point = p;
      return mip_status;
    }
    if (!have_incumbent_solution
        || (mip.optimization_mode() == MAXIMIZATION
            && tmp_rational > incumbent_solution_value)
        || tmp_rational < incumbent_solution_value) {
      incumbent_solution_value = tmp_rational;
      incumbent_solution_point = p;
      have_incumbent_solution = true;
#if PPL_NOISY_SIMPLEX
      PPL_DIRTY_TEMP_COEFFICIENT(numer);
      PPL_DIRTY_TEMP_COEFFICIENT(denom);
      mip.evaluate_objective_function(p, numer, denom);
      std::cout << "MIP_Problem::solve_mip(): "
                << "new value found: " << numer << "/" << denom
                << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
    }
    return mip_status;
  }

  PPL_ASSERT(non_int_dim < mip.space_dimension());

  assign_r(tmp_rational.get_num(), p.coefficient(Variable(non_int_dim)),
           ROUND_NOT_NEEDED);
  assign_r(tmp_rational.get_den(), p_divisor, ROUND_NOT_NEEDED);
  tmp_rational.canonicalize();
  assign_r(tmp_coeff1, tmp_rational, ROUND_DOWN);
  assign_r(tmp_coeff2, tmp_rational, ROUND_UP);
  {
    MIP_Problem mip_aux(mip, Inherit_Constraints());
    mip_aux.add_constraint(Variable(non_int_dim) <= tmp_coeff1);
#if PPL_NOISY_SIMPLEX
    using namespace IO_Operators;
    std::cout << "MIP_Problem::solve_mip(): "
              << "descending with: "
              << (Variable(non_int_dim) <= tmp_coeff1)
              << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
    solve_mip(have_incumbent_solution, incumbent_solution_value,
              incumbent_solution_point, mip_aux, i_vars);
  }
  // TODO: change this when we will be able to remove constraints.
  mip.add_constraint(Variable(non_int_dim) >= tmp_coeff2);
#if PPL_NOISY_SIMPLEX
  using namespace IO_Operators;
  std::cout << "MIP_Problem::solve_mip(): "
            << "descending with: "
            << (Variable(non_int_dim) >= tmp_coeff2)
            << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
  solve_mip(have_incumbent_solution, incumbent_solution_value,
            incumbent_solution_point, mip, i_vars);
  return have_incumbent_solution ? mip_status : UNFEASIBLE_MIP_PROBLEM;
}

bool
PPL::MIP_Problem::choose_branching_variable(const MIP_Problem& mip,
                                            const Variables_Set& i_vars,
                                            dimension_type& branching_index) {
  // Insert here the variables that do not satisfy the integrality
  // condition.
  const std::vector<Constraint*>& input_cs = mip.input_cs;
  const Generator& last_generator = mip.last_generator;
  Coefficient_traits::const_reference last_generator_divisor
    = last_generator.divisor();
  Variables_Set candidate_variables;

  // TODO: This can be optimized more, exploiting the (possible)
  // sparseness of last_generator, if the size of i_vars is expected to be
  // greater than the number of nonzeroes in last_generator in most cases.
  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  for (Variables_Set::const_iterator v_it = i_vars.begin(),
         v_end = i_vars.end(); v_it != v_end; ++v_it) {
    gcd_assign(gcd,
               last_generator.coefficient(Variable(*v_it)),
               last_generator_divisor);
    if (gcd != last_generator_divisor)
      candidate_variables.insert(*v_it);
  }
  // If this set is empty, we have finished.
  if (candidate_variables.empty())
    return true;

  // Check how many `active constraints' we have and track them.
  const dimension_type input_cs_num_rows = input_cs.size();
  std::deque<bool> satisfiable_constraints (input_cs_num_rows, false);
  for (dimension_type i = input_cs_num_rows; i-- > 0; )
    // An equality is an `active constraint' by definition.
    // If we have an inequality, check if it is an `active constraint'.
    if (input_cs[i]->is_equality()
        || is_saturated(*(input_cs[i]), last_generator))
      satisfiable_constraints[i] = true;

  dimension_type winning_num_appearances = 0;

  std::vector<dimension_type>
    num_appearances(candidate_variables.space_dimension(), 0);

  // For every candidate variable, check how many times this appear in the
  // active constraints.
  for (dimension_type i = input_cs_num_rows; i-- > 0; ) {
    if (!satisfiable_constraints[i])
      continue;
    // TODO: This can be optimized more, exploiting the (possible)
    // sparseness of input_cs, if the size of candidate_variables is expected
    // to be greater than the number of nonzeroes of most rows.
    for (Variables_Set::const_iterator v_it = candidate_variables.begin(),
            v_end = candidate_variables.end(); v_it != v_end; ++v_it) {
      if (*v_it >= input_cs[i]->space_dimension())
        break;
      if (input_cs[i]->coefficient(Variable(*v_it)) != 0)
        ++num_appearances[*v_it];
    }
  }
  for (Variables_Set::const_iterator v_it = candidate_variables.begin(),
         v_end = candidate_variables.end(); v_it != v_end; ++v_it) {
    const dimension_type n = num_appearances[*v_it];
    if (n >= winning_num_appearances) {
      winning_num_appearances = n;
      branching_index = *v_it;
    }
  }
  return false;
}

bool
PPL::MIP_Problem::is_mip_satisfiable(MIP_Problem& mip,
                                     const Variables_Set& i_vars,
                                     Generator& p) {
#if PPL_NOISY_SIMPLEX
  ++mip_recursion_level;
  std::cout << "MIP_Problem::is_mip_satisfiable(): "
            << "entering recursion level " << mip_recursion_level
            << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
  PPL_ASSERT(mip.integer_space_dimensions().empty());

  // Solve the MIP problem.
  if (!mip.is_lp_satisfiable()) {
#if PPL_NOISY_SIMPLEX
    std::cout << "MIP_Problem::is_mip_satisfiable(): "
              << "exiting from recursion level " << mip_recursion_level
              << "." << std::endl;
    --mip_recursion_level;
#endif // PPL_NOISY_SIMPLEX
    return false;
  }

  PPL_DIRTY_TEMP(mpq_class, tmp_rational);
  PPL_DIRTY_TEMP_COEFFICIENT(tmp_coeff1);
  PPL_DIRTY_TEMP_COEFFICIENT(tmp_coeff2);
  bool found_satisfiable_generator = true;
  dimension_type non_int_dim;
  p = mip.last_generator;
  Coefficient_traits::const_reference p_divisor = p.divisor();

#if PPL_SIMPLEX_USE_MIP_HEURISTIC
  found_satisfiable_generator
    = choose_branching_variable(mip, i_vars, non_int_dim);
#else
  PPL_DIRTY_TEMP_COEFFICIENT(gcd);
  // TODO: This can be optimized more, exploiting the (possible)
  // sparseness of p, if the size of i_vars is expected to be greater than
  // the number of nonzeroes in p in most cases.
  for (Variables_Set::const_iterator v_begin = i_vars.begin(),
         v_end = i_vars.end(); v_begin != v_end; ++v_begin) {
    gcd_assign(gcd, p.coefficient(Variable(*v_begin)), p_divisor);
    if (gcd != p_divisor) {
      non_int_dim = *v_begin;
      found_satisfiable_generator = false;
      break;
    }
  }
#endif

  if (found_satisfiable_generator)
    return true;

  PPL_ASSERT(non_int_dim < mip.space_dimension());

  assign_r(tmp_rational.get_num(), p.coefficient(Variable(non_int_dim)),
           ROUND_NOT_NEEDED);
  assign_r(tmp_rational.get_den(), p_divisor, ROUND_NOT_NEEDED);
  tmp_rational.canonicalize();
  assign_r(tmp_coeff1, tmp_rational, ROUND_DOWN);
  assign_r(tmp_coeff2, tmp_rational, ROUND_UP);
  {
    MIP_Problem mip_aux(mip, Inherit_Constraints());
    mip_aux.add_constraint(Variable(non_int_dim) <= tmp_coeff1);
#if PPL_NOISY_SIMPLEX
    using namespace IO_Operators;
    std::cout << "MIP_Problem::is_mip_satisfiable(): "
              << "descending with: "
              << (Variable(non_int_dim) <= tmp_coeff1)
              << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
    if (is_mip_satisfiable(mip_aux, i_vars, p)) {
#if PPL_NOISY_SIMPLEX
      std::cout << "MIP_Problem::is_mip_satisfiable(): "
                << "exiting from recursion level " << mip_recursion_level
                << "." << std::endl;
      --mip_recursion_level;
#endif // PPL_NOISY_SIMPLEX
      return true;
    }
  }
  mip.add_constraint(Variable(non_int_dim) >= tmp_coeff2);
#if PPL_NOISY_SIMPLEX
  using namespace IO_Operators;
  std::cout << "MIP_Problem::is_mip_satisfiable(): "
            << "descending with: "
            << (Variable(non_int_dim) >= tmp_coeff2)
            << "." << std::endl;
#endif // PPL_NOISY_SIMPLEX
  const bool satisfiable = is_mip_satisfiable(mip, i_vars, p);
#if PPL_NOISY_SIMPLEX
  std::cout << "MIP_Problem::is_mip_satisfiable(): "
            << "exiting from recursion level " << mip_recursion_level
            << "." << std::endl;
  --mip_recursion_level;
#endif // PPL_NOISY_SIMPLEX
  return satisfiable;
}

bool
PPL::MIP_Problem::OK() const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif
  const dimension_type input_cs_num_rows = input_cs.size();

  // Check that every member used is OK.

  if (inherited_constraints > input_cs_num_rows) {
#ifndef NDEBUG
    cerr << "The MIP_Problem claims to have inherited from its ancestors "
         << "more constraints than are recorded in this->input_cs."
         << endl;
    ascii_dump(cerr);
#endif
    return false;
  }

  if (first_pending_constraint > input_cs_num_rows) {
#ifndef NDEBUG
    cerr << "The MIP_Problem claims to have pending constraints "
         << "that are not recorded in this->input_cs."
         << endl;
    ascii_dump(cerr);
#endif
    return false;
  }

  if (!tableau.OK() || !last_generator.OK())
    return false;

  // Constraint system should contain no strict inequalities.
  for (dimension_type i = input_cs_num_rows; i-- > 0; )
    if (input_cs[i]->is_strict_inequality()) {
#ifndef NDEBUG
      cerr << "The feasible region of the MIP_Problem is defined by "
           << "a constraint system containing strict inequalities."
           << endl;
      ascii_dump(cerr);
#endif
      return false;
    }

  if (external_space_dim < internal_space_dim) {
#ifndef NDEBUG
    cerr << "The MIP_Problem claims to have an internal space dimension "
         << "greater than its external space dimension."
         << endl;
    ascii_dump(cerr);
#endif
    return false;
  }

  if (external_space_dim > internal_space_dim
      && status != UNSATISFIABLE
      && status != PARTIALLY_SATISFIABLE) {
#ifndef NDEBUG
    cerr << "The MIP_Problem claims to have a pending space dimension "
         << "addition, but the status is incompatible."
         << endl;
    ascii_dump(cerr);
#endif
    return false;
  }

  // Constraint system and objective function should be dimension compatible.
  if (external_space_dim < input_obj_function.space_dimension()) {
#ifndef NDEBUG
    cerr << "The MIP_Problem and the objective function have "
         << "incompatible space dimensions ("
         << external_space_dim << " < "
         << input_obj_function.space_dimension() << ")."
         << endl;
    ascii_dump(cerr);
#endif
    return false;
  }

  if (status != UNSATISFIABLE && initialized) {
    // Here `last_generator' has to be meaningful.
    // Check for dimension compatibility and actual feasibility.
    if (internal_space_dim != last_generator.space_dimension()) {
#ifndef NDEBUG
      cerr << "The MIP_Problem and the cached feasible point have "
           << "incompatible space dimensions ("
           << internal_space_dim << " != "
           << last_generator.space_dimension() << ")."
           << endl;
      ascii_dump(cerr);
#endif
      return false;
    }

    for (dimension_type i = 0; i < first_pending_constraint; ++i)
      if (!is_satisfied(*(input_cs[i]), last_generator)) {
#ifndef NDEBUG
        cerr << "The cached feasible point does not belong to "
             << "the feasible region of the MIP_Problem."
             << endl;
        ascii_dump(cerr);
#endif
        return false;
      }

    // Check that every integer declared variable is really integer.
    // in the solution found.
    if (!i_variables.empty()) {
      PPL_DIRTY_TEMP_COEFFICIENT(gcd);
      // TODO: This can be optimized more, exploiting the (possible)
      // sparseness of last_generator, if the size of i_variables is expected
      // to be greater than the number of nonzeroes in last_generator in most
      // cases.
      for (Variables_Set::const_iterator v_it = i_variables.begin(),
             v_end = i_variables.end(); v_it != v_end; ++v_it) {
        gcd_assign(gcd, last_generator.coefficient(Variable(*v_it)),
                   last_generator.divisor());
        if (gcd != last_generator.divisor())
          return false;
      }
    }

    const dimension_type tableau_num_rows = tableau.num_rows();
    const dimension_type tableau_num_columns = tableau.num_columns();

    // The number of rows in the tableau and base should be equal.
    if (tableau_num_rows != base.size()) {
#ifndef NDEBUG
      cerr << "tableau and base have incompatible sizes" << endl;
      ascii_dump(cerr);
#endif
      return false;
    }
    // The size of `mapping' should be equal to the internal
    // space dimension plus one.
    if (mapping.size() != internal_space_dim + 1) {
#ifndef NDEBUG
      cerr << "The internal space dimension and `mapping' "
           << "have incompatible sizes" << endl;
      ascii_dump(cerr);
#endif
      return false;
    }

    // The number of columns in the tableau and working_cost should be equal.
    if (tableau_num_columns != working_cost.size()) {
#ifndef NDEBUG
      cerr << "tableau and working_cost have incompatible sizes" << endl;
      ascii_dump(cerr);
#endif
      return false;
    }

    // The vector base should contain indices of tableau's columns.
    for (dimension_type i = base.size(); i-- > 0; ) {
      if (base[i] > tableau_num_columns) {
#ifndef NDEBUG
        cerr << "base contains an invalid column index" << endl;
        ascii_dump(cerr);
#endif
        return false;
      }
    }
    {
      // Needed to sort accesses to tableau_j, improving performance.
      typedef std::vector<std::pair<dimension_type, dimension_type> >
        pair_vector_t;
      pair_vector_t vars_in_base;
      for (dimension_type i = base.size(); i-- > 0; )
        vars_in_base.push_back(std::make_pair(base[i], i));

      std::sort(vars_in_base.begin(), vars_in_base.end());

      for (dimension_type j = tableau_num_rows; j-- > 0; ) {
        const Row& tableau_j = tableau[j];
        pair_vector_t::iterator i = vars_in_base.begin();
        pair_vector_t::iterator i_end = vars_in_base.end();
        Row::const_iterator itr = tableau_j.begin();
        Row::const_iterator itr_end = tableau_j.end();
        for ( ; i != i_end && itr != itr_end; ++i) {
          // tableau[i][base[j]], with i different from j, must be zero.
          if (itr.index() < i->first)
            itr = tableau_j.lower_bound(itr, itr.index());
          if (i->second != j && itr.index() == i->first && *itr != 0) {
#ifndef NDEBUG
            cerr << "tableau[i][base[j]], with i different from j, must be "
                 << "zero" << endl;
            ascii_dump(cerr);
#endif
            return false;
          }
        }
      }
    }
    // tableau[i][base[i]] must not be a zero.
    for (dimension_type i = base.size(); i-- > 0; ) {
      if (tableau[i].get(base[i]) == 0) {
#ifndef NDEBUG
        cerr << "tableau[i][base[i]] must not be a zero" << endl;
        ascii_dump(cerr);
#endif
        return false;
      }
    }

    // The last column of the tableau must contain only zeroes.
    for (dimension_type i = tableau_num_rows; i-- > 0; )
      if (tableau[i].get(tableau_num_columns - 1) != 0) {
#ifndef NDEBUG
        cerr << "the last column of the tableau must contain only"
          "zeroes"<< endl;
        ascii_dump(cerr);
#endif
        return false;
      }
  }

  // All checks passed.
  return true;
}

void
PPL::MIP_Problem::ascii_dump(std::ostream& s) const {
  using namespace IO_Operators;
  s << "\nexternal_space_dim: " << external_space_dim << " \n";
  s << "\ninternal_space_dim: " << internal_space_dim << " \n";

  const dimension_type input_cs_size = input_cs.size();

  s << "\ninput_cs( " << input_cs_size << " )\n";
  for (dimension_type i = 0; i < input_cs_size; ++i)
    input_cs[i]->ascii_dump(s);

  s << "\ninherited_constraints: " <<  inherited_constraints
    << std::endl;

  s << "\nfirst_pending_constraint: " <<  first_pending_constraint
    << std::endl;

  s << "\ninput_obj_function\n";
  input_obj_function.ascii_dump(s);
  s << "\nopt_mode "
    << ((opt_mode == MAXIMIZATION) ? "MAXIMIZATION" : "MINIMIZATION") << "\n";

  s << "\ninitialized: " << (initialized ? "YES" : "NO") << "\n";
  s << "\npricing: ";
  switch (pricing) {
  case PRICING_STEEPEST_EDGE_FLOAT:
    s << "PRICING_STEEPEST_EDGE_FLOAT";
    break;
  case PRICING_STEEPEST_EDGE_EXACT:
    s << "PRICING_STEEPEST_EDGE_EXACT";
    break;
  case PRICING_TEXTBOOK:
    s << "PRICING_TEXTBOOK";
    break;
  }
  s << "\n";

  s << "\nstatus: ";
  switch (status) {
  case UNSATISFIABLE:
    s << "UNSATISFIABLE";
    break;
  case SATISFIABLE:
    s << "SATISFIABLE";
    break;
  case UNBOUNDED:
    s << "UNBOUNDED";
    break;
  case OPTIMIZED:
    s << "OPTIMIZED";
    break;
  case PARTIALLY_SATISFIABLE:
    s << "PARTIALLY_SATISFIABLE";
    break;
  }
  s << "\n";

  s << "\ntableau\n";
  tableau.ascii_dump(s);
  s << "\nworking_cost( " << working_cost.size()<< " )\n";
  working_cost.ascii_dump(s);

  const dimension_type base_size = base.size();
  s << "\nbase( " << base_size << " )\n";
  for (dimension_type i = 0; i != base_size; ++i)
    s << base[i] << ' ';

  s << "\nlast_generator\n";
  last_generator.ascii_dump(s);

  const dimension_type mapping_size = mapping.size();
  s << "\nmapping( " << mapping_size << " )\n";
  for (dimension_type i = 1; i < mapping_size; ++i)
    s << "\n"<< i << " -> " << mapping[i].first << " -> " << mapping[i].second
      << ' ';

  s << "\n\ninteger_variables";
  i_variables.ascii_dump(s);
}

PPL_OUTPUT_DEFINITIONS(MIP_Problem)

bool
PPL::MIP_Problem::ascii_load(std::istream& s) {
  std::string str;
  if (!(s >> str) || str != "external_space_dim:")
    return false;

  if (!(s >> external_space_dim))
    return false;

  if (!(s >> str) || str != "internal_space_dim:")
    return false;

  if (!(s >> internal_space_dim))
    return false;

  if (!(s >> str) || str != "input_cs(")
    return false;

  dimension_type input_cs_size;

  if (!(s >> input_cs_size))
    return false;

  if (!(s >> str) || str != ")")
    return false;

  Constraint c(Constraint::zero_dim_positivity());
  input_cs.reserve(input_cs_size);
  for (dimension_type i = 0; i < input_cs_size; ++i) {
    if (!c.ascii_load(s))
      return false;
    add_constraint_helper(c);
  }

  if (!(s >> str) || str != "inherited_constraints:")
    return false;

  if (!(s >> inherited_constraints))
    return false;
  // NOTE: we loaded the number of inherited constraints, but we nonetheless
  // reset to zero the corresponding data member, since we do not support
  // constraint inheritance via ascii_load.
  inherited_constraints = 0;

  if (!(s >> str) || str != "first_pending_constraint:")
    return false;

  if (!(s >> first_pending_constraint))
    return false;

  if (!(s >> str) || str != "input_obj_function")
    return false;

  if (!input_obj_function.ascii_load(s))
    return false;

  if (!(s >> str) || str != "opt_mode")
    return false;

  if (!(s >> str))
    return false;

  if (str == "MAXIMIZATION")
    set_optimization_mode(MAXIMIZATION);
  else {
    if (str != "MINIMIZATION")
      return false;
    set_optimization_mode(MINIMIZATION);
  }

  if (!(s >> str) || str != "initialized:")
    return false;
  if (!(s >> str))
    return false;
  if (str == "YES")
    initialized = true;
  else if (str == "NO")
    initialized = false;
  else
    return false;

  if (!(s >> str) || str != "pricing:")
    return false;
  if (!(s >> str))
    return false;
  if (str == "PRICING_STEEPEST_EDGE_FLOAT")
    pricing = PRICING_STEEPEST_EDGE_FLOAT;
  else if (str == "PRICING_STEEPEST_EDGE_EXACT")
    pricing = PRICING_STEEPEST_EDGE_EXACT;
  else if (str == "PRICING_TEXTBOOK")
    pricing = PRICING_TEXTBOOK;
  else
    return false;

  if (!(s >> str) || str != "status:")
    return false;

  if (!(s >> str))
    return false;

  if (str == "UNSATISFIABLE")
    status = UNSATISFIABLE;
  else if (str == "SATISFIABLE")
    status = SATISFIABLE;
  else if (str == "UNBOUNDED")
    status = UNBOUNDED;
  else if (str == "OPTIMIZED")
    status = OPTIMIZED;
  else if (str == "PARTIALLY_SATISFIABLE")
    status = PARTIALLY_SATISFIABLE;
  else
    return false;

  if (!(s >> str) || str != "tableau")
    return false;

  if (!tableau.ascii_load(s))
    return false;

  if (!(s >> str) || str != "working_cost(")
    return false;

  dimension_type working_cost_dim;

  if (!(s >> working_cost_dim))
    return false;

  if (!(s >> str) || str != ")")
    return false;

  if (!working_cost.ascii_load(s))
    return false;

  if (!(s >> str) || str != "base(")
    return false;

  dimension_type base_size;
  if (!(s >> base_size))
    return false;

  if (!(s >> str) || str != ")")
    return false;

  for (dimension_type i = 0; i != base_size; ++i) {
    dimension_type base_value;
    if (!(s >> base_value))
      return false;
    base.push_back(base_value);
  }

  if (!(s >> str) || str != "last_generator")
    return false;

  if (!last_generator.ascii_load(s))
    return false;

  if (!(s >> str) || str != "mapping(")
    return false;

  dimension_type mapping_size;
  if (!(s >> mapping_size))
    return false;

  if (!(s >> str) || str != ")")
    return false;

  // The first `mapping' index is never used, so we initialize
  // it pushing back a dummy value.
  if (tableau.num_columns() != 0)
    mapping.push_back(std::make_pair(0, 0));

  for (dimension_type i = 1; i < mapping_size; ++i) {
    dimension_type index;
    if (!(s >> index))
      return false;

    if (!(s >> str) || str != "->")
      return false;

    dimension_type first_value;
    if (!(s >> first_value))
      return false;

    if (!(s >> str) || str != "->")
      return false;

    dimension_type second_value;
    if (!(s >> second_value))
      return false;

    mapping.push_back(std::make_pair(first_value, second_value));
  }

  if (!(s >> str) || str != "integer_variables")
    return false;

  if (!i_variables.ascii_load(s))
    return false;

  PPL_ASSERT(OK());
  return true;
}

/*! \relates Parma_Polyhedra_Library::MIP_Problem */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s, const MIP_Problem& mip) {
  s << "Constraints:";
  for (MIP_Problem::const_iterator i = mip.constraints_begin(),
         i_end = mip.constraints_end(); i != i_end; ++i)
    s << "\n" << *i;
  s << "\nObjective function: "
    << mip.objective_function()
    << "\nOptimization mode: "
    << ((mip.optimization_mode() == MAXIMIZATION)
        ? "MAXIMIZATION"
        : "MINIMIZATION");
  s << "\nInteger variables: " << mip.integer_space_dimensions();
  return s;
}
