/* Octagonal_Shape class implementation (non-inline functions).
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
#include "Octagonal_Shape_defs.hh"

namespace PPL = Parma_Polyhedra_Library;

#ifdef PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS
/*! \relates Parma_Polyhedra_Library::Octagonal_Shape */
#endif // defined(PPL_DOXYGEN_INCLUDE_IMPLEMENTATION_DETAILS)
bool
PPL::Octagonal_Shape_Helper
::extract_octagonal_difference(const Constraint& c,
                               const dimension_type c_space_dim,
                               dimension_type& c_num_vars,
                               dimension_type& c_first_var,
                               dimension_type& c_second_var,
                               Coefficient& c_coeff,
                               Coefficient& c_term) {
  // Check for preconditions.
  PPL_ASSERT(c.space_dimension() == c_space_dim);
  PPL_ASSERT(c_num_vars == 0 && c_first_var == 0 && c_second_var == 0);

  c_first_var = c.expression().first_nonzero(1, c_space_dim + 1);

  if (c_first_var == c_space_dim + 1) {
    c_term = c.inhomogeneous_term();
    return true;
  }

  ++c_num_vars;
  --c_first_var;

  c_second_var = c.expression().first_nonzero(c_first_var + 2, c_space_dim + 1);

  if (c_second_var == c_space_dim + 1) {
    c_term = c.inhomogeneous_term();
    const Coefficient& c0 = c.coefficient(Variable(c_first_var));
    c_term *= 2;
    c_first_var *= 2;
    if (sgn(c0) < 0) {
      c_second_var = c_first_var;
      ++c_first_var;
    }
    else
      c_second_var = c_first_var + 1;
    c_coeff = c0;
    return true;
  }

  ++c_num_vars;
  --c_second_var;

  if (!c.expression().all_zeroes(c_second_var + 2, c_space_dim + 1))
    return false;

  using std::swap;

  // FIXME: The calling code expects c_first_var > c_second_var, when
  // c_num_vars==2, but it shouldn't.
  swap(c_first_var, c_second_var);

  // Make sure that `c' is indeed an octagonal difference,
  // i.e., it is of this form:
  //   (+/-) a*x (+/-) a*y <=/= b.
  c_term = c.inhomogeneous_term();
  const Coefficient& c0 = c.coefficient(Variable(c_first_var));
  const Coefficient& c1 = c.coefficient(Variable(c_second_var));
  if (c0 != c1 && c0 != -c1)
    // Constraint `c' is not an octagonal difference.
    return false;
  
  c_first_var *= 2;
  c_second_var *= 2;
  if (sgn(c0) < 0)
    ++c_first_var;
  if (sgn(c1) > 0)
    ++c_second_var;
  c_coeff = c0;
  
  return true;
}

