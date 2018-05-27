/* Grid_Generator_System class implementation (non-inline functions).
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
#include "Grid_Generator_System_defs.hh"
#include "Grid_Generator_System_inlines.hh"
#include "Scalar_Products_defs.hh"
#include "Scalar_Products_inlines.hh"
#include "Variables_Set_defs.hh"
#include "assert.hh"
#include <iostream>

namespace PPL = Parma_Polyhedra_Library;

void
PPL::Grid_Generator_System::insert(Grid_Generator_System& gs, Recycle_Input) {
  const dimension_type gs_num_rows = gs.num_rows();

  if (space_dimension() < gs.space_dimension())
    set_space_dimension(gs.space_dimension());
  else
    gs.set_space_dimension(space_dimension());

  for (dimension_type i = 0; i < gs_num_rows; ++i)
    sys.insert(gs.sys.rows[i], Recycle_Input());

  gs.clear();

  unset_pending_rows();
}

void
PPL::Grid_Generator_System::insert(const Grid_Generator& g) {
  Grid_Generator tmp(g, representation());
  insert(tmp, Recycle_Input());
}

void
PPL::Grid_Generator_System::insert(Grid_Generator& g, Recycle_Input) {
  if (g.is_parameter() && g.all_homogeneous_terms_are_zero()) {
    // There is no need to add the origin as a parameter,
    // as it will be immediately flagged as redundant.
    // However, we still have to adjust space dimension.
    if (space_dimension() < g.space_dimension())
      set_space_dimension(g.space_dimension());
    return;
  }

  sys.insert(g, Recycle_Input());

  PPL_ASSERT(OK());
}

void
PPL::Grid_Generator_System
::affine_image(Variable v,
               const Linear_Expression& expr,
               Coefficient_traits::const_reference denominator) {
  // This is mostly a copy of Generator_System::affine_image.

  Grid_Generator_System& x = *this;
  PPL_ASSERT(v.space_dimension() <= x.sys.space_dimension());
  PPL_ASSERT(expr.space_dimension() <= x.sys.space_dimension());
  PPL_ASSERT(denominator > 0);

  const dimension_type num_rows = x.num_rows();

  // Compute the numerator of the affine transformation and assign it
  // to the column of `*this' indexed by `v'.
  PPL_DIRTY_TEMP_COEFFICIENT(numerator);

  for (dimension_type i = num_rows; i-- > 0; ) {
    Grid_Generator& row = sys.rows[i];
    Scalar_Products::assign(numerator, expr, row.expr);
    if (denominator != 1) {
      // Since we want integer elements in the matrix,
      // we multiply by `denominator' all the columns of `*this'
      // having an index different from `v'.
      // Note that this operation also modifies the coefficient of v, but
      // it will be overwritten by the set_coefficient() below.
      row.expr *= denominator;
    }
    row.expr.set_coefficient(v, numerator);
    // Check that the row is stll OK after fiddling with its internal data.
    PPL_ASSERT(row.OK());
  }

  PPL_ASSERT(sys.OK());

  // If the mapping is not invertible we may have transformed valid
  // lines and rays into the origin of the space.
  const bool not_invertible = (v.space_dimension() >= expr.space_dimension()
                               || expr.coefficient(v) == 0);
  if (not_invertible)
    x.remove_invalid_lines_and_parameters();
}

PPL_OUTPUT_DEFINITIONS(Grid_Generator_System)

void
PPL::Grid_Generator_System::ascii_dump(std::ostream& s) const {
  sys.ascii_dump(s);
}

bool
PPL::Grid_Generator_System::ascii_load(std::istream& s) {
  if (!sys.ascii_load(s))
    return false;

  PPL_ASSERT(OK());
  return true;
}

const PPL::Grid_Generator_System*
PPL::Grid_Generator_System::zero_dim_univ_p = 0;

void
PPL::Grid_Generator_System::initialize() {
  PPL_ASSERT(zero_dim_univ_p == 0);
  zero_dim_univ_p
    = new Grid_Generator_System(Grid_Generator::zero_dim_point());
}

void
PPL::Grid_Generator_System::finalize() {
  PPL_ASSERT(zero_dim_univ_p != 0);
  delete zero_dim_univ_p;
  zero_dim_univ_p = 0;
}

bool
PPL::Grid_Generator_System::OK() const {
  if (sys.topology() == NOT_NECESSARILY_CLOSED) {
#ifndef NDEBUG
    std::cerr << "Grid_Generator_System is NOT_NECESSARILY_CLOSED"
              << std::endl;
#endif
    return false;
  }

  if (sys.is_sorted()) {
#ifndef NDEBUG
    std::cerr << "Grid_Generator_System is marked as sorted."
              << std::endl;
#endif
    return false;
  }

  return sys.OK();
}

/*! \relates Parma_Polyhedra_Library::Grid_Generator_System */
std::ostream&
PPL::IO_Operators::operator<<(std::ostream& s,
                              const Grid_Generator_System& gs) {
  Grid_Generator_System::const_iterator i = gs.begin();
  const Grid_Generator_System::const_iterator gs_end = gs.end();
  if (i == gs_end)
    return s << "false";
  while (true) {
    s << *i;
    ++i;
    if (i == gs_end)
      return s;
    s << ", ";
  }
}

