/* Box class implementation (non-inline functions).
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
#include "Box_defs.hh"

namespace PPL = Parma_Polyhedra_Library;

bool
PPL::Box_Helpers::extract_interval_constraint(const Constraint& c,
                                              dimension_type& c_num_vars,
                                              dimension_type& c_only_var) {
  // Check for preconditions.
  PPL_ASSERT(c_num_vars == 0 && c_only_var == 0);

  c_only_var = c.expression().first_nonzero(1, c.space_dimension() + 1);
  if (c_only_var == c.space_dimension() + 1)
    // All the inhomogeneous coefficients are zero.
    return true;

  ++c_num_vars;
  --c_only_var;
  return c.expression().all_zeroes(c_only_var + 2, c.space_dimension() + 1);
}

bool
PPL::Box_Helpers::extract_interval_congruence(const Congruence& cg,
                                              dimension_type& cg_num_vars,
                                              dimension_type& cg_only_var) {
  // Check for preconditions.
  PPL_ASSERT(cg_num_vars == 0 && cg_only_var == 0);
  // Only equality congruences can be intervals.
  PPL_ASSERT(cg.is_equality());

  cg_only_var = cg.expression().first_nonzero(1, cg.space_dimension() + 1);
  if (cg_only_var == cg.space_dimension() + 1)
    // All the inhomogeneous coefficients are zero.
    return true;

  ++cg_num_vars;
  --cg_only_var;
  return cg.expression().all_zeroes(cg_only_var + 2, cg.space_dimension() + 1);
}
