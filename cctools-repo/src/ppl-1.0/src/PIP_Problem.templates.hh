/* PIP_Problem class implementation: non-inline template functions.
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

#ifndef PPL_PIP_Problem_templates_hh
#define PPL_PIP_Problem_templates_hh 1

#include "Variables_Set.defs.hh"

namespace Parma_Polyhedra_Library {

template <typename In>
PIP_Problem::PIP_Problem(dimension_type dim,
                         In first, In last,
                         const Variables_Set& p_vars)
  : external_space_dim(dim),
    internal_space_dim(0),
    status(PARTIALLY_SATISFIABLE),
    current_solution(0),
    input_cs(),
    first_pending_constraint(0),
    parameters(p_vars),
    initial_context(),
    big_parameter_dimension(not_a_dimension()) {
  // Check that integer Variables_Set does not exceed the space dimension
  // of the problem.
  if (p_vars.space_dimension() > external_space_dim) {
    std::ostringstream s;
    s << "PPL::PIP_Problem::PIP_Problem(dim, first, last, p_vars):\n"
      << "dim == " << external_space_dim
      << " and p_vars.space_dimension() == "
      << p_vars.space_dimension()
      << " are dimension incompatible.";
    throw std::invalid_argument(s.str());
  }

  // Check for space dimension overflow.
  if (dim > max_space_dimension())
    throw std::length_error("PPL::PIP_Problem::"
                            "PIP_Problem(dim, first, last, p_vars):\n"
                            "dim exceeds the maximum allowed "
                            "space dimension.");
  // Check the constraints.
  for (In i = first; i != last; ++i) {
    if (i->space_dimension() > dim) {
      std::ostringstream s;
      s << "PPL::PIP_Problem::"
        << "PIP_Problem(dim, first, last, p_vars):\n"
        << "range [first, last) contains a constraint having space "
        << "dimension == " << i->space_dimension()
        << " that exceeds this->space_dimension == " << dim << ".";
      throw std::invalid_argument(s.str());
    }
    input_cs.push_back(*i);
  }
  control_parameters_init();
  PPL_ASSERT(OK());
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_PIP_Problem_templates_hh)
