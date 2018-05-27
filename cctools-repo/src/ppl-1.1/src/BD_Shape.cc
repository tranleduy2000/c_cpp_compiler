/* BD_Shape class implementation: non-inline functions.
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
#include "BD_Shape_defs.hh"

namespace PPL = Parma_Polyhedra_Library;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Parma_Polyhedra_Library::BD_Shape */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool
PPL::BD_Shape_Helpers::extract_bounded_difference(const Constraint& c,
                                                  dimension_type& c_num_vars,
                                                  dimension_type& c_first_var,
                                                  dimension_type& c_second_var,
                                                  Coefficient& c_coeff) {
  // Check for preconditions.
  const dimension_type space_dim = c.space_dimension();
  PPL_ASSERT(c_num_vars == 0 && c_first_var == 0 && c_second_var == 0);

  c_first_var = c.expression().first_nonzero(1, space_dim + 1);
  if (c_first_var == space_dim + 1)
    // All the inhomogeneous coefficients are zero.
    return true;

  ++c_num_vars;

  c_second_var = c.expression().first_nonzero(c_first_var + 1, space_dim + 1);
  if (c_second_var == space_dim + 1) {
    // c_first_var is the only inhomogeneous coefficient different from zero.
    c_coeff = -c.expression().get(Variable(c_first_var - 1));

    c_second_var = 0;
    return true;
  }

  ++c_num_vars;

  if (!c.expression().all_zeroes(c_second_var + 1, space_dim + 1))
    // The constraint `c' is not a bounded difference.
    return false;

  // Make sure that `c' is indeed a bounded difference, i.e., it is of the
  // form:
  // a*x - a*y <=/= b.
  Coefficient_traits::const_reference c0 = c.expression().get(Variable(c_first_var - 1));
  Coefficient_traits::const_reference c1 = c.expression().get(Variable(c_second_var - 1));
  if (sgn(c0) == sgn(c1) || c0 != -c1)
    // Constraint `c' is not a bounded difference.
    return false;

  c_coeff = c1;
  
  return true;
}

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Parma_Polyhedra_Library::BD_Shape */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
void
PPL::compute_leader_indices(const std::vector<dimension_type>& predecessor,
                            std::vector<dimension_type>& indices) {
  // The vector `indices' contains one entry for each equivalence
  // class, storing the index of the corresponding leader in
  // increasing order: it is used to avoid repeated tests for leadership.
  PPL_ASSERT(indices.size() == 0);
  PPL_ASSERT(0 == predecessor[0]);
  indices.push_back(0);
  for (dimension_type i = 1, p_size = predecessor.size(); i != p_size; ++i)
    if (i == predecessor[i])
      indices.push_back(i);
}
