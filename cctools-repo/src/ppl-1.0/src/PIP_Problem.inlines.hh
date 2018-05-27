/* PIP_Problem class implementation: inline functions.
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

#ifndef PPL_PIP_Problem_inlines_hh
#define PPL_PIP_Problem_inlines_hh 1

namespace Parma_Polyhedra_Library {

inline dimension_type
PIP_Problem::space_dimension() const {
  return external_space_dim;
}

inline dimension_type
PIP_Problem::max_space_dimension() {
  return Constraint::max_space_dimension();
}

inline PIP_Problem::const_iterator
PIP_Problem::constraints_begin() const {
  return input_cs.begin();
}

inline PIP_Problem::const_iterator
PIP_Problem::constraints_end() const {
  return input_cs.end();
}

inline const Variables_Set&
PIP_Problem::parameter_space_dimensions() const {
  return parameters;
}

inline void
PIP_Problem::m_swap(PIP_Problem& y) {
  using std::swap;
  swap(external_space_dim, y.external_space_dim);
  swap(internal_space_dim, y.internal_space_dim);
  swap(status, y.status);
  swap(current_solution, y.current_solution);
  swap(input_cs, y.input_cs);
  swap(first_pending_constraint, y.first_pending_constraint);
  swap(parameters, y.parameters);
  swap(initial_context, y.initial_context);
  for (dimension_type i = CONTROL_PARAMETER_NAME_SIZE; i-- > 0; )
    swap(control_parameters[i], y.control_parameters[i]);
  swap(big_parameter_dimension, y.big_parameter_dimension);
}

inline PIP_Problem&
PIP_Problem::operator=(const PIP_Problem& y) {
  PIP_Problem tmp(y);
  m_swap(tmp);
  return *this;
}

inline PIP_Problem::Control_Parameter_Value
PIP_Problem::get_control_parameter(Control_Parameter_Name name) const {
  PPL_ASSERT(name >= 0 && name < CONTROL_PARAMETER_NAME_SIZE);
  return control_parameters[name];
}

inline dimension_type
PIP_Problem::get_big_parameter_dimension() const {
  return big_parameter_dimension;
}

/*! \relates PIP_Problem */
inline void
swap(PIP_Problem& x, PIP_Problem& y) {
  x.m_swap(y);
}

} // namespace Parma_Polyhedra_Library

#endif // !defined(PPL_PIP_Problem_inlines_hh)
