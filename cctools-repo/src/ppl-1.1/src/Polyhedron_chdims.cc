/* Polyhedron class implementation
   (non-inline operators that may change the dimension of the vector space).
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
#include "Variables_Set_defs.hh"
#include "assert.hh"

#define BE_LAZY 1

namespace PPL = Parma_Polyhedra_Library;

void
PPL::Polyhedron::add_space_dimensions_and_embed(dimension_type m) {
  // The space dimension of the resulting polyhedron should not
  // overflow the maximum allowed space dimension.
  check_space_dimension_overflow(m, max_space_dimension() - space_dimension(),
                                 topology(),
                                 "add_space_dimensions_and_embed(m)",
                                 "adding m new space dimensions exceeds "
                                 "the maximum allowed space dimension");

  // Adding no dimensions to any polyhedron is a no-op.
  if (m == 0)
    return;

  // Adding dimensions to an empty polyhedron is obtained by adjusting
  // `space_dim' and clearing `con_sys' (since it can contain the
  // unsatisfiable constraint system of the wrong dimension).
  if (marked_empty()) {
    space_dim += m;
    con_sys.clear();
    return;
  }

  // The case of a zero-dimensional space polyhedron.
  if (space_dim == 0) {
    // Since it is not empty, it has to be the universe polyhedron.
    PPL_ASSERT(status.test_zero_dim_univ());
    // We swap `*this' with a newly created
    // universe polyhedron of dimension `m'.
    Polyhedron ph(topology(), m, UNIVERSE);
    m_swap(ph);
    return;
  }

  // To embed an n-dimension space polyhedron in a (n+m)-dimension space,
  // we just add `m' zero-columns to the rows in the system of constraints;
  // in contrast, the system of generators needs additional rows,
  // corresponding to the vectors of the canonical basis
  // for the added dimensions. That is, for each new dimension `x[k]'
  // we add the line having that direction. This is done by invoking
  // the function add_space_dimensions() giving the system of generators
  // as the second argument.
  if (constraints_are_up_to_date())
    if (generators_are_up_to_date()) {
      // `sat_c' must be up to date for add_space_dimensions().
      if (!sat_c_is_up_to_date())
        update_sat_c();
      // Adds rows and/or columns to both matrices.
      // `add_space_dimensions' correctly handles pending constraints
      // or generators.
      add_space_dimensions(con_sys, gen_sys, sat_c, sat_g, m);
    }
    else {
      // Only constraints are up-to-date: no need to modify the generators.
      con_sys.set_space_dimension(con_sys.space_dimension() + m);
    }
  else {
    // Only generators are up-to-date: no need to modify the constraints.
    PPL_ASSERT(generators_are_up_to_date());
    gen_sys.add_universe_rows_and_space_dimensions(m);
  }
  // Update the space dimension.
  space_dim += m;

  // Note: we do not check for satisfiability, because the system of
  // constraints may be unsatisfiable.
  PPL_ASSERT_HEAVY(OK());
}

void
PPL::Polyhedron::add_space_dimensions_and_project(dimension_type m) {
  // The space dimension of the resulting polyhedron should not
  // overflow the maximum allowed space dimension.
  check_space_dimension_overflow(m, max_space_dimension() - space_dimension(),
                                 topology(),
                                 "add_space_dimensions_and_project(m)",
                                 "adding m new space dimensions exceeds "
                                 "the maximum allowed space dimension");

  // Adding no dimensions to any polyhedron is a no-op.
  if (m == 0)
    return;

  // Adding dimensions to an empty polyhedron is obtained
  // by merely adjusting `space_dim'.
  if (marked_empty()) {
    space_dim += m;
    con_sys.clear();
    return;
  }

  if (space_dim == 0) {
    PPL_ASSERT(status.test_zero_dim_univ() && gen_sys.has_no_rows());
    // The system of generators for this polyhedron has only
    // the origin as a point.
    // In an NNC polyhedron, all points have to be accompanied
    // by the corresponding closure points
    // (this time, dimensions are automatically adjusted).
    if (!is_necessarily_closed())
      gen_sys.insert(Generator::zero_dim_closure_point());
    gen_sys.insert(Generator::zero_dim_point());
    gen_sys.adjust_topology_and_space_dimension(topology(), m);
    set_generators_minimized();
    space_dim = m;
    PPL_ASSERT_HEAVY(OK());
    return;
  }

  // To project an n-dimension space polyhedron in a (n+m)-dimension space,
  // we just add to the system of generators `m' zero-columns;
  // In contrast, in the system of constraints, new rows are needed
  // in order to avoid embedding the old polyhedron in the new space.
  // Thus, for each new dimensions `x[k]', we add the constraint
  // x[k] = 0: this is done by invoking the function add_space_dimensions()
  // giving the system of constraints as the second argument.
  if (constraints_are_up_to_date())
    if (generators_are_up_to_date()) {
      // `sat_g' must be up to date for add_space_dimensions().
      if (!sat_g_is_up_to_date())
        update_sat_g();
      // Adds rows and/or columns to both matrices.
      // `add_space_dimensions()' correctly handles pending constraints
      // or generators.
      add_space_dimensions(gen_sys, con_sys, sat_g, sat_c, m);
    }
    else {
      // Only constraints are up-to-date: no need to modify the generators.
      con_sys.add_universe_rows_and_space_dimensions(m);
    }
  else {
    // Only generators are up-to-date: no need to modify the constraints.
    PPL_ASSERT(generators_are_up_to_date());
    gen_sys.set_space_dimension(gen_sys.space_dimension() + m);
  }
  // Now we update the space dimension.
  space_dim += m;

  // Note: we do not check for satisfiability, because the system of
  // constraints may be unsatisfiable.
  PPL_ASSERT_HEAVY(OK());
}

void
PPL::Polyhedron::concatenate_assign(const Polyhedron& y) {
  if (topology() != y.topology())
    throw_topology_incompatible("concatenate_assign(y)", "y", y);

  // The space dimension of the resulting polyhedron should not
  // overflow the maximum allowed space dimension.
  const dimension_type added_columns = y.space_dim;
  check_space_dimension_overflow(added_columns,
                                 max_space_dimension() - space_dimension(),
                                 topology(),
                                 "concatenate_assign(y)",
                                 "concatenation exceeds the maximum "
                                 "allowed space dimension");

  // If `*this' or `y' are empty polyhedra, it is sufficient to adjust
  // the dimension of the space.
  if (marked_empty() || y.marked_empty()) {
    space_dim += added_columns;
    set_empty();
    return;
  }

  // If `y' is a non-empty 0-dim space polyhedron, the result is `*this'.
  if (added_columns == 0)
    return;

  // If `*this' is a non-empty 0-dim space polyhedron, the result is `y'.
  if (space_dim == 0) {
    *this = y;
    return;
  }

  // TODO: this implementation is just an executable specification.
  Constraint_System cs = y.constraints();

  // The constraints of `x' (possibly with pending rows) are required.
  if (has_pending_generators())
    process_pending_generators();
  else if (!constraints_are_up_to_date())
    update_constraints();

  // The matrix for the new system of constraints is obtained
  // by leaving the old system of constraints in the upper left-hand side
  // and placing the constraints of `cs' in the lower right-hand side.
  // NOTE: here topologies agree, whereas dimensions may not agree.
  const dimension_type added_rows = cs.num_rows();

  // We already dealt with the cases of an empty or zero-dim `y' polyhedron;
  // also, `cs' contains the low-level constraints, at least.
  PPL_ASSERT(added_rows > 0 && added_columns > 0);

  con_sys.set_space_dimension(con_sys.space_dimension() + added_columns);

  if (can_have_something_pending()) {
    // Steal the constraints from `cs' and put them in `con_sys'
    // using the right displacement for coefficients.
    for (dimension_type i = 0; i < added_rows; ++i) {
      cs.sys.rows[i].shift_space_dimensions(Variable(0), space_dim);
      con_sys.insert_pending(cs.sys.rows[i], Recycle_Input());
    }
    cs.clear();

    // If `*this' can support pending constraints, then, since we have
    // resized the system of constraints, we must also add to the generator
    // system those lines corresponding to the newly added dimensions,
    // because the non-pending parts of `con_sys' and `gen_sys' must still
    // be a DD pair in minimal form.
    gen_sys.add_universe_rows_and_space_dimensions(added_columns);
    // Since we added new lines at the beginning of `x.gen_sys',
    // we also have to adjust the saturation matrix `sat_c'.
    // FIXME: if `sat_c' is not up-to-date, could not we directly update
    // `sat_g' by resizing it and shifting its columns?
    if (!sat_c_is_up_to_date()) {
      sat_c.transpose_assign(sat_g);
      set_sat_c_up_to_date();
    }
    clear_sat_g_up_to_date();
    sat_c.resize(sat_c.num_rows() + added_columns, sat_c.num_columns());
    // The old saturation rows are copied at the end of the matrix.
    // The newly introduced lines saturate all the non-pending constraints,
    // thus their saturation rows are made of zeroes.
    using std::swap;
    for (dimension_type i = sat_c.num_rows() - added_columns; i-- > 0; )
      swap(sat_c[i], sat_c[i+added_columns]);
    // Since `added_rows > 0', we now have pending constraints.
    set_constraints_pending();
  }
  else {
    // Steal the constraints from `cs' and put them in `con_sys'
    // using the right displacement for coefficients.
    for (dimension_type i = 0; i < added_rows; ++i) {
      cs.sys.rows[i].shift_space_dimensions(Variable(0), space_dim);
      con_sys.insert(cs.sys.rows[i], Recycle_Input());
    }
#if !BE_LAZY
    con_sys.sort_rows();
#endif
    clear_constraints_minimized();
    clear_generators_up_to_date();
    clear_sat_g_up_to_date();
    clear_sat_c_up_to_date();
  }
  // Update space dimension.
  space_dim += added_columns;

  // The system of constraints may be unsatisfiable,
  // thus we do not check for satisfiability.
  PPL_ASSERT_HEAVY(OK());
}

void
PPL::Polyhedron::remove_space_dimensions(const Variables_Set& vars) {
  // The removal of no dimensions from any polyhedron is a no-op.
  // Note that this case also captures the only legal removal of
  // dimensions from a polyhedron in a 0-dim space.
  if (vars.empty()) {
    PPL_ASSERT_HEAVY(OK());
    return;
  }

  // Dimension-compatibility check.
  const dimension_type min_space_dim = vars.space_dimension();
  if (space_dim < min_space_dim)
    throw_dimension_incompatible("remove_space_dimensions(vs)", min_space_dim);

  const dimension_type new_space_dim = space_dim - vars.size();

  // We need updated generators; note that keeping pending generators
  // is useless because the constraints will be dropped anyway.
  if (marked_empty()
      || (has_something_pending() && !remove_pending_to_obtain_generators())
      || (!generators_are_up_to_date() && !update_generators())) {
    // Removing dimensions from the empty polyhedron:
    // we clear `con_sys' since it could have contained the
    // unsatisfiable constraint of the wrong dimension.
    con_sys.clear();
    // Update the space dimension.
    space_dim = new_space_dim;
    PPL_ASSERT_HEAVY(OK());
    return;
  }

  // When removing _all_ dimensions from a non-empty polyhedron,
  // we obtain the zero-dimensional universe polyhedron.
  if (new_space_dim == 0) {
    set_zero_dim_univ();
    return;
  }

  gen_sys.remove_space_dimensions(vars);

  // Constraints are not up-to-date and generators are not minimized.
  clear_constraints_up_to_date();
  clear_generators_minimized();

  // Update the space dimension.
  space_dim = new_space_dim;

  PPL_ASSERT_HEAVY(OK(true));
}

void
PPL::Polyhedron::remove_higher_space_dimensions(dimension_type new_dimension) {
  // Dimension-compatibility check.
  if (new_dimension > space_dim)
    throw_dimension_incompatible("remove_higher_space_dimensions(nd)",
                                 new_dimension);

  // The removal of no dimensions from any polyhedron is a no-op.
  // Note that this case also captures the only legal removal of
  // dimensions from a polyhedron in a 0-dim space.
  if (new_dimension == space_dim) {
    PPL_ASSERT_HEAVY(OK());
    return;
  }

  // We need updated generators; note that keeping pending generators
  // is useless because constraints will be dropped anyway.
  if (marked_empty()
      || (has_something_pending() && !remove_pending_to_obtain_generators())
      || (!generators_are_up_to_date() && !update_generators())) {
    // Removing dimensions from the empty polyhedron:
    // just updates the space dimension.
    space_dim = new_dimension;
    con_sys.clear();
    PPL_ASSERT_HEAVY(OK());
    return;
  }

  if (new_dimension == 0) {
    // Removing all dimensions from a non-empty polyhedron:
    // just return the zero-dimensional universe polyhedron.
    set_zero_dim_univ();
    return;
  }

  gen_sys.set_space_dimension(new_dimension);

  // Constraints are not up-to-date and generators are not minimized.
  clear_constraints_up_to_date();
  clear_generators_minimized();

  // Update the space dimension.
  space_dim = new_dimension;

  PPL_ASSERT_HEAVY(OK(true));
}

void
PPL::Polyhedron::expand_space_dimension(Variable var, dimension_type m) {
  // `var' should be one of the dimensions of the vector space.
  if (var.space_dimension() > space_dim)
    throw_dimension_incompatible("expand_space_dimension(v, m)", "v", var);

  // The space dimension of the resulting polyhedron should not
  // overflow the maximum allowed space dimension.
  check_space_dimension_overflow(m, max_space_dimension() - space_dimension(),
                                 topology(),
                                 "expand_dimension(v, m)",
                                 "adding m new space dimensions exceeds "
                                 "the maximum allowed space dimension");

  // Nothing to do, if no dimensions must be added.
  if (m == 0)
    return;

  // Keep track of the dimension before adding the new ones.
  const dimension_type old_dim = space_dim;

  // Add the required new dimensions.
  add_space_dimensions_and_embed(m);

  const Constraint_System& cs = constraints();
  Constraint_System new_constraints(cs.topology());
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i) {
    const Constraint& c = *i;

    Coefficient_traits::const_reference coeff = c.coefficient(var);

    // If `c' does not constrain `var', skip it.
    if (coeff == 0)
      continue;

    Constraint c_template = c;
    c_template.expr.set_coefficient(var, Coefficient_zero());
    
    // Each relevant constraint results in `m' new constraints.
    for (dimension_type dst_d = old_dim; dst_d < old_dim+m; ++dst_d) {
      Constraint new_c = c_template;
      add_mul_assign(new_c.expr, coeff, Variable(dst_d));
      PPL_ASSERT(new_c.OK());
      new_constraints.insert(new_c, Recycle_Input());
    }
  }
  add_recycled_constraints(new_constraints);
  PPL_ASSERT_HEAVY(OK());
}

void
PPL::Polyhedron::fold_space_dimensions(const Variables_Set& vars,
                                       Variable dest) {
  // TODO: this implementation is _really_ an executable specification.

  // `dest' should be one of the dimensions of the polyhedron.
  if (dest.space_dimension() > space_dim)
    throw_dimension_incompatible("fold_space_dimensions(vs, v)", "v", dest);

  // The folding of no dimensions is a no-op.
  if (vars.empty())
    return;

  // All variables in `vars' should be dimensions of the polyhedron.
  if (vars.space_dimension() > space_dim)
    throw_dimension_incompatible("fold_space_dimensions(vs, v)",
                                 "vs.space_dimension()",
                                 vars.space_dimension());

  // Moreover, `dest.id()' should not occur in `vars'.
  if (vars.find(dest.id()) != vars.end())
    throw_invalid_argument("fold_space_dimensions(vs, v)",
                           "v should not occur in vs");

  // All of the affine images we are going to compute are not invertible,
  // hence we will need to compute the generators of the polyhedron.
  // Since we keep taking copies, make sure that a single conversion
  // from constraints to generators is computed.
  (void) generators();
  // Having generators, we now know if the polyhedron is empty:
  // in that case, folding is equivalent to just removing space dimensions.
  if (!marked_empty()) {
    for (Variables_Set::const_iterator i = vars.begin(),
           vs_end = vars.end(); i != vs_end; ++i) {
      Polyhedron copy = *this;
      copy.affine_image(dest, Linear_Expression(Variable(*i)));
      poly_hull_assign(copy);
    }
  }
  remove_space_dimensions(vars);
  PPL_ASSERT_HEAVY(OK());
}
