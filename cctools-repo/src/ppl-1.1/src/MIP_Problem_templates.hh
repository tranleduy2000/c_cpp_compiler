/* MIP_Problem class implementation: non-inline template functions.
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

#ifndef PPL_MIP_Problem_templates_hh
#define PPL_MIP_Problem_templates_hh 1

#include "Variables_Set_defs.hh"

namespace Parma_Polyhedra_Library {

template <typename In>
MIP_Problem::MIP_Problem(const dimension_type dim,
                         In first, In last,
                         const Variables_Set& int_vars,
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
    i_variables(int_vars) {
  // Check that integer Variables_Set does not exceed the space dimension
  // of the problem.
  if (i_variables.space_dimension() > external_space_dim) {
    std::ostringstream s;
    s << "PPL::MIP_Problem::MIP_Problem"
      << "(dim, first, last, int_vars, obj, mode):\n"
      << "dim == "<< external_space_dim << " and int_vars.space_dimension() =="
      << " " << i_variables.space_dimension() << " are dimension"
      "incompatible.";
    throw std::invalid_argument(s.str());
  }

  // Check for space dimension overflow.
  if (dim > max_space_dimension())
    throw std::length_error("PPL::MIP_Problem:: MIP_Problem(dim, first, "
                            "last, int_vars, obj, mode):\n"
                            "dim exceeds the maximum allowed"
                            "space dimension.");
  // Check the objective function.
  if (obj.space_dimension() > dim) {
    std::ostringstream s;
    s << "PPL::MIP_Problem::MIP_Problem(dim, first, last,"
      << "int_vars, obj, mode):\n"
      << "obj.space_dimension() == "<< obj.space_dimension()
      << " exceeds d == "<< dim << ".";
    throw std::invalid_argument(s.str());
  }
  // Check the constraints.
  try {
    for (In i = first; i != last; ++i) {
      if (i->is_strict_inequality())
        throw std::invalid_argument("PPL::MIP_Problem::"
                                    "MIP_Problem(dim, first, last, int_vars,"
                                    "obj, mode):\nrange [first, last) contains"
                                    "a strict inequality constraint.");
      if (i->space_dimension() > dim) {
        std::ostringstream s;
        s << "PPL::MIP_Problem::"
          << "MIP_Problem(dim, first, last, int_vars, obj, mode):\n"
          << "range [first, last) contains a constraint having space"
          << "dimension  == " << i->space_dimension() << " that exceeds"
          "this->space_dimension == " << dim << ".";
        throw std::invalid_argument(s.str());
      }
      add_constraint_helper(*i);
    }
  } catch (...) {
    // Delete the allocated constraints, to avoid memory leaks.

    for (Constraint_Sequence::const_iterator
          i = input_cs.begin(), i_end = input_cs.end(); i != i_end; ++i)
      delete *i;

    throw;
  }
  PPL_ASSERT(OK());
}

template <typename In>
MIP_Problem::MIP_Problem(dimension_type dim,
                         In first, In last,
                         const Linear_Expression& obj,
                         Optimization_Mode mode)
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
    throw std::length_error("PPL::MIP_Problem::"
                            "MIP_Problem(dim, first, last, obj, mode):\n"
                            "dim exceeds the maximum allowed space "
                            "dimension.");
  // Check the objective function.
  if (obj.space_dimension() > dim) {
    std::ostringstream s;
    s << "PPL::MIP_Problem::MIP_Problem(dim, first, last,"
      << " obj, mode):\n"
      << "obj.space_dimension() == "<< obj.space_dimension()
      << " exceeds d == "<< dim << ".";
    throw std::invalid_argument(s.str());
  }
  // Check the constraints.
  try {
    for (In i = first; i != last; ++i) {
      if (i->is_strict_inequality())
        throw std::invalid_argument("PPL::MIP_Problem::"
                                    "MIP_Problem(dim, first, last, obj, mode):"
                                    "\n"
                                    "range [first, last) contains a strict "
                                    "inequality constraint.");
      if (i->space_dimension() > dim) {
        std::ostringstream s;
        s << "PPL::MIP_Problem::"
          << "MIP_Problem(dim, first, last, obj, mode):\n"
          << "range [first, last) contains a constraint having space"
          << "dimension" << " == " << i->space_dimension() << " that exceeds"
          "this->space_dimension == " << dim << ".";
        throw std::invalid_argument(s.str());
      }
      add_constraint_helper(*i);
    }
  } catch (...) {
    // Delete the allocated constraints, to avoid memory leaks.

    for (Constraint_Sequence::const_iterator
          i = input_cs.begin(), i_end = input_cs.end(); i != i_end; ++i)
      delete *i;

    throw;
  }
  PPL_ASSERT(OK());
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_MIP_Problem_templates_hh)
