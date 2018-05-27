/* NNC_Polyhedron class implementation (non-inline functions).
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
#include "NNC_Polyhedron.defs.hh"
#include "C_Polyhedron.defs.hh"
#include "Grid.defs.hh"
#include "algorithms.hh"

namespace PPL = Parma_Polyhedra_Library;

PPL::NNC_Polyhedron::NNC_Polyhedron(const C_Polyhedron& y, Complexity_Class)
  : Polyhedron(NOT_NECESSARILY_CLOSED, y.space_dimension(), UNIVERSE) {
  add_constraints(y.constraints());
  PPL_ASSERT_HEAVY(OK());
}

PPL::NNC_Polyhedron::NNC_Polyhedron(const Congruence_System& cgs)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_space_dimension_overflow(cgs.space_dimension(),
                                              NOT_NECESSARILY_CLOSED,
                                              "NNC_Polyhedron(cgs)",
                                              "the space dimension of cgs "
                                              "exceeds the maximum allowed "
                                              "space dimension"),
	       UNIVERSE) {
  add_congruences(cgs);
  PPL_ASSERT_HEAVY(OK());
}

PPL::NNC_Polyhedron::NNC_Polyhedron(Congruence_System& cgs, Recycle_Input)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_space_dimension_overflow(cgs.space_dimension(),
                                              NOT_NECESSARILY_CLOSED,
                                              "NNC_Polyhedron(cgs, recycle)",
                                              "the space dimension of cgs "
                                              "exceeds the maximum allowed "
                                              "space dimension"),
	       UNIVERSE) {
  add_congruences(cgs);
  PPL_ASSERT_HEAVY(OK());
}

PPL::NNC_Polyhedron::NNC_Polyhedron(const Grid& grid, Complexity_Class)
  : Polyhedron(NOT_NECESSARILY_CLOSED,
               check_space_dimension_overflow(grid.space_dimension(),
                                              NOT_NECESSARILY_CLOSED,
                                              "NNC_Polyhedron(grid)",
                                              "the space dimension of grid "
                                              "exceeds the maximum allowed "
                                              "space dimension"),
	       UNIVERSE) {
  add_constraints(grid.constraints());
}

bool
PPL::NNC_Polyhedron::poly_hull_assign_if_exact(const NNC_Polyhedron& y) {
#define USE_BHZ09 1
#if USE_BHZ09 // [BagnaraHZ09]
  // Dimension-compatibility check.
  if (space_dimension() != y.space_dimension())
    throw_dimension_incompatible("poly_hull_assign_if_exact(y)", "y", y);
  return BHZ09_poly_hull_assign_if_exact(y);
#else // Old implementation.
  return PPL::poly_hull_assign_if_exact(*this, y);
#endif
#undef USE_BHZ09
}
