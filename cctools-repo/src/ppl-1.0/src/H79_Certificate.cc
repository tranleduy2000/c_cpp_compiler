/* H79_Certificate class implementation
   (non-inline member functions).
   Copyright (C) 2001-2010 Roberto Bagnara <bagnara@cs.unipr.it>
   Copyright (C) 2010-2012 BUGSENG srl (http://bugseng.com)

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
#include "H79_Certificate.defs.hh"
#include "Polyhedron.defs.hh"
#include "assert.hh"
#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

PPL::H79_Certificate::H79_Certificate(const Polyhedron& ph)
  : affine_dim(0), num_constraints(0) {
  // The affine dimension of the polyhedron is obtained by subtracting
  // the number of equalities from the space dimension.
  // When counting constraints, for a correct reasoning, we have
  // to disregard the low-level constraints (i.e., the positivity
  // constraint and epsilon bounds).
  const dimension_type space_dim = ph.space_dimension();
  affine_dim = space_dim;
  const Constraint_System& cs = ph.minimized_constraints();
  // It is assumed that `ph' is not an empty polyhedron.
  PPL_ASSERT(!ph.marked_empty());
  for (Constraint_System::const_iterator i = cs.begin(),
	 cs_end = cs.end(); i != cs_end; ++i) {
    ++num_constraints;
    if (i->is_equality())
      --affine_dim;
  }

  // TODO: this is an inefficient workaround.
  // For NNC polyhedra, generators might be no longer up-to-date
  // (and hence, neither minimized) due to the strong minimization
  // process applied to constraints when constructing the certificate.
  // We have to reinforce the (normal) minimization of the generator
  // system. The future, lazy implementation of the strong minimization
  // process will solve this problem.
  if (!ph.is_necessarily_closed())
    ph.minimize();
}

int
PPL::H79_Certificate::compare(const H79_Certificate& y) const {
  if (affine_dim != y.affine_dim)
    return (affine_dim > y.affine_dim) ? 1 : -1;
  if (num_constraints != y.num_constraints)
    return (num_constraints > y.num_constraints) ? 1 : -1;
  // All components are equal.
  return 0;
}

int
PPL::H79_Certificate::compare(const Polyhedron& ph) const {
  // The affine dimension of the polyhedron is obtained by subtracting
  // the number of equalities from the space dimension.
  // When counting constraints, for a correct reasoning, we have
  // to disregard the low-level constraints (i.e., the positivity
  // constraint and epsilon bounds).
  const dimension_type space_dim = ph.space_dimension();
  dimension_type ph_affine_dim = space_dim;
  dimension_type ph_num_constraints = 0;
  const Constraint_System& cs = ph.minimized_constraints();
  // It is assumed that `ph' is a polyhedron containing the
  // polyhedron described by `*this': hence, it cannot be empty.
  PPL_ASSERT(!ph.marked_empty());
  for (Constraint_System::const_iterator i = cs.begin(),
	 cs_end = cs.end(); i != cs_end; ++i) {
    ++ph_num_constraints;
    if (i->is_equality())
      --ph_affine_dim;
  }
  // TODO: this is an inefficient workaround.
  // For NNC polyhedra, generators might be no longer up-to-date
  // (and hence, neither minimized) due to the strong minimization
  // process applied to constraints when constructing the certificate.
  // We have to reinforce the (normal) minimization of the generator
  // system. The future, lazy implementation of the strong minimization
  // process will solve this problem.
  if (!ph.is_necessarily_closed())
    ph.minimize();

  // If the affine dimension of `ph' is increasing, the chain is stabilizing.
  if (ph_affine_dim > affine_dim)
    return 1;

  // At this point the two polyhedra must have the same affine dimension.
  PPL_ASSERT(ph_affine_dim == affine_dim);

  // If the number of constraints of `ph' is decreasing, then the chain
  // is stabilizing. If it is increasing, the chain is not stabilizing.
  if (ph_num_constraints != num_constraints)
    return (ph_num_constraints < num_constraints) ? 1 : -1;

  // All components are equal.
  return 0;
}