void
PPL::Grid_Generator_System
::add_universe_rows_and_columns(dimension_type dims) {
  dimension_type col = sys.space_dimension();

  set_space_dimension(space_dimension() + dims);

  // Add the new rows and set their diagonal element.
  for (dimension_type i = 0; i < dims; ++i) {
    Grid_Generator tmp(space_dimension(), Grid_Generator::LINE_OR_EQUALITY,
                       NECESSARILY_CLOSED, representation());
    tmp.expr += Variable(col);
    PPL_ASSERT(tmp.OK());
    ++col;
    sys.insert(tmp, Recycle_Input());
  }
}

void
PPL::Grid_Generator_System
::remove_space_dimensions(const Variables_Set& vars) {
  sys.remove_space_dimensions(vars);
}

void
PPL::Grid_Generator_System
::shift_space_dimensions(Variable v, dimension_type n) {
  sys.shift_space_dimensions(v, n);
}

void
PPL::Grid_Generator_System
::set_space_dimension(const dimension_type new_dimension) {
  sys.set_space_dimension(new_dimension);
  PPL_ASSERT(OK());
}

void
PPL::Grid_Generator_System::remove_invalid_lines_and_parameters() {
  // The origin of the vector space cannot be a valid line/parameter.
  // NOTE: the following swaps will mix grid generators without even trying
  // to preserve sortedness: as a matter of fact, it will almost always
  // be the case that the input generator system is NOT sorted.
  
  // Note that the num_rows() value is *not* constant because remove_row()
  // decreases it.
  for (dimension_type i = 0; i < num_rows(); ) {
    const Grid_Generator& g = (*this)[i];
    if (g.is_line_or_parameter() && g.all_homogeneous_terms_are_zero())
      sys.remove_row(i, false);
    else
      ++i;
  }
}

bool
PPL::Grid_Generator_System::has_points() const {
  const Grid_Generator_System& ggs = *this;
  for (dimension_type i = num_rows(); i-- > 0; ) {
    if (!ggs[i].is_line_or_parameter())
      return true;
  }
  return false;
}

PPL::dimension_type
PPL::Grid_Generator_System::num_lines() const {
  // We are sure that this method is applied only to a matrix
  // that does not contain pending rows.
  PPL_ASSERT(sys.num_pending_rows() == 0);
  const Grid_Generator_System& ggs = *this;
  dimension_type n = 0;
  // If the Linear_System happens to be sorted, take advantage of the fact
  // that lines are at the top of the system.
  if (sys.is_sorted()) {
    const dimension_type nrows = num_rows();
    for (dimension_type i = 0; i < nrows && ggs[i].is_line(); ++i)
      ++n;
  }
  else {
    for (dimension_type i = num_rows(); i-- > 0 ; )
      if (ggs[i].is_line())
        ++n;
  }
  return n;
}

PPL::dimension_type
PPL::Grid_Generator_System::num_parameters() const {
  // We are sure that this method is applied only to a matrix
  // that does not contain pending rows.
  PPL_ASSERT(sys.num_pending_rows() == 0);
  const Grid_Generator_System& ggs = *this;
  dimension_type n = 0;
  // If the Linear_System happens to be sorted, take advantage of the fact
  // that rays and points are at the bottom of the system and
  // rays have the inhomogeneous term equal to zero.
  if (sys.is_sorted()) {
    for (dimension_type i = num_rows();
         i != 0 && ggs[--i].is_parameter_or_point(); )
      if (ggs[i].is_line_or_parameter())
        ++n;
  }
  else {
    for (dimension_type i = num_rows(); i-- > 0 ; )
      if (ggs[i].is_parameter())
        ++n;
  }
  return n;
}
