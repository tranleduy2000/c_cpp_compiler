/* BHRZ03_Certificate class implementation (non-inline member functions).
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
#include "BHRZ03_Certificate_defs.hh"
#include "Polyhedron_defs.hh"
#include "assert.hh"
#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

PPL::BHRZ03_Certificate::BHRZ03_Certificate(const Polyhedron& ph)
  : affine_dim(0), lin_space_dim(0), num_constraints(0), num_points(0),
    num_rays_null_coord(ph.space_dimension(), 0) {
  // TODO: provide a correct and reasonably efficient
  // implementation for NNC polyhedra.

  // The computation of the certificate requires both the
  // constraint and the generator systems in minimal form.
  ph.minimize();
  // It is assumed that `ph' is not an empty polyhedron.
  PPL_ASSERT(!ph.marked_empty());

  // The dimension of the polyhedron is obtained by subtracting
  // the number of equalities from the space dimension.
  // When counting constraints, for a correct reasoning, we have
  // to disregard the low-level constraints (i.e., the positivity
  // constraint and epsilon bounds).
  const dimension_type space_dim = ph.space_dimension();
  affine_dim = space_dim;
  PPL_ASSERT(num_constraints == 0);
  const Constraint_System& cs = ph.minimized_constraints();
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i) {
    ++num_constraints;
    if (i->is_equality())
      --affine_dim;
  }

  PPL_ASSERT(lin_space_dim == 0);
  PPL_ASSERT(num_points == 0);
  const Generator_System& gs = ph.minimized_generators();
  for (Generator_System::const_iterator i = gs.begin(),
         gs_end = gs.end(); i != gs_end; ++i)
    switch (i->type()) {
    case Generator::POINT:
      // Intentionally fall through.
    case Generator::CLOSURE_POINT:
      ++num_points;
      break;
    case Generator::RAY:
      // For each i such that 0 <= j < space_dim,
      // `num_rays_null_coord[j]' will be the number of rays
      // having exactly `j' coordinates equal to 0.
      ++num_rays_null_coord[i->expression().num_zeroes(1, space_dim + 1)];
      break;
    case Generator::LINE:
      // Since the generator systems is minimized, the dimension of
      // the lineality space is equal to the number of lines.
      ++lin_space_dim;
      break;
    }
  PPL_ASSERT(OK());

  // TODO: this is an inefficient workaround.
  // For NNC polyhedra, constraints might be no longer up-to-date
  // (and hence, neither minimized) due to the strong minimization
  // process applied to generators when constructing the certificate.
  // We have to reinforce the (normal) minimization of the constraint
  // system. The future, lazy implementation of the strong minimization
  // process will solve this problem.
  if (!ph.is_necessarily_closed())
    ph.minimize();
}

int
PPL::BHRZ03_Certificate::compare(const BHRZ03_Certificate& y) const {
  PPL_ASSERT(OK() && y.OK());
  if (affine_dim != y.affine_dim)
    return (affine_dim > y.affine_dim) ? 1 : -1;
  if (lin_space_dim != y.lin_space_dim)
    return (lin_space_dim > y.lin_space_dim) ? 1 : -1;
  if (num_constraints != y.num_constraints)
    return (num_constraints > y.num_constraints) ? 1 : -1;
  if (num_points != y.num_points)
    return (num_points > y.num_points) ? 1 : -1;

  const dimension_type space_dim = num_rays_null_coord.size();
  PPL_ASSERT(num_rays_null_coord.size() == y.num_rays_null_coord.size());
  // Note: iterating upwards, because we have to check first
  // the number of rays having more NON-zero coordinates.
  for (dimension_type i = 0; i < space_dim; ++i)
    if (num_rays_null_coord[i] != y.num_rays_null_coord[i])
      return (num_rays_null_coord[i] > y.num_rays_null_coord[i]) ? 1 : -1;
  // All components are equal.
  return 0;
}

int
PPL::BHRZ03_Certificate::compare(const Polyhedron& ph) const {
  PPL_ASSERT(ph.space_dimension() == num_rays_null_coord.size());

  // TODO: provide a correct and reasonably efficient
  // implementation for NNC polyhedra.

  // The computation of the certificate requires both the
  // constraint and the generator systems in minimal form.
  ph.minimize();
  // It is assumed that `ph' is a polyhedron containing the
  // polyhedron described by `*this': hence, it cannot be empty.
  PPL_ASSERT(!ph.marked_empty());

  // The dimension of the polyhedron is obtained by subtracting
  // the number of equalities from the space dimension.
  // When counting constraints, for a correct reasoning, we have
  // to disregard the low-level constraints (i.e., the positivity
  // constraint and epsilon bounds).
  const dimension_type space_dim = ph.space_dimension();
  dimension_type ph_affine_dim = space_dim;
  dimension_type ph_num_constraints = 0;
  const Constraint_System& cs = ph.minimized_constraints();
  for (Constraint_System::const_iterator i = cs.begin(),
         cs_end = cs.end(); i != cs_end; ++i) {
    ++ph_num_constraints;
    if (i->is_equality())
      --ph_affine_dim;
  }
  // TODO: this is an inefficient workaround.
  // For NNC polyhedra, constraints might be no longer up-to-date
  // (and hence, neither minimized) due to the strong minimization
  // process applied to generators when constructing the certificate.
  // We have to reinforce the (normal) minimization of the constraint
  // system. The future, lazy implementation of the strong minimization
  // process will solve this problem.
  if (!ph.is_necessarily_closed())
    ph.minimize();

  // If the dimension of `ph' is increasing, the chain is stabilizing.
  if (ph_affine_dim > affine_dim)
    return 1;

  // At this point the two polyhedra must have the same dimension.
  PPL_ASSERT(ph_affine_dim == affine_dim);

  // Speculative optimization: in order to better exploit the incrementality
  // of the comparison, we do not compute information about rays here,
  // hoping that the other components will be enough.
  dimension_type ph_lin_space_dim = 0;
  dimension_type ph_num_points = 0;
  const Generator_System& gs = ph.minimized_generators();
  for (Generator_System::const_iterator i = gs.begin(),
         gs_end = gs.end(); i != gs_end; ++i)
    switch (i->type()) {
    case Generator::POINT:
      // Intentionally fall through.
    case Generator::CLOSURE_POINT:
      ++ph_num_points;
      break;
    case Generator::RAY:
      break;
    case Generator::LINE:
      // Since the generator systems is minimized, the dimension of
      // the lineality space is equal to the number of lines.
      ++ph_lin_space_dim;
      break;
    }
  // TODO: this is an inefficient workaround.
  // For NNC polyhedra, constraints might be no longer up-to-date
  // (and hence, neither minimized) due to the strong minimization
  // process applied to generators when constructing the certificate.
  // We have to reinforce the (normal) minimization of the constraint
  // system. The future, lazy implementation of the strong minimization
  // process will solve this problem.
  if (!ph.is_necessarily_closed())
    ph.minimize();

  // If the dimension of the lineality space is increasing,
  // then the chain is stabilizing.
  if (ph_lin_space_dim > lin_space_dim)
    return 1;

  // At this point the lineality space of the two polyhedra must have
  // the same dimension.
  PPL_ASSERT(ph_lin_space_dim == lin_space_dim);

  // If the number of constraints of `ph' is decreasing, then the chain
  // is stabilizing. If it is increasing, the chain is not stabilizing.
  // If they are equal, further investigation is needed.
  if (ph_num_constraints != num_constraints)
    return (ph_num_constraints < num_constraints) ? 1 : -1;

  // If the number of points of `ph' is decreasing, then the chain
  // is stabilizing. If it is increasing, the chain is not stabilizing.
  // If they are equal, further investigation is needed.
  if (ph_num_points != num_points)
    return (ph_num_points < num_points) ? 1 : -1;

  // The speculative optimization was not worth:
  // compute information about rays.
  std::vector<dimension_type> ph_num_rays_null_coord(ph.space_dim, 0);
  for (Generator_System::const_iterator i = gs.begin(),
         gs_end = gs.end(); i != gs_end; ++i)
    if (i->is_ray())
      ++ph_num_rays_null_coord[i->expression().num_zeroes(1, space_dim + 1)];

  // Compare (lexicographically) the two vectors:
  // if ph_num_rays_null_coord < num_rays_null_coord the chain is stabilizing.
  for (dimension_type i = 0; i < space_dim; ++i)
    if (ph_num_rays_null_coord[i] != num_rays_null_coord[i])
      return (ph_num_rays_null_coord[i] < num_rays_null_coord[i]) ? 1 : -1;

  // All components are equal.
  return 0;
}

bool
PPL::BHRZ03_Certificate::OK() const {
#ifndef NDEBUG
  using std::endl;
  using std::cerr;
#endif

  // The dimension of the vector space.
  const dimension_type space_dim = num_rays_null_coord.size();

  if (affine_dim > space_dim) {
#ifndef NDEBUG
    cerr << "In the BHRZ03 certificate about a non-empty polyhedron:"
         << endl
         << "the affine dimension is greater than the space dimension!"
         << endl;
#endif
    return false;
  }

  if (lin_space_dim > affine_dim) {
#ifndef NDEBUG
    cerr << "In the BHRZ03 certificate about a non-empty polyhedron:"
         << endl
         << "the lineality space dimension is greater than "
         << "the affine dimension!"
         << endl;
#endif
    return false;
  }

  if (num_constraints < space_dim - affine_dim) {
#ifndef NDEBUG
    cerr << "In the BHRZ03 certificate about a non-empty polyhedron:"
         << endl
         << "in a vector space of dimension `n',"
         << "any polyhedron of affine dimension `k'" << endl
         << "should have `n-k' non-redundant constraints at least."
         << endl
         << "Here space_dim = " << space_dim << ", "
         << "affine_dim = " << affine_dim << ", "
         << "but num_constraints = " << num_constraints << "!"
         << endl;
#endif
    return false;
  }

  if (num_points == 0) {
#ifndef NDEBUG
    cerr << "In the BHRZ03 certificate about a non-empty polyhedron:"
         << endl
         << "the generator system has no points!"
         << endl;
#endif
    return false;
  }

  if (lin_space_dim == space_dim) {
    // This was a universe polyhedron.
    if (num_constraints > 0) {
#ifndef NDEBUG
      cerr << "In the BHRZ03 certificate about a non-empty polyhedron:"
           << endl
           << "a universe polyhedron has non-redundant constraints!"
           << endl;
#endif
      return false;
    }

    if (num_points != 1) {
#ifndef NDEBUG
      cerr << "In the BHRZ03 certificate about a non-empty polyhedron:"
           << endl
           << "a universe polyhedron has more than one non-redundant point!"
           << endl;
#endif
      return false;
    }
  }

  // All tests passed.
  return true;
}
